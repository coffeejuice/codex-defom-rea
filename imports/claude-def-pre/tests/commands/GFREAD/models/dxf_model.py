# Reference model of DEF_PRE_64 GFREAD 2D DXF path (FUN_1404093a0 mode 300).
# Written from the decompiles; used to predict OUT.KEY DIEGEO lines.
import math, struct, re, sys

def f32(x): return struct.unpack('<f', struct.pack('<f', x))[0]
NEG1E10 = f32(-1e-10)
TWO_PI_F = 6.2831853071796
PI = 3.141592653589793; HPI = 1.5707963267948966
DEG2RAD = 0.017453292519943295; RAD2DEG = 57.29577951308232

# ---------------- Fortran runtime sin/cos/atan (0x1404b3960, 0x1404b3f48, 0x1404b3878)
SC = [0.16666666666666666, 0.008333333333333165, 0.0001984126984120184, 2.7557319210152756e-06,
      2.5052106798274583e-08, 1.605893649037159e-10, 7.642917806891047e-13, 2.7204790957888847e-15]
EPS = 1.05367121277235e-08
def fsincos(x, iscos):
    ax = abs(x); neg = 0
    if iscos: y = ax + HPI
    else:
        y = ax
        if 0.0 > x: neg = 1
    if y > 210828714.0: return 0.0
    n = int(y * 0.3183098861837907 + 0.5)
    xn = float(n)
    if n & 1: neg = 0 if neg else 1
    if iscos: xn = xn + (0.0 - 0.5)
    f = (ax - xn * 3.1416015625) - xn * -8.908910206761538e-06
    if not (abs(f) < EPS):
        g = f * f
        r = g * SC[7] - SC[6]; r = r * g; r = r + SC[5]; r = r * g; r = r - SC[4]; r = r * g
        r = r + SC[3]; r = r * g; r = r - SC[2]; r = r * g; r = r + SC[1]; r = r * g; r = r - SC[0]
        r = r * f; r = r * g; f = f + r
    return (-0.0 - f) if neg else f
def fsin(x): return fsincos(x, 0)
def fcos(x): return fsincos(x, 1)
def fatan(x):
    neg = 0; n = 0; f = x
    if 0.0 > x: f = -0.0 - x; neg = 1
    if f > 1.0: f = 1.0 / f; n = 2
    if f > 0.2679491924311227:
        n += 1
        t = f * 0.7320508075688773; t = t - 0.5; t = t - 0.5; t = t + f
        f = t / (f + 1.7320508075688772)
    if abs(f) > EPS:
        g = f * f
        p = -8.494624035132068 - g * 0.8375829936815006
        q = g + 15.024001160028575
        p = p * g; p = p - 20.505855195861653
        q = q * g; q = q + 59.57843614259735
        p = p * g; p = p - 13.688768894191927
        q = q * g; q = q + 86.15734959713025
        p = p * g
        q = q * g; q = q + 41.06630668257578
        p = p / q; p = p * f; f = f + p
    if n > 1: f = -0.0 - f
    if n == 1: f += 0.5235987755982989
    elif n == 2: f += HPI
    elif n == 3: f += 1.0471975511965979
    return (-0.0 - f) if neg else f
def fatan2(y, x):
    if x == 0.0:
        if y == 0.0: return 0.0
        return HPI if 0.0 <= y else -HPI
    a = fatan(abs(y / x))
    if 0.0 > x: a = PI - a
    if math.copysign(1.0, y) < 0: a = -0.0 - a
    return a

# ---------------- C helpers
def fgets_lines(data):
    out = []; i = 0; n = len(data)
    while i < n:
        j = data.find(b'\n', i, i + 511)
        if j < 0: j = min(i + 511, n) - 1
        out.append(data[i:j + 1]); i = j + 1
    return out
def trim(b):
    s = b.decode('latin1')
    k = 0
    while k < len(s) and s[k] == ' ': k += 1
    s = s[k:]
    s = s.rstrip('\n\r ') if s else s
    return s
def eq(a, b): return a.upper() == b.upper()
_fre = re.compile(r'[ \t\n\v\f\r]*([+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?)')
def atof(b):
    m = _fre.match(b.decode('latin1'))
    return float(m.group(1)) if m else 0.0
_ire = re.compile(r'[ \t\n\v\f\r]*([+-]?\d+)')
def atol(b):
    m = _ire.match(b.decode('latin1'))
    return int(m.group(1)) if m else 0

