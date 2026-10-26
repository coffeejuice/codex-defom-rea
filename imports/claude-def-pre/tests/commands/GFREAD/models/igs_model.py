# Prototype of the spec in IGES_READER.md: predicts the DIEGEO block KFWRIT writes after
# GFREAD of a 2D .IGS file.  Scratch tool, not a deliverable.
import sys, math, re, struct

F01 = struct.unpack('<d', bytes.fromhex('00000040e17a843f'))[0]   # 0.01f as double
F1E9 = struct.unpack('<d', bytes.fromhex('000000e00b2e113e'))[0]  # 1e-9f
F1E5 = struct.unpack('<d', bytes.fromhex('00000080b5f8e43e'))[0]  # 1e-5f
NEG1E10 = struct.unpack('<d', bytes.fromhex('000000e0df7cdbbd'))[0]
TWOPI_L = struct.unpack('<d', bytes.fromhex('282d4454fb211940'))[0]  # 6.2831853071796
PI = 3.141592653589793
HALFPI = 1.5707963267948966
RAD2DEG = 57.29577951308232
C = dict(s1=0.2679491924311227, s2=0.7320508075688773, s3=1.7320508075688772,
         eps=1.05367121277235e-08, p598=-8.494624035132068, p590=0.8375829936815006,
         p580=20.505855195861653, p570=13.688768894191927, q588=15.024001160028575,
         q578=59.57843614259735, q568=86.15734959713025, q560=41.06630668257578,
         a1=0.5235987755982989, a3=1.0471975511965979)


def nint(x):
    if abs(x) >= 4503599627370496.0:
        return int(x)
    return int(x + (0.5 if x >= 0.0 else -0.5))


def atan_cw(x):
    neg = x < 0.0
    if neg:
        x = -0.0 - x
    n = 0
    if 1.0 < x:
        x = 1.0 / x
        n = 2
    if C['s1'] < x:
        n += 1
        x = (((x * C['s2'] - 0.5) - 0.5) + x) / (x + C['s3'])
    if abs(x) > C['eps']:
        g = x * x
        num = ((((C['p598'] - g * C['p590']) * g - C['p580']) * g - C['p570']) * g)
        den = ((((g + C['q588']) * g + C['q578']) * g + C['q568']) * g + C['q560'])
        x = x + (num / den) * x
    if n > 1:
        x = -0.0 - x
    if n == 1:
        x = x + C['a1']
    elif n == 2:
        x = x + HALFPI
    elif n == 3:
        x = x + C['a3']
    if neg:
        x = -0.0 - x
    return x


def atan2_f(y, x):
    if x == 0.0:
        if y == 0.0:
            return 0.0
        return -HALFPI if y < 0.0 else HALFPI
    t = atan_cw(abs(y / x))
    if x < 0.0:
        t = PI - t
    if math.copysign(1.0, y) < 0:
        t = -0.0 - t
    return t


def ang3(a, b, c):  # FUN_14046736c(a=start, b=end, c=center)
    ax = a[0] - c[0]; ay = a[1] - c[1]
    r1 = math.sqrt(ay * ay + ax * ax)
    bx = b[0] - c[0]; by = b[1] - c[1]
    r2 = math.sqrt(by * by + bx * bx)
    s = ax * by - bx * ay
    co = ay * by + ax * bx
    return atan2_f((s / r1) / r2, (co / r1) / r2)


def dist(p, q):  # FUN_1404681dc: diff = q - p
    dx = q[0] - p[0]; dy = q[1] - p[1]; dz = q[2] - p[2]
    return math.sqrt((dx * dx + dy * dy) + dz * dz)


class ParseErr(Exception):
    pass


def fort_real(field):
    """FUN_140468960: value of one P field; returns (value, err).  None value = keep stale."""
    s = field[:80]
    if len(s) < 1:
        return None, 0
    if len(s) < 2:
        if s in ('E', 'e'):
            return 0.0, 0
        if not ('0' <= s <= '9'):
            return None, 1
    t = s.replace(' ', '')
    if t == '':
        return 0.0, 0
    m = re.fullmatch(r'([+-]?)(\d*)(?:\.(\d*))?(?:(?:[EeDdQq]([+-]?\d+))|([+-]\d+))?', t)
    if not m or (m.group(2) == '' and (m.group(3) is None or m.group(3) == '')):
        return None, 1
    mant = m.group(1) + (m.group(2) or '0') + '.' + (m.group(3) or '')
    e = m.group(4) or m.group(5) or '0'
    return float(mant + 'e' + e), 0


