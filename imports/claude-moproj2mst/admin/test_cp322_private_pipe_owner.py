"""Injected callable tests only: no native pipe, DLL, process, or GUI calls."""
import ctypes as C
import gc
import unittest

import cp317_win32_named_pipe_abi as abi
import cp319_pipe_security as security
import cp322_private_pipe_owner as private
from test_cp317_win32_named_pipe_abi import Fn
from test_cp319_pipe_security import Fake as SecurityFake


class Fake(SecurityFake):
    cp317_fake_api = True

    def __init__(self):
        super().__init__()
        self.next_handle = 200
        self.flags = {}
        self.io_calls = []
        self.results = [(True, 0), (True, 0)]
        self.create_count = 0
        self.fail_create = 0
        self.fail_client = False
        self.bad_inherit = False
        self.alias_client = False
        self.close_failure = False
        self.cancel_failure = False
        self.cancel_error = abi.ERROR_NOT_FOUND
        self.connect_error = abi.ERROR_IO_PENDING
        self.submit_error = abi.ERROR_IO_PENDING
        self.snapshots = []
        for name in set(abi.SIGNATURES) | {"GetHandleInformation"}:
            setattr(self, name, Fn(lambda *args, name=name: self.call(name, args)))

    def call(self, name, args):
        if name not in set(abi.SIGNATURES) | {"GetHandleInformation"}:
            return super().call(name, args)
        self.io_calls.append((name, args))
        if name == "CloseHandle":
            value = getattr(args[0], "value", args[0])
            if self.close_failure:
                self.error = 6
                return False
            self.closed.append(value)
            return True
        if name in ("CreateNamedPipeW", "CreateFileW", "CreateEventW"):
            if name == "CreateNamedPipeW":
                self.create_count += 1
                if self.create_count == self.fail_create:
                    self.error = 5
                    return abi.INVALID_HANDLE_VALUE
                gc.collect()
                sa = C.cast(args[7], C.POINTER(security.SECURITY_ATTRIBUTES)).contents
                sd = C.cast(sa.lpSecurityDescriptor, C.POINTER(security.SECURITY_DESCRIPTOR)).contents
                acl = C.string_at(sd.Dacl, 16 + len(self.sid))
                self.snapshots.append((args[:7], sa.bInheritHandle,
                                       sa.lpSecurityDescriptor, acl))
            if name == "CreateFileW" and self.fail_client:
                self.error = 5
                return abi.INVALID_HANDLE_VALUE
            self.next_handle += 1
            handle = self.next_handle
            if name == "CreateFileW" and self.alias_client:
                handle -= 2
            inherit = (C.cast(args[3], C.POINTER(abi.SECURITY_ATTRIBUTES)).contents.bInheritHandle
                       if name == "CreateFileW" else 0)
            self.flags[handle] = inherit
            return handle
        if name == "GetHandleInformation":
            C.cast(args[1], abi.LPDWORD).contents.value = self.flags[args[0]] ^ self.bad_inherit
            return True
        if name == "ConnectNamedPipe":
            self.error = self.connect_error
            return not self.connect_error
        if name in ("ReadFile", "WriteFile"):
            if name == "ReadFile":
                C.memset(args[1], ord("r"), args[2])
            self.error = self.submit_error
            return not self.submit_error
        if name == "GetOverlappedResultEx":
            ok, value = self.results.pop(0)
            if ok:
                C.cast(args[2], abi.LPDWORD).contents.value = value
            else:
                self.error = value
            return ok
        if name == "CancelIoEx":
            if self.cancel_failure:
                raise InterruptedError("cancel uncertain")
            self.error = self.cancel_error
            return not self.cancel_error
        raise AssertionError(name)