NAMES = ['SECTION', 'ENDSEC', 'LINE', 'POLYLINE', 'LWPOLYLINE', 'VERTEX', 'SEQEND', 'ARC', 'CIRCLE', 'SPLINE']
T_LINE = [('210', 4), ('220', 5), ('230', 6), ('10', 0), ('20', 1), ('11', 2), ('21', 3)]
T_LW = [('10', 0), ('20', 1), ('42', 2), ('70', 4)]
T_VTX = [('10', 0), ('20', 1), ('42', 2)]
T_ARC = [('210', 5), ('220', 6), ('230', 7), ('10', 0), ('20', 1), ('40', 2), ('50', 3), ('51', 4)]
T_CIR = [('210', 3), ('220', 4), ('230', 5), ('10', 0), ('20', 1), ('40', 2)]
T_SPL = [('210', 0), ('220', 1), ('230', 2), ('70', 3), ('71', 4), ('72', 5), ('73', 6), ('40', 7),
         ('10', 8), ('20', 9), ('30', 10), ('41', 11)]
def lookup(code, tab, n):
    """returns (found, idx, term)"""
    if eq(code, '0'): return False, None, True
    for nm in NAMES:
        if eq(code, nm): return False, None, True
    for k in range(n):
        if eq(code, tab[k][0]): return True, tab[k][1], False
    return False, None, False

class Stop(Exception):
    pass

