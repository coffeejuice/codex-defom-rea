"""Compare deformdb output with a KEY file exported by DEFORM for the same step.

    dbinfo <db> --step N --dump RZ --dump NDTMP ... > ours.txt
    python compare_key.py <exported.KEY> ours.txt

The KEY file is DEFORM's own text export (DEF_PRE: DBREAD <step> / KFWRIT), used
only as a test oracle. Sparse KEY blocks (e.g. STRAIN, DAMAGE) list non-default
rows; missing rows take the block default.
"""
import re
import sys
from collections import defaultdict

INT_KEYWORDS = {"ELMCON", "BCCDEF", "BCCDFN", "BCCTMP", "BCCTFN", "MTLGRP"}


def is_int(tok):
    return re.fullmatch(r"-?\d+", tok) is not None


def parse_key(path, wanted):
    blocks = {}  # (kw, obj) -> dict(default, width, rows{id: [values]})
    cur = None
    last_row = None
    with open(path, encoding="latin-1") as f:
        for line in f:
            if not line.strip():
                continue
            if not line[0].isspace():
                tok = line.split()
                kw = tok[0]
                cur = None
                if kw in wanted and len(tok) >= 3 and is_int(tok[1]):
                    obj = int(tok[1])
                    rest = tok[2:]
                    default = None
                    if kw in INT_KEYWORDS:
                        if kw.startswith("BCC") and len(rest) >= 2:
                            default = int(rest[1])
                    else:
                        floats = [t for t in rest if not is_int(t)]
                        if floats:
                            default = float(floats[0])
                    cur = {"default": default, "rows": {}}
                    blocks[(kw, obj)] = cur
                    last_row = None
                continue
            if cur is None:
                continue
            tok = line.split()
            if is_int(tok[0]) and len(tok) > 1:
                rid = int(tok[0])
                vals = [int(t) if is_int(t) else float(t) for t in tok[1:]]
                cur["rows"][rid] = vals
                last_row = rid
            elif last_row is not None:
                cur["rows"][last_row].extend(float(t) for t in tok)
    return blocks


def parse_ours(path):
    data = defaultdict(dict)  # (kw, obj) -> {row: values}
    with open(path) as f:
        for line in f:
            tok = line.split()
            if len(tok) < 4 or not is_int(tok[1]) or not is_int(tok[2]):
                continue
            kw, obj, row = tok[0], int(tok[1]), int(tok[2])
            data[(kw, obj)][row] = [int(t) if is_int(t) else float(t) for t in tok[3:]]
    return data


def close(a, b):
    if isinstance(a, int) and isinstance(b, int):
        return a == b
    return abs(a - b) <= 1e-9 * max(1.0, abs(a), abs(b))


def main():
    key_path, ours_path = sys.argv[1], sys.argv[2]
    ours = parse_ours(ours_path)
    wanted = {kw for kw, _ in ours}
    key = parse_key(key_path, wanted)
    total_bad = 0
    for (kw, obj), rows in sorted(ours.items()):
        blk = key.get((kw, obj))
        if blk is None:
            print(f"{kw:7s} obj {obj}: not in KEY file")
            continue
        bad = 0
        checked = 0
        for rid, vals in rows.items():
            ref = blk["rows"].get(rid)
            if ref is None:
                if blk["default"] is None:
                    continue
                ref = [blk["default"]] * len(vals)
            checked += 1
            if len(ref) != len(vals) or not all(close(a, b) for a, b in zip(vals, ref)):
                if bad < 3:
                    print(f"  {kw} obj {obj} row {rid}: ours {vals[:6]} key {ref[:6]}")
                bad += 1
        extra = [r for r in blk["rows"] if r not in rows]
        status = "OK" if bad == 0 and not extra else "MISMATCH"
        print(f"{kw:7s} obj {obj}: {checked} rows checked, {bad} differ, {len(extra)} KEY rows not produced -> {status}")
        total_bad += bad + len(extra)
    print("RESULT:", "all equal" if total_bad == 0 else f"{total_bad} differences")
    return 0 if total_bad == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
