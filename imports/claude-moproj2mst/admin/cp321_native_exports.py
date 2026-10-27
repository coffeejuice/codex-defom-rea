"""CP321 canonical Win64 ABI declarations; live construction remains HARD HOLD.

Importing this module loads no DLL and calls no Win32 export. The injected
address binder is for declaration/isolation tests, not permission to execute.
Legacy CP317/319/320/318 owners still require reviewed integration; this module
does not add their fake markers or change their ownership/error policies.
"""
from __future__ import annotations

import ctypes as C
from dataclasses import dataclass
import sys
from types import MappingProxyType

NATIVE_ENTRY = "HARD HOLD"
DWORD, BOOL, WORD, BYTE = C.c_uint32, C.c_int32, C.c_uint16, C.c_ubyte
HANDLE, PVOID, SIZE_T, ULONG_PTR = C.c_void_p, C.c_void_p, C.c_size_t, C.c_size_t
LPDWORD, LPHANDLE = C.POINTER(DWORD), C.POINTER(HANDLE)
LPWSTR, LPCWSTR = C.POINTER(C.c_wchar), C.c_wchar_p


class SECURITY_ATTRIBUTES(C.Structure):
    _fields_ = [("nLength", DWORD), ("lpSecurityDescriptor", PVOID), ("bInheritHandle", BOOL)]


class _Offsets(C.Structure):
    _fields_ = [("Offset", DWORD), ("OffsetHigh", DWORD)]


class _Location(C.Union):
    _fields_ = [("offsets", _Offsets), ("Pointer", PVOID)]


class OVERLAPPED(C.Structure):
    _fields_ = [("Internal", ULONG_PTR), ("InternalHigh", ULONG_PTR),
                ("location", _Location), ("hEvent", HANDLE)]


class SID_AND_ATTRIBUTES(C.Structure):
    _fields_ = [("Sid", PVOID), ("Attributes", DWORD)]


class SECURITY_DESCRIPTOR(C.Structure):
    _fields_ = [("Revision", BYTE), ("Sbz1", BYTE), ("Control", WORD),
                ("Owner", PVOID), ("Group", PVOID), ("Sacl", PVOID), ("Dacl", PVOID)]


class FILETIME(C.Structure):
    _fields_ = [("dwLowDateTime", DWORD), ("dwHighDateTime", DWORD)]


class BY_HANDLE_FILE_INFORMATION(C.Structure):
    _fields_ = [("dwFileAttributes", DWORD), ("ftCreationTime", FILETIME),
                ("ftLastAccessTime", FILETIME), ("ftLastWriteTime", FILETIME)] + [
                    (name, DWORD) for name in ("dwVolumeSerialNumber", "nFileSizeHigh",
                    "nFileSizeLow", "nNumberOfLinks", "nFileIndexHigh", "nFileIndexLow")]


class FILE_ID_128(C.Structure):
    _fields_ = [("Identifier", BYTE * 16)]


class FILE_ID_INFO(C.Structure):
    _fields_ = [("VolumeSerialNumber", C.c_uint64), ("FileId", FILE_ID_128)]


class STARTUPINFOW(C.Structure):
    _fields_ = [("cb", DWORD), ("lpReserved", LPWSTR), ("lpDesktop", LPWSTR),
                ("lpTitle", LPWSTR)] + [(name, DWORD) for name in (
                    "dwX", "dwY", "dwXSize", "dwYSize", "dwXCountChars",
                    "dwYCountChars", "dwFillAttribute", "dwFlags")] + [
                ("wShowWindow", WORD), ("cbReserved2", WORD),
                ("lpReserved2", C.POINTER(BYTE)), ("hStdInput", HANDLE),
                ("hStdOutput", HANDLE), ("hStdError", HANDLE)]


class STARTUPINFOEXW(C.Structure):
    _fields_ = [("StartupInfo", STARTUPINFOW), ("lpAttributeList", PVOID)]


class PROCESS_INFORMATION(C.Structure):
    _fields_ = [("hProcess", HANDLE), ("hThread", HANDLE),
                ("dwProcessId", DWORD), ("dwThreadId", DWORD)]


class BASIC_LIMIT(C.Structure):
    _fields_ = [("PerProcessUserTimeLimit", C.c_int64), ("PerJobUserTimeLimit", C.c_int64),
                ("LimitFlags", DWORD), ("MinimumWorkingSetSize", SIZE_T),
                ("MaximumWorkingSetSize", SIZE_T), ("ActiveProcessLimit", DWORD),
                ("Affinity", ULONG_PTR), ("PriorityClass", DWORD), ("SchedulingClass", DWORD)]


