# Model of GFREAD STL (FUN_140244fa0 + FUN_140246ad0), see docs/re/commands/GFREAD_STL_GEO_NAS.md section 2.
# Usage: python -I gfread_stl_model.py FILE.STL OBJNO  -> prints the DIEGEO block KFWRIT writes.
# Simplifications: well-formed ASCII only; the stale extra facet of an endsolid line is not
# modelled (it always collapses); the bounds message and the exit(1) case are only flagged.
import struct, sys, math, re

def f32(x):
    return struct.unpack('<f', struct.pack('<f', x))[0]

FLTBIG = f32(1e29)

def detect(path):
    b = open(path, 'rb').read()
    # text mode: CRLF->LF and ^Z = EOF; approximate: stop at 0x1A
    t = b.replace(b'\r\n', b'\n')
    if b'\x1a' in t:
        t = t[:t.index(b'\x1a')]
    t = t[:255]
    if any(c > 0x7f for c in t):
        return 1
    return 0 if b'facet normal' in t.lower() else 1

def read_ascii(path):
    toks = open(path).read().split('\n')
    # simple tokenised model of the fscanf sequence for well-formed files
    data = open(path).read()
    lines = data.split('\n')
    pts = []; tris = []
    it = iter(lines[1:])
    words = ' '.join(lines[1:]).split()
    i = 0
    while i < len(words):
        if words[i].lower() != 'facet':
            break
        i += 5  # facet normal nx ny nz
        i += 2  # outer loop
        ids = []
        for k in range(3):
            v = [f32(float(words[i+1])), f32(float(words[i+2])), f32(float(words[i+3]))]
            i += 4
            pts.append(v); ids.append(len(pts))
        i += 2  # endloop endfacet
        tris.append(ids)
    return pts, tris

def read_bin(path):
    b = open(path, 'rb').read()
    pos = 84; pts = []; tris = []
    while pos + 48 <= len(b):
        v = struct.unpack_from('<12f', b, pos)
        pos += 48 + 2
        ids = []
        for k in range(3):
            pts.append(list(v[3+3*k:6+3*k])); ids.append(len(pts))
        tris.append(ids)
    return pts, tris

def bounds(pts):
    mn = [FLTBIG]*3; mx = [-FLTBIG]*3
    for p in pts:
        for c in range(3):
            if p[c] < mn[c]: mn[c] = p[c]
            if mx[c] < p[c]: mx[c] = p[c]
    return mn, mx

def merge(pts, tris, tol):
    mn, mx = bounds(pts)
    dims = [100, 100, 100]
    for c in range(3):
        if mx[c] <= mn[c]:
            mx[c] = f32(mn[c] + 1.0)
    h = [f32(f32(mx[c]-mn[c]) / 100.0) for c in range(3)]
    tol = f32(tol); tol2 = f32(tol*tol)
    minh = FLTBIG
    for c in range(3):
        if not (minh < h[c] or minh == h[c]):
            minh = h[c]
    level = int(math.ceil(tol / minh))
    level = min(level, 100)
    buckets = {}
    newpts = []
    def bidx(p):
        r = []
        for c in range(3):
            a = f32(p[c]-mn[c]) * 0.999 / f32(mx[c]-mn[c])
            r.append(int(f32(f32(a) * 100.0)))
        return r
    def dist2(a, b):
        dx = f32(a[0]-b[0]); dy = f32(a[1]-b[1]); dz = f32(a[2]-b[2])
        return f32(f32(f32(dx*dx) + f32(dy*dy)) + f32(dz*dz))
    def insert(p):
        ix = bidx(p)
        key = ix[0] + ix[1]*100 + ix[2]*10000
        for r in range(level+1):
            if r == 0:
                nb = [ix]
            else:
                lo = [max(ix[c]-r, 0) if ix[c]-r >= 1 else 0 for c in range(3)]
                hi = [min(ix[c]+r, dims[c]-1) for c in range(3)]
                nb = []
                for x in range(lo[0], hi[0]+1):
                    for y in range(lo[1], hi[1]+1):
                        for z in range(lo[2], hi[2]+1):
                            if x in (ix[0]+r, ix[0]-r) or y in (ix[1]+r, ix[1]-r) or z in (ix[2]+r, ix[2]-r):
                                nb.append([x, y, z])
            for q in nb:
                k2 = q[0] + q[1]*100 + q[2]*10000
                for pid in buckets.get(k2, []):
                    if dist2(p, newpts[pid-1]) <= tol2:
                        return pid
        newpts.append(p)
        buckets.setdefault(key, []).append(len(newpts))
        return len(newpts)
    cells = []
    for t in tris:
        m = [insert(pts[i-1]) for i in t]
        if m[0] != m[1] and m[0] != m[2] and m[1] != m[2]:
            cells.append(m)
    return newpts, cells

