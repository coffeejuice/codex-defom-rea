"""Windows same-handle collector tests; all fixtures are local disposable bytes."""
import hashlib
import json
from pathlib import Path
import os
import tempfile
import unittest
from unittest.mock import patch
import color_installed_file_identity as c

@unittest.skipUnless(os.name == "nt", "Windows handle APIs required")
class IdentityTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="cp288-", dir=c.ROOT / "analysis")
        self.root = Path(self.temp.name)
        self.pins = []
        for index in range(3):
            path = self.root / (str(index) + ".bin")
            data = ("disposable fixture " + str(index)).encode()
            path.write_bytes(data)
            self.pins.append((str(path), len(data), hashlib.sha256(data).hexdigest()))
        self.api = c.WindowsFiles()

    def tearDown(self):
        self.temp.cleanup()

    def check_closed(self, receipt):
        self.assertTrue(receipt["allHandlesClosed"])
        self.assertEqual(receipt["handlesOpened"], receipt["handlesClosed"])
        for path, _, _ in self.pins:
            # This write open would conflict with an unclosed collector read handle.
            with open(path, "r+b") as stream:
                stream.write(b"d")

    def test_identity_and_collective_lifetime(self):
        live = set()
        snapshots = []
        original_open, original_close, original_snapshot = self.api.open, self.api.close, self.api.snapshot
        def opened(path):
            handle = original_open(path)
            live.add(handle)
            return handle
        def closed(handle):
            original_close(handle)
            live.remove(handle)
        def snapshot(handle):
            snapshots.append(len(live))
            return original_snapshot(handle)
        with patch.object(self.api, "open", opened), patch.object(self.api, "close", closed), patch.object(self.api, "snapshot", snapshot):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(receipt["status"], "ACQUIRED_STATIC_IDENTITIES_ONLY")
        self.assertEqual(snapshots[-3:], [3, 3, 3])
        self.assertEqual(live, set())
        self.assertFalse(receipt["original_run_ready"])
        self.assertEqual(receipt["runtimeLoadedModuleIdentity"], "unknown")
        self.assertEqual(receipt["collectorSha256"], hashlib.sha256(Path(c.__file__).read_bytes()).hexdigest())
        for observed, pin in zip(receipt["files"], self.pins):
            self.assertEqual(observed["sha256"], pin[2])
            self.assertEqual(observed["before"]["size"], pin[1])
            self.assertEqual(observed["before"]["finalPath"], "\\\\?\\" + pin[0])
            self.assertEqual(observed["before"], observed["collectiveFinal"])
            self.assertGreater(observed["before"]["fileIndex"], 0)
        self.check_closed(receipt)

    def test_existing_writer_refuses_sharing(self):
        handle = self.api.k.CreateFileW(self.pins[1][0], 0x40000000, 7, None, 3, 0, None)
        self.assertNotEqual(handle, c.INVALID_HANDLE_VALUE)
        try:
            receipt = c._collect(self.pins, self.api)
        finally:
            self.api.close(handle)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.assertEqual(receipt["handlesOpened"], 1)
        self.assertIn("32", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_held_reader_refuses_writer_and_delete_sharing(self):
        handle = self.api.open(self.pins[0][0])
        try:
            for access in (0x40000000, 0x10000):
                other = self.api.k.CreateFileW(self.pins[0][0], access, 7, None, 3, 0, None)
                if other != c.INVALID_HANDLE_VALUE:
                    self.api.close(other)
                    self.fail("writer/delete access accepted during held identity read")
        finally:
            self.api.close(handle)

    def test_wrong_digest_no_partial_success(self):
        pins = list(self.pins)
        pins[1] = (pins[1][0], pins[1][1], "0" * 64)
        receipt = c._collect(pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.assertEqual(receipt["handlesOpened"], 2)
        self.check_closed(receipt)

    def test_wrong_size(self):
        pins = [(self.pins[0][0], 999, self.pins[0][2])]
        receipt = c._collect(pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.check_closed(receipt)

    def test_injected_second_read_failure_closes_all(self):
        original = self.api.digest
        calls = 0
        def fail_second(handle):
            nonlocal calls
            calls += 1
            if calls == 2:
                raise OSError("injected mid-acquisition read failure")
            return original(handle)
        with patch.object(self.api, "digest", fail_second):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.assertEqual(receipt["handlesOpened"], 2)
        self.check_closed(receipt)

    def test_collective_changed_identity_holds(self):
        original = self.api.snapshot
        calls = 0
        def changed(handle):
            nonlocal calls
            calls += 1
            value = original(handle)
            if calls == 7:
                value["lastWriteFileTime"] += 1
            return value
        with patch.object(self.api, "snapshot", changed):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.assertEqual(receipt["handlesOpened"], 3)
        self.check_closed(receipt)

    def test_wrong_final_path_holds(self):
        original = self.api.snapshot
        def changed(handle):
            value = original(handle)
            value["finalPath"] += ".other"
            return value
        with patch.object(self.api, "snapshot", changed):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.check_closed(receipt)

    def test_reparse_attribute_refused_deterministically(self):
        # Exercise the real snapshot refusal with a successful metadata API call
        # reporting FILE_ATTRIBUTE_REPARSE_POINT; no symlink privilege required.
        original = self.api.k.GetFileInformationByHandle
        def reparse_metadata(handle, output):
            ok = original(handle, output)
            if ok:
                c.ctypes.cast(output, c.ctypes.POINTER(c.Info)).contents.attributes |= 0x400
            return ok
        with patch.object(self.api.k, "GetFileInformationByHandle", reparse_metadata):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertEqual(receipt["files"], [])
        self.assertIn("reparse", receipt["errors"][0]["message"])
        self.assertEqual(receipt["handlesOpened"], 1)
        self.check_closed(receipt)

    def test_reparse_point_rejected(self):
        link = self.root / "link.bin"
        try:
            os.symlink(self.pins[0][0], link)
        except OSError as error:
            self.skipTest("symlink privilege unavailable: " + str(error))
        receipt = c._collect([(str(link), self.pins[0][1], self.pins[0][2])], self.api)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertIn("reparse", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_close_failure_is_hold_and_remaining_handles_attempted(self):
        original = self.api.close
        calls = 0
        def fail_receipt(handle):
            nonlocal calls
            calls += 1
            original(handle)
            if calls == 1:
                raise OSError("injected close reporting failure after actual close")
        with patch.object(self.api, "close", fail_receipt):
            receipt = c._collect(self.pins, self.api)
        self.assertEqual(calls, 3)
        self.assertEqual(receipt["status"], "HOLD")
        self.assertFalse(receipt["allHandlesClosed"])
        self.assertEqual(receipt["files"], [])

if __name__ == "__main__":
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(IdentityTests)
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    summary = {"schema": "cp288-identity-tests-v1", "testsRun": result.testsRun,
               "failures": len(result.failures), "errors": len(result.errors),
               "skipped": [{"test": str(test), "reason": reason} for test, reason in result.skipped],
               "sourceSha256": hashlib.sha256(Path(c.__file__).read_bytes()).hexdigest(),
               "testSha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
               "passed": result.wasSuccessful(), "finishedUtc": c.datetime.now(c.timezone.utc).isoformat()}
    (c.ROOT / "analysis/cp288-installed-file-identity-tests.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    raise SystemExit(0 if result.wasSuccessful() else 1)
