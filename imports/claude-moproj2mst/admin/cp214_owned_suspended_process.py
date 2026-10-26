"""Own one Windows child; JSONL stdin commands and stdout receipts.

Example: python admin/owned_suspended_process.py --exe ABS --cwd ABS
  --sha256 DIGEST --max-seconds 20 -- arg1 arg2
Never attaches to an existing PID. EOF, malformed input, errors and deadline
terminate the held process. Abrupt launcher death is outside this guarantee.
"""
from __future__ import annotations

import argparse
import ctypes as C
from ctypes import wintypes as W
import functools
import hashlib
import json
import os
from pathlib import Path
import queue
import socket
import subprocess
import sys
import threading
import time


class STARTUPINFO(C.Structure):
    _fields_ = [("cb", W.DWORD), ("lpReserved", W.LPWSTR),
                ("lpDesktop", W.LPWSTR), ("lpTitle", W.LPWSTR),
                ("dwX", W.DWORD), ("dwY", W.DWORD), ("dwXSize", W.DWORD),
                ("dwYSize", W.DWORD), ("dwXCountChars", W.DWORD),
                ("dwYCountChars", W.DWORD), ("dwFillAttribute", W.DWORD),
                ("dwFlags", W.DWORD), ("wShowWindow", W.WORD),
                ("cbReserved2", W.WORD), ("lpReserved2", C.POINTER(W.BYTE)),
                ("hStdInput", W.HANDLE), ("hStdOutput", W.HANDLE),
                ("hStdError", W.HANDLE)]


class PROCESS_INFORMATION(C.Structure):
    _fields_ = [("hProcess", W.HANDLE), ("hThread", W.HANDLE),
                ("dwProcessId", W.DWORD), ("dwThreadId", W.DWORD)]


def kernel():
    if sys.platform != "win32":
        raise RuntimeError("Windows only")
    k = C.WinDLL("kernel32", use_last_error=True)
    specs = {
        "CreateProcessW": ([W.LPCWSTR, W.LPWSTR, W.LPVOID, W.LPVOID, W.BOOL,
                            W.DWORD, W.LPVOID, W.LPCWSTR,
                            C.POINTER(STARTUPINFO), C.POINTER(PROCESS_INFORMATION)], W.BOOL),
        "GetProcessTimes": ([W.HANDLE] + [C.POINTER(W.FILETIME)] * 4, W.BOOL),
        "QueryFullProcessImageNameW": ([W.HANDLE, W.DWORD, W.LPWSTR, C.POINTER(W.DWORD)], W.BOOL),
        "ResumeThread": ([W.HANDLE], W.DWORD),
        "TerminateProcess": ([W.HANDLE, W.UINT], W.BOOL),
        "WaitForSingleObject": ([W.HANDLE, W.DWORD], W.DWORD),
        "GetExitCodeProcess": ([W.HANDLE, C.POINTER(W.DWORD)], W.BOOL),
        "CloseHandle": ([W.HANDLE], W.BOOL),
    }
    for name, (args, result) in specs.items():
        f = getattr(k, name)
        f.argtypes, f.restype = args, result
    return k


def checked(value):
    if not value:
        raise C.WinError(C.get_last_error())
    return value


def canonical(path):
    return os.path.normcase(str(Path(path).resolve(strict=True)))


def locked(method):
    @functools.wraps(method)
    def call(self, *args, **kwargs):
        with self.lock:
            return method(self, *args, **kwargs)
    return call