class Parser:
    def __init__(self, data, G):
        self.L = fgets_lines(data); self.pos = 0; self.G = G
        self.inEnt = 0; self.lines = []; self.polys = []; self.arcs = []
        self.cur = None; self.inPoly = 0; self.log = []
    def rd(self):
        if self.pos >= len(self.L): return None
        s = self.L[self.pos]; self.pos += 1; return s
    def run(self):
        first = True
        while True:
            c = self.rd()
            if c is None: return 0
            code = trim(c)
            if first:
                if eq(code, 'AutoCAD Binary DXF'): return -10000
                first = False
            v = self.rd()
            if v is None: return 0
            val = trim(v)
            if eq(code, '0'):
                for nm in NAMES:
                    if eq(val, nm):
                        r = getattr(self, 'h_' + nm)()
                        self.log.append((nm, r))
                        if r == 0: return 0
    def h_SECTION(self):
        a = self.rd()
        if a is not None:
            b = self.rd()
            if b is not None:
                if eq(trim(a), '2') and eq(trim(b), 'ENTITIES'): self.inEnt = 1
        return 1
    def h_ENDSEC(self):
        self.inEnt = 0; return 1
    def generic(self, tab, n, on_val, done):
        save = self.pos
        while True:
            c = self.rd()
            if c is None: return 'eof1'
            found, idx, term = lookup(trim(c), tab, n)
            v = self.rd()
            if v is None: return 'eof2'
            if found:
                if on_val(idx, v): term = True
            if term:
                self.pos = save; return 'term'
            save = self.pos
            if done(): return 'done'
    def h_LINE(self):
        if not self.inEnt: return 1
        s = {}; ext = [0.0, 0.0, 1.0]; vals = {}
        def on(idx, vb):
            v = atof(vb)
            if idx == 0:
                if 0 in s: return True
                s[0] = 1; vals[0] = v
            elif idx == 1:
                if 0 in s:
                    if 1 in s: return True
                    s[1] = 1; vals[1] = v
            elif idx == 2:
                if 0 in s and 1 in s:
                    if 2 in s: return True
                    s[2] = 1; vals[2] = v
            elif idx == 3:
                if 0 in s and 1 in s and 2 in s:
                    if 3 in s: return True
                    s[3] = 1; vals[3] = v
            else:
                if idx in s: return True
                s[idx] = 1; ext[idx - 4] = v
            return False
        r = self.generic(T_LINE, 7, on, lambda: len(s) == 7)
        if r == 'eof2': return 0
        if not all(k in s for k in (0, 1, 2, 3)): return 0
        self.lines.append(dict(p1=(vals[0], vals[1]), p2=(vals[2], vals[3]), ext=tuple(ext)))
        return 1
    def newpoly(self):
        p = dict(verts=[], flag=0, ext=(0.0, 0.0, 1.0)); self.polys.append(p); self.cur = p; self.inPoly = 1
        return p
    def h_POLYLINE(self):
        if not self.inEnt: return 1
        p = self.newpoly(); st = {}
        def on(idx, vb):
            v = atof(vb)
            if idx == 4:
                if 'f' in st: return True
                st['f'] = 1 if v == 1.0 else 0
            return False
        r = self.generic(T_LINE, 4, on, lambda: False)
        if r == 'eof2': return 0
        if 'f' in st: p['flag'] = st['f']; p['ext'] = (0.0, 0.0, 1.0)
        return 1
    def h_LWPOLYLINE(self):
        if not self.inEnt: return 1
        tmp = dict(verts=[], flag=0, ext=(0.0, 0.0, 1.0))
        term = False; sx = sy = False; fxp = fyp = True; flags = 0; fx = fy = None; cx = cy = None
        save = self.pos; lastidx = None
        while True:
            c = self.rd()
            if c is None: break
            found, idx, t = lookup(trim(c), T_LW, 4)
            if t: term = True
            if found: lastidx = idx
            v = self.rd()
            if v is None: return 0
            if found:
                val = atof(v)
                if idx == 0:
                    sx = True; cx = val
                    if fxp: fxp = False; fx = val
                elif idx == 1:
                    sy = True; cy = val
                    if fyp: fyp = False; fy = val
                elif idx == 4:
                    flags = atol(v); tmp['flag'] = flags
                if sx and sy:
                    sx = sy = False; bul = 0.0
                    save = self.pos
                    c2 = self.rd()
                    if c2 is None: return 0  # (code line kept from buffer; value fgets fails)
                    f2, i2, t2 = lookup(trim(c2), T_LW, 4)
                    if t2: term = True
                    if f2: lastidx = i2
                    v2 = self.rd()
                    if v2 is None: return 0
                    if f2 or term:
                        val2 = atof(v2)
                        if lastidx == 2: bul = val2
                        self.pos = save
                    tmp['verts'].append((cx, cy, bul))
            if term:
                self.pos = save; break
            save = self.pos
        if flags != 0:
            tmp['verts'].append((fx, fy, 0.0))
        vs = tmp['verts']
        if len(vs) == 2 and not ((vs[1][0] - vs[0][0]) > 1e-8 or (vs[1][1] - vs[0][1]) > 1e-8):
            self.log.append(('LWPOLY-2pt-dropped', vs))
            return 1
        p = self.newpoly(); p['flag'] = tmp['flag']; p['ext'] = tmp['ext']; p['verts'] = list(vs)
        return 1
    def h_VERTEX(self):
        if not self.inEnt or not self.inPoly: return 1
        st = {}
        save = self.pos
        while True:
            c = self.rd()
            if c is None: break
            found, idx, term = lookup(trim(c), T_VTX, 3)
            v = self.rd()
            if v is None: return 0
            if found:
                val = atof(v)
                if idx in st: term = True
                else: st[idx] = val
            if term: self.pos = save; break
            save = self.pos
            if 0 in st and 1 in st and (not self.inPoly or 2 in st): break
        if 0 not in st or 1 not in st: return 0
        self.cur['verts'].append((st[0], st[1], st.get(2, 0.0)))
        return 1
    def h_SEQEND(self):
        if self.inEnt and self.inPoly: self.inPoly = 0
        return 1
    def h_ARC(self):
        if not self.inEnt: return 1
        st = {}
        def on(idx, vb):
            v = atof(vb)
            if idx == 2 and 0 not in st: return False
            if idx in st: return True
            st[idx] = v; return False
        r = self.generic(T_ARC, 8, on, lambda: all(k in st for k in (0, 1, 2, 3, 4)))
        if r == 'eof2': return 0
        if not all(k in st for k in (0, 1, 2, 3, 4)): return 0
        self.arcs.append(dict(c=(st[0], st[1]), r=st[2], a=st[3], b=st[4], ext=(0.0, 0.0, 0.0)))
        return 1
    def h_CIRCLE(self):
        if not self.inEnt: return 1
        st = {}
        def on(idx, vb):
            v = atof(vb)
            if idx in st: return True
            st[idx] = v; return False
        r = self.generic(T_CIR, 6, on, lambda: len(st) == 6)
        if r == 'eof2': return 0
        if not all(k in st for k in (0, 1, 2)): return 0
        ext = (st.get(3, 0.0), st.get(4, 0.0), st.get(5, 1.0))
        for a, b in ((0.0, 120.0), (120.0, 240.0), (240.0, 360.0)):
            self.arcs.append(dict(c=(st[0], st[1]), r=st[2], a=a, b=b, ext=ext))
        return 1
    def h_SPLINE(self):
        if not self.inEnt: return 1
        p = self.newpoly()
        deg = None; knots = None; ctrl = None; ki = 0; ci = 0; s10 = s20 = False
        save = self.pos
        while True:
            c = self.rd()
            if c is None: break
            found, idx, term = lookup(trim(c), T_SPL, 12)
            v = self.rd()
            if v is None: return 0
            if found:
                val = atof(v)
                if idx == 4: deg = int(val)
                elif idx == 5: knots = [0.0] * int(val); ki = 0
                elif idx == 6: ctrl = [[0.0, 0.0, 0.0] for _ in range(int(val))]
                elif idx == 7: knots[ki] = val; ki += 1
                elif idx == 8: s10 = True; ctrl[ci][0] = val
                elif idx == 9: s20 = True; ctrl[ci][1] = val
                elif idx == 10:
                    if s10 and s20: ctrl[ci][2] = val; ci += 1
            if term: self.pos = save; break
            save = self.pos
        pts = spline_eval(ctrl, knots, deg, 30, knots[0], knots[-1])
        for q in pts: self.cur['verts'].append((q[0], q[1], 0.0))
        self.inPoly = 0
        return 1

