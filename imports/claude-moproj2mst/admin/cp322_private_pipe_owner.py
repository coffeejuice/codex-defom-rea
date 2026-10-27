"""CP322 injected private two-pipe owner. Live entry remains HARD HOLD.

CP319 owns the effective-user descriptor; CP317 owns pending operations and
handles. No DLL, process, HANDLE_LIST bridge, or live-export binding is used.
Fake markers describe test doubles only and never authorize native exports.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass

import cp317_win32_named_pipe_abi as abi
import cp319_pipe_security as security

NATIVE_ENTRY = "HARD HOLD"
HANDLE_FLAG_INHERIT = 1
Hold, ApiFailure = abi.Hold, abi.ApiFailure


@dataclass(frozen=True)
class ConnectedEndpoints:
    """Receipt issued only after both connections and inheritance verification."""
    parent_write: int
    client_read: int
    parent_read: int
    client_write: int

    @property
    def pairs(self):
        return ((self.parent_write, self.client_read),
                (self.parent_read, self.client_write))


def _require_test_double(functions):
    # Marking a WinDLL or one of its exports as fake must not bypass the hold.
    if isinstance(functions, C.CDLL):
        raise Hold("native DLL rejected; CP321 canonical binding pending")
    if (getattr(functions, "cp317_fake_api", False) is not True or
            getattr(functions, "cp319_fake_api", False) is not True):
        raise Hold("explicit injected fakes required; native entry HARD HOLD")
    for name in set(abi.SIGNATURES) | set(security.SIGNATURES) | {"GetHandleInformation"}:
        fn = getattr(functions, name)
        if isinstance(fn, C._CFuncPtr):
            raise Hold("native export rejected; CP321 canonical binding pending")


class _BudgetedSecurityCall:
    def __init__(self, owner, name):
        self.owner, self.name = owner, name

    def __call__(self, *args):
        # Token cleanup remains available after the transaction deadline.
        if self.name != "CloseHandle":
            self.owner._budget()
        return getattr(self.owner.functions, self.name)(*args)


class _BudgetedSecurityFake:
    """Budget only previously validated injected fakes; never wraps a DLL."""
    cp319_fake_api = True

    def __init__(self, owner):
        for name in security.SIGNATURES:
            setattr(self, name, _BudgetedSecurityCall(owner, name))


class InjectedPrivatePipeOwner(abi.InjectedPipeABI):
    """Retain this owner after any failure and retry cleanup until empty.

    The supplied absolute deadline starts before identity/descriptor creation;
    synchronous API calls cannot be preempted, so late returns fail closed.
    Security ownership deliberately lasts for the entire endpoint lifetime.
    """
    def __init__(self, functions, *, last_error, deadline, clock=abi.time.monotonic):
        _require_test_double(functions)
        super().__init__(functions, last_error=last_error, deadline=deadline, clock=clock)
        functions.GetHandleInformation.argtypes = [abi.HANDLE, abi.LPDWORD]
        functions.GetHandleInformation.restype = abi.BOOL
        self.security = None
        self.receipt = None
        self.started = False
        self.cancel_errors = {}
        self.token_owner = None

    def _submission_budget(self, op):
        try:
            self._budget()
        except BaseException:
            # Event creation completed, but no I/O was submitted. Its resources
            # may be reaped without cancellation or a completion query.
            op.terminal = True
            raise

    def _cancel_pending(self):
        for op in tuple(self.operations):
            if not op.terminal:
                try:
                    self.cancel(op)
                except BaseException:
                    # Preserve the original failure and retry cancellation later.
                    pass

    def cancel(self, op):
        if op not in self.operations or op.terminal or op.cancel_requested:
            return
        failure = None
        try:
            accepted = self.functions.CancelIoEx(op.handle, C.byref(op.overlapped))
            if not accepted:
                code = int(self.last_error())
                if code != abi.ERROR_NOT_FOUND:
                    self.cancel_errors[op.event] = code
                    failure = ApiFailure("CancelIoEx", code)
            # ERROR_NOT_FOUND is a completion race, never terminal evidence.
            # Other failures did not request cancellation: keep retry enabled.
            op.cancel_requested = failure is None
            if failure is None:
                self.cancel_errors.pop(op.event, None)
            self._result(op, 0)
        except BaseException:
            op.cancel_requested = False
            raise
        if failure is not None:
            raise failure

    def create_pair(self, base_name):
        if self.held or self.started:
            raise Hold("owner already started/held; cleanup only")
        if (not isinstance(base_name, str) or not base_name.startswith("\\\\.\\pipe\\")
                or len(base_name) <= 9 or "\0" in base_name):
            raise ValueError("nonempty local named-pipe path required")
        self.started = True
        try:
            self._budget()
            try:
                self.security = security.InjectedPrivatePipeSecurity(
                    _BudgetedSecurityFake(self), last_error=self.last_error)
            except security.TokenCleanupFailure as error:
                self.token_owner = error.token_owner
                raise
            self._budget()
            pairs = []
            for suffix, direction, access in (
                    ("-in", 2, abi.CLIENT_READ_ACCESS),
                    ("-out", 1, abi.CLIENT_WRITE_ACCESS)):
                self._budget()
                # Adopt immediately, before any clock check or further API call.
                server = self._own(self.security.create_pipe(base_name + suffix,
                                                             direction=direction),
                                   "CreateNamedPipeW")
                op = self._new_operation(server, "connect")
                self._submission_budget(op)
                if not self.functions.ConnectNamedPipe(server, C.byref(op.overlapped)):
                    error = int(self.last_error())
                    if error == abi.ERROR_PIPE_CONNECTED:
                        op.terminal = True
                    elif error != abi.ERROR_IO_PENDING:
                        op.terminal, op.error = True, error
                if op.error:
                    self.finish(op)
                self._budget()
                sa = abi.SECURITY_ATTRIBUTES(C.sizeof(abi.SECURITY_ATTRIBUTES), None, True)
                client = self._own(self.functions.CreateFileW(
                    base_name + suffix, access, 0, C.byref(sa), 3,
                    abi.FILE_FLAG_OVERLAPPED, None), "CreateFileW")
                self.finish(op)
                pairs.append((server, client))
            flat = tuple(h for pair in pairs for h in pair)
            if len(set(flat)) != 4 or set(flat) != self.handles or self.operations:
                raise Hold("four distinct endpoints and no pending connects required")
            for index, handle in enumerate(flat):
                self._budget()
                flags = abi.DWORD()
                if not self.functions.GetHandleInformation(handle, C.byref(flags)):
                    raise ApiFailure("GetHandleInformation", int(self.last_error()))
                if bool(flags.value & HANDLE_FLAG_INHERIT) != bool(index % 2):
                    raise Hold("inheritance must be enabled on exactly the client handles")
            self._budget()
            self.receipt = ConnectedEndpoints(*flat)
            return self.receipt
        except BaseException:
            self.held = True
            self._cancel_pending()
            raise

    def transfer(self, handle, kind, payload):
        """Complete exactly 48/80 bytes, including short overlapped transfers."""
        if self.held or self.receipt is None:
            raise Hold("connected receipt required; held owner permits cleanup only")
        if kind == "write" and type(payload) is bytes and len(payload) in (48, 80):
            size = len(payload)
        elif kind == "read" and type(payload) is int and payload in (48, 80):
            size = payload
        else:
            raise ValueError("read size or write bytes must be exactly 48/80")
        if handle not in (self.receipt.parent_write if kind == "write" else self.receipt.parent_read,
                          self.receipt.client_write if kind == "write" else self.receipt.client_read):
            raise Hold("endpoint direction mismatch")
        offset, chunks = 0, []
        try:
            while offset < size:
                self._budget()
                remaining = size - offset
                buffer = ((C.c_ubyte * remaining).from_buffer_copy(payload[offset:])
                          if kind == "write" else (C.c_ubyte * remaining)())
                op = self._new_operation(handle, kind, buffer)
                fn = self.functions.WriteFile if kind == "write" else self.functions.ReadFile
                self._submission_budget(op)
                if not fn(handle, buffer, remaining, None, C.byref(op.overlapped)):
                    error = int(self.last_error())
                    if error != abi.ERROR_IO_PENDING:
                        op.terminal, op.error = True, error
                count = self.finish(op)
                if kind == "read":
                    chunks.append(bytes(buffer[:count]))
                offset += count
            self._budget()
            return b"".join(chunks) if kind == "read" else offset
        except BaseException:
            self.held = True
            self._cancel_pending()
            raise

    def cleanup(self):
        """One nonblocking retry pass; failures preserve all unclosed resources."""
        self.held = True
        self.receipt = None
        self._cancel_pending()
        self.reap()
        if self.token_owner is not None and not self.token_owner.closed:
            self.token_owner.retry_close()
        if self.operations:
            return False
        for handle in tuple(self.handles):
            self.close(handle)
        return not self.handles


def create_live_private_pipes(*_args, **_kwargs):
    raise Hold("native entry HARD HOLD pending CP321 canonical export binding and live review")
