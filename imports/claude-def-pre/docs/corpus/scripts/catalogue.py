# Keyword coverage catalogue of the test corpus, from the files' text/structure only.
# Usage: python -I catalogue.py <repo> <corpus_dir> <deformdb.dll>
import ctypes as C, re, sys, collections
from pathlib import Path
REPO = Path(sys.argv[1]); CORPUS = Path(sys.argv[2]); DLL = sys.argv[3]
OUT = REPO / "docs" / "corpus"; OUT.mkdir(parents=True, exist_ok=True)

reg = {}
for line in (REPO / "docs/re/keyword_registry.tsv").read_text().splitlines()[1:]:
    f = line.split("\t"); reg[f[1]] = f[3]
manifest = [l.split("\t") for l in (CORPUS / "manifest.tsv").read_text(encoding="utf-8").splitlines()]
hdr, manifest = manifest[0], [dict(zip(manifest[0], r)) for r in manifest[1:]]

# ---- KEY / KWD files: registered keywords at the start of a line ----
cand = re.compile(r"^[A-Z][A-Z0-9]{2,7}$")
key_rows, unreg = [], collections.Counter()
kw_files = collections.defaultdict(lambda: collections.Counter())  # kw -> Counter(set/units)
kw_example, kw_example_opt = {}, {}
for m in manifest:
    if m["kind"] not in ("KEY", "KWD", "CMD"): continue
    text = (CORPUS / m["path"]).read_text(encoding="latin-1")
    found, header, unit, seen_unreg = {}, "", "", set()
    for line in text.splitlines():
        if not line or line[0] in " \t": continue
        if line[0] == "*":
            if not header and "DEFORM" in line.upper(): header = line.strip("* \t")[:40]
            continue
        tok = line.split()[0]
        if tok in reg:
            found[tok] = found.get(tok, 0) + 1
            if tok == "UNIT" and not unit: unit = (line.split() + [""])[1]
        elif cand.match(tok): seen_unreg.add(tok)
    unreg.update(seen_unreg)
    tag = f'{m["dim"]}/{m["units"]}'
    for k in found:
        kw_files[k][m["set"]] += 1; kw_files[k][tag] += 1
        (kw_example if m["set"] == "primary" else kw_example_opt).setdefault(k, m["path"])
    cmds = [k for k in found if reg[k] == "command"]
    key_rows.append((m["path"], m["kind"], m["dim"], m["units"], m["set"], unit, header,
                     str(len(found)), " ".join(sorted(cmds)), " ".join(sorted(found))))
with open(OUT / "key_coverage.tsv", "w", encoding="utf-8", newline="\n") as f:
    f.write("path\tkind\tdim\tunits\tset\tUNIT_value\tdeform_header\tn_keywords\tcommands\tkeywords\n")
    for r in key_rows: f.write("\t".join(r) + "\n")

# ---- DBs through deformdb.dll ----
L = C.CDLL(DLL)
class Entry(C.Structure): _fields_ = [(n, C.c_int32) for n in "slot mesh_no step header_size mar_size a_size sim_no sim_mode dim version rec_dynamic rec_header rec_mar rec_a".split()]
class Obj(C.Structure): _fields_ = [(n, C.c_int32) for n in "number status deform_code porous first_node node_count first_elem elem_count nodes_per_elem".split()] + [("name", C.c_char * 72)]
class Field(C.Structure): _fields_ = [("data", C.c_void_p), ("rows", C.c_int64), ("width", C.c_int32), ("is_int", C.c_int32), ("row_stride", C.c_int64), ("comp_stride", C.c_int64)]
src = (REPO / "deformdb/src/model.cpp").read_text()
db_kws = re.findall(r'\{"([A-Z0-9]+)", "([A-Z0-9]+)", K::', src)
db_step_rows, db_kw = [], collections.defaultdict(lambda: [0, None, None])  # kw -> [steps, first db, first step]
for m in manifest:
    if m["kind"] != "DB": continue
    db = C.c_void_p()
    assert L.deformdb_open(str(CORPUS / m["path"]).encode(), C.byref(db)) == 0
    n = C.c_int32(); L.deformdb_entry_count(db, C.byref(n))
    for i in range(n.value):
        e = Entry(); L.deformdb_get_entry(db, i, C.byref(e))
        st = C.c_void_p()
        if L.deformdb_step_read(db, i, 3, C.byref(st)) != 0:
            db_step_rows.append((m["path"], str(e.step), str(e.sim_no), str(e.sim_mode), "-", "-", "read error")); continue
        no = C.c_int32()
        if e.mar_size == 0 or L.deformdb_step_object_count(st, C.byref(no)) != 0:
            db_step_rows.append((m["path"], str(e.step), str(e.sim_no), str(e.sim_mode), "0", "0", "(header-only step)"))
            L.deformdb_step_free(st); continue
        present = collections.Counter()
        for o in range(no.value):
            for kw, _region in db_kws:
                fld = Field()
                if L.deformdb_step_keyword(st, o, kw.encode(), C.byref(fld)) == 0 and fld.rows > 0 and fld.width > 0:
                    present[kw] += 1
        for kw in present:
            d = db_kw[kw]; d[0] += 1
            if d[1] is None: d[1], d[2] = m["path"], e.step
        db_step_rows.append((m["path"], str(e.step), str(e.sim_no), str(e.sim_mode), str(no.value), str(len(present)), " ".join(sorted(present))))
        L.deformdb_step_free(st)
    L.deformdb_close(db)
