"""CP312 in-memory topology rehearsal; these tests never open a process/job."""
import ctypes as C
from ctypes import wintypes as W
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp303_job_creation as abi
import cp310_native_job_adapter as observation
import cp311_frame_protocol as framing
import cp312_topology_harness as m


IMAGE = r"C:\PROBLEM\m289_0123456789abcdef\fixture.exe"
NONCE = "a" * 64


class FakeBackend:
    cp312_fake_backend = True

    def __init__(self, events):
        self.events = events
        self.jobs = iter((10, 20))
        self.members = {("worker", 10): True, ("worker", 20): False,
                        ("child", 10): True, ("child", 20): True}
        self.fail = None
        self.worker_request = None
        self.active = {10: 0, 20: 0}
        self.closed = []

    def _step(self, name, *values):
        self.events.append((name, *values))
        if self.fail == name:
            raise RuntimeError(name)

    def create_job(self):
        handle = observation.LocalHandle(next(self.jobs))
        self._step("create_job", handle.value)
        return handle

    def set_kill_on_close(self, job):
        self._step("set_kill_on_close", job.value)

    def handle_flags(self, handle):
        self._step("handle_flags", handle.value)
        return 0

    def job_limit_flags(self, handle):
        self._step("job_limit_flags", handle.value)
        return abi.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE

    def create_suspended_worker(self, request):
        self.worker_request = request
        self._step("create_worker", request.job.value)
        if self.fail == "partial_create_worker":
            raise m.CreationInterrupted(process=observation.LocalHandle(30),
                                        thread=observation.LocalHandle(31))
        return m.CreatedProcess(observation.LocalHandle(30),
                                observation.LocalHandle(31), 3000, 3001)

    def in_job(self, process, job):
        who = "worker" if process.value == 30 else "child"
        self._step("in_job", who, job.value)
        return self.members[(who, job.value)]

    def duplicate_to_worker(self, job, worker, *, inherit):
        self._step("duplicate_to_worker", job.value, worker.value, inherit)
        return observation.RemoteHandle(40, worker.value)

    def duplicate_from_worker(self, worker, remote, *, inherit):
        self._step("duplicate_from_worker", worker.value, remote.value, inherit)
        if self.fail == "partial_duplicate":
            raise m.DuplicationInterrupted(observation.LocalHandle(50))
        return observation.LocalHandle(50)

    def process_identity(self, process):
        self._step("process_identity", process.value)
        return m.Identity(5000, 123456789, IMAGE)

    def terminate_job(self, job, exit_code):
        self._step("terminate_job", job.value, exit_code)
        return True

    def wait(self, process, wait_ms):
        self._step("wait", process.value, wait_ms)
        if self.fail == "child_wait_false" and process.value == 50:
            return False  # bool aliases WAIT_OBJECT_0 numerically; never accept it.
        return (observation.WAIT_TIMEOUT if self.fail == "child_wait_timeout" and
                process.value == 50 else observation.WAIT_OBJECT_0)

    def active_count(self, job):
        self._step("active_count", job.value)
        return self.active[job.value]

    def close(self, handle):
        self._step("close", handle.value)
        self.closed.append(handle.value)
        return True


class FakeChannel:
    cp312_fake_channel = True

    def __init__(self, events):
        self.events = events
        self.queue = []
        self.fail = None
        self.command = None
        self.child_request = None

    def send(self, data):
        obj = framing.decode_frames(data)[0]
        self.events.append(("send", obj["kind"]))
        if self.fail == "send_" + obj["kind"].lower():
            raise RuntimeError("send interrupted")
        if obj["kind"] == "COMMAND":
            self.command = obj
            self.child_request = m.fake_worker_child_request(
                data, worker_process=observation.LocalHandle(30),
                expected_nonce=obj["nonce"])
            self.queue.append(framing.encode_frame({
                "kind": "OFFER", "nonce": obj["nonce"], "handle": "60",
                "pid": 5000, "creation_filetime": 123456789, "image": obj["image"]}))
        elif obj["kind"] == "ACK":
            self.queue.append(framing.encode_frame({
                "kind": "RECEIPT", "nonce": obj["nonce"], "handle": obj["handle"]}))

    def receive(self):
        self.events.append(("receive",))
        if self.fail == "receive":
            raise RuntimeError("receive interrupted")
        return self.queue.pop(0)


