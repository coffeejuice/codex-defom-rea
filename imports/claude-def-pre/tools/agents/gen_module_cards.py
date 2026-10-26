#!/usr/bin/env python3
"""Write or check the MODULE.md card of every port library (AGENTS.md, "Module cards").

A card has hand-written sections (purpose, interface, fidelity class, oracle tests worth knowing,
perf baseline, swap notes) and one generated block between the markers below. This script
rewrites only the generated block, from the tree itself:

  * dependencies (def_port_module(... DEPS ...) in the module's CMakeLists.txt) and dependents;
  * file counts and the public header folders;
  * distinct `// @src <addr>` addresses in the module, by ledger status, area and phase
    (docs/re/pre64_batch_ledger.tsv after fold_status.py);
  * DEF_NOT_PORTED stubs left outside tests, and `// @data` globals declared here;
  * the packets whose docs/re/status/*.tsv rows point at files of this module;
  * tier-1 tests (ctest name, labels), which ones replay oracle runs, and the
    tests/commands/<CMD> case folders they read.

    python -I tools/agents/gen_module_cards.py            # rewrite the generated blocks
    python -I tools/agents/gen_module_cards.py --check    # exit 1 if a card is missing or stale
    python -I tools/agents/gen_module_cards.py --print defcmd   # print one generated block

A missing card is created from the template with TODO hand sections. Standard library only.
"""

import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEDGER = ROOT / "docs/re/pre64_batch_ledger.tsv"
STATUS_DIR = ROOT / "docs/re/status"
CASES_DIR = ROOT / "tests/commands"
BEGIN = "<!-- BEGIN GENERATED: tools/agents/gen_module_cards.py; do not edit by hand -->"
END = "<!-- END GENERATED -->"
CODE_EXT = {".cpp", ".hpp", ".h", ".inc", ".asm"}

SRC_RE = re.compile(r"//.*?@src\s+(?:0x)?(14[0-9a-fA-F]{7})\b")
DATA_RE = re.compile(r"//.*?@data\s+(?:0x)?(14[0-9a-fA-F]{7})\b")
STUB_RE = re.compile(r"DEF_NOT_PORTED\(\s*0x(14[0-9a-fA-F]{7})")
LABEL_RE = re.compile(r"^// ctest-labels:\s*(.*)$")
CMD_RE = re.compile(r'"([A-Z][A-Z0-9_]{2,})[/"]')
ORACLE_HINT = re.compile(r"cmdoracle|def_pre_oracle|DEFORM_ORACLE_CACHE|\.deform-oracle|Oracle\b")


def modules():
    """(CMake target, directory, deps) for every port library, plus deformdb."""
    out = []
    for cm in sorted((ROOT / "port").glob("*/CMakeLists.txt")):
        text = cm.read_text(encoding="utf-8")
        m = re.search(r"def_port_module\(\s*(\w+)([^)]*)\)", text)
        if m:
            deps = re.findall(r"\w+", m.group(2).replace("DEPS", " "))
            out.append((m.group(1), cm.parent, deps))
    db = ROOT / "deformdb"
    if (db / "CMakeLists.txt").exists():
        out.append(("deformdb_core", db, []))
    return out


def code_files(d):
    for p in sorted(d.rglob("*")):
        parts = p.relative_to(d).parts
        if p.is_file() and p.suffix in CODE_EXT and not parts[0].startswith("build"):
            yield p


def read_ledger():
    led = {}
    with open(LEDGER, encoding="utf-8") as f:
        head = f.readline().rstrip("\n").split("\t")
        for line in f:
            row = dict(zip(head, line.rstrip("\n").split("\t")))
            led[row["address"].lower().removeprefix("0x")] = row
    return led


def status_rows():
    """{packet: [(address, status, note)]} from docs/re/status/*.tsv (comment rows skipped)."""
    out = {}
    for f in sorted(STATUS_DIR.glob("*.tsv")):
        rows = []
        for i, line in enumerate(f.read_text(encoding="utf-8").splitlines()):
            if i == 0 or not line.strip() or line.startswith("#"):
                continue
            parts = line.split("\t")
            if len(parts) >= 2:
                rows.append((parts[0], parts[1], parts[2] if len(parts) > 2 else ""))
        out[f.stem] = rows
    return out


