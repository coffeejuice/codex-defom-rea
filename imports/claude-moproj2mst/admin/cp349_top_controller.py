"""Independent fixed top job for one disposable CP347 watchdog occurrence.

This is native-capable only when explicitly supplied CP325 BoundExports. Public
launch is HARD HOLD. The controller itself never opens an original executable,
selects a project, or controls a GUI. A timer thread can request termination
while the launch thread is inside a synchronous Win32 call. That is a practical
independent ceiling, not a hard real-time or crash-atomic cleanup guarantee.
"""
from __future__ import annotations

import ctypes as C
import hashlib
import json
import secrets
import subprocess
import threading
from pathlib import Path

import cp321_native_exports as abi
import cp325_real_bindings as binding
import cp326_native_pipe_security as security
import cp328_native_pipes as pipes
import cp329_native_worker_creator as creator
import cp331_outer_watchdog as outer
import cp334_runner_client_pipe as client
import cp347_fixed_parent_entry as parent

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
SAFETY_MS = 10000
FIXED_MODULE = "cp347_fixed_parent_entry"
CHANNEL_MS = 5000
DIAGNOSTIC_DIR = Path(__file__).resolve().parent.parent / "analysis"
DIAGNOSTIC_PREFIX = "cp349-child-failure-"
DIAGNOSTIC_MAX_BYTES = 4096
TRACE_PREFIX = "cp349-child-trace-"
TRACE_MAX_BYTES = 2048


def _child_diagnostic_path():
    return DIAGNOSTIC_DIR / (DIAGNOSTIC_PREFIX + secrets.token_hex(16) + ".json")


def _child_trace_path():
    return DIAGNOSTIC_DIR / (TRACE_PREFIX + secrets.token_hex(16) + ".jsonl")


def _child_bootstrap(path, trace_path=None):
    """Return fixed child code; only the fixed CP347 entry is importable."""
    trace_path = trace_path or _child_trace_path()
    return ("import json,os,sys,traceback\n"
            "sys.path.insert(0," + repr(outer.RUNNER_CWD) + ")\n"
            "p=" + repr(str(path)) + "\n"
            "q=" + repr(str(trace_path)) + "\n"
            "_trace_ok=False\n"
            "def _stage(s):\n"
            " if not _trace_ok:return\n"
            " try:\n"
            "  b=(json.dumps({'stage':s},separators=(',',':'))+'\\n').encode('ascii')\n"
            "  if os.path.getsize(q)+len(b)>" + str(TRACE_MAX_BYTES) + ":return\n"
            "  with open(q,'ab') as f:f.write(b);f.flush();os.fsync(f.fileno())\n"
            " except BaseException:pass\n"
            "try:\n"
            " with open(q,'xb') as f:pass\n"
            " _trace_ok=True\n"
            "except BaseException:\n"
            " _trace_ok=False\n"
            "_stage('script-entry')\n"
            "try:\n"
            " try:\n"
            "  import " + FIXED_MODULE + " as e\n"
            " except BaseException:\n"
            "  _stage('cp347-module-import-exception');raise\n"
            " _stage('cp347-module-imported')\n"
            " try:\n"
            "  x=e.FixedParentEntry.from_client_argv(sys.argv[1:],stage=_stage)\n"
            " except BaseException:\n"
            "  _stage('from-client-argv-exception');raise\n"
            " _stage('from-client-argv-constructed')\n"
            " try:\n"
            "  x.run()\n"
            " except BaseException:\n"
            "  _stage('run-exception');raise\n"
            " _stage('run-returned')\n"
            "except BaseException as exc:\n"
            " try:\n"
            "  try:\n"
            "   tb=traceback.extract_tb(exc.__traceback__)[-4:]\n"
            "   d={'stage':'fixed-parent-import-or-run','type':type(exc).__name__[:120],"
            "'message':str(exc)[:1000],"
            "'traceback':[{'file':x.filename[-300:],'line':x.lineno,"
            "'name':x.name[:120]} for x in tb]}\n"
            "   b=json.dumps(d,ensure_ascii=True,separators=(',',':')).encode('ascii')[:"
            + str(DIAGNOSTIC_MAX_BYTES) + "]\n"
            "   with open(p,'xb') as f:f.write(b);f.flush();os.fsync(f.fileno())\n"
            "  except FileExistsError:\n"
            "   _stage('diagnostic-write-collision')\n"
            "  except BaseException:\n"
            "   _stage('diagnostic-write-failed')\n"
            "  else:\n"
            "   _stage('diagnostic-write-succeeded')\n"
            " except BaseException: pass\n"
            " raise\n")


