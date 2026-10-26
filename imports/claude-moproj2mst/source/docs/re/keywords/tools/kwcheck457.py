"""Reference KEY reader for keyword groups 4, 5 and 7 of DEF_PRE_64.exe (v11.0.2).

Transcribed from the decompiled group parsers FUN_140384a50 (group 4), FUN_140383120 (group 5),
FUN_14037e530 (group 7) and their line helpers (see ../_common_g457.md). It reproduces how many
characters/lines each keyword consumes and which values it stores, so the per-keyword grammar can
be checked on real KEY files.

Usage:  python -I kwcheck457.py <registry.tsv> <KEY file or directory>... [--dump KW] [--fail]

For every group 4/5/7 keyword found it runs the reader and checks that the stream then stands at
the end of a line and that the next token is a known keyword, a comment or EOF ("clean"). Other
keywords are skipped by resynchronising on the next line that starts with a keyword or '*'.
Read-only: it never writes the KEY files.
"""
import os
import re
import sys
from collections import defaultdict

WS = ' \t\n\v\f\r'

# ---------------------------------------------------------------- C stdio emulation


def i32(v):
    v &= 0xffffffff
    return v - (1 << 32) if v & 0x80000000 else v


_re_i = re.compile(r'[+-]?(0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)')
_re_f = re.compile(r'[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?')


def _conv_i(m):
    s = m.group(0)
    sign = -1 if s[0] == '-' else 1
    b = m.group(1)
    if b[:2] in ('0x', '0X'):
        v = int(b[2:], 16)
    elif b.startswith('0'):
        v = int(b, 8)
    else:
        v = int(b)
    return i32(sign * v)


def c_sscanf(s, fmt):
    """fmt: string of 'i' (%i) / 'f' (%lf) / 's' (%s). Returns (count or -1, values)."""
    pos, out = 0, []
    for k, d in enumerate(fmt):
        while pos < len(s) and s[pos] in WS:
            pos += 1
        if pos >= len(s):
            return (-1 if k == 0 else k), out
        if d == 's':
            e = pos
            while e < len(s) and s[e] not in WS:
                e += 1
            out.append(s[pos:e])
            pos = e
            continue
        m = (_re_i if d == 'i' else _re_f).match(s, pos)
        if not m:
            return k, out
        out.append(_conv_i(m) if d == 'i' else float(m.group(0)))
        pos = m.end()
    return len(fmt), out


class CFile:
    def __init__(self, text):
        self.t = text.replace('\r\n', '\n')
        self.p = 0

    def eof(self):
        return self.p >= len(self.t)

    def _skipws(self):
        while self.p < len(self.t) and self.t[self.p] in WS:
            self.p += 1

    def scan(self, d):
        """fscanf with one directive: returns (1, v), (0, None) on mismatch, (-1, None) on EOF."""
        self._skipws()
        if self.p >= len(self.t):
            return -1, None
        if d == 's39':
            e = self.p
            while e < len(self.t) and self.t[e] not in WS and e - self.p < 39:
                e += 1
            v = self.t[self.p:e]
            self.p = e
            return 1, v
        m = (_re_i if d == 'i' else _re_f).match(self.t, self.p)
        if not m:
            return 0, None
        self.p = m.end()
        return 1, (_conv_i(m) if d == 'i' else float(m.group(0)))

    def scanf(self, fmt):
        """fscanf with several directives (each preceded by whitespace skipping)."""
        out = []
        for k, d in enumerate(fmt):
            r, v = self.scan(d)
            if r == -1:
                return (-1 if k == 0 else k), out
            if r == 0:
                return k, out
            out.append(v)
        return len(fmt), out

    def fgets(self, n=0x400):
        if self.p >= len(self.t):
            return None
        e = self.t.find('\n', self.p)
        e = len(self.t) if e < 0 else e + 1
        e = min(e, self.p + n - 1)
        s = self.t[self.p:e]
        self.p = e
        return s

    def line_no(self, p=None):
        return self.t.count('\n', 0, self.p if p is None else p) + 1


# ---------------------------------------------------------------- line helpers (section 3)


def line_vals(f, fmt, skip_blank=False):
    """Generic FUN_1404xxxx line helper: fgets, ',' -> ' ', sscanf. Returns (err, values)."""
    s = f.fgets()
    if s is None:
        return 1, [0] * len(fmt)
    if skip_blank and s == '\n':
        s = f.fgets()
        if s is None:
            return 1, [0] * len(fmt)
    s = s.replace(',', ' ')
    n, v = c_sscanf(s, fmt)
    v = v + [0] * (len(fmt) - len(v))
    return (1 if n == -1 else 0), v


def read_name(f, size):
    """FUN_140413ab0: discard rest of line, next line is the string."""
    if f.fgets() is None:
        return 1, ''
    s = f.fgets()
    if s is None:
        return 1, ''
    s = s.split('\n')[0][:size - 1]
    return 0, s.rstrip(' \n\r')


