# usage: python -I rip.py <lo>[-<hi>] ... ; every RIP-relative operand (any instruction, disp32 followed
# by 0/1/2/4 immediate bytes) whose target falls in [lo, hi), grouped by containing function (.pdata).
# Heuristic (no decoder): a hit needs ModRM mod=00 rm=101 right before the disp; false positives are rare.
import bisect, struct, sys
EXE = 'C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'
B = 0x140000000
d = open(EXE, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]; opt = pe + 24
ns = struct.unpack_from('<H', d, pe + 6)[0]; sh = opt + struct.unpack_from('<H', d, pe + 20)[0]
secs = []
for i in range(ns):
    o = sh + 40 * i
    vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8)
    secs.append((d[o:o + 8].rstrip(b'\0').decode(), va, vs, ra, rs))
t = secs[0]
tx = memoryview(d)[t[3]:t[3] + t[4]]
tva = t[1]
pd = [s for s in secs if s[0] == '.pdata'][0]
fs, fe = [], []
for k in range(0, pd[4], 12):
    a, b, _ = struct.unpack_from('<III', d, pd[3] + k)
    if a == 0:
        break
    fs.append(a); fe.append(b)


def fn(r):
    k = bisect.bisect_right(fs, r) - 1
    return fs[k] if k >= 0 and r < fe[k] else None


ranges = []
for a in sys.argv[1:]:
    lo, _, hi = a.partition('-')
    lo = int(lo, 16); hi = int(hi, 16) if hi else lo + 1
    ranges.append((lo - B, hi - B))
lo_all = min(r[0] for r in ranges); hi_all = max(r[1] for r in ranges)
import array
n = len(tx)
hits = {}
for i in range(1, n - 4):
    m = tx[i - 1]
    if (m & 0xc7) != 0x05:
        continue
    disp = struct.unpack_from('<i', tx, i)[0]
    for k in (0, 1, 2, 4):
        tgt = tva + i + 4 + k + disp
        if lo_all <= tgt < hi_all and any(a <= tgt < b for a, b in ranges):
            f = fn(tva + i)
            hits.setdefault(f, []).append((tva + i - 2 + B, tgt + B, k))
for f in sorted(hits, key=lambda x: x or 0):
    print('%x:' % (B + f if f else 0), ' '.join('%x->%x' % (s, g) for s, g, k in hits[f][:12]))
