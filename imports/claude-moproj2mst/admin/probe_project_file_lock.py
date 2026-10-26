import ctypes
import hashlib
import json
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1] / "build" / "probe-project-file-lock"
ROOT.mkdir(parents=True, exist_ok=True)
K32 = ctypes.WinDLL("kernel32", use_last_error=True)
HANDLE = ctypes.c_void_p
DWORD = ctypes.c_uint32
BOOL = ctypes.c_int
INVALID = ctypes.c_void_p(-1).value
GENERIC_READ = 0x80000000
GENERIC_WRITE = 0x40000000
FILE_SHARE_READ = 1
FILE_SHARE_WRITE = 2
OPEN_EXISTING = 3
OPEN_ALWAYS = 4
FILE_ATTRIBUTE_NORMAL = 0x80
LOCKFILE_EXCLUSIVE_LOCK = 0x2
LOCKFILE_FAIL_IMMEDIATELY = 0x1

class OVERLAPPED(ctypes.Structure):
    _fields_ = [
        ("Internal", ctypes.c_size_t),
        ("InternalHigh", ctypes.c_size_t),
        ("Offset", DWORD),
        ("OffsetHigh", DWORD),
        ("hEvent", HANDLE),
    ]

K32.CreateFileW.argtypes = [ctypes.c_wchar_p, DWORD, DWORD, ctypes.c_void_p,
                            DWORD, DWORD, HANDLE]
K32.CreateFileW.restype = HANDLE
K32.LockFileEx.argtypes = [HANDLE, DWORD, DWORD, DWORD, DWORD, ctypes.POINTER(OVERLAPPED)]
K32.LockFileEx.restype = BOOL
K32.UnlockFileEx.argtypes = [HANDLE, DWORD, DWORD, DWORD, ctypes.POINTER(OVERLAPPED)]
K32.UnlockFileEx.restype = BOOL
K32.SetEndOfFile.argtypes = [HANDLE]
K32.SetEndOfFile.restype = BOOL
K32.WriteFile.argtypes = [HANDLE, ctypes.c_void_p, DWORD, ctypes.POINTER(DWORD), ctypes.c_void_p]
K32.WriteFile.restype = BOOL
K32.GetFileSizeEx.argtypes = [HANDLE, ctypes.POINTER(ctypes.c_int64)]
K32.GetFileSizeEx.restype = BOOL
K32.CloseHandle.argtypes = [HANDLE]
K32.CloseHandle.restype = BOOL


def size_of(handle):
    n = ctypes.c_int64()
    ok = bool(K32.GetFileSizeEx(handle, ctypes.byref(n)))
    return {"ok": ok, "size": n.value if ok else None,
            "error": 0 if ok else ctypes.get_last_error()}


def run_one(index):
    path = ROOT / f"locked-project-{index}.moproj"
    original = (b"ORIGINAL-PROJECT-DATA\n" * 2048)  # 45,056 bytes, > test payload
    path.write_bytes(original)
    lock_handle = K32.CreateFileW(str(path), GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE, None,
                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, None)
    if lock_handle == INVALID:
        raise ctypes.WinError(ctypes.get_last_error())
    ov = OVERLAPPED()
    ov.Offset = 0
    lock_ok = bool(K32.LockFileEx(lock_handle,
                                  LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                                  0, 1, 0, ctypes.byref(ov)))
    lock_error = 0 if lock_ok else ctypes.get_last_error()
    if not lock_ok:
        K32.CloseHandle(lock_handle)
        raise ctypes.WinError(lock_error)

    # Match mst::FileDevice: GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
    # OPEN_ALWAYS, followed by SetEndOfFile at initial file pointer 0.
    writer = K32.CreateFileW(str(path), GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE, None,
                             OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, None)
    if writer == INVALID:
        raise ctypes.WinError(ctypes.get_last_error())
    pre_size = size_of(writer)
    set_eof_ok = bool(K32.SetEndOfFile(writer))
    set_eof_error = 0 if set_eof_ok else ctypes.get_last_error()
    post_eof_size = size_of(writer)

    payload = (ctypes.c_char * 20000)(*([b"X"] * 20000))
    wrote = DWORD()
    write_ok = bool(K32.WriteFile(writer, payload, 20000, ctypes.byref(wrote), None))
    write_error = 0 if write_ok else ctypes.get_last_error()
    post_write_size = size_of(writer)
    close_writer_ok = bool(K32.CloseHandle(writer))
    close_writer_error = 0 if close_writer_ok else ctypes.get_last_error()

    unlock_ok = bool(K32.UnlockFileEx(lock_handle, 0, 1, 0, ctypes.byref(ov)))
    unlock_error = 0 if unlock_ok else ctypes.get_last_error()
    close_lock_ok = bool(K32.CloseHandle(lock_handle))
    close_lock_error = 0 if close_lock_ok else ctypes.get_last_error()
    final = path.read_bytes()
    return {
        "path": str(path),
        "initial_length": len(original),
        "initial_sha256": hashlib.sha256(original).hexdigest(),
        "lock": {"offset": 0, "length": 1, "succeeded": lock_ok, "error": lock_error},
        "file_size_before_set_eof": pre_size,
        "set_eof_succeeded": set_eof_ok,
        "set_eof_error": set_eof_error,
        "file_size_after_set_eof": post_eof_size,
        "write_length": 20000,
        "write_succeeded": write_ok,
        "write_error": write_error,
        "bytes_written": wrote.value,
        "file_size_after_write": post_write_size,
        "writer_close": {"succeeded": close_writer_ok, "error": close_writer_error},
        "unlock": {"succeeded": unlock_ok, "error": unlock_error},
        "lock_close": {"succeeded": close_lock_ok, "error": close_lock_error},
        "final_length": len(final),
        "final_sha256": hashlib.sha256(final).hexdigest(),
        "final_equals_original": final == original,
        "final_empty": not final,
        "final_prefix_of_payload": final == b"X" * len(final),
    }


results = {
    "scope": "disposable Windows file/byte-lock API probe only; no converter, original app, or source edits",
    "root": str(ROOT),
    "runs": [run_one(i) for i in range(1, 4)],
}
out = ROOT / "raw-results.json"
out.write_text(json.dumps(results, indent=2) + "\n", encoding="utf-8")
print(json.dumps(results, indent=2))
print(f"raw_results={out}")