def spline_eval(ctrl, U0, p, nout, t0, tN):
    nc = len(ctrl); U = [None] + list(U0) + [float('nan')] * 4  # 1-based
    n = p + 1; nk = p + 1 + nc
    out = []
    for j in range(nout):
        t = float(j) * ((tN - t0) / float(nout - 1)) + t0
        for i in range(p + 1, nk + 1):
            if U[i] <= t and t <= U[i + 1]:
                B = {}
                for c in range(1, n + 1):
                    for r in range(1, n + 1): B[(r, c)] = 0.0
                B[(n, 1)] = 1.0
                for k in range(1, p + 1):
                    for m in range(1, k + 2):
                        jj = m - 1
                        d1 = U[i + k - jj] - U[i - jj]
                        A = ((t - U[i - jj]) / d1) * B[(p - jj + 1, k)] if d1 != 0.0 else 0.0
                        d2 = U[i + k + 1 - jj] - U[i + 1 - jj]
                        if d2 != 0.0:
                            rr = p - jj + 2
                            Bt = 0.0 if rr > n else ((U[k + 1 + i - jj] - t) / d2) * B[(rr, k)]
                        else: Bt = 0.0
                        B[(n - jj, k + 1)] = A + Bt
                o = [0.0, 0.0, 0.0]; W = [0.0, 0.0, 0.0]; row = 0
                for cp in range(i - p, i + 1):
                    row += 1; w = 1.0; b = B[(row, n)]
                    for d in range(3):
                        o[d] = w * ctrl[cp - 1][d] * b + o[d]; W[d] = w * b + W[d]
                out.append([o[d] / W[d] for d in range(3)])
                break
    return out

# ---------------- OCS transform
def norm(v):
    l = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
    if l == 0.0: return v
    return (v[0] / l, v[1] / l, v[2] / l)
def cross(a, b):
    return (a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0])
def isdef(e): return e == (0.0, 0.0, 1.0) or e == (0.0, 0.0, 0.0)
def axes(e, G):
    N = norm(e)
    if G <= abs(N[0]) or G <= abs(N[1]): ax = cross((0.0, 0.0, 1.0), N)
    else: ax = cross((0.0, 1.0, 0.0), N)
    ax = norm(ax); ay = norm(cross(N, ax))
    return ax, ay
def tp(p, ax, ay): return (p[0] * ax[0] + p[1] * ax[1], p[0] * ay[0] + p[1] * ay[1])
def tr2(e, p1, p2, G):
    if isdef(e): return p1, p2
    ax, ay = axes(e, G); return tp(p1, ax, ay), tp(p2, ax, ay)

