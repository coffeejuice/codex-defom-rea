# Prototype of the SfOprCogging MST writer 1407520c0/140752190 (cogging-specific lines only;
# shared-emitter output is marked as GEN:<addr> and skipped in comparisons).
import struct, re

SEP = '*' + '-' * 73 + '*'

def g(x):
    """QTextStream << double (precision 6, SmartNotation); -0 prints as 0 (evidence: COG_P1_B1)."""
    if x == 0: return '0'
    s = '%g' % x
    return s

DIRS = [(0.0, 0.0, -1.0), (0.0, 0.0, 1.0), (0.0, -1.0, 0.0), (0.0, 1.0, 0.0)]   # 140766010
DIE_IDS = [2, 3, 8, 9]                                                          # 140765f30
MAN_IDS = [4, 5, 6, 7]                                                          # 140765fa0

class Cog:
    def __init__(self, m):
        self.m = m; self.pd = m['pd']; self.R = m['reheats']
    def npass(self, r): return len(self.R[r-1])
    def pas(self, r, p): return self.R[r-1][p-1]
    def bite(self, r, p, b):       # 140767bc0
        P = self.pas(r, p)
        if P['AutoBite'] != 0 and b > 1: b = 1
        return P['bites'][b-1]
    def ndies(self): return 4 if self.m['die']['four'] else 2           # 140765b00
    def bdim(self, a, b):          # 140768650 (billet part)
        B = self.m['billet']
        if a == 1:
            if b == 0: return B['Length']
            gt = B['GeoTyp']
            if gt == 0: return B['Dia']
            if gt == 1: return B['Width']
            if gt == 2: return B['Height']
            return B['Width']
        raise NotImplementedError
    def nauto(self, r, p):         # 140768770
        P = self.pas(r, p); L = self.m['billet']['Length']
        if P['DiePosMethod'] == 1: L = abs(P['DieStrtPos'] - P['DieStopPos'])
        elif P['DiePosMethod'] == 0: L = ((1.0 - P['DieStrtPos']) - P['DieStopPos']) * L
        else: L = (L - P['DieStrtPos']) - P['DieStopPos']
        nom = P['bites'][0]['NomBite']
        if nom <= 0.0: return 1
        x = L / nom; n = int(x)
        if n == 0: n = 1
        return n + 1 if x - n < 0.5 else n + 2
    def tx(self, r, p, b):         # 1407682a0 getTranslationX
        P = self.pas(r, p); d = P['DieMovDir']; L = self.bdim(1, 0)
        if P['DiePosMethod'] == 1:
            x = (P['DieStrtPos'] if d == self.pas(r, 1)['DieMovDir'] else P['DieStopPos']) + 0.0
        elif P['DiePosMethod'] == 0: x = float(d) * P['DieStrtPos'] * L + 0.0
        else: x = float(d) * P['DieStrtPos'] + 0.0
        if P['AutoBite'] == 0:
            for i in range(b - 1): x = x + float(d) * P['bites'][i]['NomBite']
            if self.m['UseManip'] and d == 1 and P['DiePosMethod'] != 1: x = x + -1.0 * L
        elif d == -1: x = x + float(-(b - 1)) * P['bites'][0]['NomBite']
        elif P['DiePosMethod'] == 1: x = x + float(b - 1) * P['bites'][0]['NomBite']
        else: x = x + -1.0 * L + float(b - 1) * P['bites'][0]['NomBite']
        return x

