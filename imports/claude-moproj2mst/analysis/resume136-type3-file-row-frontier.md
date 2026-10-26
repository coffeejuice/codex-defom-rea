# CP136 Type3 parser frontier: header probe and NUM_CON row value

The smallest newly source-closed row effect is the `NUM_CON` row’s QString member: the parser passes an empty QString into the append method. This can tighten a private already-tokenized row projection, but it does not justify a file-level parser. The file/stream boundary still has unresolved Type-flag decoding/line input behavior, and `REGIONS` row conversion plus malformed/short-row behavior remain open.

## First-line probe: exact ordered effect

In `analysis/resume127-type3-opt-parser.asm`, after successful QFile open the parser first sets optimizer `+0x48` true (`0x1409087e4–0x1409087f9`), constructs QTextStream on the QFile (`0x1409087ff–0x140908820`), and calls `readLine()` into the original local QString (`0x140908810–0x140908822`). At `0x140908827–0x140908845` it calls `QString::trimmed()` into a separate temporary and immediately destroys that temporary. It then calls `QString::toInt(base 10)` on the **original readLine QString**, stores the result in an unused local, sets the parser return byte false, and only then starts the `atEnd`/line-dispatch loop. Thus the header is consumed outside the dispatcher; the trimmed return is discarded. Do not implement the probe as “convert the trimmed result,” even though `toInt` itself accepts surrounding whitespace for ordinary inputs. The result does not influence state or the parser return on observed paths.

This establishes a source-closed in-memory header boundary: a driver whose line span begins at the first dispatchable body line is a projection after the native header read. It does not supply physical byte decoding or line boundaries. The state mutation order remains open success → purge flag true → header read/probe → body lines; open failure precedes the purge setter.

## Newly closed `NUM_CON` row text

At parser `0x140908d13–0x140908d38`, the original constructs a temporary QString using QtCore IAT `0x141e04480` and pointer `0x1427b9f0f`, then passes that QString as the first stack argument to DefOptModel vtable `+0x8` at `0x140908d8e`. The immutable original PE bytes at that address are:

```text
00 5c 73 2b 00 52 75 6e ...
```

So the C string begins with NUL: the QString supplied to each successful `NUM_CON` append is empty. The adjacent `\\s+` regex starts at `0x1427b9f10`; that is a different pointer one byte later. This resolves the formatter/value uncertainty for that append argument without making a claim about other field semantics. Original binary identity is `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

The remaining successful row order is already captured in `resume131-type3-next.md`: parse field 1 int, field 2 int, field 3 double, field 4 int; classify field 1; call append with field 1, classifier, field 3, empty QString, field 2, field 4; append occurs immediately and the return index is ignored. The current prepared-row helper can represent this by passing `qt::String{}`. Its generic QString input remains useful as an abstract prepared-value seam, but the native parser-generated value for this call is specifically empty.

## Recommended next private cut and dependency

After CP135 validation, the smallest meaningful row increment is to extend the private in-memory sequence driver for one finite `NUM_CON` profile: accept a positive declared count and complete tokenized rows in ordinary decimal/int and C-locale finite-double syntax; apply the recovered classifier and append each row with an empty QString in native encounter order. Preserve already appended rows and earlier scalar changes if a later row is missing/guarded. Keep incomplete rows, unsupported numeric spellings/locale, unmodeled count overflow, `REGIONS`, and public Type3/ShapeOptim wiring guarded. A distinct focused test file such as `tests/doe/test_type3_optimizer_numcon_sequence.cpp` should check exact six-field mapping, empty `value20`, returned append index/state order, and earlier-row retention at the chosen guarded boundary. This is an in-memory tokenized-row projection, not a file parser or claim about native malformed-row behavior.

A file-level implementation remains blocked on a Type3-specific byte/line source and owner. Original QFile mode is ReadOnly|Text, while retained PPV reader captures use ReadOnly without Text; the Text-mode decoder/filter path is therefore not interchangeable. Default QTextStream BOM autodetection and locale-codec selection are statically described in the shared Qt captures, but the original process’s actual locale codec is not identified. The `readLine` CRLF mechanics are shared Qt code, but the Text-mode CR filtering interaction must be bounded for the chosen Type3 input profile. The candidate also lacks a SUB-scoped native optimizer owner, so plugging a private scalar value into the public load path would invent ownership/lifetime.

The `NUM_CON` row builder is source-closed for successful field order and empty text, but not for short rows, malformed numeric tokens, conversion locale/overflow details, allocation/refcount/unwind failures, or row cleanup. The regex is recovered, but that alone does not close these effects. `REGIONS` still needs its row-to-QString derivation. The observed parser returns false even after effects and the caller ignores it; any private adapter result must keep its own guard state separate from that native return fact.

## Qualification

All native claims above are static PE/call-flow evidence. The QString literal bytes were read from the immutable executable; no target process, GUI, build/test, source file, or mutable REA state was used or modified. No source-equivalent file-level parse or original runtime differential is claimed.
