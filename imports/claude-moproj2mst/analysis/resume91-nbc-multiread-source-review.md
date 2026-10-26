# CP91 independent review: NBC two-double values and partial EOF

No blocker was found in the released test-only increment. The two new numeric expectations match the retained native reader's zero initialization, extraction loop and ignored status. Production source is byte-identical to frozen CP90; no new NBC-specific parser route or multi-value BOM admission was introduced.

Only this report was written. Source inspection began after the owner's explicit release. No source/test edit, build, executable test, GUI, original-process execution or mutable REA action occurred. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Bound files

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp, unchanged | c043d36ff5fe5bf54809c7316db01b3aa90f1783d6f848d807b468271f4755c7 |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp, 654 lines | 9cae0e8493ed99d11655b8bdee0df246a5568af54376ce5ba7f46c0376cb065d |
| analysis/resume91-nbc-multiread-implementation.md | 183f710f64bccc1d20763610bc75a4c1347c469b8782c405c92297dc3b6ff8ea |

The source/test hashes match the owner release. Compared with the actual CP90 archive (source tree `bc2e716be52a0490d6e90cf703da2150c1a1cadac2a2c538957b4766290942f9`, archive SHA-256 `d74ef171f751e23814e492ad48c9c505d347a4fd691ae8da1eefde3c9b115f1a`), production bytes are exactly unchanged. Removing new case native_extra3d_nbc_two_doubles_preserve_valid_row_and_eof_zero_tail yields the CP90 test source after newline normalization. Case count rises from 23 to 24.

## Native loop and status

FUN_140ec8110 passes count 2 to FUN_1410da510 for literal 0x142b14e60, NBC.DAT, as retained in the caller and keyword receipt. The shared reader initializes every requested double to zero, opens QFile ReadOnly, constructs default QTextStream, and repeats atEnd → count bound → operator extraction → index increment. Successful file open determines the final true return; it does not inspect stream status.

Retained evidence hashes:

- wave2-save-diffs-300-support-1410da510.c: `07e66b6bab250aace19742c6eff86f32c2b1b951e7267b16087124ff4cb4d277`.
- wave2-save-diffs-qt-textstream-read-double-complete.asm: `39d552ec0897dc6985e1006919a1e08e31d5a7077d5fa3f13ecb84f9c8c93ae9`.
- wave2-save-diffs-qt-textstream-atend-complete.asm: `2332ba77c525952c8c73da391359f3542650a79c030d18047df3a134a558cbaf`.

The int/double getter evidence is bound to original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; caller provenance remains original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This review uses retained static evidence, not a new native run.

At 0x670c6300 the double operator calls getReal without an initial nonzero-status rejection. Success writes the double at 0x670c6312. Failure writes destination zero at 0x670c6324, calls atEnd, chooses internal status 1 at end or 2 otherwise, and assigns it only if existing private+0xcc is zero (0x670c633b–6344). A prior status therefore does not itself suppress a later extraction; it only remains sticky when another failure would update status. Candidate readDoubles does not expose/store that Qt status, and the original file helper ignores it.

## Exact new numeric cases

For bytes `1.5\r\n2.5\r\n`, the first readReal leaves CR at byte 3. The next extraction skips CR/LF to byte 5 and reads 2.5, leaving the next CR at byte 8. Both destinations are [1.5,2.5]. The wrapper stops after two requested values; trailing whitespace is not a third extraction. The new assertions agree.

For bytes `1.5\r\n`, the first extraction again leaves CR at byte 3. AtEnd is not yet true merely because no second numeric token exists: CR/LF remain. The second getter consumes that whitespace, reaches byte 5/EOF, fails, and the operator explicitly writes zero with fresh internal status 1. Thus [1.5,0.0] is correct. This fixture is not a proof that the second extraction was skipped immediately by atEnd; a number ending at EOF without trailing whitespace would exercise that different branch. Resetting the candidate array to [99,99] before this read also verifies that stale second-slot data cannot survive.

The candidate's unchanged range-for, pre-zeroing and byte cursor produce these same selected values. It is not a literal reconstruction of every native atEnd invocation, packet refill or stream status transition. The new case asserts values/open success only, not cursor/status directly. Existing generic source tests exercise invalid-delimiter pushback and failure-then-later-value behavior; this increment does not broaden those into a full native stream-state claim.

## Codec/path/count boundaries

No new exact NBC route was added: existing readDoubles already handles the qualified ASCII two-value call. The only BOM admission remains exact B_WEIGHT.DAT and count one. Zero-count open/no-decode handling, ordinary path/open checks, high-byte rejection, integer SW2SP/STRETCH routing, SPRING guards and Save writers remain unchanged.

The new 13-byte UTF-8-BOM NBC row is a candidate scope refusal check and requires unsupported for count two. The new STRETCH count-three BOM row similarly requires unsupported. These are not assertions that original default QTextStream rejects such BOM-bearing rows; they preserve candidate decoding boundaries and leave that native support as separate work. No numeric-field failure is converted into file-open false, and no source condition silently skips part of a valid two-value row.

The two valid ASCII rows strictly require successful read and exact values. The negative cases require Error code unsupported through the existing helper; there is no skip or success-or-unsupported alternative in a positive row. The new test uses existing disposable Rig files and Release-active Testkit checks, leaving entrypoint and all previous cases intact.

Root can run the focused target and required full source-stable suite/archive. This review makes no passing-test, complete malformed-input, multi-value codec, full page/lifecycle or original-runtime parity claim. Token measurements are unavailable.
