"""Focused bounded launch of the one fixed benign CP282 native child."""
from __future__ import annotations

import ctypes as C
import hashlib
import json
from pathlib import Path
import subprocess

import build_native_color_entry_fixture as build

OUT = build.ROOT / "build/cp282-native-startup-fixture"
FIXTURE = OUT / "native_color_entry_fixture.exe"
WAIT_OBJECT_0, WAIT_TIMEOUT = 0, 258


class STARTUPINFO(C.Structure):
    _fields_ = [("cb", C.c_uint32), ("lpReserved", C.c_void_p), ("lpDesktop", C.c_void_p),
        ("lpTitle", C.c_void_p), ("dwX", C.c_uint32), ("dwY", C.c_uint32),
        ("dwXSize", C.c_uint32), ("dwYSize", C.c_uint32), ("dwXCountChars", C.c_uint32),
        ("dwYCountChars", C.c_uint32), ("dwFillAttribute", C.c_uint32),
        ("dwFlags", C.c_uint32), ("wShowWindow", C.c_uint16), ("cbReserved2", C.c_uint16),
        ("lpReserved2", C.c_void_p), ("hStdInput", C.c_void_p), ("hStdOutput", C.c_void_p),
        ("hStdError", C.c_void_p)]

class PROCESS_INFORMATION(C.Structure):
    _fields_ = [("hProcess", C.c_void_p), ("hThread", C.c_void_p),
                ("dwProcessId", C.c_uint32), ("dwThreadId", C.c_uint32)]


def main() -> int:
    if C.sizeof(C.c_void_p) != 8 or not FIXTURE.is_file():
        raise SystemExit("HOLD: requires built x64 fixed fixture")
    data = FIXTURE.read_bytes()
    pe = build.parse_pe(data)
    expected = json.loads((OUT / "fixture.json").read_text(encoding="utf-8"))
    if hashlib.sha256(data).hexdigest() != expected["sha256"]:
        raise SystemExit("fixture hash differs from build receipt")
    if pe["entry_rva"] != expected["entry_rva"] or pe["entry_bytes"] != expected["entry_bytes"]:
        raise SystemExit("read-only PE export/RVA/byte verification differs")
    k = C.WinDLL("kernel32", use_last_error=True)
    handle = C.c_void_p
    create = k.CreateProcessW
    create.argtypes = [C.c_wchar_p, C.c_wchar_p, C.c_void_p, C.c_void_p, C.c_int,
                       C.c_uint32, C.c_void_p, C.c_wchar_p, C.POINTER(STARTUPINFO),
                       C.POINTER(PROCESS_INFORMATION)]
    create.restype = C.c_int
    wait = k.WaitForSingleObject
    wait.argtypes, wait.restype = [handle, C.c_uint32], C.c_uint32
    terminate = k.TerminateProcess
    terminate.argtypes, terminate.restype = [handle, C.c_uint32], C.c_int
    getexit = k.GetExitCodeProcess
    getexit.argtypes, getexit.restype = [handle, C.POINTER(C.c_uint32)], C.c_int
    close = k.CloseHandle
    close.argtypes, close.restype = [handle], C.c_int

    def launch(args):
        si, pi = STARTUPINFO(), PROCESS_INFORMATION()
        si.cb = C.sizeof(si)
        command = C.create_unicode_buffer(subprocess.list2cmdline([str(FIXTURE), *args]))
        if not create(str(FIXTURE), command, None, None, False, 0x08000000,
                      None, str(OUT), C.byref(si), C.byref(pi)):
            raise C.WinError(C.get_last_error())
        return pi

    result = {"schema": "cp282-native-color-entry-fixture-test-v2",
        "child_image": str(FIXTURE), "entry_rva": hex(pe["entry_rva"]),
        "entry_bytes": pe["entry_bytes"], "normal_exit": False,
        "timeout_cleanup": {"attempted": False, "terminate_succeeded": None,
                            "wait_signaled": None, "exit_code": None, "cleanup_complete": False}}
    pi = launch([])
    result.update(pid=pi.dwProcessId, primary_tid=pi.dwThreadId)
    close_errors = []
    try:
        status = wait(pi.hProcess, 5000)
        if status != WAIT_OBJECT_0: raise RuntimeError(f"normal child wait status {status:#x}")
        code = C.c_uint32()
        if not getexit(pi.hProcess, C.byref(code)): raise C.WinError(C.get_last_error())
        result["exit_code"] = code.value
        result["normal_exit"] = code.value == 0
        if not result["normal_exit"]: raise RuntimeError(f"fixed child exit code {code.value}")
    finally:
        if not close(pi.hThread): close_errors.append({"handle": "primary-thread", "error": C.get_last_error()})
        if not close(pi.hProcess): close_errors.append({"handle": "owned-process", "error": C.get_last_error()})

    # Fixed negative case: only this executable receives its bounded-hold mode.
    hold = launch(["--bounded-hold"])
    timeout = result["timeout_cleanup"]
    timeout.update(attempted=True, pid=hold.dwProcessId)
    try:
        first_wait = wait(hold.hProcess, 100)
        timeout["first_wait_timed_out"] = first_wait == WAIT_TIMEOUT
        if first_wait != WAIT_TIMEOUT:
            raise RuntimeError(f"bounded-hold child did not take timeout path: {first_wait:#x}")
        timeout["terminate_succeeded"] = bool(terminate(hold.hProcess, 0x282))
        if not timeout["terminate_succeeded"]:
            timeout["terminate_error"] = C.get_last_error()
        second_wait = wait(hold.hProcess, 2000)
        timeout["wait_status"] = hex(second_wait)
        timeout["wait_signaled"] = second_wait == WAIT_OBJECT_0
        if timeout["wait_signaled"]:
            code = C.c_uint32()
            if getexit(hold.hProcess, C.byref(code)):
                timeout["exit_code"] = code.value
            else:
                timeout["exit_code_error"] = C.get_last_error()
        timeout["cleanup_complete"] = bool(timeout["terminate_succeeded"] and timeout["wait_signaled"]
                                             and timeout["exit_code"] == 0x282)
    finally:
        if not close(hold.hThread): close_errors.append({"handle": "timeout-primary-thread", "error": C.get_last_error()})
        if not close(hold.hProcess): close_errors.append({"handle": "timeout-owned-process", "error": C.get_last_error()})
    result["handle_close_errors"] = close_errors
    result["passed"] = bool(result["normal_exit"] and timeout["cleanup_complete"] and not close_errors)
    (OUT / "test.json").write_text(json.dumps(result, indent=2)+"\n", encoding="utf-8")
    print(json.dumps(result))
    return 0 if result["passed"] else 1


if __name__ == "__main__": raise SystemExit(main())
