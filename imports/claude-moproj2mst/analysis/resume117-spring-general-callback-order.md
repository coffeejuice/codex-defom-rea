# CP117 SPRING general callback and writer order

## Source boundary

This static review uses the retained original `DEF_GUI_MO_64.exe` (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), especially `wave2-save-diffs-300-support-140ec8110.c`, `140ec9040.c`, `1410dab40.c`, `1410da800.c`, `1410da8f0.c`, `1410da690.c`, `1410db030.c`, and `1410da9e0.c`, plus CP114 selection/item captures and CP116 parser/writer reports. The conclusions below are original static call/control-flow evidence, not matched original runtime results.

## Open, selected row and later item clicks

Open clears the existing Spring list, reads `n` records, allocates one custom `0x78` item per record and copies each `0x40` payload into that item's `+0x30` storage. The list item constructor receives the list and type `0`; no explicit insertion index is passed, and construction occurs in reader order. Open then uses the live QListWidget count to set the page checkbox, stores current row `-1`, selects row zero when the list is nonempty, and explicitly invokes the selection helper for item zero.

The constructor connection record `analysis/wave2-save-diffs-300-constructor-connections.json:109`, cross-checked by editor300, connects the helper to `2itemClicked(QListWidgetItem *)`, not `currentItemChanged`. Therefore the programmatic row-zero selection alone should not be modeled as an implicit click callback; the caller's explicit helper call is the source-backed initial fill. The available call evidence qualifies later item-click transitions, not every way a selection could change (for example, keyboard/programmatic row movement without a click).

The helper processes a non-null requested item in this order:

1. If stored current row is not `-1`, read the old row's current combo IDs, five visible numeric fields and row-flag checkbox; convert them into a `0x40` payload and copy that payload back to the old item's `+0x30`.
2. Ask the list for the requested item's row and compare it with the stored index.
3. If it is the same row, return after the writeback without reloading controls.
4. If it differs, load the requested item's payload, select combo items only on matching item-data IDs, display its five doubles with QString general format/precision 6, set the row-flag checkbox, then record the new current row.

So a click from row 0 to row 1 first commits row 0's visible values, then loads row 1. A Save-time call for the already selected row writes that selected row's controls and returns. A no-edit Save while row 0 remains current therefore normalizes row 0 through the display/`toDouble` roundtrip, while row 1 remains the separate item-owned payload until snapshot. If row 1 was clicked before Save, that earlier callback committed row 0; Save then commits row 1, and both payloads are later copied. A missing combo ID does not select a replacement: the current combo selection remains. It is unsafe to treat such a row as an exact no-edit selection, because later writeback may store a different ID.

## Save list-count and snapshot order

The Save caller reads the page checkbox and current QListWidget count at the Spring branch. Unchecked or count below one takes the branch that attempts removal of both `SPRING.INI` and `SPRING2.INI`. Otherwise it obtains the current item and calls the same selection helper before allocating a `count * 0x40` snapshot. It then loops from item index zero through `count - 1`, copies each item's `+0x30` payload into the same indexed record, and calls `FUN_1410db030`. It does not serialize only the currently visible row or reuse the reader's original byte buffer.

The original writer loops over its signed count without a fixed one/two-row cap. It scans every record flag at `+0x38`, builds the main QStringList in item order, and builds the companion flag QStringList in parallel. Integers use base 10; doubles use `QString::number(value, 'g', 6)`. Literal LF items delimit count, each first-object value, and each row's first and third direction component. The `FUN_1410da9e0` stream helper opens the final QFile directly in WriteOnly mode, writes each list item, appends a tab after every item other than LF, appends final CRLF, then closes the file. This writer contract generalizes beyond the CP116 two-row fixture; it does not validate the input count against allocation bounds itself. The Save caller allocates the all-row snapshot after checking the size multiplication, and the writer's direct loop has no fixed one/two-row limit. Zero count returns false before either file attempt, while the outer Save gate normally removes both files instead of calling the writer.

## Output failure and partial-effect order

`FUN_1410db030` attempts the main `SPRING.INI` writer first. The helper's false result is specifically the QFile open gate; after successful open it performs QTextStream writes and closes QFile but does not check stream status or a close result. Thus a post-open write/close problem is not reported as helper failure by this code path, and the destination may contain a partial direct-write result. If main open fails and any record flag is nonzero, the companion write is skipped; if main open succeeds, the companion write is attempted even if the main stream later failed because the helper still reports open success. The outer Save caller ignores the SPRING writer's Boolean result, so this does not roll back earlier sidecars or prevent Save from continuing.

Companion behavior follows the flags and the main open result:

- If every row flag is zero, `SPRING2.INI` is passed to the readable-open/delete helper after the main write attempt regardless of the main helper's Boolean result. The delete attempt itself can silently leave an unreadable or locked existing file.
- If any row flag is nonzero, the companion list is written only when the main helper returned true (main file opened). The companion writer then has its own open gate; failure returns false from the Spring writer, but the caller ignores that result. Main/companion writes are direct and nontransactional; no rollback is established.

These are source-qualified control-flow results. The retained evidence does not determine exact bytes after a host-level short write, nor does it qualify allocator failure cleanup or hostile/unbounded counts.

## Candidate gap

The currently reviewed candidate reader and page state admit only exact one/two-row fixtures. `exactSpringItems` hardcodes their field values; `SpringSelectionState.currentRow` is initialized to zero; `applyExtra3D` accepts only row zero and requires the visible texts and selected IDs to equal the initial first row. It parses and rewrites only that first item, preserves the other item payloads, formats a generic vector loop, and always follows the zero-flag companion-delete branch. This is a bounded unchanged-fixture bridge, not the original general list/callback/writer behavior.

General positive-count support still needs separately justified count/allocation limits, complete-row parse admission, ordered item payload ownership for every admitted row, source-qualified combo IDs for every row, row-click transition writeback/load behavior, and all-row item-to-snapshot copying. Nonzero companion flags, arbitrary edits and UI movement without the captured click path must remain guarded. Parser and output failures must retain the native attempt order rather than being collapsed into atomic batch success/failure.

## Cross-check and changes

The lifecycle conclusion agrees with `analysis/resume116-spring-multirow-parser.md` and `analysis/resume116-spring-count-two-writer.md`. The additional event detail (itemClicked rather than currentItemChanged) was cross-checked with the editor300 constructor connection capture. The reader review in `analysis/resume117-spring-general-parse-contract.md` independently confirms count/row helper order, zeroing, absent-companion behavior, partial-group handling, and lack of final EOF check; it recommends a count-256 bound as an explicitly candidate-only resource policy, not original behavior. No source, test, build, GUI or mutable REA state changed.
