# CP89 independent review: corrected UTF-16LE B_WEIGHT admission

No blocker remains in the corrected frozen source. Exact count-one B_WEIGHT.DAT may decode a UTF-16LE BOM followed by complete ASCII-valued code units; odd or non-ASCII units remain unsupported. The four-byte UTF-32LE overlap discovered during review is now explicitly excluded before the two-byte UTF-16LE branch. The existing UTF-8 path and all outer gates remain intact.

Only this report was written by the reviewer. No source/test edit, build, executable test, GUI, original process or mutable REA action occurred. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Final reviewed bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp (302 lines) | 9ee38e9d92db970a6095c185969757585f09e44356c9b447343534252746b13c |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp (598 lines) | f5d82005ce84c663fc08086e77e571ea564b204fd33143278e205ddbb21ead4d |
| analysis/resume89-bweight-utf16le-correction.md | 3a9d6ab4fbdd7243b218f6a11c11f59a2d23262a722592fb057e656110770242 |
| analysis/resume89-bweight-utf16le-bom.md | 3d5ef03793267292671ad8249fc1a32052b250a820846aabdb987e0275fb20c8 |

These source/test hashes match the owner's explicit corrected release and correction receipt. That receipt supersedes the initial implementation hashes e053d594…/c12774b5…. This is a source snapshot, not a build acceptance.

Comparison baseline: actual CP88 archive, source tree `eacfe403abee631861befe6f9f35b274c9be1928af0e52f95e6e1e671c54cfef`, archive SHA-256 `4fad0d2e7c9b7fcd4c69beb443e26e462573c67ee8d99e3aea39ed794b64cc94`. All production translation-unit text outside asciiBytes is exactly unchanged after newline normalization. Existing UTF-8 test source is unchanged; removing the single new UTF-16LE test case yields the CP88 test file. Case count rises from 21 to 22.

## Resolved BOM precedence blocker

Initial CP89 matched any FF FE as UTF-16LE. Therefore FF FE 00 00, the UTF-32LE marker, was admitted as a UTF-16LE zero code unit followed by interleaved ASCII/NUL units. A UTF-32LE number could become a successful zero result, violating the planned UTF-32 boundary.

The retained original codecForUtfText capture explicitly checks FF FE 00 00 first at 0x6714e083–0x6714e09f and selects MIB 0x3fb. Only later does the two-byte FF FE branch at 0x6714e0c6–0x6714e0d6 select UTF-16LE MIB 0x3f6. Capture SHA-256: `c474c0c796b286d74e9d916e388bcad36ecfb6cbe935790cb771da6c8b12b3c0` for analysis/resume58-qt-codec-for-utftext-overload.asm, bound to QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The corrected code tests the full four-byte marker first and returns an explicit unsupported error. It does not implement UTF-32 or reinterpret that marker as a UTF-16 payload. This guard also applies to the ambiguous byte sequence of a UTF-16LE marker followed by U+0000, because the original autodetection chooses the longer marker on that prefix. A strict UTF-32LE negative fixture was added. The blocker is removed by source inspection; no runtime result is claimed.

## Gate and unit decoding

The outer gate remains exactly one requested double and exact path string B_WEIGHT.DAT. File open and span initialization precede it. Other filename spellings/paths, spans greater than one, and integer readers use default-false BOM allowance and retain the existing high-byte rejection. No global codec or host-codepage selection was introduced.

After the UTF-32 exclusion, UTF-8 handling remains the same leading EF BB BF removal from CP88. Otherwise, the UTF-16LE branch requires size at least two and unsigned FF/FE comparisons. It tests `(size-2)%2` before any pair access. The loop visits offsets 2,4,… and reads offset+1 only for an established complete pair. Both bytes are converted to unsigned before `low | (high<<8)`, so signed char cannot sign-extend a code unit. The assembled range is bounded by 0xffff; only units <=0x7f are appended to the decoded string, where conversion to char is representable.

High-valued units, surrogate units, non-ASCII characters and odd trailing bytes fail explicitly. UTF-16BE/UTF-32BE markers and unsupported BOM-less non-ASCII bytes still encounter the original high-byte guard. A second UTF-16 marker is a non-ASCII unit and is rejected. The implementation does not drop, replace or repair malformed units.

BOM-less byte sequences consisting only of ASCII-range bytes, including NUL, retain the pre-existing raw ASCII reader behavior; this is not new BOM-less UTF-16 support or a claim that every BOM-less encoding is recognized/rejected. General Unicode, surrogate decoding and original malformed-sequence replacement/status semantics remain outside the profile.

## Count-zero, UTF-8 and cursor preservation

Count zero returns after successful file open and before readAll/decoding, exactly as before; missing/open-denied files still return false first. The new empty-span assertion checks successful open with the valid UTF-16 fixture, while the unchanged early return supplies the source proof that decoding is skipped. Count greater than one leaves the BOM allowance false and rejects FF/FE through the old guard. The integer SW2SP/STRETCH parser paths and SPRING count logic are unchanged.

For the exact 12 bytes FF FE 31 00 2E 00 35 00 0D 00 0A 00, decoding yields the five ASCII characters 1.5 CRLF. The unchanged real scanner begins at decoded byte zero and leaves CR at decoded offset 3, corresponding to raw offset 8. One admitted code unit maps to one ASCII byte/QChar position; no raw QFile byte-position equality is inferred. The adapter exposes only the resulting double, not that cursor.

The UTF-32 guard begins with FF FE, so it does not intercept EF BB BF. CP88's UTF-8 strip/high-byte behavior and its exact positive/malformed/other-path/multi-value regression remain present. readReal and the B_WEIGHT finite-value check are unchanged. simcontrol300_io.cpp and the integer helper retain hashes `3e796cc212cc02767485848eb67a6f8c44dd049161061ca01bd01a6eed45b7d4` and `7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7` respectively.

## Focused checks and acceptance limit

New Release-active case native_extra3d_b_weight_one_double_accepts_utf16le_bom_ascii_payload checks:

- Exact 12-byte UTF-16LE ASCII number returns true and double 1.5.
- Empty span returns successful open; two-value and other-name requests remain unsupported.
- FF FE plus one unpaired trailing byte is unsupported.
- FF FE E9 00, code unit U+00E9, is unsupported.
- Exact 24-byte UTF-32LE encoding of 1.5 CRLF is unsupported under the corrected longer-marker guard.

There is no positive-case skip or success-or-unsupported alternative. Existing Testkit entrypoint, supported assertions and quoted include resolution remain valid. Tests write only the existing disposable Rig directory. No fixture/evidence/public API/Save writer is changed.

This extends a source-qualified common default-QTextStream codec path to one known caller, not full native page/lifecycle parity. Root can rebuild the corrected hashes and run the focused target plus exact full source-stable validation/archive. No passing-test, broader codec or objective-completion claim is made here. Token measurements are unavailable.