class Sim:
    def __init__(self, data):
        self.recs = self.split(data)
        self.pos = 0
        self.stale = 0.0

    @staticmethod
    def split(data):
        out = []
        for ln in data.split(b'\n'):
            if ln.endswith(b'\r'):
                ln = ln[:-1]
            out.append(ln.decode('latin-1'))
        if out and out[-1] == '':
            out.pop()
        return out

    def read(self):
        if self.pos >= len(self.recs):
            return None
        r = self.recs[self.pos].ljust(80)[:80]
        self.pos += 1
        return r

    def parse_line(self, rec72):
        """FUN_140464245: values of one 72-col record."""
        vals = []
        st = 1
        for pos in range(1, 73):
            ch = rec72[pos - 1]
            if ch in ',;':
                prev = rec72[71] if pos == 1 else rec72[pos - 2]
                if prev == ',':
                    vals.append(0.0)
                else:
                    v, err = fort_real(rec72[st - 1:pos - 1])
                    if err:
                        return vals
                    if v is None:
                        v = self.stale
                    self.stale = v
                    vals.append(v)
                st = pos + 1
        return vals

    def inpsec(self, want):
        """FUN_140463e15"""
        got = []
        while True:
            r = self.read()
            if r is None:
                raise ParseErr('INPSEC : STOPPING (process stops)')
            self.curline += 1
            got += self.parse_line(r[:72])
            if len(got) >= want:
                return got

    def pass1(self):
        # main loop of FUN_1404579fc (first open only; no-newline retry not modelled)
        recs = []
        while True:
            r = self.read()
            if r is None:
                return 'EOF-before-P', None
            sec = r[72]
            if sec in 'Dd':
                cnt = 1
                cur = r
                while True:
                    f = [int(cur[i*8:i*8+8].strip() or 0) for i in range(8)]
                    l2 = self.read()
                    cnt += 1
                    etype = int(l2[0:8].strip() or 0)
                    ptr, trf = f[1], f[6]
                    if etype == 100: recs.append([1, ptr, trf, 0, 0])
                    elif etype == 110: recs.append([2, ptr, trf, 0, 0])
                    elif etype == 124: recs.append([4, ptr, cnt - 1, 0, 0])
                    elif etype == 112: recs.append([5, ptr, trf, 0, 0])
                    elif etype == 126: recs.append([6, ptr, trf, 0, 0])
                    nxt = self.read()
                    if nxt is None:
                        return 'EOF-in-D', None
                    cnt += 1
                    if nxt[72] not in 'Dd':
                        r = nxt
                        break
                    cur = nxt
                if r[72] == 'P':
                    break
            elif sec == 'P':
                break
        first = r
        # bubble sort by ptr (stable)
        recs.sort(key=lambda x: x[1])
        self.records = recs
        return 'P', first

    def parse_P(self, first):
        recs = self.records
        slots = []  # list of 3-double slots
        nA = 0
        def setslot(i, v):  # 1-based
            while len(slots) < i:
                slots.append([0.0, 0.0, 0.0])
            slots[i - 1] = list(v)
        flag = 0
        self.curline = 1
        buf0 = []
        if recs and recs[0][1] == 1:
            buf0 = self.parse_line(first[:72])
        elif recs and recs[0][1] > 1:
            flag = 1
        for rec in recs:
            ptr, t = rec[1], rec[0]
            while self.curline + 1 <= ptr - 1:
                self.read(); self.curline += 1
            def get(n):
                nonlocal flag
                if flag == 0:
                    b = list(buf0)
                    if n - len(b) > 0:
                        b += self.inpsec(n - len(b))
                    flag = 1
                else:
                    b = self.inpsec(n)
                return b
            rec[3] = nA + 1
            if t == 2:
                b = get(7)
                setslot(nA + 1, b[1:4]); setslot(nA + 2, b[4:7]); nA += 2
            elif t == 1:
                b = get(8)
                zt = b[1]
                setslot(nA + 1, (b[4], b[5], zt)); setslot(nA + 2, (b[2], b[3], zt)); setslot(nA + 3, (b[6], b[7], zt)); nA += 3
            elif t == 4:
                b = get(13)
                # slot i comp k = M[k][i]
                for i in range(4):
                    setslot(nA + 1 + i, (b[1 + i], b[5 + i], b[9 + i]))
                nA += 4
            elif t == 5:
                if flag == 0:
                    b = list(buf0); flag = 1
                else:
                    b = self.inpsec(0)
                N = nint(b[4])
                need = 13 * N - len(b) + 6
                b += self.inpsec(need)
                q = (N + 1) // 3
                blk = [0.0] * (3 * (2 + 4 * N + q))
                blk[0] = float(N); blk[1] = float(4 * N + q + 2)
                for c in range(N + 1):
                    blk[3 + c] = b[5 + c]
                d0 = q * 3 + 6; s0 = N + 6
                for s in range(N):
                    src = b[s0 + 12 * s: s0 + 12 * s + 12]
                    for i in range(4):
                        for k in range(3):
                            blk[d0 + 12 * s + 3 * i + k] = src[i + 4 * k]
                for i in range(len(blk) // 3):
                    setslot(nA + 1 + i, blk[3 * i:3 * i + 3])
                nA += 2 + 4 * N + q
            elif t == 6:
                if flag == 0:
                    b = list(buf0); flag = 1
                else:
                    b = self.inpsec(0)
                K = nint(b[1]); M = nint(b[2])
                need = 5 * K + M + 15 - len(b)
                b += self.inpsec(need)
                n8 = 5 * K + M + 9
                blk = [0.0] * (3 * (1 + (5 * K + M + 15) // 3))
                blk[0] = float(K); blk[1] = float(M)
                blk[3] = float(K + M + 9); blk[4] = float(2 * K + M + 10); blk[5] = float(5 * K + M + 13)
                for c in range(n8):
                    blk[6 + c] = b[7 + c] if 7 + c < len(b) else 0.0
                for i in range(len(blk) // 3):
                    setslot(nA + 1 + i, blk[3 * i:3 * i + 3])
                nA += 1 + (5 * K + M + 15) // 3
        self.slots = slots
        self.nA = nA

    # ---- FUN_14046519b
    def build(self):
        S = self.slots
        P = lambda i: S[i - 1]
        out = []   # points
        orec = []
        cur = 1
        for i, rec in enumerate(self.records, 1):
            t = rec[0]
            if t in (4, 3):
                continue
            trf, f = rec[2], rec[3]
            k = len(orec) + 1
            r = [t, k, cur, 0, i]
            last = cur + 1 if t != 1 else cur + 2
            if t == 5:
                pts = self.eval112(f)
            elif t == 6:
                pts = self.eval126(f)
            else:
                pts = [list(P(f + j)) for j in range(last - cur + 1)]
            last = cur + len(pts) - 1
            r[3] = last
            # transform
            if trf != 0:
                for j, q in enumerate(self.records, 1):
                    if j != i and q[0] == 4 and q[2] == trf:
                        m = q[3]
                        if last >= cur:
                            Rm = [P(m), P(m + 1), P(m + 2)]; T = P(m + 3)
                            for p in pts:
                                o = []
                                for rr in range(3):
                                    s = 0.0
                                    for c in range(3):
                                        s = s + Rm[c][rr] * p[c]
                                    o.append(s + T[rr])
                                p[:] = o
                        break
            if t == 1:
                a1 = ang3(P(f), P(f + 2), P(f + 1))
                if a1 < 0.0: a1 += TWOPI_L
                a2 = ang3(pts[0], pts[2], pts[1])
                if a2 < 0.0: a2 += TWOPI_L
                if abs(a1 - a2) > F1E5:
                    pts[0], pts[2] = pts[2], pts[0]
            out += pts
            orec.append(r)
            cur += len(pts)
        self.P = out
        self.R = orec

    def eval112(self, f):
        S = self.slots
        blk = []
        N = nint(S[f - 1][0])
        q = (N + 1) // 3
        n = 2 + 4 * N + q
        for i in range(n):
            blk += S[f - 1 + i]
        T = lambda s: blk[2 + s]
        coef = lambda row, k: blk[(row - 1) * 3 + k - 1]
        pts = []
        Mdiv = 8
        A = q + 3
        for s in range(1, N + 1):
            dt = (T(s + 1) - T(s)) / float(Mdiv)
            for j in range(Mdiv):
                u = float(j) * dt
                u2 = u * u; u3 = u * u2
                pts.append([((u * coef(A + 1, k) + coef(A, k)) + u2 * coef(A + 2, k)) + u3 * coef(A + 3, k) for k in (1, 2, 3)])
            A += 4
        u = T(N + 1) - T(N)
        u2 = u * u; u3 = u * u2
        A -= 4
        pts.append([((coef(A + 1, k) * u + coef(A, k)) + u2 * coef(A + 2, k)) + u3 * coef(A + 3, k) for k in (1, 2, 3)])
        return pts

    def eval126(self, f):
        S = self.slots
        K = int(S[f - 1][0]); M = int(S[f - 1][1])
        n = 1 + (5 * K + M + 15) // 3
        blk = []
        for i in range(n):
            blk += S[f - 1 + i]
        kn = lambda j: blk[6 + j - 1]
        w = lambda i: blk[int(blk[3]) - 1 + i - 1]
        cp = lambda i, k: blk[int(blk[4]) - 1 + 3 * (i - 1) + k - 1]
        V0 = blk[int(blk[5]) - 1]; V1 = blk[int(blk[5])]
        L = M + 1
        pts = []
        for ii in range(30):
            u = float(ii) * ((V1 - V0) / float(29)) + V0
            for j in range(M + 1, K + M + 2):
                if kn(j) <= u and u <= kn(j + 1):
                    Nt = [[0.0] * (L + 1) for _ in range(L + 1)]  # Nt[r][c] 1-based
                    Nt[L][1] = 1.0
                    for p in range(1, M + 1):
                        for i in range(1, p + 2):
                            s = i - 1
                            d1 = kn(j + p - s) - kn(j - s)
                            A = ((u - kn(j - s)) / d1) * Nt[M - s + 1][p] if d1 != 0.0 else 0.0
                            d2 = kn(j + p + 1 - s) - kn(j + 1 - s)
                            if d2 != 0.0:
                                r2 = M - s + 2
                                B = 0.0 if r2 > L else ((kn(p + 1 + j - s) - u) / d2) * Nt[r2][p]
                            else:
                                B = 0.0
                            Nt[L - s][p + 1] = A + B
                    o = [0.0, 0.0, 0.0]; dn = [0.0, 0.0, 0.0]
                    cnt = 0
                    for i in range(j - M, j + 1):
                        cnt += 1
                        wi = w(i); b = Nt[cnt][M + 1]
                        for k in range(3):
                            o[k] = wi * cp(i, k + 1) * b + o[k]
                            dn[k] = wi * b + dn[k]
                    pts.append([o[k] / dn[k] for k in range(3)])
                    break
        return pts

    def tol(self):
        mn = 1000000.0
        for r in self.R:
            for j in range(r[2], r[3]):
                d = dist(self.P[j - 1], self.P[j])
                if d != 0.0 and d < mn:
                    mn = d
        t = mn * F01
        if t == 0.0: t = F1E9
        self.mindist = mn
        return t

    # ---- FUN_140464abc
    def match(self, tol):
        P, R = self.P, self.R
        nP, nR = len(P), len(R)
        L1 = [0] * (nP + 1); L2 = [0] * (nP + 1); D = [tol] * (nP + 1)
        D[0] = P[-1][2] if P else 0.0
        for ri, r in enumerate(R, 1):
            step = r[3] - r[2]
            for e1 in range(2):
                p = r[2] + e1 * step
                d = D[p]
                L2[p] = ri
                if d == 0.0:
                    continue
                done = False
                best = None
                for qi, q in enumerate(R, 1):
                    if qi == ri: continue
                    st2 = q[3] - q[2]
                    for e2 in range(2):
                        e = q[2] + e2 * st2
                        if p == e:
                            D[p] = 0.0; L1[p] = e; L2[p] = ri; L1[e] = p; L2[e] = qi; D[e] = 0.0
                            done = True; break
                        if D[e] == 0.0:
                            continue
                        dd = dist(P[p - 1], P[e - 1])
                        if dd < d:
                            best = qi; d = dd; L1[p] = e; L2[p] = ri
                            if dd == 0.0:
                                done = True; break
                    if done: break
                    if d < D[p]:
                        e = L1[p]
                        D[p] = d; D[e] = d; L1[e] = p; L2[e] = best
                if done:
                    continue
        for p in range(1, nP + 1):
            e = L1[p]
            if e > 0:
                L1[e] = -L1[e]
                mx = (P[p - 1][0] + P[e - 1][0]) * 0.5
                my = (P[p - 1][1] + P[e - 1][1]) * 0.5
                P[p - 1][0] = mx; P[e - 1][0] = mx
                P[p - 1][1] = my; P[e - 1][1] = my
                D[p] = 0.0; D[e] = 0.0
        for p in range(1, nP + 1):
            L1[p] = abs(L1[p])
        self.L1, self.L2, self.D = L1, L2, D

    # ---- FUN_140467473 (flag = 1)
    def trace(self, tol):
        P, R, L1, L2, D = self.P, self.R, self.L1, self.L2, self.D
        nP, nR = len(P), len(R)
        nl = 0
        S = None
        def neg(x):
            return NEG1E10 if x == 0.0 else -abs(x)
        for p0 in range(1, nP + 1):
            if not (0.0 <= D[p0]):
                continue
            nl += 1
            if nl > 1:
                return 3, [[R[r - 1][2], r] for r in range(1, nR + 1)]
            rec0 = L2[p0]
            A = [[p0, rec0]]; B = []
            cur = R[rec0 - 1][2]
            if p0 == cur: cur = R[rec0 - 1][3]
            p = p0; d = D[p]
            while True:
                if d < 0.0 and cur == 0:
                    break
                if d < 0.0:
                    p = cur; cur = 0; d = D[p]
                    continue
                dold = d
                D[p] = neg(d); d = D[p]
                if tol < dold:
                    continue
                e = L1[p]
                de = D[e]
                if dold != de:
                    continue
                D[e] = neg(de)
                r2 = L2[e]
                if r2 == rec0:
                    break
                (B if cur == 0 else A).append([e, r2])
                f2, l2 = R[r2 - 1][2], R[r2 - 1][3]
                nxt = f2 if l2 == e else l2
                p = nxt; d = D[p]
            Mg = list(reversed(B)) + A
            S = [list(x) for x in Mg]
            for pt, r in S:
                for j in range(R[r - 1][2], R[r - 1][3] + 1):
                    D[j] = neg(D[j])
            if len(S) > 1:
                self.orient(S)
        return 0, S

    def orient(self, S):
        P, R = self.P, self.R
        case = 1
        for i in range(len(S) - 1):
            ri, rj = S[i][1], S[i + 1][1]
            fi, li = R[ri - 1][2], R[ri - 1][3]
            fj, lj = R[rj - 1][2], R[rj - 1][3]
            d = dist(P[fi - 1], P[fj - 1]); case = 1
            d2 = dist(P[li - 1], P[fj - 1])
            if d2 < d: d = d2; case = 2
            d3 = dist(P[fi - 1], P[lj - 1])
            if d3 < d: d = d3; case = 3
            d4 = dist(P[li - 1], P[lj - 1])
            if d4 < d: d = d3; case = 4
            S[i][0] = li if case in (1, 3) else fi
        rl = S[-1][1]
        S[-1][0] = R[rl - 1][2] if case in (1, 2) else R[rl - 1][3]

    def output(self, S):
        P, R = self.P, self.R
        segs = []
        for pt, r in S:
            t, f, l = R[r - 1][0], R[r - 1][2], R[r - 1][3]
            rev = f != pt
            if t == 1:
                a = ang3(P[f - 1], P[f + 1], P[f])
                if a < 0.0: a += TWOPI_L
                st = P[f - 1]
                if rev:
                    a = -a; st = P[f + 1]
                segs.append([2.0, st[0], st[1], P[f][0], P[f][1], a])
            else:
                n = l - f + 1
                idx = list(range(f, l + 1))
                if rev: idx.reverse()
                for a_, b_ in zip(idx, idx[1:]):
                    segs.append([1.0, P[a_ - 1][0], P[a_ - 1][1], P[b_ - 1][0], P[b_ - 1][1], 0.0])
        return segs


def E(v):
    s = '%20.10E' % v
    m = re.match(r'(.*E[+-])(\d+)$', s)
    return ('%s%03d' % (m.group(1).strip(), int(m.group(2)))).rjust(20)


def run(path, objn=1):
    data = open(path, 'rb').read()
    s = Sim(data)
    st, first = s.pass1()
    if st != 'P':
        print('pass1:', st); return
    s.parse_P(first)
    s.build()
    if len(s.P) < 1:
        print('return -8 (no points)'); return
    tol = s.tol()
    s.match(tol)
    code, S = s.trace(tol)
    segs = s.output(S)
    print('records', len(s.R), 'points', len(s.P), 'mindist', repr(s.mindist), 'tol', repr(tol), 'status', code)
    print('DIEGEO %7i %7i %7i' % (objn, 2, len(segs)))
    for k, g in enumerate(segs, 1):
        print(' %7i %7i %s %s %s %s %s' % (k, int(g[0]), E(g[1]), E(g[2]), E(g[3]), E(g[4]), E(g[5] * RAD2DEG)))


if __name__ == '__main__':
    run(sys.argv[1])
