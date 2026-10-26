"""mbcheck.py <case dir>...: MultiBlow facts per saved case (moproj and MST), for the P14 reruns.

moproj: Type of the MultiBlow operation, name of its root property group (MultiBlowForging2D/3D),
UseRelativeEnergy, NumHeats, NumBlowsN. MST: lines naming MultiBlow, and every DOESCL line.
"""
import os, re, sys

for d in sys.argv[1:]:
    mp = [f for f in os.listdir(d) if f.lower().endswith(".moproj")][0]
    s = open(os.path.join(d, mp), encoding="utf-8").read()
    i = s.index("<Type>SfOprMultiBlow"); j = s.index("</Operation>", i); blk = s[i:j]
    typ = re.match(r"<Type>([^<]*)", blk).group(1)
    root = re.findall(r"<Name>(MultiBlowForging\w*)</Name>", blk)
    def val(n): return re.findall(r"<Name>%s</Name>\s*<TypeName>[^<]*</TypeName>\s*<Value>([^<]*)" % n, blk)
    print("== %s" % os.path.basename(d))
    print("moproj type=%s root=%s UseRelativeEnergy=%s NumHeats=%s NumBlows=%s Energy=%s"
          % (typ, root, val("UseRelativeEnergy"), val("NumHeats"), val(r"NumBlows\d+"), val(r"BlowEnergy\d+")))
    mst = os.path.join(d, mp[:-7] + ".MST")
    for k, l in enumerate(open(mst, encoding="latin-1").read().splitlines(), 1):
        if re.search(r"multi.?blow|DOESCL|NHEAT|RELENG", l, re.I): print("MST L%d: %r" % (k, l))
