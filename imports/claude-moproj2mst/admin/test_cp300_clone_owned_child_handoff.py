"""Fake API/owned-process contract tests. No executable or child is run."""
import hashlib
from pathlib import Path
import unittest
from unittest.mock import patch

import color_atomic_clone_acquire as clone
import cp300_clone_owned_child_handoff as cp300


DATA = b"x"
DIGEST = hashlib.sha256(DATA).hexdigest()
SOURCE = Path(r"C:\PROBLEM\s300")
PARENT = Path(r"C:\PROBLEM")
NAME = "m289_0123456789abcdef"
ROOT = PARENT / NAME
PIN = {"exe": str(SOURCE / "fixture.exe"), "sha256": DIGEST}
ENTRIES = [{"kind": "file", "path": clone.gate.PROJECT_NAME,
            "bytes": "1", "sha256": DIGEST},
           {"kind": "file", "path": "fixture.exe",
            "bytes": "1", "sha256": DIGEST}]


class FakeAPI:
    def __init__(self):
        self.handles = {}
        self.next_handle = 100
        self.opened = 0
        self.closed = 0

    def _open(self, path, directory, size=0):
        self.next_handle += 1
        self.handles[self.next_handle] = [str(path), directory, size]
        self.opened += 1
        return self.next_handle

    def open_directory(self, path):
        return self._open(path, True)

    def open_source(self, path):
        return self._open(path, False, 1)

    def create_relative(self, parent, name, directory):
        assert parent in self.handles
        path = Path(self.handles[parent][0]) / name
        handle = self._open(path, directory)
        return handle, {"ntstatusSigned": 0, "ioStatusSigned": 0,
                        "information": clone.FILE_CREATED}

    def snapshot(self, handle, path, directory):
        actual, kind, size = self.handles[handle]
        assert actual == str(path) and kind == directory
        return {"finalPath": "\\\\?\\" + actual, "volumeSerialNumber": 1,
                "fileIndex": str(handle), "creationFileTime": "77",
                "lastWriteFileTime": "77", "size": size,
                "attributes": 16 if directory else 128}

    def copy(self, source, destination, expected_size):
        assert self.handles[source][2] == expected_size == 1
        self.handles[destination][2] = 1
        return DIGEST, 1

    def close(self, handle):
        del self.handles[handle]
        self.closed += 1


class FakeOwnedProcess:
    instances = []
    status_override = None
    cleanup_override = None

    def __init__(self, exe, cwd, args, digest):
        self.api = CURRENT_API[0]
        self.live_at_creation = len(self.api.handles)
        self.resume_called = False
        self.close_called = 0
        self.identity = {"pid": 123, "primary_tid": 456,
                         "creation_filetime": 789,
                         "canonical_image": exe, "sha256": digest,
                         "cwd": cwd, "argv": [exe]}
        self.instances.append(self)

    def status(self):
        return self.status_override or {"state": "suspended", "wait_result": 258,
                                        "exit_verified": False, "exit_code": 259}

    def close(self):
        self.close_called += 1
        assert self.close_called == 1
        return self.cleanup_override or {"state": "exited", "wait_result": 0,
                                         "exit_verified": True,
                                         "termination_requested": True,
                                         "termination_attempts": [{"wait_result": 0,
                                                                   "exit_verified": True}]}

    def resume(self):
        self.resume_called = True
        raise AssertionError("resume forbidden")


CURRENT_API = [None]