def segments(P):
    G = P.G
    est = len(P.lines) + len(P.arcs)
    for p in P.polys:
        est += len(p['verts']) - 1
        if p['flag'] != 0 and len(p['verts']) > 2: est += 1
    if est < 1: return -500, 0, 0, []
    segs = []; npts = 0
    for L in P.lines:
        a, b = tr2(L['ext'], L['p1'], L['p2'], G)
        segs.append([1.0, a[0], a[1], b[0], b[1], 0.0]); npts += 2
    for p in P.polys:
        V = p['verts']
        for k in range(1, len(V)):
            v0 = V[k - 1]; v1 = V[k]
            if v0[2] == 0.0:
                a, b = tr2(p['ext'], v0[:2], v1[:2], G)
                segs.append([1.0, a[0], a[1], b[0], b[1], 0.0]); npts += 2
            else:
                th = 4.0 * math.atan(v0[2])
                mx = (v1[0] + v0[0]) / 2.0; my = (v1[1] + v0[1]) / 2.0
                a_ = mx - v1[0]; e_ = my - v1[1]
                h = math.sqrt(a_ * a_ + e_ * e_)
                if h != 0.0 and math.tan(th / 2.0) != 0.0:
                    d = h / math.tan(th / 2.0)
                    ox = (e_ * d) / h; oy = (a_ * d) / h
                    cx = mx + ox; cy = my - oy
                    _, c = tr2(p['ext'], v0[:2], (cx, cy), G)
                    if 0.0 <= th: segs.append([2.0, v0[0], v0[1], c[0], c[1], th])
                    else: segs.append([2.0, v1[0], v1[1], c[0], c[1], -th])
                    npts += 3
        if p['flag'] != 0 and len(V) > 2:
            last = V[-1]; first = V[0]
            if struct.pack('<3d', *last) == struct.pack('<3d', *first): est -= 1
            else:
                a, b = tr2(p['ext'], last[:2], first[:2], G)
                if (b[0] - a[0]) > 1e-8 or (b[1] - a[1]) > 1e-8:
                    segs.append([1.0, a[0], a[1], b[0], b[1], 0.0]); npts += 2
                else: est -= 1
    for A in P.arcs:
        if A['r'] <= 0.0: est -= 1; continue
        cx, cy = A['c']; a50 = A['a']; a51 = A['b']
        if not isdef(A['ext']):
            ax, ay = axes(A['ext'], G)
            cx, cy = tp((cx, cy), ax, ay)
            def ta(x):
                r = x * DEG2RAD; cc = math.cos(r); ss = math.sin(r)
                c2, s2 = tp((cc, ss), ax, ay)
                return math.atan2(s2, c2) * RAD2DEG
            a50 = ta(a50); a51 = ta(a51)
            if a51 < a50: a50, a51 = a51, a50
        s = a50 * DEG2RAD
        sw = a51 - a50
        if sw < 0.0: sw = sw + 360.0
        sw = sw * DEG2RAD
        r = A['r']
        segs.append([2.0, cx + r * math.cos(s), cy + r * math.sin(s), cx, cy, sw]); npts += 3
    while len(segs) < est: segs.append([0.0] * 6)
    return 0, est, npts, segs[:est]

def arcend_f(s):
    sx, sy, cx, cy, a = (f32(s[1]), f32(s[2]), f32(s[3]), f32(s[4]), f32(s[5]))
    dx = sx - cx; dy = sy - cy
    sn = fsin(a); cs = fcos(a)
    oy = dy * cs + dx * sn + cy
    ox = (cs * dx + cx) - sn * dy
    return f32(ox), f32(oy)

def size_of(segs):
    n = len(segs)
    if n < 1: return 0.0
    st = {'first': True, 'm': 0.0}
    def upd(d):
        if (not st['first']) or d <= 1e-6:
            if d < st['m'] and 1e-6 < d: st['m'] = d
        else: st['first'] = False; st['m'] = d
    last = segs[n - 1]
    prev = (last[3], last[4]) if int(last[0]) == 1 else arcend_f(last)
    for s in segs:
        if int(s[0]) == 1:
            a = prev[0] - s[1]; b = prev[1] - s[2]; upd(a * a + b * b)
            c = s[1] - s[3]; d = s[2] - s[4]; upd(c * c + d * d)
            prev = (last[3], last[4])
        else:
            E = arcend_f(last)
            a = prev[0] - s[1]; b = prev[1] - s[2]; upd(a * a + b * b)
            c = s[1] - E[0]; d = s[2] - E[1]; upd(c * c + d * d)
            prev = E
    return math.sqrt(st['m']) if st['m'] > 0.0 else 0.0

