"""Shared corpus line checker for the moproj2mst packets (stage 1, plan section 4).

A packet describes the lines its emitters write as a *forms module* (tools/moproj2mst/forms/<pkt>.py);
this script checks that every such line, in every oracle-A MST of the corpus, has exactly one of the
forms the emitters can produce. It reads the corpus only.

Forms module API (plain Python, stdlib only):

    FORMS = [Form(...), ...]          # required
    def init_file(f): ...             # optional: before the line scan (set up f.state)
    def check_file(f): ...            # optional: whole-file checks, after the line scan

    Form(name, prefix, lines=None, check=None, simtyp=None)
      prefix   the exact leading bytes that select the form (e.g. "DBREAD    \\t"); the first form
               whose prefix a line starts with owns the line, so use the full literal with its padding
      lines    alternatives; each alternative is a list of regexes for this line and the lines after
               it (fullmatch). "{sim}" in a regex is the current SIMULATION number. Tokens:
               {INT} integer, {NUM} a number as QTextStream writes a double (%g-like), {NAME} a file
               name without '/' or '\\\\', {PATH} a non-empty relative path with '/'.
      check    callable(f, i) -> error text or None, for forms a regex table cannot say
      simtyp   optional regex: only lines inside simulations whose *!SIMTYP matches are checked

    The File object `f` has: name, lines (list of str, LF split, Latin-1), sim (current SIMULATION
    number at the line), simtyp (current type), sims (list of numbers seen), counts (dict), state (dict
    for the module's own use), at(i) (line i or None), err(i, text).

Usage: python -I corpus_lines.py <forms module path or packet id> [--corpus DIR] [--quiet]
       python -I corpus_lines.py --self-test     (the checker finds planted errors; no corpus needed)
Exit: 0 pass, 1 problems, 77 corpus missing (ctest skip).
"""
import argparse, importlib.util, os, pathlib, re, sys

