# Cogging.MDT reader emulating 14076a460 / 140761d90 (positional fscanf/fgets), Python prototype for s1-03 spec.
import struct, re

def f32(x):
    try: return struct.unpack('<f', struct.pack('<f', x))[0]
    except OverflowError: return float('inf') if x > 0 else float('-inf')  # open: MSVC %f overflow result

class Scan:
    """Minimal MSVC fscanf/fgets emulation on a byte string (text mode: CRLF already -> LF)."""
    def __init__(self, data):
        self.d = data.replace(b'\r\n', b'\n'); self.i = 0
    def ws(self):
        while self.i < len(self.d) and self.d[self.i] in b' \t\n\r\v\f': self.i += 1
    def tok(self):
        self.ws(); j = self.i
        while self.i < len(self.d) and self.d[self.i] not in b' \t\n\r\v\f': self.i += 1
        return self.d[j:self.i].decode('latin1')
    def num(self, pat):
        self.ws(); m = re.compile(pat).match(self.d, self.i)
        if not m: raise ValueError('scan fail at %d: %r' % (self.i, self.d[self.i:self.i+40]))
        self.i = m.end(); return m.group(0).decode()
    def i32(self):
        return int(self.num(rb'[+-]?\d+'))
    def dbl(self):
        return float(self.num(rb'[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?'))
    def fgets(self, n):
        j = self.i; k = j
        while k < len(self.d) and k - j < n - 1:
            k += 1
            if self.d[k-1:k] == b'\n': break
        self.i = k; return self.d[j:k].decode('latin1')
    def rest_of_line(self):  # %[^\n]
        self.ws(); j = self.i
        while self.i < len(self.d) and self.d[self.i:self.i+1] != b'\n': self.i += 1
        return self.d[j:self.i].decode('latin1')
    # format helpers ("%s %d\n" etc.; the trailing \n skips all whitespace)
    def sd(self):  self.tok(); v = self.i32(); self.ws(); return v
    def sf(self):  self.tok(); v = f32(self.dbl()); self.ws(); return v
    def slf(self): self.tok(); v = self.dbl(); self.ws(); return v
    def ss(self):  self.tok(); v = self.tok(); self.ws(); return v
    def skip(self, mode):
        n = 5 if mode in (2, 3) else (1 if mode == 4 else 3)
        for _ in range(n): self.fgets(0x1000)

def heat(s, ver):  # 140797ef0 -> 0x80 struct
    s.skip(0)
    h = {'Active': s.sd(), 'NSTEP': s.sd(), 'STPINC': s.sd(), 'SOLMTT': 1}
    h['TMAX'] = s.sf(); h['DTPMAX'] = s.sf()
    if ver < 0xce7: s.sf()
    else:
        s.tok(); flag = s.i32(); cnt = f32(s.dbl()); s.ws()
        if flag == 1:
            k = 0
            while float(k) < cnt: s.dbl(); s.dbl(); s.ws(); k += 1
    h['IHTCOF'] = s.slf()
    if ver > 0xce4: h['IHTCOF2'] = s.slf()
    s.tok(); s.ws(); h['UserKey'] = s.fgets(0x4f).rstrip('\n')
    return h

