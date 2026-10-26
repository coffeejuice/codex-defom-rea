"""Manifest for the kw-g57 corpus check (test_kw_g57_corpus.cpp).

Lists the cached oracle KEY round trips (label "kfrw/...": KFREAD of a corpus KEY file, then
KFWRIT) whose output has phase-pair records: one tab-separated line per case with the corpus
input path and the cached KFWRIT output path. Reads the oracle cache only.

    python -I kw_g57_corpus_manifest.py OUT.tsv
"""
import glob
import json
import os
import sys


def main():
    root = os.environ.get("DEFORM_ORACLE_CACHE") or os.path.join(os.environ["USERPROFILE"], ".deform-oracle")
    rows = []
    for rj in sorted(glob.glob(os.path.join(root, "entries", "*", "*", "result.json"))):
        with open(rj, encoding="utf-8") as fh:
            r = json.load(fh)
        if not r.get("label", "").startswith("kfrw/") or r.get("exit_code") != 0:
            continue
        src = [i["source"] for i in r.get("inputs", []) if i.get("source") and i["name"] != "CMD.KEY"]
        outs = [k for k, v in r.get("files", {}).items() if v.get("status") == "created"]
        if len(src) != 1 or len(outs) != 1:
            continue
        out = os.path.join(os.path.dirname(rj), "files", outs[0])
        if not os.path.exists(out):
            continue
        with open(out, "rb") as fh:
            data = fh.read()
        if b"\nPHASTF" not in data:
            continue
        rows.append((src[0], out, r["label"]))
    with open(sys.argv[1], "w", encoding="utf-8") as fh:
        for s, o, lab in rows:
            fh.write(f"{s}\t{o}\t{lab}\n")
    print(len(rows), "cases")


if __name__ == "__main__":
    main()