def rel(p):
    return p.relative_to(ROOT).as_posix()


def table(header, rows):
    lines = ["| " + " | ".join(header) + " |", "|" + "---|" * len(header)]
    lines += ["| " + " | ".join(str(c) for c in r) + " |" for r in rows]
    return lines


def generate(name, d, deps, dependents, led, packets):
    files = list(code_files(d))
    tests = [p for p in files if "tests" in p.relative_to(d).parts]
    src = [p for p in files if p not in tests]
    lines_total = 0
    srcs, datas, stubs = set(), set(), []
    for p in files:
        text = p.read_text(encoding="utf-8", errors="replace")
        lines_total += text.count("\n")
        if p in tests:
            continue
        for n, line in enumerate(text.splitlines(), 1):
            srcs.update(a.lower() for a in SRC_RE.findall(line))
            datas.update(a.lower() for a in DATA_RE.findall(line))
            code = line.split("//", 1)[0]
            stubs += [(a.lower(), f"{rel(p)}:{n}") for a in STUB_RE.findall(code)]

    out = [BEGIN, "", f"Directory `{rel(d)}/`, CMake target `{name}`.", ""]
    out.append(f"* Depends on: {', '.join(f'`{x}`' for x in deps) or 'nothing in the repo'}")
    out.append(f"* Used by: {', '.join(f'`{x}`' for x in dependents) or 'no other port library'}")
    out.append(f"* Files: {len(src)} sources/headers, {len(tests)} test files, {lines_total:,} lines")

    inc = d / "include"
    if inc.is_dir():
        groups = Counter()
        for p in inc.rglob("*"):
            if p.is_file() and p.suffix in CODE_EXT:
                parts = p.relative_to(inc).parts
                key = "/".join(parts[:2]) + "/" if len(parts) > 2 else parts[0] + " (top level)"
                groups[key] += 1
        tops = sorted(groups.items(), key=lambda kv: (-kv[1], kv[0]))
        shown = ", ".join(f"`{k}` {v}" for k, v in tops[:14])
        more = f", and {len(tops) - 14} more folders" if len(tops) > 14 else ""
        out.append(f"* Public headers ({sum(groups.values())}): {shown}{more}")
    out.append("")

    out.append(f"### Binary functions ported here: {len(srcs)} `@src` addresses")
    out.append("")
    if srcs:
        st, ar, ph = Counter(), Counter(), Counter()
        missing = 0
        for a in srcs:
            row = led.get(a)
            if row is None:
                missing += 1
                continue
            st[row.get("status", "?")] += 1
            ar[row.get("area", "?")] += 1
            ph[row.get("first_phase", "?")] += 1
        out.append("Ledger status: " + ", ".join(f"{k} {v}" for k, v in st.most_common())
                   + (f"; {missing} not in the ledger (CRT/data rows, see the status files)" if missing else ""))
        out.append("")
        out.append("Ledger area: " + ", ".join(f"`{k}` {v}" for k, v in ar.most_common()))
        out.append("")
        out.append("First phase: " + ", ".join(
            f"{k} {v}" for k, v in sorted(ph.items(), key=lambda kv: (len(kv[0]), kv[0]))))
        out.append("")
    out.append(f"`@data` globals declared here: {len(datas)}. "
               f"`DEF_NOT_PORTED` stubs outside tests: {len(stubs)}"
               + (" (a stub whose address is ported elsewhere is a seam to rewire: "
                  "`python -I docs/re/stub_sweep.py port`)" if stubs else "") + ".")
    if stubs:
        out.append("")
        for a, where in stubs[:12]:
            out.append(f"* `{a}` at `{where}`")
        if len(stubs) > 12:
            out.append(f"* ... and {len(stubs) - 12} more")
    out.append("")

    prefix = rel(d) + "/"
    owners = Counter()
    for pk, rows in packets.items():
        for _a, _s, note in rows:
            if prefix in note:
                owners[pk] += 1
    if owners:
        out.append("### Packets with status rows pointing here")
        out.append("")
        out.append(", ".join(f"`{k}` {v}" for k, v in sorted(owners.items(), key=lambda kv: (-kv[1], kv[0]))))
        out.append("")

    rows = []
    for t in tests:
        if not (t.name.startswith("test_") and t.suffix == ".cpp"):
            continue
        text = t.read_text(encoding="utf-8", errors="replace")
        labels = []
        for line in text.splitlines()[:20]:
            m = LABEL_RE.match(line)
            if m:
                labels += m.group(1).split()
        oracle = bool(ORACLE_HINT.search(text) or re.search(r"oracle|replay|e2e|corpus", t.stem))
        cmds = sorted({c for c in CMD_RE.findall(text) if (CASES_DIR / c).is_dir()})
        topic = t.stem.removeprefix("test_")
        ctest = f"{name}.{topic}" if name != "deformdb_core" else topic
        rows.append((f"`{ctest}`", " ".join(labels), "yes" if oracle else "",
                     " ".join(cmds[:6]) + (" ..." if len(cmds) > 6 else "")))
    # tests added by hand with add_test() in the module's CMakeLists.txt (scripts, other names)
    have = {r[0] for r in rows}
    cm = (d / "CMakeLists.txt").read_text(encoding="utf-8")
    for t in re.findall(r"add_test\(\s*NAME\s+\"?([\w.:-]+)", cm):
        if f"`{t}`" not in have:
            rows.append((f"`{t}`", "(add_test in CMakeLists.txt)", "", ""))
    out.append(f"### Tier-1 tests ({len(rows)})")
    out.append("")
    if rows:
        out += table(["ctest", "extra labels", "oracle", "tests/commands"], rows)
    else:
        out.append("None.")
    label = "deformdb" if name == "deformdb_core" else name
    out += ["", f"Run: `ctest --test-dir port/build -L {label}` (after `port\\build.cmd`).", "", END]
    return "\n".join(out)