# ---------------------------------------------------------------- DYNARY (section 4)

DEFAULTS = {  # type: (ndim, var[], ncomp, n0)
    0x07: (1, [3], 1, 0), 0x0a: (1, [5], 1, 0), 0x1c: (1, [31], 1, 0),
    0x27: (1, [3], 2, 0), 0x28: (1, [12], 2, 0), 0x29: (2, [3, 12], 1, 0),
    0x2c: (3, [12, 6, 3], 2, 0), 0x2d: (3, [7, 14, 3], 2, 0), 0x2e: (3, [12, 6, 3], 2, 0),
    0x2f: (3, [7, 14, 3], 2, 0), 0x30: (3, [12, 15, 16], 2, 0), 0x32: (1, [3], 1, 0),
    0x36: (1, [3], 1, 0), 0x37: (3, [12, 6, 3], 2, 0), 0x38: (1, [3], 1, 0),
    0x43: (3, [1, 6, 3], 1, 0), 0x44: (3, [7, 6, 3], 1, 0), 0x45: (1, [3], 2, 0),
    0x46: (1, [3], 2, 0), 0x47: (1, [3], 5, 0), 0x48: (1, [3], 3, 0),
    0x4f: (3, [7, 8, 3], 1, 0), 0x50: (3, [7, 12, 3], 1, 0), 0x51: (3, [7, 8, 3], 1, 0),
    0x52: (2, [13, 12], 1, 0), 0x53: (1, [1], 1, 0), 0x54: (1, [3], 6, 0),
    0x55: (1, [3], 4, 0), 0x56: (1, [3], 7, 0), 0x57: (1, [3], 6, 0), 0x58: (1, [3], 7, 0),
    0x59: (1, [3], 6, 0), 0x5a: (1, [3], 9, 0), 0x5b: (1, [3], 6, 0), 0x5c: (1, [3], 3, 0),
    0x65: (0, [], 6, 0), 0x66: (0, [], 3, 0), 0x67: (0, [], 1, 0), 0x68: (1, [], 6, 0),
    0x69: (1, [], 3, 0), 0xa6: (1, [0], 2, 1), 0xa7: (0, [], 1, 0), 0xa8: (0, [], 1, 0),
    0xa9: (1, [0], 2, 1), 0xaa: (0, [], 1, 0), 0xab: (0, [], 1, 0), 0xaf: (0, [], 2, 0),
    0xb4: (1, [32], 1, 0), 0xb5: (0, [], 0, 0),
}


class Dyn:
    def __init__(self, typ=0):
        self.init(typ)

    def init(self, typ):  # FUN_14012d2a0 / FUN_14012fc70
        self.type, self.ndim, self.const = typ, 0, 0.0
        self.var, self.n, self.ncomp = [0, 0, 0], [0, 0, 0], 1
        nd, var, nc, n0 = DEFAULTS.get(typ, (0, [], 1, 0))
        if typ == 0xae:
            self.const = 1.0
        self.ndim, self.ncomp = nd, nc
        for k, v in enumerate(var):
            self.var[k] = v
        if n0:
            self.n[0] = n0
        self.axes, self.values = [[], [], []], []
        return self

    def total(self):  # FUN_14012ff20
        if self.ndim == 0:
            if self.ncomp < 2 and self.type not in (0xb5, 0xa7, 0xa8, 0xaa, 0xab):
                return 0
            return self.ncomp
        t = self.ncomp
        for k in range(self.ndim):
            t *= self.n[k]
        return max(t, 0)

    def read_body(self, f):  # FUN_14041a800
        if not (self.ndim != 0 or self.ncomp > 1):
            return 0
        self.n = [max(x, 0) for x in self.n]
        self.ncomp = max(self.ncomp, 1)
        nd = self.ndim
        if nd == 0:
            if self.ncomp < 2:
                return 0
            for _ in range(self.ncomp):
                r, v = f.scan('f')
                if r == -1:
                    return 1
                self.values.append(v if r == 1 else 0.0)
        elif nd == 1:
            for i in range(self.n[0]):
                r, v = f.scan('f')
                if r == -1:
                    return 1
                self.axes[0].append(v if r == 1 else 0.0)
                for _ in range(self.ncomp):
                    r, v = f.scan('f')
                    if r == -1:
                        return 1
                    self.values.append(v if r == 1 else 0.0)
        elif 1 < nd < 4:
            for k in range(nd):
                for _ in range(self.n[k]):
                    r, v = f.scan('f')
                    if r == -1:
                        return 1
                    self.axes[k].append(v if r == 1 else 0.0)
            for _ in range(self.total()):
                r, v = f.scan('f')
                if r == -1:
                    return 1
                self.values.append(v if r == 1 else 0.0)
        if self.total() > 0:
            f.fgets()
        return 0

    def desc(self):
        if self.ndim == 0:
            return 'const %g' % self.const + (' %s' % self.values if self.values else '')
        return 'ndim%d var%s n%s ncomp%d' % (self.ndim, self.var[:self.ndim], self.n[:self.ndim],
                                             self.ncomp)


