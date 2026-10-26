"""Map the top-level switch of FUN_14038b8a0 (group-2 KEY parser) to keywords.

usage: python -I g2_casemap.py <p_main.c> <keyword_registry.tsv>
p_main.c is the Ghidra decompile of FUN_14038b8a0 alone (Decomp.java depth 0).
Prints one TSV row per top-level case: case, id, keyword, first line, last line,
helpers called, object offsets written (local_1080 + off).
"""
import re, sys

src = open(sys.argv[1], encoding="utf-8", errors="replace").read().split("\n")
names = {}
for row in open(sys.argv[2], encoding="utf-8").read().split("\n")[1:]:
    f = row.split("\t")
    if len(f) > 3 and f[2] == "2":
        names[int(f[0])] = f[1]

# top-level case labels are indented by exactly 4 spaces inside "switch(local_68)"
start = next(i for i, l in enumerate(src) if "switch(local_68)" in l)
labels = []
for i in range(start, len(src)):
    m = re.match(r"^    (case (0x[0-9a-f]+|\d+)|default):", src[i])
    if m:
        labels.append((i, m.group(2)))
end = next(i for i in range(len(src) - 1, 0, -1) if src[i].startswith("    local_108c = iVar2;"))
print("case\tid\tkeyword\tline_from\tline_to\thelpers\toffsets")
for k, (ln, c) in enumerate(labels):
    stop = labels[k + 1][0] if k + 1 < len(labels) else end
    body = "\n".join(src[ln:stop])
    if c is None:
        print(f"default\t-\t-\t{ln+1}\t{stop}\t-\treturn 2")
        continue
    cv = int(c, 0)
    kid = cv + 0x90
    helpers = sorted(set(re.findall(r"FUN_([0-9a-f]{9})", body)) - {"1400015e0", "1400015f0"})
    offs = sorted(set(int(o, 16) for o in re.findall(r"local_1080 \+ (0x[0-9a-f]+)", body)))
    print(f"0x{cv:02x}\t{kid}\t{names.get(kid, '?')}\t{ln+1}\t{stop}\t{' '.join(helpers)}\t"
          + " ".join(hex(o) for o in offs))
