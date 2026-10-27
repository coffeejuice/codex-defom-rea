"""CP325 explicit Win64 DLL binding; worker construction remains HARD HOLD.

Import performs no DLL loading. Loading resolves addresses and constructs typed
wrappers only; it never invokes an export or establishes worker-run authority.
"""
from __future__ import annotations

import ctypes as C
from types import MappingProxyType

import cp321_native_exports as abi

NATIVE_ENTRY = "HARD HOLD"
LOAD_LIBRARY_SEARCH_SYSTEM32 = 0x00000800
Hold = abi.Hold


class BoundExports(abi.BoundExports):
    """Independent typed functions with strongly retained system DLL owners.

    Read last_error immediately after the relevant native call, before any
    further native operation; the hook uses ctypes' thread-local error copy.
    """
    def __init__(self, functions, libraries, last_error):
        super().__init__(functions, libraries.values(), last_error)
        self.libraries = MappingProxyType(dict(libraries))

    @property
    def functions(self):
        return self._functions

    def fn(self, name):
        return self._functions[name]


def _address_of(library, name):
    # Do not assign argtypes/restype on WinDLL's cached function objects.
    return C.cast(getattr(library, name), C.c_void_p).value


def load_native_exports(*, dll_loader=None, address_of=None,
                        prototype_factory=None, last_error=None,
                        platform=None, pointer_size=None, machine=None):
    """Explicitly load system DLLs and bind every CP321 canonical signature.

    Injectable dependencies support inert tests without native calls. The
    default loader is ctypes.WinDLL with system32-only lookup. Every wrapper
    uses WINFUNCTYPE(use_last_error=True); the exact get_last_error hook is
    returned without calling it. Missing libraries/exports propagate failures;
    there is no alternate loader or signature fallback.
    """
    abi.require_win64(platform=platform, pointer_size=pointer_size, machine=machine)
    dll_loader = C.WinDLL if dll_loader is None else dll_loader
    address_of = _address_of if address_of is None else address_of
    prototype_factory = C.WINFUNCTYPE if prototype_factory is None else prototype_factory
    last_error = C.get_last_error if last_error is None else last_error
    if not all(callable(value) for value in
               (dll_loader, address_of, prototype_factory, last_error)):
        raise TypeError("DLL loader, address reader, prototype factory and last-error hook required")
    names = tuple(dict.fromkeys(spec.dll for spec in abi.EXPORTS.values()))
    libraries = {name: dll_loader(name, use_last_error=True,
                                 winmode=LOAD_LIBRARY_SEARCH_SYSTEM32)
                 for name in names}
    bound = abi.bind_injected_exports(
        resolve_address=lambda dll, name: address_of(libraries[dll], name),
        prototype_factory=prototype_factory, last_error=last_error,
        owners=libraries.values(), platform=platform,
        pointer_size=pointer_size, machine=machine)
    return BoundExports(bound._functions, libraries, last_error)


def create_live_exports(*_args, **_kwargs):
    raise Hold("CP325 native entry HARD HOLD: worker integration review pending")
