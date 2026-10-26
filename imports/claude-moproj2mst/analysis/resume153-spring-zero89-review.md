# CP154 unsigned `08`/`09` decimal fallback review

## Result

No blocking defect found. The new branch matches the retained Qt base-selection rule for the bounded unsigned ASCII family `0[89][0-9]*` and preserves the separately qualified exact `09`, unsigned octal, signed-leading-zero, malformed-token, and count-two behavior.

## Frozen source and tests

The inspected files match the implementation receipt hashes:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `8AD57C52B787DD68B49AE3B44BF201C614C6ADC0AC299BF4FCF1ED53B8A31841`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `7A5994C23A94095B64EE938A5A2E649C44E686675A86161B8CAAB82FA64EEFB8`

## Findings

`readSpringFlagInt` skips leading ASCII whitespace before setting `start`. Its route order is significant and correct: exact partial `018`/`019` witnesses first; complete one-row octal `0[0-7]+` next; the exact `09` case after that; then the new decimal fallback. Thus unsigned `010` is consumed by the octal path as 8, exact `09` retains its one-row and explicitly qualified two-row behavior, and `08`/longer `08` or `09` tokens reach the new decimal path. The signed-zero path remains later and only matches a leading sign, so this unsigned selector cannot absorb `+010`/`-010`.

The new branch requires `allowSingleRowQualifiedToken`, an initial `0`, and second byte `8` or `9`. It accumulates the entire following ASCII digit run into `uint32_t`, which gives defined modulo-2^32 low-dword behavior, then admits only EOF or ASCII whitespace. It sets the cursor to the first delimiter (leaving it unread, as described in the retained contract) and returns the low bits through `bit_cast<int32_t>`. A non-digit, non-whitespace suffix throws before accepting; count-two calls do not satisfy the one-row gate and then encounter the ordinary leading-zero refusal.

Focused tests cover `08\r\n` -> 8, `089\t` -> 89, `090` at EOF -> 90, and `09223372036854775807\r\n` -> -1 by low-dword wrap. Every accepted case checks exact byte preservation of both files. One-row suffix cases `08x` and `089x` remain refused, and the later count-two refusal matrix contains `08`, `089`, and `090`. The count-one signed-leading-zero cases and the unsigned `010` octal set remain present with their own expected values and preservation assertions.

The source evidence cited by the CP154 proposal supports the branch's base choice: Qt probes the second digit after leading zero and selects octal only for 0–7, while the decimal path accumulates then stores the low dword into `int`. The candidate is ASCII-bounded and does not claim Qt's broader Unicode digit semantics.

## Limits

This is a static source/test review only; no build or CTest was run. There is no fresh original-process differential. Conclusions are limited to the covered count-one grammar and nearby guards; no general claim is made for Unicode digits, locale behavior, BOMs, multi-row extraction status/cursor behavior, or signed-prefixed forms.
