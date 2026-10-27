"""CP326 callable-only exports: never resolve or invoke native functions."""
import ctypes as C
import gc
import struct
import unittest
import weakref

import cp321_native_exports as abi
import cp326_native_pipe_security as security


class FakeBoundExports:
    sid = b"\x01\x01\x00\x00\x00\x00\x00\x05" + struct.pack("<I", 12345)

    def __init__(self):
        self.calls, self.closed, self.lookups = [], [], []
        self.error, self.thread_error = 0, security.ERROR_NO_TOKEN
        self.fail = self.raise_on = self.sid_fault = self.size_fault = None
        self.wrong_length = False

    def last_error(self):
        return self.error

    def fn(self, name):
        assert name in abi.EXPORTS
        self.lookups.append(name)
        return lambda *args: self.call(name, args)

    def call(self, name, args):
        self.calls.append(name)
        if name == self.raise_on:
            raise InterruptedError(name)
        if name == self.fail:
            self.error = 5
            return 0
        if name in ("GetCurrentThread", "GetCurrentProcess"):
            return -2 if name == "GetCurrentThread" else -1
        if name == "OpenThreadToken":
            assert args[1:3] == (security.TOKEN_QUERY, True)
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
            needed = C.cast(args[4], C.POINTER(security.DWORD)).contents
            needed.value = size
            if args[2] is None:
                if self.size_fault == "small":
                    needed.value = 1
                if self.size_fault == "large":
                    needed.value = 65537
                self.error = security.ERROR_INSUFFICIENT_BUFFER
                return 0
            if self.size_fault == "returned":
                needed.value += 1
            sid = C.addressof(args[2]) + C.sizeof(security.SID_AND_ATTRIBUTES)
            C.memmove(sid, self.sid, len(self.sid))
            security.SID_AND_ATTRIBUTES.from_buffer(args[2]).Sid = sid
            if self.sid_fault == "outside":
                security.SID_AND_ATTRIBUTES.from_buffer(args[2]).Sid = 1
            if self.sid_fault == "truncated":
                C.c_ubyte.from_address(sid + 1).value = 15
            if self.sid_fault == "revision":
                C.c_ubyte.from_address(sid).value = 2
            return 1
        if name == "CloseHandle":
            self.closed.append(args[0].value)
            return 1
        if name in ("IsValidSid", "IsValidSecurityDescriptor"):
            return 1
        if name == "GetLengthSid":
            return len(self.sid) + (4 if self.wrong_length else 0)
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
            C.cast(args[0], C.POINTER(security.SECURITY_DESCRIPTOR)).contents.Revision = args[1]
            return 1
        if name == "SetSecurityDescriptorDacl":
            assert args[1] is True and args[3] is False
            sd = C.cast(args[0], C.POINTER(security.SECURITY_DESCRIPTOR)).contents
            sd.Control, sd.Dacl = 4, C.addressof(args[2])
            return 1
        raise AssertionError(name)


