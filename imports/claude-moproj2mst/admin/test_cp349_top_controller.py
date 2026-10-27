"""CP349 injected-only top-job tests; no process or native export is invoked."""
from pathlib import Path
import io
import json
import os
import tempfile
from unittest import mock
import sys
import threading
import unittest
from types import ModuleType, SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp349_top_controller as m
from test_cp331_outer_watchdog import Harness
from test_cp326_native_pipe_security import FakeBoundExports as SecurityExports
from test_cp328_native_pipes import Fake as PipeExports


class TopHarness(Harness):
    def dispatch(self, name, *args):
        if name == "WaitForSingleObject" and args[1] > 5000:
            self.calls.append((name, args))
            assert args[0] == 31 and args[1] <= 10000
            return 0 if self.dead else 258
        return super().dispatch(name, *args)


class ComposedOwnerHarness(TopHarness):
    """One injected export seam for real CP326, CP328, and CP349 owners."""
    SECURITY = frozenset((
        "GetCurrentThread", "OpenThreadToken", "GetCurrentProcess",
        "OpenProcessToken", "GetTokenInformation", "IsValidSid",
        "GetLengthSid", "CopySid", "InitializeAcl", "AddAccessAllowedAceEx",
        "InitializeSecurityDescriptor", "SetSecurityDescriptorDacl",
        "IsValidSecurityDescriptor"))
    PIPE = frozenset((
        "CreateNamedPipeW", "CreateFileW", "CreateEventW", "ConnectNamedPipe",
        "GetOverlappedResultEx", "GetHandleInformation", "CancelIoEx"))

    def __init__(self):
        super().__init__()
        self.security_exports = SecurityExports()
        self.pipe_exports = PipeExports()
        self.pipe_exports.next_handle = 40  # disjoint from the fixed job handle
        self.create_false = self.no_outputs = True

    def dispatch(self, name, *args):
        if name in self.SECURITY:
            self.calls.append((name, args))
            result = self.security_exports.call(name, args)
            self.error = self.security_exports.error
            return result
        if name in self.PIPE and (name != "GetHandleInformation" or
                                  args[0] in self.pipe_exports.inherit):
            self.calls.append((name, args))
            result = getattr(self.pipe_exports, name)(*args)
            self.error = self.pipe_exports.error
            return result
        if name == "CloseHandle":
            handle = getattr(args[0], "value", args[0])
            if handle == 100:
                self.calls.append((name, args))
                result = self.security_exports.call(name, args)
                self.error = self.security_exports.error
                return result
            if handle in self.pipe_exports.inherit:
                self.calls.append((name, args))
                result = self.pipe_exports.CloseHandle(*args)
                self.error = self.pipe_exports.error
                return result
        return super().dispatch(name, *args)


