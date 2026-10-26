"""Build group2-index.md from the registry, the case map, the corpus index and the spec files.

usage: python -I g2_index.py <keyword_registry.tsv> <casemap.tsv> <corpus_kw.tsv> <keywords_dir>
Prints Markdown to stdout. The one-line summary is the first non-empty paragraph line after the
spec's title.
"""
import os, sys
sys.stdout.reconfigure(encoding="utf-8")

reg = [l.split("\t") for l in open(sys.argv[1], encoding="utf-8").read().splitlines()[1:]]
cases = {}
for l in open(sys.argv[2], encoding="utf-8").read().splitlines()[1:]:
    f = l.split("\t")
    if f[1] != "-":
        cases[f[2]] = f[0]
corp = {}
for l in open(sys.argv[3], encoding="utf-8").read().splitlines()[1:]:
    f = l.split("\t")
    corp[f[0]] = int(f[1])
NOT_WRITTEN = {"MOVANG": "never written", "BCCANG": "never written", "MGSIZM": "never written", "OBJPHS": "not parsed, never written",
               "GEO23": "dead-code writer only", "GEOSEC": "dead-code writer only",
               "MSHSEC": "dead-code writer only", "CRDSYS": "dead-code writer only"}
print("| Id | Keyword | Case | Corpus files | KEY output | Summary |")
print("|---:|---|---|---:|---|---|")
for r in reg:
    if r[2] != "2":
        continue
    name = r[1]
    p = os.path.join(sys.argv[4], name + ".md")
    summ = "**spec missing**"
    if os.path.exists(p):
        lines = open(p, encoding="utf-8").read().splitlines()
        for l in lines[1:]:
            if l.strip() and not l.startswith("#"):
                summ = l.strip()
                break
        if len(summ) > 110:
            summ = summ[:107].rsplit(" ", 1)[0] + " …"
    print(f"| {r[0]} | [{name}]({name}.md) | {cases.get(name, 'none')} | {corp.get(name, 0)} | "
          f"{NOT_WRITTEN.get(name, 'written')} | {summ.replace('|', '/')} |")
