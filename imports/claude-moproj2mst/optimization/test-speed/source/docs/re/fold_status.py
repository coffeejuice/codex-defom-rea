#!/usr/bin/env python3
"""Fold per-packet status files into the batch ledger (docs/port-conventions.md, "Ledger status").

Each porting packet owns one file docs/re/status/<packet>.tsv:

    address	status	note
    0x1401c8950	ported	port/defmodel/src/db_step_data.cpp
    0x140484f90	not-needed	crt: strcpy

status is todo | ported | verified | not-needed. Packets never edit pre64_batch_ledger.tsv; this
script rewrites its status column from the status files, so parallel packets do not conflict.

    python -I docs/re/fold_status.py            # rewrite the ledger status column, print a summary
    python -I docs/re/fold_status.py --check    # also check @src tags under port/ (exit 1 on errors)
    python -I docs/re/fold_status.py --dry-run  # summary only, ledger untouched

Checks (errors make the exit code 1):
  * unknown status words, malformed addresses, an address listed twice with different statuses;
  * --check: a ported/verified address with no "// @src <addr>" tag anywhere under port/, and
    @src tags naming an address that is not a function in the ledger (warning only: the ledger
    may be missing a function the port found; add it to the packet's status file with a note),
    and the same "// @data <addr>" global declared in more than one file (warning).
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LEDGER = ROOT / "docs/re/pre64_batch_ledger.tsv"
STATUS_DIR = ROOT / "docs/re/status"
PORT_DIR = ROOT / "port"
STATUSES = ("todo", "ported", "verified", "not-needed")
ADDR_RE = re.compile(r"^0x14[0-9a-f]{7}$")


def norm(addr: str) -> str:
    a = addr.strip().lower()
    if not a.startswith("0x"):
        a = "0x" + a
    return a


def read_status_files(errors: list[str]) -> dict[str, tuple[str, str, str]]:
    """address -> (status, note, packet file)."""
    result: dict[str, tuple[str, str, str]] = {}
    for path in sorted(STATUS_DIR.glob("*.tsv")):
        i_status, i_note = 1, 2  # default layout: address, status, note
        for lineno, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            if not line.strip() or line.startswith("#"):
                continue
            cols = line.split("\t")
            if cols[0] == "address":  # a header names the columns; extra columns are allowed
                i_status = cols.index("status") if "status" in cols else 1
                i_note = cols.index("note") if "note" in cols else -1
                continue
            if len(cols) <= i_status:
                errors.append(f"{path.name}:{lineno}: need at least address and status")
                continue
            addr, status = norm(cols[0]), cols[i_status].strip()
            note = cols[i_note].strip() if 0 <= i_note < len(cols) else ""
            if not ADDR_RE.match(addr):
                errors.append(f"{path.name}:{lineno}: bad address {cols[0]!r}")
                continue
            if status not in STATUSES:
                errors.append(f"{path.name}:{lineno}: unknown status {status!r}")
                continue
            prev = result.get(addr)
            if prev and prev[0] != status:
                errors.append(f"{path.name}:{lineno}: {addr} is {status} here but {prev[0]} in {prev[2]}")
                continue
            result[addr] = (status, note, path.name)
    return result


def src_tags(kind: str = "src") -> dict[str, list[str]]:
    """address -> files, one entry per tag. Every `@<kind> <addr>` in a line comment counts, also a
    second or third tag after the first one on the line."""
    tag_re = re.compile(r"@" + kind + r"\s+(?:0x)?(14[0-9a-fA-F]{7})\b")
    tags: dict[str, list[str]] = defaultdict(list)
    for path in PORT_DIR.rglob("*"):
        if path.suffix not in (".cpp", ".hpp", ".h", ".inc") or "build" in path.relative_to(PORT_DIR).parts[:1]:
            continue
        rel = str(path.relative_to(ROOT))
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            i = line.find("//")
            if i < 0:
                continue
            for m in tag_re.finditer(line, i):
                tags["0x" + m.group(1).lower()].append(rel)
    return tags


# A namespace-scope variable definition (not a reference, not a function, not a type) whose name
# carries a data address: dat_140b84720, g_common_140b84720, g_work_1407017f0 ...
STORAGE_RE = re.compile(r"^(?:inline\s+|static\s+|constexpr\s+|const\s+|thread_local\s+)*"
                        r"(?!return\b|using\b|typedef\b|class\b|struct\b|union\b|enum\b|namespace\b|friend\b)"
                        r"[A-Za-z_][\w:<>,]*(?:\s*\*+|\s+)\s*"
                        r"(\w*?(14[0-9a-fA-F]{7})\w*)\s*(?:\[[^\]]*\])*\s*(?:=|\{|;)")


def named_storage() -> dict[str, list[str]]:
    """data address -> 'file: name' of each variable definition named after it (port sources, not
    tests). Two of them for one address are two objects for what the original keeps once."""
    defs: dict[str, list[str]] = defaultdict(list)
    for path in PORT_DIR.rglob("*"):
        if path.suffix not in (".cpp", ".hpp", ".h", ".inc"):
            continue
        parts = path.relative_to(PORT_DIR).parts
        if parts[0].startswith("build") or "tests" in parts:
            continue
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            head = line.split("=")[0]
            if "&" in head or "(" in head:
                continue
            m = STORAGE_RE.match(line)
            if m:
                defs["0x" + m.group(2).lower()].append(f"{path.relative_to(ROOT)}: {m.group(1)}")
    return defs


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true", help="also cross-check @src tags under port/")
    ap.add_argument("--dry-run", action="store_true", help="do not rewrite the ledger")
    args = ap.parse_args()

    errors: list[str] = []
    warnings: list[str] = []
    status = read_status_files(errors)

    raw = LEDGER.read_bytes().decode("utf-8")
    eol = "\r\n" if "\r\n" in raw[:4096] else "\n"  # keep the checkout's line endings
    lines = raw.splitlines()
    header = lines[0].split("\t")
    i_addr, i_status, i_phase = header.index("address"), header.index("status"), header.index("first_phase")
    out = [lines[0]]
    seen: set[str] = set()
    per_phase: dict[str, Counter] = defaultdict(Counter)
    for line in lines[1:]:
        cols = line.split("\t")
        addr = norm(cols[i_addr])
        seen.add(addr)
        cols[i_status] = status[addr][0] if addr in status else "todo"
        per_phase[cols[i_phase]][cols[i_status]] += 1
        out.append("\t".join(cols))

    for addr, (st, note, pkt) in sorted(status.items()):
        if addr not in seen:
            warnings.append(f"{pkt}: {addr} ({st}) is not in the ledger; note: {note or '-'}")

    if args.check:
        tags = src_tags("src")
        data_tags = src_tags("data")
        for addr, (st, _note, pkt) in sorted(status.items()):
            if st in ("ported", "verified") and addr not in tags and addr not in data_tags:
                errors.append(f"{pkt}: {addr} is {st} but no '// @src {addr[2:]}' tag exists under port/")
        for addr, files in sorted(tags.items()):
            if addr not in seen:
                warnings.append(f"@src {addr[2:]} in {files[0]} is not a ledger function")
        # one object per data address: two variables named after the same address are two copies
        # of one piece of the original's state, whatever their tags say (a second name must be a
        # reference to the owner, e.g. `inline double& dat_140b84720 = ...u00;`)
        for addr, names in sorted(named_storage().items()):
            if len(names) > 1:
                errors.append(f"data {addr[2:]} has {len(names)} variable definitions: {'; '.join(sorted(names))}"
                              " (keep one object, make the others references to it)")
        # one owner per data address: a second definition is a second copy of the state (the
        # 140b84720 COMMON bug of the KEY merge); other uses refer to the owner without the tag
        for addr, files in sorted(data_tags.items()):
            if len(files) > 1:
                errors.append(f"@data {addr[2:]} is defined {len(files)} times: {', '.join(sorted(files))}"
                              " (keep one @data owner, make the others references to it)")

    if not args.dry_run and not errors:
        LEDGER.write_bytes((eol.join(out) + eol).encode("utf-8"))

    for w in warnings:
        print("warning:", w)
    for e in errors:
        print("error:", e)
    print("phase\t" + "\t".join(STATUSES))
    for phase in sorted(per_phase, key=lambda p: (len(p), p)):
        c = per_phase[phase]
        print(phase + "\t" + "\t".join(str(c[s]) for s in STATUSES))
    if errors:
        print(f"{len(errors)} error(s); ledger not rewritten" if not args.dry_run else f"{len(errors)} error(s)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
