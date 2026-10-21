"""Check evidence fidelity, selection boundaries and source-identity failures."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

SPEC = importlib.util.spec_from_file_location("evidence_slice", Path(__file__).resolve().parents[1] / "tools/evidence_slice.py")
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class EvidenceSliceTests(unittest.TestCase):
    def setUp(self):
        scratch = Path(__file__).resolve().parents[1] / 'build' / 'evidence-slice-tests'
        scratch.mkdir(parents=True, exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=scratch)
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "evidence.json"
        self.doc = {"evidence_id": "fixture-only", "limitations": ["partial"],
                    "normalized_result": {"pseudocode": "alpha\r\nbeta Ω\nalpha",
                                          "assembly": [{"address": "0x10"}, {"address": "0x20"}]},
                    "a/b": {"~": [0, 1]}}
        self.raw = b'\xef\xbb\xbf' + json.dumps(self.doc, ensure_ascii=False).encode("utf-8")
        self.path.write_bytes(self.raw)

    def test_text_reconstruction_preserves_line_endings_and_unicode(self):
        output = MODULE.extract(self.path, "/normalized_result/pseudocode")
        self.assertEqual(''.join(x['value'] for x in output['selection']['items']), self.doc['normalized_result']['pseudocode'])
        self.assertEqual(output['source']['sha256'], hashlib.sha256(self.raw).hexdigest())
        self.assertEqual(output['qualifications']['/limitations'], ['partial'])
        self.assertEqual(self.path.read_bytes(), self.raw)

    def test_filtered_paging_retains_original_indices(self):
        output = MODULE.extract(self.path, "/normalized_result/pseudocode", 1, 1, "alpha")['selection']
        self.assertEqual(output['items'], [{'index': 2, 'value': 'alpha'}])
        self.assertEqual(output['matching_items'], 2)
        self.assertEqual(output['omitted_items'], 2)
        self.assertIsNone(output['next_start'])

    def test_page_continuation(self):
        first = MODULE.extract(self.path, "/normalized_result/assembly", count=1)['selection']
        second = MODULE.extract(self.path, "/normalized_result/assembly", start=first['next_start'], count=1)['selection']
        self.assertEqual([x['value'] for x in first['items'] + second['items']], self.doc['normalized_result']['assembly'])

    def test_escaped_pointer(self):
        self.assertEqual(MODULE.extract(self.path, '/a~1b/~0/1')['selection']['value'], 1)

    def test_nested_record_identity_not_container_identity(self):
        self.path.write_text(json.dumps({'records': [self.doc]}), encoding='utf-8')
        output = MODULE.extract(self.path, '/normalized_result/pseudocode', record_pointer='/records/0')
        self.assertEqual(output['identity']['evidence_id'], 'fixture-only')
        self.assertEqual(output['source']['record_pointer'], '/records/0')
        self.assertEqual(output['qualifications']['/limitations'], ['partial'])

    def test_rejects_wrong_source_and_invalid_selection(self):
        for kwargs in ({'expected_sha256': '0' * 64}, {'pointer': '/missing'},
                       {'pointer': '/normalized_result/assembly/-1'}, {'count': 0},
                       {'pointer': '/a~2b'}, {'contains': 'alpha'}):
            with self.subTest(kwargs=kwargs), self.assertRaises((ValueError, KeyError)):
                MODULE.extract(self.path, **kwargs)


if __name__ == '__main__':
    unittest.main()
