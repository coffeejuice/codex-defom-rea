# CP137 independent review: two-row SPRING2 `09` EOF-short profile

## Result

No blocker found. Candidate admission is limited to exactly two requested rows and the complete companion byte string `09\r\n`. It produces public flags `[9, 0]` while leaving both files unchanged. The first extraction and caller's EOF-short behavior are consistent with retained static Qt/helper evidence. The candidate deliberately does not model the second attempted extraction's internal QTextStream status, matching the receipt's stated limit.

## Frozen identity

The source and test match the CP137 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `ec8a68078cff4515e4fec9b7d3d70361e5b70dc31fc2569524ef59e2086c08a7` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `bacba785e5274b920ef8b0f697cbb5e78c52af503dc8fb8c0655ff5b0008e5d6` |

## Findings

- The permission is created only by `count == 2 && flagBytes == "09\r\n"`, and is passed to `readSpringFlagInt` separately from the one-row permission. `exactDecimal09` recognizes the token only when one of those explicit gates is present. The two-row branch cannot therefore authorize a wider set of leading-zero spellings or counts.
- The first extraction returns 9 and advances to offset 2, leaving CRLF. On the next candidate loop iteration the ordinary whitespace skip consumes CRLF to EOF; the pre-zeroed second slot remains zero because the loop breaks before another parser call. This produces the same public `[9,0]` value as the retained caller flow: `atEnd()` is initially false with CRLF remaining; the second extraction skips it, reaches EOF, stores zero, then the helper increments and returns true after the requested count. This is static combination of Qt and caller evidence, not a captured two-call original run.
- The test asserts two rows with flags 9 and 0 and byte-exact unchanged SPRING and SPRING2 contents. It rejects mixed/spaced/trailing-space variants at count two and explicitly rejects exact `09\r\n` at count three. Existing one-row `09` remains a separate permission.
- The candidate does not represent Qt's second extraction attempt or its EOF-side status. That is an internal-state qualification, not a public-value mismatch; neither the helper's public result nor these tests claim stream-status equivalence.

## Validation boundary

No source edits, build/test execution, GUI/original app interaction, or mutable REA operation occurred. The original binary and Qt hashes plus parser/caller evidence are bound by the CP137 receipt. Root owns integrated validation.