def write(m, sim0, dbname='X.DB', firstop=False):
    c = Cog(m); pd = c.pd; o = []; st = {'sim': sim0, 'first': True, 'state': 0, 'mf': True}
    M = m['manip']; man = m['UseManip']; nd = c.ndies()
    def hdr(full_ok=True):
        o.append('GEN:140131c10 SIMULATION %d' % st['sim'])
    def head_first():
        if not st['first']:
            o.append('GEN:1401343e0'); o.append('GEN:140134bf0')
        else:
            st['first'] = False
            if not firstop: o.append('GEN:1401343e0')
            o.extend(['GEN:140134bf0', 'GEN:140134db0', 'GEN:1401360e0', 'GEN:140135220', 'GEN:140135da0'])
    def man13(v):
        o.append('OBJPOS\t4\t13\t0\t-1\t0\t0\t1\t2\t%s\t1' % g(v)); o.append('5')
        o.append('OBJPOS\t6\t13\t0\t1\t0\t0\t1\t2\t%s\t1' % g(v)); o.append('7')
    def retract(filt):
        half = -c.bdim(1, 2) / 2.0
        for i in range(nd):
            if filt(DIE_IDS[i]):
                dx, dy, dz = DIRS[i]
                o.append('OBJPOS \t%d\t11\t%s\t%s\t%s' % (DIE_IDS[i], g(half * dx), g(half * dy), g(half * dz)))
    def steps(h, dt, dsmax_first=False):
        o.append('NSTEP\t%d' % h['NSTEP']); o.append('STPINC\t%d' % h['STPINC']); o.append('SOLMTT\t%d' % h['SOLMTT'])
        if dsmax_first: o.append('DSMAX \t0'); o.append('DTMAX \t%s' % g(dt / h['NSTEP']))
        else: o.append('DTMAX \t%s' % g(dt / h['NSTEP'])); o.append('DSMAX \t0')
    def tail(name):
        o.append('GEN:140134680'); o.append('SIMNAM'); o.append(name); o.append('GEN:OPRNAM/CURSIM/GENDB')
    TRANS = 'TRANS \t1\t0\t-1\t-1\t-1\t-1'
    cyc_src = lambda: (1.0 / m['die']['CYCLES']) if m['die'].get('MovType') == 4 else pd['Cycle_Time']
    tt = pd['Temperaure_Type'] != 0; ht = (pd['Heat_Transfer'] & 0xff) != 0
    dwell = pd['Dwell_Time']; dec = pd['Decompression_Time']
    bDec = bDw = bBht = False
    if ht and tt:
        bDec = dec > 1e-6; bDw = dwell > 1e-6
        if (pd['Remaining'] & 0xff) == 0: bBht = True
        else: bDw = True; dwell = 0.0
    adapt = (pd['Use_Adaptive_Reheat'] & 0xff) != 0
    for r in range(1, len(c.R) + 1):
        for p in range(1, c.npass(r) + 1):
            st['mf'] = True
            if p > 1: st['first'] = False
            P = c.pas(r, p)
            if P['HT']['TMAX'] > 1e-6 and tt:      # PHT 1407532b0
                st['sim'] += 1; hdr(); head_first()
                if st['mf']:
                    st['mf'] = False
                    if man:
                        v = M['StartPos'] + M['Length'] / 2.0; man13(v)
                        o += ['OBJPOS\t4\t14\t5\t0\t0.001\t1', 'OBJPOS\t6\t14\t7\t0\t0.001\t1',
                              'OBJPOS\t4\t14\t5\t1\t0.001\t1', 'OBJPOS\t6\t14\t7\t1\t0.001\t1']
                if st['state'] != 3: retract(lambda i, s=st['state']: i == 3 or s != 2)
                steps(P['HT'], P['HT']['TMAX']); o.append('GEN:140136040'); o.append(TRANS)
                tail('RH%d-PS%d-PHT' % (r, p)); st['state'] = 3
            if P['DwellTime'] > 1e-6 and tt:        # PDWL 140754250
                st['sim'] += 1; hdr(); head_first()
                if st['mf']:
                    st['mf'] = False
                    if man:
                        v = M['StartPos'] + M['Length'] / 2.0; man13(v)
                        o += ['OBJPOS\t4\t14\t5\t0\t0.001\t1', 'OBJPOS\t6\t14\t7\t0\t0.001\t1',
                              'OBJPOS\t4\t14\t5\t1\t0.001\t1', 'OBJPOS\t6\t14\t7\t1\t0.001\t1']
                if st['state'] != 3: retract(lambda i: True)
                o.append('OBJPOS\t3\t12\t1e-05\t0\t0\t1\t1\t0\t0')
                steps(P['HT'], P['DwellTime']); o.append('GEN:140136040'); o.append(TRANS)
                tail('RH%d-PS%d-PDWL' % (r, p)); st['state'] = 2
            auto = P['AutoBite'] != 0
            if not auto: nb = P['nBites']; t24 = 0
            else:
                nb = c.nauto(r, p) * 2 - 1
                t24 = st['sim'] + 1 + nb * (int(bBht) + 1 + int(bDec) + int(bDw))
            for b in range(1, nb + 1):
                if b > 1: st['first'] = False
                B = c.bite(r, p, b)
                st['sim'] += 1; bdef(c, m, o, st, r, p, b, firstop, hdr, head_first, man13, retract, dbname)
                st['state'] = 1
                if ht:
                    if bDec:
                        st['sim'] += 1; hdr(); o.append('GEN:1401343e0'); o.append('GEN:140134bf0')
                        steps(B['HT'], dec); o.append(TRANS); tail('RH%d-PS%d-BDCT%d' % (r, p, b))
                    if bDw:
                        st['sim'] += 1; hdr(); o.append('GEN:1401343e0'); o.append('GEN:140134bf0')
                        retract(lambda i: i != 3)
                        steps(B['HT'], dwell, dsmax_first=True)
                        if dwell == 0.0:
                            cyc = cyc_src()
                            if dec + dwell < cyc:
                                o.append('TCYCLE\t%d\t%d\t%s\t%s\t%s' % (B['HT']['NSTEP'], P['BitesToSkip'], g(cyc), g(dec), g(dwell)))
                        o.append('GEN:140136040'); o.append(TRANS); tail('RH%d-PS%d-BDW%d' % (r, p, b)); st['state'] = 2
                    if bBht:
                        st['sim'] += 1; hdr(); o.append('GEN:1401343e0'); o.append('GEN:140134bf0')
                        bf = pd['Bottom_Fixed']; s0 = st['state']
                        retract(lambda i: (i != 3 or bf != 2) and (i == 3 or s0 != 2))
                        steps(B['HT'], B['HT']['TMAX'])
                        cyc = cyc_src()
                        if dec + pd['Dwell_Time'] < cyc:
                            o.append('TCYCLE\t%d\t%d\t%s\t%s\t%s' % (B['HT']['NSTEP'], P['BitesToSkip'], g(cyc), g(dec), g(pd['Dwell_Time'])))
                        o.append('GEN:140136040'); o.append(TRANS)
                        tail('RH%d-PS%d-BHT%d' % (r, p, b) + ('(Auto)' if auto else '')); st['state'] = 3
                if auto and b > 1:
                    o += [SEP, 'CHKSTOP', '%d' % (int(adapt) + 1), '1', '0\t%d' % t24,
                          '2\t3\t%s\t%d' % (g(P['MinDieDis']), pd['RotationalSymmetry'] & 0xff),
                          '%d\t0\t0' % P['DieMovDir'], '0.5', g(B['NomBite']), '%d' % P['DiePosMethod'],
                          ('%d' % P['DieStopNod']) if P['DiePosMethod'] == 1 else g(P['DieStopPos'])]
                    if adapt:
                        o += ['2', '%d\t%d' % (t24 + 1, t24), '0', g(pd['TR0']), '%d' % pd['Checking_Method']]
                    o.append(SEP)
            if adapt:
                st['sim'] += 1
                if not auto:
                    o += [SEP, 'CHKSTOP', '1', '2', '0\t%d' % (st['sim'] + 1), '1', g(pd['TR0']), '%d' % pd['Checking_Method'], SEP]
                hdr(); o.append('GEN:1401343e0'); o.append('GEN:140134bf0')
                retract(lambda i: True)
                h = P['HT']
                o += ['NSTEP\t%d' % h['NSTEP'], 'STPINC\t%d' % h['STPINC'], 'SOLMTT\t%d' % h['SOLMTT'], 'DSMAX \t0',
                      'ENVTMP\t0\t%s' % g(pd['TR1']), 'DTMAX \t%s' % g(pd['Adaptive_Heating_Time'] / h['NSTEP'])]
                o.append('GEN:140136040'); o.append(TRANS); tail('RH%d-PS%d-SHT' % (r, p))
    return o, st['sim']

