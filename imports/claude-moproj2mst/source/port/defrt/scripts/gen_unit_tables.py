# Writes port/defrt/src/ftn/unit_tables.inc from DEF_PRE_64.exe (read only):
#   python -I port/defrt/scripts/gen_unit_tables.py port/defrt/src/ftn/unit_tables.inc
import struct, sys

exe = r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe"
b = open(exe, 'rb').read()
pe = struct.unpack_from('<I', b, 0x3c)[0]
ns = struct.unpack_from('<H', b, pe + 6)[0]
osz = struct.unpack_from('<H', b, pe + 20)[0]
base = struct.unpack_from('<Q', b, pe + 24 + 24)[0]
o = pe + 24 + osz
secs = [struct.unpack_from('<8sIIII', b, o + i * 40)[1:] for i in range(ns)]


def rd(addr, n):
    rva = addr - base
    for vs, va, rs, ra in secs:
        if va <= rva < va + rs:
            return b[ra + rva - va: ra + rva - va + n]
    raise ValueError(hex(addr))


BS = chr(92)


def cstr(bs):
    out = ''
    for ch in bs:
        if ch == 0:
            break
        c = chr(ch)
        if c == BS or c == '"':
            out += BS + c
        elif 32 <= ch < 127:
            out += c
        else:
            out += BS + 'x%02x' % ch
    return '"' + out + '"'


L = ["// Generated from DEF_PRE_64.exe .data by reading the bytes (gen_units.py); do not edit by hand.",
     "// @data 1406214a0  CONV_FAC source records (28 x 0x88): id, name[100], 4 doubles (bit patterns)",
     "inline constexpr ConvFacSrc kConvFacSrc[28] = {"]
for i in range(28):
    bb = rd(0x1406214a0 + i * 0x88, 0x88)
    assert bb[4:0x68].find(b'\0') >= 0
    q = struct.unpack('<4Q', bb[0x68:0x88])
    L.append("    {%d, %s, {0x%016xULL, 0x%016xULL, 0x%016xULL, 0x%016xULL}}," %
             (struct.unpack('<i', bb[:4])[0], cstr(bb[4:0x68]), *q))
L.append("};")
L.append("// @data 140622380  CONV_UNIT source records (28 x 0x1f8): id, five strings of 100 bytes")
L.append("inline constexpr ConvUnitSrc kConvUnitSrc[28] = {")
for i in range(28):
    bb = rd(0x140622380 + i * 0x1f8, 0x1f8)
    ss = [bb[x:x + 0x64] for x in (4, 0x68, 0xcc, 0x130, 0x194)]
    for s_ in ss:
        assert s_.find(b'\0') >= 0
    L.append("    {%d, {%s}}," % (struct.unpack('<i', bb[:4])[0], ", ".join(cstr(s_) for s_ in ss)))
L.append("};")
open(sys.argv[1], 'w', newline='\n').write("\n".join(L) + "\n")
print("ok")
