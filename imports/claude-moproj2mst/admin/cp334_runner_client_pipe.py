"""CP334 child-side custody of two inherited pipe clients; live entry HARD HOLD.

No DLL loading, process creation, pipe-name lookup, or parent endpoint exists
here. Adoption consumes only the explicitly supplied handles. The fixed CP331
argv must carry the original absolute GetTickCount64 deadline before any read.
Failed owners must be retained until cleanup() returns True.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass

import cp321_native_exports as abi
from cp325_real_bindings import BoundExports
from cp328_native_pipes import Operation, ApiFailure, PIPE_REJECT_REMOTE_CLIENTS

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
PENDING, INCOMPLETE, TIMEOUT, NOT_FOUND = 997, 996, 258, 1168
INVALID = (1 << 64) - 1
ARGV_TAG = "--cp334-client"


@dataclass(frozen=True)
class ClientEndpoints:
    client_read: int
    client_write: int


class NativeRunnerClientPipe:
    """Explicit native-capable owner, with no authority to launch a runner.

    GetNamedPipeInfo success proves FILE_READ_ATTRIBUTES query capability;
    exactly PIPE_REJECT_REMOTE_CLIENTS flags establish the client byte-pipe
    end with the fixed CP328 local-only mode. Direction and
    OVERLAPPED provenance remain the fixed CP328/CP331 creation contract;
    Win32 GetNamedPipeInfo does not report those access/creation flags.
    """
    def __init__(self, exports, *, deadline, stage=None):
        if not isinstance(exports, BoundExports):
            raise TypeError("CP325 BoundExports required")
        if type(deadline) is not int or not 0 < deadline < 2**64:
            raise ValueError("explicit absolute GetTickCount64 deadline required")
        if stage is not None and not callable(stage):
            raise TypeError("optional diagnostic stage callback must be callable")
        self._exports, self._deadline = exports, deadline
        self._stage = stage
        self._receipt = None
        self.handles, self.operations, self.cancel_errors = set(), [], {}
        self.started = self.held = False
        self._last_tick = None

    @property
    def exports(self):
        return self._exports

    @property
    def deadline(self):
        return self._deadline

    @property
    def receipt(self):
        return self._receipt

    def _call(self, name, *args):
        return self._exports.fn(name)(*args)

    def _failure(self, name):
        return ApiFailure(name, int(self._exports.last_error()))

    def _mark(self, name):
        if self._stage is not None:
            self._stage(name)

    def _budget(self):
        now = self._call("GetTickCount64")
        if (type(now) is not int or not 0 <= now < 2**64 or
                (self._last_tick is not None and now < self._last_tick) or
                not 0 < self.deadline - now <= 5000):
            raise Hold("original runner deadline expired or invalid")
        self._last_tick = now
        return self.deadline - now

    def adopt(self, input_read, output_write):
        """Claim exactly two inherited handles before querying either.

        A failed query retains both handles for explicit retryable cleanup.
        This does not enumerate the process handle table or claim other handles.
        """
        if self.started or self.held:
            raise Hold("one-use adoption; cleanup only")
        values = (input_read, output_write)
        if (any(type(h) is not int or not 0 < h < INVALID - 15 for h in values)
                or input_read == output_write):
            raise Hold("two distinct real client handles required")
        self.started = True
        self.handles.update(values)
        try:
            self._mark("cp334_adopt_enter")
            for index, handle in enumerate(values):
                self._mark(f"cp334_h{index}_before_tick1")
                self._budget()
                self._mark(f"cp334_h{index}_after_tick1")
                flags = abi.DWORD()
                self._mark(f"cp334_h{index}_before_inherit")
                if not self._call("GetHandleInformation", handle, C.byref(flags)):
                    raise self._failure("GetHandleInformation")
                self._mark(f"cp334_h{index}_after_inherit")
                if flags.value != 1:
                    raise Hold("inheritable, unprotected client handle required")
                self._mark(f"cp334_h{index}_before_tick2")
                self._budget()
                self._mark(f"cp334_h{index}_after_tick2")
                pipe_flags = abi.DWORD()
                self._mark(f"cp334_h{index}_before_pipeinfo")
                if not self._call("GetNamedPipeInfo", handle, C.byref(pipe_flags),
                                  None, None, None):
                    raise self._failure("GetNamedPipeInfo")
                self._mark(f"cp334_h{index}_after_pipeinfo")
                self._mark(f"cp334_h{index}_pipeflags_{pipe_flags.value}")
                if pipe_flags.value != PIPE_REJECT_REMOTE_CLIENTS:
                    raise Hold("local-only named byte-pipe client end required")
                self._mark(f"cp334_h{index}_flags_validated")
            self._mark("cp334_before_final_tick")
            self._budget()
            self._mark("cp334_after_final_tick")
            self._receipt = ClientEndpoints(*values)
            return self._receipt
        except BaseException:
            self._hold()
            raise

    def _close(self, handle):
        if not self._call("CloseHandle", handle):
            raise self._failure("CloseHandle")
        self.handles.remove(handle)

    def _result(self, op, timeout):
        count = abi.DWORD()
        if self._call("GetOverlappedResultEx", op.handle, C.byref(op.overlapped),
                      C.byref(count), timeout, False):
            op.count, op.terminal = count.value, True
        else:
            code = int(self._exports.last_error())
            if code not in (PENDING, INCOMPLETE, TIMEOUT):
                op.error, op.terminal = code, True
        return op.terminal

    def cancel(self, op):
        if op not in self.operations:
            raise Hold("foreign operation")
        if op.terminal:
            return
        if not op.cancel_requested:
            if self._call("CancelIoEx", op.handle, C.byref(op.overlapped)):
                op.cancel_requested = True
                self.cancel_errors.pop(op.event, None)
            else:
                code = int(self._exports.last_error())
                if code == NOT_FOUND:
                    op.cancel_requested = True
                    self.cancel_errors.pop(op.event, None)
                else:
                    self.cancel_errors[op.event] = code
        self._result(op, 0)

    def _hold(self):
        self.held = True
        self._receipt = None
        for op in tuple(self.operations):
            try:
                self.cancel(op)
            except BaseException:
                pass  # submission/cancellation outcomes remain retained

    def transfer(self, handle, kind, payload):
        if self.held or self.receipt is None:
            raise Hold("adopted client channel required")
        if kind == "read" and type(payload) is int and payload in (48, 80):
            size, expected = payload, self.receipt.client_read
        elif kind == "write" and type(payload) is bytes and len(payload) in (48, 80):
            size, expected = len(payload), self.receipt.client_write
        else:
            raise ValueError("exact 48/80-byte read or write required")
        if type(handle) is not int or handle != expected or handle not in self.handles:
            raise Hold("client endpoint direction/custody mismatch")
        offset, chunks = 0, []
        try:
            while offset < size:
                self._budget()
                remaining = size - offset
                buffer = ((C.c_ubyte * remaining).from_buffer_copy(payload[offset:])
                          if kind == "write" else (C.c_ubyte * remaining)())
                event = self._call("CreateEventW", None, True, False, None)
                event = getattr(event, "value", event)
                if event in (None, 0, -1, INVALID):
                    raise self._failure("CreateEventW")
                if type(event) is not int or not 0 < event < INVALID - 15 or event in self.handles:
                    raise Hold("invalid or aliased event handle")
                self.handles.add(event)  # retain before the next clock/API call
                op = Operation(handle, kind, event, abi.OVERLAPPED(), buffer)
                op.overlapped.hEvent = event
                self.operations.append(op)
                self._budget()
                op.terminal = False  # a raised submission has unknown outcome
                if not self._call("ReadFile" if kind == "read" else "WriteFile",
                                  handle, buffer, remaining, None, C.byref(op.overlapped)):
                    code = int(self._exports.last_error())
                    if code != PENDING:
                        op.error, op.terminal = code, True
                while not op.terminal:
                    self._result(op, self._budget())
                self._close(op.event)
                self.operations.remove(op)
                self._budget()
                if op.error:
                    raise ApiFailure(kind, op.error)
                if not 0 < op.count <= remaining:
                    raise Hold("zero or oversized transfer completion")
                if kind == "read":
                    chunks.append(bytes(buffer[:op.count]))
                offset += op.count
            self._budget()
            return b"".join(chunks) if kind == "read" else offset
        except BaseException:
            self._hold()
            raise

    def cleanup(self):
        """Nonblocking retry; retain all pipes until every operation is reaped."""
        self._hold()
        for op in tuple(self.operations):
            try:
                if op.terminal or self._result(op, 0):
                    self._close(op.event)
                    self.operations.remove(op)
                    self.cancel_errors.pop(op.event, None)
            except BaseException:
                pass
        if self.operations:
            return False
        for handle in tuple(self.handles):
            try:
                self._close(handle)
            except BaseException:
                pass
        return not self.handles


def from_bootstrap_argv(exports, argv, *, stage=None):
    """Parse only fixed argv tail: TAG input-read output-write absolute-deadline.

    Accept canonical unsigned decimal strings only. Call before protocol reads;
    no BOOTSTRAP-derived timeout or environment/default deadline is accepted.
    On adoption failure the raised exception carries .owner for cleanup.
    """
    if (type(argv) not in (tuple, list) or len(argv) != 4 or
            type(argv[0]) is not str or argv[0] != ARGV_TAG or
            any(type(s) is not str or not s.isascii() or not s.isdecimal() or
                len(s) > 20 or str(int(s)) != s for s in argv[1:])):
        raise Hold("fixed CP334 bootstrap argv required")
    input_read, output_write, deadline = map(int, argv[1:])
    if stage is not None:
        stage("cp334_argv_parsed")
    owner = NativeRunnerClientPipe(exports, deadline=deadline, stage=stage)
    try:
        owner._mark("cp334_owner_created")
        owner.adopt(input_read, output_write)
    except BaseException as error:
        error.owner = owner
        raise
    return owner


def launch_trial(*_args, **_kwargs):
    raise Hold("CP334 live entry HARD HOLD pending CP331/CP333 hook integration review")


if __name__ == "__main__":
    launch_trial()