class IO_COUNTERS(C.Structure):
    _fields_ = [(name, C.c_uint64) for name in ("ReadOperationCount", "WriteOperationCount",
                "OtherOperationCount", "ReadTransferCount", "WriteTransferCount", "OtherTransferCount")]


class EXTENDED_LIMIT(C.Structure):
    _fields_ = [("BasicLimitInformation", BASIC_LIMIT), ("IoInfo", IO_COUNTERS)] + [
        (name, SIZE_T) for name in ("ProcessMemoryLimit", "JobMemoryLimit",
                                 "PeakProcessMemoryUsed", "PeakJobMemoryUsed")]


class BASIC_ACCOUNTING(C.Structure):
    _fields_ = [(name, C.c_int64) for name in ("TotalUserTime", "TotalKernelTime",
        "ThisPeriodTotalUserTime", "ThisPeriodTotalKernelTime")] + [
        (name, DWORD) for name in ("TotalPageFaultCount", "TotalProcesses",
                                 "ActiveProcesses", "TotalTerminatedProcesses")]


SA, OV = C.POINTER(SECURITY_ATTRIBUTES), C.POINTER(OVERLAPPED)


@dataclass(frozen=True)
class Export:
    dll: str
    argtypes: tuple
    restype: object


# Only explicit system-library membership; never use a global DLL namespace.
_KERNEL = {
    "CreateNamedPipeW": ((LPCWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, SA), HANDLE),
    "CreateFileW": ((LPCWSTR, DWORD, DWORD, SA, DWORD, DWORD, HANDLE), HANDLE),
    "ConnectNamedPipe": ((HANDLE, OV), BOOL),
    "ReadFile": ((HANDLE, PVOID, DWORD, LPDWORD, OV), BOOL),
    "WriteFile": ((HANDLE, PVOID, DWORD, LPDWORD, OV), BOOL),
    "GetOverlappedResultEx": ((HANDLE, OV, LPDWORD, DWORD, BOOL), BOOL),
    "CancelIoEx": ((HANDLE, OV), BOOL),
    "CreateEventW": ((SA, BOOL, BOOL, LPCWSTR), HANDLE),
    "CloseHandle": ((HANDLE,), BOOL),
    "GetCurrentThread": ((), HANDLE), "GetCurrentProcess": ((), HANDLE),
    "GetHandleInformation": ((HANDLE, LPDWORD), BOOL),
    "SetHandleInformation": ((HANDLE, DWORD, DWORD), BOOL),
    "GetFileInformationByHandle": ((HANDLE, C.POINTER(BY_HANDLE_FILE_INFORMATION)), BOOL),
    "GetFileInformationByHandleEx": ((HANDLE, C.c_int32, PVOID, DWORD), BOOL),
    "GetFinalPathNameByHandleW": ((HANDLE, LPWSTR, DWORD, DWORD), DWORD),
    "SetFilePointerEx": ((HANDLE, C.c_int64, C.POINTER(C.c_int64), DWORD), BOOL),
    "CreateJobObjectW": ((SA, LPCWSTR), HANDLE),
    "SetInformationJobObject": ((HANDLE, C.c_int32, PVOID, DWORD), BOOL),
    "QueryInformationJobObject": ((HANDLE, C.c_int32, PVOID, DWORD, LPDWORD), BOOL),
    "CreatePipe": ((LPHANDLE, LPHANDLE, SA, DWORD), BOOL),
    "InitializeProcThreadAttributeList": ((PVOID, DWORD, DWORD, C.POINTER(SIZE_T)), BOOL),
    "UpdateProcThreadAttribute": ((PVOID, DWORD, ULONG_PTR, PVOID, SIZE_T, PVOID, C.POINTER(SIZE_T)), BOOL),
    "DeleteProcThreadAttributeList": ((PVOID,), None),
    "CreateProcessW": ((LPCWSTR, LPWSTR, SA, SA, BOOL, DWORD, PVOID, LPCWSTR,
                         C.POINTER(STARTUPINFOW), C.POINTER(PROCESS_INFORMATION)), BOOL),
    "GetProcessId": ((HANDLE,), DWORD),
    "QueryFullProcessImageNameW": ((HANDLE, DWORD, LPWSTR, LPDWORD), BOOL),
    "IsProcessInJob": ((HANDLE, HANDLE, C.POINTER(BOOL)), BOOL),
    "DuplicateHandle": ((HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD), BOOL),
    "TerminateJobObject": ((HANDLE, DWORD), BOOL),
    "TerminateProcess": ((HANDLE, DWORD), BOOL),
    "WaitForSingleObject": ((HANDLE, DWORD), DWORD),
    "GetProcessTimes": ((HANDLE,) + (C.POINTER(FILETIME),) * 4, BOOL),
    "GetTickCount64": ((), C.c_uint64),
    "ResumeThread": ((HANDLE,), DWORD),
    "GetExitCodeProcess": ((HANDLE, LPDWORD), BOOL),
}
_ADVAPI = {
    "OpenThreadToken": ((HANDLE, DWORD, BOOL, LPHANDLE), BOOL),
    "OpenProcessToken": ((HANDLE, DWORD, LPHANDLE), BOOL),
    "GetTokenInformation": ((HANDLE, C.c_int32, PVOID, DWORD, LPDWORD), BOOL),
    "IsValidSid": ((PVOID,), BOOL), "GetLengthSid": ((PVOID,), DWORD),
    "CopySid": ((DWORD, PVOID, PVOID), BOOL),
    "InitializeAcl": ((PVOID, DWORD, DWORD), BOOL),
    "AddAccessAllowedAceEx": ((PVOID, DWORD, DWORD, DWORD, PVOID), BOOL),
    "InitializeSecurityDescriptor": ((PVOID, DWORD), BOOL),
    "SetSecurityDescriptorDacl": ((PVOID, BOOL, PVOID, BOOL), BOOL),
    "IsValidSecurityDescriptor": ((PVOID,), BOOL),
}
EXPORTS = MappingProxyType({name: Export(dll, args, result)
    for dll, declarations in (("kernel32.dll", _KERNEL), ("advapi32.dll", _ADVAPI))
    for name, (args, result) in declarations.items()})
