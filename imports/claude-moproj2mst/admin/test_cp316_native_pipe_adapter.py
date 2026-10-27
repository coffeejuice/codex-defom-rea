"""Fake-only CP316 lifecycle tests. No native, process, GUI or Git calls."""
import importlib.util
from pathlib import Path
import sys
import unittest

spec = importlib.util.spec_from_file_location(
    "cp316_native_pipe_adapter", Path(__file__).with_name("cp316_native_pipe_adapter.py"))
m = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = m
spec.loader.exec_module(m)


class Clock:
    now = 10.0
    def __call__(self): return self.now


class Kernel:
    cp316_fake_kernel = True
    def __init__(self, clock, *, pending=False, error=False, chunk=7):
        self.clock, self.pending, self.error, self.chunk = clock, pending, error, chunk
        self.submitted = []; self.closed = []; self.cancelled = []; self.waits = []
        self.poll_result = None; self.cancel_error = False; self.submit_error = False
    def create_event(self): return object()
    def create_overlapped(self, event): return {"event": event}
    def submit(self, op):
        self.submitted.append(op)
        if self.submit_error: raise OSError("uncertain submit")
        if self.pending: return m.Completion("pending")
        return self.complete(op)
    def complete(self, op):
        if self.error: return m.Completion("error", error=109)
        count = min(len(op.buffer), self.chunk)
        if op.direction == "read": op.buffer[:count] = b"x" * count
        return m.Completion("complete", count)
    def poll(self, op):
        return self.poll_result if self.poll_result is not None else self.complete(op)
    def wait(self, event, remaining_ms):
        self.waits.append(remaining_ms); self.clock.now += .002
    def cancel(self, op):
        self.cancelled.append(op)
        if self.cancel_error: raise OSError(1168, "ERROR_NOT_FOUND")
    def close_event(self, event): self.closed.append(event)


