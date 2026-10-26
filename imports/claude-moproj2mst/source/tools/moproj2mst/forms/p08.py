"""P08 line forms: shared emitters A (s1-02 6, apps/moproj2mst/src/mst/emit_a.cpp).

SIMULATION header 140131c10, DIM/NSTART 140134250/360, DBREAD 1401343e0, KFREAD (140134510,
140134680, 140134bf0, 140134db0 and every other KFREAD writer), MDSOBJ/TNOW 140134bf0, OPRNAM
140134730, SIMNAM 1401347e0, CURSIM 140134900, GENDB 140134a10.
"""
import re
from corpus_lines import Form, RULE  # noqa: F401  (corpus_lines is the loader's module)

MDSOBJ = "MDSOBJ    \t  0       0       0    0.0000000000E+000"
TNOW = "TNOW\t   \t  -1E21       0       1"


def _simulation(f, i):
    if f.at(i - 1) != RULE or f.at(i - 2) != "":
        return "not preceded by an empty line and the rule"
    return None


def _dim(f, i):
    return None if f.sim == 1 else "DIM outside simulation 1"


def _nstart(f, i):
    return None if (f.at(i - 1) or "").startswith("DIM    \t") else "NSTART without DIM"


def _dbread(f, i):
    f.state["db"].add(f.at(i + 1))
    return None if f.sim != 1 else "DBREAD in simulation 1"


def _tnow(f, i):
    return None if f.at(i - 1) == MDSOBJ else "TNOW without MDSOBJ"


def _gendb(f, i):
    rx = r"GENDB     \t2\t[12]\t0" if f.sim == 1 else r"GENDB     \t1\t[12]\t-?\d+"
    f.state["db"].add(f.at(i + 1))
    return None if re.fullmatch(rx, f.at(i)) else "wrong values for simulation %d" % f.sim


FORMS = [
    Form("SIMULATION", "SIMULATION",
         [[r"SIMULATION\t{sim}", r"\*!SIMTYP  \t\S.*", re.escape(RULE)]], _simulation),
    Form("DIM", "DIM    \t", [[r"DIM    \t{INT}", r"NSTART    \t{INT}"]], _dim),
    Form("NSTART", "NSTART    \t", [[r"NSTART    \t{INT}"]], _nstart),
    Form("DBREAD", "DBREAD    \t", [[r"DBREAD    \t{INT}", r"{NAME}"]], _dbread),
    Form("MDSOBJ", "MDSOBJ    \t",
         [[re.escape(MDSOBJ), re.escape(TNOW), r"KFREAD    \t1", r"{PATH}/SimCtrl\.KEY"]]),
    Form("TNOW", "TNOW\t   \t", [[re.escape(TNOW)]], _tnow),
    Form("KFREAD", "KFREAD    \t", [[r"KFREAD    \t1", r".+"]]),
    Form("OPRNAM", "OPRNAM", [[r"OPRNAM", r".+"]]),
    Form("SIMNAM", "SIMNAM", [[r"SIMNAM", r".+"]]),
    Form("CURSIM", "CURSIM    \t", [[r"CURSIM    \t{sim}\t\d+\t0"]]),
    Form("GENDB", "GENDB     \t", [[r"GENDB     \t{INT}\t{INT}\t{INT}", r"{NAME}"]], _gendb),
]


def init_file(f):
    f.state["db"] = set()


def check_file(f):
    if f.sims != list(range(1, len(f.sims) + 1)):
        f.errors.append("%s: SIMULATION numbers %s" % (f.name, f.sims))
    names = {n for n in f.state["db"] if n is not None}
    if len(names) > 1:
        f.errors.append("%s: DBREAD/GENDB name more than one file: %s" % (f.name, sorted(names)))
    for k in ("CURSIM", "GENDB", "OPRNAM", "SIMNAM"):
        if f.counts.get(k, 0) != len(f.sims):
            f.errors.append("%s: %d simulations but %d %s" % (f.name, len(f.sims), f.counts.get(k, 0), k))