def near(a, b, t): return (b - a) <= t and (a - b) <= t
def dedupe(segs, tol):
    keep = [True] * len(segs)
    for i in range(len(segs) - 1):
        if not keep[i]: continue
        a = segs[i]
        for j in range(i + 1, len(segs)):
            if not keep[j]: continue
            b = segs[j]
            if not (near(a[0], 1.0, 1e-4) and near(b[0], 1.0, 1e-4)):
                if near(a[0], 2.0, 1e-4) and near(b[0], 2.0, 1e-4) and all(near(a[k], b[k], tol) for k in range(1, 6)):
                    keep[j] = False
            else:
                if near(a[1], b[3], tol) and near(a[2], b[4], tol) and near(a[3], b[1], tol) and near(a[4], b[2], tol):
                    keep[j] = False
                elif all(near(a[k], b[k], tol) for k in range(1, 5)):
                    keep[j] = False
    return [s for s, k in zip(segs, keep) if k]

def fortran_chain(segs, tol):
    """full Fortran stage incl. writeback; returns (status, outsegs)"""
    if tol <= 0.0: tol = f32(1e-9)
    nseg = len(segs)
    P = [None]; E = [None]; cnt = 0
    for k, s in enumerate(segs, 1):
        if int(s[0]) == 2:
            x1, y1, cx, cy, a = s[1], s[2], s[3], s[4], s[5]
            dx = x1 - cx; dy = y1 - cy
            sn = fsin(a); cs = fcos(a)
            d2 = sn * dy; d1 = cs * dx
            ny = sn * dx + cs * dy; nx = d1 - d2
            P += [[x1, y1, 0.0], [cx, cy, 0.0], [cx + nx, cy + ny, 0.0]]
            E.append([1, k, cnt + 1, cnt + 3]); cnt += 3
        else:
            P += [[s[1], s[2], 0.0], [s[3], s[4], 0.0]]
            E.append([2, k, cnt + 1, cnt + 2]); cnt += 2
    npts = cnt
    D = [0.0] + [tol] * npts; M = [[0, 0] for _ in range(npts + 1)]
    def dist(p, q):
        dx = P[q][0] - P[p][0]; dy = P[q][1] - P[p][1]; dz = P[q][2] - P[p][2]
        return math.sqrt(dz * dz + dy * dy + dx * dx)
    c910 = 0
    for e in range(1, nseg + 1):
        f, l = E[e][2], E[e][3]; step = l - f
        pp = f
        for it in range(2):
            p = pp; dmin = D[p]; M[p][1] = e
            if dmin != 0.0:
                for e2 in range(1, nseg + 1):
                    if e2 == e: continue
                    f2, l2 = E[e2][2], E[e2][3]; step2 = l2 - f2
                    q = f2; done = False
                    for i in range(2):
                        if p == q:
                            D[p] = 0.0; M[p] = [q, e]; M[q] = [p, e2]; D[q] = 0.0; done = True; break
                        if D[q] != 0.0:
                            d = dist(p, q)
                            if d < dmin:
                                c910 = e2; dmin = d; M[p] = [q, e]
                                if d == 0.0: done = True; break
                        q += step2
                    if done: break
                    if dmin < D[p]:
                        q2 = M[p][0]; D[p] = dmin; D[q2] = dmin; M[q2] = [p, c910]
            pp += step
    for p in range(1, npts + 1):
        q = M[p][0]
        if q > 0:
            M[q][0] = -M[q][0]
            x = (P[p][0] + P[q][0]) * 0.5; y = (P[p][1] + P[q][1]) * 0.5
            P[p][0] = x; P[q][0] = x; P[p][1] = y; P[q][1] = y; D[p] = 0.0; D[q] = 0.0
    for p in range(1, npts + 1): M[p][0] = abs(M[p][0])
    def mark(i):
        was0 = D[i] == 0.0
        D[i] = -abs(D[i])
        if was0: D[i] = NEG1E10
        return D[i]
    Lst = []; loops = []; status = 0
    for p in range(1, npts + 1):
        dv = D[p]
        if not (0.0 <= dv): continue
        if len(loops) >= 1:
            status = 3
            Lst = [(E[e][2], e) for e in range(1, nseg + 1)]
            loops = [[x] for x in Lst]
            break
        A = M[p][1]; fw = [(p, A)]; bw = []
        d804 = E[A][2]
        if p == d804: d804 = E[A][3]
        cur = p; v = dv
        try:
            while True:
                while True:
                    while True:
                        while True:
                            d808 = v
                            if v < 0.0 and d804 == 0: raise Stop
                            if 0.0 <= v: break
                            cur = d804; d804 = 0; v = D[cur]
                        v = mark(cur)
                        if not (tol < d808): break
                    q = M[cur][0]
                    if d808 == D[q]: break
                mark(q)
                ent = M[q][1]
                if A == ent: break
                if d804 == 0: bw.append((q, ent))
                else: fw.append((q, ent))
                nxt = E[ent][3]
                if nxt == q: nxt = E[ent][2]
                cur = nxt; v = D[cur]
        except Stop:
            pass
        seq = list(reversed(bw)) + fw
        for (_, e) in seq:
            for pt in range(E[e][2], E[e][3] + 1): mark(pt)
        seq = [list(x) for x in seq]
        if len(seq) > 1:
            best = None
            for k in range(len(seq) - 1):
                e1 = seq[k][1]; e2 = seq[k + 1][1]
                a1, b1 = E[e1][2], E[e1][3]; a2, b2 = E[e2][2], E[e2][3]
                best = 1; dm = dist(a1, a2)
                d = dist(b1, a2)
                if d < dm: dm = d; best = 2
                d3 = dist(a1, b2)
                if d3 < dm: dm = d3; best = 3
                d4 = dist(b1, b2)
                if d4 < dm: dm = d3; best = 4
                seq[k][0] = b1 if best in (1, 3) else a1
            el = seq[-1][1]
            seq[-1][0] = E[el][2] if best in (1, 2) else E[el][3]
        loops.append([tuple(x) for x in seq])
    out = []
    for lp in loops:
        for (entry, e) in lp:
            t, k, f, l = E[e]
            rev = (f != entry)
            if t == 1:
                a = P[f]; b = P[f + 2]; c = P[f + 1]
                ax = a[0] - c[0]; ay = a[1] - c[1]; la = math.sqrt(ay * ay + ax * ax)
                bx = b[0] - c[0]; by = b[1] - c[1]; lb = math.sqrt(by * by + bx * bx)
                ang = fatan2(((ax * by - bx * ay) / la) / lb, ((ay * by + ax * bx) / la) / lb)
                if ang < 0.0: ang = ang + TWO_PI_F
                if rev: out.append([2.0, b[0], b[1], c[0], c[1], -0.0 - ang if False else -ang])
                else: out.append([2.0, a[0], a[1], c[0], c[1], ang])
            else:
                if rev: out.append([1.0, P[f + 1][0], P[f + 1][1], P[f][0], P[f][1], 0.0])
                else: out.append([1.0, P[f][0], P[f][1], P[f + 1][0], P[f + 1][1], 0.0])
    return status, len(loops), out

