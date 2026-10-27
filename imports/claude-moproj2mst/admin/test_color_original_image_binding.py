"""CP285 tests use copied evidence and synthetic data only; no child process."""
import copy
import struct
import unittest
import color_original_image_binding as m


class OriginalBindingTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.blob = m.ORIGINAL.read_bytes()
        cls.pe = m.parse_pe(cls.blob)

    def sample(self, base=0x7ff700000000):
        header = bytearray(self.blob[:m.HEADERS])
        struct.pack_into('<Q', header, m.IMAGE_BASE_OFFSET, base)
        return dict(base=base, header=bytes(header), call=m.CALL, prologue=m.PROLOGUE,
                    runtime_function=struct.pack('<III', m.ENTRY_RVA, m.FUNCTION_END, m.UNWIND_RVA),
                    unwind_header=m.UNWIND_HEADER, caller_return=base + m.RETURN_RVA)

    def test_pinned_original_and_full_relocation_audit(self):
        result = m.validate_file_binding(self.blob)
        self.assertEqual(result['relocation_entries_checked'], 300898)
        self.assertEqual(result['full_prologue_bytes_checked'], 31)
        self.assertFalse(result['original_run_ready'])

    def test_size_and_hash_mismatch(self):
        for blob in (b'', self.blob[:-1], self.blob + b'x'):
            with self.subTest(size=len(blob)), self.assertRaises(m.BindingError):
                m.validate_file_binding(blob)
        blob = bytearray(self.blob); blob[0x11f1ae0 + 25] ^= 1
        with self.assertRaises(m.BindingError):
            m.validate_file_binding(bytes(blob))

    def test_synthetic_mapped_header_rewrite(self):
        for base in (m.BASE, 0x7ff700000000):
            result = m.validate_mapped_sample(self.blob, **self.sample(base))
            self.assertFalse(result['original_run_ready'])
        sample = self.sample(); h = bytearray(sample['header']); h[20] ^= 1
        sample['header'] = bytes(h)
        with self.assertRaises(m.BindingError):
            m.validate_mapped_sample(self.blob, **sample)

    def test_mapped_mismatch_and_exact_lengths(self):
        fields = ('header', 'call', 'prologue', 'runtime_function', 'unwind_header')
        for field in fields:
            for transform in (lambda b: b[:-1], lambda b: b + b'\0',
                              lambda b: b[:-1] + bytes([b[-1] ^ 1])):
                sample = self.sample(); sample[field] = transform(sample[field])
                with self.subTest(field=field), self.assertRaises(m.BindingError):
                    m.validate_mapped_sample(self.blob, **sample)
        for i in (20, 21, 30):
            sample = self.sample(); p = bytearray(sample['prologue']); p[i] ^= 1
            sample['prologue'] = bytes(p)
            with self.subTest(index=i), self.assertRaises(m.BindingError):
                m.validate_mapped_sample(self.blob, **sample)
        sample = self.sample(); sample['caller_return'] += 1
        with self.assertRaises(m.BindingError):
            m.validate_mapped_sample(self.blob, **sample)

    def test_arithmetic_and_bounds(self):
        for base in (0, -0x10000, True, 1, 0xffffffffffff0000, 0x800000000000):
            with self.subTest(base=base), self.assertRaises(m.BindingError):
                m.address(base, m.ENTRY_RVA, 31)
        for rva, size in ((-1, 1), (m.IMAGE_SIZE, 1), (m.IMAGE_SIZE - 1, 2), (0, 0), (0, -1)):
            with self.subTest(rva=rva, size=size), self.assertRaises(m.BindingError):
                m.address(m.BASE, rva, size)
        for rva, size in ((0, 1), (m.IMAGE_SIZE, 1), (m.ENTRY_RVA, 0)):
            with self.assertRaises(m.BindingError):
                m.raw_span(self.blob, self.pe, rva, size)
        with self.assertRaises(m.BindingError):
            m.span(b'abc', 2, 2)

    def test_malformed_pe(self):
        for blob in (b'', b'MZ', self.blob[:0x200]):
            with self.assertRaises(m.BindingError):
                m.parse_pe(blob)
        mutations = ((0x110 + 4, '<H', 0x14c), (0x128, '<H', 0x10b),
                     (0x110 + 20, '<H', 224), (0x128 + 56, '<I', 10),
                     (0x128 + 240 + 40 + 12, '<I', 0x1000),
                     (0x128 + 240 + 20, '<I', len(self.blob)))
        for offset, fmt, value in mutations:
            blob = bytearray(self.blob); struct.pack_into(fmt, blob, offset, value)
            with self.subTest(offset=offset), self.assertRaises(m.BindingError):
                m.parse_pe(bytes(blob))

    def reloc_image(self, page, items, block=None):
        data = struct.pack('<II', page, block if block is not None else 8 + 2 * len(items))
        data += b''.join(struct.pack('<H', x) for x in items)
        pe = dict(image_size=m.IMAGE_SIZE, sections=[dict(va=0x1000, size=len(data), raw=0, flags=0)],
                  directories=[(0, 0)] * 16)
        pe['directories'][5] = (0x1000, len(data))
        return data, pe

    def test_relocation_overlap_full_prologue_and_prior_write(self):
        protected = [(m.CALL_RVA, 5), (m.ENTRY_RVA, 31)]
        for site in (m.CALL_RVA - 7, m.CALL_RVA, m.ENTRY_RVA - 7,
                     m.ENTRY_RVA + 20, m.ENTRY_RVA + 30):
            data, pe = self.reloc_image(site & ~0xfff, [0xa000 | (site & 0xfff)])
            with self.subTest(site=site), self.assertRaises(m.BindingError):
                m.audit_relocations(data, pe, protected)
        site = m.ENTRY_RVA + 31
        data, pe = self.reloc_image(site & ~0xfff, [0xa000 | (site & 0xfff), 0])
        self.assertEqual(m.audit_relocations(data, pe, protected), 1)

    def test_malformed_relocations(self):
        for page, items, block in ((0x1000, [0xa000], 7), (0x1000, [0xa000], 12),
                                   (0x1001, [0xa000], None), (0x1000, [0x3000], None),
                                   (m.IMAGE_SIZE, [0xa000], None)):
            data, pe = self.reloc_image(page, items, block)
            with self.subTest(page=page, block=block), self.assertRaises(m.BindingError):
                m.audit_relocations(data, pe, [(m.ENTRY_RVA, 31)])

    def identities(self):
        path = r'C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe'
        contract = dict(installed_path=path, size=m.FILE_SIZE, sha256=m.SHA256,
                        volume_serial=123, file_index='456')
        image = {k: contract[k] for k in ('size', 'sha256', 'volume_serial', 'file_index')}
        image['final_path'] = '\\\\?\\' + path
        receipt = dict(creation_image=image, queried_process_path=path, pid=12, primary_tid=13,
                       retained_pid=12, retained_primary_tid=13)
        return receipt, contract

    def test_supplied_identity_positive_is_not_runtime_authorization(self):
        receipt, contract = self.identities()
        self.assertFalse(m.validate_identity_receipt(receipt, contract)['original_run_ready'])

    def test_missing_identity_fields(self):
        receipt, contract = self.identities()
        for key in receipt:
            altered = copy.deepcopy(receipt); del altered[key]
            with self.subTest(key=key), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(altered, contract)
        for key in contract:
            altered = dict(contract); del altered[key]
            with self.subTest(key=key), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(receipt, altered)
        for key in receipt['creation_image']:
            altered = copy.deepcopy(receipt); del altered['creation_image'][key]
            with self.subTest(key=key), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(altered, contract)

    def test_identity_disagreement_and_type_refusal(self):
        receipt, contract = self.identities()
        for key, value in (('final_path', r'C:\wrong.exe'), ('size', 1), ('sha256', '0'*64),
                           ('volume_serial', 124), ('file_index', '457'), ('size', True)):
            altered = copy.deepcopy(receipt); altered['creation_image'][key] = value
            with self.subTest(key=key), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(altered, contract)
        for key, value in (('queried_process_path', r'C:\wrong.exe'), ('pid', 14),
                           ('primary_tid', 14), ('retained_pid', 0), ('retained_primary_tid', True)):
            altered = copy.deepcopy(receipt); altered[key] = value
            with self.subTest(key=key), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(altered, contract)
        for value in ('', '01', '-1', str(1 << 64), 456):
            altered = dict(contract); altered['file_index'] = value
            with self.subTest(value=value), self.assertRaises(m.BindingError):
                m.validate_identity_receipt(receipt, altered)
        altered = dict(contract); altered['installed_path'] = str(m.ORIGINAL).replace('/', '\\')
        with self.assertRaises(m.BindingError):
            m.validate_identity_receipt(receipt, altered)


if __name__ == '__main__':
    unittest.main()