def set_dims(d, ndim, var, n):
    d.ndim = ndim
    for k, v in enumerate(var):
        d.var[k] = v
    for k, v in enumerate(n):
        d.n[k] = v


def gen_prop(f, d, want_extra=-1):
    """FUN_14041b270. Returns (err, extra or None)."""
    s = f.fgets()
    if s is None:
        return 1, None
    n, v = c_sscanf(s.replace(',', ' '), 'ifff')
    if n == -1:
        return 1, None
    v += [0] * (4 - len(v))
    t, a, b, c = v
    d.init(0)
    m = {0: None, 1: (1, [3], [int(a)]), 2: (1, [9], [int(a)]), 3: (1, [12], [int(a)]),
         4: (2, [3, 12], [int(a), int(b)]), 5: (2, [3, 8], [int(a), int(b)]),
         6: (1, [8], [int(a)]), 7: (2, [3, 4], [int(a), int(b)])}
    if t == 0:
        d.const = a
    elif t in m:
        set_dims(d, *m[t])
    extra = None
    if want_extra >= 0 and t >= 0:
        if t < 4 and n > 2:
            extra = b
        elif t == 4 and n > 3:
            extra = c
    return d.read_body(f), extra


def sub_fn(f, d):
    """FUN_14041c070 (GRNDAT sub-functions)."""
    s = f.fgets()
    if s is None:
        return 1
    if s == '\n':
        s = f.fgets()
        if s is None:
            return 1
    n, v = c_sscanf(s.replace(',', ' '), 'ifff')   # result not checked; t starts at -1
    v += [-1, 0, 0, 0][len(v):]
    t, a, b, c = v
    if t == 0:
        d.ndim, d.const = 0, a
    elif t == 1:
        set_dims(d, 1, [3], [int(a)])
    elif t == 2:
        set_dims(d, 1, [23], [int(a)])
    elif t == 3:
        set_dims(d, 2, [3, 23], [int(a), int(b)])
    if t > 0:
        return d.read_body(f)
    return 0


def grn_item(f, rec):
    """FUN_14041bed0: one GRNDAT type-2 list record."""
    r, n = f.scan('i')
    if r == -1:
        return 1
    n = min(n or 0, 12)          # FUN_14029ccd0 clamps to 12 ("Error - SubbdryVarSize is incorrect.")
    rec['coef'] = []
    for _ in range(max(n, 0)):
        r, v = f.scan('f')
        if r == -1:
            return 1
        rec['coef'].append(v)
    c, v = f.scanf('if')
    if c == -1:
        return 1
    rec['flag'] = v[0] if v else 0
    rec['val'] = v[1] if len(v) > 1 else 0.0
    # FUN_14029cd30: byte at +0x1e0 of the record; a fresh record has it 0 -> two sub-functions
    if not rec.get('b1e0'):
        for key in ('f78', 'ff0'):
            rec[key] = Dyn()
            if sub_fn(f, rec[key]):
                return 1
    rec['f168'] = Dyn()
    return 1 if sub_fn(f, rec['f168']) else 0


# ---------------------------------------------------------------- objects
# DYNARYs persist inside the material / phase-pair object between keywords, so cases that do not
# re-initialise their DYNARY see the constructor state. Keys are byte offsets in the object.

MAT_CTOR = {  # FUN_14028c6f0 and its sub-constructors (see _common_g457.md section 5)
    0x078: 0x4f, 0x138: 0x25, 0x1b0: 0x24, 0x228: 0x50, 0x388: 0xae, 0x400: 0x26, 0x480: 0xa6,
    0x4f8: 0xa7, 0x570: 0xa8, 0x5e8: 0x51,                       # FSTRES block at +0x50
    0x670: 0, 0x6e8: 0, 0x760: 0, 0x7e0: 0, 0x858: 0, 0x8d8: 0,
    0x958: 0x29, 0x9d0: 0x27, 0xa48: 0x28,                      # DIFCOE block at +0x950
    0xad0: 0x43, 0xb48: 0x44, 0xbc0: 0x45, 0xc38: 0x46, 0xcb0: 0x47, 0xd28: 0x48,
    0xda8: 0xa9, 0xe20: 0xaa, 0xe98: 0xab,                      # CREEP block at +0xac8
    0xf10: 0, 0xf88: 0x53, 0x1000: 0x52, 0x17f0: 0x5d, 0x1878: 0xb6,
    0x18f0: 0, 0x1968: 0, 0x19e0: 0, 0x1a58: 0,                 # BURGRS..RECVRY
    0x1b10: 0x1b, 0x1b90: 0x1c, 0x1c08: 0xb4,                   # FRCMOD block at +0x1ad0
    0x1cd8: 0, 0x1d50: 0, 0x1dc8: 0, 0x1e48: 0, 0x1ef0: 0, 0x1f68: 0, 0x1fe0: 0, 0x2058: 0,
    0x20d8: 0, 0x2160: 0xaf,
}
PAIR_CTOR = {0x78: 0x2c, 0xf8: 0x37, 0x180: 0xb3, 0x1f8: 0x30, 0x2c8: 0x32, 0x3f8: 0x36,
             0x478: 0, 0x4f0: 0, 0x568: 0, 0x5e8: 0, 0x678: 0x6d, 0x6f0: 0x6e}


