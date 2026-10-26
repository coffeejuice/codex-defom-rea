# CP74: next exact SW2SP integer divergence

## Selected fixture: uppercase hex digit after the lowercase base prefix

Use exact bytes `0xA\r\n` in `SW2SP.DAT`, read through the existing one-element `readInts` seam. The original one-int extraction leaves the destination value at zero, sets QTextStream conversion status, and the enclosing sidecar reader still returns success after the file opens. The current candidate instead rejects the token because its lowercase-hex scanner finds no accepted hex digit after `0x`.

This is a distinct, narrow case from valid lowercase hex and from the already qualified `0X...` decimal-prefix fallback. The lowercase `x` selects the original hexadecimal path; uppercase `A` is not one of that path's accepted digits.

## Evidence chain

- Original QtCore identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` captures `QTextStream::getInteger` at `0x670C5210`. Lowercase `0x` selects base 16 at `0x670C53D3–53E5`. The base-16 digit loop accepts ASCII `0`–`9` and lowercase `a`–`f`; uppercase `A` is not accumulated. With zero accepted digits, the getter returns its conversion-failure code.
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows `operator>>(int&)` writes zero and sets stream status on that failed conversion; the successful path instead copies the getter result to the destination.
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds `FUN_1410da690` to QFile/QTextStream integer extraction. It initializes requested slots to zero, requests up to the supplied count, does not inspect QTextStream status, and returns true once QFile open succeeded. `analysis/resume67-sw2sp-locale.md` binds the reachable Extra3D Open call to one requested SW2SP integer.
- `analysis/resume69-sidecar-int-next.md` already records this exact uppercase-hex failure/status distinction as an unimplemented case. This report selects it as the next exact input; no broader case is inferred.

## Candidate-only acceptance and limits

The minimal focused assertion is: write `0xA\r\n` to `SW2SP.DAT`, request one integer, expect `readInts` success and value zero. Keep the existing valid lowercase-hex tests unchanged. Do not generalize to uppercase `A`–`F` acceptance, other radix forms, malformed prefixes with arbitrary suffixes, multi-value extraction after the sticky error, or stream status exposure. The original reader's caller-visible one-value result is established from static evidence; there is no claim of original-process runtime comparison.

No source, test, build, GUI, or mutable REA state was changed for this read-only audit.
