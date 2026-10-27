"""CP307 in-memory WORKER job ABI, order, gate and ownership tests."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp307_worker_job as m
import cp308_real_job_protocol as downstream


class FakeApi:
    cp307_fake_api = True

    def __init__(self, fail=None):
        self.fail = fail
        self.calls = []

    def last_error(self):
        return 307

    def CreateJobObjectW(self, security, name):
        self.calls.append(("CreateJobObjectW", security, name))
        return 0 if self.fail == "job" else 71

    def GetHandleInformation(self, handle, flags):
        self.calls.append(("GetHandleInformation", handle))
        C.cast(flags, C.POINTER(W.DWORD))[0] = m.abi.HANDLE_FLAG_INHERIT if self.fail == "inherit" else 0
        return int(self.fail != "handle-info")

    def SetInformationJobObject(self, handle, kind, limits, size):
        value = C.cast(limits, C.POINTER(m.abi.JOBOBJECT_EXTENDED_LIMIT_INFORMATION)).contents
        self.calls.append(("SetInformationJobObject", handle, kind, size,
                           value.BasicLimitInformation.LimitFlags))
        return int(self.fail != "set-job")

    def InitializeProcThreadAttributeList(self, ptr, count, flags, size):
        self.calls.append(("InitializeProcThreadAttributeList", bool(ptr), count, flags))
        if not ptr:
            C.cast(size, C.POINTER(C.c_size_t))[0] = 0 if self.fail == "size" else 128
            return 0
        return int(self.fail != "initialize")

    def UpdateProcThreadAttribute(self, ptr, flags, key, value, length, previous, returned):
        self.calls.append(("UpdateProcThreadAttribute", flags, key,
                           C.cast(value, C.POINTER(W.HANDLE))[0], length, previous, returned))
        return int(self.fail != "update")

    def DeleteProcThreadAttributeList(self, ptr):
        self.calls.append(("DeleteProcThreadAttributeList", bool(ptr)))

    def CreateProcessW(self, exe, command, process_attrs, thread_attrs, inherit,
                       flags, environment, cwd, startup, process):
        si = C.cast(startup, C.POINTER(m.abi.STARTUPINFOEXW)).contents
        self.calls.append(("CreateProcessW", exe, command.value, process_attrs,
                           thread_attrs, inherit, flags, environment, cwd,
                           si.StartupInfo.cb, bool(si.lpAttributeList)))
        pi = C.cast(process, C.POINTER(m.abi.PROCESS_INFORMATION)).contents
        if self.fail in ("create-partial", "create-raise"):
            pi.hProcess = 101
            if self.fail == "create-raise":
                raise RuntimeError("creator interrupted")
            return 0
        pi.hProcess, pi.hThread = 101, 102
        pi.dwProcessId, pi.dwThreadId = 103, 104
        return 1

    def IsProcessInJob(self, process, job, output):
        self.calls.append(("IsProcessInJob", process, job))
        C.cast(output, C.POINTER(W.BOOL))[0] = int(self.fail != "not-member")
        return int(self.fail != "membership-query")

    def TerminateJobObject(self, job, code):
        self.calls.append(("TerminateJobObject", job, code))
        return int(self.fail != "terminate")

    def WaitForSingleObject(self, process, millis):
        self.calls.append(("WaitForSingleObject", process, millis))
        return 0x102 if self.fail == "wait" else 0

    def QueryInformationJobObject(self, job):
        self.calls.append(("QueryInformationJobObject", job))
        return 1 if self.fail == "active" else 0

    def CloseHandle(self, handle):
        self.calls.append(("CloseHandle", handle))
        return int(self.fail != "close" and not (self.fail == "close-job" and handle == 71))


class WorkerJobTests(unittest.TestCase):
    @staticmethod
    def gated_worker(api=None):
        api = api or FakeApi()
        worker = m.FakeWorkerJob(api)
        worker.stage_job()
        worker.create_suspended_worker()
        worker.verify_worker_gate()
        return api, worker

    def test_transfer_disarms_exact_worker_handles_once(self):
        class Receiver:
            worker_job = worker_process = worker_thread = None

            def bind_created(self, **handles):
                self.__dict__.update(handles)

        api, worker = self.gated_worker()
        receiver = Receiver()
        worker.transfer_to(receiver, child_job=201)
        self.assertEqual((receiver.worker_job, receiver.worker_process,
                          receiver.worker_thread, receiver.child_job), (71, 101, 102, 201))
        self.assertEqual(worker.report.retained_handles, {})
        self.assertEqual(worker.report.phase, "transferred")
        self.assertEqual(worker.report.worker_gate, "closed")
        with self.assertRaisesRegex(RuntimeError, "transferred once"):
            worker.transfer_to(Receiver(), child_job=201)
        worker.cleanup()
        self.assertNotIn("CloseHandle", [call[0] for call in api.calls])

    def test_transfer_to_actual_cp308_fake_owner(self):
        class SharedApi(FakeApi):
            cp308_fake_api = True

            def GetHandleInformation(self, handle, flags=None):
                if flags is None:
                    self.calls.append(("GetHandleInformation", handle))
                    return 0
                return super().GetHandleInformation(handle, flags)

            def GetJobLimitFlags(self, handle):
                self.calls.append(("GetJobLimitFlags", handle))
                return downstream.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE

            def IsProcessInJob(self, process, job, output=None):
                if output is None:
                    self.calls.append(("IsProcessInJob", process, job))
                    return job == 71
                return super().IsProcessInJob(process, job, output)

        class Transport:
            cp308_fake_transport = True

        api, worker = self.gated_worker(SharedApi())
        owner = downstream.FakeRealJobProtocol(
            api, Transport(), occurrence=downstream.Occurrence("run", "child"),
            worker_id="worker", fixed_clone_image=r"C:\PROBLEM\m289_0123456789abcdef\fixture.exe",
            fixture_sha256=downstream.CP302_SHA256)
        worker.transfer_to(owner, child_job=201)
        self.assertEqual(owner.report.phase, "worker-gate-checked")
        self.assertEqual(owner.report.retained,
                         {"worker_job": 71, "child_job": 201,
                          "worker_process": 101, "worker_thread": 102})
        self.assertEqual(worker.report.retained_handles, {})

    def test_transfer_requires_gate_and_empty_receiver(self):
        class Receiver:
            worker_job = worker_process = worker_thread = None

            def bind_created(self, **handles):
                self.__dict__.update(handles)

        api = FakeApi()
        worker = m.FakeWorkerJob(api)
        worker.stage_job()
        worker.create_suspended_worker()
        with self.assertRaisesRegex(RuntimeError, "gated"):
            worker.transfer_to(Receiver(), child_job=201)
        worker.verify_worker_gate()
        receiver = Receiver()
        receiver.worker_job = 999
        with self.assertRaisesRegex(ValueError, "empty CP308 receiver"):
            worker.transfer_to(receiver, child_job=201)
        self.assertEqual(worker.report.retained_handles,
                         {"worker_job": 71, "worker_process": 101, "worker_thread": 102})
        worker.transfer_to(Receiver(), child_job=201)

    def test_transfer_exception_disarms_only_adopted_handles(self):
        for taken in ((), ("worker_job",), ("worker_job", "worker_process"),
                      ("worker_job", "worker_process", "worker_thread")):
            with self.subTest(taken=taken):
                class Receiver:
                    worker_job = worker_process = worker_thread = None

                    def bind_created(self, **handles):
                        for name in taken:
                            setattr(self, name, handles[name])
                        raise RuntimeError("receiver failed")

                api, worker = self.gated_worker()
                receiver = Receiver()
                with self.assertRaisesRegex(RuntimeError, "receiver failed"):
                    worker.transfer_to(receiver, child_job=201)
                expected = {name: value for name, value in (
                    ("worker_job", 71), ("worker_process", 101),
                    ("worker_thread", 102)) if name not in taken}
                self.assertEqual(worker.report.retained_handles, expected)
                for name in taken:
                    self.assertEqual(getattr(receiver, name),
                                     {"worker_job": 71, "worker_process": 101,
                                      "worker_thread": 102}[name])
                with self.assertRaisesRegex(RuntimeError, "transferred once"):
                    worker.transfer_to(Receiver(), child_job=201)
                worker.cleanup()
                self.assertTrue(all(call[1] not in [
                    {"worker_job": 71, "worker_process": 101,
                     "worker_thread": 102}[name] for name in taken]
                    for call in api.calls if call[0] == "CloseHandle"))

    def test_transfer_cleanup_retry_after_receiver_takes_no_handles(self):
        class Receiver:
            worker_job = worker_process = worker_thread = None

            def bind_created(self, **_handles):
                raise RuntimeError("bind failed")

        api, worker = self.gated_worker(FakeApi("wait"))
        with self.assertRaisesRegex(RuntimeError, "bind failed"):
            worker.transfer_to(Receiver(), child_job=201)
        self.assertEqual(worker.cleanup().retained_handles,
                         {"worker_job": 71, "worker_process": 101, "worker_thread": 102})
        api.fail = None
        self.assertEqual(worker.cleanup().retained_handles, {})
        self.assertEqual([call[1] for call in api.calls if call[0] == "CloseHandle"],
                         [102, 101, 71])

    def test_abi_and_creation_order(self):
        api = FakeApi()
        job = m.FakeWorkerJob(api)
        self.assertEqual(job.stage_job(), 71)
        pi = job.create_suspended_worker()
        self.assertEqual((pi.hProcess, pi.hThread), (101, 102))
        self.assertEqual([c[0] for c in api.calls], [
            "CreateJobObjectW", "GetHandleInformation", "SetInformationJobObject",
            "InitializeProcThreadAttributeList", "InitializeProcThreadAttributeList",
            "UpdateProcThreadAttribute", "CreateProcessW", "DeleteProcThreadAttributeList"])
        self.assertEqual(api.calls[0], ("CreateJobObjectW", None, None))
        self.assertEqual(api.calls[2], ("SetInformationJobObject", 71, 9,
            C.sizeof(m.abi.JOBOBJECT_EXTENDED_LIMIT_INFORMATION), 0x2000))
        self.assertEqual(api.calls[5], ("UpdateProcThreadAttribute", 0, 0x2000D,
            71, C.sizeof(W.HANDLE), None, None))
        self.assertEqual(api.calls[6][1:9], (m.FAKE_WORKER_IMAGE,
            '"' + m.FAKE_WORKER_IMAGE + '"', None, None, False,
            m.abi.CREATE_SUSPENDED | m.abi.EXTENDED_STARTUPINFO_PRESENT | m.abi.CREATE_NO_WINDOW,
            None, m.FAKE_WORKER_CWD))
        self.assertEqual(api.calls[6][9], C.sizeof(m.abi.STARTUPINFOEXW))
        self.assertTrue(api.calls[6][10])
        self.assertEqual(job.report.worker_gate, "closed")
        self.assertTrue(job.verify_worker_gate())
        self.assertEqual(api.calls[-1], ("IsProcessInJob", 101, 71))
        self.assertEqual(job.report.phase, "gate-open")
        self.assertEqual(job.report.retained_handles,
                         {"worker_job": 71, "worker_process": 101, "worker_thread": 102})

    def test_setup_and_attribute_failures_never_create_worker(self):
        for failure in ("handle-info", "inherit", "set-job"):
            with self.subTest(failure=failure):
                api = FakeApi(failure)
                job = m.FakeWorkerJob(api)
                with self.assertRaises((RuntimeError, m.abi.ApiFailure)):
                    job.stage_job()
                self.assertEqual(api.calls[-1], ("CloseHandle", 71))
                self.assertEqual(job.report.retained_handles, {})
        for failure in ("size", "initialize", "update"):
            with self.subTest(failure=failure):
                api = FakeApi(failure)
                job = m.FakeWorkerJob(api)
                job.stage_job()
                with self.assertRaises(m.abi.ApiFailure):
                    job.create_suspended_worker()
                self.assertNotIn("CreateProcessW", [c[0] for c in api.calls])
                self.assertEqual(job.report.retained_handles, {"worker_job": 71})
                self.assertEqual("DeleteProcThreadAttributeList" in [c[0] for c in api.calls],
                                 failure == "update")

    def test_gate_closed_on_nonmembership_or_failed_query(self):
        for failure in ("not-member", "membership-query"):
            api = FakeApi(failure)
            job = m.FakeWorkerJob(api)
            job.stage_job()
            job.create_suspended_worker()
            with self.assertRaises((RuntimeError, m.abi.ApiFailure)):
                job.verify_worker_gate()
            self.assertEqual(job.report.worker_gate, "closed")

    def test_partial_or_interrupted_create_retains_exact_job_and_partial_handle(self):
        for failure in ("create-partial", "create-raise"):
            api = FakeApi(failure)
            job = m.FakeWorkerJob(api)
            job.stage_job()
            with self.assertRaises(BaseException):
                job.create_suspended_worker()
            self.assertEqual(job.report.worker_gate, "closed")
            self.assertEqual(job.report.retained_handles,
                             {"worker_job": 71, "worker_process": 101})
            self.assertEqual(api.calls[-1], ("DeleteProcThreadAttributeList", True))

    def test_cleanup_exact_handles_and_uncertain_close(self):
        api = FakeApi()
        job = m.FakeWorkerJob(api)
        job.stage_job()
        job.create_suspended_worker()
        job.verify_worker_gate()
        report = job.cleanup()
        self.assertEqual(report.retained_handles, {})
        self.assertEqual(report.worker_gate, "closed")
        self.assertEqual([c[0] for c in api.calls[-6:]], [
            "TerminateJobObject", "WaitForSingleObject", "QueryInformationJobObject",
            "CloseHandle", "CloseHandle", "CloseHandle"])
        self.assertEqual([c[1] for c in api.calls[-3:]], [102, 101, 71])
        api = FakeApi("close-job")
        job = m.FakeWorkerJob(api)
        job.stage_job()
        job.create_suspended_worker()
        report = job.cleanup()
        self.assertEqual(report.retained_handles, {"worker_job": 71})
        self.assertEqual(report.close_results["worker_job"], "failed or unknown")
        self.assertEqual(report.phase, "cleanup-uncertain")

    def test_uncertain_wait_or_job_accounting_retains_container(self):
        for failure in ("wait", "active"):
            with self.subTest(failure=failure):
                api = FakeApi(failure)
                job = m.FakeWorkerJob(api)
                job.stage_job()
                job.create_suspended_worker()
                report = job.cleanup()
                self.assertIn("worker_job", report.retained_handles)
                if failure == "wait":
                    self.assertIn("worker_process", report.retained_handles)
                    self.assertIn("worker_thread", report.retained_handles)
                self.assertNotIn(("CloseHandle", 71), api.calls)

    def test_live_entry_hold(self):
        with self.assertRaisesRegex(RuntimeError, "HOLD"):
            m.FakeWorkerJob(object())
        with self.assertRaisesRegex(RuntimeError, "HOLD"):
            m.create_real_worker()

    def test_marked_forwarding_wrapper_cannot_bind_live_createprocess(self):
        # Construct without initializing Kernel32Api; no WinAPI function is called.
        live = object.__new__(m.abi.Kernel32Api)

        class ForwardingFake(FakeApi):
            cp307_fake_api = True

        api = ForwardingFake()
        api.CreateProcessW = live.CreateProcessW
        with self.assertRaisesRegex(RuntimeError, "bound live CreateProcessW HOLD"):
            m.FakeWorkerJob(api)
        self.assertEqual(api.calls, [])


if __name__ == "__main__":
    unittest.main()
