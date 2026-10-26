# CP79: next exact SW2SP binary decision branch

## Selected input: invalid first binary digit `0b2`

To cover a distinct decision branch rather than another valid binary value, use exact bytes `0b2\r\n` in `SW2SP.DAT`. The original base-0 getter recognizes the lowercase `0b` prefix, then rejects `2` as a binary digit before accumulating any value. It returns conversion failure; `QTextStream::operator>>(int&)` writes zero and marks stream status, while the original one-value SW2SP file helper ignores that status and returns true after QFile successfully opens. Thus the caller-visible result is successful read with value 0. The candidate currently rejects this token as unsupported syntax.

## Retained source evidence

- QtCore binary SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows lowercase `0b` selecting base 2 (`0x670C53EF–5405`). The binary loop compares the next QChar only with `0` and `1` (`0x670C5924–592F`); any other character takes the unget path (`0x670C5954–595F`). Since no bit was accepted, the zero-count path ungets the binary prefix characters and returns the getter's failure code (`0x670C5964–59A5`).
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows a nonzero getter result causes the integer operator to write zero and set stream status (`0x670C6125–6165`).
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` shows `FUN_1410da690` initializes the integer destination, performs the requested QTextStream extraction, does not inspect stream status, closes the file, and returns success once QFile open succeeded. `analysis/resume67-sw2sp-locale.md` binds this one-int reader to the Extra3D `SW2SP.DAT` Open path.

## Minimal test proposal

For a one-element `readInts` call on `SW2SP.DAT`, write `0b2\r\n` and assert the adapter returns true with value zero. Keep the existing valid `0b10\r\n` case, and retain all other binary forms, other paths, multi-value behavior, and status exposure outside this exact failure branch. This is static evidence for the original shipped QtCore path, not a fresh original-process comparison.

No source, test, build, GUI, or mutable REA state was changed for this audit.
