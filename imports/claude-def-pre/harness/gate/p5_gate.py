"""P5 gate check (packet p5-01): merge the menu shell into the gate branch locally, clean build, tier 1, and
check that every interim copy of p5-01 is compiled out once its owner's header is in the tree.

    python -I harness/gate/p5_gate.py [--base origin/claude/p5-gate]
        [--merge origin/claude/p5-01-follow] [--worktree DIR] [--keep] [-j 8]

Run from any checkout of the repository (--base also takes origin/claude/int-all or origin/main).
Nothing is pushed: the merge happens in a throwaway
detached worktree (default <repo>/../p5gate-<pid>), removed at the end unless --keep.

Interim copies and their owners (the copy is compiled only while the owner's header is absent;
each compiled copy defines the marker symbol p501_interim_<address>, and its TU prints a
"P501-INTERIM" #pragma message):

    1401b5f00                              p8-07  port/defcmd/include/defcmd/key_file.hpp
    140009c50 140009e90 1401b1060 1401b0cc0  p4-01  port/def_pre/include/def_pre/menu7.hpp
    1401d7f40 1401d6250 1401d63c0          P4 gate port/defdbio/include/defdbio/gendb/pre_steps.hpp

Per address: PASS when the owner's header is present and neither the marker symbol (dumpbin
/symbols of defcmd.lib and def_pre_core.lib) nor the build-log message is found; FAIL otherwise
(with the reason). Exit code 0 only when the merge, the build, tier 1 and every item pass.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

VCVARS = r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
ITEMS = [
    ("1401b5f00", "p8-07", "port/defcmd/include/defcmd/key_file.hpp"),
    ("140009c50", "p4-01", "port/def_pre/include/def_pre/menu7.hpp"),
    ("140009e90", "p4-01", "port/def_pre/include/def_pre/menu7.hpp"),
    ("1401b1060", "p4-01", "port/def_pre/include/def_pre/menu7.hpp"),
    ("1401b0cc0", "p4-01", "port/def_pre/include/def_pre/menu7.hpp"),
    ("1401d7f40", "P4 gate", "port/defdbio/include/defdbio/gendb/pre_steps.hpp"),
    ("1401d6250", "P4 gate", "port/defdbio/include/defdbio/gendb/pre_steps.hpp"),
    ("1401d63c0", "P4 gate", "port/defdbio/include/defdbio/gendb/pre_steps.hpp"),
]


def git(*args, cwd=None, check=True):
    p = subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True, errors="replace")
    if check and p.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)}: {p.stderr.strip() or p.stdout.strip()}")
    return p


def main(argv=None) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="origin/claude/p5-gate",
                    help="branch to merge into (default origin/claude/p5-gate; int-all or main work too)")
    ap.add_argument("--merge", default="origin/claude/p5-01-follow")
    ap.add_argument("--worktree")
    ap.add_argument("--keep", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    a = ap.parse_args(argv)

    repo = Path(git("rev-parse", "--show-toplevel").stdout.strip())
    wt = Path(a.worktree or repo.parent / f"p5gate-{os.getpid()}")
    results = []

    def item(name, ok, why=""):
        results.append(ok)
        print(f"{'PASS' if ok else 'FAIL'}  {name}" + (f"  ({why})" if why else ""), flush=True)

    git("fetch", "-q", "origin", cwd=repo)
    base = git("rev-parse", a.base, cwd=repo).stdout.strip()
    head = git("rev-parse", a.merge, cwd=repo).stdout.strip()
    print(f"base  {a.base} @{base[:9]}\nmerge {a.merge} @{head[:9]}\nworktree {wt}", flush=True)
    git("worktree", "add", "-q", "--detach", str(wt), base, cwd=repo)
    try:
        m = git("-c", "user.name=p5gate", "-c", "user.email=p5gate@localhost", "merge", "--no-edit",
                "-q", head, cwd=wt, check=False)
        item("merge", m.returncode == 0, (m.stdout + m.stderr).strip()[:300])
        if m.returncode != 0:
            return 1
        port = wt / "port"
        build = port / "build"
        shutil.rmtree(build, ignore_errors=True)  # clean: no stale __has_include results
        log = wt / "p5gate_build.log"
        cmd = wt / "p5gate_build.cmd"
        cmd.write_text(
            "@echo off\r\n"
            f'call "{VCVARS}" >nul || exit /b 1\r\n'
            f'cmake -S "{port}" -B "{build}" -G Ninja -DCMAKE_BUILD_TYPE=Release > "{log}" 2>&1 || exit /b 1\r\n'
            f'cmake --build "{build}" -- -k 0 >> "{log}" 2>&1\r\n'
            "set RC=%ERRORLEVEL%\r\n"
            f'dumpbin /symbols "{build / "defcmd" / "defcmd.lib"}" > "{wt / "p5gate_syms.txt"}" 2>&1\r\n'
            f'dumpbin /symbols "{build / "def_pre" / "def_pre_core.lib"}" >> "{wt / "p5gate_syms.txt"}" 2>&1\r\n'
            "exit /b %RC%\r\n", encoding="ascii")
        b = subprocess.run(["cmd", "/c", str(cmd)], capture_output=True, text=True, errors="replace")
        blog = log.read_text(encoding="utf-8", errors="replace") if log.exists() else ""
        errs = [ln for ln in blog.splitlines() if "FAILED:" in ln][:5]
        item("clean build", b.returncode == 0, "; ".join(errs) or f"exit {b.returncode}")
        t = subprocess.run(["ctest", "--test-dir", str(build), "-L", "tier1", "-j", str(a.jobs)],
                           capture_output=True, text=True, errors="replace", env=os.environ)
        summ = re.search(r"\d+% tests passed(, \d+ tests failed)? out of \d+", t.stdout)
        failed = [ln.strip() for ln in t.stdout.splitlines() if re.match(r"\s*\d+ - ", ln)]
        item("tier 1", t.returncode == 0, summ.group(0) if summ else t.stdout.strip()[-200:])
        for f in failed:
            print(f"      {f}")
        syms = (wt / "p5gate_syms.txt").read_text(encoding="utf-8", errors="replace") \
            if (wt / "p5gate_syms.txt").exists() else ""
        for addr, owner, header in ITEMS:
            have = (wt / header).exists()
            in_syms = f"p501_interim_{addr}" in syms
            in_log = re.search(rf"P501-INTERIM[^\n]*{addr}", blog) is not None
            if not have:
                item(f"interim {addr}", False, f"owner {owner}: {header} not in the tree, interim copy in use"
                     f" (symbol {in_syms}, build log {in_log})")
            elif in_syms or in_log:
                item(f"interim {addr}", False, f"{header} present but the interim copy is still compiled"
                     f" (symbol {in_syms}, build log {in_log})")
            else:
                item(f"interim {addr}", True, f"compiled out, owner {owner} header present")
        ok = all(results)
        print(f"P5 gate: {'PASS' if ok else 'FAIL'} ({sum(results)}/{len(results)} items)")
        return 0 if ok else 1
    finally:
        if not a.keep:
            git("worktree", "remove", "--force", str(wt), cwd=repo, check=False)


if __name__ == "__main__":
    sys.exit(main())
