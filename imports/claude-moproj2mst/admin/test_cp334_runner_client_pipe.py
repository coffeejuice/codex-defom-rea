"""Injected exports only: no DLL, process, GUI, or original application run."""
import ctypes as C
import unittest

import cp334_runner_client_pipe as client
from cp325_real_bindings import BoundExports


class Fake:
    def __init__(self):
        self.error, self.tick = 0, 1
        self.calls, self.pending, self.counts = [], {}, []
        self.flags, self.pipe_flags = {11: 1, 12: 1}, {11: 0, 12: 0}
        self.block, self.cancel_error, self.query_fail = False, 0, False
        self.close_fail, self.next_event = set(), 20
        self.submit_error, self.raise_submit, self.late_event = 0, False, False
        names = ("GetTickCount64", "GetHandleInformation", "GetNamedPipeInfo",
                 "CreateEventW", "ReadFile", "WriteFile", "GetOverlappedResultEx",
                 "CancelIoEx", "CloseHandle")
        self.exports = BoundExports({n: getattr(self, n) for n in names}, {}, lambda: self.error)

    def GetTickCount64(self):
        self.calls.append(("tick", self.tick))
        return self.tick

    def GetHandleInformation(self, handle, flags):
        self.calls.append(("inherit", handle))
        flags._obj.value = self.flags[handle]
        return True

    def GetNamedPipeInfo(self, handle, flags, outsize, insize, instances):
        self.calls.append(("info", handle))
        assert (outsize, insize, instances) == (None, None, None)
        if self.query_fail:
            self.error = 5
            return False
        flags._obj.value = self.pipe_flags[handle]
        return True

    def CreateEventW(self, sa, manual, initial, name):
        assert (sa, manual, initial, name) == (None, True, False, None)
        self.next_event += 1
        self.calls.append(("event", self.next_event))
        if self.late_event:
            self.tick = 100
        return self.next_event

    def _submit(self, kind, handle, buffer, size, count, ov):
        assert count is None
        self.pending[C.addressof(ov._obj)] = size
        self.calls.append((kind, handle, size, bytes(buffer)))
        if kind == "read":
            for i in range(size):
                buffer[i] = 65
        if self.raise_submit:
            raise RuntimeError("uncertain submit")
        self.error = self.submit_error or client.PENDING
        return False

    def ReadFile(self, *args):
        return self._submit("read", *args)

    def WriteFile(self, *args):
        return self._submit("write", *args)

    def GetOverlappedResultEx(self, handle, ov, count, timeout, alertable):
        self.calls.append(("wait", timeout))
        assert alertable is False
        if self.block:
            self.error = client.TIMEOUT
            self.tick += timeout
            return False
        count._obj.value = self.counts.pop(0) if self.counts else self.pending[C.addressof(ov._obj)]
        return True

    def CancelIoEx(self, handle, ov):
        self.calls.append(("cancel", handle))
        self.error = self.cancel_error
        return not self.cancel_error

    def CloseHandle(self, handle):
        self.calls.append(("close", handle))
        self.error = 5
        return handle not in self.close_fail


