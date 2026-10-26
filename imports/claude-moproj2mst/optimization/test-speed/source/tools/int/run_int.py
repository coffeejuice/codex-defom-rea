"""Run the integration oracle replay one label per process (a reached DEF_NOT_PORTED stub aborts
the process) and summarise. Usage:
    python -I tools/int/run_int.py [--exe port/build/defcmd/defcmd_test_int_oracle.exe] [filter...]
Labels: OBJPOS drop cases and every label of tests/commands/{GENCTC,INICTC}/cases*.json.
Writes out/int/<timestamp>/summary.tsv and one log per label (outside the repo: %TEMP%)."""
import glob
import json
import os
import re
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DROP = ["drop-gap05-3d", "drop-inplace-3d", "drop-nohit-3d", "7-drop-3d", "7-default-tol-3d", "7-tilted-3d",
        "8-drop-axis-3d", "9-drop-vector-3d", "9-drop-zerovector-3d", "7-drop-coupled-3d", "7-drop-rigid-3d",
        "7-drop-up-3d", "7-drop-zerodir-3d", "7-undefined-3d", "7-in-2d"]


def labels():
    out = ["OBJPOS/" + c for c in DROP]
    for cmd in ("INICTC", "GENCTC"):
        for f in sorted(glob.glob(os.path.join(ROOT, "tests", "commands", cmd, "cases*.json"))):
            for case in json.load(open(f, encoding="utf-8")):
                out.append(case["label"])
    return out


def main():
    args = sys.argv[1:]
    exe = os.path.join(ROOT, "port", "build", "defcmd", "defcmd_test_int_oracle.exe")
    if args[:1] == ["--exe"]:
        exe, args = args[1], args[2:]
    todo = [l for l in labels() if not args or any(a in l for a in args)]
    outdir = os.path.join(tempfile.gettempdir(), "p67int", time.strftime("%Y%m%d-%H%M%S"))
    os.makedirs(outdir, exist_ok=True)
    rows = []
    for label in todo:
        env = dict(os.environ, INT_ORACLE_LABELS=label)
        try:
            p = subprocess.run([exe], env=env, capture_output=True, text=True, errors="replace", timeout=900)
            text = p.stdout + p.stderr
        except subprocess.TimeoutExpired as e:
            text = (e.stdout or "") + "\nTIMEOUT\n"
        with open(os.path.join(outdir, label.replace("/", "_") + ".log"), "w", encoding="utf-8") as fh:
            fh.write(text)
        m = re.search(r"RESULT \S+ (PASS|FAIL|SKIP)(.*)", text)
        stub = re.search(r"function 0x([0-9a-f]+) not ported", text)
        if m:
            status, detail = m.group(1), m.group(2).strip()
        elif stub:
            status, detail = "STUB", "0x" + stub.group(1)
        else:
            status, detail = "CRASH", text.strip().splitlines()[-1][:120] if text.strip() else ""
        rows.append((label, status, detail))
        print("%-34s %-5s %s" % (label, status, detail), flush=True)
    with open(os.path.join(outdir, "summary.tsv"), "w", encoding="utf-8") as fh:
        fh.write("label\tstatus\tdetail\n")
        for r in rows:
            fh.write("\t".join(r) + "\n")
    counts = {}
    for _, s, _ in rows:
        counts[s] = counts.get(s, 0) + 1
    print("summary:", counts, "logs:", outdir)


if __name__ == "__main__":
    main()