class Obj(dict):
    def __init__(self, ctor):
        super().__init__()
        self.ctor = ctor

    def dyn(self, off, reinit=None):
        if off not in self:
            self[off] = Dyn(self.ctor.get(off, 0))
        if reinit is not None:
            self[off].init(reinit)
        return self[off]


def tab3(d, t, a, b, second):
    """Shared 't a b' mapping: 1 -> (3), 2 -> (second), 3 -> (3, second)."""
    if t == 1:
        set_dims(d, 1, [3], [int(a)])
    elif t == 2:
        set_dims(d, 1, [second], [int(a)])
    elif t == 3:
        set_dims(d, 2, [3, second], [int(a), int(b)])


# ---------------------------------------------------------------- group 4: FUN_140384a50


def g4(f, kid, mats):
    r, num = f.scan('i')
    if r == -1:
        return 2, None
    if num not in mats:
        mats[num] = Obj(MAT_CTOR)
        mats[num].update({'FRAE2H': 0.9})
    m = mats[num]
    k = kid - 0x5c
    st = 0
    D = m.dyn
    if k == 0:                                   # MTNAME
        st, m['name'] = read_name(f, 0x41)
    elif k in (1, 10, 11, 0x20):                 # FRAE2H TMPSOL TMPLIQ MATDEN
        st, (m[kid],) = line_vals(f, 'f')
    elif k == 2:                                 # FSTRES
        st, (t, f0, ec) = line_vals(f, 'iii')
        if st:
            return 2, m
        m['FSTRES.t'] = t
        if t == 1:
            st, m['FSTRES.c'] = line_vals(f, 'ffff')
        elif t in (2, 3):
            d = D(0x78, 0x4f)
            st, d.n = line_vals(f, 'iii')
            if st:
                return 2, m
            st = d.read_body(f)
        elif t == 4:
            st, m['FSTRES.c'] = line_vals(f, 'fffff')
        elif t == 5:
            st, m['FSTRES.c'] = line_vals(f, 'ffff')
        elif t in (6, 0xc):
            pairs = [(0x1b0, 0x24, f0), (0x138, 0x25, ec)] if t == 6 else [(0x400, 0x26, f0)]
            for off, typ, sel in pairs:
                st, (x, y) = line_vals(f, 'ff')
                if st:
                    return 2, m
                d = D(off, typ)
                if sel == 0:
                    d.const = x
                else:
                    tab3(d, sel, x, y, 12 if t == 6 else 8)
                if d.read_body(f):
                    return 2, m
            st = 0
        elif t in (7, 8):
            d = D(0x228, 0x50)
            st, d.n = line_vals(f, 'iii')
            if st:
                return 2, m
            st = d.read_body(f)
        elif t == 9:
            st, m['FSTRES.c1'] = line_vals(f, 'fffffff')
            st, m['FSTRES.c2'] = line_vals(f, 'fffffff')
        elif t == 10:
            st, m['FSTRES.c'] = line_vals(f, 'fffffff')
        elif t == 0xb:
            st, m['FSTRES.c'] = line_vals(f, 'fffff')
        elif t in (0xd, 0xf):
            st, (cnt,) = line_vals(f, 'i')
            if st:
                return 2, m
            d = D(0x480, 0xa6)
            d.n[0] = cnt
            if d.read_body(f):
                return 2, m
            xs = [int(x) for x in d.axes[0][:cnt]]
            s_, p_ = sum(xs), 1
            for x in xs:
                p_ = i32(p_ * x)
            for off, typ, nc in ((0x4f8, 0xa7, s_), (0x570, 0xa8, p_)):
                d = D(off, typ)
                d.ncomp = nc
                if d.read_body(f):
                    return 2, m
            st = 0
        elif t == 0xe:
            st, m['FSTRES.c'] = line_vals(f, 'ffff')
            st, _ = gen_prop(f, D(0x388))
        elif t == 0x10:
            d = D(0x5e8, 0x51)
            st, (m['FSTRES.16'],) = line_vals(f, 'i')
            if st:
                return 2, m
            st, d.n = line_vals(f, 'iii')
            if st:
                return 2, m
            st = d.read_body(f)
    elif k in (3, 4, 6, 8, 0x17, 0x1e, 0x21, 0x27, 0x29):   # plain property tables
        off = {3: 0x670, 4: 0x6e8, 6: 0x7e0, 8: 0x8d8, 0x17: 0x1cd8, 0x1e: 0x1dc8, 0x21: 0x1fe0,
               0x27: 0x17f0, 0x29: 0x1878}[k]
        st, _ = gen_prop(f, D(off))
    elif k == 5:                                 # EXPAND (+ reference temperature)
        st, m['EXPAND.ref'] = gen_prop(f, D(0x760), 1)
    elif k == 7:                                 # HEATCP (+ int)
        st, x = gen_prop(f, D(0x858), 1)
        m['HEATCP.i'] = int(x or 0)
    elif k == 0x1d:                              # PMEAB: 2-D tables get var[1] = 20
        d = D(0x1d50)
        st, _ = gen_prop(f, d)
        if d.ndim == 2:
            d.var[1] = 0x14
    elif k in (9, 0x11):                         # LTHEAT GRNMDL: one int, discarded
        st, _ = line_vals(f, 'i')
    elif k == 0xc:                               # FPERV
        st, (a, b, c, e) = line_vals(f, 'ffff')
        if st:
            return 2, m
        d1, d2 = D(0x1ef0, 0), D(0x1f68, 0)
        for d, sel, val in ((d1, int(b), a), (d2, int(e), c)):
            if sel == 0:
                d.const = val
            elif sel == 1:
                set_dims(d, 1, [1], [int(val)])
                st = d.read_body(f)
    elif k == 0xd:                               # FRCMOD
        st, v = line_vals(f, 'ifffff')
        if st:
            return 2, m
        t = v[0]
        m['FRCMOD'] = v
        if t == 0xb:
            if line_vals(f, 'fffffffff')[0]:
                return 2, m
        elif t == 0xc:
            d = D(0x1b90)
            d.ndim, d.var[0], d.n[0] = 1, 0x1f, int(v[2])
            st = d.read_body(f)
        elif t == 0x11:
            d = D(0x1c08)
            d.n[0] = int(v[2])
            st = d.read_body(f)
    elif k == 0xe:                               # CREEP
        st, (t, c8, _) = line_vals(f, 'iii')
        if st:
            return 2, m
        if t in (1, 2, 3, 4):
            d = D({1: 0xbc0, 2: 0xc38, 3: 0xcb0, 4: 0xd28}[t], 0x44 + t)
            d.n[0] = c8
            st = d.read_body(f)
        elif t in (5, 6):
            st, n = line_vals(f, 'iii')
            if st:
                return 2, m
            d = D(0xb48, 0x44) if t == 5 else D(0xad0, 0x43)
            d.n = n
            st = d.read_body(f)
        elif t in (7, 8):
            st, (cnt,) = line_vals(f, 'i')
            if st:
                return 2, m
            d = D(0xda8, 0xa9)
            d.n[0] = cnt
            if d.read_body(f):
                return 2, m
            xs = [int(x) for x in d.axes[0][:cnt]]
            s_, p_ = sum(xs), 1
            for x in xs:
                p_ = i32(p_ * x)
            for off, typ, nc in ((0xe20, 0xaa, s_), (0xe98, 0xab, p_)):
                d = D(off, typ)
                d.ncomp = nc
                if d.read_body(f):
                    return 2, m
            st = 0
    elif k == 0xf:                               # DIFCOE
        st, (t, a, b, c, e) = line_vals(f, 'iffff')
        if st:
            return 2, m
        if t == 0:
            D(0x958, 0).const = a
        elif t == 1:
            d = D(0x958, 0x29)
            d.n[0], d.n[1] = int(a), int(b)
            st = d.read_body(f)
        elif t in (2, 3):
            d = D(0x9d0, 0x27) if t == 2 else D(0xa48, 0x28)
            d.n[0] = int(a)
            st = d.read_body(f)
    elif k == 0x10:                              # GRNDAT
        st, (t, c4) = line_vals(f, 'ii')
        if st:
            return 2, m
        m['GRNDAT.t'] = t
        if t == 1:
            spec = [(0x1088, 0x55, {1: 3, 2: 8}), (0x1100, 0x54, {1: 3, 2: 8}),
                    (0x1178, 0x56, {1: 3, 2: 8}), (0x11f0, 0x57, {1: 3, 2: 8, 3: 0x21}),
                    (0x1268, 0x58, {1: 3, 2: 8}), (0x12e0, 0x59, {1: 3, 2: 8, 3: 0x21}),
                    (0x1358, 0x5a, {1: 3, 2: 8}), (0x13d0, 0x5b, {1: 3, 2: 8, 3: 0x21}),
                    (0x1448, 0x5c, {1: 3, 2: 8, 3: 0x21})]
            ds = []
            for off, typ, vm in spec:
                st, (sel, cnt) = line_vals(f, 'ii')
                if st:
                    return 2, m
                d = D(off, typ)
                if sel in vm:
                    d.var[0] = vm[sel]
                d.n[0] = cnt
                ds.append(d)
            for d in ds:
                if d.read_body(f):
                    return 2, m
            st, m['GRNDAT.tail'] = line_vals(f, 'ff')
        elif t == 2:
            if grn_item(f, {'b1e0': 1}):   # main record: FUN_14028b640 sets byte +0x1e0
                return 2, m
            for _ in range(5):
                _, (cnt,) = line_vals(f, 'i', True)
                for _ in range(cnt):
                    if grn_item(f, {}):
                        return 2, m
            st = 0
        elif t == 4:
            st, _ = gen_prop(f, D(0x1740))
        elif t == 5:
            st, m['GRNDAT.5'] = line_vals(f, 'ffffff')
    elif k == 0x12:                              # BURGRS (+0x18f0, not re-initialised)
        st, (t, a, b) = line_vals(f, 'iff')
        if st:
            return 2, m
        d = D(0x18f0)
        mp = {1: (1, [3]), 2: (1, [6]), 3: (1, [24]), 4: (2, [3, 6]), 5: (2, [3, 24]),
              6: (2, [6, 24])}
        if t == 0:
            d.ndim, d.const = 0, a
        elif t in mp:
            set_dims(d, mp[t][0], mp[t][1], [int(a), int(b)][:mp[t][0]])
        elif t == 7:
            st, n = line_vals(f, 'iii')
            if st:
                return 2, m
            set_dims(d, 3, [3, 6, 24], n)
        else:
            return 2, m
        st = d.read_body(f)
    elif k in (0x13, 0x14, 0x15):                # ALPHA NDISFM RECVRY (not re-initialised)
        st, (t, a, b) = line_vals(f, 'iff')
        if st:
            return 2, m
        d = D({0x13: 0x1968, 0x14: 0x19e0, 0x15: 0x1a58}[k])
        if t == 0:
            d.ndim, d.const = 0, a
        elif t in (1, 2, 3):
            tab3(d, t, a, b, 8)
        else:
            return 2, m
        st = d.read_body(f)
    elif k == 0x16:                              # HDNPHA
        st, (t, a, b) = line_vals(f, 'iff')
        if st:
            return 2, m
        d = D(0xf10, 0x23)
        mp = {1: (1, [12]), 2: (1, [3]), 3: (1, [9]), 4: (2, [3, 12])}
        if t == 0:
            d.ndim, d.const = 0, a
        elif t in mp:
            set_dims(d, mp[t][0], mp[t][1], [int(a), int(b)][:mp[t][0]])
        else:
            return 3, m
        st = d.read_body(f)
    elif k == 0x18:                              # MSTMTR
        st, (cnt,) = line_vals(f, 'i')
        if st:
            return 2, m
        lst = []
        for _ in range(cnt):                     # only when cnt >= 1
            r, v = f.scan('f')
            if r == -1:
                return 2, m
            lst.append(int(v) if r == 1 else 0)
        m[kid] = lst
    elif k == 0x19:                              # HDNTIM
        st, (cnt,) = line_vals(f, 'i')
        if st:
            return 2, m
        d = D(0xf88, 0x53)
        d.n[0] = cnt
        st = d.read_body(f)
    elif k == 0x1a:                              # JOMINY
        st, (t, a, b) = line_vals(f, 'iii')
        if st:
            return 2, m
        d = D(0x1000, 0x52)
        if t == 1:
            d.n[0], d.n[1] = a, b
            st = d.read_body(f)
    elif k == 0x1b:                              # UTSDAT
        st, (t, a) = line_vals(f, 'if')
        if st:
            return 2, m
        d = D(0x1b10, 0x1b)
        if t == 0:
            d.const = a
        elif t == 1:
            set_dims(d, 1, [3], [int(a)])
            st = d.read_body(f)
    elif k in (0x1c, 0x23):                      # HDNRUL SIZEMD
        st, (m[kid],) = line_vals(f, 'i')
    elif k == 0x1f:                              # ANISO
        st, (t, nc, sel) = line_vals(f, 'iii')
        if st:
            return 2, m
        if t < 0:
            m['ANISO.t'] = 5
            return 0, m
        d = D(0x1e48)
        if t in (1, 2, 3, 4):
            d.init({1: 0x68, 2: 0x69, 3: 0x67, 4: 0x65}[t])
            d.ncomp = nc
        if sel in (1, 2):
            d.ndim, d.var[0] = 1, (3 if sel == 1 else 7)
            st, (d.n[0],) = line_vals(f, 'i')
        elif sel in (3, 4):
            d.ndim = 2
            d.var[0], d.var[1] = (7, 3) if sel == 3 else (3, 0x1e)
            st, (d.n[0], d.n[1]) = line_vals(f, 'ii')
        if st:
            return 2, m
        if d.read_body(f):
            return 2, m
        st = 0
    elif k == 0x22:                              # COARSE
        st, (flag,) = line_vals(f, 'i')
        m['COARSE.flag'] = flag
        if flag > 0:
            st, _ = gen_prop(f, D(0x20d8))
    elif k == 0x24:                              # TXTURE
        st, m[kid] = line_vals(f, 'iii')
        if st:
            return 2, m
    elif k == 0x25:                              # DIFBND
        st, (t, a, b) = line_vals(f, 'iff')
        d = D(0x2160)
        if st == 0 and t == 7:
            set_dims(d, 2, [3, 4], [int(a), int(b)])
            st = d.read_body(f)
        else:
            if st or t != 0:
                return 2, m
            d.n[0] = d.n[1] = 0
            d.const = a
    elif k == 0x26:                              # SINTER
        st, (flag, cnt) = line_vals(f, 'ii')
        if flag != 0 and cnt > 0:
            st, m[kid] = line_vals(f, 'ffff')
    elif k == 0x28:                              # GBMOBI
        st, m[kid] = line_vals(f, 'ff')
    else:
        return 2, m
    return (0 if st == 0 else 2), m


