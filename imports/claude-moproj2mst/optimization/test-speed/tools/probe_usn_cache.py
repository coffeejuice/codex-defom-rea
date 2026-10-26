"""Probe per-file NTFS USN access and mutation detection on an isolated scratch file.

This script never opens fixture roots. It creates one temporary directory under
optimization/test-speed/analysis, performs a same-size write and rename there,
and removes only that directory. It does not create or modify a USN journal.
"""
from __future__ import annotations

import argparse
import ctypes
from ctypes import wintypes
import json
import os
from pathlib import Path
import shutil
import tempfile
import time


ROOT = Path(__file__).resolve().parents[1]
ANALYSIS = ROOT / "analysis"
FSCTL_READ_FILE_USN_DATA = 0x000900EB  # CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 58, METHOD_NEITHER, FILE_ANY_ACCESS)
FSCTL_QUERY_USN_JOURNAL = 0x000900F4  # CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)
FILE_ID_INFO_CLASS = 18
FILE_READ_ATTRIBUTES = 0x0080
FILE_SHARE_ALL = 0x00000001 | 0x00000002 | 0x00000004
OPEN_EXISTING = 3
FILE_FLAG_BACKUP_SEMANTICS = 0x02000000
INVALID_HANDLE_VALUE = ctypes.c_void_p(-1).value


def _windows_apis():
    if os.name != "nt":
        raise RuntimeError("This probe requires Windows")
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.CreateFileW.argtypes = [wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
                                   wintypes.LPVOID, wintypes.DWORD, wintypes.DWORD,
                                   wintypes.HANDLE]
    kernel.CreateFileW.restype = wintypes.HANDLE
    kernel.DeviceIoControl.argtypes = [wintypes.HANDLE, wintypes.DWORD, wintypes.LPVOID,
                                       wintypes.DWORD, wintypes.LPVOID, wintypes.DWORD,
                                       ctypes.POINTER(wintypes.DWORD), wintypes.LPVOID]
    kernel.DeviceIoControl.restype = wintypes.BOOL
    kernel.GetFileInformationByHandleEx.argtypes = [wintypes.HANDLE, ctypes.c_int,
                                                    wintypes.LPVOID, wintypes.DWORD]
    kernel.GetFileInformationByHandleEx.restype = wintypes.BOOL
    kernel.CloseHandle.argtypes = [wintypes.HANDLE]
    kernel.CloseHandle.restype = wintypes.BOOL
    return kernel


def _open(kernel, path: str, access: int, flags: int = 0):
    handle = kernel.CreateFileW(path, access, FILE_SHARE_ALL, None, OPEN_EXISTING,
                                flags, None)
    if handle == INVALID_HANDLE_VALUE:
        code = ctypes.get_last_error()
        raise OSError(code, ctypes.FormatError(code), path)
    return handle


def _file_identity(kernel, path: Path) -> dict:
    handle = _open(kernel, str(path), FILE_READ_ATTRIBUTES)
    try:
        data = (ctypes.c_ubyte * 24)()
        if not kernel.GetFileInformationByHandleEx(handle, FILE_ID_INFO_CLASS,
                                                   data, ctypes.sizeof(data)):
            code = ctypes.get_last_error()
            raise OSError(code, ctypes.FormatError(code), str(path))
        raw = bytes(data)
        return {"volume_serial": int.from_bytes(raw[:8], "little"),
                "file_id": raw[8:24].hex()}
    finally:
        kernel.CloseHandle(handle)


def _file_usn(kernel, path: Path) -> dict:
    handle = _open(kernel, str(path), FILE_READ_ATTRIBUTES)
    try:
        # READ_FILE_USN_DATA: accept records v2-v3; output starts with USN_RECORD_COMMON_HEADER.
        request = (ctypes.c_ushort * 2)(2, 3)
        output = (ctypes.c_ubyte * 4096)()
        returned = wintypes.DWORD()
        ok = kernel.DeviceIoControl(handle, FSCTL_READ_FILE_USN_DATA, request,
                                    ctypes.sizeof(request), output, ctypes.sizeof(output),
                                    ctypes.byref(returned), None)
        if not ok:
            code = ctypes.get_last_error()
            return {"ok": False, "winerror": code, "message": ctypes.FormatError(code)}
        raw = bytes(output[:returned.value])
        if len(raw) < 8:
            return {"ok": False, "error": "short USN record", "bytes_returned": len(raw)}
        major = int.from_bytes(raw[4:6], "little")
        if major == 2 and len(raw) >= 32:
            usn = int.from_bytes(raw[24:32], "little", signed=True)
        elif major == 3 and len(raw) >= 40:
            usn = int.from_bytes(raw[32:40], "little", signed=True)
        else:
            return {"ok": False, "error": "unsupported USN record", "major": major,
                    "bytes_returned": len(raw)}
        return {"ok": True, "usn": usn, "major": major,
                "record_length": int.from_bytes(raw[:4], "little")}
    finally:
        kernel.CloseHandle(handle)


