"""Static size survey of DEF_GUI_MO_64.exe for docs/plan-gui-mo-reconstruction.md.

Reads a COPY of the exe (never the install). No disassembler needed: functions come
from .pdata, classes from RTTI complete-object locators, subsystems from address
locality of vtable slots (MSVC keeps an object file's functions together).

    python -I docs/re/gui_mo_survey.py <copy of DEF_GUI_MO_64.exe>
"""
import collections
import re
import struct
import sys

IB = 0x140000000
APP_END = 0x141800000   # above this: SEH unwind funclets, then static initialisers

GROUPS = [
    ('licence', r'^(CLManager|Csdspm|CDFM|SfLicense|CDEFORMLM)'),
    ('qtlib', r'^(\?\$)?Qt[A-Z]|^Q[A-Z]|FormBuilder|^\?\$Q'),
    ('doe', r'Doe'),
    ('shell', r'^(SfMo|SfMain|SfDiag|ISfDiag|SfMoving|SfUndo|SfProject|SfAbout|SfPreferences|'
              r'SfSession|SfAction|SfTree|SfWidget|SfButton)'),
    ('operations', r'^SfOpr'),
    ('display', r'^(SfDisp|OGLData|SfScene|LayerView|SfPicking|Sf2[Dd]|Sf3D|SfColor|SfAnimation|'
                r'SfAnnotation|Manipu|SfMeasure|SfText|SfLine|SfShape)'),
    ('post', r'^(SfTask|SfSv|SfFN|SfPtrk|SfGraph|SfChart|SfReport|SfCoupon|SfSlice|SfUsr|'
             r'SfStateVar|SfSummary|SfStep|SfLoad|SfPoint|SfFunction)'),
    ('pre', r'^(SfPre|SfWindef|SfMat|SfMaterial|SfFlowstress|SfMov|SfHT|SfDie|SfGeo|SfMesh|'
            r'SfObject|SfBoolean|SfLubricant|SfMedium|SfTemplate|SfMirroring|SfPart|SfMulti|'
            r'SfUnits|SfConv|SfCogging|SfHeat|SfForce|SfPress|SfInterp|SfEmbed|SfSelect|'
            r'SfSymmetry|SfSeparator|SfSC|SfTool|SfItem|SfState)'),
    ('sql', r'^Def(Sql|Sq)'),
    ('engine', r'^(_|[A-Z0-9_]+$|LL|Dds|Uic2C|Sdctl|\?\$Sdctl|StateVar|OBJ_|GEO2D|DYNARY|OPSTOP|'
               r'FRICTION|Sf[A-Z]?Linked|DefOpt)'),
    ('stl', r'^\?\$|^(bad_|out_of|length_err|logic_err|runtime_err|exception|ios_base|type_info)'),
]


def group(name):
    for g, p in GROUPS:
        if re.search(p, name):
            return g
    return 'other'


def main(path):
    d = open(path, 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3c)[0]
    sh = pe + 24 + struct.unpack_from('<H', d, pe + 20)[0]
    secs = []
    for i in range(struct.unpack_from('<H', d, pe + 6)[0]):
        o = sh + 40 * i
        vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8)
        secs.append((d[o:o + 8].rstrip(b'\0').decode(), va, vs, ra, rs))
    sec = {s[0]: s for s in secs}

    def o2r(o):
        for _, va, _, ra, rs in secs:
            if ra <= o < ra + rs:
                return o - ra + va

    _, _, pvs, pra, _ = sec['.pdata']
    funcs = sorted((b, e - b) for b, e, _ in
                   (struct.unpack_from('<III', d, pra + 12 * i) for i in range(pvs // 12)) if b)
    app = [f for f in funcs if f[0] + IB < APP_END]
    print('functions', len(funcs), 'app range', len(app), 'app bytes', sum(s for _, s in app))

    td = {}
    for m in re.finditer(rb'\.\?A[VU][A-Za-z0-9_@?$]+@@', d):
        r = o2r(m.start() - 16)
        if r:
            td[r] = m.group(0).decode()[4:-2]
    print('RTTI types', len(set(td.values())))

    _, _, _, rra, rrs = sec['.rdata']
    cols = {}
    for off in range(rra, rra + rrs - 24, 4):
        if struct.unpack_from('<I', d, off)[0] != 1:
            continue
        _, _, _, ptd, _, pself = struct.unpack_from('<IIIIII', d, off)
        if pself == o2r(off) and ptd in td:
            cols[IB + pself] = td[ptd]
    label = {}
    rdb = d[rra:rra + rrs]
    for i in range(0, len(rdb) - 8, 8):
        name = cols.get(struct.unpack_from('<Q', rdb, i)[0])
        if not name:
            continue
        vt = rra + i + 8
        while True:
            f = struct.unpack_from('<Q', d, vt)[0] - IB
            if not 0x1000 <= f < APP_END - IB:
                break
            label.setdefault(f, name)
            vt += 8
    print('vtable slots', len(label))

    cur, gap = 'start', 0
    cnt, byt = collections.Counter(), collections.Counter()
    for b, s in app:
        if b in label:
            cur, gap = group(label[b]), 0
        else:
            gap += 1
        g = cur if gap < 400 else 'non-class'
        cnt[g] += 1
        byt[g] += s
    for g, n in cnt.most_common():
        print(f'{g:12s} {n:6d} {byt[g] // 1024:7d} KB')

    names = set(m.group(0).decode('utf-16-be') for m in re.finditer(
        rb'(?:\x00[A-Za-z0-9_\-]){2,60}\x00\.\x00(?:p\x00n\x00g|u\x00i|x\x00p\x00m|i\x00c\x00o|g\x00i\x00f)', d))
    print('qrc resources', len(names),
          collections.Counter(n.rsplit('.', 1)[1] for n in names))


if __name__ == '__main__':
    main(sys.argv[1])
