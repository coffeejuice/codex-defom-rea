# CP162 SPRING2 Unicode digit plan review

## Disposition

**Source-ready for the proposed bounded SPRING2 increment, with one clarification:** keep the initial `int32` result bound explicit and do not let the new decoded-code-unit path relax the existing byte-decoder gate for any other sidecar. I found no missing endianness, category, or locale witness for the specified well-formed BMP profile. This is static PE qualification; original-runtime comparison and public Open/Save acceptance remain pending.

## Reviewed artifacts

| Artifact | SHA-256 |
|---|---|
| `analysis/resume162-spring-unicode-digit-next.md` | `394D507B618E2D020FE14C9B4B02D57BF9A477CFC488DA279D013402C54F27F9` |
| `analysis/resume162-spring-unicode-digit-evidence.json` | `310778828C17BBD74939F0AC54F6BEB9DBA3694F315AEA5B926343C8BD466FAA` |
| `analysis/resume162-spring-unicode-digit-evidence.py` | `514268853B5C3E2B7FC4C4FE2BAD4B4D4909F6F38554ED6AC7A69DB57E12C90A` |
| Current reader snapshot `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `26CFABD97925362007D8FEC1EFBFFF6880C975E765B8060F123D3B51B014DE96` |
| Current focused test snapshot `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `55C52C1F64672654FA00ED36C86BAADBE76020CECFBABE9246925D349056BF4B` |
| UTF-16 decoder capture `analysis/resume162-spring-codec-body-6714f1c0.asm` | `E4323C6B4D5B8141B11F542C067C9B7EF0C2565FBD00369011EF17986C012F30` |
| UTF-32 decoder capture `analysis/resume162-spring-codec-body-6714f570.asm` | `FFDA619EF3DEC9C40840FCB552CF8CFBD6FF171A3BE89CC1F8C31AC972506CB0` |

The reader and test hashes are read snapshots. This review does not assert CP161 is frozen.

## Contract check

The Qt property evidence supports exactly the proposed 230 BMP code units: `QChar::isDigit` checks category byte 4, while `digitValue` is recovered from the packed Qt property record. The evidence JSON reports 23 blocks of ten consecutive values, each mapping 0 through 9; 220 units are non-ASCII. Its recorded exceptions distinguish `digitValue` from `isDigit` (for example U+00B2 has value 2 but category 6). Therefore the proposed parser must consult the retained table-derived membership, not a current platform Unicode API or a broad “numeric character” predicate.

The integer-parser capture supports the radix distinction: automatic base selection compares the first unit with literal ASCII `0` at `0x670C5371`; the non-ASCII first-digit route reaches the decimal loop, which calls `isDigit`/`digitValue`. Thus U+0660 U+0661 U+0660 enters decimal and yields 10, while ASCII `010` remains on its existing octal route and yields 8. Mixed Nd blocks are valid after decimal entry. The tests proposed for Arabic-Indic input, mixed blocks, ASCII `010`, and ASCII signs exercise these distinct decisions without pretending that replacing all digits with ASCII is equivalent.

The codec evidence closes all four explicit BOM profiles. The BOM selector handles UTF-32 before the overlapping UTF-16LE signature. UTF-16BE/LE decode complete pairs using the recorded endian field and suppress the leading U+FEFF; UTF-32BE/LE similarly decode complete four-byte units, suppress the BOM, and emit BMP scalar values as one UTF-16 code unit. Each of the four fixture byte sequences in the plan is consistent with the decoder body. UTF-32 supplementary scalars use a separate surrogate-pair path; excluding them and surrogate code units is appropriate for the initial bounded cut. The current reader already has an `allowUnicodeBom` argument used only for SPRING2, while global `asciiBytes` otherwise rejects decoded units above ASCII; the new admission should remain behind that companion-specific gate.

The stated C-locale numeric behavior is compatible with this Unicode-digit branch: radix selection and signs remain ASCII decisions; only digit classification/value come from QChar. An explicit BOM selects the decoder independently of the process's startup codec. The plan correctly avoids claiming BOM-less ACP, UTF-8 non-ASCII, non-ASCII whitespace, malformed-unit, repeated-BOM, or surrogate behavior. No locale-specific witness is needed for the explicit BOM + QChar decimal path described.

The tests are appropriately discriminating: exhaustive membership over all 220 non-ASCII units and four BOMs checks the recovered category and endian routes; U+00B2/U+2160 and non-ASCII whitespace negatives protect the category boundary; mixed scripts and `010` prevent accidental normalization/base changes; count-two EOF representatives exercise the caller's zero-fill shape. Preserve raw primary and companion bytes on both success and refusal. The plan's path remains private `readSprings`/SPRING2 only and does not claim full-project Save parity.

## Clarification for implementation

The new route should explicitly state whether its admitted digit run is restricted to values representable as signed `int32`; the proposal currently says so, which is a safe initial guard. Keep the overflow tests as refusals unless a separate original-Qt narrowing profile is authorized. Also ensure that count-three primary, malformed suffix after a digit, ASCII-zero-prefixed mixed digits, and non-ASCII whitespace do not fall through to the general ASCII parser after decoding. The existing exact special-token routes and CP160 sign-failure profile should remain intact.

No source or tests were edited, and no build, CTest, GUI, live REA, or mutable database operation was performed.
