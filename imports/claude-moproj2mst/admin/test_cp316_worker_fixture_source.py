"""Static CP316 source contract checks: never compile or execute a worker."""
import importlib.util
from pathlib import Path
import re
import struct
import unittest
from unittest.mock import patch

MODULE = Path(__file__).with_name("build_cp316_worker_fixture.py")
SPEC = importlib.util.spec_from_file_location("build_cp316_worker_fixture", MODULE)
fixture = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fixture)
SOURCE = fixture.SOURCE.read_text(encoding="ascii")
CODE = re.sub(r"/\*.*?\*/", "", SOURCE, flags=re.S)


def c_wire_format(name):
    body = re.search(r"typedef struct \{([^{}]+)\} " + name + ";", CODE).group(1)
    result = "<"
    for declaration in body.split(";"):
        declaration = declaration.strip()
        if not declaration:
            continue
        if declaration == "unsigned char nonce[32]":
            result += "32s"
        else:
            kind, names = declaration.split(" ", 1)
            result += {"uint32_t": "I", "uint64_t": "Q"}[kind] * len(names.split(","))
    return result


class WorkerOnlySourceTests(unittest.TestCase):
    def test_source_pin(self):
        fixture.check_source()
        self.assertNotEqual(fixture.SOURCE_SHA256, "TO_BE_PINNED")
        with patch.object(fixture, "sha256", return_value="0" * 64):
            with self.assertRaisesRegex(ValueError, "changed"):
                fixture.check_source()

    def test_separate_fixed_output(self):
        self.assertEqual(fixture.SOURCE.parent.name, "cp316")
        self.assertEqual(fixture.OUTPUT, Path(r"C:\PROBLEM\s316\worker.exe"))
        self.assertLess(len(str(fixture.OUTPUT)), 100)
        self.assertEqual(fixture.NATIVE_ENTRY, "HARD HOLD")

    def test_wire_layout_matches_cp312_v2(self):
        self.assertIn("#pragma pack(push, 1)", CODE)
        self.assertIn("#define CP312_MAGIC 0x32313343u", CODE)
        self.assertIn("#define CP312_VERSION 2u", CODE)
        for name, expected, size in (("CP312_INIT", "<IIQ32s", 48),
                                     ("CP312_RESULT", "<" + "I" * 10 + "Q32s", 80),
                                     ("CP312_ACK", "<IIQ32s", 48)):
            self.assertEqual(c_wire_format(name), expected)
            self.assertEqual(struct.calcsize(expected), size)
            self.assertIn(f'_Static_assert(sizeof({name}) == {size},', CODE)

    def test_no_child_creation_image_open_or_mode_selector(self):
        for forbidden in ("CreateProcess", "ShellExecute", "WinExec", "CreateFile",
                          "LoadLibrary", "GetProcAddress", "ResumeThread", "bcrypt",
                          "NtQuery", "CP302", "STARTUPINFO", "PROCESS_INFORMATION"):
            self.assertNotIn(forbidden, CODE)
        self.assertIn("argc != 2", CODE)
        self.assertNotIn("getenv", CODE)
        self.assertNotIn("GetEnvironmentVariable", CODE)

    def test_result_only_sets_header_and_nonce(self):
        self.assertIn("ZeroMemory(&result, sizeof(result));", CODE)
        assignments = re.findall(r"result\.(\w+)\s*=(?!=)", CODE)
        self.assertEqual(assignments, ["magic", "version"])
        self.assertIn("memcpy(result.nonce, init.nonce, sizeof(init.nonce));", CODE)
        self.assertIn("init.child_job == 0", CODE)
        self.assertIn("!safe_child_job(child_job)", CODE)

    def test_ack_is_strict_and_precedes_close(self):
        for check in ("ack.magic != CP312_MAGIC", "ack.version != CP312_VERSION",
                      "ack.child_process_handle != 0",
                      "memcmp(ack.nonce, result.nonce, sizeof(ack.nonce)) != 0"):
            self.assertIn(check, CODE)
            self.assertLess(CODE.index(check), CODE.index("CloseHandle(child_job)"))
        self.assertLess(CODE.index("transfer_exact(input, &ack"), CODE.index("CloseHandle(event)"))

    def test_absolute_deadline_precedes_first_init_read(self):
        main = CODE[CODE.index("int wmain("):]
        self.assertLess(main.index("parse_deadline(argc, argv)"), main.index("transfer_exact(input, &init"))
        self.assertEqual(main.count("parse_deadline("), 1)
        self.assertIn("now >= deadline", CODE)
        self.assertIn("deadline - now > MAX_REMAINING_MS", CODE)
        self.assertIn("#define MAX_REMAINING_MS 5000u", CODE)
        self.assertIn("value > (UINT64_MAX - digit) / 10u", CODE)
        self.assertIn("i >= 20", CODE)
        self.assertNotIn("deadline = GetTickCount64()", CODE)
        for frame in ("input, &init, sizeof(init), 0", "output, &result, sizeof(result), 1",
                      "input, &ack, sizeof(ack), 0"):
            self.assertIn("transfer_exact(" + frame + ", deadline, event);", main)

    def test_overlapped_transfer_and_uncertain_retention(self):
        for name in ("OVERLAPPED operation", "CP312_INIT init", "CP312_RESULT result", "CP312_ACK ack"):
            self.assertIn("static " + name + ";", CODE)
        transfer = CODE[CODE.index("static void transfer_exact"):CODE.index("int wmain(")]
        self.assertEqual(transfer.count("NULL, &operation)"), 2)
        self.assertIn("error != ERROR_IO_PENDING", transfer)
        self.assertIn("WaitForSingleObject(event, budget) != WAIT_OBJECT_0", transfer)
        self.assertIn("GetOverlappedResult(pipe, &operation, &count, FALSE)", transfer)
        self.assertIn("(void)CancelIoEx(pipe, &operation);\n                hold_until_supervisor_terminates();", transfer)
        self.assertIn("count == 0 || count > length - done", transfer)
        self.assertNotIn("CloseHandle", transfer)
        self.assertNotIn("INFINITE", transfer)
        self.assertIn("PIPE_SERVER_END | PIPE_TYPE_MESSAGE", CODE)

    def test_limits_documented_without_live_claim(self):
        readme = fixture.SOURCE.with_name("README.md").read_text(encoding="utf-8")
        for required in ("HARD HOLD", "cannot prove asynchronous handle mode", "BEFORE",
                         "FILE_FLAG_OVERLAPPED", "not successful timeout cleanup",
                         "but never executed"):
            self.assertIn(required, readme)

    def test_builder_only_invokes_compiler_and_linker(self):
        self.assertEqual(fixture.ALLOWED_IMPORTS, frozenset({"KERNEL32.DLL"}))
        self.assertEqual(fixture.pe_gate.ALLOWED_IMPORTS, fixture.ALLOWED_IMPORTS)
        for flag in ("/W4", "/WX", "/MT", "/std:c11"):
            self.assertIn(flag, fixture.COMPILE_FLAGS)
        builder = MODULE.read_text(encoding="utf-8")
        self.assertEqual(len(re.findall(r"subprocess\.run\(", builder)), 2)
        self.assertNotIn("Popen(", builder)

    def test_existing_artifacts_refused_before_compiler(self):
        for occupied in (fixture.OUTPUT, fixture.OBJECT, fixture.RECEIPT):
            with self.subTest(path=occupied), patch.object(Path, "exists", lambda p: p == occupied), \
                 patch.object(fixture.pe_gate, "check_plain_path"), patch.object(fixture, "check_source"), \
                 patch.object(fixture.subprocess, "run") as execute:
                with self.assertRaises(FileExistsError):
                    fixture.build()
                execute.assert_not_called()

    def test_implicit_compiler_linker_options_removed(self):
        injected = {"CL": "/FIunreviewed.h", "_CL_": "/DUNREVIEWED=1",
                    "LINK": "/ENTRY:unreviewed", "_LINK_": "/INCLUDE:unreviewed",
                    "cL": "/DLOWERCASE=1", "Path": "sdk-tools", "INCLUDE": "sdk-headers"}
        with patch.dict(fixture.os.environ, injected, clear=True):
            cleaned = fixture.compiler_environment()
            self.assertFalse(set(name.upper() for name in cleaned) & fixture.OPTION_ENVIRONMENT)
            self.assertEqual(cleaned.get("Path", cleaned.get("PATH")), "sdk-tools")
            self.assertEqual(cleaned["INCLUDE"], "sdk-headers")
        builder = MODULE.read_text(encoding="utf-8")
        self.assertEqual(builder.count("env=child_environment)"), 2)


if __name__ == "__main__":
    unittest.main()
