"""No native I/O: CP317's exact ctypes-shaped seam uses callable fakes."""
import ctypes as C
import unittest
import cp317_win32_named_pipe_abi as abi


class Fn:
    def __init__(self, callback):
        self.callback = callback
    def __call__(self, *args):
        return self.callback(*args)


class Fake:
    cp317_fake_api = True
    def __init__(self):
        self.error = 0
        self.next_handle = 100
        self.calls = []
        self.results = [(True, 48)]
        self.connect_error = abi.ERROR_IO_PENDING
        self.submit_error = abi.ERROR_IO_PENDING
        self.create_file_error = 0
        self.cancel_error = abi.ERROR_NOT_FOUND
        self.close_error = 0
        for name in abi.SIGNATURES:
            setattr(self, name, Fn(lambda *args, name=name: self.call(name, args)))
    def call(self, name, args):
        self.calls.append((name, args))
        if name in ("CreateEventW", "CreateNamedPipeW", "CreateFileW"):
            if name == "CreateFileW" and self.create_file_error:
                self.error = self.create_file_error
                return abi.INVALID_HANDLE_VALUE
            self.next_handle += 1
            return self.next_handle
        if name == "ConnectNamedPipe":
            self.error = self.connect_error
            return not self.connect_error
        if name in ("ReadFile", "WriteFile"):
            self.error = self.submit_error
            if name == "ReadFile":
                C.memset(args[1], ord("r"), args[2])
            return not self.submit_error
        if name == "GetOverlappedResultEx":
            ok, value = self.results.pop(0)
            if ok:
                C.cast(args[2], abi.LPDWORD).contents.value = value
            else:
                self.error = value
            return ok
        if name == "CancelIoEx":
            self.error = self.cancel_error
            return not self.cancel_error
        if name == "CloseHandle":
            self.error = self.close_error
            return not self.close_error
        raise AssertionError(name)


