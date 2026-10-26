"""fmatch.py: match functions of DEF_GUI_MO_64.exe to DEF_PRE_64.exe (same DEFORM engine sources)
by a masked byte signature: every .pdata function's bytes with the 4 bytes after E8/E9 and after a
RIP-relative ModRM (mod 00, rm 101) zeroed. Unique signature in both images -> match.
usage: python -I fmatch.py [gui_va ...]   (no args: write fmatch.tsv next to this script)
Reads the installed DEF_PRE_64.exe (read only)."""
import hashlib, os, struct, sys

GUI = 'C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'
PRE = 'C:/Program Files/SFTC/DEFORM/v11.0.2/DEF_PRE_64.exe'
B = 0x140000000


def funcs(path):
    d = open(path, 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3c)[0]
    ns = struct.unpack_from('<H', d, pe + 6)[0]
    sh = pe + 24 + struct.unpack_from('<H', d, pe + 20)[0]
    secs = []
    for i in range(ns):
        o = sh + 40 * i
        vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8)
        secs.append((d[o:o + 8].rstrip(b'\0').decode(), va, vs, ra, rs))

    def off(r):
        for n, va, vs, ra, rs in secs:
            if va <= r < va + rs:
                return ra + r - va
        return None
    pd = [s for s in secs if s[0] == '.pdata'][0]
    out = {}
    seen = set()
    for k in range(0, pd[4], 12):
        a, b, _ = struct.unpack_from('<III', d, pd[3] + k)
        if a == 0:
            break
        if a in seen:
            continue
        seen.add(a)
        o = off(a)
        if o is None or b <= a:
            continue
        body = bytearray(d[o:o + (b - a)])
        i = 0
        n = len(body)
        while i < n:
            c = body[i]
            if c in (0xe8, 0xe9) and i + 5 <= n:
                body[i + 1:i + 5] = b'\0\0\0\0'
                i += 5
                continue
            if (c & 0xc7) == 0x05 and i >= 1 and i + 5 <= n:
                body[i + 1:i + 5] = b'\0\0\0\0'
                i += 5
                continue
            i += 1
        out[a + B] = hashlib.sha1(bytes(body)).hexdigest() + ':%d' % len(body)
    return out


def index(f):
    by = {}
    for a, h in f.items():
        by.setdefault(h, []).append(a)
    return by


g = funcs(GUI)
p = funcs(PRE)
gi, pi = index(g), index(p)
if len(sys.argv) > 1:
    for a in sys.argv[1:]:
        va = int(a, 16)
        h = g.get(va)
        m = pi.get(h, []) if h else []
        print('%x -> %s%s' % (va, ' '.join('%x' % x for x in m) or '-', '' if len(gi.get(h, [])) == 1 else ' (sig not unique in GUI)'))
else:
    rows = []
    for h, gl in gi.items():
        pl = pi.get(h)
        if pl and len(gl) == 1 and len(pl) == 1 and int(h.split(':')[1]) >= 24:
            rows.append((gl[0], pl[0], int(h.split(':')[1])))
    rows.sort()
    with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'fmatch.tsv'), 'w') as f:
        f.write('gui\tpre\tsize\n')
        for r in rows:
            f.write('%x\t%x\t%d\n' % r)
    print(len(g), len(p), 'unique matches', len(rows))
