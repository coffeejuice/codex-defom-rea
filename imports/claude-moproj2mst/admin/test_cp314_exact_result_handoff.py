"""In-memory CP314 exact RESULT handoff tests; no process or job is created."""
import unittest

import cp310_native_job_adapter as observed
import cp313_native_worker_creator as creator
import cp313_worker_binary_protocol as wire
import cp314_exact_result_handoff as handoff


NONCE = bytes(range(32))
INIT = wire.Init(300, NONCE)
RESULT = wire.WorkerTelemetry(1, 1, 0, 444, 445, 1, 0, 1, 350, NONCE)


def prepared():
    return creator.Receipt(
        phase="prepared-suspended",
        local={"worker_process": 100, "worker_thread": 101,
               "worker_job": 200, "child_job": 201,
               "input_write": 202, "output_read": 203},
        remote_child_job=creator.RemoteHandle(300, 100),
        pid=111, tid=112, create_outcome="returned-success",
        duplicate_outcome="returned-success")


class BoundedTransport:
    cp314_bounded_fake_transport = True

    def __init__(self, incoming=None, *, chunk=9, read_failure=None, write_failure=None):
        self.incoming = bytearray(wire.encode_result(RESULT) if incoming is None else incoming)
        self.outgoing = bytearray()
        self.chunk = chunk
        self.read_failure, self.write_failure = read_failure, write_failure
        self.read_calls = self.write_calls = 0
        self.timeouts = []

    def read(self, size, timeout_ms):
        assert 0 < timeout_ms <= 5000 and 0 < size <= 80
        self.timeouts.append(timeout_ms)
        self.read_calls += 1
        if self.read_failure == self.read_calls:
            raise TimeoutError("read deadline")
        count = min(size, self.chunk, len(self.incoming))
        result = bytes(self.incoming[:count])
        del self.incoming[:count]
        return result

    def write(self, data, timeout_ms):
        assert 0 < timeout_ms <= 5000 and 0 < len(data) <= 48
        self.timeouts.append(timeout_ms)
        self.write_calls += 1
        if self.write_failure == self.write_calls:
            raise TimeoutError("write deadline")
        count = min(len(data), self.chunk)
        self.outgoing.extend(data[:count])
        return count


class FakeApi:
    cp314_fake_api = True

    def __init__(self, *, fail=None, partial=None, pid=444, created=123456,
                 image=handoff.CHILD_IMAGE, flags=0, worker_member=True,
                 child_member=True):
        self.fail, self.partial = fail, partial
        self.pid, self.created, self.image, self.flags = pid, created, image, flags
        self.worker_member, self.child_member = worker_member, child_member
        self.calls = []

    def _call(self, name):
        self.calls.append(name)
        if self.fail == name:
            if name == "DuplicateHandle" and self.partial is not None:
                raise handoff.DuplicationInterrupted(observed.LocalHandle(self.partial))
            raise OSError(name)

    def GetCurrentProcess(self):
        self._call("GetCurrentProcess")
        return -1

    def DuplicateHandle(self, source_process, source_handle, target_process):
        self._call("DuplicateHandle")
        assert source_process == observed.LocalHandle(100)
        assert source_handle == observed.RemoteHandle(350, 100)
        assert target_process == -1
        return observed.LocalHandle(400)

    def GetProcessId(self, process):
        self._call("GetProcessId")
        assert process == observed.LocalHandle(400)
        return self.pid

    def GetProcessTimes(self, process):
        self._call("GetProcessTimes")
        assert process == observed.LocalHandle(400)
        return self.created

    def QueryFullProcessImageNameW(self, process):
        self._call("QueryFullProcessImageNameW")
        assert process == observed.LocalHandle(400)
        return self.image

    def GetHandleInformation(self, process):
        self._call("GetHandleInformation")
        assert process == observed.LocalHandle(400)
        return self.flags

    def IsProcessInJob(self, process, job):
        self._call(f"IsProcessInJob:{job.value}")
        assert process == observed.LocalHandle(400)
        return self.worker_member if job.value == 200 else self.child_member


