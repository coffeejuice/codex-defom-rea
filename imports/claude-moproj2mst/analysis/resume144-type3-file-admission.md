# CP144 Type3 `.OPT` ASCII file-admission audit

## Finding

A very narrow private file-reader profile can be connected to CP141/143 without guessing how `ReadOnly|Text` transforms CR or how a process-specific codec decodes high bytes: accept only a fully readable, BOM-free, seven-bit ASCII file with LF separators, no CR bytes, and a final LF. For this profile, the QFile Text flag has no CR byte to affect, all bytes are ASCII under the default QTextStream codec, and line boundaries are LF. This is a candidate admission restriction, not the original parser's supported-input limit.

This does not justify public Type3 activation or a general file parser. The adapter would need to distinguish preflight refusal from native open/read outcomes, model the first-line header read separately, and accept only complete supported row shapes. Exact behavior after an OS/QTextStream read error remains unavailable; exclude such errors from the successful-read profile instead of silently treating them as EOF. The native optimizer owner is also still missing.

## Native call order and byte behavior

`analysis/resume127-type3-opt-parser.asm` (original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`) proves:

- The parser constructs QFile from the supplied path and opens it with `ReadOnly | Text` at `0x14090869f–0x1409086cf`.
- Only after open succeeds does it set the supplied optimizer's `+0x48` flag true, construct one default QTextStream on that QFile, and call `readLine` for the header. It calls `trimmed()` into a temporary, discards it, then calls base-10 `QString::toInt` on the original header string; the result is unused.
- The loop then checks QTextStream `atEnd`, reads one full QString line, splits with the recovered `QRegExp("\\s+")`/`SkipEmptyParts`, and dispatches in order. `NUM_CON` rows use split fields; `REGIONS` derives its stored value from the raw line QString (CP141) and appends immediately. The per-block count and `atEnd` checks preserve completed rows on natural early EOF (CP136/143).
- ENDDATA reaches the same close/cleanup path. The observed parser return remains false after parse completion; its caller stores and ignores that return.

Retained QtCore evidence in `analysis/resume58-ppv-qt-codec.md` proves default QTextStream Unicode autodetection and UTF-16LE BOM selection, and `readLine()` removes CR when paired directly before LF. That evidence is from a separate PPV path whose QFile is opened `ReadOnly` without `Text`; it does not prove Type3's text-device filter. The repository's Qt4 adapter comments in `src/engine/l5_slots.cpp` and `src/xml/dom.cpp` state that `QIODevice::Text` drops CR while reading, but the retained direct Type3 QtCore call window does not show that QFile filter's implementation. The proposed profile has no CR bytes, so it does not depend on this unresolved filter detail.

No codec is set in the Type3 parser window. A BOM-free ASCII-only file avoids BOM detection and non-ASCII locale decoding; ASCII bytes retain their characters through the supported locale-codec path. The rule is explicitly a finite candidate profile. Reject a BOM, any byte `>=0x80`, NUL, and CR before parsing. Do not use the generic `readTextLines` adapter as a claim for Type3: it strips CR unconditionally, uses CP_ACP, and does not reproduce QTextStream BOM behavior.

## EOF and error boundary

For LF-only input with a final LF, header/body line boundaries are unambiguous and no unterminated-tail convention is needed. An empty file still follows the native open-success mutation and separate header `readLine`; then `atEnd` prevents body dispatch. A file containing only the header has no body records. A file that ends naturally before a declared row count reaches its target is also observable in the assembly: the inner row loop tests `atEnd` before reading, exits with earlier rows retained, and the top-level loop subsequently sees EOF. CP136/143 private sequence helpers can represent that state once the file front-end passes only the rows actually available.

The native path does not inspect QTextStream status after reads, and there is no retained Type3 capture for a physical read error. Therefore a candidate file front-end must not map a failed byte read to ordinary EOF and claim parity. Its bounded positive profile should require a complete successful byte read; a failed preflight/read is a candidate guard and lies outside the native-equivalence claim. To preserve known ordering on any later extension, open success must still precede optimizer `+0x48=true`, and rows must be applied as their lines are consumed rather than transactionally rolled back. A future injected read-error profile needs separate native status/partial-effect evidence.

The final-LF restriction is also an explicit candidate bound. While Qt `readLine` generally returns a final unterminated line, the retained Type3 path has not been separately qualified at that edge; the chosen bounded profile avoids depending on it. CRLF and bare-CR input are outside this initial profile.

## Smallest next private integration

The smallest implementation is a private Type3 byte-to-lines adapter with these explicit steps:

1. Validate/open the file under the bounded path policy; distinguish open failure before optimizer mutation.
2. Require a complete successful byte read and the strict ASCII/LF/final-LF byte profile. Unsupported bytes produce a candidate guard, not a native parse failure claim.
3. Consume the first line as the native header probe (including calling the equivalent trim/toInt in the observed order, with the result discarded).
4. Feed body lines incrementally to a combined private sequence that handles scalar dispatch, CP136 `NUM_CON`, CP143 `REGIONS`, and ENDDATA while retaining prior effects. Keep the native always-false parse return distinct from candidate acceptance/guard status.

Before connecting even this private parser to application flow, obtain a Type3 optimizer owner/lifetime separate from `SubProject::ownedDoeStudy`/`SubProject::doe`, and keep `ShapeOptimClass::loadClassData`'s Type3 refusal. A whole-file read before any parsing can claim only successful full reads; it cannot reproduce a late read error after a native prefix has already mutated the optimizer.

## Qualification

Read-only audit of retained PE/QtCore captures, current private helper/source contracts, and the cited reports. No source, test, build, GUI, original runtime, or mutable REA database was touched. No new native runtime parity or accepted-input claim is made.