class TopologyHarnessTests(unittest.TestCase):
    def setUp(self):
        self.events = []
        self.backend = FakeBackend(self.events)
        self.channel = FakeChannel(self.events)
        self.harness = m.FakeTopologyHarness(
            self.backend, self.channel, child_image=IMAGE, nonce=NONCE)

    def test_fake_nested_topology_order_and_exact_cleanup(self):
        report = self.harness.run()
        self.assertEqual(report.phase, "fake-cleanup-confirmed")
        self.assertEqual(report.retained, {})
        self.assertEqual(report.remote_child_job, 40)
        self.assertEqual(report.membership, {
            "worker_in_worker": True, "worker_in_child": False,
            "child_in_worker": True, "child_in_child": True})
        self.assertEqual(report.waits, {"child_process": "exited",
                                        "worker_process": "exited"})
        self.assertEqual(report.active_counts, {"child_job": 0, "worker_job": 0})
        names = [event[0] for event in self.events]
        self.assertLess(names.index("create_job"), names.index("create_worker"))
        self.assertLess(names.index("create_worker"), names.index("duplicate_to_worker"))
        self.assertLess(names.index("duplicate_to_worker"), names.index("send"))
        self.assertLess(names.index("duplicate_from_worker"), names.index("process_identity"))
        self.assertLess(names.index("process_identity"), names.index("terminate_job"))
        self.assertEqual([event[1] for event in self.events
                          if event[0] == "terminate_job"], [20, 10])
        self.assertEqual([event[1] for event in self.events if event[0] == "wait"], [50, 30])
        self.assertEqual([event[1] for event in self.events
                          if event[0] == "active_count"], [20, 10])
        self.assertEqual(self.backend.closed, [50, 31, 30, 20, 10])
        self.assertEqual(self.channel.command, {
            "kind": "COMMAND", "nonce": NONCE, "child_job": "40",
            "image": IMAGE, "sha256": m.CP302_SHA256, "flags": m.CREATE_FLAGS,
            "inherit_handles": False, "attribute_count": 1})
        self.assertEqual(self.channel.child_request.job,
                         observation.RemoteHandle(40, 30))
        self.assertEqual(self.channel.child_request.image, IMAGE)
        self.assertEqual(self.channel.child_request.cwd,
                         r"C:\PROBLEM\m289_0123456789abcdef")
        self.assertEqual(self.channel.child_request.flags, m.CREATE_FLAGS)
        with self.assertRaises(m.TopologyHold):
            self.harness.stage()

    def test_worker_creation_interruption_retains_jobs_and_uncertain_outcome(self):
        self.backend.fail = "create_worker"
        report = self.harness.run()
        self.assertEqual(report.create_outcome["worker"],
                         "unknown: inside CreateProcessW")
        self.assertEqual(report.phase, "cleanup-uncertain")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertNotIn("send", [event[0] for event in self.events])
        self.assertEqual(self.backend.closed, [])

    def test_partial_creation_handles_are_waited_and_closed_but_jobs_retained(self):
        self.backend.fail = "partial_create_worker"
        report = self.harness.run()
        self.assertEqual(report.create_outcome["worker"],
                         "unknown: inside CreateProcessW")
        self.assertEqual(report.waits["worker_process"], "exited")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertEqual(self.backend.closed, [31, 30])

    def test_failed_nested_membership_never_acks_and_retains_jobs(self):
        self.backend.members[("child", 10)] = False
        report = self.harness.run()
        self.assertEqual(report.membership["child_in_worker"], False)
        self.assertEqual(report.create_outcome["child"],
                         "unknown: command delivery or CreateProcessW")
        self.assertNotIn(("send", "ACK"), self.events)
        self.assertEqual(report.phase, "cleanup-uncertain")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertEqual(report.waits["child_process"], "exited")

    def test_missing_offer_keeps_creation_uncertain_and_job_handles(self):
        self.channel.fail = "receive"
        report = self.harness.run()
        self.assertEqual(report.create_outcome["child"],
                         "unknown: command delivery or CreateProcessW")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertNotIn("child_process", report.waits)
        self.assertEqual(self.backend.closed, [31, 30])

    def test_command_send_interruption_keeps_possible_child_unknown(self):
        self.channel.fail = "send_command"
        report = self.harness.run()
        self.assertEqual(report.create_outcome["child"],
                         "unknown: command delivery or CreateProcessW")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertEqual(report.remote_child_job, 40)

    def test_nonzero_job_count_blocks_job_handle_close(self):
        self.backend.active[10] = 1
        report = self.harness.run()
        self.assertEqual(report.active_counts["worker_job"], 1)
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertEqual(report.closes["worker_job"],
                         "deferred; exact handle retained")

    def test_partial_child_duplicate_has_exact_wait_and_uncertain_job_retention(self):
        self.backend.fail = "partial_duplicate"
        report = self.harness.run()
        self.assertEqual(report.create_outcome["child"],
                         "unknown: command delivery or CreateProcessW")
        self.assertEqual(report.waits["child_process"], "exited")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20})
        self.assertEqual(self.backend.closed, [50, 31, 30])

    def test_unconfirmed_exact_child_wait_preserves_child_and_jobs(self):
        self.backend.fail = "child_wait_timeout"
        report = self.harness.run()
        self.assertEqual(report.waits["child_process"], "unknown")
        self.assertEqual(report.phase, "cleanup-uncertain")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20,
                                           "child_process": 50})
        self.assertNotIn(50, self.backend.closed)

    def test_false_wait_is_not_dword_wait_object_zero(self):
        self.backend.fail = "child_wait_false"
        report = self.harness.run()
        self.assertEqual(report.waits["child_process"], "unknown")
        self.assertEqual(report.phase, "cleanup-uncertain")
        self.assertEqual(report.retained, {"worker_job": 10, "child_job": 20,
                                           "child_process": 50})
        self.assertEqual(report.closes["child_process"],
                         "deferred; exact handle retained")

    def test_abi_intent_and_live_hold(self):
        self.assertEqual(C.sizeof(C.c_void_p), 8)
        self.assertEqual(C.sizeof(W.BOOL), 4)
        self.assertEqual(C.sizeof(abi.STARTUPINFOEXW), 112)
        self.assertEqual(C.sizeof(abi.PROCESS_INFORMATION), 24)
        self.assertEqual(abi.STARTUPINFOEXW.lpAttributeList.offset, 104)
        self.assertEqual(abi.PROC_THREAD_ATTRIBUTE_JOB_LIST, 0x0002000D)
        self.assertEqual(len(observation.SIGNATURES["IsProcessInJob"][0]), 3)
        self.assertEqual(len(observation.SIGNATURES["DuplicateHandle"][0]), 7)
        self.assertEqual(len(observation.SIGNATURES["WaitForSingleObject"][0]), 2)
        self.assertEqual(m.CREATE_FLAGS & 0x01000000, 0)  # no breakaway
        self.assertEqual(m.CREATE_FLAGS & abi.CREATE_SUSPENDED, abi.CREATE_SUSPENDED)
        self.assertEqual(m.CREATE_FLAGS & abi.EXTENDED_STARTUPINFO_PRESENT,
                         abi.EXTENDED_STARTUPINFO_PRESENT)
        request = m.CreationRequest(m.WORKER_IMAGE, m.WORKER_CWD,
                                    f'"{m.WORKER_IMAGE}"', observation.LocalHandle(10))
        self.assertFalse(request.inherit_handles)
        self.assertEqual(request.attribute_count, 1)
        with self.assertRaises(ValueError):
            m.CreationRequest(m.WORKER_IMAGE, m.WORKER_CWD,
                              f'"{m.WORKER_IMAGE}"', observation.LocalHandle(10),
                              flags=m.CREATE_FLAGS | 0x01000000)
        with self.assertRaises(m.TopologyHold):
            m.run_native_topology()
        self.assertEqual(m.PREPARED_WORKER_IMAGE, r"C:\PROBLEM\s312\worker.exe")
        self.assertEqual(m.PREPARED_CHILD_IMAGE, r"C:\PROBLEM\s302\fixture.exe")
        self.assertIn("CP312_INIT/RESULT/ACK", m.HARD_HOLD_DEPENDENCIES[1])
        self.assertIn("not worker.c packed", self.harness.report.scope)

    def test_fixed_path_digest_and_backend_gate(self):
        self.assertEqual(m.fixed_child_image(IMAGE, m.CP302_SHA256), IMAGE)
        for image, digest in ((IMAGE, "0" * 64),
                              (r"C:\Program Files\DEFORM\DEF_PRE_64.exe", m.CP302_SHA256),
                              (r"C:\PROBLEM\m289_0123456789abcdef\other.exe", m.CP302_SHA256)):
            with self.subTest(image=image, digest=digest), self.assertRaises(ValueError):
                m.fixed_child_image(image, digest)
        with self.assertRaises(m.TopologyHold):
            m.FakeTopologyHarness(object(), self.channel, child_image=IMAGE)


if __name__ == "__main__":
    unittest.main()