class OwnedProcess:
    def __init__(self, exe, cwd, args, digest):
        self.k = kernel()
        self.lock = threading.RLock()
        self.pi = PROCESS_INFORMATION()
        self.resumed = False
        self.termination_attempts = []
        self.identity = None
        if not Path(exe).is_absolute() or not Path(cwd).is_absolute():
            raise ValueError("exe and cwd must be absolute")
        self.exe, self.cwd = canonical(exe), canonical(cwd)
        if not Path(self.exe).is_file() or not Path(self.cwd).is_dir():
            raise ValueError("exe must be a file and cwd a directory")
        actual = hashlib.sha256(Path(self.exe).read_bytes()).hexdigest()
        if actual != digest.lower() or len(digest) != 64:
            raise ValueError("executable SHA-256 mismatch")
        if any("\0" in arg for arg in args):
            raise ValueError("NUL in argument")
        si = STARTUPINFO(cb=C.sizeof(STARTUPINFO))
        command = C.create_unicode_buffer(subprocess.list2cmdline([self.exe, *args]))
        checked(self.k.CreateProcessW(self.exe, command, None, None, False,
                                      0x4 | 0x08000000, None, self.cwd,
                                      C.byref(si), C.byref(self.pi)))
        try:
            times = [W.FILETIME() for _ in range(4)]
            checked(self.k.GetProcessTimes(self.pi.hProcess, *[C.byref(t) for t in times]))
            buf, size = C.create_unicode_buffer(32768), W.DWORD(32768)
            checked(self.k.QueryFullProcessImageNameW(self.pi.hProcess, 0, buf, C.byref(size)))
            image = canonical(buf.value)
            if image != self.exe:
                raise RuntimeError("held process image mismatch")
            self.identity = {"pid": self.pi.dwProcessId, "primary_tid": self.pi.dwThreadId,
                             "creation_filetime": (times[0].dwHighDateTime << 32) | times[0].dwLowDateTime,
                             "canonical_image": image, "sha256": actual,
                             "cwd": self.cwd, "argv": [self.exe, *args],
                             "host": socket.gethostname(), "launcher_pid": os.getpid()}
        except BaseException:
            self.close()
            raise

    @locked
    def status(self):
        result = self.k.WaitForSingleObject(self.pi.hProcess, 0)
        if result not in (0, 258):
            raise C.WinError(C.get_last_error())
        code = W.DWORD()
        checked(self.k.GetExitCodeProcess(self.pi.hProcess, C.byref(code)))
        return {"state": "exited" if result == 0 else ("running" if self.resumed else "suspended"),
                "wait_result": result, "exit_code": code.value,
                "exit_verified": result == 0}

    @locked
    def resume(self):
        if self.resumed or self.status()["exit_verified"]:
            raise RuntimeError("resume is allowed exactly once on the live owned primary thread")
        previous = self.k.ResumeThread(self.pi.hThread)
        if previous == 0xffffffff:
            raise C.WinError(C.get_last_error())
        self.resumed = True
        if previous != 1:
            raise RuntimeError(f"unexpected primary-thread suspend count {previous}")
        return {**self.status(), "previous_suspend_count": previous}

    @locked
    def terminate(self, source="final_cleanup"):
        before = self.status()
        requested = not before["exit_verified"]
        if requested:
            attempt = {"source": source, "requested_exit_code": 0xe201}
            self.termination_attempts.append(attempt)
            succeeded = bool(self.k.TerminateProcess(self.pi.hProcess, 0xe201))
            error = C.get_last_error() if not succeeded else None
            attempt.update(api_succeeded=succeeded, winerror=error)
            # TerminateProcess can fail while a natural exit is completing.
            # Give the exact retained process handle a bounded chance to signal;
            # PID enumeration is not evidence for this ownership receipt.
            if not succeeded:
                race_wait = self.k.WaitForSingleObject(self.pi.hProcess, 5000)
                attempt["post_failure_wait_result"] = race_wait
                if race_wait != 0:
                    raise C.WinError(error)
                attempt["exit_during_termination_race"] = True
        wait = self.k.WaitForSingleObject(self.pi.hProcess, 5000)
        if requested:
            attempt["wait_result"] = wait
        if wait != 0:
            raise RuntimeError(f"owned process termination not verified; wait={wait}")
        result = self.status()
        if requested:
            attempt.update(exit_code=result["exit_code"], exit_verified=result["exit_verified"])
        return {**result, "termination_requested": bool(self.termination_attempts),
                "termination_attempts": [dict(item) for item in self.termination_attempts]}

    @locked
    def close(self):
        try:
            return self.terminate() if self.pi.hProcess else None
        finally:
            errors = []
            for field in ("hThread", "hProcess"):
                handle = getattr(self.pi, field)
                if handle:
                    if not self.k.CloseHandle(handle):
                        errors.append(str(C.WinError(C.get_last_error())))
                    setattr(self.pi, field, None)
            if errors:
                raise RuntimeError("; ".join(errors))


