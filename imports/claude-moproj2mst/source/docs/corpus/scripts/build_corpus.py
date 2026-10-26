# Copies the DEF_PRE test corpus out of the DEFORM install (originals are only read) and
# writes manifest.tsv. Usage: python -I build_corpus.py [corpus_dir]
import hashlib, os, shutil, sys
from pathlib import Path
DEF = Path(r"C:\Program Files\SFTC\DEFORM\v11.0.2")
OUT = Path(sys.argv[1] if len(sys.argv) > 1 else r"C:\Users\alext\Projects\DEFORM_corpus")
rows = []
import re
CMD = re.compile(r"^(PROBID|RUNTYP|DBREAD|KFREAD|GENDB|INICTC|GENCTC|OBJPOS|TLOC|WRTOBJ|KFWRIT|EXECMD|SVSCAL|CPYOBJ|DOESCL|GENINT|IMPLUB|IMPEQP|TCYCLE|GENAXS|GENSUB|ARMCTL|DSTROKE|ACTOUT|CNVT3D|REMESH|BRDEXT|DEFAMG|DEFINT|GENBCC|DEFFRZ|MORP|GFREAD)( |$)", re.M)
def sha(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 22), b""): h.update(b)
    return h.hexdigest()
def add(src, rel, kind, dim, units, set_):
    dst = OUT / rel
    dst.parent.mkdir(parents=True, exist_ok=True)
    if not dst.exists() or dst.stat().st_size != src.stat().st_size:
        shutil.copy2(src, dst)
    h = sha(dst)
    assert h == sha(src), src
    rows.append((rel.as_posix(), kind, dim, units, set_, str(dst.stat().st_size), h, str(src)))
# DBs
for p in sorted(Path(r"C:\PROBLEM\TEST_REA").glob("*/*.DB")):
    add(p, Path("db") / p.name, "DB", "3D", "SI", "primary")  # project SimCtrl.KEY: UNIT 1
# KEY examples
for d in ("2D", "3D"):
    root = DEF / d / f"{d}_Examples"
    for p in sorted(root.rglob("*")):
        if not p.is_file(): continue
        ext = p.suffix.upper()
        if ext in (".KEY", ".KWD"): kind = ext[1:]
        elif ext in (".MST", ".D2MO", ".D3MO", ".INI") and CMD.search(p.read_text(encoding="latin-1")): kind = "CMD"
        else: continue
        r = p.relative_to(root)
        u = r.parts[0]
        units = {"SI": "SI", "EN": "English"}.get(u, "?")
        add(p, Path("key") / d / r, kind, d, units, "primary" if units == "SI" else "optional")
# Materials
mat = DEF / "MaterialLibrary" / "Standard"
for stem, sub in [("IN718_6um[1650-2200F(900-1200C)]", "Superalloy"), ("JIS-S45C(HeatTreatment)", "Steel"),
                  ("AISI-1020(Machining)", "Steel"), ("AISI4340-JohnsonCook", "Steel")]:
    for p in sorted((mat / sub).iterdir()):
        if not p.name.startswith(stem) or p.name.startswith(stem + ","): continue
        if p.suffix.upper() == ".KEY":
            units = "SI" if "_s0" in p.name else "English"
            add(p, Path("materials") / p.name, "KEY", "-", units, "primary" if units == "SI" else "optional")
        else:
            add(p, Path("materials") / p.name, p.suffix.upper()[1:], "-", "-", "primary")
with open(OUT / "manifest.tsv", "w", encoding="utf-8", newline="\n") as f:
    f.write("path\tkind\tdim\tunits\tset\tsize\tsha256\tsource\n")
    for r in rows: f.write("\t".join(r) + "\n")
print(len(rows), "files")
