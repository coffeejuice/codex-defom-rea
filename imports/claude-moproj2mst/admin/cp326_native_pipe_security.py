"""CP326 explicit native-capable descriptor owner; live factory HARD HOLD.

Import and construction are inert. Only prepare() uses the supplied CP325
BoundExports .fn(name)/.last_error seam. No DLL loading, pipe creation, process
creation, legacy fake marker, or ctypes signature mutation occurs here.
"""
from __future__ import annotations

import ctypes as C
import cp321_native_exports as abi

NATIVE_ENTRY = "HARD HOLD"
DWORD, HANDLE = abi.DWORD, abi.HANDLE
SECURITY_ATTRIBUTES = abi.SECURITY_ATTRIBUTES
SECURITY_DESCRIPTOR = abi.SECURITY_DESCRIPTOR
SID_AND_ATTRIBUTES = abi.SID_AND_ATTRIBUTES
TOKEN_QUERY, TOKEN_USER = 8, 1
ERROR_NO_TOKEN, ERROR_INSUFFICIENT_BUFFER = 1008, 122
FILE_READ_DATA, FILE_WRITE_DATA, FILE_READ_ATTRIBUTES = 1, 2, 0x80
SYNCHRONIZE = 0x100000
# The inherited worker endpoints query GetNamedPipeInfo before INIT. Both need
# READ_ATTRIBUTES in addition to their direction-specific data access.
CLIENT_READ_ACCESS = FILE_READ_DATA | FILE_READ_ATTRIBUTES | SYNCHRONIZE
CLIENT_WRITE_ACCESS = FILE_WRITE_DATA | FILE_READ_ATTRIBUTES | SYNCHRONIZE
PIPE_ACCESS_MASK = CLIENT_READ_ACCESS | CLIENT_WRITE_ACCESS  # Never bit 4.
INVALID_HANDLE_VALUE = C.c_void_p(-1).value
Hold = abi.Hold


class ApiFailure(Hold):
    def __init__(self, operation, code):
        self.operation, self.code = operation, code
        super().__init__(f"{operation}: Win32 error {code}")


class TokenCleanupFailure(ApiFailure):
    def __init__(self, token_owner, code):
        self.token_owner = token_owner
        super().__init__("CloseHandle(token)", code)


class RetainedTokenOwner:
    """Keep the handle and export owners until a successful close receipt."""
    def __init__(self, exports, handle):
        self.exports, self.handle, self.closed = exports, handle, False

    def retry_close(self):
        if self.closed:
            return
        try:
            ok = self.exports.fn("CloseHandle")(HANDLE(self.handle))
        except BaseException as error:
            raise TokenCleanupFailure(self, None) from error
        if not ok:
            try:
                code = int(self.exports.last_error())
            except BaseException as error:
                raise TokenCleanupFailure(self, None) from error
            raise TokenCleanupFailure(self, code)
        self.closed = True


class NativePrivatePipeSecurity:
    """Own SID/ACL/absolute SD and each returned SA for its entire use.

    prepare() selects the effective thread TokenUser, falling back to process
    TokenUser only for ERROR_NO_TOKEN. A failed prepare is terminal: retain this
    owner and retry cleanup(); create a fresh owner for a new preparation.
    """
    def __init__(self, exports):
        if not callable(getattr(exports, "fn", None)) or not callable(getattr(exports, "last_error", None)):
            raise TypeError("CP325 BoundExports fn/last_error seam required")
        self.exports = exports
        self.token_owner = None
        self._sid = self._acl = self._sd = None
        self.started = self.prepared = False

    def _call(self, name, *args):
        return self.exports.fn(name)(*args)

    def _check(self, name, *args):
        if not self._call(name, *args):
            raise ApiFailure(name, int(self.exports.last_error()))

    def _copy_user(self):
        token = HANDLE()
        if not self._call("OpenThreadToken", self._call("GetCurrentThread"), TOKEN_QUERY, True, C.byref(token)):
            error = int(self.exports.last_error())
            if error != ERROR_NO_TOKEN:
                raise ApiFailure("OpenThreadToken", error)
            self._check("OpenProcessToken", self._call("GetCurrentProcess"), TOKEN_QUERY, C.byref(token))
        if token.value in (None, 0, INVALID_HANDLE_VALUE):
            raise Hold("invalid token handle")
        self.token_owner = RetainedTokenOwner(self.exports, token.value)
        try:
            needed = DWORD()
            ok = self._call("GetTokenInformation", token, TOKEN_USER, None, 0, C.byref(needed))
            error = int(self.exports.last_error())
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
            header = C.string_at(sid, 8)
            length = 8 + 4 * header[1]
            if header[0] != 1 or header[1] > 15 or sid + length > end:
                raise Hold("invalid token SID extent")
            self._check("IsValidSid", sid)
            if self._call("GetLengthSid", sid) != length:
                raise Hold("inconsistent token SID length")
            self._sid = C.create_string_buffer(length)
            self._check("CopySid", length, self._sid, sid)
            self._check("IsValidSid", self._sid)
        finally:
            self.token_owner.retry_close()

    def prepare(self):
        if self.started:
            raise Hold("owner already started; cleanup only")
        self.started = True
        self._copy_user()
        # ACL header (8) + ACE header/mask (8) + aligned SID bytes.
        self._acl = C.create_string_buffer(16 + C.sizeof(self._sid))
        self._sd = SECURITY_DESCRIPTOR()
        self._check("InitializeAcl", self._acl, C.sizeof(self._acl), 2)
        self._check("AddAccessAllowedAceEx", self._acl, 2, 0, PIPE_ACCESS_MASK, self._sid)
        self._check("InitializeSecurityDescriptor", C.byref(self._sd), 1)
        self._check("SetSecurityDescriptorDacl", C.byref(self._sd), True, self._acl, False)
        self._check("IsValidSecurityDescriptor", C.byref(self._sd))
        self.prepared = True
        return self

    def security_attributes(self, inheritable=False):
        if type(inheritable) is not bool:
            raise TypeError("inheritable must be bool")
        if not self.prepared:
            raise Hold("successful prepare required")
        sa = SECURITY_ATTRIBUTES(C.sizeof(SECURITY_ATTRIBUTES), C.addressof(self._sd), inheritable)
        sa._descriptor_owner = self
        return sa

    def cleanup(self):
        """Retry token close only; Python buffers stay alive with this owner/SA."""
        if self.token_owner is not None:
            self.token_owner.retry_close()
        return self.token_owner is None or self.token_owner.closed


def create_live_private_pipe_security(*_args, **_kwargs):
    raise Hold("CP326 native entry HARD HOLD pending end-to-end review")