def emit(event, **fields):
    print(json.dumps({"event": event, **fields}, sort_keys=True), flush=True)


def read_commands(inbox):
    try:
        while True:
            line = sys.stdin.readline(4097)
            if not line:
                inbox.put(None)
                return
            if len(line) > 4096 or not line.endswith("\n"):
                inbox.put(ValueError("command must be a JSON line of at most 4096 characters"))
                return
            inbox.put(line)
    except BaseException as exc:
        inbox.put(exc)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", required=True)
    parser.add_argument("--cwd", required=True)
    parser.add_argument("--sha256", required=True)
    parser.add_argument("--max-seconds", required=True, type=float)
    parser.add_argument("args", nargs=argparse.REMAINDER)
    ns = parser.parse_args()
    if not 0 < ns.max_seconds <= 30:
        parser.error("max-seconds must be in (0, 30]")
    child, failed = None, False
    stop_watchdog = threading.Event()
    watchdog_errors = []
    deadline = time.monotonic() + ns.max_seconds
    try:
        child = OwnedProcess(ns.exe, ns.cwd, ns.args[1:] if ns.args[:1] == ["--"] else ns.args, ns.sha256)
        def watchdog():
            if not stop_watchdog.wait(max(0, deadline - time.monotonic())):
                try:
                    with child.lock:
                        if child.pi.hProcess:
                            child.terminate(source="watchdog_deadline")
                except BaseException as exc:
                    watchdog_errors.append(repr(exc))
        watcher = threading.Thread(target=watchdog, daemon=True)
        watcher.start()
        emit("created", identity=child.identity, **child.status(), gui_control_acquired=False)
        inbox = queue.Queue(maxsize=64)
        threading.Thread(target=read_commands, args=(inbox,), daemon=True).start()
        while True:
            if time.monotonic() >= deadline:
                emit("endpoint", reason="deadline")
                break
            if child.status()["exit_verified"]:
                emit("endpoint", reason="target_exited")
                break
            try:
                line = inbox.get(timeout=min(.1, max(.001, deadline - time.monotonic())))
            except queue.Empty:
                continue
            if line is None:
                emit("endpoint", reason="eof")
                break
            if isinstance(line, BaseException):
                raise line
            command = json.loads(line)
            if not isinstance(command, dict) or set(command) != {"command"}:
                raise ValueError("expected exactly {command: status|resume|terminate}")
            action = command["command"]
            if action == "status":
                emit("status", **child.status())
            elif action == "resume":
                emit("resumed", **child.resume())
            elif action == "terminate":
                emit("endpoint", reason="terminate_command")
                break
            else:
                raise ValueError("unsupported command")
    except BaseException as exc:
        failed = True
        try:
            emit("error", error=repr(exc))
        except (OSError, ValueError):
            pass
    finally:
        if child:
            try:
                receipt = child.close()
                stop_watchdog.set()
                watcher.join(timeout=6)
                if watchdog_errors:
                    raise RuntimeError("watchdog cleanup error: " + "; ".join(watchdog_errors))
                emit("cleanup", **receipt, handles_closed=True)
            except BaseException as exc:
                failed = True
                try:
                    emit("cleanup_error", error=repr(exc), exit_verified=False)
                except (OSError, ValueError):
                    pass
        stop_watchdog.set()
    return int(failed)


if __name__ == "__main__":
    raise SystemExit(main())
