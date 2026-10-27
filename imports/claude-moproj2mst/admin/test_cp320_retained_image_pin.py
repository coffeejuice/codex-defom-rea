"""CP320 injected tests: no native file open, DLL, process, GUI or original."""
import ctypes as C
import hashlib
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp320_retained_image_pin as m

DATA = b"F" * m.WORKER_BYTES


class Function:
    def __init__(self, call):
        self.call = call

    def __call__(self, *args):
        return self.call(*args)


def out(pointer, kind, value):
    C.cast(pointer, C.POINTER(kind)).contents.value = value


class FakeApi:
    cp320_fake_api = True

    def __init__(self):
        self.calls, self.offsets = [], {}
        self.next_handle = 90
        self.data = DATA
        self.inherit = False
        self.directory = False
        self.file_id = 7
        self.volume = 12
        self.size = len(DATA)
        self.final_path = "\\\\?\\" + m.WORKER_IMAGE
        self.fail = None
        self.close_fail = False
        self.reopen_id = None
        self.read_count = None
        self.truncated = False
        for name in m.SIGNATURES:
            setattr(self, name, Function(lambda *args, name=name: self.dispatch(name, *args)))

    def dispatch(self, name, *args):
        self.calls.append((name, args))
        if self.fail == name:
            return 0
        if name == "CreateFileW":
            self.next_handle += 1
            self.offsets[self.next_handle] = 0
            return self.next_handle
        handle = args[0]
        if name == "GetHandleInformation":
            out(args[1], m.DWORD, int(self.inherit))
        elif name == "GetFileInformationByHandle":
            value = C.cast(args[1], C.POINTER(m.BY_HANDLE_FILE_INFORMATION)).contents
            value.dwFileAttributes = m.FILE_ATTRIBUTE_DIRECTORY if self.directory else 0x80
            value.nFileSizeHigh, value.nFileSizeLow = self.size >> 32, self.size & 0xffffffff
            value.dwVolumeSerialNumber = 12
            value.nFileIndexLow = 7
        elif name == "GetFileInformationByHandleEx":
            value = C.cast(args[2], C.POINTER(m.FILE_ID_INFO)).contents
            value.VolumeSerialNumber = self.volume
            value.FileId.Identifier[0] = (self.reopen_id if handle > 91 and self.reopen_id is not None
                                          else self.file_id)
        elif name == "GetFinalPathNameByHandleW":
            args[1].value = self.final_path
            return len(args[1]) if self.truncated else len(self.final_path)
        elif name == "SetFilePointerEx":
            self.offsets[handle] = 0
        elif name == "ReadFile":
            offset = self.offsets[handle]
            data = self.data[offset:offset + args[2]]
            C.memmove(args[1], data, len(data))
            self.offsets[handle] += len(data)
            out(args[3], m.DWORD, len(data) if self.read_count is None else self.read_count)
        elif name == "CloseHandle" and self.close_fail:
            return 0
        return 1


