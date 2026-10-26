# Worker check script (not a full reference): emulates read+write of FSTRES/FPERV/FRCMOD/CREEP
# and compares with the oracle golden KFWRIT outputs; matched all 8 material and 7 variant files.
# Emulates FSTRES/FPERV/FRCMOD/CREEP read (FUN_140384a50) + write handlers; compares with golden.
import sys, re
def E(x, w=20, p=10):
    s = '%.*E' % (p, x); m, e = s.split('E'); e = int(e)
    s = '%sE%s%03d' % (m, '+' if e >= 0 else '-', abs(e)); return s.rjust(w)
def I(x): return '%7d' % x
class R:
    def __init__(s, path): s.d = open(path, encoding='latin-1').read(); s.p = 0
    def line(s):
        i = s.d.find('\n', s.p); i = len(s.d) if i < 0 else i + 1
        l = s.d[s.p:i]; s.p = i; return l.replace(',', ' ')
    def tok(s):
        m = re.compile(r'\s*(\S+)').match(s.d, s.p); s.p = m.end(); return m.group(1)
    def f(s): return float(s.tok())
def nums(l, fmt):
    out = []; t = l.split()
    for k, c in enumerate(fmt):
        if k >= len(t): out.append(0); continue
        out.append(int(t[k], 0) if c == 'i' else float(t[k]))
    return out
def dyn(ndim=0, var=(0,0,0), n=(0,0,0), ncomp=1, const=0.0):
    return dict(ndim=ndim, var=list(var), n=list(n), ncomp=ncomp, const=const, ax=[[],[],[]], v=[])
def total(d):
    if d['ndim'] == 0: return d['ncomp'] if d['ncomp'] >= 2 else 0
    t = d['ncomp']
    for k in range(d['ndim']): t *= d['n'][k]
    return t
def rbody(r, d):
    if d['ndim'] == 0 and d['ncomp'] < 2: return
    if d['ndim'] == 0: d['v'] = [r.f() for _ in range(d['ncomp'])]
    elif d['ndim'] == 1:
        for i in range(d['n'][0]):
            d['ax'][0].append(r.f()); d['v'] += [r.f() for _ in range(d['ncomp'])]
    else:
        for k in range(d['ndim']): d['ax'][k] = [r.f() for _ in range(d['n'][k])]
        d['v'] = [r.f() for _ in range(total(d))]
    if total(d) > 0: r.line()
def wrow(vals, per):
    if per < 1 or not vals: return ''
    o = ''
    for i, x in enumerate(vals):
        if i and i % per == 0: o += '\n' + ' ' * 8
        o += ' ' + E(x)
    return o + '\n'
def wbody(d, mode):
    o = ''
    if mode not in (1, 2) and any(d['n'][k] < 1 for k in range(d['ndim'])): return ''
    mx = max([d['n'][k] for k in range(d['ndim'])] + [0])
    if d['ndim'] == 0:
        if mode not in (1, 2, 6): return ''
        if d['ncomp'] == 1: return ' ' + E(d['const']) + '\n'
        return wrow(d['v'], 5) if d['ncomp'] >= 2 else ''
    if d['ndim'] == 1:
        if mode in (1, 2): o += ' ' + I(d['n'][0]) + '\n'
        nc = d['ncomp']
        for i in range(d['n'][0]): o += ' ' + E(d['ax'][0][i]) + wrow(d['v'][i*nc:(i+1)*nc], 5)
        return o
    if mode in (1, 2): o += ''.join(' ' + I(d['n'][k]) for k in range(d['ndim'])) + '\n'
    for k in range(d['ndim']): o += wrow(d['ax'][k], mx)
    return o + wrow(d['v'], mx)
