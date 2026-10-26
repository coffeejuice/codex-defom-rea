# Compare the cogging-specific lines of corpus MST files with the cogw.py prototype (read-only on the corpus).
import sys, os, re, xml.dom.minidom
sys.path.insert(0, os.path.dirname(__file__))
import mdt, cogw

GEN_HEAD = ('MDSOBJ    ', 'TNOW', 'OBJTYP', 'MTLGRP', 'CNTACT', 'IMPEQP', 'OPRNAM', 'CURSIM', 'GENDB', 'SIMULATION', '*!SIMTYP', 'DIM', 'NSTART')
TAKES_NEXT = ('DBREAD', 'KFREAD', 'IMPEQP', 'OPRNAM', 'GENDB')

def ops_of(moproj):
    dom = xml.dom.minidom.parse(moproj); res = {}
    for op in dom.getElementsByTagName('Operation'):
        def t(n):
            e = op.getElementsByTagName(n)
            return e[0].firstChild.data if e and e[0].firstChild else ''
        ak = []
        for k in op.getElementsByTagName('Keyword'):
            if k.firstChild: ak += [x for x in k.firstChild.data.split('\n') if x.strip()]
        res.setdefault(t('Name'), []).append((int(t('Id')), t('Type'), ak))
    return res

def filt(lines, ak):
    out = []; skip = False
    akset = set(x.rstrip() for x in ak)
    for l in lines:
        if skip: skip = False; continue
        if l.startswith(TAKES_NEXT) and not l.startswith('KFREAD\t1') or l.startswith('KFREAD    '):
            skip = l.startswith(TAKES_NEXT); continue
        if l.startswith(GEN_HEAD) or l.strip() == '' or l.startswith('*') : continue
        if l.rstrip() in akset: continue
        out.append(l)
    return out

def check(proj_moproj, verbose=False):
    d = os.path.dirname(proj_moproj)
    mst = [f for f in os.listdir(d) if f.upper().endswith('.MST')][0]
    L = open(os.path.join(d, mst), 'rb').read().decode('latin1').split('\n')
    ops = ops_of(proj_moproj)
    # split into blocks
    blocks = []; cur = None
    for i, l in enumerate(L):
        m = re.match(r'SIMULATION\t(\d+)$', l)
        if m: cur = [int(m.group(1)), None, []]; blocks.append(cur); continue
        if cur is None: continue
        if l.startswith('*!SIMTYP'): cur[1] = l.split('\t')[-1]; continue
        if l.startswith('* END OF MASTER'): break
        cur[2].append(l)
    # the separator line preceding next SIMULATION belongs to the previous block; fine (filtered)
    res = []; i = 0
    while i < len(blocks):
        if blocks[i][1] != 'SfOprCogging': i += 1; continue
        j = i; body = []
        opnam = None
        def curop(bl):
            for k, l in enumerate(bl[2]):
                if l.startswith('CURSIM'): return l.split('\t')[2]
        c0 = curop(blocks[i])
        while j < len(blocks) and blocks[j][1] == 'SfOprCogging' and curop(blocks[j]) == c0:
            bl = blocks[j][2]
            for k, l in enumerate(bl):
                if l.startswith('OPRNAM'): opnam = bl[k + 1]
            body += bl; j += 1
        cand = ops.get(opnam, [])
        cand = [x for x in cand if x[1] == 'SfOprCogging']
        if len(cand) != 1: res.append((opnam, 'op?')); i = j; continue
        oid, _, ak = cand[0]
        path = os.path.join(d, 'Operations', 'Task%05d' % oid, 'Cogging.MDT')
        m = mdt.read_mdt(path)
        gen, last = cogw.write(m, blocks[i][0] - 1, firstop=(blocks[i][0] == 1))
        exp = [x for x in gen if not x.startswith('GEN:')]
        exp = filt(exp, ak); act = filt(body, ak)
        nblk = j - i
        ok = exp == act and last == blocks[j - 1][0]
        if not ok and verbose:
            import difflib
            print('\n'.join(list(difflib.unified_diff(act, exp, 'actual', 'proto', lineterm='', n=2))[:60]))
        res.append((opnam, 'OK' if ok else 'DIFF', nblk, last - blocks[i][0] + 1))
        i = j
    return res

if __name__ == '__main__':
    for p in sys.argv[1:]:
        print(p, check(p, verbose=True))
