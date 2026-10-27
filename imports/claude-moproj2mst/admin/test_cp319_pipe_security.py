"""CP319 callable fakes only: no Windows DLL, pipe, process or GUI execution."""
import ctypes as C
import gc
import struct
import unittest

import cp319_pipe_security as security


class Fn:
    def __init__(self, callback):
        self.callback = callback
    def __call__(self, *args):
        return self.callback(*args)


class Fake:
    cp319_fake_api = True
    sid = b"\x01\x01\x00\x00\x00\x00\x00\x05" + struct.pack("<I", 12345)

    def __init__(self):
        self.calls = []
        self.error = 0
        self.thread_error = security.ERROR_NO_TOKEN
        self.fail = None
        self.sid_fault = None
        self.size_fault = None
        self.fail_token_read = False
        self.wrong_sid_length = False
        self.close_exception = None
        self.create_handle = 123
        self.snapshot = None
        self.closed = []
        for name in security.SIGNATURES:
            setattr(self, name, Fn(lambda *args, name=name: self.call(name, args)))

    def call(self, name, args):
        self.calls.append(name)
        if name == self.fail:
            self.error = 5
            return 0
        if name in ("GetCurrentThread", "GetCurrentProcess"):
            return -2 if name == "GetCurrentThread" else -1
        if name == "OpenThreadToken":
            self.assert_token_query(args[1], args[2])
            if self.thread_error:
                self.error = self.thread_error
                return 0
            C.cast(args[3], C.POINTER(security.HANDLE)).contents.value = 99
            return 1
        if name == "OpenProcessToken":
            assert args[1] == security.TOKEN_QUERY
            C.cast(args[2], C.POINTER(security.HANDLE)).contents.value = 100
            return 1
        if name == "GetTokenInformation":
            assert args[1] == security.TOKEN_USER
            size = C.sizeof(security.SID_AND_ATTRIBUTES) + len(self.sid)
            C.cast(args[4], C.POINTER(security.DWORD)).contents.value = size
            if args[2] is None:
                if self.size_fault == "small":
                    C.cast(args[4], C.POINTER(security.DWORD)).contents.value = 1
                if self.size_fault == "large":
                    C.cast(args[4], C.POINTER(security.DWORD)).contents.value = 65537
                self.error = security.ERROR_INSUFFICIENT_BUFFER
                return 0
            if self.fail_token_read:
                self.error = security.ERROR_INSUFFICIENT_BUFFER
                return 0
            if self.size_fault == "returned":
                C.cast(args[4], C.POINTER(security.DWORD)).contents.value = size + 1
            sid = C.addressof(args[2]) + C.sizeof(security.SID_AND_ATTRIBUTES)
            C.memmove(sid, self.sid, len(self.sid))
            user = security.SID_AND_ATTRIBUTES.from_buffer(args[2])
            user.Sid = sid
            if self.sid_fault == "outside":
                user.Sid = 1
            if self.sid_fault == "truncated":
                C.c_ubyte.from_address(sid + 1).value = 15
            if self.sid_fault == "revision":
                C.c_ubyte.from_address(sid).value = 2
            return 1
        if name == "CloseHandle":
            if self.close_exception is not None:
                raise self.close_exception
            self.closed.append(args[0].value)
            return 1
        if name in ("IsValidSid", "IsValidSecurityDescriptor"):
            return 1
        if name == "GetLengthSid":
            return len(self.sid) + (4 if self.wrong_sid_length else 0)
        if name == "CopySid":
            C.memmove(args[1], args[2], args[0])
            return 1
        if name == "InitializeAcl":
            C.memmove(args[0], struct.pack("<BBHHH", args[2], 0, args[1], 0, 0), 8)
            return 1
        if name == "AddAccessAllowedAceEx":
            acl, revision, flags, mask, sid = args
            assert revision == 2
            ace = struct.pack("<BBHI", 0, flags, 8 + len(self.sid), mask) + bytes(sid)
            C.memmove(C.addressof(acl) + 8, ace, len(ace))
            C.c_uint16.from_buffer(acl, 4).value = 1
            return 1
        if name == "InitializeSecurityDescriptor":
            sd = C.cast(args[0], C.POINTER(security.SECURITY_DESCRIPTOR)).contents
            sd.Revision = args[1]
            return 1
        if name == "SetSecurityDescriptorDacl":
            sd = C.cast(args[0], C.POINTER(security.SECURITY_DESCRIPTOR)).contents
            assert args[1] is True and args[3] is False
            sd.Control = 4  # SE_DACL_PRESENT, not DEFAULTED or SELF_RELATIVE
            sd.Dacl = C.addressof(args[2])
            return 1
        if name == "CreateNamedPipeW":
            gc.collect()  # owned descriptor, ACL and SID must still be valid
            sa = C.cast(args[7], C.POINTER(security.SECURITY_ATTRIBUTES)).contents
            sd = C.cast(sa.lpSecurityDescriptor, C.POINTER(security.SECURITY_DESCRIPTOR)).contents
            acl = C.string_at(sd.Dacl, 16 + len(self.sid))
            self.snapshot = (args[:7], sa.nLength, sa.bInheritHandle,
                             sd.Revision, sd.Control, sd.Owner, sd.Group, sd.Sacl, acl)
            return self.create_handle
        raise AssertionError(name)

    @staticmethod
    def assert_token_query(access, open_as_self):
        assert access == security.TOKEN_QUERY and open_as_self is True


