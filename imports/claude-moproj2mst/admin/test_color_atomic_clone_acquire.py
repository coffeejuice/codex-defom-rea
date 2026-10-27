"""CP289 Windows native tests; only tiny disposable trees under verified TEMP.

Never invokes acquire_fixed(), C:\\PROBLEM, original applications or GUI APIs.
"""
import ctypes
import hashlib
import json
import os
from pathlib import Path
import shutil
import tempfile
import time
import unittest
from unittest.mock import patch

import color_atomic_clone_acquire as c


@unittest.skipUnless(os.name == "nt", "Windows native handle APIs required")
class AtomicCloneTests(unittest.TestCase):
    def setUp(self):
        self.temp_base = Path(tempfile.gettempdir()).resolve(strict=True)
        c.gate._check_components(self.temp_base)
        self.root = Path(tempfile.mkdtemp(prefix="m289t_", dir=self.temp_base)).resolve(strict=True)
        self.assertEqual(self.root.parent, self.temp_base)
        self.assertNotEqual(self.root, self.temp_base)
        c.gate._check_components(self.root)
        self.addCleanup(self.cleanup_temp)
        self.source = self.root / "s"
        self.parent = self.root / "p"
        self.source.mkdir()
        self.parent.mkdir()
        (self.source / "D").mkdir()
        (self.source / c.gate.PROJECT_NAME).write_bytes(b"tiny project\r\n")
        (self.source / "D/a.bin").write_bytes(b"x" * 73)
        (self.source / "empty.bin").write_bytes(b"")
        self.entries = c.gate.directory_manifest(self.source, enforce_problem_containment=False)["entries"]
        self.name = "m289_0123456789abcdef"
        self.clone = self.parent / self.name
        self.api = c.WindowsCloneFiles()

    def cleanup_temp(self):
        # Explicitly verify the resolved recursive-cleanup target remains the
        # one disposable TEMP child before recursive cleanup removes anything.
        resolved = self.root.resolve(strict=True)
        self.assertEqual(resolved, self.root)
        self.assertEqual(resolved.parent, self.temp_base)
        self.assertTrue(resolved.name.startswith("m289t_"))
        c.gate._check_components(resolved)
        shutil.rmtree(resolved)

    def acquire(self, **kwargs):
        return c._acquire(self.source, self.parent, self.entries,
                          api=self.api, name=self.name, **kwargs)

    def check_closed(self, receipt):
        self.assertTrue(receipt["allHandlesClosed"], receipt["errors"])
        self.assertEqual(receipt["handlesOpened"], receipt["handlesClosed"])
        self.assertEqual(receipt["status"], "HOLD")
        self.assertFalse(receipt["original_run_ready"])
        self.assertEqual(receipt["runtimeLoadedModuleIdentity"], "unknown")

    def test_x64_layout_and_native_signature(self):
        self.assertEqual(c.layout(), {"pointerBytes": 8, "ntstatusBytes": 4,
            "unicodeStringBytes": 16, "objectAttributesBytes": 48,
            "ioStatusBlockBytes": 16, "unicodeBufferOffset": 8,
            "rootDirectoryOffset": 8, "objectNameOffset": 16,
            "attributesOffset": 24, "informationOffset": 8})
        self.assertIs(self.api.n.NtCreateFile.restype, ctypes.c_int32)
        self.assertEqual(len(self.api.n.NtCreateFile.argtypes), 11)

    def test_native_success_manifest_command_and_lifetime(self):
        live, boundaries = set(), []
        original_open, original_source = self.api.open_directory, self.api.open_source
        original_create, original_close = self.api.create_relative, self.api.close
        original_gate, original_manifest = c._command_gate, c.gate.directory_manifest
        def opened(path):
            handle = original_open(path)
            live.add(handle)
            return handle
        def source(path):
            handle = original_source(path)
            live.add(handle)
            return handle
        def created(parent, name, directory):
            self.assertIn(parent, live)
            handle, result = original_create(parent, name, directory)
            if c._valid_handle(handle):
                live.add(handle)
            return handle, result
        def closed(handle):
            original_close(handle)
            live.remove(handle)
        def manifest(path, **kwargs):
            boundaries.append(("manifest", str(path), len(live)))
            return original_manifest(path, **kwargs)
        def command(*args):
            boundaries.append(("command", str(args[1]), len(live)))
            self.assertEqual(len(live), 9)  # parent, root, D, 3 source + 3 dest
            # Retained root handle denies DELETE access; no rename is attempted.
            h = self.api.k.CreateFileW(str(self.clone), 0x10000, 7, None, 3, 0x02000000, None)
            if c._valid_handle(h):
                original_close(h)
                self.fail("clone directory accepted DELETE access while retained")
            self.assertEqual(ctypes.get_last_error(), 32)
            return original_gate(*args)
        with patch.object(self.api, "open_directory", opened), patch.object(self.api, "open_source", source), \
                patch.object(self.api, "create_relative", created), patch.object(self.api, "close", closed), \
                patch.object(c.gate, "directory_manifest", manifest), patch.object(c, "_command_gate", command):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY", receipt["errors"])
        self.assertEqual(live, set())
        self.assertEqual([b[2] for b in boundaries], [1, 9, 9, 9])
        self.assertEqual(len(receipt["creationAttempts"]), 5)
        self.assertEqual(len(receipt["copiedFiles"]), 3)
        for attempt in receipt["creationAttempts"]:
            self.assertEqual(attempt["ntstatusHex"], "0x00000000")
            self.assertEqual(attempt["ioStatusHex"], "0x00000000")
            self.assertEqual(attempt["information"], c.FILE_CREATED)
            self.assertEqual(attempt["createDisposition"], c.FILE_CREATE)
            self.assertTrue(attempt["createOptions"] & c.FILE_SYNCHRONOUS_IO_NONALERT)
            self.assertEqual(attempt["createOptions"] & c.FILE_DIRECTORY_FILE,
                             c.FILE_DIRECTORY_FILE if attempt["directory"] else 0)
        self.assertEqual(receipt["sourceBefore"]["entries"], receipt["cloneAfter"]["entries"])
        self.assertEqual(receipt["sourceBefore"], receipt["sourceAfter"])
        contract = receipt["launchContract"]
        self.assertEqual(c.gate.decode_utf16_units(contract["encodedPath"]), contract["projectPath"])
        self.assertEqual(contract["cwd"], str(self.clone))
        self.assertLess(contract["maxFixturePathCharacters"], 100)
        self.assertEqual(receipt["collectorSha256"], hashlib.sha256(Path(c.__file__).read_bytes()).hexdigest())
        self.check_closed(receipt)

    def test_collision_no_reuse_retry_or_delete(self):
        self.clone.mkdir()
        sentinel = self.clone / "sentinel"
        sentinel.write_bytes(b"leave me")
        receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(len(receipt["creationAttempts"]), 1)
        attempt = receipt["creationAttempts"][0]
        self.assertEqual(attempt["ntstatusHex"], "0xc0000035")
        self.assertEqual(attempt["ntstatusSigned"], ctypes.c_int32(0xc0000035).value)
        self.assertEqual(sentinel.read_bytes(), b"leave me")
        self.assertEqual(list(self.parent.iterdir()), [self.clone])
        self.check_closed(receipt)

    def test_native_relative_name_and_utf16_buffer(self):
        original = self.api.n.NtCreateFile
        seen = []
        def inspect(handle, access, attrs_pointer, iosb, allocation, attributes,
                    sharing, disposition, options, ea, ea_length):
            attrs = ctypes.cast(attrs_pointer, ctypes.POINTER(c.OBJECT_ATTRIBUTES)).contents
            text = attrs.ObjectName.contents
            name = ctypes.wstring_at(text.Buffer, text.Length // 2)
            seen.append(name)
            self.assertEqual(text.Length, len(name.encode("utf-16-le")))
            self.assertEqual(text.MaximumLength, text.Length + 2)
            self.assertEqual(ctypes.string_at(text.Buffer + text.Length, 2), b"\0\0")
            self.assertTrue(c._valid_handle(attrs.RootDirectory))
            self.assertEqual(attrs.Length, 48)
            self.assertEqual(attrs.Attributes, c.OBJ_CASE_INSENSITIVE | c.OBJ_DONT_REPARSE)
            self.assertFalse(attrs.Attributes & 0x80)  # OBJ_OPENIF
            self.assertEqual(disposition, c.FILE_CREATE)
            self.assertFalse(options & 0x1000)  # FILE_DELETE_ON_CLOSE
            return original(handle, access, attrs_pointer, iosb, allocation,
                            attributes, sharing, disposition, options, ea, ea_length)
        with patch.object(self.api.n, "NtCreateFile", inspect):
            receipt = self.acquire()
        self.assertEqual(seen[0], self.name)
        self.assertEqual(len(seen), 5)
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY")
        self.check_closed(receipt)

    def test_injected_pending_closes_returned_handle_preserves_directory(self):
        original = self.api.create_relative
        def incomplete(*args):
            handle, result = original(*args)
            result["ntstatusSigned"], result["ntstatusHex"] = 0x103, "0x00000103"
            return handle, result
        with patch.object(self.api, "create_relative", incomplete):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertTrue(self.clone.is_dir())
        self.assertEqual(receipt["handlesOpened"], 2)
        self.assertEqual(len(receipt["creationAttempts"]), 1)
        self.check_closed(receipt)

    def test_wrong_information_is_not_freshness(self):
        original = self.api.create_relative
        def changed(*args):
            handle, result = original(*args)
            result["information"] = 1  # FILE_OPENED must never count as creation
            return handle, result
        with patch.object(self.api, "create_relative", changed):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertTrue(self.clone.is_dir())
        self.check_closed(receipt)

    def test_invalid_returned_handle_rejected(self):
        def invalid(*args):
            return None, {"ntstatusSigned": 0, "ntstatusHex": "0x00000000",
                "ioStatusSigned": 0, "ioStatusHex": "0x00000000",
                "information": c.FILE_CREATED, "returnedHandle": "None"}
        with patch.object(self.api, "create_relative", side_effect=invalid) as create:
            receipt = self.acquire()
        self.assertEqual(create.call_count, 1)
        self.assertEqual(receipt["handlesOpened"], 1)
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.check_closed(receipt)

    def test_one_uuid_name_generated_once(self):
        value = c.uuid.UUID("76543210fedcba980123456789abcdef")
        with patch.object(c.uuid, "uuid4", return_value=value) as new_uuid:
            receipt = c._acquire(self.source, self.parent, self.entries, api=self.api)
        self.assertEqual(new_uuid.call_count, 1)
        self.assertEqual(receipt["cloneRoot"], str(self.parent / "m289_76543210fedcba98"))
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY", receipt["errors"])
        self.check_closed(receipt)

    def test_fixed_identity_receipt_mismatch_prevents_acquisition(self):
        bad = self.root / "bad.json"
        bad.write_bytes(b"{}")
        with patch.object(c, "INSTALLED_RECEIPT", bad), patch.object(c, "_acquire") as acquire:
            with self.assertRaisesRegex(ValueError, "SHA-256 mismatch"):
                c.acquire_fixed()
        acquire.assert_not_called()

    def test_repeat_cli_refuses_existing_receipt_before_acquisition(self):
        existing = self.root / "receipt.json"
        existing.write_bytes(b"existing receipt")
        with patch.object(c, "OUTPUT", existing), patch.object(c.sys, "argv", ["collector"]), \
                patch.object(c, "acquire_fixed") as acquire:
            with self.assertRaises(FileExistsError):
                c.main()
        acquire.assert_not_called()
        self.assertEqual(existing.read_bytes(), b"existing receipt")

    def test_nested_directory_collision_preserved(self):
        original = self.api.snapshot
        injected = False
        def collision(handle, path, directory):
            nonlocal injected
            value = original(handle, path, directory)
            if path == self.clone and directory and not injected:
                (self.clone / "D").mkdir()
                (self.clone / "D/sentinel").write_bytes(b"foreign")
                injected = True
            return value
        with patch.object(self.api, "snapshot", collision):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(len(receipt["creationAttempts"]), 2)
        self.assertEqual(receipt["creationAttempts"][-1]["ntstatusHex"], "0xc0000035")
        self.assertEqual((self.clone / "D/sentinel").read_bytes(), b"foreign")
        self.check_closed(receipt)

    def test_source_extra_entry_after_copy_detected(self):
        original = c.gate.directory_manifest
        def changed(path, **kwargs):
            value = original(path, **kwargs)
            if path == self.clone:
                (self.source / "extra.bin").write_bytes(b"extra")
            return value
        with patch.object(c.gate, "directory_manifest", changed):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertIn("source changed", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_raw_ntstatus_failure_without_fallback(self):
        def denied(*args):
            return None, {"ntstatusSigned": ctypes.c_int32(0xc0000022).value,
                "ntstatusHex": "0xc0000022", "ioStatusSigned": ctypes.c_int32(0xc0000022).value,
                "ioStatusHex": "0xc0000022", "information": 0, "returnedHandle": "None"}
        with patch.object(self.api, "create_relative", side_effect=denied) as create:
            receipt = self.acquire()
        self.assertEqual(create.call_count, 1)
        self.assertFalse(self.clone.exists())
        self.assertEqual(receipt["creationAttempts"][0]["ntstatusHex"], "0xc0000022")
        self.check_closed(receipt)

    def test_copy_failure_preserves_partial_files_and_closes_handles(self):
        with patch.object(self.api, "copy", side_effect=OSError("injected copy failure")):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertTrue(self.clone.is_dir())
        self.assertTrue((self.clone / "D/a.bin").is_file())
        self.assertEqual((self.clone / "D/a.bin").stat().st_size, 0)
        self.assertIn("injected copy failure", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_source_changed_before_creation_no_clone(self):
        (self.source / "D/a.bin").write_bytes(b"changed")
        receipt = self.acquire()
        self.assertEqual(receipt["creationAttempts"], [])
        self.assertFalse(self.clone.exists())
        self.check_closed(receipt)

    def test_extra_clone_entry_fails_manifest(self):
        original = self.api.copy
        injected = False
        def copy(*args):
            nonlocal injected
            result = original(*args)
            if not injected:
                (self.clone / "extra.bin").write_bytes(b"extra")
                injected = True
            return result
        with patch.object(self.api, "copy", copy):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertIn("clone manifest mismatch", receipt["errors"][0]["message"])
        self.assertTrue((self.clone / "extra.bin").exists())
        self.check_closed(receipt)

    def test_parent_reparse_attribute_refused_before_creation(self):
        original = self.api.k.GetFileInformationByHandle
        def reparse(handle, pointer):
            result = original(handle, pointer)
            if result:
                ctypes.cast(pointer, ctypes.POINTER(c.Info)).contents.attributes |= 0x400
            return result
        with patch.object(self.api.k, "GetFileInformationByHandle", reparse):
            receipt = self.acquire()
        self.assertEqual(receipt["creationAttempts"], [])
        self.assertIn("reparse", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_wrong_final_path_refused_before_creation(self):
        original = self.api.k.GetFinalPathNameByHandleW
        def wrong(handle, buffer, size, flags):
            result = original(handle, buffer, size, flags)
            buffer.value = "\\\\?\\C:\\wrong"
            return result
        with patch.object(self.api.k, "GetFinalPathNameByHandleW", wrong):
            receipt = self.acquire()
        self.assertEqual(receipt["creationAttempts"], [])
        self.assertIn("final path mismatch", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_collective_file_identity_change_holds(self):
        original = self.api.snapshot
        counts = {}
        def changed(handle, path, directory):
            result = original(handle, path, directory)
            if not directory:
                counts[handle] = counts.get(handle, 0) + 1
                if counts[handle] == 3:
                    result["fileIndex"] = "999"
            return result
        with patch.object(self.api, "snapshot", changed):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertIn("collective validation", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_close_failure_prevents_success_attempts_remaining_closes(self):
        original = self.api.close
        calls = []
        def failed(handle):
            original(handle)  # actually close, then inject uncertain API receipt
            calls.append(handle)
            if len(calls) == 1:
                raise OSError("injected CloseHandle receipt failure")
        with patch.object(self.api, "close", failed):
            receipt = self.acquire()
        self.assertEqual(len(calls), receipt["handlesOpened"])
        self.assertFalse(receipt["allHandlesClosed"])
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertFalse(receipt["original_run_ready"])

    def test_short_write_progress_loop(self):
        original = self.api.k.WriteFile
        counts = []
        def short(handle, data, count, output, overlapped):
            counts.append(count)
            return original(handle, data, min(count, 7), output, overlapped)
        with patch.object(self.api.k, "WriteFile", short):
            receipt = self.acquire()
        self.assertGreater(len(counts), 3)
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY", receipt["errors"])
        self.check_closed(receipt)

    def test_zero_write_failure(self):
        def zero(handle, data, count, output, overlapped):
            ctypes.cast(output, ctypes.POINTER(c.w.DWORD)).contents.value = 0
            return 1
        with patch.object(self.api.k, "WriteFile", zero):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertIn("no progress", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_invalid_manifest_escape_rejected(self):
        self.entries = [{"kind": "file", "path": "../escape", "bytes": "0", "sha256": "0" * 64}]
        receipt = self.acquire()
        self.assertEqual(receipt["handlesOpened"], 0)
        self.assertEqual(receipt["creationAttempts"], [])
        self.check_closed(receipt)

    def test_path_100_plus_rejected_before_creation(self):
        self.entries = [{"kind": "file", "path": "x" * 99, "bytes": "0", "sha256": "0" * 64}]
        receipt = self.acquire()
        self.assertEqual(receipt["handlesOpened"], 0)
        self.assertIn("shorter than 100", receipt["errors"][0]["message"])
        self.check_closed(receipt)

    def test_exact_command_gate_rejects_changes(self):
        encoded = c.gate.encode_utf16_units(str(self.clone / c.gate.PROJECT_NAME))
        argv = [str(c.gate.EXECUTABLE), "-m", "open", "-f", encoded]
        for bad_encoded, bad_cwd, bad_argv in (
            (encoded.upper(), self.clone, argv),
            (encoded, self.parent, argv),
            (encoded, self.clone, argv + ["extra"]),
        ):
            with self.assertRaises(ValueError):
                c._command_gate(self.source, self.clone, bad_encoded, bad_cwd, bad_argv, False)

    def test_os_symlink_rejected_or_explicit_privilege_skip(self):
        link = self.source / "link"
        try:
            link.symlink_to(self.source / "D", target_is_directory=True)
        except OSError as exc:
            if getattr(exc, "winerror", None) == 1314:
                self.skipTest("OS symlink fixture requires privilege: WinError 1314")
            raise
        receipt = self.acquire()
        self.assertEqual(receipt["creationAttempts"], [])
        self.assertIn("reparse", receipt["errors"][0]["message"])
        self.check_closed(receipt)


def main():
    started = time.monotonic()
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(AtomicCloneTests)
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    receipt = {"schema": "cp289-atomic-clone-tests-v1", "status": "HOLD",
        "original_run_ready": False,
        "collectorSha256": hashlib.sha256(Path(c.__file__).read_bytes()).hexdigest(),
        "testSourceSha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        "testsRun": result.testsRun, "passed": result.testsRun - len(result.failures) - len(result.errors) - len(result.skipped),
        "failures": len(result.failures), "errors": len(result.errors),
        "skips": [{"test": str(test), "reason": reason} for test, reason in result.skipped],
        "seconds": round(time.monotonic() - started, 6),
        "realProblemCloneCreated": False, "originalApplicationRun": False,
        "qualification": "native Windows disposable tests only; not B98 production acquisition"}
    output = c.ROOT / "analysis/cp289-atomic-clone-tests.json"
    output.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(receipt))
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    raise SystemExit(main())