HERE = pathlib.Path(__file__).resolve().parent
DEFAULT_CORPUS = os.environ.get("M2M_CORPUS", r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus")

RULE = "*" + "-" * 73 + "*"
TOKENS = {
    "{INT}": r"-?\d+",
    "{NUM}": r"(?:-?(?:\d+(?:\.\d*[1-9])?|\.\d+)(?:e[+-]\d{2,3})?|-?inf|nan)",
    "{NAME}": r"[^/\\\n]+",
    "{PATH}": r"[^\n\\]+",
}


class Form:
    def __init__(self, name, prefix, lines=None, check=None, simtyp=None):
        self.name, self.prefix, self.check = name, prefix, check
        self.simtyp = re.compile(simtyp) if simtyp else None
        self.alts = [list(a) for a in (lines or [])]


def expand(rx, sim):
    for k, v in TOKENS.items():
        rx = rx.replace(k, v)
    return rx.replace("{sim}", str(sim))


class File:
    def __init__(self, path):
        self.path, self.name = path, path.name
        self.lines = path.read_bytes().decode("latin-1").split("\n")
        self.sim, self.simtyp, self.sims = 0, "", []
        self.counts, self.errors, self.state = {}, [], {}

    def err(self, i, text):
        line = self.lines[i] if 0 <= i < len(self.lines) else None
        self.errors.append("%s:%d: %s: %r" % (self.name, i + 1, text, line))

    def at(self, i):
        return self.lines[i] if 0 <= i < len(self.lines) else None


def load_forms(spec):
    if str(HERE) not in sys.path:
        sys.path.insert(0, str(HERE))   # forms modules import corpus_lines (python -I drops it)
    p = pathlib.Path(spec)
    if not p.suffix:
        p = HERE / "forms" / (spec + ".py")
    sp = importlib.util.spec_from_file_location("m2m_forms_" + p.stem, p)
    mod = importlib.util.module_from_spec(sp)
    sys.modules[sp.name] = mod
    sp.loader.exec_module(mod)
    return mod


def check_file(path, mod):
    f = File(path)
    forms = mod.FORMS
    if hasattr(mod, "init_file"):
        mod.init_file(f)
    for i, ln in enumerate(f.lines):
        m = re.fullmatch(r"SIMULATION\t(\d+)", ln)
        if m:
            f.sim = int(m.group(1))
            f.sims.append(f.sim)
            t = f.at(i + 1) or ""
            f.simtyp = t[len("*!SIMTYP  \t"):] if t.startswith("*!SIMTYP  \t") else ""
        form = next((x for x in forms if ln.startswith(x.prefix)), None)
        if form is None or (form.simtyp and not form.simtyp.fullmatch(f.simtyp)):
            continue
        ok = True
        if form.alts:
            ok = any(all(f.at(i + k) is not None and re.fullmatch(expand(rx, f.sim), f.at(i + k))
                         for k, rx in enumerate(alt)) for alt in form.alts)
            if not ok:
                f.err(i, form.name)
        if ok and form.check:
            e = form.check(f, i)
            if e:
                f.err(i, "%s: %s" % (form.name, e))
                ok = False
        if ok:
            f.counts[form.name] = f.counts.get(form.name, 0) + 1
    if hasattr(mod, "check_file"):
        mod.check_file(f)
    return f


SELF_TEST_MST = "\n".join([
    "", RULE, "SIMULATION\t1", "*!SIMTYP  \tSfOprHeatTransfer", RULE,
    "DIM    \t3", "NSTART    \t-1",
    "CURSIM    \t1\t1\t0", "GENDB     \t2\t1\t0", "A.DB",
    "", RULE, "SIMULATION\t3", "*!SIMTYP  \tSfOprHeatTransfer", RULE,   # numbering gap
    "DBREAD    \t0", "dir/A.DB",                                       # path, not a file name
    "CURSIM    \t2\t1\t0",                                            # wrong simulation number
    "GENDB     \t2\t1\t0", "B.DB",                                    # sim-1 form, second DB
    "",
])
SELF_TEST_EXPECT = {"DBREAD", "CURSIM", "GENDB", "SIMULATION numbers", "more than one file",
                    "simulations but"}


def self_test():
    import tempfile
    mod = load_forms("p08")
    with tempfile.TemporaryDirectory() as d:
        p = pathlib.Path(d) / "SELF.MST"
        p.write_bytes(SELF_TEST_MST.encode("latin-1"))
        f = check_file(p, mod)
    found = {k for k in SELF_TEST_EXPECT if any(k in e for e in f.errors)}
    for e in f.errors:
        print(e)
    ok = found == SELF_TEST_EXPECT and f.counts.get("DIM") == 1 and f.counts.get("SIMULATION") == 2
    print("self-test", "PASS" if ok else "FAIL: missing %s" % sorted(SELF_TEST_EXPECT - found))
    return 0 if ok else 1


def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    if argv == ["--self-test"]:
        return self_test()
    ap = argparse.ArgumentParser()
    ap.add_argument("forms", help="forms module path, or a packet id under tools/moproj2mst/forms")
    ap.add_argument("--corpus", default=DEFAULT_CORPUS)
    ap.add_argument("--quiet", action="store_true", help="print at most 20 problems")
    a = ap.parse_args(argv)
    mod = load_forms(a.forms)
    root = pathlib.Path(a.corpus)
    if not root.is_dir():
        print("skipped: no corpus at", root)
        return 77
    msts = sorted(p for p in root.rglob("*") if p.suffix.lower() == ".mst" and p.is_file())
    total, errors = {}, []
    for p in msts:
        f = check_file(p, mod)
        errors += f.errors
        for k, v in f.counts.items():
            total[k] = total.get(k, 0) + v
    for e in errors[:20] if a.quiet else errors:
        print(e)
    names = [x.name for x in mod.FORMS]
    print("%d MST files; lines checked: %s" % (len(msts), ", ".join(
        "%s %d" % (n, total.get(n, 0)) for n in dict.fromkeys(names))))
    print("FAIL: %d problems" % len(errors) if errors else "PASS")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