def _read_child_diagnostic(path):
    """Read only a small, schema-checked diagnostic from our nonce path."""
    if path is None:
        return None
    try:
        path = Path(path)
        if (path.parent.resolve() != DIAGNOSTIC_DIR.resolve() or
                not path.name.startswith(DIAGNOSTIC_PREFIX) or
                not path.name.endswith(".json")):
            return None
        with path.open("rb") as stream:
            raw = stream.read(DIAGNOSTIC_MAX_BYTES + 1)
        if len(raw) > DIAGNOSTIC_MAX_BYTES:
            return None
        value = json.loads(raw.decode("ascii"))
        if (type(value) is not dict or type(value.get("type")) is not str or
                type(value.get("message")) is not str or
                type(value.get("stage")) is not str or
                type(value.get("traceback")) is not list or
                len(value["traceback"]) > 4 or
                any(type(row) is not dict or type(row.get("file")) is not str or
                    type(row.get("line")) is not int or type(row.get("name")) is not str
                    for row in value["traceback"])):
            return None
        return {"status": "valid", "bytes": len(raw),
                "sha256": hashlib.sha256(raw).hexdigest(), "data": value}
    except (OSError, UnicodeError, ValueError, TypeError):
        return None


def _child_diagnostic_result(path):
    result = _read_child_diagnostic(path)
    if result is not None:
        return result
    try:
        exists = Path(path).exists()
    except (OSError, TypeError):
        exists = False
    return {"status": "malformed" if exists else "absent", "bytes": 0,
            "sha256": None, "data": None}


def _child_trace_result(path):
    """Return a bounded, parsed trace from the occurrence-specific path."""
    try:
        path = Path(path)
        if (path.parent.resolve() != DIAGNOSTIC_DIR.resolve() or
                not path.name.startswith(TRACE_PREFIX) or
                not path.name.endswith(".jsonl")):
            raise ValueError("trace path outside expected namespace")
        with path.open("rb") as stream:
            raw = stream.read(TRACE_MAX_BYTES + 1)
    except FileNotFoundError:
        return {"status": "absent", "bytes": 0, "sha256": None, "stages": []}
    except (OSError, UnicodeError, ValueError, TypeError):
        return {"status": "malformed", "bytes": 0, "sha256": None, "stages": []}
    result = {"status": "valid", "bytes": len(raw),
              "sha256": hashlib.sha256(raw).hexdigest(), "stages": []}
    if len(raw) > TRACE_MAX_BYTES:
        result["status"] = "malformed"
        return result
    allowed = {"script-entry", "cp347-module-import-exception",
               "cp347-module-imported", "from-client-argv-exception",
               "from-client-argv-constructed", "run-exception", "run-returned",
               "diagnostic-write-collision", "diagnostic-write-failed",
               "diagnostic-write-succeeded", "entry",
               "before_load_native_exports", "after_load_native_exports",
               "before_cp334_from_bootstrap_argv", "after_cp334_adopt",
               "before_cp333_acknowledge", "after_cp333_acknowledge",
               "before_cp333_complete", "after_cp333_complete"}
    allowed.update({"cp334_argv_parsed", "cp334_owner_created",
                    "cp334_adopt_enter", "cp334_before_final_tick",
                    "cp334_after_final_tick"})
    allowed.update(f"cp334_h{index}_{suffix}" for index in (0, 1)
                   for suffix in ("before_tick1", "after_tick1",
                                  "before_inherit", "after_inherit",
                                  "before_tick2", "after_tick2",
                                  "before_pipeinfo", "after_pipeinfo"))
    if not raw:
        result["status"] = "partial"
        return result
    for line in raw.splitlines(keepends=True):
        if not line.endswith(b"\n"):
            result["status"] = "partial"
            break
        try:
            row = json.loads(line[:-1].decode("ascii"))
        except (ValueError, UnicodeError):
            result["status"] = "malformed"
            break
        if (type(row) is not dict or set(row) != {"stage"} or
                type(row["stage"]) is not str or row["stage"] not in allowed or
                len(result["stages"]) >= 40):
            result["status"] = "malformed"
            break
        result["stages"].append(row["stage"])
    return result