class HandoffTests(unittest.TestCase):
    def setUp(self):
        self.api = FakeAPI()
        CURRENT_API[0] = self.api
        FakeOwnedProcess.instances = []
        FakeOwnedProcess.status_override = None
        FakeOwnedProcess.cleanup_override = None
        self.patches = [
            patch.object(clone.gate, "_check_components", return_value=None),
            patch.object(clone.gate, "directory_manifest",
                         return_value={"entries": ENTRIES}),
            patch.object(clone, "_command_gate",
                         return_value={"exactUtf16ArgvCwd": True, "status": "HOLD"}),
            patch.object(cp300, "_validate_pin", return_value=PIN),
        ]
        for item in self.patches:
            item.start()
            self.addCleanup(item.stop)

    def acquire(self):
        return cp300._acquire_synthetic_test(SOURCE, PARENT, ENTRIES, PIN,
                                       api=self.api, owned_factory=FakeOwnedProcess,
                                       name=NAME)

    def test_child_runs_at_retained_boundary_then_exact_cleanup(self):
        receipt = self.acquire()
        handoff = receipt["retainedHandoff"]
        child = FakeOwnedProcess.instances[0]
        self.assertEqual(child.live_at_creation, 6)
        self.assertEqual(child.close_called, 1)
        self.assertFalse(child.resume_called)
        self.assertTrue(handoff["completed"])
        self.assertEqual(handoff["childIdentity"]["creation_filetime"], 789)
        self.assertEqual(handoff["childIdentity"]["argv"], [str(ROOT / "fixture.exe")])
        self.assertEqual(handoff["childIdentity"]["canonical_image"], str(ROOT / "fixture.exe"))
        self.assertEqual(handoff["clonedImageDestinationIdentity"]["finalPath"],
                         "\\\\?\\" + str(ROOT / "fixture.exe"))
        self.assertEqual(handoff["childIdentity"]["sha256"],
                         next(file["sha256"] for file in receipt["copiedFiles"]
                              if file["path"] == "fixture.exe"))
        self.assertEqual(handoff["cloneIdentity"], receipt["cloneFinalIdentity"])
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY")
        self.assertTrue(receipt["allHandlesClosed"])
        self.assertEqual(self.api.opened, self.api.closed)

    def test_bad_suspended_state_refuses_and_closes(self):
        FakeOwnedProcess.status_override = {"state": "running", "wait_result": 258,
                                            "exit_verified": False}
        receipt = self.acquire()
        self.assertFalse(receipt["retainedHandoff"]["completed"])
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(FakeOwnedProcess.instances[0].close_called, 1)
        self.assertEqual(self.api.opened, self.api.closed)

    def test_cleanup_ambiguity_refuses(self):
        FakeOwnedProcess.cleanup_override = {"state": "exited", "wait_result": 258,
                                              "exit_verified": False,
                                              "termination_requested": True,
                                              "termination_attempts": []}
        receipt = self.acquire()
        self.assertFalse(receipt["retainedHandoff"]["completed"])
        self.assertEqual(receipt["retainedHandoff"]["cleanupCertainty"],
                         "uncertain_child_may_remain_live")
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(self.api.opened, self.api.closed)

    def test_constructor_failure_keeps_cleanup_uncertain(self):
        class ConstructorFailure:
            def __init__(self, *_args):
                raise RuntimeError("post-create cleanup unknown")
        receipt = cp300._acquire_synthetic_test(SOURCE, PARENT, ENTRIES, PIN,
                    api=self.api, owned_factory=ConstructorFailure, name=NAME)
        self.assertFalse(receipt["retainedHandoff"]["completed"])
        self.assertEqual(receipt["retainedHandoff"]["cleanupCertainty"], "unknown")
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(self.api.opened, self.api.closed)

    def test_manifest_mismatch_prevents_child(self):
        with patch.object(clone.gate, "directory_manifest",
                          return_value={"entries": []}):
            receipt = self.acquire()
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["creationAttempts"], [])
        self.assertEqual(FakeOwnedProcess.instances, [])
        self.assertEqual(self.api.opened, self.api.closed)

    def test_child_identity_mismatch_refuses_and_closes(self):
        class WrongIdentity(FakeOwnedProcess):
            def __init__(self, *args):
                super().__init__(*args)
                self.identity["argv"] = [str(ROOT / "fixture.exe"), "unexpected"]
        receipt = cp300._acquire_synthetic_test(SOURCE, PARENT, ENTRIES, PIN,
                    api=self.api, owned_factory=WrongIdentity, name=NAME)
        self.assertFalse(receipt["retainedHandoff"]["completed"])
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(WrongIdentity.instances[0].close_called, 1)
        self.assertEqual(self.api.opened, self.api.closed)

    def test_prior_clone_name_refused_before_creation(self):
        with self.assertRaises(ValueError):
            cp300._acquire_synthetic_test(SOURCE, PARENT, ENTRIES, PIN,
                                    api=self.api, owned_factory=FakeOwnedProcess,
                                    name="m289_999cfa585601449a")
        self.assertEqual(self.api.opened, 0)


class PinRefusalTests(unittest.TestCase):
    def test_original_digest_rejected_without_file_read(self):
        pin = {"exe": PIN["exe"], "sha256": cp300.ORIGINAL_EXE_SHA256}
        with self.assertRaises(ValueError):
            cp300._validate_pin(pin)

    def test_original_executable_path_rejected(self):
        pin = {"exe": str(clone.gate.EXECUTABLE), "sha256": "a" * 64}
        with self.assertRaises(ValueError):
            cp300._validate_pin(pin)


if __name__ == "__main__":
    unittest.main()
