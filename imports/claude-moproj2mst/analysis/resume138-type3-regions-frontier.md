# CP138 Type3 REGIONS row frontier

## Finding

The `REGIONS` branch is not yet a source-closed row projection. Its count and line loop are visible, and the parser ultimately calls the `{int, QString}` append method with integer zero, but the QString is derived through several data-dependent `QStringList` searches and insertions. The current private sequence helper correctly keeps `REGIONS` guarded. The next safe cut is to recover the exact imported Qt method identities and literal semantics for this block, then model only the proven token-list transformation as a private pure helper. Do not append a guessed region value or enable file parsing/public Type3.

## Native branch order

The immutable executable is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. In `analysis/resume127-type3-opt-parser.asm`:

1. `0x140908db9–0x140908e08` dispatches on `REGIONS`; `0x140908e0e–0x140908e31` converts field 1 to an integer count (base 10).
2. `0x140908e38–0x140908e90` initializes the row index, checks the declared count and QTextStream `atEnd()`, then reads one line.
3. `0x140908e97–0x140908ed1` splits with the global `QRegExp("\\s+")`, `SkipEmptyParts`, and copies the resulting list into a mutable local list. That regex contract is qualified in `analysis/resume132-type3-regex-next.md` and the BMP tokenizer in `analysis/resume133-type3-tokenizer.md`.
4. `0x140908ed2–0x140909228` searches and conditionally inserts multiple literal tokens into that list. The strings constructed from the referenced addresses are individual C strings: `0x1427ba0a4`/`0x1427ba0b4` are `"#"`; `0x1427ba120`/`124` are `"/"`; `0x1427ba128`/`12c` are `"\\"`; and `0x1427ba130`/`134` are `"."`. The sequence uses both `0x141e05498` and `0x141e046a0` for searches and `0x141e054e8` for conditional insertion. The final search at `0x1409091fc–0x140909218` feeds an integer result into `0x141e046a8` at `0x14090921e–0x140909228`, yielding the QString passed onward.
5. `0x140909248–0x14090925a` invokes optimizer vtable `+0xa8` with `edx=0` and the derived QString. The append shape and ordered vector effect are qualified by `analysis/resume131-type3-prepared-rows.md`.

These facts establish that row conversion precedes append and that the append value depends on token-list content. They do not establish the exact resulting QString for a general input row, nor malformed/short-row behavior.

## Smallest next increment and dependency

The smallest source-closed implementation opportunity is a private pure helper taking the already-tokenized row and reproducing the native search/insert/selection sequence, followed by the existing prepared region append. Before coding it, bind the IAT slots `0x141e05498`, `0x141e046a0`, `0x141e054e8`, and `0x141e046a8` to their exact QtCore imports/signatures from retained import evidence. The assembly argument registers strongly constrain the operations, but this report does not substitute guessed names for an import map. Then test the helper with rows that exercise each conditional token branch, plus a no-match case, and assert the exact derived QString and append order.

That helper would still be only a prepared-token projection. File-level `.OPT` parsing remains blocked by the original QFile `ReadOnly|Text`/QTextStream line owner and codec domain noted in `analysis/resume134-type3-parser-next.md`; no public Type3 loading is justified. Also keep the conversion guard for unqualified short/malformed rows and Qt list allocation/unwind behavior. The count/EOF loop should only be moved into a private sequence once both `NUM_CON` and `REGIONS` row bodies have bounded normal-path conversions.

## Qualification

This was a read-only review of retained disassembly, PE literal bytes, and current private helper sources. No production source, tests, build, GUI, original execution, or mutable REA state was changed. The existing `REGIONS` guard remains appropriate; no candidate/native behavior claim is made for malformed input.