class Tests(unittest.TestCase):
    def owner(self, fake=None, clock=lambda: 0.):
        fake = fake or Fake()
        return private.InjectedPrivatePipeOwner(fake, last_error=lambda: fake.error,
                                               deadline=5., clock=clock), fake

    def connected(self):
        owner, fake = self.owner()
        receipt = owner.create_pair(r"\\.\pipe\cp322-unit")
        return owner, fake, receipt

    def test_private_two_pipe_receipt_lifetime_and_flags(self):
        owner, fake, receipt = self.connected()
        self.assertIsInstance(receipt, private.ConnectedEndpoints)
        self.assertEqual(len(owner.handles), 4)
        self.assertFalse(owner.operations)
        self.assertEqual(len(set(h for p in receipt.pairs for h in p)), 4)
        self.assertIsNotNone(owner.security)
        self.assertEqual(fake.snapshots[0][2:], fake.snapshots[1][2:])
        for index, (args, inherit, _, acl) in enumerate(fake.snapshots):
            self.assertEqual(args[1], (2 if index == 0 else 1) | 0x40080000)
            self.assertEqual(args[2:7], (8, 1, 80, 80, 0))
            self.assertEqual(inherit, 0)
            self.assertEqual(acl[16:], fake.sid)
        clients = [args for name, args in fake.io_calls if name == "CreateFileW"]
        self.assertEqual([a[1] for a in clients], [0x100001, 0x100002])
        self.assertTrue(owner.cleanup())
        self.assertFalse(owner.handles)

    def test_short_write_and_read_exact_under_original_deadline(self):
        owner, fake, r = self.connected()
        fake.results = [(True, 17), (True, 31)]
        self.assertEqual(owner.transfer(r.parent_write, "write", b"w" * 48), 48)
        writes = [a for n, a in fake.io_calls if n == "WriteFile"]
        self.assertEqual([a[2] for a in writes], [48, 31])
        self.assertEqual(bytes(writes[1][1]), b"w" * 31)
        fake.results = [(True, 1), (True, 39), (True, 40)]
        self.assertEqual(owner.transfer(r.parent_read, "read", 80), b"r" * 80)
        self.assertEqual(owner.deadline, 5.)

    def test_expired_before_security_has_no_api_calls(self):
        owner, fake = self.owner(clock=lambda: 5.)
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.create_pair(r"\\.\pipe\expired")
        self.assertFalse(fake.calls)
        self.assertFalse(fake.io_calls)

    def test_security_late_return_cannot_create_pipe(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        original = fake.IsValidSecurityDescriptor.callback
        def late(*args):
            result = original(*args)
            now[0] = 5.
            return result
        fake.IsValidSecurityDescriptor.callback = late
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.create_pair(r"\\.\pipe\late")
        self.assertEqual(fake.create_count, 0)
        self.assertIsNotNone(owner.security)

    def test_security_calls_share_deadline_but_token_cleanup_survives(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        original = fake.OpenProcessToken.callback
        def late(*args):
            result = original(*args)
            now[0] = 5.
            return result
        fake.OpenProcessToken.callback = late
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.create_pair(r"\\.\pipe\late-token")
        self.assertNotIn("GetTokenInformation", fake.calls)
        self.assertEqual(fake.closed, [100])
        self.assertTrue(owner.cleanup())

    def test_second_server_failure_retains_first_pair(self):
        owner, fake = self.owner()
        fake.fail_create = 2
        with self.assertRaises(security.ApiFailure):
            owner.create_pair(r"\\.\pipe\partial")
        self.assertEqual(len(owner.handles), 2)
        self.assertIsNone(owner.receipt)
        self.assertTrue(owner.cleanup())

    def test_late_server_adopted_before_deadline_check(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        original = fake.CreateNamedPipeW.callback
        def late(*args):
            handle = original(*args)
            now[0] = 5.
            return handle
        fake.CreateNamedPipeW.callback = late
        with self.assertRaises(abi.Hold):
            owner.create_pair(r"\\.\pipe\late-server")
        self.assertEqual(owner.handles, {201})
        self.assertTrue(owner.cleanup())

    def test_client_failure_pending_cancel_and_retry_reap(self):
        owner, fake = self.owner()
        fake.fail_client = True
        fake.results = [(False, abi.ERROR_IO_INCOMPLETE)] * 2 + [(False, 995)]
        with self.assertRaises(abi.ApiFailure):
            owner.create_pair(r"\\.\pipe\pending")
        self.assertIsNone(owner.receipt)
        self.assertFalse(owner.cleanup())
        self.assertTrue(owner.operations)
        self.assertTrue(owner.cleanup())

    def test_inheritance_or_alias_failure_never_receipts(self):
        for attr in ("bad_inherit", "alias_client"):
            owner, fake = self.owner()
            setattr(fake, attr, True)
            with self.assertRaises(abi.Hold):
                owner.create_pair(r"\\.\pipe\bad")
            self.assertIsNone(owner.receipt)
            self.assertTrue(owner.held)
            if owner.operations:
                fake.results = [(False, 995)] * 4
            self.assertTrue(owner.cleanup())

    def test_timeout_short_transfer_pending_lifetime(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        r = owner.create_pair(r"\\.\pipe\timeout")
        original = fake.GetOverlappedResultEx.callback
        def late(*args):
            result = original(*args)
            now[0] = 5.
            return result
        fake.GetOverlappedResultEx.callback = late
        fake.results = [(False, 258), (False, 996), (False, 995)]
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.transfer(r.parent_read, "read", 48)
        self.assertTrue(owner.operations)
        self.assertIsNotNone(owner.operations[0].buffer)
        with self.assertRaises(abi.Hold):
            owner.close(r.parent_read)
        self.assertTrue(owner.cleanup())

    def test_zero_overlarge_and_immediate_error_fail_closed(self):
        for result in ((True, 0), (True, 49), (False, 109)):
            owner, fake, r = self.connected()
            fake.results = [result]
            with self.assertRaises(abi.Hold):
                owner.transfer(r.parent_write, "write", b"x" * 48)
            self.assertTrue(owner.held)
            self.assertTrue(owner.cleanup())

    def test_cleanup_close_failure_is_retryable(self):
        owner, fake, _ = self.connected()
        fake.close_failure = True
        with self.assertRaises(abi.ApiFailure):
            owner.cleanup()
        self.assertEqual(len(owner.handles), 4)
        fake.close_failure = False
        self.assertTrue(owner.cleanup())

    def test_uncertain_submit_and_cancel_retain_then_retry(self):
        owner, fake, r = self.connected()
        def interrupted(*_args):
            raise InterruptedError("uncertain submission")
        fake.ReadFile.callback = interrupted
        fake.cancel_failure = True
        with self.assertRaises(InterruptedError):
            owner.transfer(r.parent_read, "read", 48)
        op = owner.operations[0]
        self.assertFalse(op.cancel_requested)
        self.assertIsNotNone(op.buffer)
        self.assertIn(op.event, owner.handles)
        fake.cancel_failure = False
        fake.results = [(False, 995)]
        self.assertTrue(owner.cleanup())

    def test_failed_cancel_retries_without_releasing_pending_resources(self):
        owner, fake = self.owner()
        fake.fail_client = True
        fake.cancel_error = 5
        fake.results = [(False, abi.ERROR_IO_INCOMPLETE)]
        with self.assertRaises(abi.ApiFailure):
            owner.create_pair(r"\\.\pipe\cancel-failure")
        op = owner.operations[0]
        self.assertFalse(op.cancel_requested)
        self.assertFalse(op.terminal)
        self.assertEqual(owner.cancel_errors[op.event], 5)
        self.assertIn(op.event, owner.handles)
        self.assertIn(op.handle, owner.handles)
        self.assertFalse(any(h in fake.closed for h in (op.event, op.handle)))
        fake.cancel_error = 0
        fake.results = [(False, abi.ERROR_IO_INCOMPLETE),
                        (False, abi.ERROR_IO_INCOMPLETE)]
        self.assertFalse(owner.cleanup())
        self.assertTrue(op.cancel_requested)
        self.assertNotIn(op.event, owner.cancel_errors)
        self.assertEqual(sum(n == "CancelIoEx" for n, _ in fake.io_calls), 2)
        fake.results = [(False, abi.ERROR_OPERATION_ABORTED)]
        self.assertTrue(owner.cleanup())

    def test_late_event_creation_prevents_connect_and_transfer_submission(self):
        for kind in ("connect", "write", "read"):
            with self.subTest(kind=kind):
                now = [0.]
                owner, fake = self.owner(clock=lambda: now[0])
                r = None if kind == "connect" else owner.create_pair(r"\\.\pipe\ready")
                original = fake.CreateEventW.callback
                def late(*args):
                    result = original(*args)
                    now[0] = 5.
                    return result
                fake.CreateEventW.callback = late
                before = len(fake.io_calls)
                with self.assertRaisesRegex(abi.Hold, "deadline"):
                    if kind == "connect":
                        owner.create_pair(r"\\.\pipe\late-event")
                    else:
                        owner.transfer(r.parent_write if kind == "write" else r.parent_read,
                                       kind, b"w" * 48 if kind == "write" else 80)
                forbidden = {"ConnectNamedPipe", "WriteFile", "ReadFile", "CancelIoEx",
                             "GetOverlappedResultEx"}
                self.assertFalse(any(n in forbidden for n, _ in fake.io_calls[before:]))
                self.assertTrue(owner.operations[0].terminal)
                self.assertTrue(owner.cleanup())

    def test_failed_security_token_close_remains_owned_until_retry(self):
        owner, fake = self.owner()
        fake.close_failure = True
        with self.assertRaises(security.TokenCleanupFailure):
            owner.create_pair(r"\\.\pipe\token-close")
        self.assertEqual(owner.token_owner.handle, 100)
        self.assertFalse(owner.token_owner.closed)
        with self.assertRaises(security.TokenCleanupFailure):
            owner.cleanup()
        self.assertFalse(owner.token_owner.closed)
        fake.close_failure = False
        self.assertTrue(owner.cleanup())
        self.assertTrue(owner.token_owner.closed)
        self.assertEqual(fake.create_count, 0)

    def test_raising_security_token_close_retains_owner_and_retry(self):
        owner, fake = self.owner()
        original = fake.CloseHandle.callback
        def interrupted(*_args):
            raise InterruptedError("token close outcome uncertain")
        fake.CloseHandle.callback = interrupted
        with self.assertRaises(security.TokenCleanupFailure) as caught:
            owner.create_pair(r"\\.\pipe\token-close-raises")
        self.assertIs(owner.token_owner, caught.exception.token_owner)
        self.assertEqual(owner.token_owner.handle, 100)
        self.assertFalse(owner.token_owner.closed)
        with self.assertRaises(security.TokenCleanupFailure):
            owner.cleanup()
        self.assertFalse(owner.token_owner.closed)
        fake.CloseHandle.callback = original
        self.assertTrue(owner.cleanup())
        self.assertTrue(owner.token_owner.closed)
        self.assertEqual(fake.create_count, 0)

    def test_native_export_and_unmarked_api_rejected_without_calls(self):
        fake = Fake()
        fake.cp319_fake_api = False
        with self.assertRaises(abi.Hold):
            self.owner(fake)
        self.assertFalse(fake.calls)
        fake = Fake()
        fake.GetHandleInformation = C.CFUNCTYPE(C.c_int)(lambda: 1)
        with self.assertRaisesRegex(abi.Hold, "native export"):
            self.owner(fake)
        self.assertFalse(fake.calls)
        with self.assertRaisesRegex(abi.Hold, "CP321"):
            private.create_live_private_pipes()

    def test_race_connect_completion_and_reuse_rejection(self):
        owner, fake = self.owner()
        fake.connect_error = abi.ERROR_PIPE_CONNECTED
        owner.create_pair(r"\\.\pipe\race")
        self.assertFalse(any(n == "GetOverlappedResultEx" for n, _ in fake.io_calls))
        with self.assertRaises(abi.Hold):
            owner.create_pair(r"\\.\pipe\again")


if __name__ == "__main__":
    unittest.main()
