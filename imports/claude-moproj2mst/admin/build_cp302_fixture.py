"""Build and inspect the fixed CP302 synthetic child. Never runs the child."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess

HERE = Path(__file__).resolve().parent
SOURCE = HERE / "fixtures" / "cp302" / "exit_only.c"
SOURCE_SHA256 = "31afbe79ea842eb663be7d278ea389dddd70d5c628106371b5ef3e6a55b703b1"
OUTPUT = Path(r"C:\PROBLEM\s302\fixture.exe")
OBJECT = OUTPUT.with_suffix(".obj")
RECEIPT = HERE.parent / "analysis" / "cp302-fixture-build.json"
COMPILE_FLAGS = ("/nologo", "/W4", "/WX", "/O1", "/MT", "/c",
                 "/Fo" + str(OBJECT), str(SOURCE))
LINK_FLAGS = ("/nologo", "/MACHINE:X64", "/SUBSYSTEM:CONSOLE",
              "/OUT:" + str(OUTPUT), str(OBJECT))
ALLOWED_IMPORTS = frozenset({"KERNEL32.DLL"})


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_source() -> None:
    check_plain_path(SOURCE)
    if SOURCE.is_symlink() or not SOURCE.is_file() or sha256(SOURCE) != SOURCE_SHA256:
        raise ValueError("CP302 source is missing, redirected, or changed")


def check_plain_path(path: Path) -> None:
    if path.is_symlink() or any(parent.is_symlink() for parent in path.parents):
        raise ValueError(f"reparse/symlink path refused: {path}")
    if os.name == "nt":
        import ctypes
        get_attrs = ctypes.windll.kernel32.GetFileAttributesW
        get_attrs.argtypes = [ctypes.c_wchar_p]
        get_attrs.restype = ctypes.c_uint32
        for part in (path, *path.parents):
            if part.exists() and get_attrs(str(part)) & 0x400:
                raise ValueError(f"reparse path refused: {part}")


def inspect_pe(path: Path) -> dict:
    check_plain_path(path)
    b = path.read_bytes()
    if len(b) < 0x100 or b[:2] != b"MZ":
        raise ValueError("not a PE file")
    pe = struct.unpack_from("<I", b, 0x3c)[0]
    if pe + 24 > len(b) or b[pe:pe + 4] != b"PE\0\0":
        raise ValueError("invalid PE header")
    machine, sections, _, _, _, opt_size, _ = struct.unpack_from("<HHIIIHH", b, pe + 4)
    opt = pe + 24
    if machine != 0x8664 or sections == 0 or opt + opt_size > len(b):
        raise ValueError("expected x64 PE")
    if struct.unpack_from("<H", b, opt)[0] != 0x20b or opt_size < 136:
        raise ValueError("expected PE32+ image")
    subsystem = struct.unpack_from("<H", b, opt + 68)[0]
    if subsystem != 3:
        raise ValueError("expected console subsystem")
    directory_count = struct.unpack_from("<I", b, opt + 108)[0]
    if directory_count < 2:
        raise ValueError("missing import directory")
    if directory_count > (opt_size - 112) // 8:
        raise ValueError("directory count exceeds optional header")
    import_rva, import_size = struct.unpack_from("<II", b, opt + 120)
    if not import_rva or import_size < 40:
        raise ValueError("missing import directory")
    # Reject alternate entry behavior rather than claiming to inspect callbacks.
    for index, label in ((9, "TLS"), (13, "delay imports")):
        if directory_count > index and any(struct.unpack_from("<II", b, opt + 112 + index * 8)):
            raise ValueError(f"{label} directory refused")
    section_table = opt + opt_size
    if section_table + sections * 40 > len(b):
        raise ValueError("truncated section table")
    ranges = []
    for i in range(sections):
        s = section_table + i * 40
        if s + 40 > len(b):
            raise ValueError("truncated section table")
        virtual_size, rva, raw_size, raw = struct.unpack_from("<IIII", b, s + 8)
        if not raw_size or raw + raw_size > len(b) or rva + max(virtual_size, raw_size) > 0x100000000:
            raise ValueError("invalid section bounds")
        end = rva + max(virtual_size, raw_size)
        if any(rva < prior_end and prior_rva < end for prior_rva, prior_end, _, _ in ranges):
            raise ValueError("overlapping section RVAs")
        if any(raw < prior_raw + prior_size and prior_raw < raw + raw_size
               for _, _, prior_raw, prior_size in ranges):
            raise ValueError("overlapping section raw data")
        ranges.append((rva, end, raw, raw_size))

    def offset(rva: int, length: int = 1) -> int:
        for lo, hi, raw, raw_size in ranges:
            p = raw + rva - lo
            if lo <= rva and rva + length <= hi and rva - lo + length <= raw_size:
                return p
        raise ValueError("invalid import RVA")

    imports = []
    offset(import_rva, import_size)
    if import_rva:
        for n in range(import_size // 20):
            d = offset(import_rva + n * 20, 20)
            fields = struct.unpack_from("<IIIII", b, d)
            if fields == (0, 0, 0, 0, 0):
                break
            # Each byte, including NUL, must map inside the same raw section.
            name_section = next((item for item in ranges
                                 if item[0] <= fields[3] < item[0] + item[3]), None)
            if name_section is None:
                raise ValueError("invalid import name RVA")
            name_limit = min(128, name_section[0] + name_section[3] - fields[3])
            name_bytes = bytearray()
            for j in range(name_limit):
                q = offset(fields[3] + j)
                if b[q] == 0:
                    break
                name_bytes.append(b[q])
            else:
                raise ValueError("unterminated import name")
            if not name_bytes:
                raise ValueError("empty import name")
            imports.append(name_bytes.decode("ascii").upper())
        else:
            raise ValueError("unterminated import table")
    if not imports or not set(imports) <= ALLOWED_IMPORTS:
        raise ValueError(f"unexpected imported DLL: {imports}")
    return {"machine": "AMD64", "subsystem": "CONSOLE", "imports": sorted(set(imports)),
            "sha256": hashlib.sha256(b).hexdigest(), "bytes": len(b)}


def build() -> dict:
    check_source()
    for path in (SOURCE, OUTPUT, OBJECT, RECEIPT):
        check_plain_path(path)
    if any(path.exists() for path in (OUTPUT, OBJECT, RECEIPT)):
        raise FileExistsError("CP302 output, object, or receipt already exists")
    if len(str(OUTPUT)) >= 100:
        raise ValueError("CP302 output path too long")
    tools = Path(os.environ["VCToolsInstallDir"])
    if not tools.is_absolute():
        raise ValueError("VCToolsInstallDir must be absolute")
    cl = tools / "bin" / "Hostx64" / "x64" / "cl.exe"
    link = tools / "bin" / "Hostx64" / "x64" / "link.exe"
    for tool in (cl, link):
        check_plain_path(tool)
        if not tool.is_file():
            raise FileNotFoundError(tool)
    tool_identity = {"compiler_sha256": sha256(cl), "linker_sha256": sha256(link)}
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    compile_command = [str(cl), *COMPILE_FLAGS]
    link_command = [str(link), *LINK_FLAGS]
    subprocess.run(compile_command, check=True, cwd=OUTPUT.parent, timeout=120)
    subprocess.run(link_command, check=True, cwd=OUTPUT.parent, timeout=120)
    result = inspect_pe(OUTPUT)
    receipt = {"source": str(SOURCE), "source_sha256": SOURCE_SHA256,
               "compiler": str(cl), "linker": str(link), **tool_identity,
               "compile_command": compile_command, "link_command": link_command,
               "output": str(OUTPUT), "pe": result,
               "identity_limit": "Tool paths and hashes checked before invocation; filesystem replacement between check and execution or after receipt remains possible."}
    RECEIPT.parent.mkdir(parents=True, exist_ok=True)
    RECEIPT.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return receipt


if __name__ == "__main__":
    print(json.dumps(build(), indent=2))
