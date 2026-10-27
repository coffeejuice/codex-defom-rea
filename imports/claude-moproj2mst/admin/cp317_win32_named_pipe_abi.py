"""CP317 native-shaped ctypes seam. Injected tests ONLY; live entry HARD HOLD.

No DLL is loaded. The fake marker is a test seam, not a security boundary.
Operations retain their ctypes buffer, OVERLAPPED and event until terminal
GetOverlappedResultEx evidence; cancellation alone never releases ownership.
Private DACL creation/verification and CP313 HANDLE_LIST integration are absent.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass
import math
import time

NATIVE_ENTRY = "HARD HOLD"
DWORD = C.c_uint32
BOOL = C.c_int32
HANDLE = C.c_void_p
ULONG_PTR = C.c_size_t
INVALID_HANDLE_VALUE = C.c_void_p(-1).value
ERROR_IO_PENDING, ERROR_IO_INCOMPLETE = 997, 996
ERROR_PIPE_CONNECTED, ERROR_NOT_FOUND = 535, 1168
WAIT_TIMEOUT, ERROR_OPERATION_ABORTED = 258, 995
FILE_FLAG_OVERLAPPED, FILE_FLAG_FIRST_PIPE_INSTANCE = 0x40000000, 0x80000
GENERIC_READ, GENERIC_WRITE = 0x80000000, 0x40000000
FILE_READ_DATA, FILE_WRITE_DATA, SYNCHRONIZE = 0x0001, 0x0002, 0x00100000
# FILE_GENERIC_WRITE includes FILE_APPEND_DATA (0x4), which aliases
# FILE_CREATE_PIPE_INSTANCE for pipes. Request only these individual rights;
# no append/create-instance or write-attribute access is needed by this seam.
CLIENT_READ_ACCESS = FILE_READ_DATA | SYNCHRONIZE
CLIENT_WRITE_ACCESS = FILE_WRITE_DATA | SYNCHRONIZE


class SECURITY_ATTRIBUTES(C.Structure):
    _fields_ = [("nLength", DWORD), ("lpSecurityDescriptor", C.c_void_p),
                ("bInheritHandle", BOOL)]


class _Offsets(C.Structure):
    _fields_ = [("Offset", DWORD), ("OffsetHigh", DWORD)]


class _Location(C.Union):
    _fields_ = [("offsets", _Offsets), ("Pointer", C.c_void_p)]


class OVERLAPPED(C.Structure):
    _fields_ = [("Internal", ULONG_PTR), ("InternalHigh", ULONG_PTR),
                ("location", _Location), ("hEvent", HANDLE)]


LPDWORD, LPOVERLAPPED = C.POINTER(DWORD), C.POINTER(OVERLAPPED)
SIGNATURES = {
    "CreateNamedPipeW": ([C.c_wchar_p, DWORD, DWORD, DWORD, DWORD, DWORD,
                           DWORD, C.POINTER(SECURITY_ATTRIBUTES)], HANDLE),
    "CreateFileW": ([C.c_wchar_p, DWORD, DWORD, C.POINTER(SECURITY_ATTRIBUTES),
                      DWORD, DWORD, HANDLE], HANDLE),
    "ConnectNamedPipe": ([HANDLE, LPOVERLAPPED], BOOL),
    "ReadFile": ([HANDLE, C.c_void_p, DWORD, LPDWORD, LPOVERLAPPED], BOOL),
    "WriteFile": ([HANDLE, C.c_void_p, DWORD, LPDWORD, LPOVERLAPPED], BOOL),
    "GetOverlappedResultEx": ([HANDLE, LPOVERLAPPED, LPDWORD, DWORD, BOOL], BOOL),
    "CancelIoEx": ([HANDLE, LPOVERLAPPED], BOOL),
    "CreateEventW": ([C.POINTER(SECURITY_ATTRIBUTES), BOOL, BOOL, C.c_wchar_p], HANDLE),
    "CloseHandle": ([HANDLE], BOOL),
}


class Hold(RuntimeError):
    pass


class ApiFailure(Hold):
    def __init__(self, operation, code):
        self.operation, self.code = operation, code
        super().__init__(f"{operation}: Win32 error {code}")


@dataclass(eq=False)
class Operation:
    handle: int
    kind: str
    event: int
    overlapped: OVERLAPPED
    buffer: object = None
    count: int = 0
    terminal: bool = False
    error: int = 0
    cancel_requested: bool = False


class InjectedPipeABI:
    """Small injected ABI owner; caller retains this object after any HOLD.

    A supplied descriptor pointer is only passed through; it is NOT verified.
    All handles stay local. Returned client handles require a later reviewed
    restricted HANDLE_LIST bridge. No process creation or resume exists here.
    """
    def __init__(self, functions, *, last_error, deadline, clock=time.monotonic):
        if getattr(functions, "cp317_fake_api", False) is not True:
            raise Hold("injected fake API required; native entry HARD HOLD")
        if not callable(last_error) or not callable(clock):
            raise TypeError("error and clock callables required")
        if not math.isfinite(deadline):
            raise ValueError("finite absolute deadline required")
        self.functions, self.last_error = functions, last_error
        self.deadline, self.clock = deadline, clock
        self.handles: set[int] = set()
        self.operations: list[Operation] = []
        self.held = False
        for name, (args, result) in SIGNATURES.items():
            fn = getattr(functions, name)
            if not callable(fn):
                raise TypeError(name)
            fn.argtypes, fn.restype = args, result

    def _budget(self):
        remaining = self.deadline - self.clock()
        if not math.isfinite(remaining) or remaining < .001:
            raise Hold("absolute pre-creation deadline exhausted")
        return min(int(remaining * 1000), 5000)

    def _own(self, value, name):
        value = getattr(value, "value", value)
        if value in (None, 0, INVALID_HANDLE_VALUE):
            raise ApiFailure(name, int(self.last_error()))
        if type(value) is not int or value < 0 or value in self.handles:
            raise Hold("invalid/aliased handle; ownership uncertain")
        self.handles.add(value)
        return value

    def _new_operation(self, handle, kind, buffer=None):
        if self.held:
            raise Hold("owner held after uncertain/failed operation; reap/close only")
        if handle not in self.handles:
            raise Hold("unowned pipe handle")
        self._budget()
        event = self._own(self.functions.CreateEventW(None, True, False, None), "CreateEventW")
        op = Operation(handle, kind, event, OVERLAPPED(), buffer)
        op.overlapped.hEvent = event
        self.operations.append(op)
        return op

    def _result(self, op, milliseconds):
        count = DWORD()
        if self.functions.GetOverlappedResultEx(op.handle, C.byref(op.overlapped),
                                                C.byref(count), milliseconds, False):
            op.count, op.terminal = count.value, True
        else:
            error = int(self.last_error())
            if error not in (WAIT_TIMEOUT, ERROR_IO_INCOMPLETE, ERROR_IO_PENDING):
                op.error, op.terminal = error, True
        return op.terminal

    def finish(self, op):
        if op not in self.operations:
            raise Hold("foreign operation")
        try:
            while not op.terminal:
                self._result(op, self._budget())
            self.close(op.event)
            self.operations.remove(op)
            self._budget()  # a late result remains held, never retried
            if op.error:
                raise ApiFailure(op.kind, op.error)
            if op.buffer is not None and not 0 < op.count <= len(op.buffer):
                raise Hold("invalid/zero transfer completion count")
            return op.count
        except BaseException:
            self.held = True
            if not op.terminal:
                self.cancel(op)
            raise

    def cancel(self, op):
        if op not in self.operations or op.terminal or op.cancel_requested:
            return
        op.cancel_requested = True
        # ERROR_NOT_FOUND is a race, never proof of completion.
        self.functions.CancelIoEx(op.handle, C.byref(op.overlapped))
        self._result(op, 0)

    def reap(self):
        """One nonblocking pass after HOLD; closes events only after terminal."""
        for op in tuple(self.operations):
            if op.terminal or self._result(op, 0):
                self.close(op.event)
                self.operations.remove(op)
        return len(self.operations)

    def transfer(self, handle, kind, payload):
        if self.held:
            raise Hold("owner held; reap/close only")
        if kind == "write" and type(payload) is bytes and len(payload) in (48, 80):
            buffer = (C.c_ubyte * len(payload)).from_buffer_copy(payload)
        elif kind == "read" and type(payload) is int and payload in (48, 80):
            buffer = (C.c_ubyte * payload)()
        else:
            raise ValueError("read size or write bytes must be exactly 48/80")
        op = self._new_operation(handle, kind, buffer)
        try:
            fn = self.functions.ReadFile if kind == "read" else self.functions.WriteFile
            ok = fn(handle, buffer, len(buffer), None, C.byref(op.overlapped))
            if not ok:
                error = int(self.last_error())
                if error != ERROR_IO_PENDING:
                    op.terminal, op.error = True, error
            # Even synchronous success is queried for its exact byte count.
            self.finish(op)
            return bytes(buffer[:op.count]) if kind == "read" else op.count
        except BaseException:
            self.held = True
            if not op.terminal:
                self.cancel(op)
            raise

    def create_pair(self, base_name, descriptor):
        """Create two local one-way endpoints. Explicit fake descriptor only.

        Caller owns partial handles/operations on failure and must reap/close.
        Random local names do not establish privacy; no DACL claim is made.
        """
        if self.held:
            raise Hold("owner held; reap/close only")
        if not isinstance(base_name, str) or not base_name.startswith("\\\\.\\pipe\\"):
            raise ValueError("local named-pipe path required")
        if not descriptor:
            raise Hold("explicit security descriptor required (unverified fake seam)")
        result = []
        for suffix, access, client_access in (("-in", 2, CLIENT_READ_ACCESS), ("-out", 1, CLIENT_WRITE_ACCESS)):
            self._budget()
            server_sa = SECURITY_ATTRIBUTES(C.sizeof(SECURITY_ATTRIBUTES), descriptor, False)
            client_sa = SECURITY_ATTRIBUTES(C.sizeof(SECURITY_ATTRIBUTES), descriptor, True)
            name = base_name + suffix
            server = self._own(self.functions.CreateNamedPipeW(
                name, access | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
                8, 1, 80, 80, 0, C.byref(server_sa)), "CreateNamedPipeW")
            op = self._new_operation(server, "connect")
            try:
                if not self.functions.ConnectNamedPipe(server, C.byref(op.overlapped)):
                    error = int(self.last_error())
                    if error == ERROR_PIPE_CONNECTED:
                        op.terminal = True
                    elif error != ERROR_IO_PENDING:
                        op.terminal, op.error = True, error
                if op.error:
                    self.finish(op)
                self._budget()
                client = self._own(self.functions.CreateFileW(
                    name, client_access, 0, C.byref(client_sa), 3,
                    FILE_FLAG_OVERLAPPED, None), "CreateFileW")
                self.finish(op)
                result.append((server, client))
            except BaseException:
                self.held = True
                if not op.terminal:
                    self.cancel(op)
                raise
        return tuple(result)

    def close(self, handle):
        if handle not in self.handles:
            raise Hold("unowned/already closed handle")
        if any(not op.terminal and handle in (op.handle, op.event) for op in self.operations):
            raise Hold("pending operation forbids handle/event close")
        if not self.functions.CloseHandle(handle):
            raise ApiFailure("CloseHandle", int(self.last_error()))
        self.handles.remove(handle)


def create_live_named_pipes(*_args, **_kwargs):
    raise Hold("native Win32 named-pipe entry HARD HOLD: private DACL, inheritance, native validation and bridge pending")
