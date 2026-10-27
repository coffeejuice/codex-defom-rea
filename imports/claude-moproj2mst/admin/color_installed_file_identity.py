"""CP288: fixed installed-file identity collection; no execution permission."""
from __future__ import annotations
import ctypes
from ctypes import wintypes as w
import hashlib
import json
from pathlib import Path
from datetime import datetime, timezone
import os

ROOT = Path(__file__).resolve().parents[1]
INSTALLED = r"C:\Program Files\SFTC\DEFORM\v11.0.2"
PINS = (
    (INSTALLED + r"\DEF_GUI_MO_64.exe", 67611136, "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc"),
    (INSTALLED + r"\QtGui4_64.dll", 10859008, "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887"),
    (INSTALLED + r"\QtCore4_64.dll", 3250688, "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160"),
)
GENERIC_READ = 0x80000000
FILE_SHARE_READ = 1
OPEN_EXISTING = 3
FILE_FLAG_OPEN_REPARSE_POINT = 0x00200000
INVALID_HANDLE_VALUE = ctypes.c_void_p(-1).value

class Info(ctypes.Structure):
    _fields_ = [("attributes", w.DWORD), ("creation", w.FILETIME),
                ("access", w.FILETIME), ("write", w.FILETIME),
                ("volume", w.DWORD), ("size_high", w.DWORD),
                ("size_low", w.DWORD), ("links", w.DWORD),
                ("index_high", w.DWORD), ("index_low", w.DWORD)]

class WindowsFiles:
    def __init__(self):
        if os.name != "nt":
            raise OSError("Windows required")
        self.k = ctypes.WinDLL("kernel32", use_last_error=True)
        signatures = {
            "CreateFileW": ([w.LPCWSTR, w.DWORD, w.DWORD, ctypes.c_void_p, w.DWORD, w.DWORD, w.HANDLE], w.HANDLE),
            "GetFileInformationByHandle": ([w.HANDLE, ctypes.POINTER(Info)], w.BOOL),
            "GetFinalPathNameByHandleW": ([w.HANDLE, w.LPWSTR, w.DWORD, w.DWORD], w.DWORD),
            "GetFileType": ([w.HANDLE], w.DWORD),
            "ReadFile": ([w.HANDLE, ctypes.c_void_p, w.DWORD, ctypes.POINTER(w.DWORD), ctypes.c_void_p], w.BOOL),
            "CloseHandle": ([w.HANDLE], w.BOOL),
        }
        for name, (args, result) in signatures.items():
            fn = getattr(self.k, name)
            fn.argtypes, fn.restype = args, result

    def open(self, path):
        handle = self.k.CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, None,
                                    OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, None)
        if handle == INVALID_HANDLE_VALUE:
            raise ctypes.WinError(ctypes.get_last_error())
        return handle

    def snapshot(self, handle):
        info = Info()
        if not self.k.GetFileInformationByHandle(handle, ctypes.byref(info)):
            raise ctypes.WinError(ctypes.get_last_error())
        if self.k.GetFileType(handle) != 1 or info.attributes & (0x400 | 0x10):
            raise ValueError("reparse point, directory or non-disk file rejected")
        buf = ctypes.create_unicode_buffer(32768)
        count = self.k.GetFinalPathNameByHandleW(handle, buf, len(buf), 0)
        if not count or count >= len(buf):
            raise OSError("final-path query failed or truncated")
        return {
            "finalPath": buf.value,
            "volumeSerialNumber": info.volume,
            "fileIndex": (info.index_high << 32) | info.index_low,
            "lastWriteFileTime": (info.write.dwHighDateTime << 32) | info.write.dwLowDateTime,
            "size": (info.size_high << 32) | info.size_low,
            "attributes": info.attributes,
        }

    def digest(self, handle):
        digest = hashlib.sha256()
        buf = ctypes.create_string_buffer(1024 * 1024)
        total = 0
        while True:
            count = w.DWORD()
            if not self.k.ReadFile(handle, buf, len(buf), ctypes.byref(count), None):
                raise ctypes.WinError(ctypes.get_last_error())
            if not count.value:
                return digest.hexdigest(), total
            digest.update(buf.raw[:count.value])
            total += count.value

    def close(self, handle):
        if not self.k.CloseHandle(handle):
            raise ctypes.WinError(ctypes.get_last_error())

def _collect(pins, api=None):
    """Private parameterized seam for disposable-file tests; CLI uses PINS only."""
    receipt = {
        "schema": "cp288-installed-file-identity-v1",
        "startedUtc": datetime.now(timezone.utc).isoformat(),
        "collectorSha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        "status": "HOLD", "original_run_ready": False,
        "runtimeLoadedModuleIdentity": "unknown", "files": [],
        "allHandlesClosed": False, "handlesOpened": 0, "handlesClosed": 0,
        "errors": [],
    }
    handles = []
    candidates = []
    try:
        api = api or WindowsFiles()
        for path, size, expected_digest in pins:
            handle = api.open(path)
            handles.append(handle)
            receipt["handlesOpened"] += 1
            before = api.snapshot(handle)
            if before["finalPath"] != "\\\\?\\" + path:
                raise ValueError("exact final path mismatch: " + path)
            if before["size"] != size:
                raise ValueError("pinned size mismatch: " + path)
            digest, read_size = api.digest(handle)
            after = api.snapshot(handle)
            if before != after:
                raise ValueError("identity changed while reading: " + path)
            if read_size != size or digest != expected_digest:
                raise ValueError("pinned content mismatch: " + path)
            candidates.append({"path": path, "sha256": digest, "before": before, "after": after})
        # All handles stay open through the collective final identity validation.
        for handle, candidate in zip(handles, candidates):
            final = api.snapshot(handle)
            if final != candidate["before"]:
                raise ValueError("identity changed before collective validation")
            candidate["collectiveFinal"] = final
    except Exception as exc:
        receipt["errors"].append({"type": type(exc).__name__, "message": str(exc)})
    finally:
        for handle in reversed(handles):
            try:
                api.close(handle)
                receipt["handlesClosed"] += 1
            except Exception as exc:
                receipt["errors"].append({"type": type(exc).__name__, "message": "CloseHandle: " + str(exc)})
        receipt["allHandlesClosed"] = receipt["handlesOpened"] == receipt["handlesClosed"]
    if not receipt["errors"] and len(candidates) == len(pins) and receipt["allHandlesClosed"]:
        receipt["files"] = candidates
        receipt["status"] = "ACQUIRED_STATIC_IDENTITIES_ONLY"
    receipt["finishedUtc"] = datetime.now(timezone.utc).isoformat()
    return receipt

def main():
    import sys
    if len(sys.argv) != 1:
        raise SystemExit("No arguments supported; installed paths and output are fixed.")
    receipt = _collect(PINS)
    output = ROOT / "analysis/cp288-installed-file-identities.json"
    output.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"status": receipt["status"], "allHandlesClosed": receipt["allHandlesClosed"],
                      "receipt": str(output), "errors": receipt["errors"]}))
    return 0 if receipt["status"] == "ACQUIRED_STATIC_IDENTITIES_ONLY" else 1

if __name__ == "__main__":
    raise SystemExit(main())