class Tests(unittest.TestCase):
    def owner(self, fake=None, clock=None):
        fake = fake or Fake()
        owner = abi.InjectedPipeABI(fake, last_error=lambda: fake.error,
                                   deadline=5., clock=clock or (lambda: 0.))
        return owner, fake

    def test_fixed_width_layout_and_signatures(self):
        owner, fake = self.owner()
        self.assertEqual(C.sizeof(abi.DWORD), 4)
        self.assertEqual(C.sizeof(abi.BOOL), 4)
        self.assertEqual(C.sizeof(abi.OVERLAPPED), 32 if C.sizeof(C.c_void_p) == 8 else 20)
        self.assertEqual(abi.OVERLAPPED.hEvent.offset, 24 if C.sizeof(C.c_void_p) == 8 else 16)
        for name, (args, result) in abi.SIGNATURES.items():
            self.assertEqual(getattr(fake, name).argtypes, args)
            self.assertIs(getattr(fake, name).restype, result)
        self.assertEqual(len(fake.ReadFile.argtypes), 5)
        self.assertIs(fake.ReadFile.argtypes[4], abi.LPOVERLAPPED)

    def test_live_factory_and_unmarked_api_hold_without_calls(self):
        with self.assertRaisesRegex(abi.Hold, "HARD HOLD"):
            abi.create_live_named_pipes()
        fake = Fake()
        fake.cp317_fake_api = False
        with self.assertRaises(abi.Hold):
            self.owner(fake)
        self.assertEqual(fake.calls, [])

    def test_two_one_way_overlapped_local_pairs_and_inheritance(self):
        owner, fake = self.owner()
        fake.results = [(True, 0), (True, 0)]
        pairs = owner.create_pair(r"\\.\pipe\cp317-unit", 123)
        self.assertEqual(len(pairs), 2)
        creates = [args for name, args in fake.calls if name == "CreateNamedPipeW"]
        clients = [args for name, args in fake.calls if name == "CreateFileW"]
        for i, args in enumerate(creates):
            self.assertEqual(args[1], abi.FILE_FLAG_OVERLAPPED | abi.FILE_FLAG_FIRST_PIPE_INSTANCE | (2 if i == 0 else 1))
            self.assertEqual(args[2:4], (8, 1))
            sa = C.cast(args[7], C.POINTER(abi.SECURITY_ATTRIBUTES)).contents
            self.assertEqual(sa.bInheritHandle, 0)
            self.assertEqual(sa.lpSecurityDescriptor, 123)
        for i, args in enumerate(clients):
            self.assertEqual(args[1], abi.CLIENT_READ_ACCESS if i == 0 else abi.CLIENT_WRITE_ACCESS)
            self.assertEqual(args[5], abi.FILE_FLAG_OVERLAPPED)
            self.assertEqual(C.cast(args[3], C.POINTER(abi.SECURITY_ATTRIBUTES)).contents.bInheritHandle, 1)
        self.assertEqual(owner.operations, [])
        for pair in pairs:
            for handle in pair:
                owner.close(handle)
        self.assertEqual(owner.handles, set())

    def test_write_client_uses_specific_rights_without_create_pipe_instance(self):
        owner, fake = self.owner()
        fake.results = [(True, 0), (True, 0)]
        owner.create_pair(r"\\.\pipe\least-privilege", 123)
        clients = [args for name, args in fake.calls if name == "CreateFileW"]
        desired = clients[1][1]
        self.assertEqual(clients[0][1], 0x00100001)
        self.assertEqual(desired, 0x00100002)
        self.assertTrue(desired & abi.FILE_WRITE_DATA)
        self.assertTrue(desired & abi.SYNCHRONIZE)
        self.assertEqual(desired & 0x4, 0)  # FILE_APPEND_DATA/FILE_CREATE_PIPE_INSTANCE
        self.assertEqual(desired & (abi.GENERIC_WRITE | abi.GENERIC_READ), 0)
        # No read/write attributes or EA access is needed by the current seam.
        self.assertEqual((clients[0][1] | desired) & (0x8 | 0x10 | 0x80 | 0x100), 0)

    def test_pipe_connected_race_is_terminal_without_result_query(self):
        owner, fake = self.owner()
        fake.connect_error = abi.ERROR_PIPE_CONNECTED
        owner.create_pair(r"\\.\pipe\race", 123)
        self.assertFalse(any(name == "GetOverlappedResultEx" for name, _ in fake.calls))

    def test_read_write_exact_buffers_and_partial_count(self):
        owner, fake = self.owner()
        owner.handles.add(1)
        self.assertEqual(owner.transfer(1, "write", b"w" * 48), 48)
        fake.results = [(True, 17)]
        self.assertEqual(owner.transfer(1, "read", 80), b"r" * 17)
        for name, args in fake.calls:
            if name in ("ReadFile", "WriteFile"):
                self.assertIsNone(args[3])
                self.assertTrue(C.cast(args[4], abi.LPOVERLAPPED).contents.hEvent)
        self.assertEqual(owner.handles, {1})

    def test_deadline_cancellation_retains_until_terminal_even_not_found(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        owner.handles.add(1)
        original = fake.GetOverlappedResultEx.callback
        def result(*args):
            answer = original(*args)
            now[0] = 5.
            return answer
        fake.GetOverlappedResultEx.callback = result
        fake.results = [(False, abi.WAIT_TIMEOUT), (False, abi.ERROR_IO_INCOMPLETE),
                        (False, abi.ERROR_OPERATION_ABORTED)]
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.transfer(1, "read", 48)
        op = owner.operations[0]
        self.assertTrue(op.cancel_requested)
        self.assertFalse(op.terminal)
        self.assertIsNotNone(op.buffer)
        with self.assertRaises(abi.Hold):
            owner.close(1)
        with self.assertRaises(abi.Hold):
            owner.close(op.event)
        self.assertEqual(owner.reap(), 0)
        owner.close(1)
        self.assertEqual(owner.handles, set())
        waits = [args[3] for name, args in fake.calls if name == "GetOverlappedResultEx"]
        self.assertEqual(waits, [5000, 0, 0])

    def test_immediate_error_is_terminal_and_event_closes(self):
        owner, fake = self.owner()
        owner.handles.add(1)
        fake.submit_error = 109
        with self.assertRaises(abi.ApiFailure) as caught:
            owner.transfer(1, "read", 48)
        self.assertEqual(caught.exception.code, 109)
        self.assertEqual(owner.operations, [])
        self.assertEqual(owner.handles, {1})

    def test_client_creation_failure_keeps_connect_until_terminal(self):
        owner, fake = self.owner()
        fake.create_file_error = 5
        fake.results = [(False, abi.ERROR_IO_INCOMPLETE), (False, abi.ERROR_OPERATION_ABORTED)]
        with self.assertRaises(abi.ApiFailure) as caught:
            owner.create_pair(r"\\.\pipe\failure", 123)
        self.assertEqual(caught.exception.code, 5)
        self.assertEqual(len(owner.operations), 1)
        self.assertEqual(owner.reap(), 0)
        for handle in tuple(owner.handles):
            owner.close(handle)

    def test_invalid_count_late_success_and_close_failure(self):
        for count in (0, 49):
            owner, fake = self.owner()
            owner.handles.add(1)
            fake.results = [(True, count)]
            with self.assertRaises(abi.Hold):
                owner.transfer(1, "write", b"x" * 48)
            self.assertEqual(owner.operations, [])
        owner, fake = self.owner()
        owner.handles.add(1)
        fake.close_error = 6
        with self.assertRaises(abi.ApiFailure):
            owner.close(1)
        self.assertIn(1, owner.handles)

    def test_late_success_is_not_retried(self):
        now = [0.]
        owner, fake = self.owner(clock=lambda: now[0])
        owner.handles.add(1)
        callback = fake.GetOverlappedResultEx.callback
        def late(*args):
            result = callback(*args)
            now[0] = 5.
            return result
        fake.GetOverlappedResultEx.callback = late
        with self.assertRaisesRegex(abi.Hold, "deadline"):
            owner.transfer(1, "write", b"x" * 48)
        self.assertEqual(owner.operations, [])
        before = len(fake.calls)
        with self.assertRaisesRegex(abi.Hold, "held"):
            owner.transfer(1, "write", b"x" * 48)
        self.assertEqual(len(fake.calls), before)
        owner.close(1)

    def test_interrupted_submit_retains_all_resources_if_cancel_throws(self):
        owner, fake = self.owner()
        owner.handles.add(1)
        def interrupted(*_):
            raise InterruptedError("injected uncertainty")
        fake.ReadFile.callback = interrupted
        fake.CancelIoEx.callback = interrupted
        with self.assertRaises(InterruptedError):
            owner.transfer(1, "read", 48)
        self.assertTrue(owner.held)
        self.assertEqual(len(owner.operations), 1)
        op = owner.operations[0]
        self.assertIsNotNone(op.buffer)
        self.assertIn(op.event, owner.handles)
        with self.assertRaises(abi.Hold):
            owner.close(1)
        fake.results = [(False, abi.ERROR_OPERATION_ABORTED)]
        self.assertEqual(owner.reap(), 0)
        owner.close(1)

    def test_no_creation_after_expired_deadline_or_without_descriptor(self):
        owner, fake = self.owner(clock=lambda: 5.)
        with self.assertRaises(abi.Hold):
            owner.create_pair(r"\\.\pipe\expired", 123)
        self.assertEqual(fake.calls, [])
        owner, fake = self.owner()
        with self.assertRaises(abi.Hold):
            owner.create_pair(r"\\.\pipe\private", None)
        self.assertEqual(fake.calls, [])


if __name__ == "__main__":
    unittest.main()
