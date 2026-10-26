"""Execute original 0x140074f50 bytes with synthetic, traced connector interfaces.

This is a bounded original-function experiment, not an original GUI capture.
The inspected function has only internal relative branches and virtual calls;
all virtual calls resolve to the callbacks below. No target startup code runs.
"""
import ctypes as C
import hashlib
import itertools
import json
from pathlib import Path
import struct
import time

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"
EXPECTED = "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc"
START, END = 0x140074F50, 0x140075041


def original_bytes():
    data = TARGET.read_bytes()
    if hashlib.sha256(data).hexdigest() != EXPECTED:
        raise ValueError("Original executable identity mismatch")
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    count = struct.unpack_from("<H", data, pe + 6)[0]
    opt_size = struct.unpack_from("<H", data, pe + 20)[0]
    base = struct.unpack_from("<Q", data, pe + 24 + 24)[0]
    for i in range(count):
        pos = pe + 24 + opt_size + 40 * i
        virtual_size, rva, raw_size, offset = struct.unpack_from("<IIII", data, pos + 8)
        if rva <= START - base and END - base <= rva + min(virtual_size, raw_size):
            return data[offset + START - base - rva:offset + END - base - rva]
    raise ValueError("Function range not file-backed")


def main():
    if C.sizeof(C.c_void_p) != 8:
        raise RuntimeError("Windows x64 Python required")
    code = original_bytes()
    kernel = C.WinDLL("kernel32", use_last_error=True)
    kernel.VirtualAlloc.argtypes = [C.c_void_p, C.c_size_t, C.c_ulong, C.c_ulong]
    kernel.VirtualAlloc.restype = C.c_void_p
    kernel.VirtualProtect.argtypes = [C.c_void_p, C.c_size_t, C.c_ulong, C.POINTER(C.c_ulong)]
    kernel.VirtualFree.argtypes = [C.c_void_p, C.c_size_t, C.c_ulong]
    kernel.GetCurrentProcess.restype = C.c_void_p
    kernel.FlushInstructionCache.argtypes = [C.c_void_p, C.c_void_p, C.c_size_t]
    page = kernel.VirtualAlloc(None, len(code), 0x3000, 0x04)
    if not page:
        raise C.WinError(C.get_last_error())
    try:
        C.memmove(page, code, len(code))
        old = C.c_ulong()
        if not kernel.VirtualProtect(page, len(code), 0x20, C.byref(old)):
            raise C.WinError(C.get_last_error())
        if not kernel.FlushInstructionCache(kernel.GetCurrentProcess(), page, len(code)):
            raise C.WinError(C.get_last_error())
        callback = C.WINFUNCTYPE(C.c_uint64, C.c_void_p, C.c_void_p)
        trace = []
        objects = {}

        @callback
        def compatible(this, other):
            obj = objects[this]
            trace.append(obj["name"] + ".compatible")
            return obj["compatible"]

        @callback
        def linked(this, unused):
            obj = objects[this]
            trace.append(obj["name"] + ".linked")
            return obj["linked"]

        @callback
        def owner(this, unused):
            obj = objects[this]
            trace.append(obj["name"] + ".owner")
            return obj["owner"]

        @callback
        def index(this, other):
            obj = objects[this]
            trace.append(obj["name"] + ".index")
            return obj["index"] & 0xFFFFFFFF

        connector_vtable = (C.c_void_p * 15)()
        connector_vtable[4] = C.cast(owner, C.c_void_p)
        connector_vtable[5] = C.cast(linked, C.c_void_p)
        connector_vtable[14] = C.cast(compatible, C.c_void_p)
        owner_vtable = (C.c_void_p * 15)()
        owner_vtable[14] = C.cast(index, C.c_void_p)
        source = C.c_void_p(C.addressof(connector_vtable))
        target = C.c_void_p(C.addressof(connector_vtable))
        source_owner = C.c_void_p(C.addressof(owner_vtable))
        target_owner = C.c_void_p(C.addressof(owner_vtable))
        proc = C.WINFUNCTYPE(C.c_ubyte, C.c_void_p, C.c_void_p, C.c_void_p)(page)
        results = []
        for types_match, target_linked, source_linked, indexes_equal in itertools.product((False, True), repeat=4):
            trace.clear()
            objects[C.addressof(source)] = dict(name="source", linked=int(source_linked), owner=C.addressof(source_owner))
            objects[C.addressof(target)] = dict(name="target", compatible=int(types_match), linked=int(target_linked), owner=C.addressof(target_owner))
            objects[C.addressof(source_owner)] = dict(name="source_owner", index=7)
            objects[C.addressof(target_owner)] = dict(name="target_owner", index=7 if indexes_equal else 9)
            observed = bool(proc(None, C.addressof(source), C.addressof(target)))
            results.append(dict(types_match=types_match, target_linked=target_linked,
                                source_linked=source_linked, indexes_equal=indexes_equal,
                                accepted=observed, calls=list(trace)))
        receipt = dict(kind="original_function_execution_with_synthetic_virtual_interfaces",
                       target=str(TARGET), target_sha256=EXPECTED, entry=hex(START), end_exclusive=hex(END),
                       code_sha256=hashlib.sha256(code).hexdigest(), code_bytes=len(code),
                       captured_unix=time.time(), original_gui_executed=False,
                       qualification="Admission function only. Type predicate and owner indices are controlled callbacks; this does not validate their original implementations or whole-project behavior.",
                       results=results)
        out = ROOT / "analysis/connector-admission-original.json"
        if out.exists():
            raise FileExistsError("Preserve original capture; choose a new receipt for another experiment")
        out.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
        print(json.dumps(dict(receipt=str(out), cases=len(results), code_bytes=len(code))))
    finally:
        if not kernel.VirtualFree(page, 0, 0x8000):
            raise C.WinError(C.get_last_error())


if __name__ == "__main__":
    main()