class TopFailure(Hold):
    def __init__(self, error, owner):
        self.owner, self.receipt = owner, owner.receipt
        super().__init__(str(error))


class TopController(outer.OuterWatchdog):
    """One-use controller; retain after any uncertain API or timeout outcome."""

    def __init__(self, exports):
        if not isinstance(exports, binding.BoundExports):
            raise TypeError("CP325 BoundExports required")
        super().__init__(exports, timeout_ms=5000, cleanup_wait_ms=500)
        self.timeout_ms = SAFETY_MS
        self.security = self.pipe_owner = None
        self.claim = None
        self._timer_cancel = threading.Event()
        self._timer_thread = None
        self._timer_guard = threading.Lock()
        self._timer_fired = False
        self._timer_error = None
        self._accepted = False
        self._channel_deadline = None
        self._channel_receipt = None
        self._channel_values = None
        self._channel_expected = frozenset()
        self._channel_cleanup_proven = False
        self._channel_custody_uncertain = False
        self.child_diagnostic_path = None
        self.child_trace_path = None
        self.child_diagnostic_result = None
        self.child_trace_result = None
        self._fixed_child_argv = None
        self._fixed_child_command_line = None
        self._claim_read_failed = False
        self.claim_process_exit_observation = None

    def _observe_claim_process_before_cleanup(self):
        """Failure-only, zero-wait observation on the retained exact process handle."""
        result = {"status": "unknown-before-cleanup", "exit_code": None,
                 "error": None}
        try:
            process = self._owned_handles.get("runner_process")
            if process is None:
                raise Hold("retained runner process handle unavailable")
            status = self._call("WaitForSingleObject", process, 0)
            if type(status) is not int:
                raise Hold("zero-wait returned an unknown status")
            if status == 258:  # WAIT_TIMEOUT; do not interpret STILL_ACTIVE (259) as exit.
                result["status"] = "active-before-cleanup"
            elif status == 0:  # WAIT_OBJECT_0
                code = abi.DWORD()
                self._ok("GetExitCodeProcess", process, C.byref(code))
                result.update(status="exited-before-cleanup",
                              exit_code=int(code.value) & 0xFFFFFFFF)
            else:
                raise Hold("zero-wait returned an unrecognized status")
        except BaseException as error:
            result["error"] = f"{type(error).__name__}: {str(error)[:300]}"
        self.claim_process_exit_observation = result

    def _budget(self):
        if self._timer_fired or self._timer_error is not None:
            raise Hold("independent top timer expired or termination uncertain")
        return super()._budget()

    def _timer_body(self):
        if self._timer_cancel.wait(SAFETY_MS / 1000):
            return
        with self._timer_guard:
            if self._timer_cancel.is_set():
                return
            self._timer_fired = True
            job = self._owned_handles.get("outer_job")
        if job is not None:
            try:
                if not self._call("TerminateJobObject", job, 0xE349):
                    raise creator.ApiFailure("TerminateJobObject",
                                             int(self.exports.last_error()))
                self._abort_job_terminated = True
                self.receipt.job_terminated = True
            except BaseException as error:
                self._timer_error = error

    def _start_timer(self):
        if self._timer_thread is not None:
            raise Hold("top timer is one-use")
        self._timer_thread = threading.Thread(
            target=self._timer_body, name="cp349-top-ceiling", daemon=True)
        self._timer_thread.start()

    def _stop_timer(self):
        self._timer_cancel.set()
        if self._timer_thread is not None:
            self._timer_thread.join(1)
            if self._timer_thread.is_alive():
                raise Hold("top timer still owns a possible job termination call")

    def _acceptance_custody(self, frozen, channel_frozen, expected_handles):
        claim, deadline, nonce, pipe_owner, descriptor, receipt, exports, channel_deadline = frozen
        if (self.claim is not claim or type(self.claim) is not parent.ParentSuccess or
                type(self.claim.deadline) is not int or self.claim.deadline != deadline or
                type(self.claim.nonce) is not bytes or self.claim.nonce != nonce or
                self.pipe_owner is not pipe_owner or self.security is not descriptor or
                self.receipt is not receipt or self.exports is not exports or
                type(self._channel_deadline) is not int or
                self._channel_deadline != channel_deadline):
            raise Hold("original parent claim or top owner custody changed")
        self._channel_custody(channel_frozen, expected_handles)
        self._success_custody()

    def _channel_custody(self, frozen, expected_handles):
        original_pipe, channel_receipt, channel_values = frozen
        if self.pipe_owner is None or self._channel_receipt is None:
            raise Hold("fixed private claim channel required")
        owner = self.pipe_owner
        if (owner is not original_pipe or
                self._channel_receipt is not channel_receipt or
                type(self._channel_values) is not tuple or
                self._channel_values != channel_values or
                type(self._channel_expected) is not frozenset or
                self._channel_expected != expected_handles):
            self._channel_custody_uncertain = True
            raise Hold("private claim channel ledger changed")
        if self._channel_cleanup_proven:
            if (owner.handles != set() or owner.operations != [] or
                    owner.receipt is not None):
                self._channel_custody_uncertain = True
                raise Hold("terminal claim channel cleanup custody changed")
            return
        names = ("parent_write", "client_read", "parent_read", "client_write")
        values = tuple(getattr(channel_receipt, name, None) for name in names)
        if (owner.receipt is not channel_receipt or
                any(type(value) is not int for value in values) or
                values != channel_values or
                owner.handles != expected_handles or owner.operations != []):
            self._channel_custody_uncertain = True
            raise Hold("original claim channel endpoint/handle custody changed")

    def prepare_fixed_suspended(self, *, child_read, child_write):
        """Create only the fixed CP347 child, suspended in the top job."""
        if self._used:
            raise Hold("top controller is one-use")
        self._used = True
        r = self.receipt
        try:
            handles = (child_read, child_write)
            if (any(type(h) is not int or not 0 < h < creator.INVALID_HANDLE
                    for h in handles) or child_read == child_write):
                raise Hold("two distinct private child endpoints required")
            self._endpoint_handles = set(handles)
            if r.phase != "occurrence-started":
                raise Hold("top deadline must precede private IPC preparation")
            self._budget()
            current, member = self._call("GetCurrentProcess"), abi.BOOL()
            self._ok("IsProcessInJob", current, None, C.byref(member))
            if member.value not in (0, 1):
                raise Hold("invalid host job membership")
            r.host_in_job = bool(member.value)
            for handle in handles:
                if not self._flags(handle) & creator.INHERIT:
                    raise Hold("only explicit private child endpoints may be inherited")
            job = creator.NativeWorkerCreator._job(self, "outer_job")
            self._budget()  # a fired timer cannot authorize later creation
            address = self._attribute_list(job, handles)
            startup = abi.STARTUPINFOEXW()
            startup.StartupInfo.cb = C.sizeof(startup)
            startup.StartupInfo.dwFlags = 0x100
            startup.StartupInfo.hStdInput = child_read
            startup.StartupInfo.hStdOutput = startup.StartupInfo.hStdError = child_write
            startup.lpAttributeList = address.value
            # CP334's inherited channel has its own five-second deadline
            # starting before CreateProcessW. CP333's inner occurrence later
            # allocates a separate five-second deadline; top retains ten seconds.
            self._budget()
            channel_deadline = min(self.deadline, self._tick() + CHANNEL_MS)
            if not 0 < channel_deadline - self._tick() <= CHANNEL_MS:
                raise Hold("fixed channel deadline expired before creation")
            self._channel_deadline = channel_deadline
            # Isolated Python receives no path, project or executable argument.
            self.child_diagnostic_path = _child_diagnostic_path()
            self.child_trace_path = _child_trace_path()
            bootstrap = _child_bootstrap(self.child_diagnostic_path,
                                         self.child_trace_path)
            self._fixed_child_argv = (
                outer.RUNNER_IMAGE, "-I", "-B", "-c", bootstrap, client.ARGV_TAG,
                str(child_read), str(child_write), str(channel_deadline))
            self._fixed_child_command_line = subprocess.list2cmdline(
                self._fixed_child_argv)
            command = C.create_unicode_buffer(self._fixed_child_command_line)
            info = abi.PROCESS_INFORMATION()
            self._budget()
            self._create_attempted = True
            r.create_outcome = "unknown: CreateProcessW entered"
            try:
                ok = self._call("CreateProcessW", outer.RUNNER_IMAGE, command,
                    None, None, True, creator.CREATE_FLAGS, None, outer.RUNNER_CWD,
                    C.byref(startup.StartupInfo), C.byref(info))
                error = None if ok else int(self.exports.last_error())
            finally:
                capture_errors = []
                for name, value in (("runner_process", info.hProcess),
                                    ("runner_thread", info.hThread)):
                    if value:
                        try:
                            self._own(name, value)
                        except BaseException as exc:
                            capture_errors.append(str(exc))
                r.pid, r.tid = int(info.dwProcessId) or None, int(info.dwThreadId) or None
                if capture_errors:
                    raise Hold("; ".join(capture_errors))
            if not ok:
                if not any((info.hProcess, info.hThread,
                            info.dwProcessId, info.dwThreadId)):
                    self._no_child_proven = True
                    r.create_outcome = "returned-failure-no-child"
                raise creator.ApiFailure("CreateProcessW", error)
            r.create_outcome = "returned-success"
            self._verify()
            self._delete_attributes()
            r.phase = "prepared-suspended"
            return r
        except BaseException as error:
            r.errors.append(str(error))
            r.phase = "hold"
            self.close()
            raise TopFailure(error, self) from error

    def run(self):
        """Attempt one fixed CP347 claim; no public native launch is exposed."""
        if self.receipt.phase != "new":
            raise Hold("top occurrence is one-use")
        original_receipt, original_exports = self.receipt, self.exports
        original_security = original_pipe = original_claim = None
        original_channel_deadline = None
        try:
            self.start_occurrence()  # before security/IPC and CreateProcessW
            self._start_timer()      # independent of the launch thread
            self.security = original_security = security.NativePrivatePipeSecurity(self.exports)
            self._budget()
            self.security.prepare()
            self._budget()
            self.pipe_owner = original_pipe = pipes.NativePipeOwner(
                self.exports, self.security, deadline=self.deadline)
            endpoints = self.pipe_owner.prepare(
                r"\\.\pipe\cp349-" + secrets.token_hex(32))
            self._channel_receipt = endpoints
            self._channel_values = (
                endpoints.parent_write, endpoints.client_read,
                endpoints.parent_read, endpoints.client_write)
            channel_frozen = (original_pipe, endpoints, self._channel_values)
            expected = frozenset(self._channel_values)
            self._channel_expected = expected
            if len(expected) != 4:
                raise Hold("four distinct retained claim endpoints required")
            self._channel_custody(channel_frozen, expected)
            parent_write, child_read, parent_read, child_write = self._channel_values
            self.prepare_fixed_suspended(
                child_read=child_read, child_write=child_write)
            self._channel_custody(channel_frozen, expected)
            original_channel_deadline = self._channel_deadline
            # The child received exactly the two listed handles at creation.
            # Parent copies of those clients must not prolong EOF or pipe life.
            self.pipe_owner._close(child_read)
            expected = expected - {child_read}
            self._channel_expected = expected
            self._channel_custody(channel_frozen, expected)
            self.pipe_owner._close(child_write)
            expected = expected - {child_write}
            self._channel_expected = expected
            self._channel_custody(channel_frozen, expected)
            self.resume_once()
            self._budget()
            self._channel_custody(channel_frozen, expected)
            try:
                raw = self.pipe_owner.transfer(parent_read, "read",
                                               parent.FRAME.size)
            except BaseException:
                self._claim_read_failed = True
                raise
            self._budget()
            self._channel_custody(channel_frozen, expected)
            self.claim = original_claim = parent.decode_parent(raw)
            if not self.deadline - SAFETY_MS < self.claim.deadline <= self.deadline:
                raise Hold("inner occurrence deadline exceeds the fixed top ceiling")
            frozen = (self.claim, self.claim.deadline, self.claim.nonce,
                      self.pipe_owner, self.security, self.receipt, self.exports,
                      self._channel_deadline)
            self._acceptance_custody(frozen, channel_frozen, expected)
            available = abi.DWORD()
            self._budget()
            self._ok("PeekNamedPipe", parent_read, None, 0, None,
                     C.byref(available), None)
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            if available.value:
                raise Hold("queued duplicate/trailing parent claim bytes")
            process = self._owned_handles["runner_process"]
            job = self._owned_handles["outer_job"]
            status = self._call("WaitForSingleObject", process, self._budget())
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            if type(status) is not int or status != 0:
                raise Hold("exact CP347 watchdog wait unresolved")
            code = abi.DWORD()
            self._ok("GetExitCodeProcess", process, C.byref(code))
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            if code.value != 0 or self._active(job) != 0:
                raise Hold("watchdog exit or top job accounting not successful")
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            self.receipt.runner_waited = self.receipt.job_empty = True
            self._channel_custody(channel_frozen, expected)
            if self.pipe_owner.cleanup() is not True:
                raise Hold("parent claim pipe cleanup unresolved")
            if (self.pipe_owner.handles != set() or self.pipe_owner.operations != [] or
                    self.pipe_owner.receipt is not None):
                self._channel_custody_uncertain = True
                raise Hold("terminal claim channel still owns handles or operations")
            expected = frozenset()
            self._channel_expected = expected
            self._channel_cleanup_proven = True
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            if self.security.cleanup() is not True:
                raise Hold("private descriptor cleanup unresolved")
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            self._stop_timer()
            self._budget()
            self._acceptance_custody(frozen, channel_frozen, expected)
            if self._timer_fired or self._timer_error is not None:
                raise Hold("top timer fired before acceptance")
            self._runner_success_observed = True
            self._finish_runner_success()
            self._budget()  # final close can also cross the original ceiling
            self._acceptance_custody(frozen, channel_frozen, expected)
            self._accepted = True
            self.receipt.phase = "closed-success"
            return self.receipt
        except BaseException as error:
            self.receipt, self.exports = original_receipt, original_exports
            if original_security is not None:
                self.security = original_security
            if original_pipe is not None:
                self.pipe_owner = original_pipe
            if original_claim is not None:
                self.claim = original_claim
            if original_channel_deadline is not None:
                self._channel_deadline = original_channel_deadline
            original_receipt.errors.append(f"{type(error).__name__}: {error}")
            original_receipt.phase = "hold"
            if self._claim_read_failed:
                self._observe_claim_process_before_cleanup()
            self.close()
            if self._claim_read_failed:
                terminated = (original_receipt.phase == "closed" and
                              original_receipt.runner_waited is True and
                              original_receipt.job_empty is True)
                diagnostic = (_child_diagnostic_result(self.child_diagnostic_path)
                              if terminated else {"status": "unresolved-process",
                                                  "bytes": 0, "sha256": None,
                                                  "data": None})
                diagnostic["path"] = str(self.child_diagnostic_path)
                self.child_diagnostic_result = diagnostic
                trace = (_child_trace_result(self.child_trace_path)
                         if terminated else {"status": "unresolved-process",
                                             "bytes": 0, "sha256": None,
                                             "stages": []})
                trace["path"] = str(self.child_trace_path)
                trace["launch_argv"] = list(self._fixed_child_argv or ())
                trace["command_line"] = self._fixed_child_command_line
                self.child_trace_result = trace
            raise TopFailure(error, self) from error

    def close(self):
        """Retry only exact owned cleanup; never launch or replay a claim."""
        try:
            self._stop_timer()
        except BaseException as error:
            self.receipt.errors.append(str(error))
            self.receipt.phase = "hold"
            return self.receipt
        result = super().close()
        if result.phase not in ("closed", "runner-closed-success", "closed-success"):
            return result
        if self._channel_custody_uncertain:
            result.errors.append("claim channel custody uncertain; original handles retained")
            result.phase = "hold"
            return result
        try:
            if self.pipe_owner is not None and self.pipe_owner.cleanup() is not True:
                raise Hold("top claim pipe cleanup unresolved")
            if self.security is not None and self.security.cleanup() is not True:
                raise Hold("top private descriptor cleanup unresolved")
        except BaseException as error:
            result.errors.append(str(error))
            result.phase = "hold"
        return result


def launch_trial(*_args, **_kwargs):
    raise Hold("CP349 top-controller live launch HARD HOLD pending independent review")


if __name__ == "__main__":
    launch_trial()
