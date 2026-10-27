from __future__ import annotations

import unittest
from pathlib import Path

from color_pe_byte_gate import (
    CALL_SITES,
    EXPECTED_SHA256,
    PREFIX_SPANS,
    PEFormatError,
    _parse_pe,
    _rva_to_file_offset,
    validate_image_bytes,
)


class ColorPeByteGateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.image_path = Path(__file__).resolve().parents[1] / "evidence" / "original-binaries" / "DEF_GUI_MO_64.exe"
        cls.image = cls.image_path.read_bytes()

    def test_immutable_original_matches_four_calls_and_helper_prefix(self) -> None:
        result = validate_image_bytes(self.image)
        self.assertEqual(result["inputSha256"], EXPECTED_SHA256)
        self.assertTrue(result["validationPassed"])
        self.assertFalse(result["readyForIndependentReview"])
        self.assertEqual([item["name"] for item in result["sites"]], [site.name for site in CALL_SITES])
        for site, item in zip(CALL_SITES, result["sites"], strict=True):
            self.assertTrue(item["executableSection"])
            self.assertTrue(item["bytesMatch"], item)
            self.assertTrue(item["targetMatches"], item)
            self.assertTrue(item["returnPcMatches"], item)
            self.assertEqual(item["actualBytes"], site.expected_bytes.hex(" "))
        self.assertEqual([item["name"] for item in result["prefixSpans"]], [span.name for span in PREFIX_SPANS])
        prefix = result["prefixSpans"][0]
        self.assertEqual(prefix["length"], 16)
        self.assertTrue(prefix["executableSection"])
        self.assertTrue(prefix["bytesMatch"], prefix)
        self.assertEqual(prefix["actualBytes"], PREFIX_SPANS[0].expected_bytes.hex(" "))
        self.assertNotIn("actualTarget", prefix)
        self.assertNotIn("actualReturnPc", prefix)

    def test_one_byte_call_mismatch_fails(self) -> None:
        site = CALL_SITES[0]
        mutated = bytearray(self.image)
        base_result = validate_image_bytes(self.image)
        offset = int(base_result["sites"][0]["fileOffset"], 16)
        mutated[offset + 1] ^= 0x01
        result = validate_image_bytes(bytes(mutated))
        self.assertFalse(result["sha256Matches"])
        self.assertFalse(result["validationPassed"])
        affected = next(item for item in result["sites"] if item["name"] == site.name)
        self.assertFalse(affected["bytesMatch"])
        self.assertFalse(affected["targetMatches"])
        self.assertNotEqual(affected["actualBytes"], affected["expectedBytes"])

    def test_one_byte_prefix_mismatch_fails(self) -> None:
        span = PREFIX_SPANS[0]
        mutated = bytearray(self.image)
        base_result = validate_image_bytes(self.image)
        offset = int(base_result["prefixSpans"][0]["fileOffset"], 16)
        mutated[offset + 7] ^= 0x01
        result = validate_image_bytes(bytes(mutated))
        self.assertFalse(result["sha256Matches"])
        self.assertFalse(result["validationPassed"])
        affected = next(item for item in result["prefixSpans"] if item["name"] == span.name)
        self.assertFalse(affected["bytesMatch"])
        self.assertNotEqual(affected["actualBytes"], affected["expectedBytes"])

    def test_rejects_call_span_crossing_raw_section_boundary(self) -> None:
        pe = _parse_pe(self.image)
        call_rva = CALL_SITES[0].address - pe["imageBase"]
        sections = [dict(section) for section in pe["sections"]]
        text = next(section for section in sections if section["name"] == ".text")
        text["rawSize"] = call_rva - text["virtualAddress"] + 4
        pe["sections"] = sections
        with self.assertRaisesRegex(PEFormatError, "crosses a raw section boundary"):
            _rva_to_file_offset(pe, len(self.image), call_rva)

    def test_rejects_helper_prefix_crossing_raw_section_boundary(self) -> None:
        pe = _parse_pe(self.image)
        span = PREFIX_SPANS[0]
        span_rva = span.address - pe["imageBase"]
        sections = [dict(section) for section in pe["sections"]]
        text = next(section for section in sections if section["name"] == ".text")
        text["rawSize"] = span_rva - text["virtualAddress"] + len(span.expected_bytes) - 1
        pe["sections"] = sections
        with self.assertRaisesRegex(PEFormatError, "crosses a raw section boundary"):
            _rva_to_file_offset(pe, len(self.image), span_rva, len(span.expected_bytes))

    def test_rejects_ambiguous_overlapping_raw_section_mappings(self) -> None:
        pe = _parse_pe(self.image)
        call_rva = CALL_SITES[0].address - pe["imageBase"]
        text = next(section for section in pe["sections"] if section["name"] == ".text")
        overlap = dict(text, name="synthetic-overlap", rawOffset=text["rawOffset"] + 0x1000)
        pe["sections"].append(overlap)
        with self.assertRaisesRegex(PEFormatError, "ambiguous overlapping raw section mappings"):
            _rva_to_file_offset(pe, len(self.image), call_rva)

    def test_rejects_truncated_dos_header(self) -> None:
        result = validate_image_bytes(b"MZ")
        self.assertFalse(result["peHeaderValid"])
        self.assertFalse(result["validationPassed"])
        self.assertEqual(result["peError"], "missing DOS MZ header")


if __name__ == "__main__":
    unittest.main()
