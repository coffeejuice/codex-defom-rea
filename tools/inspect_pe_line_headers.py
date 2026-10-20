"""Inspect retained DWARF .debug_line headers in a PE file without executing it.

Some PE toolchains truncate the section name to ".debug_l". This utility records
the source filenames and distinguishes a header-only unit from a line program.
It intentionally does not invent source-to-address mappings from file order.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path


def inspect(path: Path, source_name: str) -> dict:
    data = path.read_bytes()

    def integer(offset: int, fmt: str) -> int:
        size = struct.calcsize(fmt)
        if offset < 0 or offset + size > len(data):
            raise ValueError(f"Integer outside file at {offset:#x}")
        return struct.unpack_from(fmt, data, offset)[0]

    if data[:2] != b"MZ":
        raise ValueError("Not a DOS/PE image")
    pe = integer(0x3C, "<I")
    if data[pe:pe + 4] != b"PE\0\0":
        raise ValueError("Invalid PE signature")
    section_count = integer(pe + 6, "<H")
    optional_size = integer(pe + 20, "<H")
    sections = []
    for index in range(section_count):
        start = pe + 24 + optional_size + index * 40
        name = data[start:start + 8].rstrip(b"\0").decode("ascii")
        sections.append({
            "name": name,
            "virtual_size": integer(start + 8, "<I"),
            "rva": integer(start + 12, "<I"),
            "raw_size": integer(start + 16, "<I"),
            "file_offset": integer(start + 20, "<I"),
        })
    candidates = [s for s in sections if s["name"] in (".debug_l", ".debug_line")]
    if len(candidates) != 1:
        raise ValueError(f"Expected one debug line section; found {len(candidates)}")
    section = candidates[0]
    start = section["file_offset"]
    end = start + section["virtual_size"]
    if section["virtual_size"] > section["raw_size"] or end > len(data):
        raise ValueError("Debug line section is not fully file-backed")

    units = []
    cursor = start
    while cursor < end:
        unit_start = cursor
        unit_length = integer(cursor, "<I")
        if unit_length == 0xFFFFFFFF:
            raise ValueError("DWARF64 units not supported")
        unit_end = cursor + 4 + unit_length
        if unit_length < 6 or unit_end > end:
            raise ValueError(f"Invalid unit length at {unit_start:#x}")
        version = integer(cursor + 4, "<H")
        if version not in (2, 3):
            raise ValueError(f"Unsupported DWARF line version {version}")
        header_length = integer(cursor + 6, "<I")
        header_end = cursor + 10 + header_length
        if header_end > unit_end:
            raise ValueError("Line header exceeds unit")
        cursor += 10
        fixed_header_offset = cursor
        if cursor + 5 > header_end:
            raise ValueError("Truncated line header")
        min_instruction_length = data[cursor]
        default_is_stmt = data[cursor + 1]
        line_base = struct.unpack_from("<b", data, cursor + 2)[0]
        line_range = data[cursor + 3]
        opcode_base = data[cursor + 4]
        cursor += 5
        if opcode_base == 0 or cursor + opcode_base - 1 > header_end:
            raise ValueError("Invalid opcode-length table")
        standard_opcode_lengths = list(data[cursor:cursor + opcode_base - 1])
        cursor += opcode_base - 1

        def cstring() -> tuple[str, int]:
            nonlocal cursor
            where = cursor
            terminator = data.find(b"\0", cursor, header_end)
            if terminator == -1:
                raise ValueError("Unterminated filename or directory")
            text = data[cursor:terminator].decode("utf-8", errors="strict")
            cursor = terminator + 1
            return text, where

        def uleb128() -> int:
            nonlocal cursor
            result = 0
            shift = 0
            while cursor < header_end:
                value = data[cursor]
                cursor += 1
                result |= (value & 0x7F) << shift
                if value < 0x80:
                    return result
                shift += 7
                if shift >= 64:
                    raise ValueError("Oversized ULEB128")
            raise ValueError("Truncated ULEB128")

        directories = []
        while True:
            text, where = cstring()
            if not text:
                break
            directories.append({"path": text, "file_offset": hex(where)})
        files = []
        while True:
            text, where = cstring()
            if not text:
                break
            directory_index, mtime, size = uleb128(), uleb128(), uleb128()
            if directory_index > len(directories):
                raise ValueError("Invalid include directory index")
            files.append({
                "index": len(files) + 1,
                "name": text,
                "file_offset": hex(where),
                "directory_index": directory_index,
                "directory": directories[directory_index - 1]["path"]
                if directory_index else None,
                "mtime": mtime,
                "declared_size": size,
            })
        if cursor != header_end:
            raise ValueError(f"Unexpected header tail at {cursor:#x}")
        units.append({
            "unit_file_offset": hex(unit_start),
            "unit_end_file_offset_exclusive": hex(unit_end),
            "unit_length_excluding_length_field": unit_length,
            "version": version,
            "header_length": header_length,
            "fixed_header_file_offset": hex(fixed_header_offset),
            "min_instruction_length": min_instruction_length,
            "default_is_stmt": default_is_stmt,
            "line_base": line_base,
            "line_range": line_range,
            "opcode_base": opcode_base,
            "standard_opcode_lengths": standard_opcode_lengths,
            "directories": directories,
            "files": files,
            "line_program_file_offset": hex(header_end),
            "line_program_bytes": unit_end - header_end,
            "line_program_sha256": hashlib.sha256(data[header_end:unit_end]).hexdigest(),
        })
        cursor = unit_end

    selected = [
        unit for unit in units
        if any(f["name"].casefold() == source_name.casefold() for f in unit["files"])
    ]
    return {
        "artifact": {
            "path": str(path.resolve()),
            "bytes": len(data),
            "sha256": hashlib.sha256(data).hexdigest(),
        },
        "method": "Bounds-checked PE section and DWARF v2/v3 line-header decoding",
        "section": section,
        "source_name": source_name,
        "coverage": {
            "units": len(units),
            "matching_units": len(selected),
            "all_units_header_only": all(u["line_program_bytes"] == 0 for u in units),
            "consumed_section_bytes": cursor - start,
        },
        "matching_units": selected,
        "unit_summary": [
            {
                "unit_file_offset": unit["unit_file_offset"],
                "first_file": unit["files"][0]["name"] if unit["files"] else None,
                "line_program_bytes": unit["line_program_bytes"],
            }
            for unit in units
        ],
        "source_to_address_mapping": {
            "available": False,
            "address_ranges": [],
            "reason": (
                "Matching line units contain no line-program bytes and therefore "
                "no DW_LNE_set_address instructions or address/line matrix rows."
                if selected and all(u["line_program_bytes"] == 0 for u in selected)
                else "No matching line unit, or line-program interpretation not performed."
            ),
        },
        "limitations": [
            "Source paths and include names are embedded build metadata, not source contents.",
            "Unit ordering does not establish code ordering or routine ownership.",
            "No original Fortran source text or function names are recovered by this parser.",
            "This utility does not execute the target or interpret nonempty line programs.",
        ],
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=Path)
    parser.add_argument("--source", default="datarm23.f")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = inspect(args.binary, args.source)
    output = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(output, encoding="utf-8")
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