def hdr(name): return '%-6s' % name
def run(path):
    r = R(path); out = {}
    while True:
        m = re.compile(r'\s*(\S+)').match(r.d, r.p)
        if not m: break
        kw = m.group(1); r.p = m.end()
        if kw not in ('FSTRES', 'FPERV', 'FRCMOD', 'CREEP'): continue
        mat = int(r.tok()); o = ''
        if kw == 'FPERV':
            a, b, c, dd = nums(r.line(), 'ffff')
            d1 = dyn(const=a); d2 = dyn(const=c)
            if int(b) == 1: d1 = dyn(1, (1,0,0), (int(a),0,0)); rbody(r, d1)
            if int(dd) == 1: d2 = dyn(1, (1,0,0), (int(c),0,0)); rbody(r, d2)
            h = hdr(kw) + ' ' + I(mat)
            h += (' ' + E(d1['const']) + ' ' + I(0)) if d1['ndim'] == 0 else (' ' + I(d1['n'][0]) + ' ' + I(1))
            h += (' ' + E(d2['const']) + ' ' + I(0)) if d2['ndim'] == 0 else (' ' + I(d2['n'][0]) + ' ' + I(1))
            o = h + '\n' + wbody(d1, 0) + wbody(d2, 0)
        elif kw == 'FRCMOD':
            t, a, b, c, dd, e = nums(r.line(), 'ifffff')
            assert t in (0,1,2,3,4,5,6,8,9,10), t
            B = b if t in (2, 4, 5) else 0.0
            o = hdr(kw) + ' ' + I(mat) + ' ' + I(t) + ''.join(' ' + E(x) for x in (a, B, 0.0, 0.0, e)) + '\n'
        elif kw == 'CREEP':
            t, x, y = nums(r.line(), 'iii')
            if t == 0: o = hdr(kw) + ' ' + I(mat) + ' ' + I(0) + '\n'
            elif t in (5, 6):
                n = nums(r.line(), 'iii'); d = dyn(3, (7,6,3) if t == 5 else (1,6,3), n); rbody(r, d)
                o = hdr(kw)+' '+I(mat)+' '+I(t)+'\n'+wbody(d, 1)
            else: raise Exception('creep %d' % t)
        else:
            t, f0, ec = nums(r.line(), 'iii')
            if t == 1:
                v = nums(r.line(), 'ffff'); o = hdr(kw)+' '+I(mat)+' '+I(1)+'\n'+''.join(' '+E(x) for x in v)+'\n'
            elif t in (4, 11):
                v = nums(r.line(), 'fffff'); o = hdr(kw)+' '+I(mat)+' '+I(t)+'\n'+''.join(' '+E(x) for x in v)+'\n'
            elif t in (2, 3):
                n = nums(r.line(), 'iii'); d = dyn(3, (7,8,3), n); rbody(r, d)
                o = hdr(kw)+' '+I(mat)+' '+I(2 if t == 2 else 3)+'\n'+wbody(d, 1)
            elif t == 6:
                ds = []
                for sub in (f0, ec):
                    a, b = nums(r.line(), 'ff')
                    d = {0: dyn(const=a), 1: dyn(1,(3,0,0),(int(a),0,0)), 2: dyn(1,(12,0,0),(int(a),0,0)), 3: dyn(2,(3,12,0),(int(a),int(b),0))}.get(sub, dyn())
                    rbody(r, d); ds.append(d)
                def code(d):
                    return 0 if d['ndim']==0 else 1 if d['ndim']==1 and d['var'][0]==3 else 2 if d['ndim']==1 else 3
                Y, H = ds
                o = hdr(kw)+' '+I(mat)+' '+I(6)+' '+I(code(Y))+' '+I(code(H))+'\n'+wbody(Y,1)+wbody(H,1)
            else: raise Exception('type %d' % t)
        out.setdefault(kw, []).append(o)
    return out
def golden(path):
    d = open(path, encoding='latin-1').read(); res = {}
    lines = d.split('\n'); i = 0
    while i < len(lines):
        k = lines[i].split()[0] if lines[i].strip() else ''
        if k in ('FSTRES','FPERV','FRCMOD','CREEP'):
            j = i + 1
            while j < len(lines) and not re.match(r'^[A-Z*]', lines[j]): j += 1
            res.setdefault(k, []).append('\n'.join(lines[i:j]) + '\n'); i = j
        else: i += 1
    return res
src, gold = sys.argv[1], sys.argv[2]
a, b = run(src), golden(gold)
for k in sorted(set(a) | set(b)):
    for x, y in zip(a.get(k, []), b.get(k, [])):
        print(k, 'MATCH' if x == y else 'DIFF', len(y))
        if x != y:
            for p,(c1,c2) in enumerate(zip(x,y)):
                if c1!=c2: print(repr(x[max(0,p-80):p+40])); print(repr(y[max(0,p-80):p+40])); break
            else: print('len', len(x), len(y))
    if len(a.get(k,[])) != len(b.get(k,[])): print(k,'count', len(a.get(k,[])), len(b.get(k,[])))
