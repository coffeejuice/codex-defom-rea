# CP138 independent review: one-row SPRING2 bare `0x` at EOF

## Result

No blocker found. The parser permits the exact complete companion byte string `0x` only through the count-one/first-flag permission, returns public value zero, and advances the cursor to EOF. The retained Qt/caller evidence supports the public success result while predicting an internal fresh extraction status of 1; the candidate does not model that status, as explicitly stated in the receipt.

## Frozen hashes

The source and test match the CP138 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `b666f52fe00a0f59c60be1e64746098ec66e5130e254dafd583c9b54b34a63ff` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `e918760e41613538b82d7141e40abb27c127d0203c6cd914382a59e80cdb81cc` |

## Findings

- `exactBareHexEofZero` requires `allowSingleRowQualifiedToken && bytes == "0x"`. The caller supplies that permission only for count one, index zero. Whole-byte equality prevents leading whitespace, trailing CR/LF, or any other bytes from entering this branch. The prior bare-prefix rejection remains active for whitespace-terminated forms that are not this exact profile.
- The branch sets cursor to start+2, clears comma-delimiter state and returns 0. The one-row reader test asserts successful `readSprings`, one row, flag zero, and exact preservation of `SPRING.INI` and the two-byte `SPRING2.INI`.
- Existing neighboring coverage refuses `0x\r\n` in the one-row unsupported list, refuses `0x` and `0x\r\n` in the two-row list, and rejects exact `0x` under count three. The separate one-row `0xG\r\n` witness continues to return zero with its input unchanged; this review does not conflate its unread-`G`/status-2 behavior with bare-prefix EOF.
- The original helper's initial `atEnd()` check sees the two input bytes, so it attempts one extraction. Retained Qt evidence says `0x` consumes the prefix, fails at EOF with value zero and fresh status 1; the operator assigns zero and the helper ignores status, increments, and returns success after its requested slot. This is a static PE/caller-flow inference, not a fresh original-process differential. Candidate tests assert public values and file preservation only, not stream status.

## Validation boundary

No source/test changes, build/test execution, GUI/original application interaction, or mutable REA operation occurred. Root owns integrated validation.
