"""CP325 binding tests: never invoke native exports or create a process."""
import ctypes as C
import importlib.util
from pathlib import Path
import platform
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cp325_real_bindings as m


class Function:
    def __init__(self, address, args, result):
        self.address, self.argtypes, self.restype = address, args, result

    def __call__(self, *_args):
        raise AssertionError("native export invocation forbidden")


class Harness:
    def __init__(self):
        self.loads, self.addresses, self.prototypes = [], [], []
        self.owners = {}
        self.last_error = lambda: 123

    def loader(self, name, **options):
        self.loads.append((name, options))
        self.owners[name] = object()
        return self.owners[name]

    def address(self, owner, name):
        self.addresses.append((owner, name))
        return 0x100000000 + len(self.addresses)

    def prototype(self, result, *args, **options):
        self.prototypes.append((result, args, options))
        return lambda address: Function(address, args, result)

    def bind(self, **overrides):
        options = dict(dll_loader=self.loader, address_of=self.address,
                       prototype_factory=self.prototype, last_error=self.last_error,
                       platform="win32", pointer_size=8, machine="AMD64")
        options.update(overrides)
        return m.load_native_exports(**options)


class LoaderTests(unittest.TestCase):
    def test_explicit_system_dlls_signatures_owners_and_error_hook(self):
        h = Harness()
        bound = h.bind()
        self.assertEqual(h.loads, [(name, dict(use_last_error=True, winmode=0x800))
                                  for name in ("kernel32.dll", "advapi32.dll")])
        self.assertEqual(dict(bound.libraries), h.owners)
        self.assertEqual(bound._owners, tuple(h.owners.values()))
        self.assertIs(bound.last_error, h.last_error)
        self.assertEqual(set(bound.functions), set(m.abi.EXPORTS))
        for (owner, name), (result, args, options) in zip(h.addresses, h.prototypes):
            spec = m.abi.EXPORTS[name]
            self.assertIs(owner, h.owners[spec.dll])
            self.assertEqual(args, spec.argtypes)
            self.assertIs(result, spec.restype)
            self.assertEqual(options, dict(use_last_error=True))
            self.assertIs(bound.fn(name), getattr(bound, name))
        self.assertEqual(len({id(fn) for fn in bound.functions.values()}), len(m.abi.EXPORTS))
        with self.assertRaises(TypeError):
            bound.functions["ReadFile"] = None
        with self.assertRaises(TypeError):
            bound.libraries["kernel32.dll"] = None
        with self.assertRaises(KeyError):
            bound.fn("unknown")
        for marker in ("cp317_fake_api", "cp319_fake_api", "cp320_fake_api"):
            self.assertFalse(hasattr(bound, marker))

    def test_independent_wrappers_across_bindings(self):
        first, second = Harness().bind(), Harness().bind()
        for name in m.abi.EXPORTS:
            self.assertIsNot(first.fn(name), second.fn(name))
        first.ReadFile.argtypes = ()
        self.assertEqual(second.ReadFile.argtypes, m.abi.EXPORTS["ReadFile"].argtypes)

    def test_platform_rejection_precedes_loading(self):
        for overrides in (dict(platform="linux"), dict(pointer_size=4), dict(machine="ARM64")):
            h = Harness()
            with self.subTest(overrides=overrides), self.assertRaises(m.Hold):
                h.bind(**overrides)
            self.assertFalse(h.loads)

    def test_invalid_dependencies_precede_loading(self):
        for dependency in ("dll_loader", "address_of", "prototype_factory", "last_error"):
            h = Harness()
            with self.subTest(dependency=dependency), self.assertRaises(TypeError):
                h.bind(**{dependency: 42})
            self.assertFalse(h.loads)

    def test_missing_library_or_export_has_no_fallback(self):
        h = Harness()
        def missing(*_args, **_kwargs):
            raise OSError("missing")
        with self.assertRaisesRegex(OSError, "missing"):
            h.bind(dll_loader=missing)
        self.assertFalse(h.addresses)
        with self.assertRaisesRegex(OSError, "missing"):
            h.bind(address_of=missing)
        self.assertFalse(h.prototypes)
        for address in (None, 0, -1, True, 1 << 64):
            with self.subTest(address=address), self.assertRaises(m.Hold):
                Harness().bind(address_of=lambda *_args: address)

    def test_default_address_reader_leaves_cached_signature_untouched(self):
        raw = object()
        class Library:
            ReadFile = raw
        sentinel = object()
        with patch.object(C, "cast", return_value=type("Pointer", (), {"value": 123})()) as cast:
            self.assertEqual(m._address_of(Library(), "ReadFile"), 123)
            cast.assert_called_once_with(raw, C.c_void_p)
        self.assertFalse(hasattr(raw, "argtypes"))

    def test_import_and_live_factory_never_load_or_bind(self):
        def forbidden(*_args, **_kwargs):
            self.fail("unexpected loading/binding/calling")
        with patch.object(C, "WinDLL", forbidden, create=True), \
             patch.object(C, "CDLL", forbidden), \
             patch.object(C, "WINFUNCTYPE", forbidden, create=True):
            spec = importlib.util.spec_from_file_location("_cp325_import_check", m.__file__)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
            with self.assertRaisesRegex(m.Hold, "HARD HOLD"):
                module.create_live_exports()

    @unittest.skipUnless(sys.platform == "win32" and C.sizeof(C.c_void_p) == 8
                         and platform.machine().lower() in ("amd64", "x86_64"),
                         "Windows x64 DLL-load-only smoke")
    def test_windows_system_dll_load_only_smoke(self):
        # This loads two Windows system libraries and resolves addresses only.
        # No bound function, including process creation or cleanup, is called.
        bound = m.load_native_exports()
        self.assertEqual(set(bound.libraries), {"kernel32.dll", "advapi32.dll"})
        self.assertIs(bound.last_error, C.get_last_error)
        for name, spec in m.abi.EXPORTS.items():
            function = bound.fn(name)
            self.assertEqual(tuple(function.argtypes), spec.argtypes)
            self.assertIs(function.restype, spec.restype)
            self.assertTrue(callable(function))


if __name__ == "__main__":
    unittest.main()
