# CP84 SW2SP base-0 parser implementation receipt

## Result

Added a private byte-incremental Qt 4 base-0 integer model and routed only the existing one-value `SW2SP.DAT` read through it. The existing ASCII decoder, ordinary file-open behavior, exact filename test, multi-value behavior, and separate `SPRING.INI` count path remain in place. The old literal branches remain in `readDecimalInt`; the one-value `SW2SP.DAT` branch now precedes them, and they are not being removed until root runs the release-active tests.

The helper returns the wrapped 64-bit value, getter success, next unread byte, and a `sourceQualified` boundary. The adapter maps getter failure to zero and keeps the original open-success result. It rejects an initial nonnumeric byte and a sign followed by a nonnumeric byte when the evidence does not qualify those forms. The helper separately models fresh `operator>>(int&)` status for direct tests, although SW2SP ignores that status.

The 20 table rows in `resume84-sw2sp-parser-plan.md` are represented in the direct helper and adapter regression. The rows assert value, getter outcome, cursor, and status. The corrected PE-derived cases include case-folded hex prefix/digits, `08` decimal fallback versus `018` octal-prefix parsing, binary/hex zero-digit rollback differences, comma delimiter position, modulo-2^64 accumulation, low-32-bit narrowing, and conditional signed completion. Additional adapter checks keep unsupported initial text and signed-invalid text guarded.

## Scope and evidence

The behavioral source remains the offline QtCore PE qualification in `resume83-sw2sp-parser-independent-review.md` and the corrected contract in `resume84-sw2sp-parser-plan.md`. This implementation is limited to ASCII bytes already admitted by the current decoder, default C locale, and one requested value from `SW2SP.DAT`. It does not claim Unicode/ACP decoding, BOM behavior, custom locale, multi-value stream state, `SPRING.INI`, other filenames, or original-process parity. Root still needs to run the build and release-active test suite; no worker build or test execution was performed.

## Static preflight and hashes

`git diff --check` produced no diagnostics for the owned native adapter and focused test. The helper was reviewed directly; no compile or runtime check was performed.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `40B7ACCDB7A2A9D63D4B7788755F7AD1F29831C99D42E3AA58276109D0562598`
- `source/apps/moproj2mst/src/engine/simcontrol300_qt_int_parser.hpp`: `7784B177F2DF89C2EA8BA413A40B51B5A985A2D40D7F5CAE0E21A976ED05A8D7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `1A865DF23F8E5E68EF07FC67EEB0F966AD850FB5ABA1C7DF681CE2E31FBEE224`

The prior exact-token reports are retained as historical receipts. Their corrected witness outcomes are captured in the independent PE review and current CP84 oracle. No broader legacy branches were changed as incidental cleanup.
