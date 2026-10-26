"""mkmdt.py: make the Oracle-B cases of the MDT packet in gui_mo/oracle_b (copies of oracle-A corpus
cases; the originals are never opened). Each case keeps a copy of every Cogging.MDT as it was before
the GUI save in <case>/_before/<Task>/Cogging.MDT.

Edited cases change selected lines of one operation's MDT (same keys, other values) to show which
values the GUI writes back: Defaults HEAT slot 0 TMAX, POSERR, AVGSTR, CTLPCT, KeyDie1, and the
first pass's RotAng (model data)."""
import os, re, shutil, sys
C = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus"
O = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b"


def edit(path):
    b = open(path, 'rb').read()
    lines = b.split(b'\r\n')
    done = set()
    for i, l in enumerate(lines):
        k = l.split(b' ')[0]
        if k == b'TMAX' and 'tmax' not in done:
            lines[i] = b'TMAX     1.5000000E+000'; done.add('tmax')
        elif k == b'POSERR':
            lines[i] = b'POSERR     2.0000000E-003'; done.add('poserr')
        elif k == b'AVGSTR':
            lines[i] = b'AVGSTR     7.5000000E-001'; done.add('avgstr')
        elif k == b'CTLPCT':
            lines[i] = b'CTLPCT     2.5000000E+001'; done.add('ctlpct')
        elif k == b'KeyDie1':
            lines[i] = b'KeyDie1   FOO1.KEY'; done.add('keydie1')
        elif l.startswith(b'* Pass 1') and 'pass' not in done:
            done.add('pass')
            for j in range(i, i + 12):
                if lines[j].startswith(b'RotAng'):
                    lines[j] = b'RotAng         4.5000000E+001'; break
    assert len(done) == 6, done
    open(path, 'wb').write(b'\r\n'.join(lines))


def set_active(d, k):
    m = [f for f in os.listdir(d) if f.endswith('.moproj')][0]
    b = open(os.path.join(d, m), 'rb').read()
    assert b.count(b'Active="0"') == 1
    open(os.path.join(d, m), 'wb').write(b.replace(b'Active="0"', b'Active="%d"' % k))


def make(case, src, edit_task=None, active=None):
    d = os.path.join(O, case)
    if os.path.exists(d):
        sys.exit('exists: ' + d)
    shutil.copytree(os.path.join(C, src), d)
    for f in os.listdir(d):
        if f.upper().endswith('.MST'):
            os.rename(os.path.join(d, f), os.path.join(d, f + '.oracleA'))
    if active is not None:
        set_active(d, active)
    if edit_task:
        edit(os.path.join(d, 'Operations', edit_task, 'Cogging.MDT'))
    ops = os.path.join(d, 'Operations')
    for t in sorted(os.listdir(ops)):
        m = os.path.join(ops, t, 'Cogging.MDT')
        if os.path.exists(m):
            os.makedirs(os.path.join(d, '_before', t))
            shutil.copy2(m, os.path.join(d, '_before', t, 'Cogging.MDT'))
    print(case)


CASES = [
    ('MDT1_COG01', '01_COG_P1_B1', None),
    ('MDT2_LAB2', '54_Cogging_Multiple_Ops_Lab2', None),
    ('MDT3_LAB2_E1', '54_Cogging_Multiple_Ops_Lab2', 'Task00001'),
    ('MDT4_LAB2_E3', '54_Cogging_Multiple_Ops_Lab2', 'Task00003'),
    ('MDT5_CHAMF1', '07_CHAMF1', None),
    ('MDT6_VDIE', '12_VDIE', None),
    ('MDT7_BRICK', '39_brick_mesh_01', None),
    ('MDT8_VDIE42', '42_vdie', None),
    ('MDTA2_LAB2', '54_Cogging_Multiple_Ops_Lab2', None, 2),
    ('MDTA3_LAB2_E1', '54_Cogging_Multiple_Ops_Lab2', 'Task00001', 2),
    ('MDTA4_LAB2_E3', '54_Cogging_Multiple_Ops_Lab2', 'Task00003', 2),
    ('MDTC_02', '02_COG_P1_B10', None),
    ('MDTC_03', '03_COG_P1_B10_NEGATIVE_FEED', None),
    ('MDTC_04', '04_COG_P1_B1_LEFT_MASTER', None),
    ('MDTC_05', '05_COG_P1_B1_MINUS_X', None),
    ('MDTC_25', '25_Inc718_D420_GFM_V_DIES_Angle2_18', None),
    ('MDTC_26', '26_Inc718_D420_GFM_V_DIES_Angle2_19', None),
    ('MDTC_27', '27_COG_P1_B1', None),
    ('MDTC_28', '28_Inc718_D420_GFM_V_DIES_Angle2_18', None),
    ('MDTC_41', '41_NewProject_1', None),
    ('MDTC_43', '43_vdie2', None),
    ('MDTC_44', '44_vdie3', None),
    ('MDTC_45', '45_vdie_1', None),
    ('MDTC_53', '53_Cogging_Multiple_Ops_Lab1', None),
]
if __name__ == '__main__':
    want = sys.argv[1:]
    for c in CASES:
        if not want or c[0] in want:
            make(*c)