def pass_(s, ver):  # 140792810, offsets in comments
    p = {}
    n = s.sd(); p['nBites'] = n if n else 1                  # +4
    if ver < 0xc21: s.sf()
    p['RotAng'] = s.sf()                                      # +0x08
    if ver > 2999: p['BiteAngle'] = s.sf(); p['MinDieDis'] = s.sf()   # +0x10 +0x18
    if ver > 5999: p['DiePosMethod'] = s.sd()                 # +0xa8
    if ver > 3999: p['DieStrtPos'] = s.sf(); p['DieStopPos'] = s.sf()  # +0xb0 +0xb8
    if ver > 5999: p['DieStrtNod'] = s.sd(); p['DieStopNod'] = s.sd()  # +0xc0 +0xc4
    p['DieMovDir'] = s.sd() if ver > 0xfa1 else -1            # +0xc8
    p['AutoBite'] = s.sd() if ver > 0xc1b else 0              # +0x00
    p['LeftManDis'] = s.sf(); p['RightManDis'] = s.sf()      # +0xd0 +0xd8 (ver>=6001)
    p['HorizDieShift'] = s.sf() if ver > 0x277d else 0.0     # +0xe8
    if ver > 10999:
        p['BitesToSkip'] = s.sd(); p['RadialFeedPerBite'] = s.sf(); p['DwellTime'] = s.sf()  # +0xf0 +0xf8 +0x100
    p['HT'] = heat(s, ver)                                    # +0x28
    return p

def bite(s, ver):  # 140790ca0: HT at +0xc0, then 140790d80 at +0
    s.skip(0)
    b = {'HT': heat(s, ver)}
    s.skip(0)
    b['NSTEP'] = s.sd(); b['STPINC'] = s.sd(); b['PDIE'] = s.sd()
    b['SMAX'] = s.sf(); b['LMAX'] = s.sf()
    if ver > 4000: b['ClampForce'] = s.sf()
    b['DTSUB'] = s.sf(); b['StepMod'] = s.sd(); b['DSMAX'] = s.sf(); b['DTMAX'] = s.sf()
    b['FricTyp'] = s.sd(); b['FRCFAC'] = s.sf(); b['IHTCOF'] = s.slf()
    if ver > 0xce4: s.slf()
    s.tok(); s.ws(); uk = s.fgets(199).rstrip('\n'); b['UserKey'] = uk.split('/')[-1]
    b['NomBite'] = s.sf() if ver > 0xc20 else 0.0             # +0xb8
    return b