# ---------------------------------------------------------------- group 5: FUN_140383120


def g5(f, kid, pairs):
    c, v = f.scanf('ii')
    if c == -1:
        return 2, None
    v += [0] * (2 - len(v))
    key = tuple(v)
    if key not in pairs:
        pairs[key] = Obj(PAIR_CTOR)
    p = pairs[key]
    D = p.dyn
    k = kid - 0x86
    st = 0
    if k == 0:                                   # PHASTF
        st, (p['PHASTF'],) = line_vals(f, 'i')
    elif k == 1:                                 # TTTD
        st, (t, kind) = line_vals(f, 'ii')
        if st:
            return 2, p
        p['TTTD.t'] = t
        if t == 1:
            st, (sub, nc, x3c) = line_vals(f, 'iii')
            if st:
                return 2, p
            st, _ = line_vals(f, 'ff')
            if st:
                return 2, p
            d = D(0x78) if sub not in (0, 1, 2, 3) else D(0x78, 0x2c + sub)
            d.ncomp = nc
            st, d.n = line_vals(f, 'iii')
            if st:
                return 2, p
            st = d.read_body(f)
        elif t == 2:
            st, (q, n0, n1, n2) = line_vals(f, 'iiii')
            if st:
                return 2, p
            d = D(0x1f8, 0x30)
            d.n = [n0, n1, n2]
            if q != 2:
                return 3, p
            d.ncomp = 2
            st = d.read_body(f)
        elif t in (3, 5, 7):
            c_, _ = f.scanf('f' * {3: 11, 5: 6, 7: 7}[t])
            if c_ == -1:
                return 2, p
        elif t == 4:
            st, _ = line_vals(f, 'ffff')
            if st:
                return 2, p
            st, (cnt,) = line_vals(f, 'i')
            if st:
                return 2, p
            d = D(0x2c8, 0x32)
            d.n[0] = cnt
            st = d.read_body(f)
        elif t == 6:
            st, _ = line_vals(f, 'ffff')
        elif t == 8:
            st, (cnt,) = line_vals(f, 'i')
            if st:
                return 2, p
            d = D(0x3f8, 0x36)
            d.n[0] = cnt
            st = d.read_body(f)
        elif t == 9:
            st, (_, nc) = line_vals(f, 'ii')
            if st:
                return 2, p
            st, _ = line_vals(f, 'ff')
            if st:
                return 2, p
            d = D(0xf8, 0x2c)
            d.ncomp = nc
            st, d.n = line_vals(f, 'iii')
            if st:
                return 2, p
            st = d.read_body(f)
        elif t in (10, 11, 12, 13):
            st, (cnt,) = line_vals(f, 'i')
            if st:
                return 2, p
            d = D(0x180)
            d.ncomp = cnt
            st = d.read_body(f)
    elif k == 2:                                 # TTTN
        st, p['name'] = read_name(f, 0x51)
    elif k in (3, 4, 8):                         # PHASLH PHASVL IPSNRG
        st, (t, a, b) = line_vals(f, 'iff')
        if st:
            return 2, p
        off, typ, second = {3: (0x478, 0x1f, 12), 4: (0x4f0, 0x20, 12), 8: (0x6f0, 0x6e, 0x17)}[k]
        d = D(off, typ)
        if t == 0:
            d.const = a
        else:
            tab3(d, t, a, b, second)
        st = d.read_body(f)
    elif k == 5:                                 # TRNSFP (DYNARY not re-initialised)
        st, (on, t, a, b) = line_vals(f, 'iiff')
        if st:
            return 2, p
        p['TRNSFP.on'] = 1 if on else 0
        if on:
            d = D(0x5e8)
            if t == 0:
                d.ndim, d.const = 0, a
            else:
                tab3(d, t, a, b, 12)
            st = d.read_body(f)
    elif k == 6:                                 # PHEQVF
        st, (t, a, b) = line_vals(f, 'iff')
        if st:
            return 2, p
        d = D(0x568, 0x6c)
        if t in (0, 4):
            d.const = a
        elif t in (1, 2, 3, 5, 6, 7):
            tab3(d, t if t < 4 else t - 4, a, b, 12)
        st = d.read_body(f)
    elif k == 7:                                 # COARSN
        st, (x0, x1, sel, val, _) = line_vals(f, 'fffff')
        if st:
            return 2, p
        d = D(0x678, 0x6d)
        if int(sel) == 0:
            d.const = val
        elif int(sel) == 1:
            set_dims(d, 1, [3], [int(val)])
        st = d.read_body(f)
    elif k == 9:                                 # VARSEL
        st, (p['VARSEL'],) = line_vals(f, 'i')
    else:
        return 2, p
    return (0 if st == 0 else 2), p


