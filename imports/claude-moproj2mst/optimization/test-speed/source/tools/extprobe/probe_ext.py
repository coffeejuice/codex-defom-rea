"""Observe whether a DEFORM external program runs headless (Phase 8-9 check).

Runs one program from the DEFORM install, unchanged, in a fresh scratch directory that holds
copies of the given inputs. Records exit code, run time, stdout/stderr, the files it created and
the titles of any visible windows opened by the process tree, then kills the tree on timeout.
It does not analyse the program.

    python -I probe_ext.py --exe "C:/Program Files/SFTC/DEFORM/v11.0.2/3D/DEF_AMG.exe" \
        --stdin-file DEF_AMG.INI --input path/DEF_AMG.INI --args "" --timeout 60 --out result.json

Windows only, standard library only.
"""
from __future__ import annotations

import argparse
import ctypes
import ctypes.wintypes as wt
import hashlib
import json
import os
import shutil
import subprocess
import tempfile
import time
from pathlib import Path

user32 = ctypes.WinDLL("user32", use_last_error=True)
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)

TH32CS_SNAPPROCESS = 0x2


class PROCESSENTRY32(ctypes.Structure):
    _fields_ = [("dwSize", wt.DWORD), ("cntUsage", wt.DWORD), ("th32ProcessID", wt.DWORD),
                ("th32DefaultHeapID", ctypes.c_void_p), ("th32ModuleID", wt.DWORD),
                ("cntThreads", wt.DWORD), ("th32ParentProcessID", wt.DWORD),
                ("pcPriClassBase", ctypes.c_long), ("dwFlags", wt.DWORD),
                ("szExeFile", ctypes.c_char * 260)]


def process_table() -> dict[int, tuple[int, str]]:
    snap = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
    out: dict[int, tuple[int, str]] = {}
    e = PROCESSENTRY32()
    e.dwSize = ctypes.sizeof(e)
    ok = kernel32.Process32First(snap, ctypes.byref(e))
    while ok:
        out[e.th32ProcessID] = (e.th32ParentProcessID, e.szExeFile.decode("mbcs", "replace"))
        ok = kernel32.Process32Next(snap, ctypes.byref(e))
    kernel32.CloseHandle(snap)
    return out


def tree(root: int) -> dict[int, str]:
    table = process_table()
    found = {root: table.get(root, (0, "?"))[1]}
    changed = True
    while changed:
        changed = False
        for pid, (ppid, name) in table.items():
            if ppid in found and pid not in found:
                found[pid] = name
                changed = True
    return found


def visible_windows(pids: set[int]) -> list[str]:
    titles: list[str] = []
    proto = ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)

    def cb(hwnd, _):
        pid = wt.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        if pid.value in pids and user32.IsWindowVisible(hwnd):
            n = user32.GetWindowTextLengthW(hwnd)
            buf = ctypes.create_unicode_buffer(n + 1)
            user32.GetWindowTextW(hwnd, buf, n + 1)
            cls = ctypes.create_unicode_buffer(256)
            user32.GetClassNameW(hwnd, cls, 256)
            titles.append(f"{buf.value} [{cls.value}] pid={pid.value}")
        return True

    user32.EnumWindows(proto(cb), 0)
    return titles


def snapshot(d: Path) -> dict[str, str]:
    out = {}
    for p in d.rglob("*"):
        if p.is_file():
            out[p.relative_to(d).as_posix()] = hashlib.sha256(p.read_bytes()).hexdigest()
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--exe", required=True)
    ap.add_argument("--args", default="", help="argument string appended after the program")
    ap.add_argument("--input", action="append", default=[], help="PATH[=NAME] copied into scratch")
    ap.add_argument("--stdin-file", help="name (inside scratch) fed to stdin; default empty")
    ap.add_argument("--env", action="append", default=[], help="NAME=VALUE")
    ap.add_argument("--timeout", type=float, default=60)
    ap.add_argument("--keep", help="copy the scratch dir here afterwards")
    ap.add_argument("--out", required=True)
    a = ap.parse_args()

    scratch = Path(tempfile.mkdtemp(prefix="extprobe_", dir=os.environ.get("EXTPROBE_TMP")))
    for spec in a.input:
        src, _, name = spec.partition("=")
        dst = scratch / (name or Path(src).name)
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
        os.chmod(dst, 0o666)
    before = snapshot(scratch)
    env = dict(os.environ)
    env["TEMP"] = env["TMP"] = str(scratch / ".tmp")
    (scratch / ".tmp").mkdir()
    for kv in a.env:
        k, _, v = kv.partition("=")
        env[k] = v
    stdin = open(scratch / a.stdin_file, "rb") if a.stdin_file else subprocess.DEVNULL
    out_f = open(scratch / ".probe_stdout", "wb")
    err_f = open(scratch / ".probe_stderr", "wb")
    cmd = f'"{a.exe}" {a.args}'.strip()
    t0 = time.time()
    p = subprocess.Popen(cmd, cwd=scratch, stdin=stdin, stdout=out_f, stderr=err_f, env=env)
    windows: set[str] = set()
    procs: dict[int, str] = {}
    timed_out = False
    while True:
        t = tree(p.pid)
        procs.update(t)
        windows.update(visible_windows(set(t)))
        if p.poll() is not None:
            break
        if time.time() - t0 > a.timeout:
            timed_out = True
            subprocess.run(["taskkill", "/F", "/T", "/PID", str(p.pid)], capture_output=True)
            p.wait()
            break
        time.sleep(0.25)
    dt = time.time() - t0
    out_f.close(); err_f.close()
    if stdin is not subprocess.DEVNULL:
        stdin.close()
    after = snapshot(scratch)
    created = sorted(k for k in after if k not in before and not k.startswith(".probe_")
                     and not k.startswith(".tmp/"))
    modified = sorted(k for k in after if k in before and after[k] != before[k])
    res = {
        "exe": a.exe, "args": a.args, "inputs": a.input, "stdin_file": a.stdin_file,
        "exit_code": p.returncode, "timed_out": timed_out, "seconds": round(dt, 2),
        "processes": sorted(set(procs.values())), "visible_windows": sorted(windows),
        "created": created, "modified": modified,
        "stdout_head": (scratch / ".probe_stdout").read_bytes()[:3000].decode("latin-1"),
        "stderr_head": (scratch / ".probe_stderr").read_bytes()[:2000].decode("latin-1"),
        "scratch": str(scratch),
    }
    if a.keep:
        shutil.copytree(scratch, a.keep, dirs_exist_ok=True)
    Path(a.out).write_text(json.dumps(res, indent=1))
    print(json.dumps({k: res[k] for k in ("exe", "exit_code", "timed_out", "seconds",
                                           "visible_windows", "created")}))
    shutil.rmtree(scratch, ignore_errors=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