def bdef(c, m, o, st, r, p, b, firstop, hdr, head_first, man13, retract, dbname):
    P = c.pas(r, p); B = c.bite(r, p, b); pd = c.pd; Bl = m['billet']; D = m['die']; M = m['manip']
    nd = c.ndies(); man = m['UseManip']
    hdr(); head_first()
    dbs = Bl.get('DoBilStraight', 0)
    if (dbs == 1 and (b != 1 or p != 1)) or (dbs == 2 and b == 1 and p > 1):
        o += ['EXECMD\t3', 'COG_GEO_MANIP.EXE\t' + dbname, 'KFREAD\t1', 'COG_STRAIGHT_BILLET.KEY']
    if Bl.get('AxialSub', 0.0) != 0.0 and b > 1:
        v = Bl['AxialSub'] if Bl.get('AxialType', 0) == 0 else -Bl['AxialSub']
        o += ['GENSUB\t1\t%s' % g(v), 'EXECMD\t3', 'DEF_BRK.EXE', 'KFREAD\t1', 'NEWBRICK.KEY']
    tx = c.tx(r, p, b); c1c0 = False
    def rot(a):
        s = 'OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t%s' % g(a)
        if man: o.append(s + '\t4'); o.append(''.join('%d\t' % i for i in MAN_IDS))
        else: o.append(s)
    if b == 1:
        if P['DiePosMethod'] != 3 and (p > 1 or not firstop): c1c0 = True
        if P['RotAng'] != 0.0: rot(P['RotAng'])
    else:
        tx = tx - c.tx(r, p, b - 1)
    if not c1c0:
        s = 'OBJPOS\t2\t1\t%s\t0\t0\t0\t0\t0\t0' % g(tx)
    else:
        s = 'OBJPOS\t2\t13\t0\t%d\t0\t0\t1\t%d\t' % (P['DieMovDir'], P['DiePosMethod'])
        if P['DiePosMethod'] == 1: s += '%d' % int(P['DieMovDir'] != c.pas(r, 1)['DieMovDir'])
        else: s += g(P['DieStrtPos'])
    o.append(s + '\t%d' % (nd - 1)); o.append(''.join('%d\t' % DIE_IDS[i] for i in range(1, nd)))
    if P['BiteAngle'] != 0.0 and b > 1: rot(P['BiteAngle'])
    d1b0 = P['RadialFeedPerBite']
    if b == 1:
        if p > 1:
            prev = c.pas(r, p - 1); d1b0 = (prev['MinDieDis'] - P['MinDieDis']) / 2.0
            for i in range(nd): o.append('DSTROKE\t%d\t%s' % (DIE_IDS[i], g(d1b0)))
        h = P['HorizDieShift']
        if h != 0.0:
            if h > 1e-6: k = (0, 1); ks = (8, 9)
            elif h != 1e-6: k = (2, 3); ks = (2, 3)
            else: k = ()
            if k:
                for i in k:
                    dx, dy, dz = DIRS[i]
                    o.append('OBJPOS \t%d\t11\t%s\t%s\t%s' % (DIE_IDS[i], g(h * dx), g(h * dy), g(h * dz)))
                for j in ks: o.append('DSTROKE\t%d\t%s' % (j, g(-h)))
    else:
        if pd['Process_Type'] == 1 and d1b0 > 0.0:
            for i in range(nd): o.append('DSTROKE\t%d\t%s' % (DIE_IDS[i], g(d1b0)))
    s0 = st['state']
    retract(lambda i: i == 3 or s0 != 2)
    o.append('OBJPOS\t2\t12\t1e-05\t0\t0\t0\t1\t0\t0\t%d' % (nd - 1)); o.append(''.join('%d\t' % DIE_IDS[i] for i in range(1, nd)))
    if pd['Process_Type'] == 0:
        for i in range(nd):
            o.append('FORCE\t%d\t0\t0\t0' % DIE_IDS[i])
            if D.get('MovType') == 1: o.append('STROKE \t%d\t0\t0\t0' % DIE_IDS[i])
    if man:
        if st['mf']:
            st['mf'] = False; man13(M['StartPos'] + M['Length'] / 2.0)
        o += ['OBJPOS\t4\t14\t5\t0\t1e-05\t1', 'OBJPOS\t6\t14\t7\t0\t1e-05\t1', 'GENAXS\t7\t%s\t%s' % (g(M['LeftDis']), g(M['RightDis']))]
    o += ['URZ\t1\t1 0.0 0.0 0.0', '1\t 0.0 0.0 0.0']
    if man and M.get('UseSpring', 0) != 0:
        for i in MAN_IDS: o.append('STROKE \t%d\t0.0 0.0 0.0' % i)
    o.append('GEN:140136040')
    o.append('MDSOBJ\t2\t3\t3\t%s' % g(P['MinDieDis']))
    if D.get('MovType') == 4: o.append('SMAX\t0\t0\t%s' % g(D['TOTSTROKE']))
    o.append('GEN:140134680'); o.append('SIMNAM'); o.append('RH%d-PS%d-BDEF%d' % (r, p, b) + ('(Auto)' if P['AutoBite'] != 0 else ''))
    o.append('GEN:OPRNAM/CURSIM/GENDB')