TEMPLATE = """# {name}: module card

## Purpose

TODO: one paragraph: what this library does in the original program and in the port.

## Interface

TODO: the entry points other modules or apps call (headers and functions), and what they promise.

## Fidelity class

TODO: F1 bit-exact, F2 format-exact or F3 free (AGENTS.md, "Fidelity classes"), and why.

## Oracle tests worth knowing

TODO: the tests or e2e labels that guard this module's observable output.

## Perf baseline

Not measured yet. Record machine, build (Release, commit), input, wall time and peak memory, as
the `profile` skill (.claude/skills/profile) prints them.

## Swap notes

TODO: which parts could be replaced (evolve mode) and what a replacement must keep.

{generated}
"""


def main():
    args = sys.argv[1:]
    check = "--check" in args
    only = args[args.index("--print") + 1] if "--print" in args else None
    led = read_ledger()
    packets = status_rows()
    mods = modules()
    dependents = defaultdict(list)
    for n, _d, deps in mods:
        for x in deps:
            dependents[x].append(n)
    stale = []
    for n, d, deps in mods:
        if only and only not in (n, d.name):
            continue
        gen = generate(n, d, deps, sorted(dependents[n]), led, packets)
        if only:
            print(gen)
            continue
        card = d / "MODULE.md"
        old = card.read_text(encoding="utf-8") if card.exists() else None
        if old is None:
            new = TEMPLATE.format(name=d.name, generated=gen)
        else:
            i, j = old.find(BEGIN), old.find(END)
            new = (old.rstrip("\n") + "\n\n" + gen + "\n") if i < 0 or j < 0 else old[:i] + gen + old[j + len(END):]
        if new != old:
            stale.append(rel(card))
            if not check:
                card.write_text(new, encoding="utf-8", newline="\n")
    if only:
        return 0
    for s in stale:
        print(("stale: " if check else "wrote: ") + s)
    if check and stale:
        print("run: python -I tools/agents/gen_module_cards.py")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
