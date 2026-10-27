"""Inert tests: only Python fake functions inside CP325 BoundExports."""
import ctypes as C
import unittest

import cp321_native_exports as abi
from cp325_real_bindings import BoundExports
import cp328_native_pipes as pipes


class Security:
    def security_attributes(self, *, inheritable):
        return abi.SECURITY_ATTRIBUTES(C.sizeof(abi.SECURITY_ATTRIBUTES), 1234, inheritable)


class Fake:
    def __init__(self):
        self.error, self.tick, self.next_handle = 0, 1, 10
        self.calls, self.inherit, self.pending = [], {}, {}
        self.counts = []
        self.block = False
        self.cancel_error = 0
        self.close_fail = set()
        self.fail_client = 0
        self.bad_inherit = False
        self.connect_error = pipes.PENDING
        functions = {name: getattr(self, name) for name in (
            "GetTickCount64", "CreateNamedPipeW", "CreateFileW", "CreateEventW",
            "ConnectNamedPipe", "GetOverlappedResultEx", "GetHandleInformation",
            "ReadFile", "WriteFile", "CancelIoEx", "CloseHandle")}
        self.exports = BoundExports(functions, {}, lambda: self.error)

    def GetTickCount64(self):
        return self.tick

    def allocate(self, inherit=False):
        self.next_handle += 1
        self.inherit[self.next_handle] = inherit
        return self.next_handle

    def CreateNamedPipeW(self, name, mode, pipe_mode, instances, outsize, insize, timeout, sa):
        attrs = C.cast(sa, abi.SA).contents
        self.calls.append(("server", name, mode, pipe_mode, instances, bool(attrs.bInheritHandle)))
        return self.allocate()

    def CreateEventW(self, *args):
        return self.allocate()

    def CreateFileW(self, name, access, share, sa, disposition, flags, template):
        self.calls.append(("client", access))
        if self.fail_client:
            self.error = self.fail_client
            return pipes.INVALID_HANDLE
        return self.allocate(C.cast(sa, abi.SA).contents.bInheritHandle)

    def ConnectNamedPipe(self, handle, ov):
        self.pending[C.addressof(ov._obj)] = ("connect", 0)
        self.error = self.connect_error
        return False

    def GetOverlappedResultEx(self, handle, ov, count, timeout, alertable):
        self.calls.append(("wait", timeout))
        if self.block:
            self.error = pipes.TIMEOUT
            if timeout:
                self.tick += timeout
            return False
        kind, size = self.pending.get(C.addressof(ov._obj), ("connect", 0))
        count._obj.value = self.counts.pop(0) if kind != "connect" and self.counts else size
        return True

    def GetHandleInformation(self, handle, flags):
        flags._obj.value = int(bool(self.inherit[handle])) ^ int(self.bad_inherit)
        return True

    def ReadFile(self, handle, buffer, size, count, ov):
        for index in range(size):
            buffer[index] = 65
        self.pending[C.addressof(ov._obj)] = ("read", size)
        self.calls.append(("read", size))
        self.error = pipes.PENDING
        return False

    def WriteFile(self, handle, buffer, size, count, ov):
        self.pending[C.addressof(ov._obj)] = ("write", size)
        self.calls.append(("write", bytes(buffer)))
        return True

    def CancelIoEx(self, handle, ov):
        self.calls.append(("cancel", handle))
        self.error = self.cancel_error
        return not self.cancel_error

    def CloseHandle(self, handle):
        self.calls.append(("close", handle))
        if handle in self.close_fail:
            self.error = 5
            return False
        self.inherit.pop(handle)
        return True


