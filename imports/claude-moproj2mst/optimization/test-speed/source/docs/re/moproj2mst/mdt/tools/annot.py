# usage: python -I annot.py <decompile.c>... ; appends the .rdata C string after every 0x14xxxxxxx
# / UNK_14xxxxxxx operand that points at printable text (MDT writer format strings).
import re, struct, sys
EXE = 'C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'
d = open(EXE, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]
ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<Q', d, pe + 24 + 24)[0]
secs = []
for i in range(ns):
    o = pe + 24 + osz + 40 * i
    vs, va, rs, rp = struct.unpack_from('<IIII', d, o + 8)
    secs.append((va, vs, rs, rp))


def cstr(v):
    r = v - base
    for va, vs, rs, rp in secs:
        if va <= r < va + rs:
            o = rp + r - va
            e = d.index(b'\0', o)
            s = d[o:e]
            if 0 < len(s) < 200 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
                return s.decode('latin1')
            return None
    return None


pat = re.compile(r'(?:UNK_|0x)(14[0-9a-f]{7})')
for fn in sys.argv[1:]:
    for line in open(fn, encoding='utf-8', errors='replace'):
        line = line.rstrip('\n')
        notes = []
        for m in pat.finditer(line):
            s = cstr(int(m.group(1), 16))
            if s is not None:
                notes.append('%s=%r' % (m.group(1)[-5:], s))
        print(line + ('   // ' + ' '.join(notes) if notes else ''))