# ---------------------------------------------------------------- group 7: FUN_14037e530


def g7(f, kid, model):
    if kid != 0x120:
        return 2, None
    st, (d,) = line_vals(f, 'i')
    model['DIM'] = d
    return (0 if st == 0 else 2), model


# ---------------------------------------------------------------- driver


def load_registry(path):
    reg = {}
    for row in open(path).read().splitlines()[1:]:
        c = row.split('\t')
        reg[c[1]] = (int(c[0]), int(c[2]))
    return reg


def check_file(path, reg, stats, dump=None, show_fail=False):
    f = CFile(open(path, encoding='latin-1', newline='').read())
    mats, pairs, model = {}, {}, {}
    while True:
        r, tok = f.scan('s39')
        if r == -1:
            break
        if not tok or tok[0] == '*':
            f.fgets()
            continue
        if tok not in reg:
            f.fgets()
            continue
        kid, grp = reg[tok]
        if grp not in (4, 5, 7):
            f.fgets()
            # resync: skip lines until one starts with a keyword or '*'
            while not f.eof():
                save = f.p
                line = f.fgets()
                w = line.split()
                if not w or w[0][0] == '*' or w[0] in reg:
                    f.p = save
                    break
            continue
        start = f.p
        ln = f.line_no()
        if grp == 4:
            rc, obj = g4(f, kid, mats)
        elif grp == 5:
            rc, obj = g5(f, kid, pairs)
        else:
            rc, obj = g7(f, kid, model)
        at_bol = f.p == 0 or f.t[f.p - 1] == '\n'
        eol = f.t.find('\n', f.p)
        rest = '' if at_bol else f.t[f.p:eol if eol >= 0 else len(f.t)]
        save = f.p
        r2, nxt = f.scan('s39')
        f.p = save
        clean = rest.strip() == '' and (r2 == -1 or nxt[0] == '*' or nxt in reg or nxt == 'SRFNRG')
        ok = rc == 0 and clean
        stats[tok]['ok' if ok else 'fail'] += 1
        stats[tok]['files'].add(path)
        if not ok:
            stats[tok]['bad'].append('%s:%d rc=%d next=%r' % (path, ln, rc, nxt))
            if show_fail:
                print('FAIL %s %s:%d rc=%d rest=%r next=%r' % (tok, path, ln, rc, rest, nxt))
        if dump == tok:
            print('%s:%d %s ->' % (path, ln, tok), {k: (v.desc() if isinstance(v, Dyn) else v)
                                                      for k, v in (obj or {}).items()
                                                      if not str(k).startswith('_')})


def main(argv):
    reg = load_registry(argv[1])
    dump = argv[argv.index('--dump') + 1] if '--dump' in argv else None
    show = '--fail' in argv
    paths = []
    for a in argv[2:]:
        if a.startswith('--') or a == dump:
            continue
        if os.path.isdir(a):
            for dp, _, fs in os.walk(a):
                paths += [os.path.join(dp, x) for x in fs if x.upper().endswith('.KEY')]
        else:
            paths.append(a)
    stats = defaultdict(lambda: {'ok': 0, 'fail': 0, 'files': set(), 'bad': []})
    for p in sorted(paths):
        check_file(p, reg, stats, dump, show)
    for kw, s in sorted(stats.items(), key=lambda x: reg[x[0]][0]):
        print('%-7s id=%3d ok=%5d fail=%4d files=%4d' % (kw, reg[kw][0], s['ok'], s['fail'],
                                                         len(s['files'])))
        for b in s['bad'][:3]:
            print('        ', b)


if __name__ == '__main__':
    main(sys.argv)
