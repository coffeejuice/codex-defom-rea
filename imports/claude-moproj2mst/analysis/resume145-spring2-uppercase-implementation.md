# CP145 implementation: exact two-row `1X` SPRING2 prefix/retry

## Change

The private SPRING2 reader now admits only the exact two-row case where the entire companion file is `1X\r\n`. It returns flags `[1,0]`: the first extraction consumes `1` and leaves `X`; the second retries at that unread `X` and supplies zero. The code does not publish or otherwise model QTextStream's internal sticky status. The exact gate is `count == 2 && flagBytes == "1X\r\n"`, so the uppercase accommodation is unavailable to other counts or byte spellings.

The focused reader test uses the existing complete two-row primary fixture and checks values, row count, and unchanged primary/companion bytes. It also verifies refusal for nearby `1X 2\r\n` and `1XX\r\n`, and for the same exact companion bytes with primary count one or three. Existing lowercase `1x` behavior and other parser profiles are unchanged.

## Evidence and limits

The original `FUN_1410da690` loop and Qt extraction branches are retained in `analysis/wave2-save-diffs-300-support-1410da690.c`, `analysis/resume120-spring2-malformed-flags.md`, and the PE-qualified Qt captures cited there. The source-backed `1x\r\n` count-two sequence is: successful decimal-prefix parse of 1, unget non-digit, second extraction at that character, failed getter writes zero, loop index reaches count. Uppercase `X` is also an ASCII non-digit to decimal extraction; unlike uppercase hexadecimal prefixes, this branch does not rely on QChar case folding. This is a static Qt/caller deduction, not an original-process comparison.

The candidate preserves the byte cursor needed for this count-two result: the successful parse stops at `X`, then the second invalid-character branch leaves it at the same offset. It represents the public flag values and does not expose getter status. Other uppercase suffixes, whitespace variations, arbitrary nonnumeric characters, count profiles, filenames, and multi-value generalization remain guarded.

## Static preflight

- Confirmed one `readSpringFlagInt` callsite and updated its new explicit profile argument.
- Checked the exact gate is constrained by both count two and complete companion bytes.
- Checked the accepted fixture asserts `[1,0]` and exact file-byte preservation.
- Checked adjacent spellings and count-one/count-three cases remain refused.
- Checked both owned files for trailing whitespace; none found.
- No build/CTest, GUI, original runtime, shared interface, remesh, or mutable REA change was performed. Root owns build and integration validation.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `35B209095822E0FCC4692AC9D7B8E484813311473790B451FD439106777C2536`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `A00336A8608D5325EE8893E78657810950BBDD182ED16CA15101AAB41066B0B5`

The new gate should be independently reviewed before root builds the grouped candidate.
