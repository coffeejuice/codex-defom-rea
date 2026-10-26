# Simulates the DEF_PRE group 1/3 KEY read grammar from the specs and checks that each
# keyword's consumption ends on a keyword, a comment or EOF. Prints summaries only.
import re, sys, os, collections

REG = {}
for ln in open(sys.argv[1]).read().splitlines()[1:]:
    f = ln.split('\t')
    REG[f[1]] = int(f[2])
NUM = re.compile(r'\s*([-+]?(\d+\.?\d*|\.\d+)([eEdD][-+]?\d+)?)')
INT = re.compile(r'\s*([-+]?\d+)')


class S:
    def __init__(s, t, dim):
        s.t, s.p, s.dim, s.counts = t, 0, dim, {}

    def fgets(s, n=1024):
        if s.p >= len(s.t):
            return None
        e = s.t.find('\n', s.p)
        e = len(s.t) if e < 0 else e + 1
        if e - s.p > n - 1:
            e = s.p + n - 1
        r = s.t[s.p:e]
        s.p = e
        return r

    def fnum(s):
        m = NUM.match(s.t, s.p)
        if not m:
            raise ValueError('fscanf %Flf failed')
        s.p = m.end()
        return float(m.group(1).replace('D', 'E').replace('d', 'e'))

    def fint(s):
        m = INT.match(s.t, s.p)
        if not m:
            raise ValueError('fscanf %i failed')
        s.p = m.end()
        return int(m.group(1))


def nums(line, k):
    out = []
    for tok in (line or '').replace(',', ' ').split():
        if len(out) >= k:
            break
        try:
            out.append(float(tok.replace('D', 'E')))
        except ValueError:
            break
    return out + [0.0] * (k - len(out))


def ints(line, k):
    out = []
    for tok in (line or '').replace(',', ' ').split():
        if len(out) >= k:
            break
        m = re.match(r'[-+]?\d+', tok)
        if not m:
            break
        out.append(int(m.group(0)))
        if m.end() != len(tok):
            break
    return out + [0] * (k - len(out))


def table(s, ndim, counts, ncomp):
    ncomp = max(ncomp, 1)
    counts = [max(c, 0) for c in counts]
    if ndim == 0 and ncomp < 2:
        return
    tot = ncomp
    for c in counts[:ndim]:
        tot *= c
    if ndim == 0:
        for _ in range(ncomp):
            s.fnum()
    elif ndim == 1:
        for _ in range(counts[0]):
            s.fnum()
            for _ in range(ncomp):
                s.fnum()
    else:
        for c in counts[:ndim]:
            for _ in range(c):
                s.fnum()
        for _ in range(tot):
            s.fnum()
    if tot > 0:
        s.fgets()


def fric(s, t, d1, d2, d3, extra_line=False):
    if t <= 0:
        return
    if 1 <= t <= 4:
        table(s, 1, [int(d1)], 1)
    elif t == 5:
        if extra_line:
            a, b, c = ints(s.fgets(), 3)
            table(s, 3, [a, b, c], 1)
        else:
            table(s, 3, [int(d1), int(d2), int(d3)], 1)
    elif t in (6, 7, 8) and extra_line:
        s.fgets()


def g1(s, kw):
    rest = s.fgets()
    if rest is None:
        return
    if kw in ('TITLE', 'OPRNAM', 'SIMNAM', 'FRNNAM', 'ELMNOD'):
        s.fgets()
    elif kw == 'DTTYPE':
        t, n = ints(rest, 2)
        if t != 0:
            table(s, 1, [n], 1)
    elif kw in ('ENVTMP', 'CNVCOF', 'ENVATM', 'ACVCOF'):
        t = ints(rest, 1)[0]
        v = nums(rest, 2)[1]
        if kw == 'ENVTMP' and t == 10:
            for i in range(1, int(v) + 1):
                h = s.fgets()
                a = ints(h, 3)
                d = nums(h, 5)
                typ, third, d1, d2 = a[1], a[2], d[3], d[4]
                if i == 21:
                    if typ == 1:
                        for _ in range(int(d1)):
                            s.fgets()
                elif typ == 0:
                    table(s, 0, [], third)
                elif typ == 1:
                    cnt = int(d1)
                    table(s, 1, [cnt], 0 if cnt == 0 else int(d2))
        elif (kw == 'ACVCOF' and t in (1, 2, 3)) or (kw != 'ACVCOF' and t == 1):
            table(s, 1, [int(v)], 1)
    elif kw in ('TIMEID', 'ERECID', 'NRECID'):
        a, b = ints(rest, 2)
        if a < 1 or b < 1:
            raise ValueError('Data Error')
        table(s, 1, [a], b)
    elif kw == 'FRQNCY':
        a, b = ints(rest, 2)
        if a > 0 and b > 0:
            table(s, 0, [], a * b)
    elif kw == 'VFACTR':
        if s.dim != 2:
            m, n = ints(rest, 2)
            if m == 1 and n > 0:
                for _ in range(n):
                    s.fnum()
    elif kw == 'USRDEF':
        for _ in range(min(ints(rest, 1)[0], 10)):
            s.fgets()
    elif kw in ('UENAME', 'UNNAME'):
        f, n = ints(rest, 2)
        if f < 2:
            s.counts[kw] = n
        for _ in range(s.counts.get(kw, 0)):
            s.fgets(64)
    elif kw == 'CVNAME':
        _, n = ints(rest, 2)
        s.fgets()
        for _ in range(n):
            s.fgets()
    elif kw == 'SOLMTD':
        m, k = ints(rest, 2)
        if m in (2, 6) and k == 4:
            s.fgets()


