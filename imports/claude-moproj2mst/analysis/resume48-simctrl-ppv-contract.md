# Resume 48: SimCtrl.PPV reader/writer contract

## Evidence and confidence

Reader grammar below is extracted from the complete retained decompile `evidence/gui_mo/s1/mdt/L0/A/b0.c`, function `FUN_140d4b400 @ 0x140d4b400`. The source file SHA-256 is `cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`. A bounded copy of that function is preserved in `analysis/resume48-ppv-reader-receipt.txt` (SHA-256 `e6fbe29513c30622a2086aaa13b4ae962ff65b37bfe185f6ea01a8deddd70290f`). This is retained static decompilation, not runtime observation.

The precise writer body `0x140d4afe0` was not found in the retained original files searched. Current source labels `0x140d4afe0` as the native save routine, but that is not sufficient evidence for its file grammar or failures. Do not infer writer behavior from the extension, address, or reader.

## Reader `0x140d4b400`

### Presence/open and count

1. Constructs `QFile(path)` and opens with the recovered flag passed by `func_0x000140004a60(..., 1)` (static evidence labels this as read mode). If open fails, it never enters the body and returns false (`uStack_c8` initialized to zero).
2. On successful open, it calls `FUN_1400670b0(param_1)` before reading the count line. This call is a list-preparation/clear candidate; exact effect needs its body before asserting whether preexisting rows are cleared.
3. Reads one line, calls `trimmed()` on a temporary result, then converts the original `QString` with `toInt(..., base 10)`. The return/ok flag is not inspected in the decompile.
4. Only `count > 0` enters row parsing and sets the result flag true. A zero or negative count leaves the result false even though the file opened successfully. Thus empty/zero-count is not equivalent to a successful empty list in this function.

### Row grammar and mutation order

For each of the declared positive-count rows:

1. Read one line and split using the referenced `QRegExp` `0x1446b4b20`, `Qt::KeepEmptyParts` (`split` final enum argument `1`). Its exact pattern text is not present in this decompile; it must not be guessed as whitespace or comma.
2. If fewer than six parts are returned, set the function result false, destroy the split list, and break. Prior rows have already been inserted; the function does not roll them back. The file is then closed and the partially mutated lists remain.
3. Parse part 0 and part 2 and part 3 as base-10 integers. Part 1 is copied as a `QString`. Parts 4 and 5 are parsed as doubles. Conversion success flags are ignored.
4. If and only if the split produces exactly seven parts, the decompile reads **part 5 again** as an integer, converts it to a boolean, and passes it to the optional associated-record append helper `0x140d4bb70(param_4, ...)`. There is no evident read of part 6 in this body. This may reflect native code's actual index or a decompiler/source qualification issue; preserve it as an unresolved anomaly pending the bounded assembly query below.
5. It compares part 1 case-sensitively with the literal at `0x142a13b0c`. When equal and `param_3 != 0`, it appends to `param_3`; otherwise it appends to `param_1`. The row append helper is `0x140d4af00`, called with the chosen list plus parsed fields (parts 2, 3, 4, 5). That helper's exact structure-field mapping is not inferred here.
6. The loop runs the declared count unless a row has fewer than six parts. Extra fields beyond the exact-seven special case are not inspected in this function.
7. It closes the file after the loop and returns the result flag. The function does not compare successfully parsed rows with the declared count after early malformed-row termination; a malformed row yields false with partial preceding mutation.

### Reader callers and list selection

In `b0.c` around its `loadSimCtrl` sequence, the caller first checks file existence. With a file present, the normal branch invokes KEY import and then checks for the PPV path. If that path exists, it obtains the active subproject's recovery list A through `0x14005a400`, operation recovery list B through `0x140970130`, and another operation-owned pointer through `0x1401cc700`, builds the path, and invokes `0x140d4b400(listA, path, listB, auxiliary)`. This establishes path reachability and list ownership arguments, but the callee's case-sensitive literal comparison decides which destination gets each row.

When the SimCtrl.KEY existence check fails, this caller follows `inheritSimCtrl` (`0x140062960`) and does not execute the shown PPV import branch. That is a distinct presence path; do not assume it searches/loads the PPV file from an inherited operation without tracing `inheritSimCtrl` and subsequent caller logic.

## Writer `0x140d4afe0`: unresolved

Current `source/apps/moproj2mst/src/engine/save_current.cpp` checks the operation recovery-B list after writing SimCtrl.KEY and throws unsupported when it is nonempty. The original address and normal Save reachability are retained in comments/audit material, but the actual writer function is absent from the decompile files searched. No exact claims are made here about:

- behavior when both lists are empty or only list A is nonempty;
- file creation/truncation timing;
- count line and record formatting;
- delimiter/escaping/locale/precision;
- seventh-field emission;
- write, close, or partial-failure results;
- whether the writer serializes recovery list A, operation list B, or both.

## Smallest next immutable evidence query

Use the already retained immutable original image/database only through a read-only bounded query. Do not mutate the shared REA database; another agent owns it. Request:

1. The exact bytes/disassembly and containing-function bounds for `0x140d4afe0`, plus its string/data references to `SimCtrl.PPV` and its immediate direct callers.
2. The target bytes/disassembly for `0x1446b4b20` to recover the regex pattern.
3. The exact instructions around reader `0x140d4b400`'s `parts.size()==7` branch, including which element is loaded before integer conversion and the argument passed to `0x140d4bb70`.
4. Small callee slices for `0x1400670b0` and `0x140d4af00` only if the collection clear/append field mapping is needed.

The smallest fix/test cannot be specified faithfully until writer grammar and malformed/failure ordering are recovered. After that query, implement only the reader/writer branches on the existing hooks and compare: absent PPV; present zero-count; positive count with six-field rows; exactly-seven-field row; too-few row after a valid prefix; and list-A-only/list-B-only cases. Preserve the native partial mutations and return behavior instead of normalizing malformed data.

## Status

- Reader file-presence, positive-count gate, minimum row width, per-row conversion, destination selection, early-stop and partial-mutation order: **settled from retained static decompile**.
- Exact separator pattern, clear/append structure mapping, anomalous seventh-field index: **unknown**.
- Writer grammar and all empty/count/failure branches: **unknown; native body not retained**.
- Current converter treatment: explicit unsupported Open/Save behavior; confirmed in-scope refusal, not a silent behavior proven absent from the original.
- No source edits, builds, app control, or REA database mutations were performed.