with open(OUT / "db_coverage.tsv", "w", encoding="utf-8", newline="\n") as f:
    f.write("db\tstep\tsim_no\tsim_mode\tobjects\tn_keywords\tkeywords\n")
    for r in db_step_rows: f.write("\t".join(r) + "\n")

# ---- per-keyword summary over the whole registry ----
tags = ["2D/SI", "3D/SI", "-/SI", "2D/English", "3D/English", "-/English"]
with open(OUT / "keyword_coverage.tsv", "w", encoding="utf-8", newline="\n") as f:
    f.write("keyword\tgroup\tkey_files_primary\tkey_files_optional\t" + "\t".join(tags) + "\tdb_steps\tdeformdb_reads\tcovered_primary\tcovered_any\texample\n")
    rows = []
    for k, g in reg.items():
        c = kw_files.get(k, collections.Counter()); d = db_kw.get(k, [0, None, None])
        prim = c["primary"] > 0 or d[0] > 0
        anyc = prim or c["optional"] > 0
        ex = kw_example.get(k) or kw_example_opt.get(k) or (f"{d[1]} step {d[2]}" if d[1] else "")
        rows.append((k, g, str(c["primary"]), str(c["optional"]), *[str(c[t]) for t in tags], str(d[0]),
                     "yes" if any(k == x for x, _ in db_kws) else "no", "yes" if prim else "no", "yes" if anyc else "no", ex))
        f.write("\t".join(rows[-1]) + "\n")
not_in_reg = sorted(k for k, _ in db_kws if k not in reg)
import json
json.dump({"unregistered_key_tokens": unreg.most_common(), "deformdb_keywords_not_in_registry": not_in_reg},
          open(OUT / "_extra.json", "w"), indent=0)
print("key files", len(key_rows), "db steps", len(db_step_rows))

# ---- markdown summary ----
import os
os.remove(OUT / "_extra.json")
groups = collections.OrderedDict()
for r in rows: groups.setdefault(r[1], []).append(r)
lines = ["# Keyword coverage of the test corpus", "",
         "Generated by `scripts/catalogue.py` from `manifest.tsv`, `key_coverage.tsv` and `db_coverage.tsv`.",
         "\"Primary\" = SI KEY/command files, SI material files and the two test DBs; \"optional only\" = found only in",
         "British-unit (EN) files. Counts are files (KEY side) and DB steps (DB side).", ""]
tot = len(rows); prim = sum(r[13 - 1] == "yes" for r in rows); anyc = sum(r[14 - 1] == "yes" for r in rows)
lines += [f"Registry entries: {tot}. Covered by a primary file: {prim}. Covered only by optional files: {anyc - prim}. No test file: {tot - anyc}.", ""]
lines += ["| Group | Entries | Primary | Optional only | None |", "|---|---:|---:|---:|---:|"]
for g, rs in groups.items():
    p = sum(r[12] == "yes" for r in rs); a = sum(r[13] == "yes" for r in rs)
    lines.append(f"| {g} | {len(rs)} | {p} | {a - p} | {len(rs) - a} |")
lines += ["", "## Keywords with no test file", ""]
for g, rs in groups.items():
    miss = [r[0] for r in rs if r[13] == "no"]
    if miss: lines.append(f"* **{g}** ({len(miss)}): " + ", ".join(f"`{k}`" for k in miss))
lines += ["", "## Keywords covered only by British-unit files", ""]
opt = [r for r in rows if r[12] == "no" and r[13] == "yes"]
lines += [f"* `{r[0]}` ({r[1]}): {r[3]} file(s), e.g. `{r[14]}`" for r in opt] or ["(none)"]
lines += ["", "## Command keywords", "", "| Command | Primary files | Optional files | Example |", "|---|---:|---:|---|"]
for r in groups.get("command", []):
    lines.append(f"| `{r[0]}` | {r[2]} | {r[3]} | {('`' + r[14] + '`') if r[14] else '–'} |")
lines += ["", "## Covered keywords by group", "",
          "Format: keyword (primary KEY files / DB steps).", ""]
for g, rs in groups.items():
    cov = [r for r in rs if r[12] == "yes"]
    if cov: lines.append(f"* **{g}** ({len(cov)}): " + ", ".join(f"`{r[0]}` ({r[2]}/{r[10]})" for r in cov))
extra = "\n".join([
    "", "", "## Notes", "",
    "* Line-start tokens in KEY files that are not in the registry: " +
    ", ".join(f"`{k}` ({n})" for k, n in unreg.most_common(8)) +
    " … Apart from `SRFNRG` (skipped silently by the reader) these are object/material names and reversed",
    "  strings on data lines, not keywords.",
    "* deformdb keyword names that are not registry names (deformdb's own labels for DB arrays): " +
    ", ".join(f"`{k}`" for k in not_in_reg) + ".",
    "* DB coverage only covers the node/element keywords that deformdb's typed view decodes (its keyword",
    "  table); scalar and header keywords stored in a DB step are not mapped yet, so the DB column under-reports.",
    "  The Phase 2 oracle (DBREAD + KFWRIT through the original DEF_PRE) will give the full list per step.",
    ""])
(OUT / "coverage-summary.md").write_text("\n".join(lines) + extra, encoding="utf-8")
