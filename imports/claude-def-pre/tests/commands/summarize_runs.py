"""Summarize oracle runs of tests/commands cases (folders written by def_pre_oracle --copy-to).

    python -I tests/commands/summarize_runs.py <copy-to dir> [--grep REGEX] [--full LABEL]

For each case: exit code, files created, and stdout lines that matter (messages from the
command and the external program).
"""
import json
import re
import sys
from pathlib import Path

DEFAULT = r"(?i)error|warn|fail|success|interp|extract|remesh|mesh|morph|import|convert|not found|cannot|can't|unable|DEF_|\.KEY|\.DB"


def main() -> int:
    root = Path(sys.argv[1])
    pat = re.compile(DEFAULT)
    full = None
    args = sys.argv[2:]
    while args:
        a = args.pop(0)
        if a == "--grep":
            pat = re.compile(args.pop(0))
        elif a == "--full":
            full = args.pop(0)
    for d in sorted(p for p in root.iterdir() if p.is_dir()):
        res = d / "result.json"
        info = json.loads(res.read_text()) if res.exists() else {}
        files = info.get("files", {})
        print(f"=== {d.name}  exit={info.get('exit_code')}")
        print("  files:", ", ".join(f"{k}({v.get('status', '?')[0]})" for k, v in sorted(files.items())
                                     if not k.startswith("_ext_capture/")))
        cap = d / "files" / "_ext_capture"
        if cap.is_dir():
            for c in sorted(cap.iterdir(), key=lambda p: int(p.name.split("_")[0])):
                rc = (c / "exit_code.txt").read_text().strip() if (c / "exit_code.txt").exists() else "?"
                before = {p.name for p in (c / "before").iterdir()} if (c / "before").is_dir() else set()
                after = {p.name for p in (c / "after").iterdir()} if (c / "after").is_dir() else set()
                tail = [l.strip() for l in (c / "stdout.txt").read_text(errors="replace").splitlines() if l.strip()][-3:] if (c / "stdout.txt").exists() else []
                print(f"  ext {c.name}: rc={rc} new={sorted(after - before)} gone={sorted(before - after)}")
                for l in tail:
                    print("      >", l[:150])
        out = (d / "stdout.txt").read_bytes().decode("latin-1") if (d / "stdout.txt").exists() else ""
        lines = out.splitlines()
        if full and full in d.name:
            print(out)
            continue
        seen = set()
        for ln in lines:
            s = ln.strip()
            if s and pat.search(s) and s not in seen:
                seen.add(s)
                print("   |", s[:160])
                if len(seen) > 25:
                    print("   | ...")
                    break
        err = (d / "stderr.txt").read_bytes().decode("latin-1").strip() if (d / "stderr.txt").exists() else ""
        if err:
            print("  stderr:", err[:300].replace("\n", " / "))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