class TopTests(unittest.TestCase):
    @staticmethod
    def fake_channel(h, *, payload=None):
        if payload is None:
            payload = m.parent.encode_parent(
                m.parent.ParentSuccess(5200, b"n" * 32))
        class Security:
            def __init__(self, exports):
                self.exports = exports
                self.cleaned = False
                self.prepared = False
            def prepare(self):
                self.prepared = True
                return self
            def cleanup(self):
                self.cleaned = True
                return True
        class Pipe:
            def __init__(self, exports, descriptor, *, deadline):
                self.exports, self.descriptor, self.deadline = (
                    exports, descriptor, deadline)
                self.closed, self.cleaned = [], False
                self.receipt = None
                self.handles, self.operations = set(), []
            def prepare(self, name):
                assert name.startswith(r"\\.\pipe\cp349-")
                self.receipt = SimpleNamespace(
                    parent_write=10, client_read=11,
                    parent_read=15, client_write=13)
                self.handles = {10, 11, 15, 13}
                return self.receipt
            def _close(self, handle):
                self.closed.append(handle)
                self.handles.remove(handle)
            def transfer(self, handle, kind, count):
                assert (handle, kind, count) == (15, "read", 80)
                h.dead = True  # fixed child exits after its terminal write
                h.now = 200
                return payload
            def cleanup(self):
                self.cleaned = True
                self.handles.clear()
                self.receipt = None
                return True
        return Security, Pipe

    def test_import_and_public_launch_are_inert(self):
        self.assertEqual(m.NATIVE_ENTRY, "HARD HOLD")
        with self.assertRaises(m.Hold):
            m.launch_trial()

    def test_child_diagnostic_bootstrap_is_fixed_and_compiles(self):
        path = Path("C:/PROBLEM/analysis/cp349-child-failure-" + "a" * 32 + ".json")
        code = m._child_bootstrap(path, Path("C:/PROBLEM/analysis/cp349-child-trace-" + "a" * 32 + ".jsonl"))
        compile(code, "<cp349 fixed bootstrap>", "exec")
        self.assertIn("import " + m.FIXED_MODULE + " as e", code)
        self.assertNotIn("exec(", code)
        self.assertIn("open(p,'xb')", code)
        self.assertIn("open(q,'xb')", code)
        self.assertIn("os.fsync", code)
        self.assertNotIn("original_executable", code)
        self.assertNotIn("project_path", code)

    def test_child_trace_reader_accepts_partial_prefix_and_rejects_oversize(self):
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "e" * 32 + ".jsonl")
            path.write_bytes(b'{"stage":"script-entry"}\n')
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                result = m._child_trace_result(path)
                self.assertEqual(result["status"], "valid")
                self.assertEqual(result["stages"], ["script-entry"])
                self.assertEqual(len(result["sha256"]), 64)
                path.write_bytes(b'{"stage":"script-entry"}\n{"stage":"run-returned"}')
                result = m._child_trace_result(path)
                self.assertEqual(result["status"], "partial")
                self.assertEqual(result["stages"], ["script-entry"])
                path.write_bytes(b"7\n")
                self.assertEqual(m._child_trace_result(path)["status"], "malformed")
                path.write_bytes(b"x" * (m.TRACE_MAX_BYTES + 1))
                self.assertEqual(m._child_trace_result(path)["status"], "malformed")

    def test_child_trace_reader_accepts_full_fixed_entry_stage_sequence(self):
        stages = ["script-entry", "cp347-module-imported",
                  "from-client-argv-constructed", "entry",
                  "before_load_native_exports", "after_load_native_exports",
                  "before_cp334_from_bootstrap_argv", "cp334_argv_parsed",
                  "cp334_owner_created", "cp334_adopt_enter"]
        for index in (0, 1):
            stages.extend(f"cp334_h{index}_{suffix}" for suffix in (
                "before_tick1", "after_tick1", "before_inherit",
                "after_inherit", "before_tick2", "after_tick2",
                "before_pipeinfo", "after_pipeinfo"))
            stages.append(f"cp334_h{index}_pipeflags_8")
            stages.append(f"cp334_h{index}_flags_validated")
        stages.extend(("cp334_before_final_tick", "cp334_after_final_tick",
                  "after_cp334_adopt",
                  "before_cp333_acknowledge", "after_cp333_acknowledge",
                  "before_cp333_complete", "after_cp333_complete",
                  "run-returned"))
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "f" * 32 + ".jsonl")
            path.write_bytes(b"".join(
                (json.dumps({"stage": stage}, separators=(",", ":")) + "\n")
                .encode("ascii") for stage in stages))
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                result = m._child_trace_result(path)
        self.assertEqual(result["status"], "valid")
        self.assertEqual(result["stages"], stages)

    def test_child_trace_reader_preserves_rejected_pipe_flags_prefix(self):
        stages = ["script-entry", "cp347-module-imported",
                  "from-client-argv-constructed", "entry",
                  "before_load_native_exports", "after_load_native_exports",
                  "before_cp334_from_bootstrap_argv", "cp334_argv_parsed",
                  "cp334_owner_created", "cp334_adopt_enter",
                  "cp334_h0_before_tick1", "cp334_h0_after_tick1",
                  "cp334_h0_before_inherit", "cp334_h0_after_inherit",
                  "cp334_h0_before_tick2", "cp334_h0_after_tick2",
                  "cp334_h0_before_pipeinfo", "cp334_h0_after_pipeinfo",
                  "cp334_h0_pipeflags_1073741824"]
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "b" * 32 + ".jsonl")
            path.write_bytes(b"".join(
                (json.dumps({"stage": stage}, separators=(",", ":")) + "\n")
                .encode("ascii") for stage in stages))
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                result = m._child_trace_result(path)
                for invalid in ("4294967296", "01", "-1"):
                    path.write_bytes((json.dumps({"stage":
                        "cp334_h0_pipeflags_" + invalid}) + "\n").encode("ascii"))
                    self.assertEqual(m._child_trace_result(path)["status"],
                                     "malformed")
        self.assertEqual(result["status"], "valid")
        self.assertEqual(result["stages"], stages)
        self.assertNotIn("cp334_h0_flags_validated", result["stages"])

    def test_compact_live_cp334_and_cp333_trace_fits_bound(self):
        stages = ["script-entry", "cp347-module-imported",
                  "from-client-argv-constructed", "entry",
                  "before_load_native_exports", "after_load_native_exports",
                  "before_cp334_from_bootstrap_argv", "cp334_argv_parsed",
                  "cp334_owner_created", "cp334_adopt_enter"]
        for index in (0, 1):
            stages.extend((f"cp334_h{index}_pipeflags_8",
                           f"cp334_h{index}_flags_validated"))
        stages.extend(("after_cp334_adopt",
                       "before_cp333_acknowledge"))
        stages.extend(("cp333_start", "cp333_occurrence_started",
                       "cp333_before_security", "cp333_after_security",
                       "cp333_before_pipes", "cp333_after_pipes",
                       "cp333_before_worker_prepare", "cp333_after_worker_prepare",
                       "cp333_before_resume", "cp333_after_resume",
                       "cp333_before_bootstrap", "cp333_after_bootstrap",
                       "cp333_before_offer_ack", "cp333_after_offer_ack",
                        "after_cp333_acknowledge", "before_cp333_complete",
                        "cp333_complete_enter", "cp333_complete_validated",
                        "cp333_before_terminal_read", "cp333_after_terminal",
                        "cp333_after_worker", "cp333_after_runner",
                       "after_cp333_complete", "run-returned"))
        raw = b"".join((json.dumps({"stage": stage}, separators=(",", ":"))
                        + "\n").encode("ascii") for stage in stages)
        self.assertLessEqual(len(stages), 40)
        self.assertLessEqual(len(raw), m.TRACE_MAX_BYTES)
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "c" * 32 + ".jsonl")
            path.write_bytes(raw)
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                result = m._child_trace_result(path)
        self.assertEqual(result["status"], "valid")
        self.assertEqual(result["stages"], stages)

    def test_terminal_pipe_phase_and_primary_error_marker_are_schema_checked(self):
        stages = ["cp333_before_terminal_read", "cp333_pipe_wait_retry_budget",
                  "cp333_runner_result",
                  "cp333_worker_wait_exit_58153",
                  "cp333_error:Hold:absolute pipe deadline expired"]
        raw = b"".join((json.dumps({"stage": stage}, separators=(",", ":"))
                        + "\n").encode("ascii") for stage in stages)
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "d" * 32 + ".jsonl")
            path.write_bytes(raw)
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                result = m._child_trace_result(path)
        self.assertEqual(result["status"], "valid")
        self.assertEqual(result["stages"], stages)

    def test_worker_exit_marker_requires_canonical_uint32(self):
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "e" * 32 + ".jsonl")
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                for value, expected in (("0", "valid"),
                                        ("4294967295", "valid"),
                                        ("00", "malformed"),
                                        ("4294967296", "malformed")):
                    stage = "cp333_worker_wait_exit_" + value
                    path.write_bytes((json.dumps({"stage": stage}) + "\n").encode("ascii"))
                    self.assertEqual(m._child_trace_result(path)["status"], expected)

    def test_child_trace_reader_accepts_bounded_primary_error_marker(self):
        stages = ["cp333_before_offer_ack",
                  "cp333_error:ApiFailure:read failed (109)",
                  "run-exception"]
        with tempfile.TemporaryDirectory(
                dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
            root = Path(directory)
            path = root / (m.TRACE_PREFIX + "a" * 32 + ".jsonl")
            with mock.patch.object(m, "DIAGNOSTIC_DIR", root):
                path.write_bytes(b"".join(
                    (json.dumps({"stage": stage}, separators=(",", ":"))
                     + "\n").encode("ascii") for stage in stages))
                self.assertEqual(m._child_trace_result(path)["stages"], stages)
                for invalid in ("cp333_error:Hold:x\nredirect",
                                "cp333_error:Hold:" + "x" * 121):
                    path.write_bytes((json.dumps({"stage": invalid}) + "\n")
                                     .encode("ascii"))
                    self.assertEqual(m._child_trace_result(path)["status"],
                                     "malformed")

    def test_child_bootstrap_failure_success_and_exclusive_collision(self):
        class InjectedChildFailure(RuntimeError):
            pass

        class FakeEntry:
            failure = None
            construction_failure = None
            @classmethod
            def from_client_argv(cls, argv, *, stage=None):
                assert argv == ["--fixed-test"]
                assert callable(stage)
                if cls.construction_failure is not None:
                    raise cls.construction_failure
                return cls()
            def run(self):
                if type(self).failure is not None:
                    raise type(self).failure

        fake = ModuleType(m.FIXED_MODULE)
        fake.FixedParentEntry = FakeEntry
        original_module = sys.modules.get(m.FIXED_MODULE)
        sys.modules[m.FIXED_MODULE] = fake
        try:
            with tempfile.TemporaryDirectory(
                    dir=os.environ.get("CP349_TEST_TEMP") or None) as directory:
                root = Path(directory)
                path = root / (m.DIAGNOSTIC_PREFIX + "d" * 32 + ".json")
                trace = root / (m.TRACE_PREFIX + "d" * 32 + ".jsonl")
                with (mock.patch.object(m, "DIAGNOSTIC_DIR", root),
                      mock.patch.object(sys, "argv", ["runner", "--fixed-test"]),
                      mock.patch.object(sys, "path", list(sys.path)),
                      mock.patch.object(os, "fsync") as fsync):
                    failure = InjectedChildFailure("injected CP347 failure")
                    FakeEntry.failure = failure
                    with self.assertRaises(InjectedChildFailure) as caught:
                        exec(m._child_bootstrap(path, trace), {"__name__": "__main__"})
                    self.assertIs(caught.exception, failure)
                    result = m._child_diagnostic_result(path)
                    self.assertEqual(result["status"], "valid")
                    self.assertEqual(result["data"]["type"], "InjectedChildFailure")
                    self.assertLessEqual(result["bytes"], m.DIAGNOSTIC_MAX_BYTES)
                    self.assertEqual(result["bytes"], path.stat().st_size)
                    self.assertEqual(len(result["sha256"]), 64)
                    self.assertEqual(m._child_trace_result(trace)["stages"],
                                     ["script-entry", "cp347-module-imported",
                                      "from-client-argv-constructed", "run-exception",
                                      "diagnostic-write-succeeded"])
                    self.assertGreaterEqual(fsync.call_count, 5)

                    path.unlink()
                    trace.unlink()
                    FakeEntry.failure = None
                    exec(m._child_bootstrap(path, trace), {"__name__": "__main__"})
                    self.assertFalse(path.exists(), "successful child must not write diagnostic")
                    self.assertIn("run-returned", m._child_trace_result(trace)["stages"])

                    sentinel = b"existing exclusive diagnostic"
                    path.write_bytes(sentinel)
                    trace.unlink()
                    trace.write_bytes(b"existing trace")
                    FakeEntry.failure = InjectedChildFailure("collision must not overwrite")
                    with self.assertRaises(InjectedChildFailure) as collision:
                        exec(m._child_bootstrap(path, trace), {"__name__": "__main__"})
                    self.assertIs(collision.exception, FakeEntry.failure)
                    self.assertEqual(path.read_bytes(), sentinel)
                    self.assertEqual(trace.read_bytes(), b"existing trace")

                    trace.unlink()
                    with self.assertRaises(InjectedChildFailure):
                        exec(m._child_bootstrap(path, trace), {"__name__": "__main__"})
                    self.assertIn("diagnostic-write-collision",
                                  m._child_trace_result(trace)["stages"])

                    path.unlink()
                    trace.unlink()
                    FakeEntry.failure = None
                    FakeEntry.construction_failure = InjectedChildFailure("argv failure")
                    with self.assertRaises(InjectedChildFailure) as argv_error:
                        exec(m._child_bootstrap(path, trace), {"__name__": "__main__"})
                    self.assertIs(argv_error.exception, FakeEntry.construction_failure)
                    self.assertEqual(m._child_trace_result(trace)["stages"],
                                     ["script-entry", "cp347-module-imported",
                                      "from-client-argv-exception",
                                      "diagnostic-write-succeeded"])
        finally:
            FakeEntry.failure = None
            FakeEntry.construction_failure = None
            if original_module is None:
                sys.modules.pop(m.FIXED_MODULE, None)
            else:
                sys.modules[m.FIXED_MODULE] = original_module

    def test_read_failure_attaches_only_bounded_child_diagnostic(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, base_pipe = self.fake_channel(h)
        directory = Path(__file__).resolve().parent.parent / "analysis"
        path = directory / (m.DIAGNOSTIC_PREFIX + "b" * 24 +
                            format(threading.get_ident(), "x") + ".json")
        diagnostic = {"status": "valid", "bytes": 120, "sha256": "a" * 64,
                      "data": {"stage": "fixed-parent-import-or-run",
                               "type": "InjectedChildError"}}
        trace = {"status": "valid", "bytes": 128, "sha256": "b" * 64,
                 "stages": ["script-entry", "cp347-module-imported",
                            "run-exception", "diagnostic-write-succeeded"]}
        try:
            class FailedReadPipe(base_pipe):
                def transfer(self, handle, kind, count):
                    if (handle, kind, count) == (15, "read", 80):
                        raise m.pipes.ApiFailure("read", 109)
                    return super().transfer(handle, kind, count)
            with (mock.patch.object(m, "DIAGNOSTIC_DIR", directory),
                  mock.patch.object(m, "_child_diagnostic_path", return_value=path),
                  mock.patch.object(m, "_child_diagnostic_result", return_value=diagnostic),
                  mock.patch.object(m, "_child_trace_result", return_value=trace),
                  mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
                  mock.patch.object(m.pipes, "NativePipeOwner", FailedReadPipe),
                  mock.patch.object(owner, "_start_timer"),
                  mock.patch.object(owner, "_stop_timer")):
                with self.assertRaises(m.TopFailure) as caught:
                    owner.run()
            self.assertEqual(owner.child_diagnostic_result["path"], str(path))
            self.assertEqual(owner.child_diagnostic_result["status"], "valid")
            self.assertEqual(owner.child_diagnostic_result["data"]["type"],
                             "InjectedChildError")
            self.assertEqual(owner.child_diagnostic_result["bytes"], 120)
            self.assertEqual(len(owner.child_diagnostic_result["sha256"]), 64)
            self.assertEqual(owner.child_trace_result["stages"], trace["stages"])
            self.assertEqual(owner.child_trace_result["launch_argv"][0], m.outer.RUNNER_IMAGE)
            self.assertIsInstance(owner.child_trace_result["command_line"], str)
            self.assertNotIn("child_trace", owner.receipt.local)
            self.assertEqual(owner.receipt.phase, "closed")
            self.assertFalse(owner._accepted)
            self.assertIs(caught.exception.owner, owner)
        finally:
            pass

    def test_unresolved_child_cleanup_does_not_read_racing_trace(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, base_pipe = self.fake_channel(h)

        class FailedReadPipe(base_pipe):
            def transfer(self, handle, kind, count):
                if (handle, kind, count) == (15, "read", 80):
                    raise m.pipes.ApiFailure("read", 109)
                return super().transfer(handle, kind, count)

        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", FailedReadPipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer"),
              mock.patch.object(owner, "close", side_effect=lambda: owner.receipt),
              mock.patch.object(m, "_child_trace_result", side_effect=AssertionError("racing trace read")),
              mock.patch.object(m, "_child_diagnostic_result", side_effect=AssertionError("racing diagnostic read"))):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertEqual(owner.child_trace_result["status"], "unresolved-process")
        self.assertEqual(owner.child_diagnostic_result["status"], "unresolved-process")
        self.assertNotIn("child_trace", owner.receipt.local)
        self.assertNotIn("child_diagnostic", owner.receipt.local)

    def test_child_diagnostic_reader_rejects_oversize_and_bad_schema(self):
        directory = Path(__file__).resolve().parent.parent / "analysis"
        path = directory / (m.DIAGNOSTIC_PREFIX + "c" * 24 +
                            format(threading.get_ident(), "x") + ".json")
        with (mock.patch.object(m, "DIAGNOSTIC_DIR", directory),
              mock.patch.object(Path, "open", return_value=io.BytesIO(
                  b"{" + b" " * (m.DIAGNOSTIC_MAX_BYTES + 1)))):
            self.assertIsNone(m._read_child_diagnostic(path))
        malformed = b'{"type":1,"message":"x","stage":"s","traceback":[]}'
        with (mock.patch.object(m, "DIAGNOSTIC_DIR", directory),
              mock.patch.object(Path, "open", return_value=io.BytesIO(malformed)),
              mock.patch.object(Path, "exists", return_value=True)):
            self.assertEqual(m._child_diagnostic_result(path)["status"], "malformed")

    def test_security_prepared_before_pipe_using_cp326_precondition(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, pipe = self.fake_channel(h)
        class PreconditionSecurity(security):
            security_attributes = m.security.NativePrivatePipeSecurity.security_attributes
            def __init__(self, exports):
                super().__init__(exports)
                self._sd = m.security.SECURITY_DESCRIPTOR()
        class PreconditionPipe(pipe):
            def prepare(self, name):
                attributes = self.descriptor.security_attributes()
                self.assert_descriptor(attributes)
                return super().prepare(name)
            def assert_descriptor(self, attributes):
                assert attributes._descriptor_owner is self.descriptor
                assert not attributes.bInheritHandle
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", PreconditionSecurity),
              mock.patch.object(m.pipes, "NativePipeOwner", PreconditionPipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            result = owner.run()
        self.assertEqual(result.phase, "closed-success")
        self.assertTrue(owner.security.prepared)
        self.assertTrue(owner.security.cleaned)

    def test_security_prepare_failure_retains_cleanup_owner(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, pipe = self.fake_channel(h)
        class FailedSecurity(security):
            def prepare(self):
                raise m.Hold("injected security preparation failure")
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", FailedSecurity),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaisesRegex(m.TopFailure, "injected security preparation failure"):
                owner.run()
        self.assertIsInstance(owner.security, FailedSecurity)
        self.assertTrue(owner.security.cleaned)
        self.assertIsNone(owner.pipe_owner)

    def test_real_security_and_pipe_compose_until_injected_create_failure(self):
        h = ComposedOwnerHarness()
        owner = m.TopController(h.exports)
        with (mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaisesRegex(m.TopFailure, "CreateProcessW"):
                owner.run()
        names = [name for name, _ in h.calls]
        self.assertIn("SetSecurityDescriptorDacl", names)
        self.assertEqual(names.count("CreateNamedPipeW"), 2)
        self.assertLess(names.index("SetSecurityDescriptorDacl"),
                        names.index("CreateNamedPipeW"))
        self.assertIn("CreateProcessW", names)
        self.assertNotIn("ResumeThread", names)
        self.assertFalse(h.created)
        self.assertEqual(h.security_exports.closed, [100])
        self.assertEqual(owner.pipe_owner.handles, set())
        self.assertEqual(owner.pipe_owner.operations, [])
        self.assertIsNone(owner.pipe_owner.receipt)
        self.assertEqual(h.pipe_exports.inherit, {})
        self.assertEqual(owner._owned_handles, {})
        self.assertTrue(owner._no_child_proven)
        self.assertEqual(owner.receipt.create_outcome, "returned-failure-no-child")
        self.assertEqual(owner.receipt.phase, "hold")

    def test_fixed_cp347_suspended_creation_and_exact_inheritance(self):
        h = Harness()
        owner = m.TopController(h.exports)
        self.assertEqual(owner.start_occurrence(), 10100)
        result = owner.prepare_fixed_suspended(child_read=11, child_write=13)
        self.assertIs(result, owner.receipt)
        self.assertEqual(result.phase, "prepared-suspended")
        self.assertEqual(h.attributes[m.outer.JOB_LIST], (21,))
        self.assertEqual(h.attributes[m.outer.HANDLE_LIST], (11, 13))
        args = h.creation
        self.assertEqual(args[0], m.outer.RUNNER_IMAGE)
        self.assertEqual(args[5], m.creator.CREATE_FLAGS)
        self.assertEqual(args[7], m.outer.RUNNER_CWD)
        cmd = args[1].value
        self.assertIn("cp347_fixed_parent_entry", cmd)
        self.assertIn("FixedParentEntry.from_client_argv(sys.argv[1:],stage=_stage)", cmd)
        self.assertIn("--cp334-client 11 13 5100", cmd)
        self.assertEqual(owner._channel_deadline, 5100)
        self.assertIn(" -I -B -c ", cmd)
        self.assertNotIn("cp335_fixed_runner_entry", cmd)
        self.assertNotIn("DEF_GUI_MO", cmd)
        self.assertEqual(owner._owned_handles,
                         {"outer_job": 21, "runner_process": 31, "runner_thread": 32})
        self.assertFalse(h.dead)
        self.assertNotIn("ResumeThread", [name for name, _ in h.calls])

    def test_independent_timer_targets_only_owned_top_job(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        owner.start_occurrence()
        m.creator.NativeWorkerCreator._job(owner, "outer_job")
        with mock.patch.object(owner._timer_cancel, "wait", return_value=False):
            owner._timer_body()
        self.assertTrue(owner._timer_fired)
        self.assertTrue(h.dead)
        self.assertEqual([args[0] for name, args in h.calls
                          if name == "TerminateJobObject"], [21])
        self.assertNotIn("TerminateProcess", [name for name, _ in h.calls])
        with self.assertRaises(m.Hold):
            owner._budget()
        self.assertTrue(owner._abort_job_terminated)
        owner.close()
        self.assertEqual([name for name, _ in h.calls].count("TerminateJobObject"), 1)

    def test_one_fixed_claim_needs_exact_exit_and_empty_top_job(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            result = owner.run()
        self.assertIs(result, owner.receipt)
        self.assertEqual(result.phase, "closed-success")
        self.assertEqual(owner.claim.nonce, b"n" * 32)
        self.assertEqual((owner.deadline, owner._channel_deadline,
                          owner.claim.deadline), (10100, 5100, 5200))
        self.assertEqual(owner.pipe_owner.closed, [11, 13])
        self.assertTrue(owner.pipe_owner.cleaned and owner.security.cleaned)
        names = [name for name, _ in h.calls]
        self.assertLess(names.index("ResumeThread"), names.index("GetExitCodeProcess"))
        self.assertEqual(h.closed, [32, 31, 21])
        self.assertNotIn("TerminateJobObject", names)
        self.assertTrue(owner._accepted)

    def test_malformed_private_claim_aborts_exact_top_job(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, pipe = self.fake_channel(h, payload=b"x" * 80)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertIn("TerminateJobObject", [name for name, _ in h.calls])
        self.assertNotEqual(owner.receipt.phase, "closed-success")

    def _run_failed_claim_read(self, *, exit_state, query_failure=False):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, pipe = self.fake_channel(h)
        def fail_read(_pipe, handle, kind, count):
            assert (handle, kind, count) == (15, "read", 80)
            h.dead = (exit_state == "signaled")
            raise OSError(109, "injected broken pipe")
        if query_failure:
            h.fail = "GetExitCodeProcess"
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(pipe, "transfer", fail_read),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        names = [name for name, _ in h.calls]
        self.assertLess(names.index("WaitForSingleObject"),
                        names.index("TerminateJobObject"))
        return owner, h

    def test_claim_read_failure_records_signaled_exit_before_cleanup(self):
        owner, h = self._run_failed_claim_read(exit_state="signaled")
        self.assertEqual(owner.claim_process_exit_observation,
                         {"status": "exited-before-cleanup", "exit_code": 0,
                          "error": None})
        self.assertEqual([name for name, _ in h.calls].count("TerminateJobObject"), 1)

    def test_claim_read_failure_records_active_before_cleanup_on_timeout(self):
        owner, _ = self._run_failed_claim_read(exit_state="active")
        self.assertEqual(owner.claim_process_exit_observation,
                         {"status": "active-before-cleanup", "exit_code": None,
                          "error": None})

    def test_claim_read_failure_preserves_query_failure_as_unknown(self):
        owner, _ = self._run_failed_claim_read(
            exit_state="signaled", query_failure=True)
        self.assertEqual(owner.claim_process_exit_observation["status"],
                         "unknown-before-cleanup")
        self.assertIn("GetExitCodeProcess", owner.claim_process_exit_observation["error"])

    def test_valid_claim_cannot_renew_top_safety_deadline(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        payload = m.parent.encode_parent(
            m.parent.ParentSuccess(10101, b"n" * 32))
        security, pipe = self.fake_channel(h, payload=payload)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertIn("TerminateJobObject", [name for name, _ in h.calls])

    def test_claim_mutation_during_exit_query_prevents_acceptance(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        original = h.dispatch
        def mutate(name, *args):
            value = original(name, *args)
            if name == "GetExitCodeProcess":
                object.__setattr__(owner.claim, "nonce", b"x" * 32)
            return value
        h.dispatch = mutate
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertIn("TerminateJobObject", [name for name, _ in h.calls])

    def test_parent_read_redirect_after_resume_cannot_route_claim(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        original = h.dispatch
        def redirect(name, *args):
            value = original(name, *args)
            if name == "ResumeThread":
                owner._channel_receipt.parent_read = 999
            return value
        h.dispatch = redirect
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertTrue(owner._channel_custody_uncertain)
        self.assertEqual(owner.pipe_owner.handles, {10, 15})
        self.assertFalse(owner.pipe_owner.cleaned)
        self.assertIn("TerminateJobObject", [name for name, _ in h.calls])

    def test_parent_read_redirect_during_valid_claim_is_rejected(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, base_pipe = self.fake_channel(h)
        class RedirectPipe(base_pipe):
            def transfer(self, handle, kind, count):
                result = super().transfer(handle, kind, count)
                self.receipt.parent_read = 999
                return result
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", RedirectPipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertTrue(owner._channel_custody_uncertain)
        self.assertEqual(owner.pipe_owner.handles, {10, 15})
        self.assertFalse(owner.pipe_owner.cleaned)

    def test_paired_handle_and_diagnostic_ledger_mutation_is_rejected(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        original = h.dispatch
        def redirect(name, *args):
            value = original(name, *args)
            if name == "ResumeThread":
                owner.pipe_owner.handles.remove(15)
                owner.pipe_owner.handles.add(999)
                owner._channel_expected = frozenset({10, 999})
            return value
        h.dispatch = redirect
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertTrue(owner._channel_custody_uncertain)
        self.assertFalse(owner.pipe_owner.cleaned)
        self.assertIn(999, owner.pipe_owner.handles)

    def test_replacing_pipe_owner_with_equal_clone_is_rejected(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        original_dispatch = h.dispatch
        retained = []
        def replace(name, *args):
            value = original_dispatch(name, *args)
            if name == "ResumeThread":
                original_pipe = owner.pipe_owner
                retained.append(original_pipe)
                clone = type(original_pipe)(h.exports, owner.security,
                                            deadline=owner.deadline)
                clone.receipt = original_pipe.receipt
                clone.handles = set(original_pipe.handles)
                owner.pipe_owner = clone
            return value
        h.dispatch = replace
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertIs(owner.pipe_owner, retained[0])
        self.assertEqual(retained[0].handles, {10, 15})
        self.assertFalse(retained[0].cleaned)

    def test_terminal_cleanup_crossing_top_deadline_stays_unaccepted(self):
        h = TopHarness()
        owner = m.TopController(h.exports)
        security, base_pipe = self.fake_channel(h)
        class LatePipe(base_pipe):
            def cleanup(self):
                h.now = 10100
                return super().cleanup()
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", LatePipe),
              mock.patch.object(owner, "_start_timer"),
              mock.patch.object(owner, "_stop_timer")):
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertFalse(owner._accepted)
        self.assertNotEqual(owner.receipt.phase, "closed-success")

    def test_independent_timer_terminates_job_during_blocked_create(self):
        h = Harness()
        entered, released = threading.Event(), threading.Event()
        original = h.dispatch
        def blocked(name, *args):
            if name == "CreateProcessW":
                entered.set()
                if not released.wait(1):
                    raise AssertionError("independent timer did not run")
            if name == "TerminateJobObject":
                released.set()
            return original(name, *args)
        h.dispatch = blocked
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m, "SAFETY_MS", 30),
              mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe)):
            owner = m.TopController(h.exports)
            with self.assertRaises(m.TopFailure):
                owner.run()
        self.assertTrue(entered.is_set() and released.is_set())
        self.assertTrue(owner._timer_fired)
        self.assertFalse(owner._accepted)
        names = [name for name, _ in h.calls]
        self.assertIn("TerminateJobObject", names)
        self.assertNotIn("ResumeThread", names)

    def test_partial_published_create_output_keeps_exact_cleanup_owner(self):
        h = Harness()
        h.create_raise = True
        security, pipe = self.fake_channel(h)
        with (mock.patch.object(m.security, "NativePrivatePipeSecurity", security),
              mock.patch.object(m.pipes, "NativePipeOwner", pipe),
              mock.patch.object(m.TopController, "_start_timer"),
              mock.patch.object(m.TopController, "_stop_timer")):
            owner = m.TopController(h.exports)
            with self.assertRaises(m.TopFailure) as caught:
                owner.run()
        self.assertIs(caught.exception.owner, owner)
        self.assertEqual(owner.receipt.pid, 33)
        self.assertIn("TerminateJobObject", [name for name, _ in h.calls])
        self.assertNotIn("ResumeThread", [name for name, _ in h.calls])
        self.assertFalse(owner._accepted)


if __name__ == "__main__":
    unittest.main()
