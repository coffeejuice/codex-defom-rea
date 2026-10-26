# CP162 SPRING2 Unicode digit source review

## Disposition

**No source blocker found.** The released reader matches the bounded plan: the new non-ASCII route is SPRING2-only, uses the retained 23-block Qt BMP digit membership, preserves ASCII-zero radix behavior, and is limited to one/two-row primaries with an int32 result. The focused test covers all 220 non-ASCII members under all four UTF-16/32 byte orders, selected mixed-script/sign cases, and refusal controls. Root's build/CTest remains the runtime integration check.

## Frozen artifacts inspected

| Artifact | SHA-256 |
|---|---|
| `analysis/resume162-spring-unicode-implementation.md` | `06A59EFBEDE34915433090352ABEA29931479D88138134D0BC3AC99C3FC2429F` |
| `analysis/resume162-spring-unicode-digit-next.md` | `394D507B618E2D020FE14C9B4B02D57BF9A477CFC488DA279D013402C54F27F9` |
| `analysis/resume162-spring-unicode-digit-plan-review.md` | `10828B2E184A2DBAAA78922C14CB794C98CEED2A10BC73C2039EB254FC7EB027` |
| `analysis/resume162-spring-unicode-digit-evidence.json` | `310778828C17BBD74939F0AC54F6BEB9DBA3694F315AEA5B926343C8BD466FAA` |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `1F3DF812602C0CDA75B394750FFC523A2CF938E01CDB32B01449F4A032EF5313` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `3D294B1460B39A90AD3CD72F856A11FF230A00D1D02AD995C50B68290AE0D5BB` |

## Review findings

- `qtBmpDecimalDigit` encodes the 23 ranges from the evidence, including ASCII `0030–0039` and the 22 non-ASCII ranges. Its exact range check excludes U+00B2, U+2160, non-ASCII minus, surrogates and supplementary values. `decodeSpringUnicodeDigits` rejects incomplete units, surrogate code units and values above BMP; UTF-32 signatures are checked before UTF-16LE's overlapping `FF FE` signature. Endian reconstruction follows the codec captures.
- Decoding occurs only for the SPRING2 companion after the matching BOM signatures are detected. It preserves ASCII code units and maps admitted Nd digits to a private marker range. The shared `asciiBytes` implementation is unchanged and still rejects non-ASCII payload by default. Other sidecars and the primary SPRING file therefore do not gain this decoder path.
- `readUnicodeDecimalFlag` uses ASCII whitespace and signs, then the Qt-derived decimal digit values. It rejects incomplete digit runs, attached non-whitespace suffixes, ASCII-zero-prefixed multi-digit runs, and results outside signed int32. An unsigned Unicode zero begins the new decimal path; an ASCII `010` token has no marker and continues through the pre-existing base-zero reader. The tests assert Unicode `٠١٠` -> 10 and encoded ASCII `010` -> 8 under each BOM.
- The caller selects this parser only after a non-ASCII payload digit is present and only if primary count is one or two. Within that selected file, ordinary ASCII tokens without a Unicode marker remain on `readSpringFlagInt`, preserving its existing special profiles. Count-three use is explicitly refused. Output rows are committed only after parsing succeeds, and the new tests check both files remain byte-identical for exhaustive accepted scalar fixtures and representative refusals.
- Test fixture encoding covers UTF-16LE, UTF-16BE, UTF-32LE and UTF-32BE with correct BOMs and byte order. It checks every non-ASCII block member as a one-unit value and as a count-two EOF value with zero-fill, plus Arabic-Indic `٠١٠`, mixed Arabic-Indic/fullwidth `12`, signed two-row `[1,-2]`, ASCII-octal control, category-boundary rejects, malformed suffix, mixed ASCII-zero/Unicode digit, non-ASCII whitespace, and count-three refusal. It also verifies source-byte preservation and empty output on the tested failures.

## Limits

The four BOM decoders and QChar category/value behavior are statically deduced from the retained original PE; this source review does not claim original-process execution. UTF-8 non-ASCII, BOM-less ACP, malformed/truncated sequences, supplementary digits, non-ASCII whitespace, overflow outside int32 and attached suffixes remain excluded. No public Session/Open/Save route was changed by this reader/test cut, so public lifecycle parity still requires root integration evidence. No source or tests were edited; no build, CTest, GUI, live REA, or mutable database operation was performed.
