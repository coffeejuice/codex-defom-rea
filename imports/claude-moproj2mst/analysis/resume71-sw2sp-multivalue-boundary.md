# CP71: next SW2SP decimal boundary beyond 64-bit range

## Selected exact input

A source-backed next one-value SW2SP witness is `18446744073709551616\r\n` (`2^64`). The original getter's decimal loop computes each digit as `10 * rdi + digit` in the 64-bit `rdi` register (`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `0x670C56F6–5704`). These are 64-bit `lea` operations; the loop contains no overflow branch or range-check. For the exact 20-digit value `2^64`, the final register value wraps modulo `2^64` to zero. The C-locale delimiter is CR, not the comma grouping separator, so the getter takes the successful digit-parsed path and stores qword zero. `QTextStream::operator>>(int&)` copies the low dword, also zero (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6166–616A`).

Original Extra3D `FUN_140ec8110` requests one SW2SP integer through `FUN_1410da690`, and formats that `int` as decimal for the enabled control (`analysis/wave2-save-diffs-300-support-140ec8110.c`). For this static case, the original read produces zero and completes the one-value reader; the Save writer later uses the control text (`analysis/wave2-save-diffs-300-support-140ec9040.c`). Current candidate branches handle only exact `2147483648` and `-2147483649` in a one-value SW2SP read; this 2^64 token still fails the generic `qt::toInt` int32 range check.

This is a static arithmetic consequence of the retained QtCore machine code, not a fresh original runtime observation. It does not authorize a general decimal overflow parser without more validation. The exact one-token fixture can be used to validate a future narrow branch if root chooses to continue this profile.

## Multi-value boundary

The Extra3D Open caller's `SW2SP.DAT` request count is fixed at one. `FUN_1410da690` is generic and loops for its supplied count, but the reachable SW2SP call passes `1`; do not widen the candidate's exact overflow gate to multi-value spans for this page. A two-value `SW2SP` parse is not evidence-backed as a caller path here. The spring sidecar uses a separate count-first reader (`FUN_1410dab40`/`FUN_1410da800`); its values can control allocation and should remain behind current guards rather than reusing SW2SP overflow allowances.

## Remaining arithmetic boundary

The 64-bit accumulation and final low-dword copy imply modulo-2^64 accumulator behavior and modulo-2^32 observable int narrowing for positive decimal tokens, provided the conversion reaches the successful parsed-number path and no grouping mark changes tokenization. `18446744073709551616` is the first simple exact wrap-to-zero witness. Larger values require no distinct machine branch but would need a bounded grammar and test strategy before candidate generalization. Negative decimal magnitudes that wrap to zero have an additional sign-completion branch; no broad signed-overflow rule is proposed.

No source, test, build, GUI, or mutable REA state was changed. Evidence remains the retained QtCore image SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, its two QTextStream captures cited above, and the original Extra3D call-site decompilations. No source change or runtime fixture was performed.
