"""Adversarial fake-only CP315 deadline and framing tests."""
import importlib.util
from pathlib import Path
import sys
import unittest

HERE = Path(__file__).parent
for name, filename in (("cp313_worker_binary_protocol", "cp313_worker_binary_protocol.py"),
                      ("cp315_bounded_pipe_transport", "cp315_bounded_pipe_transport.py")):
    spec = importlib.util.spec_from_file_location(name, HERE / filename)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
wire, transport = sys.modules["cp313_worker_binary_protocol"], sys.modules["cp315_bounded_pipe_transport"]


class Clock:
    def __init__(self): self.now = 10.0
    def __call__(self): return self.now


class FakePipe:
    cp315_fake_pipe = True
    def __init__(self, incoming=b"", chunk=5, clock=None, fault=None):
        self.incoming = bytearray(incoming); self.outgoing = bytearray()
        self.chunk, self.clock, self.fault = chunk, clock, fault
        self.calls = []; self.n = 0
    def _call(self, op, budget):
        self.n += 1; self.calls.append((op, budget))
        if self.fault and self.fault[0] == self.n:
            action = self.fault[1]
            if action == "late": self.clock.now += budget / 1000 + .001
            elif action == "expire": self.clock.now += 1
            elif isinstance(action, BaseException): raise action
        # Successful I/O can consume time. The transport must spend one fixed
        # transfer deadline across all fragments, not renew it per call.
        if self.clock is not None and op in ("read", "write"):
            self.clock.now += .002
    def peek(self, budget):
        self._call("peek", budget)
        return self.chunk if self.incoming else 0
    def read(self, size, budget):
        self._call("read", budget)
        part = bytes(self.incoming[:min(size, self.chunk)])
        del self.incoming[:len(part)]
        return part
    def write(self, data, budget):
        self._call("write", budget)
        part = data[:self.chunk]; self.outgoing.extend(part); return len(part)


class BoundedPipeTests(unittest.TestCase):
    def frame(self):
        return wire.encode_result(wire.WorkerTelemetry(1, 1, 0, 22, 33, 1, 0, 1, 44, b"n"*32))

    def test_fragmented_80_byte_read_and_48_byte_writes(self):
        clock = Clock(); data = self.frame()
        pipe = FakePipe(data, chunk=3, clock=clock)
        t = transport.BoundedPipeTransport(pipe, clock=clock, budget_ms=1000)
        got, receipt = t.read_frame(80)
        self.assertEqual(got, data); self.assertEqual(receipt.transferred, 80)
        self.assertFalse(receipt.uncertain)
        init = wire.encode_init(wire.Init(5, b"a"*32))
        ack = wire.encode_ack(wire.Ack(9, b"b"*32))
        for frame in (init, ack):
            p = FakePipe(clock=clock, chunk=4)
            w = transport.BoundedPipeTransport(p, clock=clock, budget_ms=1000)
            self.assertEqual(len(frame), 48)
            self.assertEqual(w.write_frame(frame).transferred, 48)
            self.assertEqual(bytes(p.outgoing), frame)

    def test_deadline_created_before_first_call_and_never_reset(self):
        c = Clock(); p = FakePipe(self.frame(), chunk=80, clock=c)
        t = transport.BoundedPipeTransport(p, clock=c, budget_ms=10)
        _, r = t.read_frame(80)
        self.assertEqual(r.deadline, 10.01)
        self.assertTrue(all(0 < budget <= 10 for _, budget in p.calls))

    def test_cumulative_partial_read_and_write_share_one_deadline(self):
        c=Clock(); data=self.frame(); p=FakePipe(data, chunk=2, clock=c)
        t=transport.BoundedPipeTransport(p, clock=c, budget_ms=7)
        with self.assertRaises(transport.Hold) as cm: t.read_frame(80)
        rr=cm.exception.receipt
        self.assertGreater(rr.transferred, 0)
        self.assertLess(rr.transferred, 80)
        self.assertEqual(rr.deadline, 10.007)
        self.assertLessEqual(c.now, rr.deadline)

        c=Clock(); p=FakePipe(clock=c, chunk=2)
        t=transport.BoundedPipeTransport(p, clock=c, budget_ms=7)
        with self.assertRaises(transport.Hold) as cm:
            t.write_frame(wire.encode_ack(wire.Ack(2, b"q"*32)))
        wr=cm.exception.receipt
        self.assertGreater(wr.transferred, 0)
        self.assertLess(wr.transferred, 48)
        self.assertEqual(wr.deadline, 10.007)
        self.assertLessEqual(c.now, wr.deadline)
        self.assertTrue(all(op != "peek" for op, _ in p.calls))

    def test_eintr_eof_exception_and_partial_transfer_are_hold(self):
        for fault in (InterruptedError("EINTR"), OSError("broken"), TimeoutError("timeout")):
            c=Clock(); p=FakePipe(self.frame(), chunk=20, clock=c, fault=(2, fault))
            with self.assertRaises(transport.Hold) as cm:
                transport.BoundedPipeTransport(p, clock=c).read_frame(80)
            self.assertTrue(cm.exception.receipt.uncertain)
        c=Clock(); p=FakePipe(self.frame()[:17], chunk=20, clock=c)
        with self.assertRaises(transport.Hold): transport.BoundedPipeTransport(p, clock=c).read_frame(80)
        c=Clock(); p=FakePipe(clock=c, chunk=10, fault=(4, OSError("write broke")))
        with self.assertRaises(transport.Hold): transport.BoundedPipeTransport(p, clock=c).write_frame(
            wire.encode_ack(wire.Ack(2, b"z"*32)))

    def test_expired_before_call_and_late_adapter_result_hold(self):
        c=Clock(); p=FakePipe(self.frame(), chunk=80, clock=c, fault=(1, "late"))
        with self.assertRaises(transport.Hold) as cm:
            transport.BoundedPipeTransport(p, clock=c, budget_ms=5).read_frame(80)
        self.assertEqual(cm.exception.receipt.calls, 1)
        c=Clock(); p=FakePipe(self.frame(), chunk=80, clock=c)
        def expired(): c.now += .01; return c.now
        with self.assertRaises(transport.Hold):
            transport.BoundedPipeTransport(p, clock=expired, budget_ms=5).read_frame(80)
        self.assertEqual(p.calls, [])

    def test_rejects_native_or_malformed_contract(self):
        with self.assertRaises(TypeError): transport.BoundedPipeTransport(object())
        c=Clock(); p=FakePipe(clock=c); t=transport.BoundedPipeTransport(p, clock=c)
        for size in (0, 47, 49, 79, 81):
            with self.assertRaises(wire.ProtocolError): t.read_frame(size)
        with self.assertRaises(wire.ProtocolError): t.write_frame(b"x"*49)
        with self.assertRaises(RuntimeError): transport.create_native_pipe_transport()
        self.assertEqual(transport.NATIVE_ENTRY, "HARD HOLD")


if __name__ == "__main__": unittest.main()
