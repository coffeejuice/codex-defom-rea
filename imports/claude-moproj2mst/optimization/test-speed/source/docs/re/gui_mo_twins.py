"""Count DEF_GUI_MO_64 functions whose code matches a DEF_PRE_64 port-ledger function.

Static, byte level, on copies. A function's bytes are normalised by masking 32-bit fields
that are likely relocations (call/jmp/jcc rel32 and RIP-relative operands), then matched by
(size, masked bytes). It is a lower bound: functions the compiler laid out differently, or
that differ only in inlined callees, are not counted.

    python -I docs/re/gui_mo_twins.py <DEF_PRE_64.exe copy> <DEF_GUI_MO_64.exe copy> <pre64_batch_ledger.tsv>
"""
import collections
import struct
import sys

IB = 0x140000000
REL_LEAD = {0xE8, 0xE9, 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D}


def load(path):
    d = open(path, 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3c)[0]
    sh = pe + 24 + struct.unpack_from('<H', d, pe + 20)[0]
    secs = {}
    for i in range(struct.unpack_from('<H', d, pe + 6)[0]):
        o = sh + 40 * i
        vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8)
        secs[d[o:o + 8].rstrip(b'\0').decode()] = (va, vs, ra, rs)
    va, vs, ra, rs = secs['.pdata']
    funcs = {}
    for i in range(vs // 12):
        b, e, _ = struct.unpack_from('<III', d, ra + 12 * i)
        if b:
            funcs[b] = e - b
    tva, _, tra, _ = secs['.text']
    return d, funcs, tva, tra


def norm(code):
    b = bytearray(code)
    i = 1
    while i + 4 <= len(b):
        lead = b[i - 1]
        if lead in REL_LEAD or (0x80 <= lead <= 0x8F and i >= 2 and b[i - 2] == 0x0F):
            v = struct.unpack_from('<i', b, i)[0]
            if 0x40 <= abs(v) <= 0x6000000:
                b[i:i + 4] = b'\0\0\0\0'
                i += 4
                continue
        i += 1
    return bytes(b)


def main(pre, mo, ledger):
    pd, pf, ptva, ptra = load(pre)
    md, mf, mtva, mtra = load(mo)
    rows = [l.rstrip('\n').split('\t') for l in open(ledger, encoding='utf-8')][1:]
    want = {int(r[0], 16) - IB: (r[2], r[5]) for r in rows}
    index = collections.defaultdict(list)
    for b, s in mf.items():
        if b + IB < 0x141800000 and s >= 16:
            index[(s, norm(md[b - mtva + mtra:b - mtva + mtra + s]))].append(b)
    hit = collections.Counter()
    tot = collections.Counter()
    hit_bytes = 0
    big = big_tot = 0
    mo_hit = set()
    for b, (area, status) in want.items():
        s = pf.get(b)
        if not s or status == 'not-needed':
            continue
        tot[area] += 1
        big_tot += s >= 64
        key = (s, norm(pd[b - ptva + ptra:b - ptva + ptra + s]))
        if key in index:
            hit[area] += 1
            hit_bytes += s
            big += s >= 64
            mo_hit.update(index[key])
    for a in sorted(tot):
        print(f'{a:24s} {hit[a]:5d} / {tot[a]:5d}')
    print('total', sum(hit.values()), '/', sum(tot.values()), 'bytes', hit_bytes)
    print('of them >= 64 bytes', big, '/', big_tot)
    print('MO functions covered', len(mo_hit), 'bytes', sum(mf[b] for b in mo_hit))


if __name__ == '__main__':
    main(*sys.argv[1:4])
