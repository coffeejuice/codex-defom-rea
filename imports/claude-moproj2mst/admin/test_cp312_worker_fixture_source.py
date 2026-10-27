"""CP312 static contract checks; no compile, WORKER start, or child start."""
import importlib.util
from pathlib import Path
import re
import unittest
from unittest.mock import patch


MODULE = Path(__file__).with_name("build_cp312_worker_fixture.py")
SPEC = importlib.util.spec_from_file_location("build_cp312_worker_fixture", MODULE)
fixture = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fixture)
SOURCE = fixture.SOURCE.read_text(encoding="ascii")


class WorkerFixtureSourceTests(unittest.TestCase):
    def test_pinned_source_and_fixed_image(self):
        fixture.check_source()
        self.assertEqual(fixture.OUTPUT, Path(r"C:\PROBLEM\s312\worker.exe"))
        self.assertLess(len(str(fixture.OUTPUT)), 100)
        self.assertIn('L"C:\\\\PROBLEM\\\\s302\\\\fixture.exe"', SOURCE)
        self.assertIn("CP302_BYTES 108032u", SOURCE)
        self.assertIn("child_sha256[32]", SOURCE)
        self.assertIn("memcmp(hash, child_sha256, sizeof(hash)) == 0", SOURCE)
        self.assertIn("FILE_SHARE_READ", SOURCE)
        self.assertNotIn("FILE_SHARE_WRITE", SOURCE)
        self.assertNotIn("FILE_SHARE_DELETE", SOURCE)

    def test_changed_source_refused(self):
        with patch.object(fixture, "sha256", return_value="0" * 64):
            with self.assertRaisesRegex(ValueError, "changed"):
                fixture.check_source()

    def test_creation_shape_and_ack_gate(self):
        self.assertIn("#define CP312_VERSION 2u", SOURCE)
        self.assertIn("JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE", SOURCE)
        self.assertIn("JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK", SOURCE)
        self.assertIn("!safe_job_limits(child_job)", SOURCE)
        self.assertLess(SOURCE.index("!safe_job_limits(child_job)"), SOURCE.index("CreateProcessW(child_path"))
        self.assertNotIn("worker_job", SOURCE)
        self.assertNotIn("worker_query", SOURCE)
        self.assertIn("CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW", SOURCE)
        self.assertIn("UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_JOB_LIST", SOURCE)
        self.assertIn("list[0] = child_job;", SOURCE)
        self.assertRegex(SOURCE, r"CreateProcessW\(child_path, command, NULL, NULL, FALSE, flags,")
        self.assertIn("result.create_error = GetLastError();", SOURCE)
        self.assertIn("result.child_query_ok = IsProcessInJob(child.hProcess, child_job", SOURCE)
        self.assertNotIn("ResumeThread(", SOURCE)
        self.assertNotIn("CREATE_BREAKAWAY_FROM_JOB", SOURCE)
        self.assertLess(SOURCE.index("read_exact(input, &ack"), SOURCE.index("CloseHandle(child.hThread)"))
        self.assertLess(SOURCE.index("read_exact(input, &ack"), SOURCE.index("CloseHandle(child.hProcess)"))
        self.assertLess(SOURCE.index("read_exact(input, &ack"), SOURCE.index("CloseHandle(child_job)"))
        self.assertIn("memcmp(ack.nonce, result.nonce, sizeof(ack.nonce))", SOURCE)
        self.assertIn("hold_until_supervisor_terminates();", SOURCE)
        readme = fixture.SOURCE.with_name("README.md").read_text(encoding="utf-8")
        self.assertIn("supervisor alone owns the WORKER job lifetime", readme)
        self.assertIn("membership in both exact jobs using supervisor-local handles", readme)

    def test_build_is_pinned_and_inert(self):
        self.assertIn("/W4", fixture.COMPILE_FLAGS)
        self.assertIn("/WX", fixture.COMPILE_FLAGS)
        self.assertIn("/MT", fixture.COMPILE_FLAGS)
        self.assertIn("/MACHINE:X64", fixture.LINK_FLAGS)
        self.assertIn("/SUBSYSTEM:CONSOLE", fixture.LINK_FLAGS)
        self.assertIn("bcrypt.lib", fixture.LINK_FLAGS)
        self.assertEqual(fixture.ALLOWED_IMPORTS, frozenset({"KERNEL32.DLL", "BCRYPT.DLL"}))
        builder = MODULE.read_text(encoding="utf-8")
        self.assertNotIn("Popen(", builder)
        self.assertNotIn("CREATE_SUSPENDED", builder)
        self.assertEqual(len(re.findall(r"subprocess\.run\(", builder)), 2)

    def test_build_refuses_existing_artifacts_before_compiler(self):
        with patch.object(Path, "exists", lambda p: p == fixture.OUTPUT), \
             patch.object(fixture.pe_gate, "check_plain_path"), \
             patch.object(fixture, "check_source"):
            with self.assertRaises(FileExistsError):
                fixture.build()


if __name__ == "__main__":
    unittest.main()
