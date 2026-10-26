"""Harmless synthetic-memory model of CP213's bounded Qt4 snapshot contract.

This executes no original code and is not the generated Frida reader test.
"""
import json
import struct

MAX_ADDR = (1 << 64) - 1


class Memory:
    def __init__(self):
        self.blocks = {}
        self.reads = 0
        self.mutate = None

    def read(self, addr, count):
        self.reads += 1
        if self.mutate:
            self.mutate(self, addr, count)
        if addr <= 0 or count < 0 or addr > MAX_ADDR - count:
            raise ValueError('address-range')
        for start, data in self.blocks.items():
            if start <= addr and addr + count <= start + len(data):
                return bytes(data[addr-start:addr-start+count])
        raise ValueError('unreadable')


def snapshot(mem, obj):
    out = dict(status='unreadable', units=None, text=None, truncated=False)
    stage = 'object'
    try:
        slot = mem.read(obj, 8)
        data, = struct.unpack('<Q', slot)
        out['header'] = hex(data)
        stage = 'header'
        head = mem.read(data, 26)
        ref, alloc, size = struct.unpack_from('<iii', head)
        chars, = struct.unpack_from('<Q', head, 16)
        out.update(ref=ref, alloc=alloc, size=size, chars=hex(chars))
        # These are conservative trial policy gates, not universal Qt validity.
        if ref <= 0:
            out['status'] = 'ref-policy'; return out
        if size < 0 or alloc < 0 or alloc < size:
            out['status'] = 'invalid-size-capacity'; return out
        if size > 128:
            out.update(status='length-cap', truncated=True); return out
        if chars <= 0 or chars % 2 or chars > MAX_ADDR - 2*size:
            out['status'] = 'invalid-data-range'; return out
        stage = 'content'
        content = mem.read(chars, 2*size) if size else b''
        stage = 'reread'
        # Header includes ref, capacity, length, pointer, flags and padding.
        # Comparing padding conservatively rejects any observed byte change.
        if mem.read(obj, 8) != slot or mem.read(data, 26) != head:
            out['status'] = 'changed'; return out
        out.update(status='ok', units=list(struct.unpack('<'+'H'*size, content)),
                   text=content.decode('utf-16-le', errors='surrogatepass'))
        return out
    except ValueError as exc:
        out.update(status=str(exc), failureStage=stage)
        return out


def fixture(units, *, external=False, size=None, alloc=None, ref=1, chars=None):
    m = Memory(); data = 0x2000; obj = 0x1000
    content = struct.pack('<'+'H'*len(units), *units)
    actual_chars = 0x3000 if external else data+26
    head = bytearray(26)
    struct.pack_into('<iii', head, 0, ref,
                     len(units) if alloc is None else alloc,
                     len(units) if size is None else size)
    struct.pack_into('<Q', head, 16, actual_chars if chars is None else chars)
    m.blocks[obj] = bytearray(struct.pack('<Q', data))
    m.blocks[data] = head + (content+b'\0\0' if not external else b'')
    if external:
        m.blocks[actual_chars] = bytearray(content)  # No NUL promise.
    return m, obj


def main():
    path = r'C:\PROBLEM\p213a\Gear_blank.moproj'
    rows = []
    def check(name, m, obj, expected, units=None):
        result = snapshot(m, obj)
        assert result['status'] == expected, (name, result)
        if units is not None:
            assert result['units'] == units, (name, result)
        rows.append(dict(name=name, status=result['status']))
    path_units = list(struct.unpack('<'+'H'*len(path), path.encode('utf-16-le')))
    assert len(path_units) == 34
    check('exact-path', *fixture(path_units), 'ok', path_units)
    check('embedded-zero-surrogates', *fixture([65,0,0xd83d,0xde00,0xd800]), 'ok', [65,0,0xd83d,0xde00,0xd800])
    check('empty-positive-owner', *fixture([]), 'ok', [])
    check('external-without-terminator', *fixture([65,66], external=True), 'ok', [65,66])
    check('negative-length', *fixture([], size=-1), 'invalid-size-capacity')
    check('capacity-below-length', *fixture([65], alloc=0), 'invalid-size-capacity')
    check('length-129', *fixture([65]*129), 'length-cap')
    check('ref-zero', *fixture([], ref=0), 'ref-policy')
    check('ref-negative', *fixture([], ref=-1), 'ref-policy')
    check('null-data', *fixture([65], chars=0), 'invalid-data-range')
    check('odd-data', *fixture([65], chars=0x3001), 'invalid-data-range')
    check('overflow-data', *fixture([65], chars=MAX_ADDR-1), 'invalid-data-range')
    check('unreadable-content', *fixture([65], chars=0x4000), 'unreadable')
    m, obj = fixture([65], external=True); m.blocks[0x3000] = bytearray(b'A')
    check('partial-content', m, obj, 'unreadable')
    check('null-object', Memory(), 0, 'address-range')
    m, obj = fixture([65])
    def mutate_header(mem, addr, count):
        if mem.reads == 5:
            struct.pack_into('<i', mem.blocks[0x2000], 8, 0)
    m.mutate = mutate_header
    check('header-changed', m, obj, 'changed')
    m, obj = fixture([65])
    def mutate_slot(mem, addr, count):
        if mem.reads == 4:
            struct.pack_into('<Q', mem.blocks[obj], 0, 0x4000)
    m.mutate = mutate_slot
    check('object-changed', m, obj, 'changed')
    print(json.dumps(dict(passed=len(rows), cases=rows), indent=2))


if __name__ == '__main__':
    main()