class Tests(unittest.TestCase):
    def owner(self, fake=None):
        fake = fake or Fake()
        return security.InjectedPrivatePipeSecurity(fake, last_error=lambda: fake.error), fake

    def test_live_and_unmarked_factories_hold_without_calls(self):
        with self.assertRaisesRegex(security.Hold, "HARD HOLD"):
            security.create_live_private_pipe_security()
        fake = Fake()
        fake.cp319_fake_api = False
        with self.assertRaises(security.Hold):
            self.owner(fake)
        self.assertEqual(fake.calls, [])

    def test_layout_and_every_declared_signature(self):
        owner, fake = self.owner()
        self.assertEqual(C.sizeof(security.DWORD), 4)
        self.assertEqual(C.sizeof(security.BOOL), 4)
        self.assertEqual(C.sizeof(security.SECURITY_ATTRIBUTES), 24 if C.sizeof(C.c_void_p) == 8 else 12)
        self.assertEqual(C.sizeof(security.SECURITY_DESCRIPTOR), 40 if C.sizeof(C.c_void_p) == 8 else 20)
        for name, (args, result) in security.SIGNATURES.items():
            self.assertEqual(getattr(fake, name).argtypes, args)
            self.assertIs(getattr(fake, name).restype, result)

    def test_only_effective_user_ace_and_owned_lifetime_through_create(self):
        owner, fake = self.owner()
        self.assertEqual(fake.closed, [100])
        self.assertEqual(owner.create_pipe(r"\\.\pipe\cp319-unit", direction=1), 123)
        args, size, inherit, revision, control, owner_sid, group, sacl, acl = fake.snapshot
        self.assertEqual(args, (r"\\.\pipe\cp319-unit", 1 | 0x40000000 | 0x80000, 8, 1, 80, 80, 0))
        self.assertEqual((size, inherit), (C.sizeof(security.SECURITY_ATTRIBUTES), 0))
        self.assertEqual((revision, control, owner_sid, group, sacl), (1, 4, None, None, None))
        self.assertEqual(struct.unpack("<BBHHH", acl[:8]), (2, 0, len(acl), 1, 0))
        self.assertEqual(struct.unpack("<BBHI", acl[8:16]), (0, 0, 20, security.PIPE_ACCESS_MASK))
        self.assertEqual(acl[16:], Fake.sid)

    def test_specific_access_never_grants_create_instance_or_generic_write(self):
        self.assertEqual(security.PIPE_ACCESS_MASK, 0x120003)
        self.assertEqual(security.CLIENT_READ_ACCESS, 0x100001)
        self.assertEqual(security.CLIENT_WRITE_ACCESS, 0x100002)
        for desired in (security.CLIENT_READ_ACCESS, security.CLIENT_WRITE_ACCESS):
            self.assertEqual(desired & ~security.PIPE_ACCESS_MASK, 0)
        self.assertEqual(security.PIPE_ACCESS_MASK & (4 | 0x40000000 | 0x40000 | 0x80000), 0)

    def test_thread_token_preferred_without_process_fallback(self):
        fake = Fake()
        fake.thread_error = 0
        self.owner(fake)
        self.assertNotIn("OpenProcessToken", fake.calls)
        self.assertEqual(fake.closed, [99])

    def test_thread_query_denial_never_falls_back(self):
        fake = Fake()
        fake.thread_error = 5
        with self.assertRaisesRegex(security.ApiFailure, "OpenThreadToken"):
            self.owner(fake)
        self.assertNotIn("OpenProcessToken", fake.calls)
        self.assertEqual(fake.closed, [])

    def test_failures_hold_close_token_and_never_create_pipe(self):
        for operation in ("OpenProcessToken", "GetTokenInformation", "CopySid", "IsValidSid",
                          "CloseHandle", "InitializeAcl", "AddAccessAllowedAceEx",
                          "InitializeSecurityDescriptor", "SetSecurityDescriptorDacl",
                          "IsValidSecurityDescriptor"):
            with self.subTest(operation=operation):
                fake = Fake()
                fake.fail = operation
                with self.assertRaises(security.ApiFailure):
                    self.owner(fake)
                self.assertNotIn("CreateNamedPipeW", fake.calls)
                if operation != "OpenProcessToken":
                    self.assertIn("CloseHandle", fake.calls)

    def test_invalid_sid_extent_rejected_before_native_sid_reader(self):
        for fault in ("outside", "truncated", "revision"):
            with self.subTest(fault=fault):
                fake = Fake()
                fake.sid_fault = fault
                with self.assertRaises(security.Hold):
                    self.owner(fake)
                self.assertNotIn("IsValidSid", fake.calls)
                self.assertEqual(fake.closed, [100])

    def test_bad_names_directions_and_create_failure(self):
        owner, fake = self.owner()
        for name in (r"\\remote\pipe\x", "", "\\\\.\\pipe\\", "\\\\.\\pipe\\x\0y"):
            with self.assertRaises(ValueError):
                owner.create_pipe(name, direction=1)
        for direction in (0, 3, True):
            with self.assertRaises(ValueError):
                owner.create_pipe(r"\\.\pipe\x", direction=direction)
        self.assertNotIn("CreateNamedPipeW", fake.calls)
        fake.create_handle = security.INVALID_HANDLE_VALUE
        fake.error = 5
        with self.assertRaisesRegex(security.ApiFailure, "CreateNamedPipeW"):
            owner.create_pipe(r"\\.\pipe\x", direction=2)

    def test_token_buffer_size_and_second_read_fail_closed(self):
        for fault in ("small", "large", "returned"):
            with self.subTest(fault=fault):
                fake = Fake()
                fake.size_fault = fault
                with self.assertRaises(security.Hold):
                    self.owner(fake)
                self.assertEqual(fake.closed, [100])
                self.assertNotIn("IsValidSid", fake.calls)
                self.assertNotIn("InitializeAcl", fake.calls)
        fake = Fake()
        fake.fail_token_read = True
        with self.assertRaisesRegex(security.ApiFailure, "GetTokenInformation"):
            self.owner(fake)
        self.assertEqual(fake.closed, [100])
        self.assertNotIn("InitializeAcl", fake.calls)

    def test_inconsistent_native_sid_length_rejected(self):
        fake = Fake()
        fake.wrong_sid_length = True
        with self.assertRaisesRegex(security.Hold, "inconsistent token SID length"):
            self.owner(fake)
        self.assertEqual(fake.closed, [100])
        self.assertNotIn("CopySid", fake.calls)

    def test_token_close_failure_retains_retryable_owner_after_constructor_aborts(self):
        fake = Fake()
        fake.fail = "CloseHandle"
        with self.assertRaises(security.TokenCleanupFailure) as caught:
            self.owner(fake)
        retained = caught.exception.token_owner
        self.assertEqual(retained.handle, 100)
        self.assertFalse(retained.closed)
        self.assertEqual(fake.closed, [])
        self.assertNotIn("InitializeAcl", fake.calls)
        with self.assertRaises(security.TokenCleanupFailure) as retry:
            retained.retry_close()
        self.assertIs(retry.exception.token_owner, retained)
        self.assertFalse(retained.closed)
        fake.fail = None
        retained.retry_close()
        self.assertTrue(retained.closed)
        self.assertEqual(fake.closed, [100])
        calls = fake.calls.count("CloseHandle")
        retained.retry_close()
        self.assertEqual(fake.calls.count("CloseHandle"), calls)

    def test_token_close_failure_preserves_original_construction_failure(self):
        fake = Fake()
        fake.sid_fault = "outside"
        fake.fail = "CloseHandle"
        with self.assertRaises(security.TokenCleanupFailure) as caught:
            self.owner(fake)
        self.assertIsInstance(caught.exception.__context__, security.Hold)
        self.assertIn("token SID outside", str(caught.exception.__context__))
        retained = caught.exception.token_owner
        self.assertFalse(retained.closed)
        fake.fail = None
        retained.retry_close()
        self.assertEqual(fake.closed, [100])

    def test_raising_token_close_retains_owner_and_cause_until_retry_receipt(self):
        fake = Fake()
        interruption = InterruptedError("injected close interrupted")
        fake.close_exception = interruption
        with self.assertRaises(security.TokenCleanupFailure) as caught:
            self.owner(fake)
        retained = caught.exception.token_owner
        self.assertIs(caught.exception.__cause__, interruption)
        self.assertIsNone(caught.exception.code)
        self.assertFalse(retained.closed)
        self.assertEqual(retained.handle, 100)
        self.assertEqual(fake.closed, [])
        self.assertNotIn("InitializeAcl", fake.calls)
        with self.assertRaises(security.TokenCleanupFailure) as retry:
            retained.retry_close()
        self.assertIs(retry.exception.token_owner, retained)
        self.assertIs(retry.exception.__cause__, interruption)
        self.assertFalse(retained.closed)
        fake.close_exception = None
        retained.retry_close()
        self.assertTrue(retained.closed)
        self.assertEqual(fake.closed, [100])
        calls = fake.calls.count("CloseHandle")
        retained.retry_close()
        self.assertEqual(fake.calls.count("CloseHandle"), calls)


if __name__ == "__main__":
    unittest.main()
