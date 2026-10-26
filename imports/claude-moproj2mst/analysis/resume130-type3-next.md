# CP130 Type3 `.OPT` continuation frontier

## Finding

The retained CP127 dumpbin body and CP128 analysis answer the control-flow portion of this question, but not the static regex setup or row ownership contracts. The smallest additional source-closed piece is to represent the already-tokenized `ENDDATA` control outcome explicitly. The CP129 scalar helper currently returns `Ignored` for `ENDDATA`; its independent review correctly notes that the value helper alone therefore does not communicate the native loop stop. A parser caller would have to special-case it outside the helper. If this helper is the planned dispatcher seam, a `Terminate` result (and a focused test) is safer and still value-only. It must not claim line parsing or file-close behavior.

## Qualified retained control flow

Evidence is `analysis/resume127-type3-opt-parser.asm`, the bounded `dumpbin /DISASM:NOBYTES /RANGE:0x140908630,0x140909467` capture, tied by CP127/128 receipts to original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

- After the header/version probe, the parser loops on QTextStream `atEnd`, reads one line, and calls QString::split with a global object at `0x143dd9a28` (first top-level split call at `0x1409088ed`; row splits also reference the same object).
- `ENDDATA` is compared as a top-level key and branches out of the line loop to the common file-close/cleanup path (CP128 identifies the terminator; the parser's common close/cleanup is at `0x140909400`). It is not a no-op key.
- `NUM_CON` reads token 1 as row count, then loops up to that count while checking QTextStream EOF. For each available row it splits again, converts selected fields, classifies a tag through optimizer vtable `+0xa0`, formats one QString, then immediately calls optimizer vtable `+0x8` with several converted values. EOF or count completion exits the row loop and returns to top-level parsing; no rollback of earlier appends appears in the body.
- `REGIONS` similarly reads token 1 as a count, reads/splits each available row, constructs a QStringList-like local, and immediately appends through optimizer vtable `+0xa8`. EOF/count completion returns to top-level parsing; earlier appends are not rolled back.
- The parser initializes its return byte to false and returns it on the retained paths, including after the common cleanup. The caller does not branch on that result. Do not interpret false as “no mutation.”

These instructions establish sequencing and partial append timing. They do not close the called virtual append implementations, element structure, shared-data copy/ref behavior, allocation failure/unwind, or destructors. The current CP129 scalar state is not connected to this parser loop.

## Exact blockers

1. **Split behavior:** the parser passes the address `0x143dd9a28` as the QRegExp separator. No retained evidence found for its initializer, regex pattern, options, or backing QString construction. The current bounded parser export contains uses of the static object, not its global initialization. Without that, producing tokens from file lines is not source-closed.
2. **NUM_CON owner:** append target is optimizer vtable `+0x8`, reached after token conversion/classification/formatting. Constructor evidence says the relevant embedded member is at `+0x18`, but its copy helper and destructor use nontrivial Qt shared-data operations. The append implementation and item cleanup are not present in the retained captures reviewed here.
3. **REGIONS owner:** append target is optimizer vtable `+0xa8`, corresponding to member `+0x20`; the parser builds a per-row string-list local, but backing list construction/append implementation and element cleanup are not closed.
4. **Outer lifetime:** the global QRegExp initialization order/lifetime is also unavailable. Inferring a local delimiter or default-constructed empty regex would invent behavior.

No new dumpbin tool was available at the CP129 location in this turn (`Get-Command dumpbin.exe` and the retained Visual Studio path check found no executable). I reused the immutable retained bounded capture rather than retrying Ghidra or initiating a wider binary import.

## Next bounded cut

1. If extending the CP129 tokenized dispatcher, add a terminal outcome for `ENDDATA`, returning it without changing scalar state. Test that it is distinguishable from `Ignored`; describe it only as a parser-loop decision over already-tokenized fields.
2. Do not implement file-to-token conversion or row schemas yet. To proceed beyond the scalar/terminal value layer, obtain a fresh bounded immutable export for the QRegExp global initializer and direct bounded bodies for the `+0x8`/`+0xa8` append targets, their member copy/cleanup helpers, and destruction paths. Validate each address against the same original executable hash.

No source, build, test, GUI, or mutable REA changes were made for CP130. No public Type3/ShapeOptim admission or full-parser equivalence is established.
