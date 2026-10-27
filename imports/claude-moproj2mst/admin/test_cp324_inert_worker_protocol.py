import sys
from pathlib import Path
import unittest

sys.path.insert(0, str(Path(__file__).parent))
import cp313_worker_binary_protocol as wire
import cp324_inert_worker_protocol as protocol


class Transport:
    cp324_exact_frame_transport = True

    def __init__(self, result=None, *, short=False, trailing=b""):
        self.result = result
        self.short = short
        self.trailing = trailing
        self.writes = []

    def write_exact(self, frame, deadline):
        self.writes.append(frame)

    def read_exact(self, size, deadline):
        return self.result[:-1] if self.short else self.result

    def assert_no_queued_bytes(self, deadline):
        if self.trailing:
            raise protocol.Hold("queued trailing bytes")


class CoordinatorTests(unittest.TestCase):
    def setup(self, result=None, short=False, now=0):
        nonce = b"n" * 32
        frame = result or wire.encode_result(wire.WorkerTelemetry(0,0,0,0,0,0,0,0,0,nonce))
        t = Transport(frame, short=short)
        calls = []
        def create(deadline):
            calls.append(("create", deadline))
            return 0x1234
        return nonce, t, calls, lambda: now, create

    def run_it(self, t, calls, clock, create, exit_result=True, nonce=b"n"*32, deadline=10):
        return protocol.coordinate(deadline=deadline, creator=create, transport=t,
                                   wait_worker_exit=lambda d: calls.append(("exit", d)) or exit_result,
                                   clock=clock, nonce_factory=lambda n: nonce)

    def test_success_zero_result_and_ack_only_confirmed_by_exit(self):
        nonce,t,calls,clock,create=self.setup()
        result=self.run_it(t,calls,clock,create)
        self.assertEqual(len(t.writes[0]),48)
        self.assertEqual(len(t.writes[1]),48)
        self.assertEqual(wire.decode_init(t.writes[0]).child_job,0x1234)
        self.assertEqual(result["ack_outcome"],"worker-exit-observed")

    def test_rejects_short_result_and_skips_ack(self):
        _,t,calls,clock,create=self.setup(short=True)
        with self.assertRaises(protocol.Hold): self.run_it(t,calls,clock,create)
        self.assertEqual(len(t.writes),1)

    def test_rejects_nonzero_telemetry(self):
        nonce=b"n"*32
        raw=wire.encode_result(wire.WorkerTelemetry(1,0,0,0,0,0,0,0,0,nonce))
        _,t,calls,clock,create=self.setup(raw)
        with self.assertRaises(protocol.Hold): self.run_it(t,calls,clock,create)
        self.assertEqual(len(t.writes),1)

    def test_ack_does_not_confirm_without_exact_exit(self):
        _,t,calls,clock,create=self.setup()
        with self.assertRaises(protocol.Hold): self.run_it(t,calls,clock,create,exit_result=False)
        self.assertEqual(len(t.writes),2)

    def test_deadline_is_not_reset_after_creator(self):
        nonce,t,calls,_,create=self.setup()
        ticks=iter((0,11))
        with self.assertRaises(protocol.Hold):
            protocol.coordinate(deadline=10,creator=create,transport=t,wait_worker_exit=lambda d:True,
                               clock=lambda:next(ticks),nonce_factory=lambda n:nonce)
        self.assertEqual(len(t.writes),0)

    def test_rejects_duplicate_or_stale_nonce(self):
        raw=wire.encode_result(wire.WorkerTelemetry(0,0,0,0,0,0,0,0,0,b"x"*32))
        _,t,calls,clock,create=self.setup(raw)
        with self.assertRaises(protocol.Hold): self.run_it(t,calls,clock,create)
        self.assertEqual(len(t.writes),1)

    def test_rejects_queued_duplicate_result_before_ack(self):
        nonce=b"n"*32
        frame=wire.encode_result(wire.WorkerTelemetry(0,0,0,0,0,0,0,0,0,nonce))
        t=Transport(frame,trailing=frame)
        calls=[]
        create=lambda deadline:0x1234
        with self.assertRaises(protocol.Hold): self.run_it(t,calls,lambda:0,create)
        self.assertEqual(len(t.writes),1)


if __name__ == "__main__": unittest.main()
