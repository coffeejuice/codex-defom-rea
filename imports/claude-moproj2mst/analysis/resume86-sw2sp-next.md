# CP86: no reachable multi-value SW2SP successor yet

## Caller decision

The next stream-fidelity increment should not route a multi-value `SW2SP.DAT` span through the CP84 parser. The recovered production Open path in `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp` uses `std::array<int, 1>` for `SW2SP.DAT`. The same local one-element array is reused for the separately named `STRETCH.DAT` integer read. The native adapter's only production call to `readInts` outside its own `SPRING.INI` count reader is from this page fill. The original caller qualification is also recorded in `resume71-sw2sp-multivalue-boundary.md`: `FUN_140ec8110` requests one value from `FUN_1410da690` for SW2SP. Although the original reader is generic and accepts a supplied count, that generic loop does not establish a reachable SW2SP count greater than one.

Current candidate routing matches that boundary: only `values.size() == 1 && path.u16() == u"SW2SP.DAT"` enters the CP84 parser. A different filename, a two-or-more element span, and the one-element `SPRING.INI` count continue through the generic legacy reader. Keep those gates unchanged.

## Qt stream evidence and its limit

The retained QtCore image is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the PE-qualified excerpts are retained in `wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `wave2-save-diffs-qt-textstream-read-int-complete.asm`, with detailed mappings in `resume83-sw2sp-parser-independent-review.md`.

The getter returns a parsed 64-bit value plus a conversion outcome, and has branch-specific `ungetChar` behavior. The int operator copies the low dword on success (`0x670C6166–616A`). On failure it writes zero and derives an internal status from end state (`0x670C6125–6165`); before assigning that newly derived nonzero status, it checks the existing status field (`0x670C6149–6150`). These facts are enough for the CP84 fresh-stream, one-extraction table. They do not establish the observable result of a second extraction on one persistent QTextStream, including whether the next getter runs after failure, how existing status affects the destination, or what exact buffer/pushback state `atEnd()` sees.

The CP84 helper intentionally exposes a fresh operator status mapper and a next-byte cursor. That is not a reusable Qt stream state: in particular binary zero-digit failure restores both prefix characters, whereas hexadecimal zero-digit failure leaves the prefix consumed and only ungets the first following character. Qt's unget operates on QChars and can restore a normalized character; an integer byte offset alone may be insufficient after case folding.

## Smallest safe next stage

For the current Open→Save goal, keep multi-value SW2SP outside the parser and rely on the one-value gate. No new production parser branch or test should claim multi-value parity based only on the generic native reader.

If a future source trace finds a real caller requesting multiple values from the same SW2SP QTextStream, make the next change a separate stateful-reader design rather than extending `parseQt4Base0Int` by passing a new offset. First bind that caller's exact requested count and the stream construction/lifetime from original native evidence. Then capture the operator's preexisting-status behavior together with getInteger/unget/end-state branches. Only after those are known, add a private state result that preserves stream status, current logical end, and QChar pushback, and table-test consecutive extractions. Minimum discriminating cases should include two valid tokens separated by whitespace, a valid token followed by a radix prefix failure and then another token, and a valid token followed by an invalid radix suffix. Do not assign expected second-value results until the operator and buffering paths are fully source-qualified.

`SPRING.INI` remains explicitly out of scope: it has a separate count-first path whose count can control allocation/iteration. Do not use the SW2SP one-value arithmetic allowance there.

## Work status

This is a read-only static conclusion. No source, test, build, GUI, original-process session, or mutable REA state changed in CP86. No multi-value behavior is claimed or implemented.
