"""Launch the original Windows application suspended for passive startup probes.

The caller attaches REA to the emitted PID and creates the fresh readiness gate
after installing its hooks. This helper then resumes the original primary
thread exactly once. No executable bytes or application memory are modified.
"""
from __future__ import annotations

import argparse
import ctypes
from ctypes import wintypes
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from capture_oracle import digest


class SECURITY_ATTRIBUTES(ctypes.Structure):
    _fields_ = [
        ("nLength", wintypes.DWORD),
        ("lpSecurityDescriptor", wintypes.LPVOID),
        ("bInheritHandle", wintypes.BOOL),
    ]


class STARTUPINFOW(ctypes.Structure):
    _fields_ = [
        ("cb", wintypes.DWORD),
        ("lpReserved", wintypes.LPWSTR),
        ("lpDesktop", wintypes.LPWSTR),
        ("lpTitle", wintypes.LPWSTR),
        ("dwX", wintypes.DWORD),
        ("dwY", wintypes.DWORD),
        ("dwXSize", wintypes.DWORD),
        ("dwYSize", wintypes.DWORD),
        ("dwXCountChars", wintypes.DWORD),
        ("dwYCountChars", wintypes.DWORD),
        ("dwFillAttribute", wintypes.DWORD),
        ("dwFlags", wintypes.DWORD),
        ("wShowWindow", wintypes.WORD),
        ("cbReserved2", wintypes.WORD),
        ("lpReserved2", ctypes.POINTER(wintypes.BYTE)),
        ("hStdInput", wintypes.HANDLE),
        ("hStdOutput", wintypes.HANDLE),
        ("hStdError", wintypes.HANDLE),
    ]


class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [
        ("hProcess", wintypes.HANDLE),
        ("hThread", wintypes.HANDLE),
        ("dwProcessId", wintypes.DWORD),
        ("dwThreadId", wintypes.DWORD),
    ]


def windows_api():
    if os.name != "nt" or ctypes.sizeof(ctypes.c_void_p) != 8:
        raise RuntimeError("this startup capture helper requires 64-bit Windows Python")
    layout = {
        "SECURITY_ATTRIBUTES": (ctypes.sizeof(SECURITY_ATTRIBUTES), 24),
        "STARTUPINFOW": (ctypes.sizeof(STARTUPINFOW), 104),
        "PROCESS_INFORMATION": (ctypes.sizeof(PROCESS_INFORMATION), 24),
    }
    if any(actual != expected for actual, expected in layout.values()):
        raise RuntimeError(f"unexpected Windows structure sizes: {layout}")
    expected_offsets = {
        "lpReserved": 8, "dwFlags": 60, "lpReserved2": 72,
        "hStdInput": 80, "hStdOutput": 88, "hStdError": 96,
    }
    if any(getattr(STARTUPINFOW, field).offset != offset
           for field, offset in expected_offsets.items()):
        raise RuntimeError("unexpected STARTUPINFOW Win64 field offsets")
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.CreateFileW.argtypes = [
        wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
        ctypes.POINTER(SECURITY_ATTRIBUTES), wintypes.DWORD,
        wintypes.DWORD, wintypes.HANDLE,
    ]
    kernel.CreateFileW.restype = wintypes.HANDLE
    kernel.CreateProcessW.argtypes = [
        wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.POINTER(SECURITY_ATTRIBUTES),
        ctypes.POINTER(SECURITY_ATTRIBUTES), wintypes.BOOL, wintypes.DWORD,
        wintypes.LPVOID, wintypes.LPCWSTR, ctypes.POINTER(STARTUPINFOW),
        ctypes.POINTER(PROCESS_INFORMATION),
    ]
    kernel.CreateProcessW.restype = wintypes.BOOL
    kernel.ResumeThread.argtypes = [wintypes.HANDLE]
    kernel.ResumeThread.restype = wintypes.DWORD
    kernel.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
    kernel.WaitForSingleObject.restype = wintypes.DWORD
    kernel.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
    kernel.GetExitCodeProcess.restype = wintypes.BOOL
    kernel.TerminateProcess.argtypes = [wintypes.HANDLE, wintypes.UINT]
    kernel.TerminateProcess.restype = wintypes.BOOL
    kernel.CloseHandle.argtypes = [wintypes.HANDLE]
    kernel.CloseHandle.restype = wintypes.BOOL
    return kernel


def winerror(operation: str) -> OSError:
    error = ctypes.WinError(ctypes.get_last_error())
    return OSError(f"{operation}: {error}")