def run(path, G=1e-9, verbose=False):
    data = open(path, 'rb').read()
    P = Parser(data, G)
    r = P.run()
    res = dict(parse=r, log=P.log, nlines=len(P.lines), npolys=len(P.polys), narcs=len(P.arcs))
    if r != 0:
        res['result'] = r; return res
    rc, nseg, npts, segs = segments(P)
    if rc != 0:
        res['result'] = rc; return res
    size = size_of(segs); tol = size * 0.01
    res.update(size=size, tol=tol, nseg_in=nseg, npts=npts, segs_in=segs)
    if nseg * 6 < 1:
        res['result'] = -1; return res
    d = dedupe(segs, tol)
    st, nl, out = fortran_chain(d, tol)
    res.update(nseg_dedup=len(d), status=st, nloops=nl, out=out, result=0)
    return res

def fmtE(x):
    s = '%.10E' % x
    m, e = s.split('E')
    return '%sE%s%03d' % (m, e[0], abs(int(e)))

def key_lines(res, obj=1):
    out = res['out']
    lines = ['DIEGEO %7d%8d%8d' % (obj, 2, len(out))]
    for i, s in enumerate(out, 1):
        lines.append('%8d%8d' % (i, int(s[0])) + ''.join('%21s' % fmtE(v) for v in (s[1:5] + [s[5] * RAD2DEG if int(s[0]) == 2 else s[5]])))
    return lines

if __name__ == '__main__':
    for pth in sys.argv[1:]:
        r = run(pth)
        print('==', pth, {k: r[k] for k in r if k not in ('segs_in', 'out', 'log')})
        if 'out' in r:
            for l in key_lines(r): print(l)
