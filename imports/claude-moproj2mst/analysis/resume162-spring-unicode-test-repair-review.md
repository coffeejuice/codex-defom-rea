# CP162 SPRING2 Unicode test repair review

## Disposition

**Repair is correct; no false positive found.** Restoring the one-row `SPRING.INI` fixture at the start of the second BOM loop fixes the carryover from the preceding count-two exhaustive loop. The remainder of the test explicitly restores the primary count before each later one-row/two-row section, and the final count-three fixture is intentionally paired with the refusal assertion.

## Reviewed hashes

| Artifact | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `A2EEAE7D75CDFC2B6D4B3A984045E4652BC19D53BA3632158274E131FB1DB86E` |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `1F3DF812602C0CDA75B394750FFC523A2CF938E01CDB32B01449F4A032EF5313` |
| `analysis/resume162-spring-unicode-implementation.md` | `06A59EFBEDE34915433090352ABEA29931479D88138134D0BC3AC99C3FC2429F` |
| Accepted plan review `analysis/resume162-spring-unicode-source-review.md` | `36BBF513151961CEAE87BADD02F69F620636B91FD9E815C01E2A61691DC28280` |

## Fixture/call-path check

The repaired `rig.write("SPRING.INI", primary);` is at the start of the second outer loop over the four explicit BOM encodings. The preceding exhaustive loop deliberately ends with the two-row primary, so without this write each subsequent Unicode `٠١٠`, mixed-script, two-row, and ASCII `010` case would call the reader using the previous two-row count and fail or test a different profile. With the reset, the first and mixed-script cases use count one; the test then explicitly installs a count-two primary for `[1,-2]`, and explicitly restores the one-row primary before the ASCII-octal control.

After that outer loop, category-boundary negatives restore the one-row primary for every case. The malformed `digit + q` check follows the loop with the last primary still one-row; the two rejection variants retain that same one-row primary. The final count-three primary is explicitly installed immediately before the count-three refusal, so it is intentional. Thus no later assertion inherits a stale row count.

The test still reaches the intended SPRING2 reader. Successful cases check the returned row count and expected flag values; the exhaustive loops also check both raw files remain identical. The mixed-script one-row assertion relies on the prior one-row primary reset and does not separately recheck `rows.size()` or raw bytes, but neither is a hidden pass condition for the Unicode digit assertion because the subsequent direct `readSprings` call overwrites `rows` and that case's expected `rows[0].flag` is checked. The count-two test checks row count and both flags. The refusal cases assert unsupported plus empty output; representative refusal loops also assert byte preservation.

This was a read-only test review. No source or test was edited, and no build, CTest, GUI, live REA, or mutable database operation was performed.
