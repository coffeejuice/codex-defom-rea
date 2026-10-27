"""CP306 in-memory clone and child join tests; never launch fixture.exe."""
from pathlib import Path
import json
import sys
import unittest
from unittest.mock import Mock, patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import color_atomic_clone_acquire as clone
import cp303_job_creation as creation
import cp306_clone_fixed_transaction as m
from test_cp300_clone_owned_child_handoff import FakeAPI as BaseCloneApi
from test_cp305_integrated_synthetic import FakeApi as BaseTransactionApi


SOURCE = Path(r"C:\PROBLEM\s302")
PARENT = Path(r"C:\PROBLEM")
NAME = "m289_0123456789abcdef"
ROOT = PARENT / NAME
IMAGE = str(ROOT / "fixture.exe")
SIZE = m.FIXTURE.stat().st_size
ENTRIES = [
    {"kind": "file", "path": clone.gate.PROJECT_NAME, "bytes": "1", "sha256": "2" * 64},
    {"kind": "file", "path": "fixture.exe", "bytes": str(SIZE), "sha256": m.CP302_SHA256},
]


class CloneApi(BaseCloneApi):
    cp306_fake_clone_api = True

    def open_source(self, path):
        return self._open(path, False, SIZE if Path(path).name == "fixture.exe" else 1)

    def copy(self, source, destination, expected_size):
        assert self.handles[source][2] == expected_size
        self.handles[destination][2] = expected_size
        return (m.CP302_SHA256 if expected_size == SIZE else "2" * 64), expected_size


class TransactionApi(BaseTransactionApi):
    def QueryFullProcessImageNameW(self, _handle):
        self.calls.append("image")
        return IMAGE