def run_suspended(exe: Path, root: Path, gate: Path, pid_file: Path,
                  timeout: float, gate_timeout: float, max_output: int,
                  record: dict) -> None:
    kernel = windows_api()
    handles = []
    process = PROCESS_INFORMATION()
    record["status"] = "launch_error"
    invalid_handle = ctypes.c_void_p(-1).value
    attrs = SECURITY_ATTRIBUTES(ctypes.sizeof(SECURITY_ATTRIBUTES), None, True)

    def open_redirect(path: Path, read: bool):
        # OPEN_EXISTING for input; CREATE_ALWAYS for output; FILE_SHARE_READ
        # permits the size checks and later capture inspection.
        handle = kernel.CreateFileW(str(path), 0x80000000 if read else 0x40000000,
                                    1, ctypes.byref(attrs), 3 if read else 2,
                                    0x80, None)
        if handle == invalid_handle:
            raise winerror("CreateFileW")
        handles.append(handle)
        return handle

    def alive() -> bool:
        result = kernel.WaitForSingleObject(process.hProcess, 0)
        if result == 0:  # WAIT_OBJECT_0
            return False
        if result == 0x102:  # WAIT_TIMEOUT
            return True
        raise winerror("WaitForSingleObject")

    def output_bytes() -> int:
        return sum((root / name).stat().st_size for name in ("stdout.bin", "stderr.bin"))

    try:
        startup = STARTUPINFOW()
        startup.cb = ctypes.sizeof(startup)
        startup.dwFlags = 0x100  # STARTF_USESTDHANDLES
        startup.hStdInput = open_redirect(root / "stdin.bin", True)
        startup.hStdOutput = open_redirect(root / "stdout.bin", False)
        startup.hStdError = open_redirect(root / "stderr.bin", False)
        command_line = ctypes.create_unicode_buffer(subprocess.list2cmdline([str(exe)]))
        flags = 0x00000004 | 0x08000000  # CREATE_SUSPENDED | CREATE_NO_WINDOW
        if not kernel.CreateProcessW(str(exe), command_line, None, None, True,
                                     flags, None, str(root), ctypes.byref(startup),
                                     ctypes.byref(process)):
            raise winerror("CreateProcessW")
        handles.extend([process.hThread, process.hProcess])
        record.update({
            "pid": int(process.dwProcessId),
            "primary_thread_id": int(process.dwThreadId),
            "creation_flags": flags, "created_suspended": True,
            "status": "waiting_for_probe",
        })
        identity = {
            "pid": record["pid"], "primary_thread_id": record["primary_thread_id"],
            "executable": str(exe), "executable_sha256": record["executable_sha256"],
            "cwd": str(root), "ready_gate": str(gate),
            "created_suspended": True, "primary_thread_resumed": False,
            "inputs": record["inputs"],
        }
        pid_file.parent.mkdir(parents=True, exist_ok=True)
        pid_file.write_text(json.dumps(identity, indent=2), encoding="utf-8")
        print(json.dumps({"observer_waiting": identity}), flush=True)
        gate_start = time.monotonic()
        while not gate.exists():
            if not alive():
                record["status"] = "exited_before_resume"
                return
            if time.monotonic() - gate_start >= gate_timeout:
                record["status"] = "gate_timeout"
                return
            time.sleep(0.02)
        record["gate_wait_seconds"] = round(time.monotonic() - gate_start, 3)
        if not alive():
            record["status"] = "exited_before_resume"
            return
        previous_count = kernel.ResumeThread(process.hThread)
        record["resume_calls"] = 1
        record["resume_previous_suspend_count"] = int(previous_count)
        if previous_count == 0xFFFFFFFF:
            raise winerror("ResumeThread")
        if previous_count != 1:
            record["status"] = "unexpected_suspend_count"
            return
        record["status"] = "completed"
        record["primary_thread_resumed"] = True
        activity_start = time.monotonic()
        while alive():
            if time.monotonic() - activity_start >= timeout:
                record["status"] = "timeout"
                break
            if output_bytes() > max_output:
                record["status"] = "output_limit"
                break
            time.sleep(0.01)
        if record["status"] == "completed" and output_bytes() > max_output:
            record["status"] = "output_limit"
    except OSError as error:
        record["status"] = "process_error"
        record["error"] = str(error)
    finally:
        if process.hProcess:
            try:
                if alive():
                    if not kernel.TerminateProcess(process.hProcess, 0xE0000001):
                        raise winerror("TerminateProcess")
                    record["terminated_by_helper"] = True
                    if kernel.WaitForSingleObject(process.hProcess, 5000) != 0:
                        raise RuntimeError("terminated process did not signal within five seconds")
                exit_code = wintypes.DWORD()
                if not kernel.GetExitCodeProcess(process.hProcess, ctypes.byref(exit_code)):
                    raise winerror("GetExitCodeProcess")
                record["exit_code"] = int(exit_code.value)
            except (OSError, RuntimeError) as error:
                record["cleanup_error"] = str(error)
        for handle in reversed(handles):
            if not kernel.CloseHandle(handle):
                record.setdefault("handle_close_errors", []).append(str(winerror("CloseHandle")))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--config", type=Path, action="append", default=[],
                        help="explicit .INI file to copy into the private cwd; repeat as needed")
    parser.add_argument("--ready-gate", type=Path)
    parser.add_argument("--pid-file", type=Path)
    parser.add_argument("--gate-timeout", type=float, default=120)
    parser.add_argument("--timeout", type=float, default=15)
    parser.add_argument("--max-output-bytes", type=int, default=2 * 1024 * 1024)
    parser.add_argument("--prepare-only", action="store_true")
    args = parser.parse_args()
    windows_api()  # Validate Win64 declarations even when preparing only.
    if not 0 < args.timeout <= 15 or not 0 < args.gate_timeout <= 120:
        parser.error("timeout must be in (0,15], gate-timeout in (0,120]")
    if not 0 < args.max_output_bytes <= 16 * 1024 * 1024:
        parser.error("max-output-bytes must be in (0,16 MiB]")
    if bool(args.ready_gate) != bool(args.pid_file):
        parser.error("ready-gate and pid-file must be supplied together")
    if not args.prepare_only and not args.ready_gate:
        parser.error("execution requires ready-gate and pid-file for pre-startup observation")
    oracle = Path(__file__).resolve().parents[1] / "analysis" / "oracle"
    root = args.output.resolve()
    if root == oracle or not root.is_relative_to(oracle) or root.exists():
        parser.error("output must be a new directory beneath analysis/oracle")
    gate = args.ready_gate.resolve() if args.ready_gate else None
    pid_file = args.pid_file.resolve() if args.pid_file else None
    for path in (gate, pid_file):
        if path and (not path.is_relative_to(oracle) or path.exists()):
            parser.error("gate and PID paths must be new paths beneath analysis/oracle")
        if path and path.is_relative_to(root):
            parser.error("gate and PID paths must be outside the new output directory")
    if gate and gate == pid_file:
        parser.error("gate and PID paths must differ")
    exe = args.exe.resolve(strict=True)
    if not exe.is_file():
        parser.error("exe must be a file")
    sources = [p.resolve(strict=True) for p in args.config]
    if len(sources) > 16 or len({p.name.lower() for p in sources}) != len(sources):
        parser.error("provide at most 16 config files with distinct names")
    if any(not p.is_file() or p.suffix.lower() != ".ini" or p.stat().st_size > 65536
           for p in sources):
        parser.error("config files must be .INI files no larger than 64 KiB")
    root.mkdir(parents=True)
    inputs = []
    for source in sources:
        copy = root / source.name
        shutil.copyfile(source, copy)
        inputs.append({"source": str(source), "copy": str(copy),
                       "sha256_before": digest(copy)})
    stdin = b"\nE\nY\n"
    (root / "stdin.bin").write_bytes(stdin)
    record = {
        "executable": str(exe), "executable_sha256": digest(exe),
        "cwd": str(root), "inputs": inputs, "stdin": stdin.decode("ascii"),
        "created_suspended": False, "primary_thread_resumed": False,
        "resume_calls": 0, "pid": None, "primary_thread_id": None,
        "status": "prepared", "exit_code": None, "terminated_by_helper": False,
        "cleanup_error": None, "ready_gate": str(gate) if gate else None,
        "timeout_seconds": args.timeout, "gate_timeout_seconds": args.gate_timeout,
        "max_output_bytes": args.max_output_bytes,
        "limitations": [
            "The fixed console transcript is initial CR, E, Y; configuration may change prompts.",
            "Bounds are checked periodically and may overshoot by one polling interval.",
            "Attaching instrumentation can run observer machinery before the primary thread resumes.",
            "Completion alone does not establish that every startup path was observed.",
        ],
    }
    start = time.monotonic()
    if not args.prepare_only:
        run_suspended(exe, root, gate, pid_file, args.timeout,
                      args.gate_timeout, args.max_output_bytes, record)
    record["elapsed_seconds"] = round(time.monotonic() - start, 3)
    record["executable_sha256_after"] = digest(exe)
    for item in inputs:
        copy = Path(item["copy"])
        item["sha256_after"] = digest(copy) if copy.is_file() else None
        item["source_sha256_after"] = digest(Path(item["source"]))
    record["files"] = [
        {"name": p.name, "bytes": p.stat().st_size, "sha256": digest(p)}
        for p in sorted(root.iterdir()) if p.is_file()
    ]
    for name in ("stdout", "stderr"):
        path = root / f"{name}.bin"
        if path.is_file():
            with path.open("rb") as stream:
                record[f"{name}_preview"] = stream.read(65536).decode(errors="replace")
    (root / "run.json").write_text(json.dumps(record, indent=2), encoding="utf-8")
    print(json.dumps({key: record[key] for key in (
        "status", "pid", "primary_thread_id", "exit_code", "resume_calls",
        "primary_thread_resumed", "elapsed_seconds", "cleanup_error"
    )}, indent=2))


if __name__ == "__main__":
    main()
