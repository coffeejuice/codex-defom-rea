"""Explicit CP325/CP321 private overlapped pipes; worker entry is HARD HOLD.

Construction is inert. Keep this owner after failures and retry cleanup until
it returns True. Security and bound DLL owners remain strongly retained.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass

import cp321_native_exports as abi

NATIVE_ENTRY = "HARD HOLD"
Hold = abi.Hold
PENDING, INCOMPLETE, TIMEOUT = 997, 996, 258
CONNECTED, NOT_FOUND = 535, 1168
OVERLAPPED_FLAG, FIRST_INSTANCE = 0x40000000, 0x00080000
INVALID_HANDLE = C.c_void_p(-1).value
# Data, FILE_READ_ATTRIBUTES (worker GetNamedPipeInfo), and SYNCHRONIZE;
# never FILE_CREATE_PIPE_INSTANCE/FILE_APPEND_DATA.
FILE_READ_ATTRIBUTES = 0x80
CLIENT_READ, CLIENT_WRITE = 0x00100081, 0x00100082


class ApiFailure(Hold):
    def __init__(self, name, code):
        self.api, self.code = name, code
        super().__init__(f"{name} failed ({code})")


@dataclass(frozen=True)
class ConnectedEndpoints:
    parent_write: int
    client_read: int
    parent_read: int
    client_write: int

    @property
    def pairs(self):
        return ((self.parent_write, self.client_read),
                (self.parent_read, self.client_write))


@dataclass(eq=False)
class Operation:
    handle: int
    kind: str
    event: int
    overlapped: abi.OVERLAPPED
    buffer: object = None
    terminal: bool = True
    count: int = 0
    error: int = 0
    cancel_requested: bool = False


class NativePipeOwner:
    """One absolute GetTickCount64 deadline, in integer milliseconds.

    security must own a private descriptor and provide the CP326 method
    security_attributes(inheritable=bool), returning canonical attributes.
    This low-level owner does not authorize any worker creation or launch.
    """
    def __init__(self, exports, security, *, deadline):
        if type(deadline) is not int or not 0 <= deadline < 2**64:
            raise ValueError("absolute integer GetTickCount64 deadline required")
        if not callable(getattr(exports, "fn", None)) or not callable(exports.last_error):
            raise TypeError("CP325 BoundExports interface required")
        if not callable(getattr(security, "security_attributes", None)):
            raise TypeError("CP326 security owner required")
        self.exports, self.security, self.deadline = exports, security, deadline
        self.handles, self.operations = set(), []
        self.started = self.held = False
        self.receipt = None
        self.cancel_errors = {}

    def _call(self, name, *args):
        return self.exports.fn(name)(*args)

    def _error(self, name):
        return ApiFailure(name, int(self.exports.last_error()))

    def _budget(self):
        remaining = self.deadline - int(self._call("GetTickCount64"))
        if remaining <= 0:
            raise Hold("absolute pipe deadline expired")
        return min(remaining, 0xFFFFFFFE)  # never INFINITE

    def _own(self, handle, name):
        handle = getattr(handle, "value", handle)
        if handle in (None, 0, -1, INVALID_HANDLE):
            raise self._error(name)
        if type(handle) is not int or handle < 0 or handle in self.handles:
            raise Hold("invalid or aliased handle")
        self.handles.add(handle)  # before any further API/clock operation
        return handle

    def _operation(self, handle, kind, buffer=None):
        self._budget()
        event = self._own(self._call("CreateEventW", None, True, False, None), "CreateEventW")
        op = Operation(handle, kind, event, abi.OVERLAPPED(), buffer)
        op.overlapped.hEvent = event
        self.operations.append(op)
        return op

    def _result(self, op, timeout):
        count = abi.DWORD()
        if self._call("GetOverlappedResultEx", op.handle, C.byref(op.overlapped),
                      C.byref(count), timeout, False):
            op.count, op.terminal = count.value, True
        else:
            code = int(self.exports.last_error())
            if code not in (PENDING, INCOMPLETE, TIMEOUT):
                op.error, op.terminal = code, True
        return op.terminal

    def _close(self, handle):
        if not self._call("CloseHandle", handle):
            raise self._error("CloseHandle")
        self.handles.remove(handle)

    def _finish(self, op):
        while not op.terminal:
            self._result(op, self._budget())
        self._close(op.event)
        self.operations.remove(op)
        self._budget()
        if op.error:
            raise ApiFailure(op.kind, op.error)
        if op.buffer is not None and not 0 < op.count <= len(op.buffer):
            raise Hold("zero or oversized transfer count")
        return op.count

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
                code = int(self.exports.last_error())
                if code == NOT_FOUND:
                    op.cancel_requested = True  # race still requires reap
                    self.cancel_errors.pop(op.event, None)
                else:
                    self.cancel_errors[op.event] = code
        self._result(op, 0)

    def _hold(self):
        self.held = True
        self.receipt = None
        for op in tuple(self.operations):
            try:
                self.cancel(op)
            except BaseException:
                pass  # retain uncertain operation; explicit cleanup retries

    def prepare(self, base_name):
        if self.started or self.held:
            raise Hold("owner already started; cleanup only")
        if (not isinstance(base_name, str) or not base_name.startswith("\\\\.\\pipe\\")
                or len(base_name) <= 9 or "\0" in base_name):
            raise ValueError("nonempty local named-pipe name required")
        self.started = True
        try:
            self._budget()
            server_sa = self.security.security_attributes(inheritable=False)
            client_sa = self.security.security_attributes(inheritable=True)
            if not all(isinstance(sa, abi.SECURITY_ATTRIBUTES) for sa in (server_sa, client_sa)):
                raise TypeError("canonical SECURITY_ATTRIBUTES required")
            if any(sa.nLength != C.sizeof(abi.SECURITY_ATTRIBUTES) or
                   not sa.lpSecurityDescriptor or bool(sa.bInheritHandle) != inherit
                   for sa, inherit in ((server_sa, False), (client_sa, True))):
                raise Hold("private descriptor and exact inheritance attributes required")
            # Keep attribute objects too, even when the caller fails midway.
            self.attributes = (server_sa, client_sa)
            pairs = []
            for suffix, direction, access in (("-in", 2, CLIENT_READ), ("-out", 1, CLIENT_WRITE)):
                self._budget()
                server = self._own(self._call("CreateNamedPipeW", base_name + suffix,
                    direction | OVERLAPPED_FLAG | FIRST_INSTANCE, 8, 1, 80, 80, 0,
                    C.byref(server_sa)), "CreateNamedPipeW")
                op = self._operation(server, "connect")
                self._budget()
                op.terminal = False  # retain even when submission raises
                if self._call("ConnectNamedPipe", server, C.byref(op.overlapped)):
                    op.terminal = True
                else:
                    code = int(self.exports.last_error())
                    if code == CONNECTED:
                        op.terminal = True
                    elif code != PENDING:
                        op.error, op.terminal = code, True
                if op.error:
                    self._finish(op)
                self._budget()
                client = self._own(self._call("CreateFileW", base_name + suffix, access, 0,
                    C.byref(client_sa), 3, OVERLAPPED_FLAG, None), "CreateFileW")
                self._finish(op)
                pairs.append((server, client))
            flat = tuple(h for pair in pairs for h in pair)
            for index, handle in enumerate(flat):
                self._budget()
                flags = abi.DWORD()
                if not self._call("GetHandleInformation", handle, C.byref(flags)):
                    raise self._error("GetHandleInformation")
                if bool(flags.value & 1) != bool(index % 2):
                    raise Hold("inheritance enabled on exactly client endpoints required")
            self._budget()
            self.receipt = ConnectedEndpoints(*flat)
            return self.receipt
        except BaseException:
            self._hold()
            raise

    def transfer(self, handle, kind, payload):
        if self.held or self.receipt is None:
            raise Hold("connected owner required")
        if kind == "write" and type(payload) is bytes and len(payload) in (48, 80):
            size = len(payload)
            allowed = (self.receipt.parent_write, self.receipt.client_write)
        elif kind == "read" and type(payload) is int and payload in (48, 80):
            size = payload
            allowed = (self.receipt.parent_read, self.receipt.client_read)
        else:
            raise ValueError("exact 48/80-byte read or write required")
        if handle not in allowed:
            raise Hold("endpoint direction mismatch")
        offset, chunks = 0, []
        try:
            while offset < size:
                remaining = size - offset
                buffer = ((C.c_ubyte * remaining).from_buffer_copy(payload[offset:])
                          if kind == "write" else (C.c_ubyte * remaining)())
                op = self._operation(handle, kind, buffer)
                self._budget()
                op.terminal = False
                if not self._call("WriteFile" if kind == "write" else "ReadFile",
                                  handle, buffer, remaining, None, C.byref(op.overlapped)):
                    code = int(self.exports.last_error())
                    if code != PENDING:
                        op.error, op.terminal = code, True
                count = self._finish(op)
                if kind == "read":
                    chunks.append(bytes(buffer[:count]))
                offset += count
            self._budget()
            return b"".join(chunks) if kind == "read" else offset
        except BaseException:
            self._hold()
            raise

    def cleanup(self):
        """One nonblocking pass; false means ownership must remain retained.

        Close failures preserve handles and are retryable. No event, buffer or
        pipe involved in pending I/O is released until terminal evidence.
        """
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


def create_live_native_pipes(*_args, **_kwargs):
    raise Hold("CP328 worker live factory HARD HOLD pending integration review")