class JoinTests(unittest.TestCase):
    def setUp(self):
        self.clone_api = CloneApi()
        self.tx_api = TransactionApi()
        patches = [
            patch.object(clone.gate, "_check_components", return_value=None),
            patch.object(clone.gate, "directory_manifest", return_value={"entries": ENTRIES}),
            patch.object(clone, "_command_gate", return_value={"exactUtf16ArgvCwd": True}),
        ]
        for item in patches:
            item.start()
            self.addCleanup(item.stop)

    def adapter(self):
        return m.FakeCloneTransactionAdapter(
            clone_api=self.clone_api, transaction_api=self.tx_api,
            worker_factory=lambda tx: 42,
            send_ack=lambda ack: True, ack_receipt_factory=lambda ack: ack,
            occurrence="cp305-0123456789abcdef")

    def acquire(self, adapter=None, entries=ENTRIES):
        return (adapter or self.adapter()).acquire_for_fake_test(
            SOURCE, PARENT, entries, name=NAME)

    def test_one_cp305_child_under_retained_clone_handles(self):
        adapter = self.adapter()
        observed = []

        def worker(tx):
            observed.append((len(self.clone_api.handles), tx.image.executable,
                             tx.image.cwd, tx.image.sha256))
            return 42

        adapter.worker_factory = worker
        receipt = self.acquire(adapter)
        handoff = receipt["retainedHandoff"]
        self.assertEqual(receipt["acquisitionOutcome"], "ACQUIRED_FRESH_CLONE_STATIC_ONLY")
        self.assertEqual(receipt["cp306JoinedOutcome"], "FAKE_JOIN_COMPLETE_STATIC_ONLY")
        self.assertTrue(handoff["completed"])
        self.assertEqual(observed, [(6, IMAGE, str(ROOT), m.CP302_SHA256)])
        self.assertEqual(handoff["cloneIdentity"], receipt["cloneFinalIdentity"])
        self.assertEqual(handoff["clonedImageDestinationIdentity"],
                         receipt["copiedFiles"][1]["destinationIdentity"])
        self.assertEqual(handoff["childOffer"], {
            "occurrence": adapter.occurrence, "worker_handle": 202, "pid": 123,
            "tid": 124, "creation_filetime": 456, "image": IMAGE})
        self.assertEqual(handoff["childAck"]["image"], IMAGE)
        self.assertEqual(handoff["workerLocalProcessHandle"], 202)
        self.assertEqual(handoff["workerLocalReturnedHandles"], {"process": 202, "thread": 203})
        self.assertEqual(len([c for c in self.tx_api.calls if isinstance(c, tuple)
                              and c[0] == "create"]), 1)
        self.assertEqual(self.clone_api.opened, self.clone_api.closed)
        self.assertEqual(adapter.transaction.report["cleanup"].retained_handles, {})
        self.assertFalse(handoff["originalRun"] or handoff["resumed"])
        self.assertIn("supplied fake callbacks", handoff["operationScope"])

    def test_bad_pin_refuses_before_clone_or_child(self):
        entries = [dict(item) for item in ENTRIES]
        entries[1]["sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "exact pinned"):
            self.acquire(entries=entries)
        self.assertEqual(self.clone_api.opened, 0)
        self.assertEqual(self.tx_api.calls, [])

    def test_clone_identity_failure_prevents_child(self):
        adapter = self.adapter()
        original = m._retained_image

        def altered(receipt):
            receipt["cloneFinalIdentity"] = dict(receipt["cloneFinalIdentity"], fileIndex="other")
            return original(receipt)

        with patch.object(m, "_retained_image", side_effect=altered):
            receipt = self.acquire(adapter)
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["cp306JoinedOutcome"], "INCOMPLETE_OR_UNCERTAIN")
        self.assertFalse(receipt["retainedHandoff"]["completed"])
        self.assertIsNone(adapter.transaction)
        self.assertFalse(any(isinstance(c, tuple) and c[0] == "create"
                             for c in self.tx_api.calls))
        self.assertEqual(self.clone_api.opened, self.clone_api.closed)

    def test_interrupted_create_retains_unknown_outcome_and_exact_handles(self):
        self.tx_api.fail.add("create-exception")
        adapter = self.adapter()
        receipt = self.acquire(adapter)
        handoff = receipt["retainedHandoff"]
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["cp306JoinedOutcome"], "INCOMPLETE_OR_UNCERTAIN")
        self.assertFalse(handoff["completed"])
        self.assertIsNone(handoff["childOffer"])
        self.assertIn("unknown", adapter.transaction.supervisor.create_outcome)
        self.assertEqual(handoff["workerLocalProcessHandle"], None)
        self.assertEqual(handoff["workerLocalReturnedHandles"], {"process": 202, "thread": 203})
        self.assertEqual(adapter.transaction.report["retained_local_handles"], {})
        self.assertEqual(self.clone_api.opened, self.clone_api.closed)

    def test_failed_cleanup_remains_incomplete_and_retryable(self):
        self.tx_api.fail.add(203)
        adapter = self.adapter()
        receipt = self.acquire(adapter)
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["cp306JoinedOutcome"], "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(adapter.transaction.report["retained_local_handles"], {"thread": 203})
        self.tx_api.fail.remove(203)
        adapter.transaction.cleanup()
        self.assertEqual(adapter.transaction.report["retained_local_handles"], {})

    def test_fake_markers_and_real_entry_hold(self):
        self.clone_api.cp306_fake_clone_api = False
        with self.assertRaises(RuntimeError):
            self.adapter()
        with self.assertRaises(RuntimeError):
            m.acquire_real()

    def test_direct_retained_callback_refused_before_child_creation(self):
        adapter = self.adapter()
        with self.assertRaisesRegex(RuntimeError, "activation"):
            adapter.on_retained({"schema": "cp289-creation-bound-clone-v1"})
        with self.assertRaisesRegex(RuntimeError, "activation"):
            adapter._on_retained({"schema": "cp289-creation-bound-clone-v1"}, object())
        self.assertEqual(self.tx_api.calls, [])
        self.assertIsNone(adapter.transaction)

    def test_cp289_close_failure_cannot_complete_join(self):
        adapter = self.adapter()
        close = self.clone_api.close
        failed = False

        def fail_once(handle):
            nonlocal failed
            if not failed:
                failed = True
                raise OSError("uncertain CP289 close")
            return close(handle)

        self.clone_api.close = fail_once
        receipt = self.acquire(adapter)
        self.assertTrue(receipt["retainedHandoff"]["completed"])
        self.assertEqual(receipt["retainedHandoff"]["completionScope"],
                         "retained callback only; CP289 handle-close result follows")
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["cp306JoinedOutcome"], "INCOMPLETE_OR_UNCERTAIN")
        self.assertFalse(receipt["allHandlesClosed"])
        self.assertEqual(len([c for c in self.tx_api.calls if isinstance(c, tuple)
                              and c[0] == "create"]), 1)

    def test_ack_not_confirmed_cannot_complete_join(self):
        adapter = self.adapter()
        adapter.send_ack = lambda _ack: False
        receipt = self.acquire(adapter)
        self.assertEqual(receipt["acquisitionOutcome"], "INCOMPLETE")
        self.assertEqual(receipt["cp306JoinedOutcome"], "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(adapter.transaction.supervisor.ack_transmission, "not-confirmed")
        self.assertEqual(adapter.transaction.report["cleanup"].retained_handles, {})

    def test_fixture_source_and_selected_build_receipt_mutations_refuse_preclone(self):
        with patch.object(m, "FIXTURE", Mock(read_bytes=lambda: b"not reviewed")):
            with self.assertRaisesRegex(ValueError, "fixture bytes changed"):
                self.acquire()
        with patch.object(m, "SOURCE", Mock(read_bytes=lambda: b"not reviewed")):
            with self.assertRaisesRegex(ValueError, "source bytes changed"):
                self.acquire()
        receipt = json.loads(m.BUILD_RECEIPT.read_text(encoding="utf-8"))
        receipt["pe"]["machine"] = "I386"
        with patch.object(m, "BUILD_RECEIPT", Mock(read_bytes=lambda: json.dumps(receipt).encode())):
            with self.assertRaisesRegex(ValueError, "build receipt bytes changed"):
                self.acquire()
        receipt = json.loads(m.BUILD_RECEIPT.read_text(encoding="utf-8"))
        receipt["compiler"] = "unexpected compiler"
        with patch.object(m, "BUILD_RECEIPT", Mock(read_bytes=lambda: json.dumps(receipt).encode())):
            with self.assertRaisesRegex(ValueError, "build receipt bytes changed"):
                self.acquire()
        self.assertEqual(self.clone_api.opened, 0)
        self.assertEqual(self.tx_api.calls, [])


if __name__ == "__main__":
    unittest.main()
