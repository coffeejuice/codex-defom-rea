# CP135 independent review: exact two-row SPRING2 binary continuation

## Result

No blocking defect found in the frozen slice. The new branch is narrowly enabled only when the primary count is two and the entire decoded companion is exactly `0b12\r\n`. It models row one as 1, leaves `2` available, then lets row two's ordinary decimal extraction yield 2. Other two-row values and broader binary forms remain guarded. This is a candidate source/test review, not fresh original-process parity.

## Frozen identity

Current source/test hashes match the CP135 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `0fb02c8a9f5857076f57ab2e921949a35f5c7dc741466ab480ccd938e27cbd7a` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `c671a56a72eb45d6644e2035c8149131a6f7e75ac2cb4852a5b2fa4e8da290b4` |

The controlling CP135 implementation receipt and its CP135 next-frontier source report were read. The latter binds the original executable and QtCore hashes and bases the behavior on bounded PE-verified Qt extraction and caller-loop evidence.

## Findings

- The new profile boolean is computed from `count == 2 && flagBytes == "0b12\r\n"`. The helper's partial-binary case accepts this new permission only alongside the already separate one-row-qualified permission. The one-row case does not accidentally authorize the two-row continuation, and count three does not get the exact two-row permission.
- On the exact input, the first helper call starts at offset 0, returns 1 and advances to offset 3, where `2` remains. The second call starts at `2`; the general decimal path reads it through offset 4 and returns 2. CR/LF remain unread, as whitespace after the requested two values is ignored by the count-bounded caller. This matches the retained Qt table and helper loop evidence; the report correctly labels it static inference rather than a differential capture.
- The test uses two actual SPRING rows with distinct values, asserts flags `[1, 2]` in row order, and checks both source files remain byte-for-byte unchanged. The one-row witness remains separate. The exact two-row test refuses count three, mixed/repeated tokens, and leading/trailing whitespace. It does not generalize this behavior to other binary tokens, counts, or encodings.
- The reader parses the full ASCII companion before applying the exact-byte gate, so non-ASCII/BOM and unsupported decoder cases do not bypass the gate. No file write or normalization is introduced by this read path.
- No additional native status semantics are claimed: the implementation maps only this source-closed two-success extraction profile. The tests establish candidate behavior for the branch, not QTextStream parity at runtime.

## Validation boundary

No source edits, build/test execution, GUI/original application interaction, or mutable REA activity occurred in this review. Root owns integrated validation.