def orient(npts, cells):
    sys.setrecursionlimit(100000)
    links = [[] for _ in range(npts)]
    for ci, c in enumerate(cells):
        for v in c:
            links[v-1].append(ci+1)
    patch = [0]*len(cells)
    st = {'b8': 0, 'bc': 0, 'pending': [], 'reversed': [], 'invalid': []}
    def nbrs(c, a, b):
        out = []
        for n in links[a-1]:
            if n != c and b in cells[n-1]:
                if n not in out: out.append(n)
        return out
    def fa70(c, p):
        patch[c-1] = p
        st['b8'] += 1
        if st['b8'] > 1000:
            st['pending'].append(c); return
        for k in range(3):
            cell = cells[c-1]
            a = cell[k]; b = cell[(k+1) % 3]
            if a == b: continue
            L = nbrs(c, a, b)
            if len(L) >= 2:
                st['invalid'].append(c)
            elif len(L) == 1 and patch[L[0]-1] == 0:
                n = L[0]; nc = cells[n-1]
                j = 0
                while j < 3 and nc[j] != b: j += 1
                if nc[(j+1) % 3] != a:
                    st['reversed'].append(n)
                    nc[0], nc[2] = nc[2], nc[0]
                fa70(n, p)
        st['b8'] -= 1
    p = 1
    while True:
        try:
            s = patch.index(0) + 1
        except ValueError:
            break
        st['pending'] = []; st['b8'] = 0
        fa70(s, p)
        i = 0
        while i < len(st['pending']):
            st['b8'] = 0
            fa70(st['pending'][i], p)
            i += 1
        p += 1
    return st

def E(x):
    s = '%20.10E' % x
    m = re.match(r'(.*E)([+-])(\d+)$', s.strip())
    s2 = '%s%s%03d' % (m.group(1), m.group(2), int(m.group(3)))
    return s2.rjust(20)

def main():
    path, objn = sys.argv[1], int(sys.argv[2])
    binflag = detect(path)
    pts, tris = (read_bin if binflag else read_ascii)(path)
    mn, mx = bounds(pts)
    d = [f32(abs(f32(mx[c]-mn[c]))) for c in range(3)]
    if any(x > 1e10 for x in d):
        print('BOUNDS FAIL', len(pts), len(tris)); return
    npt, cells = merge(pts, tris, 1e-5)
    st = orient(len(npt), cells)
    sys.stderr.write('binary=%d raw=%d merged=%d tris=%d cells=%d reversed=%d invalid=%d\n' % (
        binflag, len(pts), len(npt), len(tris), len(cells), len(st['reversed']), len(st['invalid'])))
    if not cells:
        print('STORE FAIL'); return
    out = ['%-6s %7i %7i %7i %7i' % ('DIEGEO', objn, 1, len(npt), len(cells))]
    for i, p in enumerate(npt):
        out.append(' %7i %s %s %s' % (i+1, E(p[0]), E(p[1]), E(p[2])))
    for i, c in enumerate(cells):
        out.append(' %7i %7i %7i %7i %7i' % (i+1, c[0], c[1], c[2], c[0]))
    print('\n'.join(out))

main()