class Tests(unittest.TestCase):
    def test_construction_inert_live_hold_and_canonical_types(self):
        fake = FakeBoundExports()
        owner = security.NativePrivatePipeSecurity(fake)
        self.assertEqual(fake.lookups, [])
        self.assertFalse(hasattr(owner, "cp319_fake_api"))
        self.assertIs(security.SECURITY_ATTRIBUTES, abi.SECURITY_ATTRIBUTES)
        self.assertIs(security.SECURITY_DESCRIPTOR, abi.SECURITY_DESCRIPTOR)
        with self.assertRaisesRegex(security.Hold, "HARD HOLD"):
            security.create_live_private_pipe_security(fake)
        with self.assertRaises(security.Hold):
            owner.security_attributes()
        self.assertEqual(fake.calls, [])

    def test_owned_descriptor_survives_owner_reference_drop(self):
        fake = FakeBoundExports()
        owner = security.NativePrivatePipeSecurity(fake).prepare()
        ref = weakref.ref(owner)
        sa = owner.security_attributes()
        inherited = owner.security_attributes(inheritable=True)
        self.assertEqual((sa.bInheritHandle, inherited.bInheritHandle), (0, 1))
        self.assertEqual(sa.nLength, C.sizeof(abi.SECURITY_ATTRIBUTES))
        self.assertEqual(sa.lpSecurityDescriptor, inherited.lpSecurityDescriptor)
        del owner, inherited
        gc.collect()
        self.assertIsNotNone(ref())
        sd = C.cast(sa.lpSecurityDescriptor, C.POINTER(abi.SECURITY_DESCRIPTOR)).contents
        self.assertEqual((sd.Revision, sd.Control, sd.Owner, sd.Group, sd.Sacl), (1, 4, None, None, None))
        acl = C.string_at(sd.Dacl, 16 + len(fake.sid))
        self.assertEqual(struct.unpack("<BBHHH", acl[:8]), (2, 0, len(acl), 1, 0))
        self.assertEqual(struct.unpack("<BBHI", acl[8:16]), (0, 0, 20, 0x100083))
        self.assertEqual(acl[16:], fake.sid)
        self.assertEqual(fake.closed, [100])
        self.assertFalse(any(n in fake.calls for n in ("CreateNamedPipeW", "CreatePipe", "CreateProcessW")))
        del sa
        gc.collect()
        self.assertIsNone(ref())

    def test_specific_minimum_rights(self):
        self.assertEqual(security.PIPE_ACCESS_MASK, security.CLIENT_READ_ACCESS | security.CLIENT_WRITE_ACCESS)
        self.assertEqual(security.PIPE_ACCESS_MASK & (4 | 0x40000000 | 0x40000 | 0x80000), 0)

    def test_both_worker_endpoints_may_query_pipe_info_without_create_instance(self):
        # GetNamedPipeInfo requires FILE_READ_ATTRIBUTES on the inherited handle.
        self.assertEqual(security.FILE_READ_ATTRIBUTES, 0x80)
        self.assertEqual(security.CLIENT_READ_ACCESS, 0x100081)
        self.assertEqual(security.CLIENT_WRITE_ACCESS, 0x100082)
        self.assertEqual(security.PIPE_ACCESS_MASK, 0x100083)
        for access in (security.CLIENT_READ_ACCESS, security.CLIENT_WRITE_ACCESS):
            self.assertEqual(access & security.FILE_READ_ATTRIBUTES, 0x80)
            self.assertEqual(access & ~security.PIPE_ACCESS_MASK, 0)
            self.assertEqual(access & 4, 0)  # FILE_CREATE_PIPE_INSTANCE alias.

    def test_effective_thread_and_fallback_rules(self):
        for error in (0, 5, security.ERROR_NO_TOKEN):
            fake = FakeBoundExports()
            fake.thread_error = error
            owner = security.NativePrivatePipeSecurity(fake)
            if error == 5:
                with self.assertRaises(security.ApiFailure):
                    owner.prepare()
                self.assertEqual(fake.closed, [])
            else:
                owner.prepare()
                self.assertEqual(fake.closed, [99 if error == 0 else 100])
            self.assertEqual("OpenProcessToken" in fake.calls, error == security.ERROR_NO_TOKEN)

    def test_api_failures_never_publish_attributes(self):
        for operation in ("OpenProcessToken", "GetTokenInformation", "CopySid", "IsValidSid",
                          "InitializeAcl", "AddAccessAllowedAceEx", "InitializeSecurityDescriptor",
                          "SetSecurityDescriptorDacl", "IsValidSecurityDescriptor"):
            with self.subTest(operation=operation):
                fake = FakeBoundExports()
                fake.fail = operation
                owner = security.NativePrivatePipeSecurity(fake)
                with self.assertRaises(security.ApiFailure):
                    owner.prepare()
                with self.assertRaises(security.Hold):
                    owner.security_attributes()
                self.assertTrue(owner.cleanup())
                self.assertEqual(fake.closed, [] if operation == "OpenProcessToken" else [100])

    def test_bounds_checked_before_sid_api(self):
        for kind, faults in (("sid_fault", ("outside", "truncated", "revision")),
                             ("size_fault", ("small", "large", "returned"))):
            for fault in faults:
                fake = FakeBoundExports()
                setattr(fake, kind, fault)
                with self.assertRaises(security.Hold):
                    security.NativePrivatePipeSecurity(fake).prepare()
                self.assertEqual(fake.closed, [100])
                self.assertNotIn("IsValidSid", fake.calls)
        fake = FakeBoundExports()
        fake.wrong_length = True
        with self.assertRaisesRegex(security.Hold, "inconsistent token SID"):
            security.NativePrivatePipeSecurity(fake).prepare()
        self.assertNotIn("CopySid", fake.calls)

    def test_close_false_and_exception_keep_retry_owner(self):
        for mode in ("fail", "raise_on"):
            fake = FakeBoundExports()
            setattr(fake, mode, "CloseHandle")
            owner = security.NativePrivatePipeSecurity(fake)
            with self.assertRaises(security.TokenCleanupFailure) as caught:
                owner.prepare()
            retained = caught.exception.token_owner
            self.assertIs(retained, owner.token_owner)
            self.assertFalse(retained.closed)
            self.assertEqual(retained.handle, 100)
            self.assertNotIn("InitializeAcl", fake.calls)
            with self.assertRaises(security.TokenCleanupFailure):
                owner.cleanup()
            setattr(fake, mode, None)
            self.assertTrue(owner.cleanup())
            self.assertTrue(owner.cleanup())
            self.assertEqual(fake.closed, [100])
            with self.assertRaises(security.Hold):
                owner.prepare()

    def test_close_failure_preserves_prior_error(self):
        fake = FakeBoundExports()
        fake.sid_fault, fake.fail = "outside", "CloseHandle"
        with self.assertRaises(security.TokenCleanupFailure) as caught:
            security.NativePrivatePipeSecurity(fake).prepare()
        self.assertIn("token SID outside", str(caught.exception.__context__))
        fake.fail = None
        caught.exception.token_owner.retry_close()
        self.assertEqual(fake.closed, [100])

    def test_inheritance_argument_and_prepare_one_shot(self):
        owner = security.NativePrivatePipeSecurity(FakeBoundExports()).prepare()
        for value in (1, None, "True"):
            with self.assertRaises(TypeError):
                owner.security_attributes(value)
        with self.assertRaises(security.Hold):
            owner.prepare()


if __name__ == "__main__":
    unittest.main()
