from __future__ import annotations

import argparse
import hashlib
import json
import struct
from dataclasses import dataclass
from pathlib import Path
from typing import Any

EXPECTED_SHA256 = "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc"
EXPECTED_IMAGE_BASE = 0x140000000
IMAGE_SCN_MEM_EXECUTE = 0x20000000
CALL_SIZE = 5


@dataclass(frozen=True)
class CallSite:
    name: str
    address: int
    expected_bytes: bytes
    expected_target: int
    expected_return_pc: int


@dataclass(frozen=True)
class PrefixSpan:
    name: str
    address: int
    expected_bytes: bytes


CALL_SITES = (
    CallSite("binder_to_color_dispatch", 0x1411F18F7, bytes.fromhex("e8 f4 0f 00 00"), 0x1411F28F0, 0x1411F18FC),
    CallSite("color_dispatch_mode1", 0x1411F2919, bytes.fromhex("e8 12 fb ff ff"), 0x1411F2430, 0x1411F291E),
    CallSite("color_helper_2d", 0x1411F2461, bytes.fromhex("e8 3a 00 00 00"), 0x1411F24A0, 0x1411F2466),
    CallSite("color_helper_3d", 0x1411F2482, bytes.fromhex("e8 59 02 00 00"), 0x1411F26E0, 0x1411F2487),
)

PREFIX_SPANS = (
    PrefixSpan("helper_entry_3d_prefix", 0x1411F26E0, bytes.fromhex("48 89 4c 24 08 53 48 81 ec c0 00 00 00 48 8b 05")),
)


class PEFormatError(ValueError):
    pass


