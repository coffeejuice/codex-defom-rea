"""P09 corpus check: in every oracle-A MST of the corpus, the lines the P09 emitters write have the
exact forms those emitters produce (s1-02 5.1, 6), and inside each base-writer simulation block they
come in the order of 0x1405f56c0 / 0x1405f5340. Reads only.

Forms (emitter): "OBJTYP\\t\\t<n>\\t0" + KFREAD 1 + Objects/... path, "MTLGRP    \\t<n>\\t0\\t<m>"
(140135220); "OBJTYP    \\t<n>\\t0" (140135b80); "CNTACT    \\t0" + KFREAD 1 + <task>/InterObject.KEY
(140135da0); KFREAD <task>/MeshSettingsNNNNN.KEY, "REMESH    \\t<n>" (1401360e0); KFREAD
<task>/BccDefNNNNN.KEY (1401363b0); "IMPEQP    \\t<n>" + path (140136810); KFREAD
<task>/ObjPropertiesNNNNN.KEY (1401365f0); "TLOC    \\t<d>" (140135fc0); OBJPOS lines with 0, 4
or 7 values and the optional id list (140065f80 / 140065af0); "INICTC    \\t<1..100>",
"GENCTC    \\t<d>" (1400ac010).

Usage: python -I corpus_lines.py [corpus dir]   (default $M2M_CORPUS or the s1 corpus copy;
exit 77 when it is missing).
"""
import os, pathlib, re, sys

DEFAULT = os.environ.get("M2M_CORPUS")
INT = r"-?\d+"
NUM = r"(?:-?(?:inf|nan|\d+(?:\.\d+)?(?:e[-+]\d\d+)?))"
# Classes whose slot +0x280 is the base writer 1405f5340 (s1-02 5), legacy names included.
BASE = re.compile(r"SfOpr(HeatTransfer|Forming|Pre|HTFurnace|AirConvect|HeatResting|HeatExpress|"
                  r"HeatDwelling|DiestressBM)")
# Order of the setup body and the base writer: category -> rank.
ORDER = ["simctrl", "material", "objtyp", "objclose", "cntact", "mesh", "bcc", "impeqp", "objprop",
         "tloc", "objpos", "contact", "oprnam"]
RANK = {k: i for i, k in enumerate(ORDER)}


def classify(lines, i):
    """Category and number of lines of the P09-relevant item starting at line i, or None."""
    ln = lines[i]
    nxt = lines[i + 1] if i + 1 < len(lines) else ""
    if ln.startswith("KFREAD    \t1"):
        p = nxt
        if p.endswith("/SimCtrl.KEY"): return "simctrl", 2
        if p.startswith("Materials/") or "/Materials/" in p: return "material", 2
        if p.endswith("/InterObject.KEY"): return "cntact-kfread", 2
        if re.search(r"/MeshSettings\d{5}\.KEY$", p): return "mesh", 2
        if re.search(r"/BccDef\d{5}\.KEY$", p): return "bcc", 2
        if re.search(r"/ObjProperties\d{5}\.KEY$", p): return "objprop", 2
        return "kfread-other", 2
    for pre, cat in (("OBJTYP\t\t", "objtyp"), ("MTLGRP", "objtyp"), ("OBJTYP    ", "objclose"),
                     ("CNTACT", "cntact"), ("REMESH", "mesh"), ("IMPEQP", "impeqp"),
                     ("TLOC", "tloc"), ("OBJPOS", "objpos"), ("INICTC", "contact"),
                     ("GENCTC", "contact"), ("OPRNAM", "oprnam")):
        if ln.startswith(pre): return cat, 1
    return None


