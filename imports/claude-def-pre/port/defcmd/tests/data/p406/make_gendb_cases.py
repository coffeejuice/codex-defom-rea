# p4-06 oracle cases: corpus KEY -> KFREAD -> GENDB (new DB, checks skipped). Resumable (cache).
import sys, json, concurrent.futures as cf
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from def_pre_oracle import Oracle, Case, Input
corpus = Path(r"C:\Users\alext\Projects\DEFORM_corpus")
rels = [l.strip() for l in open(sys.argv[2]) if l.strip()]
out = open(sys.argv[3], "w")
o = Oracle()
def one(rel):
    src = corpus / rel
    name = src.name
    c = Case.keyfile(f"KFREAD\n{name}\nGENDB 2 2\nOUT.DB\n", [Input(name, source=str(src))], label="p406gendb/" + rel)
    r = o.run(c)
    return {"rel": rel, "key": r.key if hasattr(r, "key") else None, "dir": str(getattr(r, "dir", "")),
            "exit": r.exit_code, "db": "OUT.DB" in r.files}
with cf.ThreadPoolExecutor(6) as ex:
    for res in ex.map(one, rels):
        out.write(json.dumps(res) + "\n"); out.flush()