class Tests(unittest.TestCase):
    def owner(self, adopt=True):
        f = Fake()
        owner = client.NativeRunnerClientPipe(f.exports, deadline=100)
        if adopt:
            owner.adopt(11, 12)
        return f, owner

    def test_inert_constructor_exact_two_handle_receipt_and_held_entry(self):
        f, owner = self.owner(False)
        self.assertEqual(f.calls, [])
        receipt = owner.adopt(11, 12)
        self.assertEqual(receipt, client.ClientEndpoints(11, 12))
        self.assertEqual(owner.handles, {11, 12})
        self.assertFalse(hasattr(receipt, "parent_write"))
        self.assertTrue(owner.cleanup())
        self.assertEqual({c[1] for c in f.calls if c[0] == "close"}, {11, 12})
        with self.assertRaises(client.Hold):
            client.launch_trial()

    def test_explicit_deadline_and_fixed_argv_before_read(self):
        f = Fake()
        owner = client.from_bootstrap_argv(f.exports, [client.ARGV_TAG, "11", "12", "100"])
        self.assertEqual(owner.deadline, 100)
        self.assertFalse(any(c[0] == "read" for c in f.calls))
        self.assertTrue(owner.cleanup())
        for argv in ([], [client.ARGV_TAG, "011", "12", "100"],
                     [client.ARGV_TAG, "11", "12", "-1"],
                     [client.ARGV_TAG, "11", "12", "100", "13"]):
            with self.assertRaises(client.Hold):
                client.from_bootstrap_argv(f.exports, argv)
        for deadline in (None, True, 0, -1, 2**64):
            with self.assertRaises(ValueError):
                client.NativeRunnerClientPipe(f.exports, deadline=deadline)
        with self.assertRaises(TypeError):
            client.NativeRunnerClientPipe(object(), deadline=100)

    def test_adoption_stages_bound_each_native_query_and_failure_retains_owner(self):
        f = Fake()
        stages = []
        owner = client.from_bootstrap_argv(
            f.exports, [client.ARGV_TAG, "11", "12", "100"],
            stage=stages.append)
        expected = ["cp334_argv_parsed", "cp334_owner_created",
                    "cp334_adopt_enter"]
        for index in (0, 1):
            expected.extend(f"cp334_h{index}_{suffix}" for suffix in (
                "before_tick1", "after_tick1", "before_inherit",
                "after_inherit", "before_tick2", "after_tick2",
                "before_pipeinfo", "after_pipeinfo"))
            expected.append(f"cp334_h{index}_pipeflags_0")
            expected.append(f"cp334_h{index}_flags_validated")
        expected.extend(("cp334_before_final_tick", "cp334_after_final_tick"))
        self.assertEqual(stages, expected)
        self.assertTrue(owner.cleanup())

        f = Fake()
        f.query_fail = True
        stages = []
        with self.assertRaises(client.ApiFailure) as caught:
            client.from_bootstrap_argv(
                f.exports, [client.ARGV_TAG, "11", "12", "100"],
                stage=stages.append)
        self.assertEqual(stages[-1], "cp334_h0_before_pipeinfo")
        self.assertTrue(caught.exception.owner.held)
        self.assertEqual(caught.exception.owner.handles, {11, 12})
        self.assertTrue(caught.exception.owner.cleanup())

        f = Fake()
        f.pipe_flags[11] = 0x40000000
        stages = []
        with self.assertRaises(client.Hold) as caught:
            client.from_bootstrap_argv(
                f.exports, [client.ARGV_TAG, "11", "12", "100"],
                stage=stages.append)
        self.assertEqual(stages[-1], "cp334_h0_pipeflags_1073741824")
        self.assertEqual(caught.exception.owner.handles, {11, 12})
        self.assertTrue(caught.exception.owner.cleanup())

        f = Fake()
        f.pipe_flags[11] = 1
        stages = []
        with self.assertRaises(client.Hold) as caught:
            client.from_bootstrap_argv(
                f.exports, [client.ARGV_TAG, "11", "12", "100"],
                stage=stages.append)
        self.assertEqual(stages[-1], "cp334_h0_pipeflags_1")
        self.assertEqual(caught.exception.owner.handles, {11, 12})
        self.assertTrue(caught.exception.owner.cleanup())

    def test_invalid_alias_and_pseudo_handles_never_claimed(self):
        for pair in ((11, 11), (0, 12), (-1, 12), (True, 12), (client.INVALID - 1, 12)):
            f, owner = self.owner(False)
            with self.assertRaises(client.Hold):
                owner.adopt(*pair)
            self.assertEqual(owner.handles, set())
            self.assertEqual(f.calls, [])

    def test_bootstrap_tag_rejects_nonexact_strings_before_equality_or_api(self):
        class EqualTag:
            def __eq__(self, other):
                raise AssertionError("non-string tag equality must never run")

        class StringTag(str):
            def __eq__(self, other):
                raise AssertionError("string-subclass tag equality must never run")

        for tag in (EqualTag(), StringTag(client.ARGV_TAG)):
            f = Fake()
            with self.assertRaises(client.Hold):
                client.from_bootstrap_argv(f.exports, [tag, "11", "12", "100"])
            self.assertEqual(f.calls, [])

    def test_inheritance_server_message_and_attributes_fail_closed(self):
        for setting, value in (("flags", 0), ("flags", 3),
                               ("pipe_flags", 1), ("pipe_flags", 4)):
            f, owner = self.owner(False)
            getattr(f, setting)[12] = value
            with self.assertRaises(client.Hold):
                owner.adopt(11, 12)
            self.assertIsNone(owner.receipt)
            self.assertEqual(owner.handles, {11, 12})
            self.assertTrue(owner.cleanup())
        f = Fake()
        f.query_fail = True
        with self.assertRaises(client.ApiFailure) as caught:
            client.from_bootstrap_argv(f.exports, [client.ARGV_TAG, "11", "12", "100"])
        self.assertEqual(caught.exception.code, 5)
        self.assertTrue(caught.exception.owner.cleanup())

    def test_partial_exact_reads_writes_direction_and_frame_size(self):
        f, owner = self.owner()
        f.counts = [7, 41]
        frame = bytes(range(48))
        self.assertEqual(owner.transfer(12, "write", frame), 48)
        self.assertEqual([c[3] for c in f.calls if c[0] == "write"], [frame, frame[7:]])
        f.counts = [30, 50]
        self.assertEqual(owner.transfer(11, "read", 80), b"A" * 80)
        for h, k, p, error in ((12, "read", 48, client.Hold),
                              (11, "write", frame, client.Hold),
                              (13, "read", 80, client.Hold),
                              (11, "read", 49, ValueError)):
            with self.assertRaises(error):
                owner.transfer(h, k, p)
        self.assertTrue(owner.cleanup())

    def test_pending_timeout_cancel_race_retains_everything_until_reaped(self):
        f, owner = self.owner()
        f.block, f.cancel_error = True, client.NOT_FOUND
        with self.assertRaises(client.Hold):
            owner.transfer(11, "read", 80)
        op = owner.operations[0]
        self.assertIsNotNone(op.buffer)
        self.assertTrue(op.cancel_requested)
        self.assertFalse(owner.cleanup())
        self.assertEqual(owner.handles, {11, 12, op.event})
        self.assertFalse(any(c[0] == "close" for c in f.calls))
        self.assertTrue(all(0 <= c[1] < 0xFFFFFFFF for c in f.calls if c[0] == "wait"))
        f.block = False
        self.assertTrue(owner.cleanup())

    def test_uncertain_submission_cancel_and_close_failures_retry(self):
        f, owner = self.owner()
        f.raise_submit, f.block, f.cancel_error = True, True, 5
        with self.assertRaises(RuntimeError):
            owner.transfer(12, "write", bytes(48))
        op = owner.operations[0]
        self.assertFalse(op.cancel_requested)
        self.assertEqual(owner.cancel_errors[op.event], 5)
        f.block, f.cancel_error = False, 0
        f.close_fail.add(op.event)
        self.assertFalse(owner.cleanup())
        self.assertIn(op, owner.operations)
        self.assertIsNotNone(op.buffer)
        f.close_fail = {11}
        self.assertFalse(owner.cleanup())
        self.assertEqual(owner.handles, {11})
        f.close_fail.clear()
        self.assertTrue(owner.cleanup())

    def test_late_event_expiry_zero_oversize_and_terminal_error(self):
        f, owner = self.owner()
        f.late_event = True
        with self.assertRaises(client.Hold):
            owner.transfer(11, "read", 48)
        self.assertEqual(owner.handles, {11, 12, 21})
        self.assertFalse(any(c[0] == "read" for c in f.calls))
        self.assertTrue(owner.cleanup())
        for count in (0, 49):
            f, owner = self.owner()
            f.counts = [count]
            with self.assertRaises(client.Hold):
                owner.transfer(11, "read", 48)
            self.assertTrue(owner.cleanup())
        f, owner = self.owner()
        f.submit_error = 5
        with self.assertRaises(client.ApiFailure):
            owner.transfer(12, "write", bytes(80))
        self.assertTrue(owner.cleanup())

    def test_expired_and_backward_clock_never_renewed(self):
        f, owner = self.owner(False)
        f.tick = 100
        with self.assertRaises(client.Hold):
            owner.adopt(11, 12)
        self.assertTrue(owner.cleanup())
        f, owner = self.owner()
        f.tick = 0
        with self.assertRaises(client.Hold):
            owner.transfer(11, "read", 48)
        self.assertTrue(owner.cleanup())


if __name__ == "__main__":
    unittest.main()