SIGNATURES = MappingProxyType({name: (spec.argtypes, spec.restype) for name, spec in EXPORTS.items()})


class Hold(RuntimeError):
    pass


def require_win64(*, platform=None, pointer_size=None, machine=None):
    """Reject unsupported ABI before any caller resolves a DLL/export address."""
    if platform is None:
        platform = sys.platform
    if pointer_size is None:
        pointer_size = C.sizeof(PVOID)
    if machine is None:
        import platform as platform_module
        machine = platform_module.machine()
    if platform != "win32" or pointer_size != 8 or machine.lower() not in ("amd64", "x86_64"):
        raise Hold("CP321 requires Windows x64")


class BoundExports:
    """Retain address owners, fresh function wrappers and ctypes last-error hook.

    Addresses must remain valid for this object's lifetime. There is no fallback
    lookup, implicit loading, fake marker, or native ownership/cleanup inference.
    """
    def __init__(self, functions, owners, last_error):
        self._functions = MappingProxyType(dict(functions))
        self._owners = tuple(owners)
        self.last_error = last_error

    def __getattr__(self, name):
        try:
            return self._functions[name]
        except KeyError:
            raise AttributeError(name) from None


def bind_injected_exports(*, resolve_address, prototype_factory, last_error, owners=(),
                          platform=None, pointer_size=None, machine=None):
    """Build fresh wrappers from injected addresses without mutating DLL caches.

    The reviewed native implementation would supply WINFUNCTYPE and
    ctypes.get_last_error; tests supply inert equivalents. Every prototype uses
    use_last_error=True. This seam does not authorize or load native libraries.
    """
    require_win64(platform=platform, pointer_size=pointer_size, machine=machine)
    if not all(callable(value) for value in (resolve_address, prototype_factory, last_error)):
        raise TypeError("address resolver, prototype factory and last-error hook required")
    functions = {}
    for name, spec in EXPORTS.items():
        address = resolve_address(spec.dll, name)
        if type(address) is not int or not 0 < address < (1 << 64):
            raise Hold(f"missing/invalid export address: {spec.dll}!{name}")
        function = prototype_factory(spec.restype, *spec.argtypes, use_last_error=True)(address)
        if not callable(function) or any(function is other for other in functions.values()):
            raise Hold("prototype factory must return independent callable wrappers")
        functions[name] = function
    return BoundExports(functions, owners, last_error)


def create_live_exports(*_args, **_kwargs):
    # Keep this unconditional: ABI declarations do not establish integrated safety.
    raise Hold("CP321 native entry HARD HOLD: end-to-end owner/cleanup review pending")
