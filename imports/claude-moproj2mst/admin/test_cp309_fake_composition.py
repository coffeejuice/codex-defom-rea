"""CP309 end-to-end in-memory composition; no executable or GUI is launched."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp309_fake_composition as m
import test_cp307_fixed_clone_preflight as fixture


class FakeApi:
    cp303_fake_api = cp307_fake_api = cp308_fake_api = True

    def __init__(self, fault=None):
        self.fault = fault
        self.calls = []
        self.jobs = 0

    def last_error(self):
        return 309

    def CreateJobObjectW(self, security, name):
        self.jobs += 1
        value = 10 if self.jobs == 1 else 20
        self.calls.append(("CreateJobObjectW", value))
        return value

    def GetHandleInformation(self, handle, flags=None):
        self.calls.append(("GetHandleInformation", handle))
        if flags is not None:
            C.cast(flags, C.POINTER(W.DWORD))[0] = 0
            return True
        return 0

    def SetInformationJobObject(self, handle, kind, limits, size):
        self.calls.append(("SetInformationJobObject", handle))
        return not (self.fault == "child-job-stage" and handle == 20)

    def GetJobLimitFlags(self, handle):
        self.calls.append(("GetJobLimitFlags", handle))
        return 0x2000

    def InitializeProcThreadAttributeList(self, ptr, count, flags, size):
        self.calls.append(("InitializeProcThreadAttributeList", bool(ptr)))
        if not ptr:
            C.cast(size, C.POINTER(C.c_size_t))[0] = 128
            return False
        return True

    def UpdateProcThreadAttribute(self, ptr, flags, key, value, length, previous, returned):
        job = int(C.cast(value, C.POINTER(W.HANDLE))[0])
        self.calls.append(("UpdateProcThreadAttribute", job))
        return True

    def DeleteProcThreadAttributeList(self, ptr):
        self.calls.append(("DeleteProcThreadAttributeList",))
        if self.fault == "delete-after-child" and ("CreateProcessW", fixture.IMAGE) in self.calls:
            raise OSError("attribute deletion lost returned PROCESS_INFORMATION")

    def CreateProcessW(self, exe, command, process_attrs, thread_attrs, inherit,
                       flags, environment, cwd, startup, process):
        self.calls.append(("CreateProcessW", exe))
        pi = C.cast(process, C.POINTER(m.child_creation.PROCESS_INFORMATION)).contents
        if exe == m.worker_job.FAKE_WORKER_IMAGE:
            pi.hProcess, pi.hThread = 30, 31
            pi.dwProcessId, pi.dwThreadId = 301, 302
        else:
            if self.fault == "child-create-unknown":
                raise OSError("creator interrupted")
            pi.hProcess, pi.hThread = 40, 41
            pi.dwProcessId, pi.dwThreadId = 400, 401
        return True

    def IsProcessInJob(self, process, job, output=None):
        self.calls.append(("IsProcessInJob", process, job))
        member = (process == 30 and job == 10) or (process == 50 and job == 20)
        if self.fault == "worker-not-member" and process == 30:
            member = False
        if self.fault == "cp308-worker-not-member" and process == 30 and output is None:
            member = False
        if self.fault == "child-not-member" and process == 50:
            member = False
        if output is not None:
            C.cast(output, C.POINTER(W.BOOL))[0] = int(member)
            return True
        return member

    def GetCurrentProcess(self):
        return 1

    def DuplicateHandle(self, source, handle, target, output, access, inherit, options):
        self.calls.append(("DuplicateHandle", source, handle, target))
        value = 60 if handle == 20 else 50
        C.cast(output, C.POINTER(W.HANDLE))[0] = value
        if handle == 20 and self.fault == "duplicate-child-raises":
            raise OSError("duplicate raised after writing remote handle")
        if handle == 20 and self.fault == "duplicate-child-false":
            return False
        return True

    def GetProcessId(self, handle):
        self.calls.append(("GetProcessId", handle))
        return 400

    def GetProcessTimes(self, handle):
        self.calls.append(("GetProcessTimes", handle))
        return 456

    def QueryFullProcessImageNameW(self, handle):
        self.calls.append(("QueryFullProcessImageNameW", handle))
        return fixture.IMAGE

    def TerminateJobObject(self, handle, code):
        self.calls.append(("TerminateJobObject", handle))
        return True

    def WaitForSingleObject(self, handle, millis):
        self.calls.append(("WaitForSingleObject", handle))
        return 0x102 if self.fault == "worker-timeout" and handle == 30 else 0

    def QueryInformationJobObject(self, handle):
        self.calls.append(("QueryInformationJobObject", handle))
        return 0

    def CloseHandle(self, handle):
        self.calls.append(("CloseHandle", handle))
        return True


class FakeTransport:
    cp308_fake_transport = True

    def __init__(self, fault=None):
        self.fault = fault
        self.sent = []

    def send_ack(self, ack):
        self.sent.append(ack)
        if self.fault == "ack-unknown":
            raise OSError("delivery unknown")


def simulated_acquire(receipt, *, close_ok=True):
    def acquire(_source, _parent, _entries, *, api, name, production, on_retained):
        assert api.cp307_fake_clone_api and production is False
        assert name == "m289_0123456789abcdef"
        receipt["handlesOpened"] = 3
        try:
            receipt["retainedHandoff"] = on_retained(receipt)
            if not receipt["retainedHandoff"]["completed"]:
                raise ValueError("retained handoff did not verify completion")
        except Exception as error:
            receipt["errors"] = [{"phase": "retainedHandoff", "message": str(error)}]
        receipt["handlesClosed"] = 3 if close_ok else 2
        receipt["allHandlesClosed"] = close_ok
        receipt["acquisitionOutcome"] = (
            "ACQUIRED_FRESH_CLONE_STATIC_ONLY" if close_ok and not receipt.get("errors")
            else "INCOMPLETE")
        receipt["finishedUtc"] = "after callback and close"
        return receipt
    return acquire


def coordinator(*, api=None, file_api=None, transport=None, occurrence_id="occ-1"):
    api = api or FakeApi()
    return m.FakeComposition(
        clone_api=fixture.FakeCloneApi(), file_api=file_api or fixture.FakeFileApi(),
        api=api, transport=transport or FakeTransport(), run_id="run-309",
        occurrence_id=occurrence_id, child_id="child-1", worker_id="worker-1",
        supervisor_id="supervisor-1", offer_id="offer-1"), api


def run_with_receipt(obj, receipt, *, close_ok=True):
    with patch.object(m.clone, "_acquire", simulated_acquire(receipt, close_ok=close_ok)):
        return obj.acquire_for_fake_test("source", "parent", [],
                                         name="m289_0123456789abcdef")


class CompositionTests(unittest.TestCase):
    def receipt(self):
        receipt = fixture.retained_receipt()
        receipt["acquisitionOutcome"] = "INCOMPLETE"
        receipt["allHandlesClosed"] = False
        receipt["finishedUtc"] = None
        return receipt

    def test_one_child_creation_order_join_and_single_cleanup_owner(self):
        obj, api = coordinator()
        receipt, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "FAKE_COMPOSITION_AND_CP289_CLOSE_COMPLETE")
        self.assertEqual(receipt["retainedHandoff"]["schema"], "cp309-fake-composition-v1")
        self.assertEqual(result.cp307_phase, "VERIFIED")
        self.assertEqual(result.cp308_phase_before_cleanup, "fake-receipt-claim-matched")
        self.assertEqual(result.cleanup_phase, "cleanup-confirmed")
        self.assertEqual(result.worker_local_close,
                         {"child_thread": "closed by fake verified WORKER exit",
                          "child_process": "closed by fake verified WORKER exit",
                          "child_job_duplicate": "closed by fake verified WORKER exit"})
        self.assertTrue(result.fake_worker_claim_only)
        self.assertIn("no resume, original or GUI calls",
                      receipt["retainedHandoff"]["cp309OwnedOperations"])
        names = [call[0] for call in api.calls]
        child_create = api.calls.index(("CreateProcessW", fixture.IMAGE))
        self.assertLess(api.calls.index(("IsProcessInJob", 30, 10)), child_create)
        self.assertLess(api.calls.index(("UpdateProcThreadAttribute", 60)), child_create)
        self.assertEqual(names.count("CreateProcessW"), 2)  # one worker, one child
        self.assertEqual([call[1] for call in api.calls if call[0] == "CloseHandle"],
                         [50, 31, 30, 20, 10])
        self.assertFalse({40, 41, 60} &
                         {call[1] for call in api.calls if call[0] == "CloseHandle"})
        self.assertNotIn("ResumeThread", names)
        self.assertNotIn("OpenProcess", names)
        with self.assertRaisesRegex(RuntimeError, "one-use"):
            run_with_receipt(obj, self.receipt())

    def test_final_cp289_close_failure_never_joins(self):
        obj, api = coordinator()
        _, result = run_with_receipt(obj, self.receipt(), close_ok=False)
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual([call[1] for call in api.calls if call[0] == "CloseHandle"],
                         [50, 31, 30, 20, 10])

    def test_cp308_occurrence_mapping_drops_cp307_occurrence_id(self):
        first, _ = coordinator(occurrence_id="occ-1")
        second, _ = coordinator(occurrence_id="occ-2")
        _, first_result = run_with_receipt(first, self.receipt())
        _, second_result = run_with_receipt(second, self.receipt())
        self.assertEqual(first_result.outcome, "FAKE_COMPOSITION_AND_CP289_CLOSE_COMPLETE")
        self.assertEqual(second_result.outcome, "FAKE_COMPOSITION_AND_CP289_CLOSE_COMPLETE")
        self.assertNotEqual(first_result.cp307_occurrence, second_result.cp307_occurrence)
        self.assertEqual(first_result.cp308_occurrence, second_result.cp308_occurrence)
        self.assertIn("native replay/isolation HOLD", first_result.occurrence_mapping_scope)
        self.assertEqual(first_result.cp308_occurrence, ("run-309", "child-1"))

    def test_preflight_failure_prevents_all_job_and_process_calls(self):
        obj, api = coordinator(file_api=fixture.FakeFileApi(data=b"wrong"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(api.calls, [])

    def test_worker_gate_failure_prevents_child_and_pretransfer_cleanup(self):
        obj, api = coordinator(api=FakeApi("worker-not-member"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual([c[0] for c in api.calls].count("CreateProcessW"), 1)
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"], [31, 30, 10])

    def test_creator_interruption_remains_unknown_and_cp308_retains_jobs(self):
        obj, api = coordinator(api=FakeApi("child-create-unknown"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertIn("unknown", obj.owner.report.create_outcome)
        self.assertEqual(result.worker_local_close["child_job_duplicate"],
                         "closed by fake verified WORKER exit")
        self.assertIn("possible_child_process_thread", result.worker_local_close)
        self.assertEqual(set(result.retained_handles), {"child_job", "worker_job"})
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"], [31, 30])
        self.assertEqual(obj.worker.report.retained_handles, {})

    def test_child_job_setup_failure_closes_pretransfer_owner_once(self):
        obj, api = coordinator(api=FakeApi("child-job-stage"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertIsNone(obj.owner)
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"],
                         [20, 31, 30, 10])
        self.assertEqual(len([c for c in api.calls if c[0] == "CreateProcessW"]), 1)

    def test_cp308_bind_hold_owns_all_transferred_handles_once(self):
        obj, api = coordinator(api=FakeApi("cp308-worker-not-member"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(obj.worker.report.retained_handles, {})
        self.assertEqual(obj.owner.report.phase, "cleanup-confirmed")
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"],
                         [31, 30, 20, 10])
        self.assertNotIn(("DuplicateHandle", 1, 20, 30), api.calls)

    def test_transfer_raises_before_bind_closes_original_owners_once(self):
        obj, api = coordinator()
        with patch.object(m.worker_job.FakeWorkerJob, "transfer_to",
                          side_effect=RuntimeError("transfer refused")):
            _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(obj.owner.report.retained, {})
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"],
                         [31, 30, 10, 20])

    def test_partial_transfer_without_receiver_report_reconciles_owner_fields(self):
        obj, api = coordinator()

        def partial_bind(receiver, **handles):
            receiver.worker_job = handles["worker_job"]
            receiver.child_job = handles["child_job"]
            receiver.worker_process = handles["worker_process"]
            self.assertEqual(receiver.report.retained, {})
            raise RuntimeError("bind failed after prefix adoption")

        with patch.object(m.owner_protocol.FakeRealJobProtocol, "bind_created", partial_bind):
            _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(result.cleanup_phase, "cleanup-confirmed")
        self.assertEqual(result.retained_handles, {"worker_thread": 31})
        self.assertEqual(obj.worker.report.retained_handles, {"worker_thread": 31})
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"],
                         [30, 20, 10])

    def test_single_adopted_worker_job_without_report_remains_explicitly_retained(self):
        obj, api = coordinator()

        def one_handle_bind(receiver, **handles):
            receiver.worker_job = handles["worker_job"]
            self.assertEqual(receiver.report.retained, {})
            raise RuntimeError("bind failed after first handle")

        with patch.object(m.owner_protocol.FakeRealJobProtocol, "bind_created", one_handle_bind):
            _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(result.retained_handles, {"worker_job": 10})
        self.assertEqual(result.cleanup_phase, "cleanup-uncertain")
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"],
                         [31, 30, 20])

    def test_duplicate_output_loss_retains_both_jobs_and_marks_unknown(self):
        for fault in ("duplicate-child-false", "duplicate-child-raises"):
            with self.subTest(fault=fault):
                obj, api = coordinator(api=FakeApi(fault))
                receipt, result = run_with_receipt(obj, self.receipt())
                self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
                self.assertEqual(result.worker_local_child_job_duplicate_outcome,
                                 "unknown; CP303 helper did not return")
                self.assertEqual(result.worker_local_child_handle_outcome, "not-attempted")
                self.assertEqual(set(result.retained_handles), {"child_job", "worker_job"})
                self.assertEqual(receipt["retainedHandoff"]["workerLocalChildJobDuplicateOutcome"],
                                 "unknown; CP303 helper did not return")
                self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"], [31, 30])
                self.assertEqual([c[0] for c in api.calls].count("CreateProcessW"), 1)

    def test_create_output_loss_retains_jobs_and_does_not_claim_exact_child(self):
        obj, api = coordinator(api=FakeApi("delete-after-child"))
        receipt, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertEqual(result.worker_local_child_handle_outcome,
                         "unknown; CP303 helper did not return")
        self.assertIn("possible CHILD process/thread handles and existence",
                      result.uncertain_worker_local)
        self.assertEqual(set(result.retained_handles), {"child_job", "worker_job"})
        self.assertIn("unknown", obj.owner.report.create_outcome)
        self.assertEqual(obj.owner.report.child_wait, "not-attempted")
        self.assertEqual(receipt["retainedHandoff"]["workerLocalChildHandleOutcome"],
                         "unknown; CP303 helper did not return")
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"], [31, 30])

    def test_uncertain_worker_exit_retains_worker_local_obligations(self):
        obj, api = coordinator(api=FakeApi("worker-timeout"))
        _, result = run_with_receipt(obj, self.receipt())
        self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
        self.assertIn("worker_local_child_job_duplicate", result.retained_handles)
        self.assertIn("worker_local_child_process", result.retained_handles)
        self.assertIn("worker_local_child_thread", result.retained_handles)
        self.assertIn("worker_job", result.retained_handles)
        self.assertEqual([c[1] for c in api.calls if c[0] == "CloseHandle"], [50])

    def test_ack_uncertainty_and_membership_failure_never_claim_completion(self):
        for api, transport in ((FakeApi("child-not-member"), FakeTransport()),
                               (FakeApi(), FakeTransport("ack-unknown"))):
            with self.subTest(fault=api.fault or transport.fault):
                obj, _ = coordinator(api=api, transport=transport)
                _, result = run_with_receipt(obj, self.receipt())
                self.assertEqual(result.outcome, "INCOMPLETE_OR_UNCERTAIN")
                self.assertNotEqual(result.cp308_phase_before_cleanup,
                                    "fake-receipt-claim-matched")
                self.assertEqual(obj.worker.report.retained_handles, {})
                self.assertEqual(len([c for c in api.calls if c[0] == "CloseHandle" and c[1] == 20]), 1)

    def test_direct_callback_and_real_entry_hold(self):
        obj, api = coordinator()
        with self.assertRaisesRegex(RuntimeError, "activation"):
            obj._callback(self.receipt(), object())
        self.assertEqual(api.calls, [])
        with self.assertRaisesRegex(RuntimeError, "HARD HOLD"):
            m.run_real()
        with self.assertRaisesRegex(RuntimeError, "HARD HOLD"):
            m.FakeComposition(clone_api=object(), file_api=object(), api=object(),
                              transport=object(), run_id="r", occurrence_id="o",
                              child_id="c", worker_id="w", supervisor_id="s", offer_id="f")


if __name__ == "__main__":
    unittest.main()
