"""CP307 private preflight contract tests. These tests never launch fixture.exe."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import color_clone_manifest_gate as gate
import cp307_fixed_clone_preflight as m


ROOT = r"C:\PROBLEM\m289_0123456789abcdef"
IMAGE = ROOT + r"\fixture.exe"
SIZE = m.FIXTURE.stat().st_size


def identity(path, index="42", size=SIZE):
    return {"finalPath": "\\\\?\\" + path, "volumeSerialNumber": 7,
            "fileIndex": index, "creationFileTime": "123", "size": size,
            "attributes": 32}


def retained_receipt():
    project = ROOT + "\\" + gate.PROJECT_NAME
    encoded = gate.encode_utf16_units(project)
    copied_id = identity(IMAGE)
    return {
        "schema": "cp289-creation-bound-clone-v1", "production": False,
        "sourceStableAcrossAcquisition": True, "manifestsMatchPinnedEntries": True,
        "acquisitionOutcome": "ACQUIRED_FRESH_CLONE_STATIC_ONLY",
        "cloneRoot": ROOT, "createdCloneIdentity": identity(ROOT, "11", 0),
        "cloneFinalIdentity": identity(ROOT, "11", 0),
        "commandGate": {"exactUtf16ArgvCwd": True},
        "launchContract": {"cwd": ROOT, "projectPath": project,
                           "encodedPath": encoded,
                           "argv": [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]},
        "copiedFiles": [{"path": "fixture.exe", "bytes": SIZE,
                         "sha256": m.CP302_SHA256,
                         "sourceIdentity": identity(r"C:\PROBLEM\s302\fixture.exe", "10"),
                         "destinationIdentity": copied_id}],
    }


class FakeFileApi:
    cp307_fake_file_api = True

    def __init__(self, *, mutate_post=False, data=None, open_none=False,
                 open_error=None, snapshot_error=None, read_error=None,
                 close_error=None, file_index="42"):
        self.data = m.FIXTURE.read_bytes() if data is None else data
        self.mutate_post = mutate_post
        self.open_none = open_none
        self.open_error = open_error
        self.snapshot_error = snapshot_error
        self.read_error = read_error
        self.close_error = close_error
        self.file_index = file_index
        self.calls = []

    def open_read(self, path):
        self.calls.append(("open", path))
        if self.open_error:
            raise OSError(self.open_error)
        if self.open_none:
            return None
        return object()

    def snapshot(self, handle, path):
        self.calls.append(("snapshot", path))
        if self.snapshot_error and self.calls.count(("snapshot", path)) == 1:
            raise OSError(self.snapshot_error)
        index = "42" if not self.mutate_post or self.calls.count(("snapshot", path)) == 1 else "43"
        return identity(path, index if self.file_index == "42" else self.file_index)

    def read_all(self, handle):
        self.calls.append(("read",))
        if self.read_error:
            raise OSError(self.read_error)
        return self.data

    def close(self, handle):
        self.calls.append(("close",))
        if self.close_error:
            raise OSError(self.close_error)


class PreflightTests(unittest.TestCase):
    def test_exact_fixed_fixture_contract_and_no_launch_callback(self):
        api = FakeFileApi()
        result = m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(result["image"], IMAGE)
        self.assertEqual(result["cwd"], ROOT)
        self.assertEqual(result["argv"], [IMAGE])
        self.assertEqual(result["fixtureSha256"], m.CP302_SHA256)
        self.assertEqual(result["measuredLoadedBytes"], "unknown; no process was created or inspected")
        self.assertIn("no process or GUI calls", result["cp307OwnedCalls"])
        self.assertIn("arbitrary side effects", result["operationScope"])
        self.assertIn("does not prove handles remain live", result["receiptQualification"])
        self.assertEqual(result["handleClose"], "fake API close returned successfully")
        self.assertNotIn("launch", [call[0] for call in api.calls])

    def test_wrong_original_launch_contract_argv_refuses_before_file_open(self):
        receipt = retained_receipt()
        receipt["launchContract"]["argv"] = [IMAGE]
        api = FakeFileApi()
        with self.assertRaisesRegex(ValueError, "launchContract"):
            m.preflight_retained(receipt, file_api=api)
        self.assertEqual(api.calls, [])

    def test_wrong_paths_and_retained_digest_refuse(self):
        for mutate in (
            lambda r: r.update(cloneRoot=r"C:\PROBLEM\elsewhere"),
            lambda r: r["copiedFiles"][0].update(sha256="0" * 64),
        ):
            receipt = retained_receipt()
            mutate(receipt)
            api = FakeFileApi()
            with self.assertRaises(ValueError):
                m.preflight_retained(receipt, file_api=api)
            self.assertEqual(api.calls, [])

    def test_measured_digest_mismatch_refuses_and_closes_handle(self):
        api = FakeFileApi(data=b"wrong fixture")
        with self.assertRaisesRegex(ValueError, "measured clone fixture bytes"):
            m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(api.calls[-1], ("close",))

    def test_pre_post_handle_identity_mismatch_refuses_and_closes(self):
        api = FakeFileApi(mutate_post=True)
        with self.assertRaisesRegex(ValueError, "pre/post handle-backed"):
            m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(api.calls[-1], ("close",))

    def test_retained_destination_and_measured_handle_identity_must_match(self):
        api = FakeFileApi(file_index="different")
        with self.assertRaisesRegex(ValueError, "retained CP289 destination"):
            m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(api.calls[-1], ("close",))

    def test_open_none_or_exception_fails_before_snapshot(self):
        for api, message in ((FakeFileApi(open_none=True), "no handle"),
                             (FakeFileApi(open_error="open failed"), "open failed")):
            with self.assertRaisesRegex((OSError, RuntimeError), message):
                m.preflight_retained(retained_receipt(), file_api=api)
            self.assertEqual([call[0] for call in api.calls], ["open"])

    def test_snapshot_or_read_exception_still_attempts_close(self):
        for api, message in ((FakeFileApi(snapshot_error="snapshot failed"), "snapshot failed"),
                             (FakeFileApi(read_error="read failed"), "read failed")):
            with self.assertRaisesRegex(OSError, message):
                m.preflight_retained(retained_receipt(), file_api=api)
            self.assertEqual(api.calls[-1], ("close",))

    def test_close_failure_does_not_mask_primary_failure_and_is_reported(self):
        api = FakeFileApi(data=b"wrong", close_error="close failed")
        with self.assertRaisesRegex(RuntimeError, "measured clone fixture bytes.*close is uncertain"):
            m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(api.calls[-1], ("close",))

    def test_close_failure_after_success_is_reported_as_uncertain(self):
        api = FakeFileApi(close_error="close failed")
        with self.assertRaisesRegex(RuntimeError, "close is uncertain"):
            m.preflight_retained(retained_receipt(), file_api=api)
        self.assertEqual(api.calls[-1], ("close",))

    def test_non_fake_api_and_real_entry_hold(self):
        with self.assertRaisesRegex(RuntimeError, "real file/process"):
            m.preflight_retained(retained_receipt(), file_api=object())
        with self.assertRaisesRegex(RuntimeError, "HOLD"):
            m.acquire_real()


if __name__ == "__main__":
    unittest.main()
