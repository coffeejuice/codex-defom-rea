"""Private CP372 startup/stdio/file discriminator. Public entry is HARD HOLD.

Import is inert. Only _run_once(BoundExports) can create the fixed Python child,
after independent review. No recovered application or runner module is imported.
The first script statement imports ctypes; an event then proves script progress
independently of stdio/files. Absence cannot distinguish startup from event setup
failure. Synchronous APIs can exceed the ceiling; waits/cleanup are bounded.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import re
import secrets
import subprocess
import threading

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp326_native_pipe_security as security
import cp328_native_pipes as pipes
import cp329_native_worker_creator as creator
import cp331_outer_watchdog as outer

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
IMAGE = r"C:\Program Files\Python312\python.exe"
CWD = str(Path(__file__).resolve().parent)
ANALYSIS = Path(CWD).parent / "analysis"
LIMIT = 4096
MARKER = b"CP372-STDOUT-MARKER:" + b"." * 27 + b"\n"
assert len(MARKER) == 48


def _bootstrap(event, path):
    # No project imports or command-line arguments; independent file attempt
    # follows even a failed stdout attempt. A partial write is not success.
    return ("import ctypes as C\n"
            "k=C.WinDLL('kernel32',use_last_error=True,winmode=0x800)\n"
            "s=C.WINFUNCTYPE(C.c_int,C.c_void_p,use_last_error=True)(('SetEvent',k))\n"
            "if not s(" + str(event) + "):raise OSError(C.get_last_error(),'SetEvent')\n"
            "import os,sys,json\n"
            "d={'schema':'cp372-child-v1','event':'signaled','stdout':'unknown'}\n"
            "try:\n"
            " n=sys.stdout.buffer.write(" + repr(MARKER) + ")\n"
            " sys.stdout.buffer.flush()\n"
            " d['stdout']='written-flushed' if n==48 else 'short-write'\n"
            "except BaseException as e:\n"
            " d['stdout']='failed';d['error_type']=type(e).__name__[:80]\n"
            "b=json.dumps(d,separators=(',',':'),sort_keys=True).encode('ascii')\n"
            "with open(" + repr(str(path)) + ",'xb') as f:\n"
            " if f.write(b)!=len(b):raise OSError('short file write')\n"
            " f.flush();os.fsync(f.fileno())\n")


def _file_observation(path):
    """Bounded observation only; absent or malformed is never success."""
    try:
        with path.open("rb") as stream:
            raw = stream.read(LIMIT + 1)
    except FileNotFoundError:
        return {"status": "absent"}
    except OSError as error:
        return {"status": "unreadable", "error": type(error).__name__}
    result = {"status": "malformed", "bytes": len(raw),
              "sha256": hashlib.sha256(raw).hexdigest()}
    if len(raw) > LIMIT:
        result["status"] = "oversized"
        return result
    try:
        data = json.loads(raw.decode("ascii"))
        if (type(data) is dict and data.get("schema") == "cp372-child-v1"
                and data.get("event") == "signaled"
                and data.get("stdout") in ("written-flushed", "short-write", "failed")
                and set(data) <= {"schema", "event", "stdout", "error_type"}
                and ("error_type" not in data or
                     type(data["error_type"]) is str and len(data["error_type"]) <= 80)):
            result.update(status="valid", data=data)
    except (ValueError, UnicodeError):
        pass
    return result


class ProbeFailure(Hold):
    def __init__(self, owner):
        self.owner, self.receipt = owner, owner.result
        super().__init__("CP372 incomplete; retain owner and uncertain outcomes")


class _Probe(outer.OuterWatchdog):
    def __init__(self, exports, *, occurrence_id=None):
        super().__init__(exports, timeout_ms=5000, cleanup_wait_ms=500)
        self.pipe_owner = self.security = None
        self.event = None
        self._cancel = threading.Event()
        self._timer = None
        self._timer_error = None
        self._guard = threading.Lock()
        nonce = secrets.token_hex(16) if occurrence_id is None else occurrence_id
        if type(nonce) is not str or re.fullmatch(r"[0-9a-f]{32}", nonce) is None:
            raise ValueError("fixed 32-digit lowercase occurrence ID required")
        self.child_path = ANALYSIS / ("cp372-child-" + nonce + ".json")
        self.receipt_path = ANALYSIS / ("cp372-receipt-" + nonce + ".json")
        self.result = {"schema": "cp372-parent-v1", "occurrence_id": nonce,
                       "image": IMAGE,
                       "event": "unobserved", "stdout": "unobserved",
                       "exit": "unobserved", "file": {"status": "unobserved"},
                       "cleanup": "unresolved", "probe_passed": False,
                       "errors": []}

    def _ceiling(self):
        if self._cancel.wait(5):
            return
        with self._guard:
            self._timer_error = "five-second ceiling fired"
            job = self._owned_handles.get("outer_job")
            if job is not None:
                try:
                    self._ok("TerminateJobObject", job, 0xE372)
                    self._abort_job_terminated = True
                    self.receipt.job_terminated = True
                except BaseException as error:
                    self._timer_error += ": " + str(error)

    def _budget(self):
        if self._timer_error is not None:
            raise Hold(self._timer_error)
        return super()._budget()

    def _call(self, name, *args):
        # CP331 proves the exact process wait before releasing its handle.
        # Capture a cleanup-induced exit separately from natural observation.
        if (name == "CloseHandle" and self._abort_runner_waited
                and args[0] == self._owned_handles.get("runner_process")
                and "cleanup_exit" not in self.result):
            code = abi.DWORD()
            try:
                ok = super()._call("GetExitCodeProcess", args[0], C.byref(code))
                if not ok:
                    raise creator.ApiFailure("GetExitCodeProcess", int(self.exports.last_error()))
                self.result["cleanup_exit"] = {"code": code.value,
                    "exact_handle_wait": True, "termination_requested": bool(
                        self._abort_process_terminated or self._abort_job_terminated)}
            except BaseException as error:
                self.result["cleanup_exit"] = "query-failed-or-unknown"
                self.result["errors"].append(str(error)[:500])
        return super()._call(name, *args)

    def _attribute_list(self, job, handles):
        size = abi.SIZE_T()
        ok = self._call("InitializeProcThreadAttributeList", None, 2, 0, C.byref(size))
        error = int(self.exports.last_error())
        if ok or error != 122 or not 0 < size.value <= 1048576:
            raise Hold("bounded attribute-list probe required")
        storage = C.create_string_buffer(size.value)
        address = C.cast(storage, abi.PVOID)
        jobs = (abi.HANDLE * 1)(job)
        inherited = (abi.HANDLE * 3)(*handles)
        self._attributes = [address, storage, jobs, inherited, None]
        ok = self._call("InitializeProcThreadAttributeList", address, 2, 0, C.byref(size))
        self._attributes[4] = bool(ok)
        if not ok:
            raise creator.ApiFailure("InitializeProcThreadAttributeList", int(self.exports.last_error()))
        for kind, values in ((creator.JOB_LIST, jobs), (creator.HANDLE_LIST, inherited)):
            self._budget()
            self._ok("UpdateProcThreadAttribute", address, 0, kind,
                     C.cast(values, abi.PVOID), C.sizeof(values), None, None)
        return address

    def _verify(self):
        self._success_custody()
        self._budget()
        r = self.receipt
        process = self._owned_handles.get("runner_process")
        if not r.pid or not r.tid or not process or "runner_thread" not in self._owned_handles:
            raise Hold("complete creation-returned identity required")
        if self._call("GetProcessId", process) != r.pid:
            raise Hold("exact process PID mismatch")
        if any(self._flags(h) & creator.INHERIT for h in self._owned_handles.values()):
            raise Hold("job/process/thread inheritance prohibited")
        path, size = C.create_unicode_buffer(32768), abi.DWORD(32768)
        self._ok("QueryFullProcessImageNameW", process, 0, path, C.byref(size))
        if size.value != len(path.value) or not 0 < size.value < len(path) or path.value.casefold() != IMAGE.casefold():
            raise Hold("fixed Python image mismatch")
        member = abi.BOOL()
        job = self._owned_handles["outer_job"]
        self._ok("IsProcessInJob", process, job, C.byref(member))
        self._job_verified(job)
        if member.value != 1 or self._active(job) != 1:
            raise Hold("sole child in exact kill-on-close job required")
        stamp = self._stamp()
        if r.creation_time is not None and r.creation_time != stamp:
            raise Hold("creation timestamp mismatch")
        r.creation_time = stamp
        self._budget()

    def _prepare(self, endpoints):
        self._used = True
        handles = (endpoints.client_read, endpoints.client_write, self.event)
        if len(set(handles)) != 3 or any(type(h) is not int or not 0 < h < creator.INVALID_HANDLE for h in handles):
            raise Hold("three distinct inherited handles required")
        self._endpoint_handles = set(handles)
        for handle in handles:
            if not self._flags(handle) & creator.INHERIT:
                raise Hold("explicit inheritance required")
        current, member = self._call("GetCurrentProcess"), abi.BOOL()
        self._ok("IsProcessInJob", current, None, C.byref(member))
        if member.value not in (0, 1):
            raise Hold("invalid host job membership")
        self.receipt.host_in_job = bool(member.value)
        with self._guard:
            self._budget()
            job = creator.NativeWorkerCreator._job(self, "outer_job")
        address = self._attribute_list(job, handles)
        startup = abi.STARTUPINFOEXW()
        startup.StartupInfo.cb = C.sizeof(startup)
        startup.StartupInfo.dwFlags = 0x100
        startup.StartupInfo.hStdInput = handles[0]
        startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = handles[1]
        startup.lpAttributeList = address.value
        command = C.create_unicode_buffer(subprocess.list2cmdline(
            [IMAGE, "-I", "-B", "-c", _bootstrap(self.event, self.child_path)]))
        info = abi.PROCESS_INFORMATION()
        self._budget()
        self._create_attempted = True
        self.receipt.create_outcome = "unknown: CreateProcessW entered"
        try:
            ok = self._call("CreateProcessW", IMAGE, command, None, None, True,
                            creator.CREATE_FLAGS, None, CWD,
                            C.byref(startup.StartupInfo), C.byref(info))
            error = None if ok else int(self.exports.last_error())
        finally:
            capture_errors = []
            for name, value in (("runner_process", info.hProcess), ("runner_thread", info.hThread)):
                if value:
                    try:
                        self._own(name, value)
                    except BaseException as exc:
                        capture_errors.append(str(exc))
            self.receipt.pid, self.receipt.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
            if capture_errors:
                raise Hold("; ".join(capture_errors))
        if not ok:
            if not any((info.hProcess, info.hThread, info.dwProcessId, info.dwThreadId)):
                self._no_child_proven = True
                self.receipt.create_outcome = "returned-failure-no-child"
            raise creator.ApiFailure("CreateProcessW", error)
        self.receipt.create_outcome = "returned-success"
        self._verify()
        self._delete_attributes()
        self.receipt.phase = "prepared-suspended"

    def close(self):
        # Stop concurrent job access before CP331 releases any owned handles.
        self._cancel.set()
        if self._timer is not None:
            self._timer.join(1)
            if self._timer.is_alive():
                self.result["errors"].append("timer still owns possible native call")
                return self.receipt
        with self._guard:
            super().close()
        if self.receipt.phase == "closed":
            if self.event is not None:
                try:
                    self._ok("CloseHandle", self.event)
                    self.event = None
                except BaseException as error:
                    self.result["errors"].append(str(error))
            pipe_ok = sec_ok = False
            try:
                pipe_ok = self.pipe_owner is None or self.pipe_owner.cleanup()
            except BaseException as error:
                self.result["errors"].append("pipe cleanup: " + str(error)[:400])
            try:
                sec_ok = self.security is None or self.security.cleanup()
            except BaseException as error:
                self.result["errors"].append("security cleanup: " + str(error)[:400])
            if self.event is None and pipe_ok and sec_ok:
                self.result["cleanup"] = "closed"
        return self.receipt

    def run_once(self):
        if self.receipt.phase != "new":
            raise Hold("one-use occurrence")
        try:
            self.start_occurrence()
            self._timer = threading.Thread(target=self._ceiling, daemon=True,
                                           name="cp372-five-second-ceiling")
            self._timer.start()
            self.security = security.NativePrivatePipeSecurity(self.exports)
            self.security.prepare()
            self._budget()
            self.pipe_owner = pipes.NativePipeOwner(self.exports, self.security, deadline=self.deadline)
            endpoints = self.pipe_owner.prepare(r"\\.\pipe\cp372-" + secrets.token_hex(32))
            sa = self.security.security_attributes(inheritable=True)
            event = self._call("CreateEventW", C.byref(sa), True, False, None)
            event = getattr(event, "value", event)
            if type(event) is not int or not 0 < event < creator.INVALID_HANDLE:
                raise Hold("private manual-reset control event unavailable")
            self.event = event  # retain before validation/next API
            if event in self.pipe_owner.handles or event in self._owned_handles.values():
                self._ownership_uncertain.append(("event-alias", event))
                raise Hold("control event aliases another owner")
            self._prepare(endpoints)
            self.pipe_owner._close(endpoints.client_read)
            self.pipe_owner._close(endpoints.client_write)
            self.resume_once()
            state = self._call("WaitForSingleObject", self.event, self._budget())
            self.result["event"] = ({0: "signaled", 258: "timeout"}.get(state, "wait-failed-or-unknown")
                                    if type(state) is int else "wait-failed-or-unknown")
            if type(state) is int and state == 0:
                try:
                    raw = self.pipe_owner.transfer(endpoints.parent_read, "read", 48)
                    self.result["stdout"] = "exact-marker" if raw == MARKER else "different-48-bytes"
                    self.result["stdout_sha256"] = hashlib.sha256(raw).hexdigest()
                except BaseException as error:
                    self.result["stdout"] = "read-failed-or-partial"
                    self.result["errors"].append(str(error)[:500])
            status = self._call("WaitForSingleObject", self._owned_handles["runner_process"], self._budget())
            if status == 0:
                code = abi.DWORD()
                self._ok("GetExitCodeProcess", self._owned_handles["runner_process"], C.byref(code))
                self.result["exit"] = {"code": code.value, "exact_handle_wait": True}
            else:
                self.result["exit"] = "timeout-or-wait-failure"
        except BaseException as error:
            self.result["errors"].append(str(error)[:500])
        finally:
            self.close()
            self.result["file"] = _file_observation(self.child_path)
            self.result["owner"] = asdict(self.receipt)
            self.result["timer"] = self._timer_error
            self.result["child_path"] = str(self.child_path)
            self.result["receipt_path"] = str(self.receipt_path)
            child = self.result["file"]
            self.result["probe_passed"] = (
                self.result["event"] == "signaled" and
                self.result["stdout"] == "exact-marker" and
                self.result["exit"] == {"code": 0, "exact_handle_wait": True} and
                child.get("status") == "valid" and
                child.get("data", {}).get("stdout") == "written-flushed" and
                self.result["cleanup"] == "closed" and
                self._timer_error is None and not self.result["errors"])
            # New path only; never overwrite an earlier partial receipt.
            raw = json.dumps(self.result, sort_keys=True, separators=(",", ":")).encode("ascii")
            if len(raw) > 16384:
                self.result["probe_passed"] = False
                self.result["receipt_write"] = "oversized"
                raise ProbeFailure(self)
            try:
                import os
                with self.receipt_path.open("xb") as stream:
                    if stream.write(raw) != len(raw):
                        raise OSError("short parent receipt write")
                    stream.flush()
                    os.fsync(stream.fileno())
            except BaseException as error:
                self.result["probe_passed"] = False
                self.result["receipt_write"] = "failed-or-partial: " + type(error).__name__
                raise ProbeFailure(self) from error
        if self.result["cleanup"] != "closed" or self.result["errors"]:
            raise ProbeFailure(self)
        return self.result


def _run_once(exports, *, occurrence_id):
    """PRIVATE: independent source review required before any native invocation."""
    return _Probe(exports, occurrence_id=occurrence_id).run_once()


def launch_probe(*_args, **_kwargs):
    raise Hold("CP372 public launch HARD HOLD; independent review required")


if __name__ == "__main__":
    launch_probe()
