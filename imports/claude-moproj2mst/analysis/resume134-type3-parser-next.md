# CP134 Type3 parser integration frontier

## Conclusion

CP133 closes line splitting for supplied BMP UTF-16 text, and CP129/130/131 close scalar dispatch, terminal signaling, and successful-call prepared-row value shapes. They do not yet close an actual `.OPT` file parser loop. The smallest safe next step is to establish a Type3-owned read/decode adapter and row conversion contract before wiring these helpers into file parsing. Do not bind the DOE InputTable reader or ShapeOptim public load path to them yet.

## What is source-qualified

- Original `.OPT` parser entry is `0x140908630`, tied by the retained CP127/128 evidence to `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
- At `0x140908630`, null optimizer returns false before QFile construction. The parser creates QFile from its first argument and calls open with flags `ReadOnly | Text` (`0x1409086af–0x1409086cf`). Open failure follows a separate error-message path, with no modeled optimizer mutations first.
- After successful open, it calls optimizer vtable `+0x160` with true (`0x1409087e4–0x1409087f9`), constructs QTextStream over QFile (`0x1409087ff–0x140908820`), reads the first line separately, trims it, and runs `toInt(base 10)` into a local whose value is not subsequently used (`0x140908827–0x140908861`). It then initializes the local return byte false (`0x140908865`) and enters the `atEnd`/`readLine` loop (`0x14090886d–0x1409088ed`).
- Each line is split with the global QRegExp at `0x143dd9a28`, behavior `SkipEmptyParts`; CP132 binds it to `QRegExp("\\s+")`, case-sensitive RegExp. CP133 implements its BMP whitespace membership and intentionally excludes surrogate input.
- `ENDDATA` exits to common cleanup/file close at `0x140909400`. The parser's retained return byte stays false on completed paths; the call site stores the result and does not branch on it (CP128).
- Scalar keys and the `ENDDATA` outcome are modeled over already-tokenized fields. Prepared `NUM_CON` and `REGIONS` values can be appended in the observed argument order, but their parser still returns `RowBodyNotImplemented`.

## Why there is no actual file loop yet

The native loop has observable ordering that the existing helpers cannot reproduce by themselves: open must succeed before `+0x160=true`; the special first-line trim/integer conversion precedes all line dispatch; successful lines mutate scalar state in order; row records append immediately; and later EOF/`ENDDATA` does not roll back earlier changes. `NUM_CON` and `REGIONS` are normal recognized branches, not native failures. Therefore making an integration loop that stops/refuses at either key would create a candidate-specific partial failure unlike the native path, even though it preserves prior scalar mutations.

The source has no Type3 `.OPT` reader/owner. `mst::FileDevice` is a write-side abstraction; DOE `StudyIo::openReadFile` is used by the active DOE/InputTable flow and does not model this QFile+QTextStream parser contract. `InputTableSource` belongs to InputTable Geometry path provenance, not the optimizer. `ShapeOptimClass::loadClassData` still explicitly refuses `sp->type == 3` in `src/ops/report.cpp`; CP134 does not justify removing it.

Row parsing also remains a real boundary: native `NUM_CON` performs field conversions, invokes the recovered tag classifier, constructs/formats a QString, then appends immediately; `REGIONS` derives a QString from each split row before append. CP131 only models the post-conversion append calls. Malformed/short row behavior, Qt numeric locale details, exact formatter/region text construction, and their effects on append/cleanup are not captured by the prepared-value API. A partial parser cannot claim native effect order while those branches are simply skipped.

## Smallest next cut

First qualify a private Type3-specific reader contract with tests around:

1. exact QFile `ReadOnly|Text` open-success/failure boundary and the `+0x160=true` timing;
2. first-line `trimmed().toInt(base 10)` probe, including its ignored result and any supported conversion-domain limits;
3. QTextStream `atEnd`/`readLine` line boundaries and encoding/locale inputs;
4. split+scalar-dispatch sequencing with `ENDDATA` cleanup semantics.

Then close at least the row-input projections needed for `NUM_CON` and `REGIONS`, including normal text derivation and failure/partial-append order. Once those are defined, an actual loop can use the already-qualified tokenizer/dispatcher and prepared append functions without inventing a refusal at ordinary native row keys. Keep public Type3/ShapeOptim admission guarded until actual optimizer ownership and GEO/listF0 consumers are separately qualified.

## Evidence scope

This is a read-only synthesis of `analysis/resume127-type3-opt-parser.asm`, `analysis/resume128-type3-constructor-parser.md`, `analysis/resume130-type3-terminate.md`, `analysis/resume131-type3-prepared-rows.md`, `analysis/resume132-type3-regex-next.md`, `analysis/wave2-doe-regexp-space-contract.md`, and `analysis/resume133-type3-tokenizer.md`, plus current source guard inspection. No source/build/GUI/mutable REA change or original execution was performed.
