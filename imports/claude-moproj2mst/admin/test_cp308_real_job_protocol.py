"""CP308 exact-handle handoff and conservative cleanup, using in-memory fakes."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp308_real_job_protocol as m


IMAGE = r"C:\PROBLEM\m289_0123456789abcdef\fixture.exe"
OCCURRENCE = m.Occurrence("run-308", "child-1")


class FakeApi:
    cp308_fake_api = True

    def __init__(self, fault=None):
        self.fault = fault
        self.calls = []

    def GetHandleInformation(self, handle):
        self.calls.append(("GetHandleInformation", handle))
        if self.fault == "flag-query-raises" and handle == 20:
            raise OSError("flag query failed")
        return m.HANDLE_FLAG_INHERIT if self.fault == "inheritable" and handle == 20 else 0

    def GetJobLimitFlags(self, handle):
        self.calls.append(("GetJobLimitFlags", handle))
        return 0 if self.fault == "missing-kill-on-close" and handle == 20 else 0x2000

    def IsProcessInJob(self, process, job):
        self.calls.append(("IsProcessInJob", process, job))
        if self.fault == "worker-membership-raises" and process == 30:
            raise OSError("worker membership query failed")
        if self.fault == "membership-raises" and process == 50:
            raise OSError("query failed")
        if process == 30:
            return job == 10 and self.fault != "worker-not-member"
        return job == 20 and self.fault != "child-not-member"

    def GetCurrentProcess(self):
        self.calls.append(("GetCurrentProcess",))
        return 1

    def DuplicateHandle(self, source, offered, target, output, access, inherit, options):
        self.calls.append(("DuplicateHandle", source, offered, target, access, inherit, options))
        if self.fault == "duplicate-raises":
            C.cast(output, C.POINTER(W.HANDLE))[0] = 50
            raise OSError("uncertain duplicate")
        if self.fault == "duplicate-fails":
            return False
        C.cast(output, C.POINTER(W.HANDLE))[0] = 50
        return True

    def GetProcessId(self, handle):
        self.calls.append(("GetProcessId", handle))
        return 999 if self.fault == "pid-mismatch" else 300

    def GetProcessTimes(self, handle):
        self.calls.append(("GetProcessTimes", handle))
        return 456

    def QueryFullProcessImageNameW(self, handle):
        self.calls.append(("QueryFullProcessImageNameW", handle))
        return r"C:\PROBLEM\m289_0123456789abcdef\other.exe" if self.fault == "image-mismatch" else IMAGE

    def TerminateJobObject(self, handle, code):
        self.calls.append(("TerminateJobObject", handle, code))
        if self.fault == "terminate-raises" and handle == 20:
            raise OSError("termination outcome unknown")
        return self.fault != "terminate-fails"

    def WaitForSingleObject(self, handle, wait_ms):
        self.calls.append(("WaitForSingleObject", handle, wait_ms))
        return 0x102 if ((self.fault == "wait-timeout" and handle == 50)
                          or (self.fault == "worker-timeout" and handle == 30)) else 0

    def QueryInformationJobObject(self, handle):
        self.calls.append(("QueryInformationJobObject", handle))
        return 1 if self.fault == "active-child" and handle == 20 else 0

    def CloseHandle(self, handle):
        self.calls.append(("CloseHandle", handle))
        return self.fault != "close-child-job" or handle != 20


class FakeTransport:
    cp308_fake_transport = True

    def __init__(self, fault=None):
        self.fault = fault
        self.sent = []

    def send_ack(self, ack):
        self.sent.append(ack)
        if self.fault == "ack-raises":
            raise OSError("delivery uncertain")


def protocol(api=None, transport=None):
    api = api or FakeApi()
    transport = transport or FakeTransport()
    obj = m.FakeRealJobProtocol(api, transport, occurrence=OCCURRENCE,
                                worker_id="worker-1", fixed_clone_image=IMAGE,
                                fixture_sha256=m.CP302_SHA256)
    return obj, api, transport


def offer(**overrides):
    fields = dict(occurrence=OCCURRENCE, offer_id="offer-1",
                  worker_local_process_handle=40, pid=300,
                  creation_filetime=456, image=IMAGE)
    fields.update(overrides)
    return m.Offer(**fields)


def bind(obj):
    obj.bind_created(worker_job=10, child_job=20, worker_process=30, worker_thread=31)


class RealJobProtocolTests(unittest.TestCase):
    def test_exact_handle_offer_ack_separate_worker_receipt(self):
        obj, api, transport = protocol()
        bind(obj)
        self.assertFalse(obj.fake_receipt_claim_matched)
        ack = obj.accept_offer(offer())
        self.assertEqual(api.calls[-1], ("IsProcessInJob", 50, 10))
        self.assertIn(("DuplicateHandle", 30, 40, 1, 0, False, 2), api.calls)
        self.assertNotIn(("OpenProcess", 300), api.calls)
        self.assertEqual(transport.sent, [ack])
        self.assertFalse(obj.fake_receipt_claim_matched)
        obj.receive_worker_receipt(m.WorkerReceipt(ack, "worker-1"))
        self.assertTrue(obj.fake_receipt_claim_matched)
        self.assertIn("caller-supplied", obj.report.worker_receipt)
        report = obj.cleanup()
        self.assertEqual(report.phase, "cleanup-confirmed")
        self.assertEqual(report.retained, {})
        self.assertEqual(report.job_active, {"child_job": 0, "worker_job": 0})
        self.assertEqual([call[1] for call in api.calls if call[0] == "CloseHandle"],
                         [50, 31, 30, 20, 10])

    def test_bad_worker_gate_and_bad_offer_never_duplicate(self):
        for fault in ("worker-not-member", "worker-membership-raises",
                      "inheritable", "flag-query-raises", "missing-kill-on-close"):
            with self.subTest(fault=fault):
                obj, api, _ = protocol(FakeApi(fault))
                with self.assertRaises(m.ProtocolHold):
                    bind(obj)
                self.assertEqual(set(obj.report.retained),
                                 {"worker_job", "child_job", "worker_process", "worker_thread"})
                self.assertFalse(any(call[0] == "DuplicateHandle" for call in api.calls))
                if fault == "worker-not-member":
                    self.assertIn("not in WORKER job", obj.report.errors[-1])
                if fault in ("worker-membership-raises", "flag-query-raises"):
                    self.assertIn("unknown", obj.report.errors[-1])
        for bad in (offer(occurrence=m.Occurrence("wrong", "child-1")),
                    offer(image=r"C:\PROBLEM\m289_0123456789abcdef\other.exe")):
            obj, api, _ = protocol()
            bind(obj)
            with self.assertRaises(m.ProtocolHold):
                obj.accept_offer(bad)
            self.assertFalse(any(call[0] == "DuplicateHandle" for call in api.calls))

    def test_identity_and_membership_fail_closed_after_duplicate(self):
        for fault in ("pid-mismatch", "image-mismatch", "child-not-member",
                      "membership-raises"):
            with self.subTest(fault=fault):
                obj, _, transport = protocol(FakeApi(fault))
                bind(obj)
                with self.assertRaises(m.ProtocolHold):
                    obj.accept_offer(offer())
                self.assertEqual(obj.report.retained["child_process"], 50)
                self.assertEqual(transport.sent, [])
                self.assertFalse(obj.fake_receipt_claim_matched)
                if fault == "child-not-member":
                    self.assertIn("membership mismatch", obj.report.errors[-1])
                if fault == "membership-raises":
                    self.assertIn("membership unknown", obj.report.errors[-1])

    def test_duplicate_or_ack_uncertainty_retains_handles_and_no_receipt(self):
        for fault in ("duplicate-raises", "duplicate-fails"):
            obj, _, transport = protocol(FakeApi(fault))
            bind(obj)
            with self.assertRaises(m.ProtocolHold):
                obj.accept_offer(offer())
            self.assertEqual("child_process" in obj.report.retained,
                             fault == "duplicate-raises")
            self.assertEqual(transport.sent, [])
            self.assertFalse(obj.fake_receipt_claim_matched)
        obj, _, transport = protocol(transport=FakeTransport("ack-raises"))
        bind(obj)
        with self.assertRaises(m.ProtocolHold):
            obj.accept_offer(offer())
        self.assertEqual(len(transport.sent), 1)
        self.assertEqual(obj.report.ack_delivery, "unknown: send attempted")

    def test_mismatched_duplicate_and_reordered_receipt_hold(self):
        obj, _, _ = protocol()
        bind(obj)
        with self.assertRaises(m.ProtocolHold):
            obj.receive_worker_receipt(m.WorkerReceipt(m.Ack(OCCURRENCE, "offer-1", 300, 456, IMAGE), "worker-1"))
        obj, _, _ = protocol()
        bind(obj)
        ack = obj.accept_offer(offer())
        with self.assertRaises(m.ProtocolHold):
            obj.receive_worker_receipt(m.WorkerReceipt(ack, "other-worker"))
        self.assertFalse(obj.fake_receipt_claim_matched)
        obj, _, _ = protocol()
        bind(obj)
        ack = obj.accept_offer(offer())
        obj.receive_worker_receipt(m.WorkerReceipt(ack, "worker-1"))
        with self.assertRaises(m.ProtocolHold):
            obj.receive_worker_receipt(m.WorkerReceipt(ack, "worker-1"))
        self.assertFalse(obj.fake_receipt_claim_matched)

    def test_creator_interruption_unknown_even_after_zero_active_cleanup(self):
        obj, _, _ = protocol()
        bind(obj)
        with self.assertRaisesRegex(m.ProtocolHold, "unknown"):
            obj.note_creator_interrupted()
        report = obj.cleanup()
        self.assertEqual(report.retained, {})
        self.assertIn("unknown", report.create_outcome)
        self.assertEqual(report.child_wait, "not-attempted")

    def test_uncertain_cleanup_keeps_exact_handles(self):
        for fault, retained in (("wait-timeout", "child_process"),
                                ("active-child", "child_job"),
                                ("close-child-job", "child_job")):
            with self.subTest(fault=fault):
                obj, _, _ = protocol(FakeApi(fault))
                bind(obj)
                obj.accept_offer(offer())
                report = obj.cleanup()
                self.assertEqual(report.phase, "cleanup-uncertain")
                self.assertIn(retained, report.retained)
                if fault == "wait-timeout":
                    self.assertEqual(report.job_active, {"child_job": 0, "worker_job": 0})
                    self.assertEqual(report.child_wait, "not-confirmed")
                    self.assertIn("child_job", report.retained)
                    self.assertIn("worker_job", report.retained)
                    self.assertNotIn(("CloseHandle", 20), obj.api.calls)
                    self.assertNotIn(("CloseHandle", 10), obj.api.calls)
        for fault in ("worker-timeout", "terminate-fails", "terminate-raises"):
            with self.subTest(fault=fault):
                obj, api, _ = protocol(FakeApi(fault))
                bind(obj)
                obj.accept_offer(offer())
                report = obj.cleanup()
                self.assertEqual(report.job_active, {"child_job": 0, "worker_job": 0})
                self.assertEqual(report.phase, "cleanup-uncertain")
                self.assertIn("child_job", report.retained)
                self.assertIn("worker_job", report.retained)
                self.assertNotIn(("CloseHandle", 20), api.calls)
                self.assertNotIn(("CloseHandle", 10), api.calls)
                if fault == "worker-timeout":
                    self.assertIn("worker_process", report.retained)
                    self.assertEqual(report.worker_wait, "not-confirmed")
        # A later exact wait and fresh zero counts may complete a retained job.
        api = FakeApi("worker-timeout")
        obj, _, _ = protocol(api)
        bind(obj)
        obj.accept_offer(offer())
        obj.cleanup()
        api.fault = None
        self.assertEqual(obj.cleanup().retained, {})
        api = FakeApi("wait-timeout")
        obj, _, _ = protocol(api)
        bind(obj)
        obj.accept_offer(offer())
        self.assertEqual(set(obj.cleanup().retained),
                         {"child_process", "child_job", "worker_job"})
        api.fault = None
        self.assertEqual(obj.cleanup().retained, {})
        api = FakeApi("close-child-job")
        obj, _, _ = protocol(api)
        bind(obj)
        obj.accept_offer(offer())
        self.assertEqual(set(obj.cleanup().retained), {"child_job"})
        api.fault = None
        self.assertEqual(obj.cleanup().retained, {})
        obj, _, _ = protocol()
        bind(obj)
        with self.assertRaises(ValueError):
            obj.cleanup(wait_ms=-1)
        self.assertEqual(obj.report.phase, "worker-gate-checked")

    def test_failure_cleanup_from_bind_duplicate_and_ack(self):
        cases = (("bind", FakeApi("worker-membership-raises"), FakeTransport()),
                 ("duplicate", FakeApi("duplicate-raises"), FakeTransport()),
                 ("ack", FakeApi(), FakeTransport("ack-raises")))
        for stage, api, transport in cases:
            with self.subTest(stage=stage):
                obj, _, _ = protocol(api, transport)
                if stage == "bind":
                    with self.assertRaises(m.ProtocolHold):
                        bind(obj)
                else:
                    bind(obj)
                    with self.assertRaises(m.ProtocolHold):
                        obj.accept_offer(offer())
                report = obj.cleanup()
                self.assertEqual(report.phase, "cleanup-confirmed")
                self.assertEqual(report.retained, {})
                self.assertEqual(report.worker_wait, "exited")
                self.assertEqual(report.job_active, {"child_job": 0, "worker_job": 0})
                if stage != "bind":
                    self.assertEqual(report.child_wait, "exited")
                self.assertFalse(obj.fake_receipt_claim_matched)

    def test_fixed_contract_and_real_entry_hold(self):
        for bad in ("C:\\Temp\\fixture.exe", r"C:\PROBLEM\m289_0123456789abcdef\other.exe"):
            with self.subTest(bad=bad):
                with self.assertRaises(ValueError):
                    m.FakeRealJobProtocol(FakeApi(), FakeTransport(), occurrence=OCCURRENCE,
                                          worker_id="worker-1", fixed_clone_image=bad,
                                          fixture_sha256=m.CP302_SHA256)
        with self.assertRaisesRegex(RuntimeError, "HARD HOLD"):
            m.FakeRealJobProtocol(object(), FakeTransport(), occurrence=OCCURRENCE,
                                  worker_id="worker-1", fixed_clone_image=IMAGE,
                                  fixture_sha256=m.CP302_SHA256)
        with self.assertRaisesRegex(RuntimeError, "HARD HOLD"):
            m.run_real_fixed_child()


if __name__ == "__main__":
    unittest.main()
