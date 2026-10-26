# CP149 implementation: exact one-row SPRING2 `019` octal prefix

## Change

The private SPRING2 reader now admits only exact complete companion bytes `019\r\n` under the existing one-row extraction permission. Qt selects octal from the second digit `1`, accepts prefix `01` as value 1, and leaves `9` unread as the first invalid octal digit. The candidate returns flag 1 and sets its private cursor to offset 2; it does not generalize octal parsing.

The focused reader test checks one-row value 1 and byte-for-byte preservation of both files. It removes only `019\r\n` from the one-row unsupported matrix and retains guards for `0190\r\n`, whitespace variants, and the same exact/nearby spellings under count two. Existing `018` cases remain unchanged.

## Evidence

The original Qt base-zero selector and octal digit loop are retained in `analysis/resume83-sw2sp-parser-independent-review.md` and `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`. The table establishes `018\r\n` → 1 with the following 8 unread; the verified octal loop rejects any first invalid digit above 7 after accepting digits, ungets it, and returns the accumulated value successfully. `019` follows that same octal branch because its second character is 1 and the following 9 fails the octal digit test. `FUN_1410da690` reads one integer per requested row, ignores stream status, and stops at count one, so the unread 9 is not retried. This is a static source derivation, not an original-process comparison.

A nonzero flag is later serialized as a decimal integer by the retained SPRING writer contract in `analysis/resume120-spring2-writer-order.md`; this private reader increment does not enable or alter Save dispatch.

## Static preflight

- Confirmed the source rule requires both one-row permission and whole-file equality to `019\r\n`.
- Confirmed the cursor becomes `start + 2`, returning value 1 while leaving the 9 unread.
- Confirmed positive and nearby/count-two test inputs are byte-preserving or refused as intended.
- Checked no trailing whitespace in either owned file.
- No build/CTest, GUI, original runtime, shared interface, or mutable REA action was performed. Root owns integration and validation.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `E8CEF08D79CC152026C432BDB012D0017BCDB10FDCF53859F382AF58046A7759`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `4E8D3A49F1392B4C2BCC2A5AED8B01E1448DE4ABEE60F027CAD2991206757811`
