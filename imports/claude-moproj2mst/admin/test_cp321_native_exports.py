"""CP321 inert ABI tests. No DLL, export, process, GUI or original is run."""
import ctypes as C
import importlib.util
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp321_native_exports as m


class InertFunction:
    def __init__(self, address, args, result):
        self.address, self.argtypes, self.restype = address, list(args), result

    def __call__(self, *_args):
        raise AssertionError("tests must never execute an export")


class Prototypes:
    def __init__(self):
        self.calls = []

    def __call__(self, result, *args, **options):
        self.calls.append((result, args, options))
        return lambda address: InertFunction(address, args, result)


def bind(**overrides):
    options = dict(resolve_address=lambda dll, name: 0x100000001,
                   prototype_factory=Prototypes(), last_error=lambda: 123,
                   platform="win32", pointer_size=8, machine="AMD64")
    options.update(overrides)
    return m.bind_injected_exports(**options)


class LayoutTests(unittest.TestCase):
    def test_peek_named_pipe_exact_canonical_signature(self):
        self.assertEqual(m.SIGNATURES["PeekNamedPipe"],
            ((m.HANDLE, m.PVOID, m.DWORD, m.LPDWORD, m.LPDWORD, m.LPDWORD), m.BOOL))
        self.assertEqual(m.EXPORTS["PeekNamedPipe"].dll, "kernel32.dll")

    @unittest.skipUnless(C.sizeof(C.c_void_p) == 8, "x64 layout checks")
    def test_canonical_x64_sizes_and_offsets(self):
        expected = {
            m.SECURITY_ATTRIBUTES: 24, m.OVERLAPPED: 32,
            m.SID_AND_ATTRIBUTES: 16, m.SECURITY_DESCRIPTOR: 40,
            m.FILETIME: 8, m.BY_HANDLE_FILE_INFORMATION: 52,
            m.FILE_ID_128: 16, m.FILE_ID_INFO: 24,
            m.STARTUPINFOW: 104, m.STARTUPINFOEXW: 112,
            m.PROCESS_INFORMATION: 24, m.BASIC_LIMIT: 64,
            m.IO_COUNTERS: 48, m.EXTENDED_LIMIT: 144, m.BASIC_ACCOUNTING: 48,
            m.PROCESSENTRY32W: 568,
        }
        for structure, size in expected.items():
            with self.subTest(structure=structure.__name__):
                self.assertEqual(C.sizeof(structure), size)
        for structure, field, offset in (
            (m.SECURITY_ATTRIBUTES, "lpSecurityDescriptor", 8),
            (m.SECURITY_ATTRIBUTES, "bInheritHandle", 16),
            (m.OVERLAPPED, "location", 16), (m.OVERLAPPED, "hEvent", 24),
            (m.SECURITY_DESCRIPTOR, "Dacl", 32),
            (m.STARTUPINFOW, "hStdInput", 80),
            (m.STARTUPINFOEXW, "lpAttributeList", 104),
            (m.BASIC_LIMIT, "Affinity", 48),
            (m.EXTENDED_LIMIT, "IoInfo", 64),
            (m.PROCESSENTRY32W, "th32DefaultHeapID", 16),
            (m.PROCESSENTRY32W, "th32ModuleID", 24),
            (m.PROCESSENTRY32W, "pcPriClassBase", 36),
            (m.PROCESSENTRY32W, "szExeFile", 44),
        ):
            self.assertEqual(getattr(structure, field).offset, offset)
        self.assertEqual(C.sizeof(m.DWORD), 4)
        self.assertEqual(C.sizeof(m.BOOL), 4)
        self.assertEqual(C.sizeof(m.SIZE_T), 8)

    def test_toolhelp_exports_have_canonical_kernel32_signatures(self):
        entry = C.POINTER(m.PROCESSENTRY32W)
        expected = {
            "CreateToolhelp32Snapshot": ((m.DWORD, m.DWORD), m.HANDLE),
            "Process32FirstW": ((m.HANDLE, entry), m.BOOL),
            "Process32NextW": ((m.HANDLE, entry), m.BOOL),
        }
        for name, signature in expected.items():
            with self.subTest(name=name):
                spec = m.EXPORTS[name]
                self.assertEqual(spec.dll, "kernel32.dll")
                self.assertEqual((spec.argtypes, spec.restype), signature)
        bound = bind()
        for name, signature in expected.items():
            function = getattr(bound, name)
            self.assertEqual(tuple(function.argtypes), signature[0])
            self.assertIs(function.restype, signature[1])

    def test_export_map_covers_all_legacy_consumers(self):
        import cp317_win32_named_pipe_abi as pipes
        import cp319_pipe_security as security
        import cp320_retained_image_pin as pin
        import cp313_native_worker_creator as creator
        import cp310_native_job_adapter as cleanup
        for module in (pipes, security, pin, creator, cleanup):
            self.assertFalse(set(module.SIGNATURES) - set(m.EXPORTS))
            for name, (args, _result) in module.SIGNATURES.items():
                self.assertEqual(len(m.EXPORTS[name].argtypes), len(args), name)

    def test_signatures_preserve_pointer_and_scalar_width(self):
        self.assertEqual(m.SIGNATURES["GetTickCount64"], ((), C.c_uint64))
        self.assertEqual(m.SIGNATURES["ResumeThread"], ((m.HANDLE,), m.DWORD))
        self.assertEqual(m.SIGNATURES["GetExitCodeProcess"], ((m.HANDLE, m.LPDWORD), m.BOOL))
        self.assertEqual(m.EXPORTS["DeleteProcThreadAttributeList"].restype, None)
        self.assertIs(m.EXPORTS["ReadFile"].argtypes[-1], C.POINTER(m.OVERLAPPED))
        self.assertIs(m.EXPORTS["CreateFileW"].argtypes[3], C.POINTER(m.SECURITY_ATTRIBUTES))
        self.assertIs(m.EXPORTS["CreateNamedPipeW"].argtypes[-1], C.POINTER(m.SECURITY_ATTRIBUTES))
        self.assertIs(m.EXPORTS["GetFileInformationByHandle"].argtypes[1],
                      C.POINTER(m.BY_HANDLE_FILE_INFORMATION))
        self.assertIs(m.EXPORTS["UpdateProcThreadAttribute"].argtypes[-1], C.POINTER(m.SIZE_T))
        self.assertIs(m.EXPORTS["GetFinalPathNameByHandleW"].argtypes[1], m.LPWSTR)
        self.assertIs(m.EXPORTS["CreateProcessW"].argtypes[1], m.LPWSTR)

    def test_get_named_pipe_info_has_four_dword_outputs(self):
        spec = m.EXPORTS["GetNamedPipeInfo"]
        self.assertEqual(spec.dll, "kernel32.dll")
        self.assertEqual(spec.argtypes, (m.HANDLE,) + (m.LPDWORD,) * 4)
        self.assertIs(spec.restype, m.BOOL)
        factory = Prototypes()
        functions = bind(prototype_factory=factory)
        self.assertEqual(functions.GetNamedPipeInfo.argtypes, list(spec.argtypes))
        self.assertIs(functions.GetNamedPipeInfo.restype, m.BOOL)
        self.assertNotIn("GetNamedPipeHandleStateW", m.EXPORTS)

    def test_explicit_dll_membership_and_immutable_declarations(self):
        advapi = {"OpenThreadToken", "OpenProcessToken", "GetTokenInformation", "IsValidSid",
                  "GetLengthSid", "CopySid", "InitializeAcl", "AddAccessAllowedAceEx",
                  "InitializeSecurityDescriptor", "SetSecurityDescriptorDacl", "IsValidSecurityDescriptor"}
        self.assertEqual({name for name, spec in m.EXPORTS.items()
                          if spec.dll == "advapi32.dll"}, advapi)
        self.assertEqual({spec.dll for spec in m.EXPORTS.values()}, {"kernel32.dll", "advapi32.dll"})
        with self.assertRaises(TypeError):
            m.EXPORTS["missing"] = None
        with self.assertRaises(TypeError):
            m.SIGNATURES["ReadFile"] = None


