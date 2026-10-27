"""CP327 native-capable retained CP316 image pin; worker run HARD HOLD.

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

import cp321_native_exports as abi

NATIVE_ENTRY = "HARD HOLD"
WORKER_IMAGE = r"C:\PROBLEM\s316\worker.exe"
WORKER_BYTES = 104960
WORKER_SHA256 = "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985"
DWORD, BOOL, HANDLE = abi.DWORD, abi.BOOL, abi.HANDLE
BY_HANDLE_FILE_INFORMATION, FILE_ID_INFO = abi.BY_HANDLE_FILE_INFORMATION, abi.FILE_ID_INFO
INVALID_HANDLE_VALUE = C.c_void_p(-1).value
GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING = 0x80000000, 1, 3
FILE_ATTRIBUTE_NORMAL, FILE_ATTRIBUTE_DIRECTORY = 0x80, 0x10
HANDLE_FLAG_INHERIT, FileIdInfo = 1, 18
REQUIRED_EXPORTS = (
    "CreateFileW", "GetHandleInformation", "GetFileInformationByHandle",
    "GetFileInformationByHandleEx", "GetFinalPathNameByHandleW",
    "SetFilePointerEx", "ReadFile", "CloseHandle",
)
SIGNATURES = {name: abi.SIGNATURES[name] for name in REQUIRED_EXPORTS}


Hold = abi.Hold


class ApiFailure(Hold):
    def __init__(self, operation, code):
        self.operation, self.code = operation, code
        super().__init__(f"{operation}: Win32 error {code}")


def open_live_pin(*_args, **_kwargs):
    raise Hold("CP327 worker run HARD HOLD: creator/cleanup integration remains unapproved")


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


class NativeImagePin:
    """One-use owner of a real or injected CP325 BoundExports image handle.

    Construction binds references only; prepare explicitly opens and hashes.
    Caller must call verify_after_create immediately after successful creation,
    while CREATE_SUSPENDED still holds, and gate resume on success. Keep this
    owner until child/job cleanup completes. This module cannot establish the
    suspension state and never creates, resumes or attests loaded child bytes.
    No destructor closes handles; failed/exceptional closes retain ownership.
    """
    def __init__(self, exports):
        self.exports = exports  # Retain CP325 functions and their DLL owners.
        self._fn = {name: exports.fn(name) for name in REQUIRED_EXPORTS}
        self._last_error = exports.last_error
        if not callable(self._last_error) or not all(map(callable, self._fn.values())):
            raise TypeError("CP325 BoundExports functions and last_error required")
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
        if isinstance(value, C.c_void_p):
            value = value.value
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
        self.post_create_verified = False
        # Failed CloseHandle keeps the uncertain handle recorded for the owner.
        for attribute in ("reopened_handle", "handle"):
            handle = getattr(self, attribute)
            if handle is not None:
                self._ok("CloseHandle", self._fn["CloseHandle"](handle))
                setattr(self, attribute, None)
        self.post_create_verified = False
