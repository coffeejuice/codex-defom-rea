"""List DEF_NOT_PORTED stubs (outside tests) whose address has an @src port elsewhere under port/.

    python docs/re/stub_sweep.py port

A hit is a seam whose default may still stop although the owner's code is in the tree; a hit
behind a __has_include switch that is live is only a dead fallback."""
import re, sys
from collections import defaultdict
from pathlib import Path

port = Path(sys.argv[1])
src = defaultdict(set)
stubs = defaultdict(list)
for p in port.rglob("*"):
    if p.suffix not in (".cpp", ".hpp", ".h", ".inc"):
        continue
    parts = p.relative_to(port).parts
    if parts[0].startswith("build"):
        continue
    t = p.read_text(encoding="utf-8", errors="replace")
    for n, line in enumerate(t.splitlines(), 1):
        i = line.find("//")
        if i >= 0:
            for m in re.finditer(r"@src\s+(?:0x)?(14[0-9a-fA-F]{7})\b", line[i:]):
                src[m.group(1).lower()].add(str(p.relative_to(port)))
        code = line if i < 0 else line[:i]
        for m in re.finditer(r"DEF_NOT_PORTED\(\s*0x(14[0-9a-fA-F]{7})", code):
            if "tests" not in parts:
                stubs[m.group(1).lower()].append(f"{p.relative_to(port)}:{n}")
for a, where in sorted(stubs.items()):
    owners = sorted(o for o in src.get(a, ()) if not any(o == w.split(":")[0] for w in where))
    if owners:
        print(a, "stub at", ", ".join(where), "| ported in", ", ".join(owners))