def check_form(lines, i, errs, name):
    ln = lines[i]
    nxt = lines[i + 1] if i + 1 < len(lines) else None

    def err(what):
        errs.append("%s:%d: %s: %r" % (name, i + 1, what, ln))

    if ln.startswith("OBJTYP\t\t"):
        if not re.fullmatch(r"OBJTYP\t\t\d+\t0", ln): err("OBJTYP form")
        elif nxt != "KFREAD    \t1" or not re.fullmatch(r"\S.*\.KEY", lines[i + 2]): err("OBJTYP KFREAD")
    elif ln.startswith("OBJTYP"):
        if not re.fullmatch(r"OBJTYP    \t\d+\t0", ln): err("OBJTYP-0 form")
    elif ln.startswith("MTLGRP"):
        if not re.fullmatch(r"MTLGRP    \t\d+\t0\t" + INT, ln): err("MTLGRP form")
    elif ln.startswith("CNTACT"):
        if ln != "CNTACT    \t0" or nxt != "KFREAD    \t1" or not lines[i + 2].endswith("/InterObject.KEY"):
            err("CNTACT block")
    elif ln.startswith("REMESH"):
        if not re.fullmatch(r"REMESH    \t\d+", ln): err("REMESH form")
    elif ln.startswith("IMPEQP"):
        if not re.fullmatch(r"IMPEQP    \t\d+", ln) or not nxt: err("IMPEQP form")
    elif ln.startswith("TLOC"):
        if not re.fullmatch(r"TLOC    \t" + NUM, ln): err("TLOC form")
    elif ln.startswith("INICTC"):
        m = re.fullmatch(r"INICTC    \t(\d+)", ln)
        if not m or not 1 <= int(m.group(1)) <= 100: err("INICTC form")
    elif ln.startswith("GENCTC"):
        if not re.fullmatch(r"GENCTC    \t" + NUM, ln): err("GENCTC form")
    elif ln.startswith("OBJPOS"):
        f = ln.split("\t")
        if f[0] != "OBJPOS" or len(f) < 4 or not re.fullmatch(r"\d+", f[1]) or not re.fullmatch(INT, f[2]):
            return err("OBJPOS head")
        vals = f[3:]
        # values (0, 4 or 7), then optionally the id-list count
        for nv in (7, 4, 0):
            body = vals[:nv] if nv else []
            rest = vals[nv:] if nv else vals[1:]
            if nv == 0 and vals[0] != "":
                continue
            if nv and len(vals) < nv:
                continue
            if not all(re.fullmatch(NUM, v) for v in body):
                continue
            if nv == 0:
                rest = vals[1:]
            if rest == []:
                return
            if len(rest) == 1 and re.fullmatch(r"\d+", rest[0]):
                k = int(rest[0])
                ids = (nxt or "").split("\t")
                if len(ids) == k and all(re.fullmatch(INT, x) for x in ids):
                    return
        err("OBJPOS values/list")


def check(path):
    lines = path.read_bytes().decode("latin-1").split("\n")
    errs, counts = [], {}
    typ, last = None, -1
    for i, ln in enumerate(lines):
        if ln.startswith("*!SIMTYP"):
            typ, last = ln.split("\t", 1)[1], -1
            continue
        if typ is None or not BASE.match(typ):
            continue
        c = classify(lines, i)
        if not c:
            continue
        cat = c[0]
        if cat in ("cntact-kfread", "kfread-other"):
            continue
        counts[cat] = counts.get(cat, 0) + 1
        check_form(lines, i, errs, path.name)
        r = RANK[cat]
        if r < last:
            errs.append("%s:%d: %s after %s" % (path.name, i + 1, cat, ORDER[last]))
        last = r
        if cat == "oprnam":
            typ = None
    return errs, counts


def main():
    if len(sys.argv) < 2 and not DEFAULT:
        print("SKIP corpus lines: M2M_CORPUS unset")
        return 77
    root = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else DEFAULT)
    if not root.is_dir():
        print("corpus missing:", root)
        return 77
    # Oracle-A cases only (cases.tsv column "oracle" == "A"): the other MSTs were saved by older
    # builds (e.g. the 2014 vendor example Cogging_Multiple_Ops_Lab2 has "INICTC\t\t1").
    files = []
    with open(root / "cases.tsv", encoding="utf-8") as fh:
        head = fh.readline().rstrip("\n").split("\t")
        for row in fh:
            r = dict(zip(head, row.rstrip("\n").split("\t")))
            if r.get("oracle") == "A" and r.get("mst") and (root / r["case_id"] / r["mst"]).is_file():
                files.append(root / r["case_id"] / r["mst"])
    allerr, total = [], {}
    for f in files:
        e, c = check(f)
        allerr += e
        for k, v in c.items():
            total[k] = total.get(k, 0) + v
    for e in allerr[:40]:
        print(e)
    print("p09 corpus lines: %d MSTs, %s, %d errors" % (len(files), dict(sorted(total.items())), len(allerr)))
    return 1 if allerr else 0


if __name__ == "__main__":
    sys.exit(main())
