"""Build the kw-g4a tier-1 round-trip cases from oracle runs of the original DEF_PRE.

Each oracle case (tools/oracle_cases457.py on branch claude/project-thread-ujpfry) made the
original DEF_PRE_64 read a corpus KEY and write it back with KFWRIT; the inputs are in
materials_rw.json / variants_rw.json and the outputs in golden/ (copies of the shared oracle
cache %USERPROFILE%\\.deform-oracle). For every case this writes, next to this script:

  <case>.in.key   the input reduced to what the kw-g4a test driver reads: every line that starts
                  with a group-4 keyword (so materials are created in the original order), plus
                  the full blocks (keyword line and body lines) of the kw-g4a keywords
  <case>.out.key  the expected text: the kw-g4a keyword blocks of the oracle output, in file order
  cases.txt       the case names

A block is the keyword line and the following lines up to the next line that starts with a
letter or '*'. Bytes are kept as they are (CR LF). Usage:
  python -I make_cases.py <deform-rea-work/out/kw457/oracle>
"""
import json
import os
import re
import sys

MINE = ["FRAE2H", "FPERV", "FRCMOD", "FSTRES", "CREEP", "ANISO", "BURGRS", "ALPHA", "NDISFM", "RECVRY"]
GROUP4 = ["MTNAME", "FRAE2H", "FSTRES", "YOUNG", "POISON", "EXPAND", "THRCND", "HEATCP", "EMSVTY", "LTHEAT",
          "TMPSOL", "TMPLIQ", "FPERV", "FRCMOD", "CREEP", "DIFCOE", "GRNDAT", "GRNMDL", "BURGRS", "ALPHA",
          "NDISFM", "RECVRY", "HDNPHA", "ELRST", "MSTMTR", "HDNTIM", "JOMINY", "UTSDAT", "HDNRUL", "PMEAB",
          "PMITT", "ANISO", "MATDEN", "MASDEN", "COARSE", "SIZEMD", "TXTURE", "DIFBND", "SINTER", "GBENGY",
          "GBMOBI", "NUCSIZ"]


def first_token(line: bytes) -> str:
    m = re.match(rb"\s*([A-Za-z0-9_]+)", line)
    return m.group(1).decode() if m else ""


def starts_block(line: bytes) -> bool:
    return line[:1].isalpha() or line[:1] == b"*"


def blocks(data: bytes, names, header_only=()):
    out = []
    lines = data.splitlines(keepends=True)
    i = 0
    while i < len(lines):
        line = lines[i]
        tok = first_token(line) if line[:1].isalpha() else ""
        if tok in names:
            out.append(line)
            i += 1
            while i < len(lines) and not starts_block(lines[i]):
                out.append(lines[i])
                i += 1
            continue
        if tok in header_only:
            out.append(line)
        i += 1
    return b"".join(out)


def main(oracle: str) -> None:
    here = os.path.dirname(os.path.abspath(__file__))
    cases = []
    for jf, gdir, name_of in (
        ("materials_rw.json", "golden", lambda lab: os.path.splitext(lab.split(" ", 2)[2])[0]),
        ("variants_rw.json", os.path.join("golden", "variants"),
         lambda lab: os.path.splitext(lab.split(" ", 2)[2].replace("key/", "", 1).replace("/", "__"))[0]),
    ):
        for case in json.load(open(os.path.join(oracle, jf), encoding="utf-8")):
            stem = name_of(case["label"])
            golden = os.path.join(oracle, gdir, stem + ".OUT.KEY")
            if not os.path.exists(golden):
                print("no golden for", stem)
                continue
            inp = case["inputs"][0]["text"].encode("latin-1")
            out = open(golden, "rb").read()
            expected = blocks(out, MINE)
            if not expected:
                continue
            safe = re.sub(r"[^A-Za-z0-9_.-]", "_", stem)
            open(os.path.join(here, safe + ".in.key"), "wb").write(
                blocks(inp, MINE, header_only=[g for g in GROUP4 if g not in MINE]))
            open(os.path.join(here, safe + ".out.key"), "wb").write(expected)
            cases.append(safe)
    open(os.path.join(here, "cases.txt"), "w", newline="\n").write("\n".join(cases) + "\n")
    print(len(cases), "cases")


if __name__ == "__main__":
    main(sys.argv[1])
