"""Hold ONE just-spawned Windows process handle for bounded cleanup.

Contract: the direct local controller launches this helper immediately after its
own Frida spawn returns a PAUSED PID. The controller supplies an unpredictable
spawn token, exact image/hash and a narrow creation FILETIME window. These are
caller assertions, NOT OS proof of Frida ownership or suspension. No attachment
to arbitrary running/user processes is authorized by this API. The controller
owns resume/detach; this helper never resumes. Rejected adoption never kills.

JSONL stdin: {"command":"status"} or {"command":"terminate"}. EOF, malformed
input and the watchdog clean up the adopted handle. Abrupt helper death is not
covered (this is not a kill-on-close Job Object). stdout is the JSONL receipt;
save it before claiming cleanup. A handle value is diagnostic, not transferable.
"""
from __future__ import annotations

import argparse
import ctypes as C
from ctypes import wintypes as W
import hashlib
import json
import math
import os
from pathlib import Path
import queue
import re
import sys
import threading
import time
import uuid


def kernel():
    if sys.platform != "win32":
        raise RuntimeError("Windows only")
    k = C.WinDLL("kernel32", use_last_error=True)
    specs = {
        "OpenProcess": ([W.DWORD, W.BOOL, W.DWORD], W.HANDLE),
        "GetProcessId": ([W.HANDLE], W.DWORD),
        "GetProcessTimes": ([W.HANDLE] + [C.POINTER(W.FILETIME)] * 4, W.BOOL),
        "GetSystemTimeAsFileTime": ([C.POINTER(W.FILETIME)], None),
        "QueryFullProcessImageNameW": ([W.HANDLE, W.DWORD, W.LPWSTR, C.POINTER(W.DWORD)], W.BOOL),
        "TerminateProcess": ([W.HANDLE, W.UINT], W.BOOL),
        "WaitForSingleObject": ([W.HANDLE, W.DWORD], W.DWORD),
        "GetExitCodeProcess": ([W.HANDLE, C.POINTER(W.DWORD)], W.BOOL),
        "CloseHandle": ([W.HANDLE], W.BOOL),
    }
    for name, (args, result) in specs.items():
        getattr(k, name).argtypes = args
        getattr(k, name).restype = result
    return k


def checked(value):
    if not value:
        raise C.WinError(C.get_last_error())
    return value


def filetime(value):
    return (value.dwHighDateTime << 32) | value.dwLowDateTime


def canonical(path):
    return os.path.normcase(str(Path(path).resolve(strict=True)))


class HeldSpawn:
    def __init__(self, pid, image, sha256, after, before, adopt_ms, token, controller_pid):
        self.k, self.handle = kernel(), None
        self.lock = threading.RLock()
        self.attempts = []
        self.identity = None
        self.closed = False
        if not 0 < adopt_ms <= 3000 or not 0 < after <= before:
            raise ValueError("adoption window must be ordered and at most 3000 ms old")
        if before - after > adopt_ms * 10000:
            raise ValueError("creation window exceeds adoption deadline")
        if pid <= 0 or controller_pid <= 0 or pid in (os.getpid(), controller_pid):
            raise ValueError("invalid child PID")
        if not isinstance(token, str) or len(token) < 32 or len(token) > 256:
            raise ValueError("fresh caller spawn token of 32..256 characters required")
        if not Path(image).is_absolute() or not re.fullmatch(r"[0-9a-fA-F]{64}", sha256):
            raise ValueError("absolute image and exact SHA-256 required")
        image = canonical(image)
        # The process image remains mapped while alive; hash the exact resolved
        # path and verify the handle's image again before accepting adoption.
        with Path(image).open("rb") as source:
            actual = hashlib.file_digest(source, "sha256").hexdigest()
        if actual != sha256.lower():
            raise ValueError("expected image SHA-256 mismatch")
        h = checked(self.k.OpenProcess(0x00100000 | 0x1000 | 0x1, False, pid))
        try:
            if self.k.WaitForSingleObject(h, 0) != 258:
                raise ValueError("child already exited or wait failed")
            times = [W.FILETIME() for _ in range(4)]
            checked(self.k.GetProcessTimes(h, *[C.byref(t) for t in times]))
            created = filetime(times[0])
            now = W.FILETIME()
            self.k.GetSystemTimeAsFileTime(C.byref(now))
            if not after <= created <= before or not 0 <= filetime(now) - created <= adopt_ms * 10000:
                raise ValueError("creation FILETIME outside short adoption deadline")
            buf, size = C.create_unicode_buffer(32768), W.DWORD(32768)
            checked(self.k.QueryFullProcessImageNameW(h, 0, buf, C.byref(size)))
            if canonical(buf.value) != image or checked(self.k.GetProcessId(h)) != pid:
                raise ValueError("held process identity mismatch")
            if self.k.WaitForSingleObject(h, 0) != 258:
                raise ValueError("child exited during adoption")
            self.identity = dict(pid=pid, creation_filetime=created, canonical_image=image,
                                 sha256=actual, controller_pid=controller_pid,
                                 guardian_pid=os.getpid(), spawn_token=token,
                                 ownership_basis="caller_asserted_direct_frida_paused_spawn",
                                 pause_verified=False, receipt_id=uuid.uuid4().hex,
                                 adoption_age_ms=(filetime(now) - created) / 10000,
                                 retained_handle_value=int(h))
            self.handle = h
        except BaseException:
            checked(self.k.CloseHandle(h))
            raise

    def status(self):
        with self.lock:
            if not self.handle:
                raise RuntimeError("handle is closed")
            wait = self.k.WaitForSingleObject(self.handle, 0)
            if wait not in (0, 258):
                raise RuntimeError(f"held-handle wait failed: {wait}")
            code = W.DWORD()
            checked(self.k.GetExitCodeProcess(self.handle, C.byref(code)))
            return dict(state="exited" if wait == 0 else "alive_pause_unverified",
                        wait_result=wait, exit_code=code.value, exit_verified=wait == 0)

    def terminate(self, source):
        with self.lock:
            before = self.status()
            if not before["exit_verified"]:
                attempt = dict(source=source, requested_exit_code=0xe202)
                self.attempts.append(attempt)
                ok = bool(self.k.TerminateProcess(self.handle, 0xe202))
                error = C.get_last_error() if not ok else None
                attempt.update(api_succeeded=ok, winerror=error)
                if not ok and not self.status()["exit_verified"]:
                    raise C.WinError(error)
            wait = self.k.WaitForSingleObject(self.handle, 5000)
            if self.attempts:
                self.attempts[-1]["wait_result"] = wait
            if wait != 0:
                raise RuntimeError(f"cleanup unverified, held-handle wait={wait}")
            return dict(**self.status(), termination_attempts=[dict(x) for x in self.attempts])

    def close(self):
        with self.lock:
            if not self.handle:
                raise RuntimeError("handle already closed")
            result = self.terminate("final_cleanup")
            checked(self.k.CloseHandle(self.handle))
            self.handle = None
            self.closed = True
            return dict(**result, handle_closed=True)