def _parse_pe(data: bytes) -> dict[str, Any]:
    if len(data) < 0x40 or data[:2] != b"MZ":
        raise PEFormatError("missing DOS MZ header")
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if pe_offset + 24 > len(data) or data[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise PEFormatError("missing or truncated PE signature/header")
    machine, section_count = struct.unpack_from("<HH", data, pe_offset + 4)
    optional_size = struct.unpack_from("<H", data, pe_offset + 20)[0]
    optional = pe_offset + 24
    if optional + optional_size > len(data) or optional_size < 112:
        raise PEFormatError("truncated optional header")
    magic = struct.unpack_from("<H", data, optional)[0]
    if magic != 0x20B:
        raise PEFormatError(f"expected PE32+ optional header, got 0x{magic:04x}")
    image_base = struct.unpack_from("<Q", data, optional + 24)[0]
    section_table = optional + optional_size
    if section_table + section_count * 40 > len(data):
        raise PEFormatError("truncated section table")
    sections = []
    for index in range(section_count):
        offset = section_table + index * 40
        name = data[offset : offset + 8].split(b"\0", 1)[0].decode("ascii", "replace")
        virtual_size, virtual_address, raw_size, raw_offset = struct.unpack_from("<IIII", data, offset + 8)
        characteristics = struct.unpack_from("<I", data, offset + 36)[0]
        sections.append({
            "name": name,
            "virtualAddress": virtual_address,
            "virtualSize": virtual_size,
            "rawSize": raw_size,
            "rawOffset": raw_offset,
            "characteristics": characteristics,
        })
    return {"machine": machine, "imageBase": image_base, "sections": sections}


def _rva_to_file_offset(
    pe: dict[str, Any], data_length: int, rva: int, span_length: int = CALL_SIZE
) -> tuple[int, dict[str, Any]]:
    """Map a full raw-backed span only when exactly one section backs every byte."""
    if span_length <= 0:
        raise PEFormatError("span length must be positive")
    span_end = rva + span_length
    overlaps = []
    for section in pe["sections"]:
        start = section["virtualAddress"]
        raw_end = start + section["rawSize"]
        if max(start, rva) < min(raw_end, span_end):
            overlaps.append(section)

    if len(overlaps) > 1:
        raise PEFormatError(f"RVA span 0x{rva:x}..0x{span_end:x} has ambiguous overlapping raw section mappings")
    if not overlaps:
        raise PEFormatError(f"RVA span 0x{rva:x}..0x{span_end:x} is not backed by raw section bytes")

    section = overlaps[0]
    start = section["virtualAddress"]
    raw_end = start + section["rawSize"]
    if not (start <= rva and span_end <= raw_end):
        raise PEFormatError(f"full {span_length}-byte RVA span 0x{rva:x}..0x{span_end:x} crosses a raw section boundary")
    file_offset = section["rawOffset"] + (rva - start)
    raw_file_end = section["rawOffset"] + section["rawSize"]
    if file_offset < section["rawOffset"] or file_offset + span_length > raw_file_end or file_offset + span_length > data_length:
        raise PEFormatError(f"full {span_length}-byte file span at 0x{file_offset:x} is outside raw section/file bounds")
    return file_offset, section


def validate_image_bytes(data: bytes) -> dict[str, Any]:
    """Validate fixed call encodings and one fixed helper prefix without executing or writing the image."""
    digest = hashlib.sha256(data).hexdigest()
    result: dict[str, Any] = {
        "schema": "codex.color-pe-byte-gate.v1",
        "inputSha256": digest,
        "expectedSha256": EXPECTED_SHA256,
        "inputSize": len(data),
        "sha256Matches": digest == EXPECTED_SHA256,
        "peHeaderValid": False,
        "imageBaseMatches": False,
        "machine": None,
        "imageBase": None,
        "sites": [],
        "prefixSpans": [],
        "validationPassed": False,
        "readyForIndependentReview": False,
        "limitations": [
            "Checks only four fixed direct E8 rel32 call encodings plus one fixed 16-byte helper-entry prefix.",
            "Does not decode surrounding instruction boundaries or prove control-flow, call ordering, reachability, or observer coverage.",
            "Does not establish runtime loaded base, module identity, process behavior, GUI neutrality, or execution readiness.",
        ],
    }
    try:
        pe = _parse_pe(data)
    except PEFormatError as exc:
        result["peError"] = str(exc)
        return result

    result["peHeaderValid"] = True
    result["machine"] = f"0x{pe['machine']:04x}"
    result["imageBase"] = f"0x{pe['imageBase']:x}"
    result["imageBaseMatches"] = pe["imageBase"] == EXPECTED_IMAGE_BASE

    for site in CALL_SITES:
        rva = site.address - pe["imageBase"]
        item: dict[str, Any] = {
            "name": site.name,
            "address": f"0x{site.address:x}",
            "rva": f"0x{rva:x}",
            "expectedBytes": site.expected_bytes.hex(" "),
            "expectedTarget": f"0x{site.expected_target:x}",
            "expectedReturnPc": f"0x{site.expected_return_pc:x}",
            "actualBytes": None,
            "actualTarget": None,
            "actualReturnPc": None,
            "section": None,
            "executableSection": False,
            "bytesMatch": False,
            "targetMatches": False,
            "returnPcMatches": False,
        }
        try:
            file_offset, section = _rva_to_file_offset(pe, len(data), rva, len(site.expected_bytes))
            actual = data[file_offset : file_offset + len(site.expected_bytes)]
            target = pe["imageBase"] + rva + CALL_SIZE + struct.unpack("<i", actual[1:5])[0] if actual[:1] == b"\xe8" else None
            return_pc = pe["imageBase"] + rva + CALL_SIZE
            item.update({
                "fileOffset": f"0x{file_offset:x}",
                "section": section["name"],
                "executableSection": bool(section["characteristics"] & IMAGE_SCN_MEM_EXECUTE),
                "actualBytes": actual.hex(" "),
                "actualTarget": f"0x{target:x}" if target is not None else None,
                "actualReturnPc": f"0x{return_pc:x}",
                "bytesMatch": actual == site.expected_bytes,
                "targetMatches": target == site.expected_target,
                "returnPcMatches": return_pc == site.expected_return_pc,
            })
        except PEFormatError as exc:
            item["error"] = str(exc)
        result["sites"].append(item)

    for span in PREFIX_SPANS:
        rva = span.address - pe["imageBase"]
        item = {
            "name": span.name,
            "address": f"0x{span.address:x}",
            "rva": f"0x{rva:x}",
            "length": len(span.expected_bytes),
            "expectedBytes": span.expected_bytes.hex(" "),
            "actualBytes": None,
            "fileOffset": None,
            "section": None,
            "executableSection": False,
            "bytesMatch": False,
        }
        try:
            file_offset, section = _rva_to_file_offset(pe, len(data), rva, len(span.expected_bytes))
            actual = data[file_offset : file_offset + len(span.expected_bytes)]
            item.update({
                "actualBytes": actual.hex(" "),
                "fileOffset": f"0x{file_offset:x}",
                "section": section["name"],
                "executableSection": bool(section["characteristics"] & IMAGE_SCN_MEM_EXECUTE),
                "bytesMatch": actual == span.expected_bytes,
            })
        except PEFormatError as exc:
            item["error"] = str(exc)
        result["prefixSpans"].append(item)

    result["validationPassed"] = bool(
        result["sha256Matches"]
        and result["peHeaderValid"]
        and result["imageBaseMatches"]
        and pe["machine"] == 0x8664
        and all(
            item.get("executableSection")
            and item.get("bytesMatch")
            and item.get("targetMatches")
            and item.get("returnPcMatches")
            for item in result["sites"]
        )
        and all(item.get("executableSection") and item.get("bytesMatch") for item in result["prefixSpans"])
    )
    return result


def validate_path(path: Path) -> dict[str, Any]:
    data = path.read_bytes()
    result = validate_image_bytes(data)
    result["inputPath"] = str(path)
    return result


def main() -> int:
    default_image = Path(__file__).resolve().parents[1] / "evidence" / "original-binaries" / "DEF_GUI_MO_64.exe"
    parser = argparse.ArgumentParser(description="Read-only static validator for four color direct calls and one helper prefix.")
    parser.add_argument("image", nargs="?", type=Path, default=default_image)
    args = parser.parse_args()
    result = validate_path(args.image)
    print(json.dumps(result, indent=2, sort_keys=True))
    return 0 if result["validationPassed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
