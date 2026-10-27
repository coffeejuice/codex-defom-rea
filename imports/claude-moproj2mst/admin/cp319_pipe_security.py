"""CP319 injected-only private pipe security. Live factory remains HARD HOLD.

No DLL loading or native execution occurs here. A fake marker is a test seam,
not a security boundary. This is not integrated into CP317. CP317 now requests
matching specific client rights (0x100001/0x100002); GENERIC_WRITE is excluded
because FILE_APPEND_DATA aliases FILE_CREATE_PIPE_INSTANCE for named pipes.
"""
from __future__ import annotations

import ctypes as C
from typing import Callable, Protocol, Any

NATIVE_ENTRY = "HARD HOLD"
DWORD, BOOL, HANDLE = C.c_uint32, C.c_int32, C.c_void_p
TOKEN_QUERY, TOKEN_USER = 8, 1
ERROR_NO_TOKEN, ERROR_INSUFFICIENT_BUFFER = 1008, 122
PIPE_REJECT_REMOTE_CLIENTS = 8
FILE_FLAG_OVERLAPPED, FILE_FLAG_FIRST_PIPE_INSTANCE = 0x40000000, 0x80000
FILE_READ_DATA, FILE_WRITE_DATA = 1, 2
READ_CONTROL, SYNCHRONIZE = 0x20000, 0x100000
CLIENT_READ_ACCESS = FILE_READ_DATA | SYNCHRONIZE
CLIENT_WRITE_ACCESS = FILE_WRITE_DATA | SYNCHRONIZE
PIPE_ACCESS_MASK = FILE_READ_DATA | FILE_WRITE_DATA | READ_CONTROL | SYNCHRONIZE
INVALID_HANDLE_VALUE = C.c_void_p(-1).value


class SECURITY_ATTRIBUTES(C.Structure):
    _fields_ = [("nLength", DWORD), ("lpSecurityDescriptor", C.c_void_p),
                ("bInheritHandle", BOOL)]


class SID_AND_ATTRIBUTES(C.Structure):
    _fields_ = [("Sid", C.c_void_p), ("Attributes", DWORD)]


class SECURITY_DESCRIPTOR(C.Structure):
    _fields_ = [("Revision", C.c_ubyte), ("Sbz1", C.c_ubyte),
                ("Control", C.c_uint16), ("Owner", C.c_void_p),
                ("Group", C.c_void_p), ("Sacl", C.c_void_p), ("Dacl", C.c_void_p)]


P = C.c_void_p
SIGNATURES = {
    "GetCurrentThread": ([], HANDLE), "GetCurrentProcess": ([], HANDLE),
    "OpenThreadToken": ([HANDLE, DWORD, BOOL, C.POINTER(HANDLE)], BOOL),
    "OpenProcessToken": ([HANDLE, DWORD, C.POINTER(HANDLE)], BOOL),
    "GetTokenInformation": ([HANDLE, C.c_int32, P, DWORD, C.POINTER(DWORD)], BOOL),
    "CloseHandle": ([HANDLE], BOOL), "IsValidSid": ([P], BOOL),
    "GetLengthSid": ([P], DWORD), "CopySid": ([DWORD, P, P], BOOL),
    "InitializeAcl": ([P, DWORD, DWORD], BOOL),
    "AddAccessAllowedAceEx": ([P, DWORD, DWORD, DWORD, P], BOOL),
    "InitializeSecurityDescriptor": ([P, DWORD], BOOL),
    "SetSecurityDescriptorDacl": ([P, BOOL, P, BOOL], BOOL),
    "IsValidSecurityDescriptor": ([P], BOOL),
    "CreateNamedPipeW": ([C.c_wchar_p, DWORD, DWORD, DWORD, DWORD, DWORD,
                           DWORD, C.POINTER(SECURITY_ATTRIBUTES)], HANDLE),
}


class Function(Protocol):
    argtypes: list[Any]
    restype: Any
    def __call__(self, *args: Any) -> Any: ...


class FakeAPI(Protocol):
    cp319_fake_api: bool
    def __getattr__(self, name: str) -> Function: ...


class Hold(RuntimeError):
    pass


class ApiFailure(Hold):
    def __init__(self, operation: str, code: int):
        self.operation, self.code = operation, code
        super().__init__(f"{operation}: Win32 error {code}")