def emit(event, child=None, **fields):
    print(json.dumps(dict(event=event, identity=child.identity if child else None, **fields)), flush=True)


def read_commands(inbox):
    try:
        while True:
            line = sys.stdin.readline(4097)
            if not line:
                inbox.put(None)
                return
            if len(line) > 4096 or not line.endswith("\n"):
                raise ValueError("command must be a JSON line <=4096 characters")
            inbox.put(line)
    except BaseException as exc:
        inbox.put(exc)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--pid", type=int, required=True)
    p.add_argument("--expected-image", required=True)
    p.add_argument("--expected-sha256", required=True)
    p.add_argument("--spawn-token", required=True)
    p.add_argument("--controller-pid", type=int, required=True)
    p.add_argument("--created-after-filetime", type=int, required=True)
    p.add_argument("--created-before-filetime", type=int, required=True)
    p.add_argument("--adopt-within-ms", type=int, default=3000)
    p.add_argument("--max-seconds", type=float, default=20)
    ns = p.parse_args()
    if not math.isfinite(ns.max_seconds) or not 0 < ns.max_seconds <= 20:
        p.error("max-seconds must be finite and in (0,20]")
    child, timer, failed = None, None, False
    stop = threading.Event()
    watchdog_errors = []
    deadline = time.monotonic() + ns.max_seconds
    try:
        if ns.controller_pid != os.getppid():
            raise ValueError("controller must be helper's direct local parent")
        child = HeldSpawn(ns.pid, ns.expected_image, ns.expected_sha256,
                          ns.created_after_filetime, ns.created_before_filetime,
                          ns.adopt_within_ms, ns.spawn_token, ns.controller_pid)
        def watchdog():
            if not stop.wait(max(0, deadline - time.monotonic())):
                try:
                    child.terminate("watchdog_deadline")
                except BaseException as exc:
                    watchdog_errors.append(str(exc))
        timer = threading.Thread(target=watchdog, daemon=True)
        timer.start()
        emit("ready", child, **child.status(), max_seconds=ns.max_seconds)
        inbox = queue.Queue(maxsize=64)
        threading.Thread(target=read_commands, args=(inbox,), daemon=True).start()
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                emit("deadline", child, **child.terminate("deadline"))
                break
            try:
                line = inbox.get(timeout=remaining)
            except queue.Empty:
                continue
            if line is None:
                emit("eof", child, **child.terminate("stdin_eof"))
                break
            if isinstance(line, BaseException):
                raise line
            command = json.loads(line)
            if command == {"command": "status"}:
                emit("status", child, **child.status())
            elif command == {"command": "terminate"}:
                emit("terminated", child, **child.terminate("controller_command"))
                break
            else:
                raise ValueError("only exact status/terminate commands accepted")
    except BaseException as exc:
        failed = True
        emit("error", child, error=str(exc), adopted=child is not None)
    finally:
        stop.set()
        if timer:
            timer.join(6)
        if child:
            try:
                result = child.close()
                emit("closed", child, **result, watchdog_errors=watchdog_errors)
            except BaseException as exc:
                failed = True
                emit("cleanup_failed", child, error=str(exc), handle_closed=child.closed)
        if watchdog_errors:
            failed = True
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