def g3(s, kw):
    if kw == 'CNTACT':
        s.fgets()
        return
    s.fint()
    s.fint()
    rest = s.fgets()
    if kw == 'FRCFAC':
        a = ints(rest, 2)
        d = nums(rest, 5)
        if a[0] == 4:
            for _ in range(2):
                h = s.fgets()
                dd = nums(h, 4)
                fric(s, ints(h, 1)[0], dd[1], dd[2], dd[3])
        else:
            fric(s, a[1], d[2], d[3], d[4])
    elif kw in ('IHTCOF', 'INTRST', 'FRCROT'):
        t = ints(rest, 1)[0]
        if t in (1, 2):
            table(s, 1, [int(nums(rest, 2)[1])], 1)
    elif kw == 'WMODEL':
        if ints(rest, 1)[0] in (1, 2):
            s.fgets()
    elif kw == 'FRCWIN':
        tok = rest.replace(',', ' ').split()

        def ti(i):
            m = re.match(r'[-+]?\d+', tok[i]) if i < len(tok) else None
            return int(m.group(0)) if m else 0

        def tf(i):
            try:
                return float(tok[i])
            except (IndexError, ValueError):
                return 0.0
        if s.dim != 2:
            shape, typ, a8, f8 = ti(1), ti(2), ti(3), tf(4)
            s.fgets()
            if shape == 1:
                for _ in range(8):
                    s.fgets()
            elif shape == 2:
                for _ in range(3):
                    s.fgets()
            else:
                raise ValueError('bad shape')
        else:
            npt, typ, a8, f8 = ti(1), ti(2), ti(3), tf(4)
            for _ in range(npt):
                s.fgets()
        if typ == 4:
            for _ in range(2):
                h = s.fgets()
                dd = nums(h, 4)
                fric(s, ints(h, 1)[0], dd[1], dd[2], dd[3])
        else:
            fric(s, a8, f8, 0, 0, extra_line=True)


WANT = {k for k, g in REG.items() if g in (1, 3)}
stat = collections.Counter()
bad = collections.defaultdict(list)
files = collections.defaultdict(set)
root = sys.argv[2]
for dp, dn, fn in os.walk(root):
    for f in fn:
        path = os.path.join(dp, f)
        t = open(path, 'rb').read().decode('latin-1').replace('\r\n', '\n')
        m = re.search(r'^\*\s*(DEFORM[-A-Z0-9]*)\s*(\S*)', t, re.M)
        if m:
            dim = 2 if (m.group(1).endswith('2') or m.group(1)[-2:] in ('2D', 'F2') or m.group(2) in ('2D', 'F2')) else 3
        else:
            dim = 2 if '/2D/' in path.replace('\\', '/') else 3
        s = S(t, dim)
        for mm in re.finditer(r'^(\S+)', t, re.M):
            kw = mm.group(1)
            if kw not in WANT:
                continue
            s.p = mm.end()
            try:
                (g1 if REG[kw] == 1 else g3)(s, kw)
                nxt = re.compile(r'\s*(\S+)').match(t, s.p)
                ok = nxt is None or nxt.group(1).startswith('*') or nxt.group(1) in REG
            except Exception:
                ok = False
            stat[(kw, ok)] += 1
            files[kw].add(os.path.relpath(path, root))
            if not ok and len(bad[kw]) < 3:
                bad[kw].append('%s:%d' % (os.path.relpath(path, root), t.count('\n', 0, mm.start()) + 1))
for k in sorted(WANT):
    ex = sorted(files[k])[0] if files[k] else '-'
    print('%s\t%d\t%d\t%d\t%s\t%s' % (k, stat[(k, True)], stat[(k, False)], len(files[k]), ex, ' '.join(bad[k])))
