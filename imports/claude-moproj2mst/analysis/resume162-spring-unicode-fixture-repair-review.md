# CP162 SPRING2 Unicode BOM fixture repair review

## Disposition

**Both fixture repairs are correct; no further embedded-NUL truncation was found in the Unicode-digit test case.** The primary is reset to the one-row fixture before the post-exhaustive one-row loop, and the UTF-32 BOM strings now retain all four bytes. All four generated BOM prefixes match their specified encodings.

## Reviewed hashes

| Artifact | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `FCE2E099E61FA1E77C5888EAA46D014934760C0E42CFF38CBED9FDD76B308413` |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `1F3DF812602C0CDA75B394750FFC523A2CF938E01CDB32B01449F4A032EF5313` |
| `analysis/resume162-spring-unicode-implementation.md` | `06A59EFBEDE34915433090352ABEA29931479D88138134D0BC3AC99C3FC2429F` |
| Prior fixture-state review `analysis/resume162-spring-unicode-test-repair-review.md` | `89BA876299CC16F395163ED76D62A93E2609B2322A8BCEE88660CEE21A899979` |

## Byte construction and carryover checks

The current `encoded()` helper starts with these exact BOM prefixes:

| Format | Constructed bytes | Review |
|---|---|---|
| UTF-16LE | `FF FE` | Correct, no embedded NUL in BOM literal. |
| UTF-16BE | `FE FF` | Correct, no embedded NUL in BOM literal. |
| UTF-32LE | `FF FE 00 00` | Correct: `std::string("\xff\xfe\x00\x00", 4)` preserves both trailing NUL bytes. |
| UTF-32BE | `00 00 FE FF` | Correct: `std::string("\x00\x00\xfe\xff", 4)` preserves both leading NUL bytes. |

The code then appends exactly `width` bytes per requested code point, shifting in the selected endian order. Both four-byte constructors use explicit lengths, so they are not truncated by C-string length. The test-wide search found the other embedded-NUL case constructions in this file also use explicit `std::string`/`assign` lengths; the earlier shared BOM fixture helper uses `assign(..., 4)` already. No additional `const char*`-to-string conversion with an embedded NUL occurs in the CP162 case.

The repaired one-row primary reset remains before the second outer loop that exercises `٠١٠`, mixed blocks, signed two-row parsing and ASCII `010`. The test therefore presents count one for its one-row assertions; it explicitly installs count two for `[1,-2]`, then restores count one before the octal control. Later negative and count-three cases likewise install their intended primary fixtures. No false-positive row-count carryover remains apparent.

This review checks the fixture bytes and assertion setup statically. It does not run the focused test or claim original-runtime behavior. No source or test was edited, and no build, CTest, GUI, live REA, or mutable database operation was performed.
