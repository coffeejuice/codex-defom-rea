"""P09 line forms: shared emitters B (s1-02 6.2-6.4, apps/moproj2mst/src/mst/emit_b.cpp).

OBJTYP/KFREAD/MTLGRP 140135220/680, closing OBJTYP-0 140135b80, CNTACT 140135da0, REMESH 1401360e0,
IMPEQP 140136810, TLOC 140135fc0, OBJPOS 140065f80/140065af0, INICTC/GENCTC 1400ac010, state
variables 14004e830/14004f1c0, SVSCAL 14004f050, NRECVL/ERECVL 1400613b0. KFREAD lines are in p08.
Only the exact literals with their padding select a line: Cogging's own "OBJPOS " and "URZ\\t" lines
are not these emitters'. Written by the forms author from s1-02, not by P09; P09 may refine it.
"""
import re
from corpus_lines import Form, TOKENS

NUM = TOKENS["{NUM}"]
NOT_COGGING = r"(?!SfOprCogging$).*"
SEP = ",    "                     # 5-byte separator of the NRECVL/ERECVL lists (s1-02 6.4)

# Doubles after "OBJPOS\t<obj>\t<type>\t" by type (s1-02 6.3); a missing type writes none.
COUNT_3D = {1: 7, 11: 7, 2: 7, 12: 7, 102: 7, 112: 7, 3: 7, 6: 7, 7: 7, 8: 7, 9: 7}
COUNT_2D = {1: 4, 11: 4, 2: 4, 12: 4, 102: 4, 112: 4, 3: 4, 4: 4, 6: 4}


def _objpos(f, i):
    tok = f.at(i).split("\t")
    if tok[0] != "OBJPOS" or len(tok) < 4 or not re.fullmatch(r"-?\d+", tok[1]) \
            or not re.fullmatch(r"-?\d+", tok[2]):
        return "start"
    t, rest = int(tok[2]), tok[3:]
    k = 0
    options = {COUNT_3D.get(t, 0), COUNT_2D.get(t, 0)}
    for c in sorted(options):
        if c == 0:
            if rest == [""]:
                break
            if len(rest) == 2 and rest[0] == "" and re.fullmatch(r"\d+", rest[1]):
                k = int(rest[1])
                break
        elif len(rest) in (c, c + 1) and all(re.fullmatch(NUM, v) for v in rest[:c]):
            if len(rest) == c + 1:
                if not re.fullmatch(r"\d+", rest[c]):
                    continue
                k = int(rest[c])
            break
    else:
        return "values do not fit type %d (3D %s, 2D %s)" % (t, COUNT_3D.get(t, 0), COUNT_2D.get(t, 0))
    if k > 0:
        nxt = f.at(i + 1) or ""
        if not re.fullmatch(r"-?\d+(\t-?\d+){%d}" % (k - 1), nxt):
            return "list line of %d values: %r" % (k, nxt)
    return None


def _positive(f, i):
    v = float(f.at(i).split("\t")[1])
    return None if v > 0 else "TLOC not > 0"


def _grain2d_same(f, i):
    tok = f.at(i).split("\t")
    if len(tok) == 7 and tok[5] != tok[6]:
        return "2D GRAIN writes the same value twice"
    return None


def _recv_list(f, i):
    tok = f.at(i).split("\t")
    if tok[2:4] == ["0", "0"]:     # 3D per-object form, one line
        return None
    n = int(tok[4])
    flags, vals = f.at(i + 1) or "", f.at(i + 2) or ""
    if not re.fullmatch(r"[01](%s[01]){%d}" % (re.escape(SEP), n - 1), flags):
        return "flags line for %d entries: %r" % (n, flags)
    if not re.fullmatch(r"%s(%s%s){%d}" % (NUM, re.escape(SEP), NUM, n - 1), vals):
        return "values line for %d entries: %r" % (n, vals)
    return None