class HandoffTests(unittest.TestCase):
    def run_handoff(self, *, state=None, transport=None, api=None, init=INIT, clock=None,
                    started=99.0, deadline=104.0):
        state = prepared() if state is None else state
        transport = BoundedTransport() if transport is None else transport
        api = FakeApi() if api is None else api
        clock = (lambda: 100.0) if clock is None else clock
        transaction = handoff.ExactResultHandoff(
            state, init, transport, api, clock=clock,
            occurrence_started=started, occurrence_deadline=deadline)
        return transaction, state, transport, api

    def test_exact_success_owns_child_before_queries_and_ack_is_unconfirmed(self):
        transaction, state, channel, api = self.run_handoff()
        receipt = transaction.run()
        self.assertEqual(receipt.phase, "verified-ack-written")
        self.assertEqual(receipt.result, RESULT)
        self.assertEqual(receipt.child_process, observed.LocalHandle(400))
        self.assertEqual((receipt.child_pid, receipt.child_creation_filetime,
                          receipt.child_image), (444, 123456, handoff.CHILD_IMAGE))
        self.assertTrue(receipt.worker_member and receipt.child_member)
        self.assertTrue(receipt.identity_verified)
        self.assertTrue(receipt.uncertain)
        self.assertEqual(receipt.ack_outcome, "written-unconfirmed")
        self.assertEqual(bytes(channel.outgoing), wire.encode_ack(wire.Ack(350, NONCE)))
        self.assertEqual(len(channel.outgoing), 48)
        self.assertEqual(api.calls, ["GetCurrentProcess", "DuplicateHandle",
                                     "GetProcessId", "GetProcessTimes",
                                     "QueryFullProcessImageNameW", "GetHandleInformation",
                                     "IsProcessInJob:200", "IsProcessInJob:201"])
        self.assertEqual(state.local["worker_job"], 200)
        self.assertEqual(state.local["child_job"], 201)
        with self.assertRaises(RuntimeError):
            transaction.run()

    def test_missing_partial_and_timed_out_result_hold_without_ack(self):
        cases = (BoundedTransport(b""), BoundedTransport(wire.encode_result(RESULT)[:17]),
                 BoundedTransport(read_failure=2))
        for channel in cases:
            with self.subTest(channel=channel):
                transaction, state, _, api = self.run_handoff(transport=channel)
                with self.assertRaises(handoff.Hold) as raised:
                    transaction.run()
                receipt = raised.exception.receipt
                self.assertEqual(receipt.phase, "hold")
                self.assertTrue(receipt.uncertain)
                self.assertIsNone(receipt.child_process)
                self.assertEqual(receipt.ack_outcome, "not-attempted")
                self.assertEqual(channel.outgoing, b"")
                self.assertEqual(api.calls, [])
                self.assertEqual(state.local["worker_process"], 100)

    def test_bad_result_or_nonce_never_duplicates(self):
        bad = [wire.encode_result(RESULT)[:4] + b"\0\0\0\0" + wire.encode_result(RESULT)[8:],
               wire.encode_result(wire.WorkerTelemetry(1, 0, 5, 0, 0, 0, 0, 0, 0, NONCE)),
               wire.encode_result(wire.WorkerTelemetry(1, 1, 0, 444, 445, 1, 0, 1, 350,
                                                       b"z" * 32))]
        for raw in bad:
            with self.subTest(raw=raw[:12]):
                channel = BoundedTransport(raw)
                transaction, _, _, api = self.run_handoff(transport=channel)
                with self.assertRaises(handoff.Hold):
                    transaction.run()
                self.assertEqual(channel.outgoing, b"")
                self.assertEqual(api.calls, [])

    def test_duplicate_failure_retains_partial_local_handle_and_jobs(self):
        transaction, state, channel, _ = self.run_handoff(
            api=FakeApi(fail="DuplicateHandle", partial=401))
        with self.assertRaises(handoff.Hold) as raised:
            transaction.run()
        receipt = raised.exception.receipt
        self.assertEqual(receipt.child_process, observed.LocalHandle(401))
        self.assertEqual(receipt.phase, "hold")
        self.assertEqual(channel.outgoing, b"")
        self.assertEqual((state.local["worker_job"], state.local["child_job"]), (200, 201))

    def test_each_independent_identity_or_membership_failure_holds(self):
        scenarios = (dict(pid=443), dict(created=0), dict(image=r"C:\PROBLEM\other.exe"),
                     dict(flags=1), dict(worker_member=False), dict(child_member=False),
                     dict(fail="GetProcessTimes"), dict(fail="IsProcessInJob:200"))
        for scenario in scenarios:
            with self.subTest(scenario=scenario):
                transaction, state, channel, _ = self.run_handoff(api=FakeApi(**scenario))
                with self.assertRaises(handoff.Hold) as raised:
                    transaction.run()
                receipt = raised.exception.receipt
                self.assertEqual(receipt.child_process, observed.LocalHandle(400))
                self.assertTrue(receipt.uncertain)
                self.assertFalse(receipt.identity_verified)
                self.assertEqual(receipt.ack_outcome, "not-attempted")
                self.assertEqual(channel.outgoing, b"")
                self.assertIn("worker_job", state.local)

    def test_ack_write_interruption_retains_verified_child_without_acceptance(self):
        channel = BoundedTransport(write_failure=2)
        transaction, state, _, _ = self.run_handoff(transport=channel)
        with self.assertRaises(handoff.Hold) as raised:
            transaction.run()
        receipt = raised.exception.receipt
        self.assertTrue(receipt.identity_verified)
        self.assertTrue(receipt.uncertain)
        self.assertEqual(receipt.child_process, observed.LocalHandle(400))
        self.assertEqual(receipt.ack_outcome, "unknown: ACK write entered")
        self.assertGreater(len(channel.outgoing), 0)
        self.assertLess(len(channel.outgoing), 48)
        self.assertIn("child_job", state.local)

    def test_creator_interruption_and_wrong_init_keep_state_without_read(self):
        for mutate in (lambda s: setattr(s, "create_outcome", "unknown: CreateProcessW entered"),
                       lambda s: setattr(s, "remote_child_job", None)):
            state = prepared()
            mutate(state)
            transaction, _, channel, api = self.run_handoff(state=state)
            with self.assertRaises(handoff.Hold):
                transaction.run()
            self.assertEqual(channel.read_calls, 0)
            self.assertEqual(api.calls, [])
            self.assertIn("worker_process", state.local)
        transaction, _, channel, _ = self.run_handoff(init=wire.Init(301, NONCE))
        with self.assertRaises(handoff.Hold):
            transaction.run()
        self.assertEqual(channel.read_calls, 0)

    def test_published_remote_job_after_interrupted_cp313_duplicate_cannot_ack(self):
        state = prepared()
        state.phase = "hold"
        state.duplicate_outcome = "unknown: DuplicateHandle entered"
        # CP313 can publish this output pointer in finally even when the call
        # raises; its presence alone is never preparation success.
        self.assertEqual(state.remote_child_job, creator.RemoteHandle(300, 100))
        transaction, _, channel, api = self.run_handoff(state=state)
        with self.assertRaises(handoff.Hold) as raised:
            transaction.run()
        self.assertEqual(raised.exception.receipt.phase, "hold")
        self.assertEqual(channel.read_calls, 0)
        self.assertEqual(channel.outgoing, b"")
        self.assertEqual(api.calls, [])
        self.assertIn("worker_job", state.local)

    def test_creator_hold_after_duplicate_or_incomplete_pipes_cannot_read(self):
        variants = (
            lambda s: setattr(s, "phase", "hold"),
            lambda s: s.errors.append("post-duplicate failure"),
            lambda s: s.local.pop("output_read"),
            lambda s: s.local.update(input_read=204),
            lambda s: setattr(s, "worker_terminated", True),
        )
        for mutate in variants:
            state = prepared()
            mutate(state)
            transaction, _, channel, api = self.run_handoff(state=state)
            with self.assertRaises(handoff.Hold):
                transaction.run()
            self.assertEqual(channel.read_calls, 0)
            self.assertEqual(channel.outgoing, b"")
            self.assertEqual(api.calls, [])

    def test_precreation_deadline_consumes_elapsed_creator_time(self):
        channel = BoundedTransport()
        transaction, _, _, _ = self.run_handoff(
            transport=channel, clock=lambda: 103.5, started=99.0, deadline=104.0)
        transaction.run()
        self.assertTrue(channel.timeouts)
        self.assertTrue(all(0 < value <= 500 for value in channel.timeouts))
        channel = BoundedTransport()
        transaction, _, _, _ = self.run_handoff(
            transport=channel, clock=lambda: 104.01, started=99.0, deadline=104.0)
        with self.assertRaises(handoff.Hold) as raised:
            transaction.run()
        self.assertEqual(channel.read_calls, 0)
        self.assertTrue(raised.exception.receipt.uncertain)
        with self.assertRaises(TypeError):
            handoff.ExactResultHandoff(prepared(), INIT, BoundedTransport(), FakeApi())
        with self.assertRaises(ValueError):
            self.run_handoff(started=99.0, deadline=104.01)

    def test_hard_hold_and_fake_only_contract(self):
        self.assertEqual(handoff.NATIVE_ENTRY, "HARD HOLD")
        with self.assertRaises(RuntimeError):
            handoff.create_live_handoff()
        channel = BoundedTransport()
        channel.cp314_bounded_fake_transport = False
        with self.assertRaises(TypeError):
            self.run_handoff(transport=channel)
        api = FakeApi()
        api.cp314_fake_api = False
        with self.assertRaises(TypeError):
            self.run_handoff(api=api)


if __name__ == "__main__":
    unittest.main()
