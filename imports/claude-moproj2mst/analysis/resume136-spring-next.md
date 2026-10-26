# CP136 next SPRING2 token: leading-zero decimal fallback

## Selected one-row profile

The next exact source-qualified token is `09\r\n` in a readable one-row `SPRING2.INI`. Qt base-zero selection checks the digit after the initial zero. Because `9` is a decimal digit but not octal, the getter falls through to base 10 and returns 9. It consumes bytes at offsets 0 and 1, leaves CR at offset 2, and succeeds with fresh stream status zero. The one-row SPRING2 helper copies 9, ignores status and returns true. Thus the predicted row flag is 9.

The candidate currently guards this spelling as unsupported leading-zero/radix input. Proposed focused test: one valid ASCII primary row, exact companion bytes `09\r\n`, successful read with flag 9, and byte-for-byte unchanged input files. Keep `08`, other leading-zero strings, signs, multi-row continuations and unrelated tokens outside this exact admission for now. Although retained Qt evidence also predicts `08` -> 8 by the same selection branch, that second token is not needed for the minimal increment; those candidate refusals are coverage limits, not original rejection claims.

## Retained original evidence

`analysis/resume83-sw2sp-parser-independent-review.md` records a corrected one-value table with `09\r\n` -> 9, successful getter, next unread byte offset 2 (CR), and fresh int extraction status zero. Its independently recaptured, PE-byte-verified `QTextStream::getInteger` region identifies base selection at `0x670c5417–0x670c5448`: it tests whether the character after leading zero is a digit in 0–7 for octal; a digit above 7 selects base 10. The disassembly at `0x670c5448` loads decimal base 10. The int operator stores the result on success.

The evidence binds to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. `analysis/resume119-spring2-normal-path.md` and `analysis/wave2-save-diffs-300-support-1410da690.c` show the helper's count-one extraction, output copy and ignored stream status. These are static PE/caller-flow conclusions, not an original-process differential. Input is ASCII and 9 is in range; no ACP or overflow behavior is implicated.

## Candidate boundary and file ownership

If implemented, the smallest source slice is the existing SPRING2 integer parser in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`, its focused reader test `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`, and a dedicated receipt. Keep exact one-row/count-one admission and guard all other leading-zero/multi-row forms until separately qualified.

No source/test edits, build, GUI, original process, or mutable REA database activity occurred for this read-only preparation. CP135 source/test hashes remain unchanged: source `0FB02C8A9F5857076F57AB2E921949A35F5C7DC741466AB480CCD938E27CBD7A`; test `C671A56A72EB45D6644E2035C8149131A6F7E75AC2CB4852A5B2FA4E8DA290B4`.