class BinderTests(unittest.TestCase):
    def test_injected_binding_uses_fresh_wrappers_and_last_error(self):
        seen, factory = [], Prototypes()
        owner = object()
        first = bind(resolve_address=lambda dll, name: seen.append((dll, name)) or 0x100000001,
                     prototype_factory=factory, owners=(owner,))
        second = bind()
        self.assertEqual(len(seen), len(m.EXPORTS))
        self.assertEqual(set(seen), {(spec.dll, name) for name, spec in m.EXPORTS.items()})
        self.assertEqual(first._owners, (owner,))
        self.assertEqual(first.last_error(), 123)
        self.assertTrue(all(options == {"use_last_error": True} for _, _, options in factory.calls))
        for name, spec in m.EXPORTS.items():
            self.assertEqual(getattr(first, name).argtypes, list(spec.argtypes))
            self.assertIs(getattr(first, name).restype, spec.restype)
            self.assertIsNot(getattr(first, name), getattr(second, name))
        first.ReadFile.argtypes = [C.c_int]
        self.assertEqual(second.ReadFile.argtypes, list(m.EXPORTS["ReadFile"].argtypes))
        self.assertEqual(len(m.EXPORTS["ReadFile"].argtypes), 5)
        self.assertIsNot(first.CreateFileW, first.CreateNamedPipeW)
        self.assertFalse(hasattr(first, "cp317_fake_api"))
        self.assertFalse(hasattr(first, "cp319_fake_api"))
        self.assertFalse(hasattr(first, "cp320_fake_api"))

    def test_platform_refusal_precedes_resolver(self):
        def forbidden(*_args):
            self.fail("unsupported platform must not resolve or load anything")
        for options in (dict(platform="linux"), dict(pointer_size=4), dict(machine="ARM64")):
            with self.subTest(options=options), self.assertRaises(m.Hold):
                bind(resolve_address=forbidden, prototype_factory=forbidden, **options)

    def test_missing_exports_and_reused_wrapper_refused(self):
        for address in (0, None, -1, True, 1 << 64):
            with self.subTest(address=address), self.assertRaises(m.Hold):
                bind(resolve_address=lambda *_args: address)
        shared = InertFunction(1, (), None)
        with self.assertRaisesRegex(m.Hold, "independent"):
            bind(prototype_factory=lambda *_args, **_kwargs: lambda _address: shared)

    def test_invalid_hooks_refused(self):
        for name in ("resolve_address", "prototype_factory", "last_error"):
            with self.subTest(name=name), self.assertRaises(TypeError):
                bind(**{name: None})

    def test_import_and_live_hold_make_no_dll_calls(self):
        def forbidden(*_args, **_kwargs):
            self.fail("DLL loading or export calling is forbidden")
        with patch.object(C, "WinDLL", forbidden, create=True), \
             patch.object(C, "CDLL", forbidden), \
             patch.object(C, "WINFUNCTYPE", forbidden, create=True):
            spec = importlib.util.spec_from_file_location("_cp321_import_check", m.__file__)
            module = importlib.util.module_from_spec(spec)
            sys.modules[spec.name] = module
            try:
                spec.loader.exec_module(module)
                with self.assertRaisesRegex(module.Hold, "HARD HOLD"):
                    module.create_live_exports()
            finally:
                del sys.modules[spec.name]


if __name__ == "__main__":
    unittest.main()
