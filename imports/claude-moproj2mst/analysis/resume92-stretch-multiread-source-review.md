# CP92 independent review: STRETCH separate opens and three-double fill

No blocker was found. The new test exercises the C++ fillExtra3D path, and its full/partial expected fields agree with the separate original integer and double readers. Production code is unchanged; three-value STRETCH BOM input remains an explicit candidate decoding boundary. Root still owns build and execution.

Only this report was written. No source/test edit, build, executable test, GUI, original runtime or mutable REA action occurred. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Review bindings

| File | SHA-256 |
|---|---|
| analysis/resume92-stretch-multiread-contract.md | 5d191845e29da2909f8097b8c308fcafecce9f0fe1c4c4b9e1552b5fcbc3e728 |
| analysis/resume92-stretch-multiread-implementation.md | 893781ebf5ba568397a43229ec2b6e166e5a044fcbd57b5c2d8d1ea2b05b5a49 |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp, 680 lines | e76efde7e7eb12f7e67aca0cb42339e023e99714fa68b720bb981a18a983b115 |
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp | c043d36ff5fe5bf54809c7316db01b3aa90f1783d6f848d807b468271f4755c7 |
| source/apps/moproj2mst/src/engine/simcontrol300_io.cpp | 3e796cc212cc02767485848eb67a6f8c44dd049161061ca01bd01a6eed45b7d4 |

Production files were compared byte-for-byte with the actual CP91 archive: recorded source tree `ee3a6df1b5503dfee1839f843f031135a2f8d5a9df1ce5b6f18707d5065929e9`, archive SHA-256 `1971062b5d51674d700a18317f54dc79f7d1ead4580c7eb69c6361d36bd9421b`. They match exactly. Removing the one new test case yields the CP91 test file after newline normalization. Case count increases from 24 to 25; no target registration or public interface changed.

## Separate reader streams

Retained FUN_140ec8110 calls FUN_1410da690 with count 1 for literal 0x142b14e90, STRETCH.DAT. If that file read succeeds, it separately calls FUN_1410da510 with count 3 for literal 0x142b14ea0, also STRETCH.DAT. Both wrappers construct their own QFile/default QTextStream, initialize their own destinations and close before returning. The double reader cannot inherit the integer stream cursor.

The candidate follows that sequence. fillExtra3D calls readInts into a one-element array for stretchKind, then readDoubles into a fresh three-double array. Each adapter method has a local openRead handle, local bytes and cursor zero. Displayed stretch fields come from double indices 1 and 2; double index 0 repeats the kind token and is not a displayed stretch value. The new public-header C++ helper test observes these fields, not a live GUI or DLL C ABI.

The retained caller/Qt evidence remains bound to original executable `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No new original execution or native comparison occurred.

## Valid row and EOF expectations

| ASCII file bytes | Integer result | Separate double destinations | Expected displayed fields |
|---|---:|---|---|
| 1 CRLF 0.25 CRLF 0.5 CRLF | 1 | [1, 0.25, 0.5] | kind 1, values 0.25 / 0.5 |
| 1 CRLF 0.25 CRLF | 1 | [1, 0.25, 0] | kind 1, values 0.25 / 0 |

The full row checks both independent-open behavior and the field-index mapping. If double parsing incorrectly continued after the integer token, its array would shift and the required 0.25/0.5 displayed pair would fail. The test strictly requires a present form, stretchEnabled and the exact strings in both profiles; it does not allow unsupported as an alternative.

In the full row, the integer cursor stops at byte 1. The independent double reader parses at offsets 0,3 and 9, leaving final CR at byte 12 of the 14-byte file. In the partial row, its second getter leaves CR at byte 7; the third getter skips CR/LF, reaches byte 9/EOF and fails. Native operator>>(double&) then explicitly stores zero and records fresh end status 1. The wrapper ignores status and still returns open success, so the displayed zero is correct.

This partial fixture does not prove that atEnd skipped the third extraction before it ran: remaining CRLF means it attempts the whitespace-only tail. The new assertions cover resulting fields rather than stream cursor/status directly. The retained operator evidence shows failure does not stop a later extraction solely through sticky status; no new malformed-prefix/invalid-delimiter behavior is tested here. Exact native atEnd call counts, packet timing and stream lifetime side effects are not inferred from a value-only regression.

Canonical formatting is consistent with the unchanged finite-value checks and qt::number(...,'g',6): 0.25, 0.5 and 0. All selected numbers are finite; the KnownC argument satisfies the existing fill profile. Other absent sidecars in the disposable Rig retain their existing disabled/default handling.

## BOM guard distinction

The negative fixture directly calls readDoubles(STRETCH.DAT, count three), with UTF-8 BOM plus the same ASCII row, and requires unsupported. This isolates the three-double decoder guard; it does not call fillExtra3D on the BOM fixture, where the earlier integer read could refuse first.

Only exact count-one B_WEIGHT.DAT has CP90's BOM allowance. STRETCH count three still reaches the existing high-byte guard, and all production bytes are unchanged. This is a candidate scope assertion, not a claim that original default QTextStream rejects this BOM row. Multi-value BOM decoding remains separate work. No filename/count/codec, integer parser, SPRING or Save writer support was broadened by this checkpoint.

## Release decision

New case native_extra3d_stretch_three_doubles_follow_separate_integer_open uses existing Testkit REQUIRE/CHECK and the disposable native IO Rig. It changes no fixture source, production path or prior case. No positive skip or permissive status allowance masks its expected fields. The source review supports integration; root must still run the focused target and exact full source-stable suite/archive with fixture and executable bindings.

This covers one C++ fill reader sequence, not constructorRefresh/apply Save, live widget behavior, complete Open/Save/Close or fresh original-process parity. No runtime pass or completion is claimed. Token measurements are unavailable.
