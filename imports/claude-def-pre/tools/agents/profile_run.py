#!/usr/bin/env python3
"""Time a command several times and report wall time and peak memory (`profile` skill).

    python -I tools/agents/profile_run.py [-n 5] [--cwd DIR] [--stdin FILE] [--fresh SRC] -- CMD ARGS...

* -n N        number of runs (default 5); min and median are printed, plus every run.
* --cwd DIR   working directory for the command (use a scratch copy of the inputs).
* --stdin F   file fed to the command's stdin (the menu protocol, for example).
* --fresh SRC copy the folder SRC to DIR before every run, so each run starts from the same files.

Peak memory is the process's peak working set (Windows GetProcessMemoryInfo), read after the
process exits while its handle is still open; child processes are not included. On other
systems only wall time is reported. Standard library only.
"""

import argparse
import ctypes
import os
import shutil
import statistics
import subprocess
import sys
import time


class PMC(ctypes.Structure):
    _fields_ = [("cb", ctypes.c_ulong), ("PageFaultCount", ctypes.c_ulong),
                ("PeakWorkingSetSize", ctypes.c_size_t), ("WorkingSetSize", ctypes.c_size_t),
                ("QuotaPeakPagedPoolUsage", ctypes.c_size_t), ("QuotaPagedPoolUsage", ctypes.c_size_t),
                ("QuotaPeakNonPagedPoolUsage", ctypes.c_size_t), ("QuotaNonPagedPoolUsage", ctypes.c_size_t),
                ("PagefileUsage", ctypes.c_size_t), ("PeakPagefileUsage", ctypes.c_size_t)]


def peak_working_set(proc):
    if os.name != "nt":
        return None
    pmc = PMC()
    pmc.cb = ctypes.sizeof(PMC)
    get = ctypes.WinDLL("psapi").GetProcessMemoryInfo
    get.argtypes = [ctypes.c_void_p, ctypes.POINTER(PMC), ctypes.c_ulong]
    if get(ctypes.c_void_p(int(proc._handle)), ctypes.byref(pmc), pmc.cb):
        return pmc.PeakWorkingSetSize
    return None


def run_once(cmd, cwd, stdin_path):
    stdin = open(stdin_path, "rb") if stdin_path else subprocess.DEVNULL
    try:
        t0 = time.perf_counter()
        p = subprocess.Popen(cmd, cwd=cwd, stdin=stdin, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        p.wait()
        wall = time.perf_counter() - t0
        return wall, peak_working_set(p), p.returncode
    finally:
        if stdin_path:
            stdin.close()


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("-n", type=int, default=5)
    ap.add_argument("--cwd")
    ap.add_argument("--stdin")
    ap.add_argument("--fresh")
    ap.add_argument("cmd", nargs=argparse.REMAINDER)
    a = ap.parse_args()
    cmd = a.cmd[1:] if a.cmd[:1] == ["--"] else a.cmd
    if not cmd:
        ap.error("no command given (put it after --)")
    if a.fresh and not a.cwd:
        ap.error("--fresh needs --cwd")
    rows = []
    for i in range(a.n):
        if a.fresh:
            shutil.rmtree(a.cwd, ignore_errors=True)
            shutil.copytree(a.fresh, a.cwd)
        wall, peak, rc = run_once(cmd, a.cwd, a.stdin)
        rows.append((wall, peak, rc))
        mb = f"{peak / 2**20:.1f} MB" if peak else "n/a"
        print(f"run {i + 1}: {wall:.3f} s, peak working set {mb}, exit {rc}")
    walls = [r[0] for r in rows]
    peaks = [r[1] for r in rows if r[1]]
    print(f"wall: min {min(walls):.3f} s, median {statistics.median(walls):.3f} s over {a.n} runs")
    if peaks:
        print(f"peak working set: max {max(peaks) / 2**20:.1f} MB")
    return 0 if all(r[2] == 0 for r in rows) else 1


if __name__ == "__main__":
    sys.exit(main())
