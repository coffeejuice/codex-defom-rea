"""Static CP302 fixture checks; never builds or starts a process."""
import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

MODULE = Path(__file__).with_name("build_cp302_fixture.py")
SPEC = importlib.util.spec_from_file_location("build_cp302_fixture", MODULE)
fixture = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fixture)


class FixtureSourceTests(unittest.TestCase):
    def test_pinned_source(self):
        fixture.check_source()
        self.assertEqual(fixture.SOURCE.read_text(encoding="ascii").strip(),
                         "/* CP302 synthetic child: no callbacks, windows, hooks, or persistent work. */\n"
                         "int main(void)\n{\n    return 0;\n}")

    def test_changed_source_refused(self):
        with patch.object(fixture, "sha256", return_value="0" * 64):
            with self.assertRaisesRegex(ValueError, "changed"):
                fixture.check_source()

    def test_fixed_build_contract(self):
        self.assertEqual(fixture.OUTPUT, Path(r"C:\PROBLEM\s302\fixture.exe"))
        self.assertLess(len(str(fixture.OUTPUT)), 100)
        self.assertEqual(fixture.ALLOWED_IMPORTS, frozenset({"KERNEL32.DLL"}))
        self.assertIn("/c", fixture.COMPILE_FLAGS)
        self.assertIn("/MACHINE:X64", fixture.LINK_FLAGS)
        self.assertIn("/SUBSYSTEM:CONSOLE", fixture.LINK_FLAGS)
        self.assertNotIn("/DLL", fixture.LINK_FLAGS)
        self.assertEqual(fixture.OBJECT.suffix, ".obj")

    def test_build_refuses_existing_artifacts_before_compiler(self):
        with patch.object(Path, "exists", lambda p: p == fixture.OUTPUT), \
             patch.object(fixture, "check_plain_path"), \
             patch.object(fixture, "check_source"):
            with self.assertRaises(FileExistsError):
                fixture.build()

    def test_pe_parser_refuses_malformed_layouts(self):
        b = bytearray(0x600)
        b[:2] = b"MZ"
        struct.pack_into("<I", b, 0x3c, 0x80)
        b[0x80:0x84] = b"PE\0\0"
        struct.pack_into("<HHIIIHH", b, 0x84, 0x8664, 1, 0, 0, 0, 136, 0)
        opt = 0x98
        struct.pack_into("<H", b, opt, 0x20b)
        struct.pack_into("<H", b, opt + 68, 3)
        struct.pack_into("<I", b, opt + 108, 2)
        struct.pack_into("<II", b, opt + 120, 0x1000, 40)
        sec = opt + 136
        struct.pack_into("<IIII", b, sec + 8, 0x200, 0x1000, 0x200, 0x200)
        struct.pack_into("<IIIII", b, 0x200, 0, 0, 0, 0x1080, 0)
        b[0x280:0x28d] = b"KERNEL32.dll\0"
        original = bytes(b)
        path = Path(r"C:\PROBLEM\s302\fixture.exe")
        with patch.object(fixture, "check_plain_path"), \
             patch.object(Path, "read_bytes", side_effect=lambda: bytes(b)):
            self.assertEqual(fixture.inspect_pe(path)["imports"], ["KERNEL32.DLL"])
            for change, reason in ((lambda x: struct.pack_into("<H", x, 0x84, 0x14c), "x64"),
                                   (lambda x: struct.pack_into("<H", x, opt + 68, 2), "console"),
                                   (lambda x: struct.pack_into("<I", x, opt + 120, 0), "import"),
                                   (lambda x: struct.pack_into("<I", x, sec + 12, 0x80), "RVA"),
                                   (lambda x: struct.pack_into("<I", x, sec + 16, 0x20), "RVA"),
                                   (lambda x: struct.pack_into("<H", x, 0x84 + 16, 120), "PE32"),
                                   (lambda x: x.__setitem__(slice(0x280, 0x400), b"A" * 0x180), "unterminated")):
                change(b)
                with self.assertRaisesRegex(ValueError, reason):
                    fixture.inspect_pe(path)
                b[:] = original

    def test_import_name_cannot_cross_raw_boundary(self):
        b = bytearray(0x400)
        b[:2] = b"MZ"
        struct.pack_into("<I", b, 0x3c, 0x80)
        b[0x80:0x84] = b"PE\0\0"
        struct.pack_into("<HHIIIHH", b, 0x84, 0x8664, 1, 0, 0, 0, 136, 0)
        opt = 0x98
        struct.pack_into("<H", b, opt, 0x20b)
        struct.pack_into("<H", b, opt + 68, 3)
        struct.pack_into("<I", b, opt + 108, 2)
        struct.pack_into("<II", b, opt + 120, 0x1000, 40)
        struct.pack_into("<IIII", b, opt + 136 + 8, 0x200, 0x1000, 0x90, 0x200)
        struct.pack_into("<IIIII", b, 0x200, 0, 0, 0, 0x108f, 0)
        b[0x28f] = ord("K")
        b[0x290] = 0
        with patch.object(fixture, "check_plain_path"), \
             patch.object(Path, "read_bytes", return_value=bytes(b)):
            with self.assertRaisesRegex(ValueError, "unterminated"):
                fixture.inspect_pe(Path("fixture.exe"))


if __name__ == "__main__":
    unittest.main()
