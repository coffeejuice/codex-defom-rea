# CP88 independent review: B_WEIGHT one-double UTF-8 BOM admission

No blocker remains in the frozen source. Exactly one double from exact B_WEIGHT.DAT may discard a leading EF BB BF marker, after which the existing ASCII guard and real parser run unchanged. All non-ASCII payload remains guarded. The initial signed-char comparison issue was corrected before the owner released the reviewed hashes.

Only this report was written by the reviewer. No source/test edit, worker build, executable test, GUI, original-process execution or mutable REA action occurred. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Reviewed immutable-source snapshot

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp | c63d62c31658a1e68504d2669cf80e75e992e85eb9fb25d8e10abec5718548cc |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp | 336277438b47f5c6056124fa4f12c40dae910d446558cf52bb19adcb3eb7b2c0 |
| analysis/resume88-bweight-utf8-bom.md | 13e57000ad8499d5452efcd0eee4a335d233243e19b3aa3a985fcdb6dfc1866b |
| analysis/resume88-bweight-utf8-bom-implementation.md | 246b17a506463c7e94a95daf4e2facbfa904f524a7779e5c221d7cdfec6a3b79 |

The active source/test hashes match the owner's release. Baseline comparison used the CP87 archive, recorded source tree `c54bcafb1622ef7612982389cb01c860fe96baf872201025cef7b9c1cdd494e5`, archive SHA-256 `79b7b16fa69ec2fa1cbeeff2a9dda89a4c48015c17a027b24dfc50c36c0f862e`. These are source bindings; this report does not establish an executable built from them.

## Native scope and expected value

Retained FUN_140ec8110 calls the shared FUN_1410da510 double reader with count 1 for the B_WEIGHT.DAT literal at 0x142b14eb8, then formats that value into the weight control. The shared reader constructs default QTextStream over QFile without a codec override. The existing common constructor/initial decoder evidence in resume58-ppv-qt-codec.md and resume59-ppv-codec-next.md establishes enabled Unicode autodetection and codecForUtfText selecting UTF-8 MIB 0x6a for EF BB BF. This is common stream decoding, not behavior specific to readLine.

For exact bytes EF BB BF 31 2E 35 0D 0A, the marker selects UTF-8 and the body characters are ASCII 1.5 CRLF. The previously qualified C-locale real parser therefore receives 1.5. Neither a process ACP assumption nor malformed UTF-8 replacement semantics are needed. This remains retained static evidence for original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; no new original runtime comparison occurred.

## Exact route and decoding guard

The CP87 diff contains only two production hunks: readDoubles computes the exact filename/count predicate and passes it to asciiBytes; asciiBytes gains a default-false parameter and optional three-byte prefix removal. readReal, readInts, path/open helpers, all writer logic and other conversion code remain unchanged.

The predicate is `values.size()==1 && path.u16()==u"B_WEIGHT.DAT"`. Matching is exact, so another filename, basename-containing path, alternate case/spelling or count greater than one does not gain BOM admission. readInts calls asciiBytes with the default false value, including one-value SW2SP/STRETCH routes; their BOM refusal is not broadened. SPRING parsing and later multi-double STRETCH reads are unaffected.

The released code checks bytes.size() >= 3 before indexing and casts each char to unsigned char before comparing against 0xef, 0xbb and 0xbf. This fixes the pre-release comparison that mixed unsigned marker bytes with signed std::string chars. The recognized prefix is removed once, only at byte zero.

After removal, the unchanged unsigned high-byte scan rejects every remaining byte >=128. Therefore a recognized marker followed by malformed/non-ASCII bytes, BOM-less non-ASCII, truncated EF/EF BB markers, another encoding's marker, a second BOM, or a nonleading BOM remains unsupported for this bounded decoder. It does not replace, drop or reinterpret those bytes. The implementation normalizes only a marker and then delegates ASCII numeric syntax/failure behavior to the existing real parser; it is not a general UTF-8 validator/decoder.

Zero-count precision: readDoubles still returns open success before reading/decoding when values.empty(). That pre-existing probe behavior remains, even if the file contains BOM or malformed bytes. The owner's broad phrase about non-one-value requests being rejected should be read as ineligible nonempty requests; count zero performs no decoding and is not a newly admitted BOM conversion. This distinction was communicated to root and the owner; no source correction is needed.

## Cursor and finite-value preservation

The raw eight-byte fixture becomes the five-byte body 1.5 CRLF before parsing. The existing cursor still starts at zero, now at the first decoded ASCII body character rather than the BOM. On this payload, the unchanged real scanner leaves its cursor at body byte 3 (CR), corresponding to raw file byte 6. The adapter does not expose or reuse that cursor in another request, and it still asks for one value.

No native byte offset or QFile position fidelity is claimed from string erasure. Each later read reopens independently as before. A marker-only decoded-empty file would use the existing pre-zero/empty-body behavior; that derived behavior is not a separately executed new test.

simcontrol300_io.cpp is byte-identical to CP87, SHA-256 `3e796cc212cc02767485848eb67a6f8c44dd049161061ca01bd01a6eed45b7d4`. Its B_WEIGHT finite-value check and field formatting are unchanged. The integer helper is also byte-identical, SHA-256 `7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7`.

## Focused test and remaining gate

The test delta is exactly one new Release-active Testkit case, native_extra3d_b_weight_one_double_accepts_utf8_bom_ascii_payload; removing it yields the CP87 test source after newline normalization. Target case count rises from 20 to 21.

The positive fixture uses adjacent C++ literals with an explicit length 8, preserving exactly EF BB BF 31 2E 35 0D 0A. It requires read success and value 1.5. Negative checks require unsupported for marker plus C3 28 malformed/non-ASCII payload, the same valid BOM input with two requested doubles, and the same input on another filename. There is no skip or success-or-unsupported alternative for the positive case.

Truncated/double/nonleading BOM branches are protected by the same explicit prefix/high-byte control flow but are not individually executed by this new case; they are source conclusions. The test checks the adapter read, not complete page/UI/native lifecycle parity. Root must run the focused native-IO target and required complete source-stable suite/archive with matching fixtures and executable bindings before runtime acceptance. No passing-test, broader codec fidelity or completion claim follows from this review. Token measurements are unavailable.
