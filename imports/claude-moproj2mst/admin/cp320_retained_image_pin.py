"""CP320 retained CP316 image pin. Injected Win32 fakes ONLY; live HARD HOLD.

A shared-read-only file handle denies compatible write/delete opens while held.
This establishes retained-file and canonical-path identity, NOT the bytes mapped
into a child process. Path traversal/reparse races before image section creation,
pre-existing mappings, privileged writers and process-loader behavior remain
outside this isolated gate. Do not infer loaded-image attestation from this API.
No DLL is loaded and no process is created or resumed here.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass
import hashlib
import ntpath

NATIVE_ENTRY = "HARD HOLD"
WORKER_IMAGE = r"C:\PROBLEM\s316\worker.exe"
WORKER_BYTES = 104960
WORKER_SHA256 = "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985"
DWORD, BOOL, HANDLE = C.c_uint32, C.c_int32, C.c_void_p
INVALID_HANDLE_VALUE = C.c_void_p(-1).value
GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING = 0x80000000, 1, 3
FILE_ATTRIBUTE_NORMAL, FILE_ATTRIBUTE_DIRECTORY = 0x80, 0x10
HANDLE_FLAG_INHERIT, FileIdInfo = 1, 18


class SECURITY_ATTRIBUTES(C.Structure):
    _fields_ = [("nLength", DWORD), ("lpSecurityDescriptor", C.c_void_p),
                ("bInheritHandle", BOOL)]


class FILETIME(C.Structure):
    _fields_ = [("dwLowDateTime", DWORD), ("dwHighDateTime", DWORD)]


class BY_HANDLE_FILE_INFORMATION(C.Structure):
    _fields_ = [("dwFileAttributes", DWORD), ("ftCreationTime", FILETIME),
                ("ftLastAccessTime", FILETIME), ("ftLastWriteTime", FILETIME),
                ("dwVolumeSerialNumber", DWORD), ("nFileSizeHigh", DWORD),
                ("nFileSizeLow", DWORD), ("nNumberOfLinks", DWORD),
                ("nFileIndexHigh", DWORD), ("nFileIndexLow", DWORD)]


class FILE_ID_128(C.Structure):
    _fields_ = [("Identifier", C.c_ubyte * 16)]


class FILE_ID_INFO(C.Structure):
    _fields_ = [("VolumeSerialNumber", C.c_uint64), ("FileId", FILE_ID_128)]


SIGNATURES = {
    "CreateFileW": ([C.c_wchar_p, DWORD, DWORD, C.POINTER(SECURITY_ATTRIBUTES),
                     DWORD, DWORD, HANDLE], HANDLE),
    "GetHandleInformation": ([HANDLE, C.POINTER(DWORD)], BOOL),
    "GetFileInformationByHandle": ([HANDLE, C.POINTER(BY_HANDLE_FILE_INFORMATION)], BOOL),
    "GetFileInformationByHandleEx": ([HANDLE, C.c_int, C.c_void_p, DWORD], BOOL),
    "GetFinalPathNameByHandleW": ([HANDLE, C.c_wchar_p, DWORD, DWORD], DWORD),
    "SetFilePointerEx": ([HANDLE, C.c_int64, C.POINTER(C.c_int64), DWORD], BOOL),
    "ReadFile": ([HANDLE, C.c_void_p, DWORD, C.POINTER(DWORD), C.c_void_p], BOOL),
    "CloseHandle": ([HANDLE], BOOL),
}


class Hold(RuntimeError):
    pass


class ApiFailure(Hold):
    def __init__(self, operation, code):
        self.operation, self.code = operation, code
        super().__init__(f"{operation}: Win32 error {code}")


def open_live_pin(*_args, **_kwargs):
    raise Hold("CP320 native HARD HOLD: isolated fake seam requires creator/cleanup integration")


def _canonical(path):
    if path.startswith("\\\\?\\"):
        path = path[4:]
    return ntpath.normcase(ntpath.normpath(path))


@dataclass(frozen=True)
class Identity:
    volume: int
    file_id: bytes
    size: int
    legacy_volume: int
    legacy_index: int
    final_path: str


@dataclass(frozen=True)
class Snapshot:
    sha256: str
    identity: Identity


class FakeRetainedImagePin:
    """One-use fake-only pin; retain owner on any failure until close succeeds.

    Call prepare before CreateProcessW, then verify_after_create immediately
    after its successful return while the child is still suspended. The caller
    must gate resume and keep this owner through process/job cleanup. These
    ordering/cleanup obligations are not implemented or proven by this module.
    The fake marker is a test seam, never a security boundary.
    """
    def __init__(self, functions, *, last_error):
        if getattr(functions, "cp320_fake_api", False) is not True:
            raise Hold("CP320 injected fake API required")
        self._fn = {}
        for name, (args, result) in SIGNATURES.items():
            function = getattr(functions, name)
            function.argtypes, function.restype = args, result
            self._fn[name] = function
        self._last_error = last_error
        self.handle = None
        self.reopened_handle = None
        self.snapshot = None
        self._used = False
        self.post_create_verified = False

    def _ok(self, name, value):
        if not value:
            raise ApiFailure(name, self._last_error())
        return value

    def _open(self, path):
        value = self._fn["CreateFileW"](path, GENERIC_READ, FILE_SHARE_READ,
                                             None, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, None)
        if value in (None, 0, -1, INVALID_HANDLE_VALUE):
            raise ApiFailure("CreateFileW", self._last_error())
        return value

    def _identity(self, handle):
        flags = DWORD()
        self._ok("GetHandleInformation", self._fn["GetHandleInformation"](handle, C.byref(flags)))
        if flags.value & HANDLE_FLAG_INHERIT:
            raise Hold("image handle must be noninheritable")
        basic, extended = BY_HANDLE_FILE_INFORMATION(), FILE_ID_INFO()
        self._ok("GetFileInformationByHandle", self._fn["GetFileInformationByHandle"](handle, C.byref(basic)))
        self._ok("GetFileInformationByHandleEx", self._fn["GetFileInformationByHandleEx"](
            handle, FileIdInfo, C.byref(extended), C.sizeof(extended)))
        if basic.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY:
            raise Hold("image is a directory")
        path = C.create_unicode_buffer(32768)
        length = self._fn["GetFinalPathNameByHandleW"](handle, path, len(path), 0)
        self._ok("GetFinalPathNameByHandleW", length)
        if length >= len(path) or not path.value or length != len(path.value):
            raise Hold("final image path truncated or invalid")
        if _canonical(path.value) != _canonical(WORKER_IMAGE):
            raise Hold("fixed canonical CP316 image path required")
        return Identity(int(extended.VolumeSerialNumber), bytes(extended.FileId.Identifier),
                        (int(basic.nFileSizeHigh) << 32) | int(basic.nFileSizeLow),
                        int(basic.dwVolumeSerialNumber),
                        (int(basic.nFileIndexHigh) << 32) | int(basic.nFileIndexLow),
                        _canonical(path.value))

    def _snapshot(self, handle):
        before = self._identity(handle)
        if before.size != WORKER_BYTES:
            raise Hold("reviewed CP316 image size mismatch")
        self._ok("SetFilePointerEx", self._fn["SetFilePointerEx"](handle, 0, None, 0))
        digest, total = hashlib.sha256(), 0
        buffer = C.create_string_buffer(65536)
        while True:
            count = DWORD()
            self._ok("ReadFile", self._fn["ReadFile"](handle, buffer, len(buffer), C.byref(count), None))
            if count.value > len(buffer) or total + count.value > WORKER_BYTES:
                raise Hold("image read exceeds reviewed bounds")
            if not count.value:
                break
            digest.update(buffer.raw[:count.value])
            total += count.value
        after = self._identity(handle)
        if before != after or total != before.size:
            raise Hold("retained image changed or short read")
        if digest.hexdigest() != WORKER_SHA256:
            raise Hold("reviewed CP316 image SHA-256 mismatch")
        return Snapshot(digest.hexdigest(), after)

    def prepare(self):
        if self._used:
            raise Hold("retained image pin is one-use")
        self._used = True
        self.handle = self._open(WORKER_IMAGE)
        self.snapshot = self._snapshot(self.handle)
        return self.snapshot

    def verify_after_create(self):
        if self.snapshot is None or self.handle is None or self.reopened_handle is not None:
            raise Hold("prepared pin and no unresolved reopen required")
        self.post_create_verified = False
        if self._snapshot(self.handle) != self.snapshot:
            raise Hold("retained image identity changed after creation")
        # Open the handle-derived canonical path again; a process-name query
        # alone establishes neither file identity nor the child's mapped bytes.
        self.reopened_handle = self._open(self.snapshot.identity.final_path)
        try:
            if self._snapshot(self.reopened_handle) != self.snapshot:
                raise Hold("canonical image path now resolves to a different file")
        finally:
            self._ok("CloseHandle", self._fn["CloseHandle"](self.reopened_handle))
            self.reopened_handle = None
        self.post_create_verified = True
        return self.snapshot

    def close(self):
        # Failed CloseHandle keeps the uncertain handle recorded for the owner.
        for attribute in ("reopened_handle", "handle"):
            handle = getattr(self, attribute)
            if handle is not None:
                self._ok("CloseHandle", self._fn["CloseHandle"](handle))
                setattr(self, attribute, None)
        self.post_create_verified = False
