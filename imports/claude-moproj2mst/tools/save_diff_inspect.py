"""Inspect preserved Save references; optionally compare retained candidate bytes.

Reads only. Does not execute either application or modify a reference/candidate.
Use --candidate-simctrl and --candidate-mst only with retained outputs from a
declared candidate run, never with expectations synthesized from source.
"""
import argparse
import difflib
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ORACLES = ROOT / "evidence/gui_mo/oracle_b"


def identity(path):
    data = path.read_bytes()
    return dict(path=str(path.resolve()), bytes=len(data), sha256=hashlib.sha256(data).hexdigest())


def keyword(data, key):
    lines = data.splitlines(keepends=True)
    offset = 0
    for number, line in enumerate(lines, 1):
        if line.split()[:1] == [key.encode("ascii")]:
            following = lines[number] if number < len(lines) else b""
            return dict(line=number, byte_offset=offset, header=line.decode("ascii").rstrip("\r\n"),
                        following_line=following.decode("ascii").rstrip("\r\n"),
                        following_byte_offset=offset + len(line))
        offset += len(line)
    return None


def compare(candidate, expected):
    a, b = candidate.read_bytes(), expected.read_bytes()
    return dict(candidate=identity(candidate), expected=identity(expected), raw_equal=a == b,
                first_difference=next((i for i, (x, y) in enumerate(zip(a, b)) if x != y),
                                      min(len(a), len(b)) if a != b else None),
                unified_lines=list(difflib.unified_diff(a.decode("utf-8").splitlines(),
                                                       b.decode("utf-8").splitlines(),
                                                       fromfile="candidate", tofile="original_Save", n=2)))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--candidate-simctrl", type=Path)
    parser.add_argument("--candidate-mst", type=Path)
    args = parser.parse_args()
    before = ORACLES / "RS_55_Multiple_Operations_Example1_before/Operations/Task00002/SimCtrl.KEY"
    expected = ORACLES / "RS_55_Multiple_Operations_Example1/Operations/Task00002/SimCtrl.KEY"
    mst = ORACLES / "98_B_DS3F/Gear_blank.MST"
    result = {
        "scope": "Static comparison of retained bytes; inspector executes no target programs",
        "reference_run": "build/test-runs/1792752423664000600",
        "rs55": dict(before=identity(before), original_save=identity(expected),
                     before_solmtd=keyword(before.read_bytes(), "SOLMTD"),
                     original_save_solmtd=keyword(expected.read_bytes(), "SOLMTD"),
                     candidate=compare(args.candidate_simctrl, expected) if args.candidate_simctrl else None,
                     candidate_limitation=None if args.candidate_simctrl else "Full folder sweep deletes converted scratch projects"),
        "b98": dict(original_save=identity(mst), first_kfread=keyword(mst.read_bytes(), "KFREAD"),
                    candidate=compare(args.candidate_mst, mst) if args.candidate_mst else None,
                    source_evidence=["evidence/gui_mo/s1/opw_d0.c:300",
                                     "evidence/gui_mo/s1/s1-05/d1s.c:1313",
                                     "source/apps/moproj2mst/src/ops/diestress.cpp:170",
                                     "source/apps/moproj2mst/tests/s/test_save_folder.cpp:122"]),
        "qualifications": ["Reference before/after changes are not an observed candidate diff",
                           "No normalization changes comparator results in this inspector",
                           "Original parameter mutation location remains unresolved",
                           "Historical Save references do not establish fresh-session state"],
    }
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
