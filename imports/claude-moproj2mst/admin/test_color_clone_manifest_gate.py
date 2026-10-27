import importlib.util
import unittest
from pathlib import Path
from unittest import mock

MODULE = Path(__file__).with_name("color_clone_manifest_gate.py")
SPEC = importlib.util.spec_from_file_location("color_clone_manifest_gate", MODULE)
gate = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(gate)


class Utf16CodeUnitArgumentTests(unittest.TestCase):
    def test_cp214_exact_literal_round_trips(self):
        literal = "0043003a005c00500052004f0042004c0045004d005c00700032003100340061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a"
        path = gate.decode_utf16_units(literal)
        self.assertEqual(path, r"C:\PROBLEM\p214a\Gear_blank.moproj")
        self.assertEqual(gate.encode_utf16_units(path), literal)

    def test_hex_is_not_byte_order_or_case_flexible(self):
        for value in ("041", "004g", "004A", "d800"):
            with self.subTest(value=value), self.assertRaises(gate.GateError):
                gate.decode_utf16_units(value)

    def test_non_bmp_uses_surrogate_code_units(self):
        value = gate.encode_utf16_units("x\U0001f642")
        self.assertEqual(value, "0078d83dde42")
        self.assertEqual(gate.decode_utf16_units(value), "x\U0001f642")


class ManifestSafetyTests(unittest.TestCase):
    def test_manifest_hashes_relative_files_and_directories(self):
        root = gate.EXPECTED_SOURCE_ROOT
        manifest = gate.directory_manifest(root, enforce_problem_containment=False)
        self.assertEqual(manifest["entryCount"], gate.EXPECTED_ENTRIES)
        self.assertEqual(manifest["fileCount"], gate.EXPECTED_FILES)
        reference = MODULE.parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
        self.assertEqual(manifest["entries"], gate.load_reference(reference))
        self.assertEqual(manifest["entries"][0]["path"], "Equipments")
        self.assertEqual(manifest["entries"][1]["sha256"],
                         "5ca3d185d7124fc473045971bdaf1d49c1a5a25a390850dcd5375211c2c1cfe7")

    def test_reparse_attribute_is_rejected(self):
        class FakeStat:
            st_file_attributes = 0x400
        self.assertTrue(gate._is_reparse(FakeStat()))

    def test_containment_and_length_boundaries(self):
        self.assertTrue(gate._inside(Path(r"C:\PROBLEM\child"), gate.PROBLEM_ROOT))
        self.assertFalse(gate._inside(Path(r"C:\PROBLEMATIC\child"), gate.PROBLEM_ROOT))
        self.assertLess(len(str(Path(r"C:\PROBLEM\p280\Gear_blank.moproj"))), 100)
        self.assertGreaterEqual(len(str(Path("C:\\PROBLEM\\" + "x" * 90 + "\\Gear_blank.moproj"))), 100)

    def test_validate_rejects_source_change_between_scans(self):
        ref = MODULE.parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
        entries = gate.load_reference(ref)
        changed = [dict(entry) for entry in entries]
        changed[1] = dict(changed[1], sha256="0" * 64)
        clone = Path(r"C:\PROBLEM\p280test")
        project = str(clone / gate.PROJECT_NAME)
        encoded = gate.encode_utf16_units(project)
        argv = [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]
        source_manifest = {"root": str(gate.EXPECTED_SOURCE_ROOT), "entries": entries,
                           "entryCount": 33, "fileCount": 27}
        clone_manifest = {"root": str(clone), "entries": entries,
                          "entryCount": 33, "fileCount": 27}
        changed_source = dict(source_manifest, entries=changed)
        with mock.patch.object(gate, "directory_manifest",
                               side_effect=[source_manifest, clone_manifest,
                                            clone_manifest, changed_source]):
            with self.assertRaisesRegex(gate.GateError, "source changed"):
                gate.validate(gate.EXPECTED_SOURCE_ROOT, clone, encoded, clone, argv)

    def test_validate_rejects_clone_change_between_scans(self):
        ref = MODULE.parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
        entries = gate.load_reference(ref)
        changed = [dict(entry) for entry in entries]
        changed[1] = dict(changed[1], sha256="0" * 64)
        clone = Path(r"C:\PROBLEM\p280test")
        encoded = gate.encode_utf16_units(str(clone / gate.PROJECT_NAME))
        argv = [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]
        source_manifest = {"root": str(gate.EXPECTED_SOURCE_ROOT), "entries": entries,
                           "entryCount": 33, "fileCount": 27}
        clone_manifest = {"root": str(clone), "entries": entries,
                          "entryCount": 33, "fileCount": 27}
        changed_clone = dict(clone_manifest, entries=changed)
        with mock.patch.object(gate, "directory_manifest",
                               side_effect=[source_manifest, clone_manifest,
                                            changed_clone]):
            with self.assertRaisesRegex(gate.GateError, "clone changed"):
                gate.validate(gate.EXPECTED_SOURCE_ROOT, clone, encoded, clone, argv)

    def test_validate_requires_and_checks_exact_argv(self):
        with self.assertRaises(TypeError):
            gate.validate(gate.EXPECTED_SOURCE_ROOT, Path(r"C:\PROBLEM\p280test"), "", Path(r"C:\PROBLEM\p280test"))
        ref = MODULE.parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
        entries = gate.load_reference(ref)
        clone = Path(r"C:\PROBLEM\p280test")
        encoded = gate.encode_utf16_units(str(clone / gate.PROJECT_NAME))
        source_manifest = {"root": str(gate.EXPECTED_SOURCE_ROOT), "entries": entries,
                           "entryCount": 33, "fileCount": 27}
        clone_manifest = {"root": str(clone), "entries": entries,
                          "entryCount": 33, "fileCount": 27}
        with mock.patch.object(gate, "directory_manifest",
                               side_effect=[source_manifest, clone_manifest,
                                            clone_manifest, source_manifest]):
            with self.assertRaisesRegex(gate.GateError, "argv must exactly"):
                gate.validate(gate.EXPECTED_SOURCE_ROOT, clone, encoded, clone,
                              [str(gate.EXECUTABLE), "-m", "open", "-f", "wrong"])

    def test_validate_rejects_overlong_project_path(self):
        ref = MODULE.parents[1] / "analysis" / "resume214-original-postopen-2026-10-26T13-17-52-922Z" / "source-fixture-reference.json"
        entries = gate.load_reference(ref)
        clone = Path("C:\\PROBLEM\\" + "x" * 90)
        encoded = gate.encode_utf16_units(str(clone / gate.PROJECT_NAME))
        argv = [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]
        source_manifest = {"root": str(gate.EXPECTED_SOURCE_ROOT), "entries": entries,
                           "entryCount": 33, "fileCount": 27}
        clone_manifest = {"root": str(clone), "entries": entries,
                          "entryCount": 33, "fileCount": 27}
        with mock.patch.object(gate, "directory_manifest",
                               side_effect=[source_manifest, clone_manifest,
                                            clone_manifest, source_manifest]):
            with self.assertRaisesRegex(gate.GateError, "shorter than 100"):
                gate.validate(gate.EXPECTED_SOURCE_ROOT, clone, encoded, clone, argv)


if __name__ == "__main__":
    unittest.main()