class Tests(unittest.TestCase):
    def owner(self, **kwargs):
        clock = Clock(); prep = m.prepare(clock=clock, budget_ms=100)
        kernel = Kernel(clock, **kwargs)
        return m.OverlappedModel(kernel, prep, clock=clock), kernel, clock

    def test_preparation_has_two_private_contract_one_way_endpoints(self):
        clock = Clock(); p = m.prepare(clock=clock, budget_ms=100)
        self.assertEqual(p.deadline, 10.1)
        self.assertNotEqual(p.to_worker.name, p.from_worker.name)
        self.assertTrue(p.to_worker.name.startswith("\\\\.\\pipe\\cp316-"))
        for endpoint in (p.to_worker, p.from_worker):
            self.assertTrue(endpoint.server_access & m.FILE_FLAG_OVERLAPPED)
            self.assertTrue(endpoint.server_access & m.FILE_FLAG_FIRST_PIPE_INSTANCE)
            self.assertFalse(endpoint.server_inheritable)
            self.assertTrue(endpoint.client_inheritable)
            self.assertEqual(endpoint.max_instances, 1)
            self.assertEqual(endpoint.pipe_mode, m.PIPE_REJECT_REMOTE_CLIENTS)
        self.assertEqual(p.to_worker.client_access, "read")
        self.assertEqual(p.from_worker.client_access, "write")
        self.assertIn("unbound", p.qualification)

    def test_immediate_partial_read_and_write(self):
        for direction, payload in (("read", 80), ("write", b"a" * 48)):
            owner, kernel, _ = self.owner()
            data = owner.transfer("server", direction, payload)
            self.assertEqual(data, b"x" * 80 if direction == "read" else b"")
            self.assertGreater(len(kernel.submitted), 1)
            self.assertEqual(len(kernel.closed), len(kernel.submitted))
            self.assertEqual(owner.retained, [])
            owner.require_releasable()

    def test_pending_completion_and_shared_budget(self):
        owner, kernel, _ = self.owner(pending=True)
        self.assertEqual(owner.transfer(1, "read", 48), b"x" * 48)
        self.assertGreater(len(kernel.waits), 1)
        self.assertLess(kernel.waits[-1], kernel.waits[0])
        self.assertEqual(kernel.cancelled, [])

    def test_terminal_error_releases_resources_but_holds_stream(self):
        owner, kernel, _ = self.owner(error=True)
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        self.assertEqual(len(kernel.closed), 1)
        self.assertEqual(owner.retained, [])
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)

    def test_timeout_retains_until_cancel_terminal_completion(self):
        owner, kernel, clock = self.owner(pending=True)
        kernel.poll_result = m.Completion("pending")
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        op = kernel.submitted[0]
        self.assertIs(owner.retained[0], op)
        self.assertIs(op.overlapped["event"], op.event)
        self.assertEqual(len(op.buffer), 48)
        self.assertEqual(kernel.cancelled, [op])
        self.assertEqual(kernel.closed, [])
        with self.assertRaises(m.Hold): owner.require_releasable()
        self.assertEqual(owner.reap(), 1)
        kernel.poll_result = m.Completion("error", error=995)
        self.assertEqual(owner.reap(), 0)
        self.assertEqual(kernel.closed, [op.event])
        owner.require_releasable()

    def test_cancel_not_found_requires_completion_not_error_assumption(self):
        owner, kernel, _ = self.owner(pending=True)
        kernel.cancel_error = True; kernel.poll_result = m.Completion("pending")
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        self.assertEqual(len(owner.retained), 1)
        self.assertEqual(kernel.closed, [])
        kernel.poll_result = m.Completion("complete", 48)  # raced successful I/O
        self.assertEqual(owner.reap(), 0)
        self.assertTrue(owner.held)

    def test_submit_exception_retains_potentially_live_operation(self):
        owner, kernel, _ = self.owner()
        kernel.submit_error = True; kernel.poll_result = m.Completion("pending")
        with self.assertRaises(OSError): owner.transfer(1, "write", b"z" * 48)
        self.assertEqual(len(owner.retained), 1)
        self.assertEqual(bytes(owner.retained[0].buffer), b"z" * 48)

    def test_deadline_includes_preparation_to_first_io(self):
        owner, kernel, clock = self.owner()
        clock.now += .101
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        self.assertEqual(kernel.submitted, [])

    def test_invalid_count_or_completion_fails_closed(self):
        for result in (m.Completion("complete", 0), m.Completion("complete", 81), None):
            owner, kernel, _ = self.owner(pending=True)
            kernel.poll = lambda op: result
            with self.assertRaises(m.Hold): owner.transfer(1, "read", 80)
            if result is None: self.assertEqual(len(owner.retained), 1)

    def test_native_hold_and_fake_marker(self):
        with self.assertRaises(m.Hold): m.create_native_pipe_adapter()
        with self.assertRaises(TypeError): m.OverlappedModel(object(), m.prepare())

    def test_late_immediate_success_is_terminal_but_not_accepted(self):
        owner, kernel, clock = self.owner(chunk=80)
        submit = kernel.submit
        def late(op):
            result = submit(op); clock.now += .101; return result
        kernel.submit = late
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 80)
        self.assertEqual(owner.retained, [])
        self.assertEqual(len(kernel.closed), 1)
        self.assertEqual(kernel.cancelled, [])

    def test_cancel_terminal_race_releases_exact_resources(self):
        owner, kernel, clock = self.owner(pending=True)
        def expire(event, budget): clock.now += .101
        kernel.wait = expire
        # _budget expires before normal poll; cancel then obtains terminal error.
        kernel.poll_result = m.Completion("error", error=995)
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        self.assertEqual(len(kernel.cancelled), 1)
        self.assertEqual(len(kernel.closed), 1)
        self.assertEqual(owner.retained, [])

    def test_nonprogressing_pending_kernel_has_explicit_iteration_bound(self):
        owner, kernel, _ = self.owner(pending=True)
        kernel.poll_result = m.Completion("pending")
        kernel.wait = lambda event, ms: kernel.waits.append(ms)
        with self.assertRaises(m.Hold): owner.transfer(1, "read", 48)
        self.assertEqual(len(kernel.waits), m.MAX_PENDING_WAITS)
        self.assertEqual(len(owner.retained), 1)


if __name__ == "__main__":
    unittest.main()