class InjectedPrivatePipeSecurity:
    """Own SID, ACL, absolute descriptor and SA through synchronous pipe create.

    Uses effective thread identity; falls back to the process only when the
    thread has no token. OpenAsSelf permits querying an impersonation token;
    it does not select a different TokenUser. Only the token handle is closed.
    Returned pipe handles belong to the caller, including subsequent I/O cleanup.
    """
    def __init__(self, functions: FakeAPI, *, last_error: Callable[[], int]):
        if getattr(functions, "cp319_fake_api", False) is not True:
            raise Hold("injected fake API required; native entry HARD HOLD")
        if not callable(last_error):
            raise TypeError("last_error callable required")
        self.functions, self.last_error = functions, last_error
        for name, (args, result) in SIGNATURES.items():
            fn = getattr(functions, name)
            if not callable(fn):
                raise TypeError(name)
            fn.argtypes, fn.restype = args, result
        self._sid = self._copy_effective_user()
        # ACCESS_ALLOWED_ACE's header and mask occupy 8 bytes; SID follows.
        self._acl = C.create_string_buffer(8 + 8 + C.sizeof(self._sid))
        self._sd = SECURITY_DESCRIPTOR()
        self._check("InitializeAcl", self._acl, C.sizeof(self._acl), 2)
        self._check("AddAccessAllowedAceEx", self._acl, 2, 0,
                    PIPE_ACCESS_MASK, self._sid)
        self._check("InitializeSecurityDescriptor", C.byref(self._sd), 1)
        self._check("SetSecurityDescriptorDacl", C.byref(self._sd), True,
                    self._acl, False)
        self._check("IsValidSecurityDescriptor", C.byref(self._sd))
        self._sa = SECURITY_ATTRIBUTES(C.sizeof(SECURITY_ATTRIBUTES),
                                      C.addressof(self._sd), False)

    def _check(self, name: str, *args: Any) -> None:
        if not getattr(self.functions, name)(*args):
            raise ApiFailure(name, int(self.last_error()))

    def _copy_effective_user(self) -> Any:
        f, token = self.functions, HANDLE()
        if not f.OpenThreadToken(f.GetCurrentThread(), TOKEN_QUERY, True, C.byref(token)):
            error = int(self.last_error())
            if error != ERROR_NO_TOKEN:
                raise ApiFailure("OpenThreadToken", error)
            self._check("OpenProcessToken", f.GetCurrentProcess(), TOKEN_QUERY, C.byref(token))
        if token.value in (None, 0, INVALID_HANDLE_VALUE):
            raise Hold("invalid token handle")
        try:
            needed = DWORD()
            ok = f.GetTokenInformation(token, TOKEN_USER, None, 0, C.byref(needed))
            error = int(self.last_error())
            if ok or error != ERROR_INSUFFICIENT_BUFFER:
                raise ApiFailure("GetTokenInformation(size)", error)
            if not C.sizeof(SID_AND_ATTRIBUTES) <= needed.value <= 65536:
                raise Hold("invalid token user buffer size")
            size = needed.value
            buf = C.create_string_buffer(size)
            self._check("GetTokenInformation", token, TOKEN_USER, buf, size, C.byref(needed))
            if not C.sizeof(SID_AND_ATTRIBUTES) <= needed.value <= size:
                raise Hold("invalid returned token user size")
            sid = SID_AND_ATTRIBUTES.from_buffer(buf).Sid
            start, end = C.addressof(buf), C.addressof(buf) + needed.value
            if not sid or sid < start + C.sizeof(SID_AND_ATTRIBUTES) or sid + 8 > end:
                raise Hold("token SID outside owned buffer")
            # Bound the SID before calling a native SID validator/length reader.
            header = C.string_at(sid, 8)
            length = 8 + 4 * header[1]
            if header[0] != 1 or header[1] > 15 or sid + length > end:
                raise Hold("invalid token SID extent")
            self._check("IsValidSid", sid)
            if f.GetLengthSid(sid) != length:
                raise Hold("inconsistent token SID length")
            result = C.create_string_buffer(length)
            self._check("CopySid", length, result, sid)
            self._check("IsValidSid", result)
            return result
        finally:
            self._check("CloseHandle", token)

    def create_pipe(self, name: str, *, direction: int) -> int:
        """Create one local, noninheritable, first-instance overlapped pipe.

        Direction is PIPE_ACCESS_INBOUND (1) or OUTBOUND (2); byte mode, one
        instance and 80-byte buffers match CP317. Remote rejection is separate
        from DACL privacy. No client, process, DLL, or bridge is created.
        """
        if not isinstance(name, str) or not name.startswith("\\\\.\\pipe\\") or "\0" in name or len(name) <= 9:
            raise ValueError("nonempty local named-pipe path required")
        if type(direction) is not int or direction not in (1, 2):
            raise ValueError("one-way pipe direction required")
        handle = self.functions.CreateNamedPipeW(
            name, direction | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE,
            PIPE_REJECT_REMOTE_CLIENTS, 1, 80, 80, 0, C.byref(self._sa))
        value = getattr(handle, "value", handle)
        if value in (None, 0, INVALID_HANDLE_VALUE):
            raise ApiFailure("CreateNamedPipeW", int(self.last_error()))
        if type(value) is not int or value < 0:
            raise Hold("invalid pipe handle; ownership uncertain")
        return value


def create_live_private_pipe_security(*args: Any, **kwargs: Any) -> None:
    raise Hold("native entry HARD HOLD pending integration and independent review")