class Tests(unittest.TestCase):
    def owner(self):
        f = Fake()
        return f, pipes.NativePipeOwner(f.exports, Security(), deadline=100)

    def prepared(self):
        f, owner = self.owner()
        return f, owner, owner.prepare(r"\\.\pipe\cp328-test")

    def test_constructor_inert_and_factory_held(self):
        f, owner = self.owner()
        self.assertEqual(f.calls, [])
        self.assertFalse(owner.started)
        with self.assertRaises(pipes.Hold):
            pipes.create_live_native_pipes()

    def test_private_flags_typed_receipt_and_exact_client_inheritance(self):
        f, owner, receipt = self.prepared()
        self.assertIsInstance(receipt, pipes.ConnectedEndpoints)
        self.assertEqual(len(owner.handles), 4)
        for call in (c for c in f.calls if c[0] == "server"):
            self.assertTrue(call[2] & pipes.FIRST_INSTANCE)
            self.assertTrue(call[2] & pipes.OVERLAPPED_FLAG)
            self.assertEqual(call[3:6], (8, 1, False))
        self.assertEqual([f.inherit[h] for pair in receipt.pairs for h in pair], [False, True, False, True])
        access_masks = [c[1] for c in f.calls if c[0] == "client"]
        self.assertEqual(access_masks, [0x100081, 0x100082])
        for access in access_masks:
            self.assertEqual(access & pipes.FILE_READ_ATTRIBUTES, 0x80)
            self.assertEqual(access & 0x4, 0)  # FILE_CREATE_PIPE_INSTANCE
        self.assertTrue(owner.cleanup())

    def test_partial_read_and_write_complete_full_frame(self):
        f, owner, ep = self.prepared()
        f.counts = [7, 13, 28]
        frame = bytes(range(48))
        self.assertEqual(owner.transfer(ep.parent_write, "write", frame), 48)
        self.assertEqual([c[1] for c in f.calls if c[0] == "write"], [frame, frame[7:], frame[20:]])
        f.counts = [30, 50]
        self.assertEqual(owner.transfer(ep.parent_read, "read", 80), b"A" * 80)
        self.assertEqual([c[1] for c in f.calls if c[0] == "read"], [80, 50])
        self.assertTrue(owner.cleanup())

    def test_timeout_cancel_race_retains_buffer_event_and_handles(self):
        f, owner, ep = self.prepared()
        f.block, f.cancel_error = True, pipes.NOT_FOUND
        with self.assertRaises(pipes.Hold):
            owner.transfer(ep.parent_read, "read", 80)
        op = owner.operations[0]
        self.assertIsNotNone(op.buffer)
        self.assertIn(op.event, owner.handles)
        self.assertFalse(owner.cleanup())
        self.assertIn(ep.parent_read, owner.handles)
        self.assertTrue(all(c[1] != 0xFFFFFFFF for c in f.calls if c[0] == "wait"))
        f.block = False
        self.assertTrue(owner.cleanup())

    def test_partial_creation_failure_is_owned_and_cleanup_retryable(self):
        f, owner = self.owner()
        f.fail_client, f.block = 5, True
        with self.assertRaises(pipes.ApiFailure):
            owner.prepare(r"\\.\pipe\cp328-test")
        self.assertEqual(len(owner.handles), 2)
        self.assertIsNone(owner.receipt)
        self.assertFalse(owner.cleanup())
        f.block = False
        server = owner.operations[0].handle
        f.close_fail.add(server)
        self.assertFalse(owner.cleanup())
        self.assertIn(server, owner.handles)
        f.close_fail.clear()
        self.assertTrue(owner.cleanup())

    def test_inheritance_failure_does_not_publish(self):
        f, owner = self.owner()
        f.bad_inherit = True
        with self.assertRaises(pipes.Hold):
            owner.prepare(r"\\.\pipe\cp328-test")
        self.assertIsNone(owner.receipt)
        self.assertTrue(owner.cleanup())

    def test_zero_and_oversize_completions_hold(self):
        for count in (0, 49):
            f, owner, ep = self.prepared()
            f.counts = [count]
            with self.assertRaises(pipes.Hold):
                owner.transfer(ep.parent_write, "write", bytes(48))
            self.assertTrue(owner.held)
            self.assertTrue(owner.cleanup())

    def test_cancel_failure_retries_and_close_event_failure_retains_operation(self):
        f, owner, ep = self.prepared()
        f.block, f.cancel_error = True, 5
        with self.assertRaises(pipes.Hold):
            owner.transfer(ep.parent_write, "write", bytes(48))
        op = owner.operations[0]
        self.assertFalse(op.cancel_requested)
        self.assertEqual(owner.cancel_errors[op.event], 5)
        f.cancel_error, f.block = 0, False
        f.close_fail.add(op.event)
        self.assertFalse(owner.cleanup())
        self.assertIn(op, owner.operations)
        self.assertIsNotNone(op.buffer)
        f.close_fail.clear()
        self.assertTrue(owner.cleanup())

    def test_deadline_not_reset_and_direction_enforced(self):
        f, owner, ep = self.prepared()
        f.tick = 100
        with self.assertRaisesRegex(pipes.Hold, "absolute pipe deadline expired"):
            owner.transfer(ep.parent_write, "write", bytes(48))
        self.assertEqual(owner.last_transfer_phase, "before_operation")
        self.assertTrue(owner.cleanup())

        f, owner, ep = self.prepared()
        f.block = True
        with self.assertRaisesRegex(pipes.Hold, "absolute pipe deadline expired"):
            owner.transfer(ep.parent_read, "read", 80)
        self.assertEqual(owner.last_transfer_phase, "wait_retry_budget")
        f.block = False
        self.assertTrue(owner.cleanup())

        f, owner, ep = self.prepared()
        functions = dict(f.exports.functions)
        original = functions["GetOverlappedResultEx"]

        def late_completion(*args):
            result = original(*args)
            f.tick = 100
            return result

        functions["GetOverlappedResultEx"] = late_completion
        owner.exports = BoundExports(functions, {}, lambda: f.error)
        with self.assertRaisesRegex(pipes.Hold, "absolute pipe deadline expired"):
            owner.transfer(ep.parent_read, "read", 80)
        self.assertEqual(owner.last_transfer_phase, "post_operation_budget")
        self.assertTrue(owner.cleanup())

    def test_wait_budget_discriminator(self):
        # Expiry before the first result remains the original phase.
        f, owner, ep = self.prepared()
        op = owner._operation(ep.parent_read, "read", (C.c_ubyte * 80)())
        op.terminal = False
        f.tick = 100
        with self.assertRaisesRegex(pipes.Hold, "absolute pipe deadline expired"):
            owner._finish(op)
        self.assertEqual(owner.last_transfer_phase, "wait_budget")
        self.assertTrue(owner.cleanup())

        # A successful first wait keeps the existing phase and completion path.
        f, owner, ep = self.prepared()
        observed = []
        budget = owner._budget

        def record_budget():
            observed.append(owner.last_transfer_phase)
            return budget()

        owner._budget = record_budget
        self.assertEqual(owner.transfer(ep.parent_write, "write", bytes(48)), 48)
        self.assertIn("wait_budget", observed)
        self.assertNotIn("wait_retry_budget", observed)
        self.assertEqual(owner.last_transfer_phase, "complete")
        self.assertTrue(owner.cleanup())

        # A nonterminal result followed by deadline expiry is distinguishable
        # from expiry at the initial wait budget check.
        f, owner, ep = self.prepared()
        functions = dict(f.exports.functions)
        def nonterminal_then_expire(*args):
            f.calls.append(("wait", args[3]))
            f.error = pipes.PENDING
            f.tick = 100
            return False

        functions["GetOverlappedResultEx"] = nonterminal_then_expire
        owner.exports = BoundExports(functions, {}, lambda: f.error)
        with self.assertRaisesRegex(pipes.Hold, "absolute pipe deadline expired"):
            owner.transfer(ep.parent_read, "read", 80)
        self.assertEqual(owner.last_transfer_phase, "wait_retry_budget")
        self.assertEqual(f.error, pipes.PENDING)
        # Cleanup can reap the retained operation after restoring the fake API.
        owner.exports = f.exports
        self.assertTrue(owner.cleanup())

    def test_already_connected_race_and_terminal_error(self):
        f, owner = self.owner()
        f.connect_error = pipes.CONNECTED
        self.assertIsNotNone(owner.prepare(r"\\.\pipe\cp328-test"))
        self.assertTrue(owner.cleanup())
        f, owner = self.owner()
        f.connect_error = 5
        with self.assertRaises(pipes.ApiFailure):
            owner.prepare(r"\\.\pipe\cp328-test")
        self.assertFalse(any(c[0] == "client" for c in f.calls))
        self.assertTrue(owner.cleanup())

    def test_late_create_retained_before_next_budget_check(self):
        f = Fake()
        functions = dict(f.exports.functions)
        original = functions["CreateNamedPipeW"]

        def late_create(*args):
            handle = original(*args)
            f.tick = 100
            return handle

        functions["CreateNamedPipeW"] = late_create
        exports = BoundExports(functions, {}, lambda: f.error)
        owner = pipes.NativePipeOwner(exports, Security(), deadline=100)
        with self.assertRaises(pipes.Hold):
            owner.prepare(r"\\.\pipe\cp328-test")
        self.assertEqual(len(owner.handles), 1)
        self.assertEqual(owner.operations, [])
        self.assertTrue(owner.cleanup())

    def test_second_connect_failure_retains_first_pair_and_pending_connect(self):
        f = Fake()
        functions = dict(f.exports.functions)
        original = functions["CreateFileW"]
        calls = []

        def second_client_failure(*args):
            calls.append(args)
            if len(calls) == 2:
                f.fail_client, f.block = 5, True
            return original(*args)

        functions["CreateFileW"] = second_client_failure
        owner = pipes.NativePipeOwner(BoundExports(functions, {}, lambda: f.error),
                                      Security(), deadline=100)
        with self.assertRaises(pipes.ApiFailure):
            owner.prepare(r"\\.\pipe\cp328-test")
        self.assertEqual(len(owner.handles), 4)
        self.assertEqual(len(owner.operations), 1)
        self.assertIsNone(owner.receipt)
        self.assertFalse(owner.cleanup())
        f.block = False
        self.assertTrue(owner.cleanup())


if __name__ == "__main__":
    unittest.main()