def read_mdt(path):
    s = Scan(open(path, 'rb').read())
    m = {}
    s.skip(2)
    s.fgets(0x1000); s.fgets(0x1000); l3 = s.fgets(0x1000)
    ver = int(re.match(r'\* MASTER DATA FILE (\d+)', l3).group(1)); s.fgets(0x1000); s.fgets(0x1000)
    m['ver'] = ver
    s.ss(); s.tok(); s.rest_of_line(); s.ws()
    s.skip(0); s.skip(2)
    for k in range(3): heat(s, ver)
    s.skip(2); pass_(s, ver)
    s.skip(2); bite(s, ver)
    # billet 140796f90
    s.skip(2); B = {}
    B['GeoTyp'] = s.sd()
    if ver > 0x27d7: s.sd(); s.tok(); s.ws(); s.fgets(0x3ff)
    B['Length'] = s.sf(); B['Dia'] = s.sf()
    if ver > 9999: B['InDia'] = s.sf()
    B['Height'] = s.sf()
    if ver > 0xce5: B['Width'] = s.sf()
    B['Radius'] = s.sf(); B['Layers'] = s.sf(); s.sd(); s.sf(); s.sd(); B['TMPBIL'] = s.sf()
    B['BccMod'] = s.sd(); B['MergeL'] = s.sd(); B['KeyBil'] = s.ss()
    if ver > 0xc1d: B['AxialSub'] = s.sf()
    if ver > 0xc1f: B['AxialType'] = s.sd()
    if ver > 0x13eb: B['DoBilStraight'] = s.sd()
    if ver > 0x27e1: s.sd()
    if ver > 0x27eb: s.sd()
    m['billet'] = B
    # die 140795760
    s.skip(2); D = {}
    D['GeoTyp'] = s.sd()
    if ver > 0x27d7: s.sd(); s.tok(); s.ws(); s.fgets(0x3ff)
    D['Length'] = s.sf(); s.sf(); s.sf(); s.sf(); s.sf(); s.sd(); s.sf(); s.sd()
    D['DieMov'] = s.sd(); D['TMPDIE'] = s.sf(); D['SPDDIE'] = s.sf()
    if ver > 0x277e: D['CYCLES'] = s.slf(); D['TOTSTROKE'] = s.slf(); s.slf()
    nd = s.sd() if ver >= 0xc80 else 2
    D['NoDies'] = nd; D['four'] = (nd == 4)
    s.ss(); s.ss()
    if nd == 4: s.ss(); s.ss()
    if ver > 2999: s.sd()
    if ver > 0xc1b: s.sf()
    if ver > 0xc1d: s.sf()
    if ver > 0xc1e: s.sd(); D['MovType'] = s.sd()
    if ver > 0xc1f: s.sf()
    if ver > 0x27e1: s.sd()
    if ver > 0x27eb: s.sd()
    m['die'] = D
    # manipulator 140793cc0
    M = {}
    if ver > 0xc1c:
        s.skip(2); s.sd()
        if ver > 0x27d7: s.sd(); s.tok(); s.ws(); s.fgets(0x3ff)
        M['Length'] = s.sf(); s.sf(); s.sf(); s.sf(); s.sf(); s.sd(); s.sf(); s.sd(); s.sf()
        s.ss(); s.ss(); s.ss(); s.ss()
        M['StartPos'] = s.sf()
        if ver >= 0x2774:
            M['LeftDis'] = s.sf(); M['RightDis'] = s.sf(); M['UseMstMan'] = int(s.sf()); M['UseSpring'] = s.sd()
            for _ in range(7): s.sf()
        if ver > 0x27e1: s.sd()
        if ver > 0x27eb: s.sd()
    m['manip'] = M
    # simulation parameters 14076aca0
    s.skip(2); m['UNIT'] = s.sd()
    for _ in range(4): s.sf()
    if ver > 0x13ec: s.sf(); s.sf()
    s.sf()
    m['UseManip'] = (s.sd() != 0) if ver > 0xc1c else True
    # simulation data
    s.skip(2)
    n = s.sd()
    for _ in range(n): s.fgets(100)
    nr = s.sd(); R = []
    for r in range(nr):
        s.skip(0); npass = s.sd()
        for k in range(3): heat(s, ver)
        P = []
        for i in range(npass):
            s.skip(0); p = pass_(s, ver)
            nb = p['nBites'] if p['AutoBite'] == 0 else 1
            p['bites'] = [bite(s, ver) for _ in range(nb)]
            P.append(p)
        R.append(P)
    m['reheats'] = R
    # process data 140761d90
    pd = {}
    if ver > 10999:
        s.skip(0)
        names = ['Process_Type', 'Temperaure_Type', 'Use_Mandrel', 'Use_Adaptive_Reheat', 'Checking_Method']
        for k in names: pd[k] = s.sd()
        s.tok(); pd['TR0'] = s.dbl(); pd['TR1'] = s.dbl(); s.ws()
        pd['Solver_Type'] = s.sd(); pd['Use_DifferentDies'] = s.sd()
        for k in ['Adaptive_Heating_Time', 'Swaging_Stroke', 'Dwell_Time', 'Cycle_Time', 'Decompression_Time']: pd[k] = s.slf()
        pd['Bottom_Fixed'] = s.sd(); pd['Heat_Transfer'] = s.sd(); pd['Remaining'] = s.sd()
        # %d into +0x505 overwrites the low byte of Cycle_Time (+0x508)
        b = bytearray(struct.pack('<d', pd['Cycle_Time'])); b[0] = (pd['Remaining'] >> 24) & 0xff
        pd['Cycle_Time'] = struct.unpack('<d', bytes(b))[0]
        pd['Clamp_Force'] = s.slf(); pd['Clamp_Speed'] = s.slf(); pd['RotationalSymmetry'] = s.sd(); pd['Num_Steps_DSMAX'] = s.sd()
    m['pd'] = pd
    return m
