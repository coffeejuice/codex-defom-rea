# P11 cross-check input: for each Cogging.MDT, dump the model read by the s1-03 Python prototype
# (tools/mdt.py) as flat "path value" lines and the prototype writer's output (tools/cogw.py) for
# firstOp = 0 and 1. Read-only on the inputs. Usage:
#   python -I gen_xcheck.py <s1-03 tools dir> <out dir> (<Cogging.MDT>... | --corpus <root>)
import os, sys
sys.dont_write_bytecode = True
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'support'))
from test_paths import output_path

tools, outdir, mdts = sys.argv[1], sys.argv[2], sys.argv[3:]
outdir = str(output_path(outdir))
if mdts[:1] == ['--corpus']:
    root = mdts[1]; mdts = []
    for d, _, fs in os.walk(root):
        mdts += [os.path.join(d, f) for f in fs if f.lower() == 'cogging.mdt']
    mdts.sort()
sys.path.insert(0, tools)
import mdt, cogw

def num(v):
    return repr(float(v)) if isinstance(v, float) else str(int(v))

def dump(m):
    L = []
    def put(k, v): L.append('%s %s' % (k, num(v)))
    pd = m['pd']
    for k in ('Process_Type', 'Temperaure_Type', 'Use_Mandrel', 'Checking_Method', 'Bottom_Fixed', 'Num_Steps_DSMAX'):
        put('pd.' + k, pd[k])
    for k in ('Use_Adaptive_Reheat', 'Heat_Transfer', 'Remaining', 'RotationalSymmetry'):
        put('pd.' + k, pd[k] & 0xff)        # byte fields as stored
    for k in ('TR0', 'TR1', 'Adaptive_Heating_Time', 'Swaging_Stroke', 'Dwell_Time', 'Cycle_Time', 'Decompression_Time'):
        put('pd.' + k, pd[k])
    put('UseManip', int(bool(m['UseManip'])))
    B = m['billet']
    for k in ('GeoTyp', 'AxialType', 'DoBilStraight'): put('billet.' + k, B.get(k, 0))
    for k in ('Length', 'Dia', 'InDia', 'Height', 'Width', 'AxialSub'): put('billet.' + k, float(B.get(k, 0.0)))
    D = m['die']
    put('die.four', int(bool(D['four']))); put('die.MovType', D.get('MovType', 0))
    for k in ('Length', 'CYCLES', 'TOTSTROKE'): put('die.' + k, float(D.get(k, 0.0)))
    M = m['manip']
    put('manip.UseSpring', M.get('UseSpring', 0))
    for k in ('Length', 'StartPos', 'LeftDis', 'RightDis'): put('manip.' + k, float(M.get(k, 0.0)))
    for r, P in enumerate(m['reheats'], 1):
        put('R%d.nPass' % r, len(P))
        for p, ps in enumerate(P, 1):
            pre = 'R%d.P%d.' % (r, p)
            for k in ('nBites', 'AutoBite', 'DiePosMethod', 'DieStrtNod', 'DieStopNod', 'DieMovDir', 'BitesToSkip'):
                put(pre + k, ps.get(k, 0))
            for k in ('RotAng', 'BiteAngle', 'MinDieDis', 'DieStrtPos', 'DieStopPos', 'HorizDieShift', 'RadialFeedPerBite', 'DwellTime'):
                put(pre + k, float(ps.get(k, 0.0)))
            for k in ('NSTEP', 'STPINC', 'SOLMTT'): put(pre + 'HT.' + k, ps['HT'][k])
            put(pre + 'HT.TMAX', float(ps['HT']['TMAX']))
            put(pre + 'nb', len(ps['bites']))
            for b, bt in enumerate(ps['bites'], 1):
                bp = pre + 'B%d.' % b
                put(bp + 'NomBite', float(bt['NomBite']))
                for k in ('NSTEP', 'STPINC', 'SOLMTT'): put(bp + 'HT.' + k, bt['HT'][k])
                put(bp + 'HT.TMAX', float(bt['HT']['TMAX']))
    put('nReheats', len(m['reheats']))
    return L

os.makedirs(outdir, exist_ok=True)
lst = []
for i, path in enumerate(mdts):
    m = mdt.read_mdt(path)
    if not m['pd']:
        print('skip (no process data):', path); continue
    name = 'c%03d' % i
    with open(os.path.join(outdir, name + '.model'), 'w', newline='\n') as f:
        f.write('\n'.join(dump(m)) + '\n')
    for fo in (0, 1):
        o, last = cogw.write(m, 4, dbname='X.DB', firstop=bool(fo))
        with open(os.path.join(outdir, '%s.f%d.expect' % (name, fo)), 'w', newline='\n') as f:
            f.write('\n'.join(o) + '\nEND %d\n' % last)
    lst.append('%s\t%s' % (name, path))
with open(os.path.join(outdir, 'cases.txt'), 'w', newline='\n') as f:
    f.write('\n'.join(lst) + '\n')
print(len(lst), 'cases')