def _svscal(f, i):
    k = int(f.at(i).split("\t")[3])
    for j in range(1, k + 1):
        if not re.fullmatch(r"%s\t%s" % (NUM, NUM), f.at(i + j) or ""):
            return "row %d of %d: %r" % (j, k, f.at(i + j))
    return None


def _sv(name, *alts):
    return Form(name, name + "    \t", [[a] for a in alts])


FORMS = [
    Form("OBJTYP", "OBJTYP\t\t", [[r"OBJTYP\t\t\d+\t0", r"KFREAD    \t1", r"{PATH}"]]),
    Form("OBJTYP-0", "OBJTYP    \t", [[r"OBJTYP    \t\d+\t0"]]),
    Form("MTLGRP", "MTLGRP    \t", [[r"MTLGRP    \t\d+\t0\t{INT}"]]),
    Form("CNTACT", "CNTACT    \t", [[r"CNTACT    \t0", r"KFREAD    \t1", r"{PATH}/InterObject\.KEY"]]),
    Form("REMESH", "REMESH    \t", [[r"REMESH    \t\d+"]]),
    Form("IMPEQP", "IMPEQP    \t", [[r"IMPEQP    \t\d+", r"{PATH}"]]),
    Form("TLOC", "TLOC    \t", [[r"TLOC    \t{NUM}"]], _positive),
    Form("INICTC", "INICTC    \t", [[r"INICTC    \t([1-9]|[1-9]\d|100)"]]),
    Form("GENCTC", "GENCTC    \t", [[r"GENCTC    \t{NUM}"]]),
    # Cogging writes its own OBJPOS lines (types 13/14, a trailing tab in the list line; s1-03),
    # while 140065f80/140065af0 put no tab after the last list value (s1-02 a2s.c).
    Form("OBJPOS", "OBJPOS\t", None, _objpos, simtyp=NOT_COGGING),
    _sv("NDTMP", r"NDTMP    \t\d+\t0\t{NUM}"),
    _sv("STRAIN", r"STRAIN    \t\d+\t0\t{NUM}"),
    _sv("STRESS", r"STRESS    \t\d+\t0\t{NUM}"),
    _sv("DAMAGE", r"DAMAGE    \t\d+\t0\t{NUM}"),
    _sv("URZ", r"URZ    \t\d+\t0\t{NUM}\t{NUM}\t", r"URZ    \t\d+\t0\t{NUM}\t{NUM}\t{NUM}"),
    _sv("DRZ", r"DRZ    \t\d+\t0\t{NUM}\t{NUM}\t", r"DRZ    \t\d+\t0\t{NUM}\t{NUM}\t{NUM}"),
    Form("GRAIN", "GRAIN    \t", [[r"GRAIN    \t\d+\t0\t16\t0\t{NUM}\t{NUM}"],
                                  [r"GRAIN    \t\d+\t0\t16\t0\t0\t{NUM}"]], _grain2d_same),
    _sv("SIZESH", r"SIZESH    \t\d+\t{NUM}\t0\t0\t0"),
    _sv("DENSTY", r"DENSTY    \t\d+\t0\t{NUM}"),
    _sv("AVGSTR", r"AVGSTR    \t\d+\t{NUM}"),
    _sv("LMTSTR", r"LMTSTR    \t\d+\t{NUM}"),
    Form("NRECVL", "NRECVL    \t", [[r"NRECVL    \t\d+\t0\t1\t[1-9]\d*"],
                                    [r"NRECVL    \t\d+\t0\t0\t\d+\t[01]"]], _recv_list),
    Form("ERECVL", "ERECVL    \t", [[r"ERECVL    \t\d+\t0\t1\t[1-9]\d*"],
                                    [r"ERECVL    \t\d+\t0\t0\t\d+\t{NUM}"]], _recv_list),
    Form("SVSCAL", "SVSCAL    \t", [[r"SVSCAL    \t\d+\t1\t\d+"]], _svscal),
]
