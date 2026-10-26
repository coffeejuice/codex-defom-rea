# CP143 Type3 `.OPT` file/parser boundary after REGIONS rewrite

## Finding

CP141 closes the normal `REGIONS` row-string transformation, so a private already-decoded-line sequence can now process both `NUM_CON` and `REGIONS` row blocks without refusing at a normal row key. It does **not** close the file-level parser. The exact native open/header/dispatch order is known, but an exact supported byte-to-line profile for this `QFile(ReadOnly|Text)` plus default `QTextStream` path is not yet established, and the candidate still has no SUB-owned Type3 optimizer matching the native pointer.

The smallest next parser cut is therefore an in-memory sequence increment: consume a positive `REGIONS` count and append each supplied raw row line after `deriveType3RegionValue`, preserving encounter order and earlier effects on EOF. Keep file opening, codec/line decoding, and public Type3/ShapeOptim activation guarded. A true file-level adapter should wait for the two dependencies called out below.

## Native `.OPT` order and row binding

The original binary is `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The retained entry capture `analysis/resume127-type3-opt-parser.asm` establishes:

1. Null optimizer returns false before QFile construction. Otherwise the path constructs a stack QFile; `open` receives `ReadOnly | Text` (`0x14090869f–0x1409086cf`). Open failure closes/destroys locals and returns false without setting optimizer `+0x48`.
2. Successful open sets optimizer `+0x48=true` (`0x1409087e4–0x1409087f9`), constructs one QTextStream over the stack QFile, and reads a separate first line (`0x1409087ff–0x140908822`). It calls `trimmed()` into a temporary and destroys that result, then calls base-10 `toInt` on the original header QString. The integer result is unused. Only after that read/probe does it initialize the parser return false and enter the `atEnd`/`readLine` dispatch loop. `analysis/resume136-type3-file-row-frontier.md` qualifies this unusual discarded-trim result order.
3. Every body line is split by `QString::split(QRegExp("\\s+"), SkipEmptyParts)`. This regex/options are qualified in `analysis/resume132-type3-regex-next.md`; the candidate BMP whitespace tokenizer is `analysis/resume133-type3-tokenizer.md`.
4. `NUM_CON` consumes the header count, then reads each row line and converts split fields. The CP136 sequence accepts a finite already-tokenized numeric profile and appends immediately. `REGIONS` consumes its count and reads each row line; it creates a split temporary, copies the raw line QString, then CP141's exact mutation chain acts on that raw row text (not on a selected split token): first-`#` prefix removal, last `/` prefix removal, last `\\` prefix removal, and final-dot truncation. It appends `{0, derivedQString}` immediately. Count and `atEnd` checks bound each block; early EOF keeps rows already appended.
5. `ENDDATA` exits to common cleanup. The parser returns its initialized false byte on the observed completion path; the Type3 caller stores but does not branch on that result. File close and stream/file/path destruction order are recorded in `analysis/resume134-type3-stream-owner.md`.

This gives a body-line ordering contract, not physical input semantics. The row line used by `REGIONS` is the whole `readLine` QString. An integrated in-memory sequence must preserve that binding and must not transform only `fields[1]` or a joined token list.

## Remaining file input evidence

The Qt4 captures establish that a default QTextStream enables Unicode auto-detection, tests the initial byte buffer through `QTextCodec::codecForUtfText`, recognizes UTF-16LE BOM `FF FE`, and that `readLine()` removes CR when it is paired immediately before LF (`analysis/resume58-ppv-qt-codec.md`). Those captures were traced through a separate PPV reader that opens QFile with `ReadOnly` only. They do not establish Type3's behavior under `ReadOnly | Text`.

Two input questions block a source-qualified general file adapter:

- **QFile Text mode:** the Type3 call explicitly adds `Text`. The retained PPV `ReadOnly` reader cannot prove how QFile's text-mode device filter translates CRLF/lone CR before QTextStream sees bytes. The resulting visible behavior for LF/CRLF likely overlaps `readLine`, but that overlap has not been bound to this QFile flag path. Do not claim a Type3 physical line decoder based only on PPV captures.
- **Default codec in the original process:** no `setCodec` call appears in the retained parser window. Default stream auto-detection covers BOM-selected codecs; BOM-less non-ASCII uses the default locale codec. Existing Qt captures establish that codec selection mechanism, not the actual process's active locale codec for a Type3 file. A finite ASCII-only/no-BOM profile is a plausible candidate policy, but still requires the Text-mode line behavior above and must be labeled a candidate restriction, not native accepted-input limit.

The first-line probe also affects what reaches dispatch: empty opened files still execute the separate `readLine`, and a one-line file containing only the header has no body line. Final unterminated lines, terminal newline state, stream read errors, and error status are not tested against the original Type3 invocation. A synthetic vector of strings cannot stand in for those details.

## Smallest safe next change

Implement only the private tokenized/in-memory `REGIONS` count loop in the Type3 sequence helper using CP141's `deriveType3RegionValue` on each supplied raw row string, then append through `appendType3RegionPreparedRow`. Use a new focused sequence test for: ordered multiple rows; combined delimiter order; zero/negative count; early input end preserving prior row/scalar changes; and `ENDDATA` after a completed block. Keep malformed count, Qt allocation failure, actual parse return status, and file decode outside the claim. This cut is source-closed over the already-supplied body-line domain and does not imply public parser support.

For a later file adapter, first bind the QFile `Text` read filter and a same-process/default codec profile (or admit only a clearly stated ASCII byte subset), and obtain a Type3 optimizer owner/lifetime that is separate from the Type2 Study and DOE manager. Then model open-before-`+0x48` mutation, header consumption, immediate ordered mutations, EOF/ENDDATA, close/cleanup, and the native false return separately from candidate acceptance. Do not bind this parser into `loadClassData` or remove the existing Type3 refusal based on CP141/143.

## Qualification

This is a read-only synthesis of retained Type3 PE/call-flow evidence and the CP132/133/134/136/141 receipts. No source, build, test, GUI, original runtime, or mutable REA database was touched. No file-level parity or codec acceptance claim is made.