def _journal_query(kernel, path: Path) -> dict:
    drive = path.drive
    if not drive:
        return {"ok": False, "error": "path has no drive-letter volume"}
    volume_path = "\\\\.\\" + drive
    try:
        handle = _open(kernel, volume_path, 0)
    except OSError as exc:
        return {"ok": False, "winerror": exc.errno, "message": str(exc),
                "volume": volume_path}
    try:
        output = (ctypes.c_ubyte * 64)()
        returned = wintypes.DWORD()
        ok = kernel.DeviceIoControl(handle, FSCTL_QUERY_USN_JOURNAL, None, 0,
                                    output, ctypes.sizeof(output), ctypes.byref(returned), None)
        if not ok:
            code = ctypes.get_last_error()
            return {"ok": False, "winerror": code, "message": ctypes.FormatError(code),
                    "volume": volume_path}
        raw = bytes(output[:returned.value])
        if len(raw) < 56:
            return {"ok": False, "error": "short journal response", "bytes_returned": len(raw)}
        return {"ok": True, "volume": volume_path,
                "journal_id": int.from_bytes(raw[:8], "little"),
                "first_usn": int.from_bytes(raw[8:16], "little", signed=True),
                "next_usn": int.from_bytes(raw[16:24], "little", signed=True),
                "lowest_valid_usn": int.from_bytes(raw[24:32], "little", signed=True)}
    finally:
        kernel.CloseHandle(handle)


def run() -> dict:
    ANALYSIS.mkdir(parents=True, exist_ok=True)
    scratch = Path(tempfile.mkdtemp(prefix="usn-probe-", dir=ANALYSIS)).resolve()
    if not scratch.is_relative_to(ANALYSIS.resolve()):
        raise RuntimeError(f"Scratch escaped analysis directory: {scratch}")
    result = {"scratch_parent": str(ANALYSIS.resolve()), "cleanup": "pending"}
    try:
        kernel = _windows_apis()
        file_a = scratch / "probe.bin"
        file_b = scratch / "renamed.bin"
        payload = b"same-size USN probe payload\n"
        file_a.write_bytes(payload)
        initial_mtime = file_a.stat().st_mtime_ns
        result["filesystem"] = {"scratch": str(scratch), "volume": file_a.drive}
        result["journal_before"] = _journal_query(kernel, file_a)
        result["identity_before"] = _file_identity(kernel, file_a)
        result["usn_before"] = _file_usn(kernel, file_a)

        replacement = bytearray(payload)
        replacement[-2] ^= 1
        file_a.write_bytes(replacement)
        os.utime(file_a, ns=(initial_mtime, initial_mtime))
        result["same_size_restored_mtime"] = {
            "size_before": len(payload), "size_after": file_a.stat().st_size,
            "mtime_ns_before": initial_mtime, "mtime_ns_after": file_a.stat().st_mtime_ns,
            "identity_after": _file_identity(kernel, file_a),
            "usn_after": _file_usn(kernel, file_a),
        }

        file_a.rename(file_b)
        result["rename"] = {
            "old_path_exists": file_a.exists(), "new_path_exists": file_b.exists(),
            "identity_after": _file_identity(kernel, file_b),
            "usn_after": _file_usn(kernel, file_b),
        }
        result["journal_after"] = _journal_query(kernel, file_b)
        result["probe_time_utc"] = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
        before = result["usn_before"]
        after = result["same_size_restored_mtime"]["usn_after"]
        result["same_size_change_detected"] = bool(
            before.get("ok") and after.get("ok") and before["usn"] != after["usn"])
        result["status"] = ("available" if result["journal_before"].get("ok") and
                             before.get("ok") and before.get("usn") not in (None, 0) and
                             after.get("ok") and after.get("usn") not in (None, 0)
                             else "unsupported-or-unavailable")
        return result
    finally:
        shutil.rmtree(scratch)
        result["cleanup"] = "removed own scratch directory"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.parse_args()
    try:
        print(json.dumps(run(), indent=2))
        return 0
    except OSError as exc:
        print(json.dumps({"status": "probe-error", "winerror": exc.errno,
                          "message": str(exc)}, indent=2))
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
