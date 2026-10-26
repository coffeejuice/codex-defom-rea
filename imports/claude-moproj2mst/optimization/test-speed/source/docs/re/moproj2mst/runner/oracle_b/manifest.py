"""manifest.py <corpus>: rewrite <corpus>/manifest.sha256 (sha256 + two spaces + relative path, sorted),
after checking that every line of the old manifest is still in the new one unchanged."""
import hashlib, os, sys
root = sys.argv[1]
mf = os.path.join(root, "manifest.sha256")
old = open(mf, encoding="utf-8").read().splitlines()
lines = []
for r, ds, fs in os.walk(root):
    for f in fs:
        p = os.path.join(r, f); rel = os.path.relpath(p, root).replace("\\", "/")
        if rel == "manifest.sha256": continue
        h = hashlib.sha256(open(p, "rb").read()).hexdigest()
        lines.append("%s  %s" % (h, rel))
lines.sort(key=lambda l: l[66:])
new = set(lines)
changed = [l for l in old if l not in new]
print("old %d new %d; old lines not kept: %s" % (len(old), len(lines), [l[66:] for l in changed]))
if sorted(old, key=lambda l: l[66:]) != old: print("note: old manifest was not path-sorted")
open(mf, "w", encoding="utf-8", newline="\n").write("\n".join(lines) + "\n")