class PinTests(unittest.TestCase):
    def setUp(self):
        # Synthetic bytes substitute only the expected digest; no reviewed PE is
        # read or claimed attested by these tests.
        self.digest = patch.object(m, "WORKER_SHA256", hashlib.sha256(DATA).hexdigest())
        self.digest.start()
        self.addCleanup(self.digest.stop)
        self.api = FakeApi()
        self.pin = m.FakeRetainedImagePin(self.api, last_error=lambda: 5)

    def test_abi(self):
        self.assertEqual(C.sizeof(m.BY_HANDLE_FILE_INFORMATION), 52)
        self.assertEqual(C.sizeof(m.FILE_ID_INFO), 24)
        self.assertEqual(m.FILE_ID_INFO.FileId.offset, 8)
        for name, (args, result) in m.SIGNATURES.items():
            self.assertEqual(getattr(self.api, name).argtypes, args)
            self.assertIs(getattr(self.api, name).restype, result)

    def test_locked_noninherit_fixed_open_and_retained_hash(self):
        snapshot = self.pin.prepare()
        self.assertEqual(snapshot.identity.size, m.WORKER_BYTES)
        self.assertEqual(self.api.calls[0], ("CreateFileW", (
            m.WORKER_IMAGE, m.GENERIC_READ, m.FILE_SHARE_READ, None,
            m.OPEN_EXISTING, m.FILE_ATTRIBUTE_NORMAL, None)))
        self.assertEqual(self.pin.handle, 91)
        self.assertNotIn("CloseHandle", [n for n, _ in self.api.calls])
        self.assertTrue(all(args[0] == 91 for n, args in self.api.calls if n == "ReadFile"))
        with self.assertRaises(m.Hold):
            self.pin.prepare()

    def test_post_create_reopens_canonical_and_keeps_original(self):
        snapshot = self.pin.prepare()
        self.assertEqual(self.pin.verify_after_create(), snapshot)
        self.assertTrue(self.pin.post_create_verified)
        opens = [args for name, args in self.api.calls if name == "CreateFileW"]
        self.assertEqual(opens[1][0], snapshot.identity.final_path)
        closes = [args[0] for name, args in self.api.calls if name == "CloseHandle"]
        self.assertEqual(closes, [92])
        self.assertEqual(self.pin.handle, 91)
        self.pin.close()
        self.assertIsNone(self.pin.handle)
        self.assertFalse(self.pin.post_create_verified)
        with self.assertRaises(m.Hold):
            self.pin.verify_after_create()

    def test_same_path_and_digest_different_id_fails(self):
        self.pin.prepare()
        self.api.reopen_id = 8
        with self.assertRaisesRegex(m.Hold, "different file"):
            self.pin.verify_after_create()
        self.assertFalse(self.pin.post_create_verified)
        self.assertEqual(self.pin.handle, 91)
        self.assertIsNone(self.pin.reopened_handle)

    def test_retained_identity_change_fails_before_reopen(self):
        for attribute, value in (("file_id", 8), ("volume", 15)):
            with self.subTest(attribute=attribute):
                api = FakeApi()
                pin = m.FakeRetainedImagePin(api, last_error=lambda: 5)
                pin.prepare()
                setattr(api, attribute, value)
                with self.assertRaises(m.Hold):
                    pin.verify_after_create()
                self.assertEqual(api.next_handle, 91)
                self.assertEqual(pin.handle, 91)

    def test_wrong_hash_size_directory_inheritance_path_truncation_and_read_bounds(self):
        cases = (("data", b"G" * len(DATA)), ("data", DATA[:-1]),
                 ("size", len(DATA) + 1), ("directory", True), ("inherit", True),
                 ("final_path", r"C:\other\worker.exe"), ("truncated", True),
                 ("read_count", 65537))
        for attribute, value in cases:
            with self.subTest(attribute=attribute):
                api = FakeApi()
                setattr(api, attribute, value)
                pin = m.FakeRetainedImagePin(api, last_error=lambda: 5)
                with self.assertRaises(m.Hold):
                    pin.prepare()
                self.assertEqual(pin.handle, 91)
                self.assertIsNone(pin.snapshot)

    def test_api_failure_retains_open_handle(self):
        for name in m.SIGNATURES:
            if name == "CloseHandle":
                continue
            with self.subTest(name=name):
                api = FakeApi()
                api.fail = name
                pin = m.FakeRetainedImagePin(api, last_error=lambda: 5)
                with self.assertRaises(m.ApiFailure) as context:
                    pin.prepare()
                self.assertEqual(context.exception.operation, name)
                self.assertEqual(context.exception.code, 5)
                self.assertEqual(pin.handle, None if name == "CreateFileW" else 91)

    def test_reopen_failure_retains_original(self):
        self.pin.prepare()
        self.api.fail = "CreateFileW"
        with self.assertRaises(m.ApiFailure):
            self.pin.verify_after_create()
        self.assertEqual(self.pin.handle, 91)
        self.assertFalse(self.pin.post_create_verified)

    def test_uncertain_close_retains_both_handles_and_blocks_reverify(self):
        self.pin.prepare()
        self.api.close_fail = True
        with self.assertRaises(m.ApiFailure):
            self.pin.verify_after_create()
        self.assertEqual((self.pin.handle, self.pin.reopened_handle), (91, 92))
        with self.assertRaises(m.Hold):
            self.pin.verify_after_create()
        with self.assertRaises(m.ApiFailure):
            self.pin.close()
        self.assertEqual((self.pin.handle, self.pin.reopened_handle), (91, 92))
        self.api.close_fail = False
        self.pin.close()
        self.assertEqual((self.pin.handle, self.pin.reopened_handle), (None, None))

    def test_live_entry_and_unmarked_injection_hold(self):
        with self.assertRaises(m.Hold):
            m.open_live_pin()
        self.api.cp320_fake_api = False
        with self.assertRaises(m.Hold):
            m.FakeRetainedImagePin(self.api, last_error=lambda: 5)
        with self.assertRaises(m.Hold):
            self.pin.verify_after_create()


class ReviewedConstantTests(unittest.TestCase):
    def test_reviewed_cp316_constants(self):
        self.assertEqual(m.WORKER_IMAGE, r"C:\PROBLEM\s316\worker.exe")
        self.assertEqual(m.WORKER_BYTES, 104960)
        self.assertEqual(m.WORKER_SHA256,
                         "19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985")


if __name__ == "__main__":
    unittest.main()
