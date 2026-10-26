# Golden values for test_mdt.cpp from the s1-03 Python prototype (docs/re/moproj2mst/s1-03/tools/mdt.py).
# usage: python -I gen_mdt_golden.py <corpus dir> <out file>
# Doubles are written as the hex of their bits, so the C++ test compares them exactly.
import os, struct, sys

here = os.path.dirname(os.path.abspath(__file__))
sys.dont_write_bytecode = True
sys.path.insert(0, os.path.join(here, '..', 'support'))
from test_paths import output_path
sys.path.insert(0, os.path.normpath(os.path.join(here, '..', '..', '..', '..', 'docs', 're', 'moproj2mst', 's1-03', 'tools')))
import mdt  # noqa: E402

def h(v):
    return format(struct.unpack('<Q', struct.pack('<d', float(v)))[0], '016x')

def heat(out, pre, t):
    out.append('%s.ht %d %d %d %s %s %s %s|%s' % (pre, t['Active'], t['NSTEP'], t['STPINC'], h(t['TMAX']),
               h(t['DTPMAX']), h(t['IHTCOF']), h(t.get('IHTCOF2', 0.0)), t['UserKey']))

def dump(m):
    out = ['ver %d' % m['ver']]
    B, D, M, pd = m['billet'], m['die'], m['manip'], m['pd']
    out.append('billet %d %s %s %s %s %s %s %s %s %d %d|%s|%s %d %d' % (
        B['GeoTyp'], h(B['Length']), h(B['Dia']), h(B.get('InDia', 0)), h(B['Height']), h(B.get('Width', 0)),
        h(B['Radius']), h(B['Layers']), h(B['TMPBIL']), B['BccMod'], B['MergeL'], B['KeyBil'],
        h(B.get('AxialSub', 0)), B.get('AxialType', 0), B.get('DoBilStraight', 0)))
    out.append('die %s %d %s %s %s %s %d %d %d' % (h(D['Length']), D['DieMov'], h(D['TMPDIE']), h(D['SPDDIE']),
               h(D.get('CYCLES', 0)), h(D.get('TOTSTROKE', 0)), D['NoDies'], int(D['four']), D.get('MovType', 0)))
    out.append('manip %s %s %s %s %d %d' % (h(M.get('Length', 0)), h(M.get('StartPos', 0)), h(M.get('LeftDis', 0)),
               h(M.get('RightDis', 0)), M.get('UseMstMan', 0), M.get('UseSpring', 0)))
    out.append('sim %d %d' % (m['UNIT'], int(m['UseManip'])))
    out.append('reheats %d' % len(m['reheats']))
    for r, P in enumerate(m['reheats']):
        out.append('r%d passes %d' % (r, len(P)))
        for i, p in enumerate(P):
            pre = 'r%dp%d' % (r, i)
            out.append('%s %d %s %s %s %d %s %s %d %d %d %d %s %s %s %d %s %s' % (
                pre, p['nBites'], h(p['RotAng']), h(p['BiteAngle']), h(p['MinDieDis']), p['DiePosMethod'],
                h(p['DieStrtPos']), h(p['DieStopPos']), p['DieStrtNod'], p['DieStopNod'], p['DieMovDir'],
                p['AutoBite'], h(p['LeftManDis']), h(p['RightManDis']), h(p['HorizDieShift']), p['BitesToSkip'],
                h(p['RadialFeedPerBite']), h(p['DwellTime'])))
            heat(out, pre, p['HT'])
            for j, b in enumerate(p['bites']):
                bp = '%sb%d' % (pre, j)
                out.append('%s %d %d %d %s %s %s %s %d %s %s %d %s %s %s|%s' % (
                    bp, b['NSTEP'], b['STPINC'], b['PDIE'], h(b['SMAX']), h(b['LMAX']), h(b.get('ClampForce', 0)),
                    h(b['DTSUB']), b['StepMod'], h(b['DSMAX']), h(b['DTMAX']), b['FricTyp'], h(b['FRCFAC']),
                    h(b['IHTCOF']), h(b['NomBite']), b['UserKey']))
                heat(out, bp, b['HT'])
    if pd:
        out.append('pd %d %d %d %d %d %s %s %d %d %s %s %s %s %s %d %d %d %s %s %d %d' % (
            pd['Process_Type'], pd['Temperaure_Type'], pd['Use_Mandrel'], pd['Use_Adaptive_Reheat'],
            pd['Checking_Method'], h(pd['TR0']), h(pd['TR1']), pd['Solver_Type'], pd['Use_DifferentDies'],
            h(pd['Adaptive_Heating_Time']), h(pd['Swaging_Stroke']), h(pd['Dwell_Time']), h(pd['Cycle_Time']),
            h(pd['Decompression_Time']), pd['Bottom_Fixed'], pd['Heat_Transfer'], pd['Remaining'],
            h(pd['Clamp_Force']), h(pd['Clamp_Speed']), pd['RotationalSymmetry'], pd['Num_Steps_DSMAX']))
    return out

corpus, outp = sys.argv[1], output_path(sys.argv[2])
files = []
for root, dirs, names in os.walk(corpus):
    dirs.sort()
    for n in sorted(names):
        if n.lower() == 'cogging.mdt':
            files.append(os.path.relpath(os.path.join(root, n), corpus).replace(os.sep, '/'))
files.sort()
lines = []
for f in files:
    lines.append('FILE ' + f)
    lines += dump(mdt.read_mdt(os.path.join(corpus, f)))
open(outp, 'w', newline='\n').write('\n'.join(lines) + '\n')
print(len(files), 'files')
