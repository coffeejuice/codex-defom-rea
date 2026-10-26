# CP116 SPRING two-row selection and Save order

## Scope and evidence

This is a static trace of the retained original PE (`DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`). It cross-checks `resume116-spring-multirow-parser.md` and `resume116-spring-count-two-writer.md` with the retained Open caller `wave2-save-diffs-300-support-140ec8110.c`, Save caller `wave2-save-diffs-300-support-140ec9040.c`, item constructor capture `resume114-spring-item-constructor-140eb2470.asm`, and selection callback capture `resume114-spring-selection-140ecdee0.asm`. This establishes original static ordering, not runtime parity or candidate two-row support.

## Open: item ownership, count and initial selection

Open clears the spring `QListWidget`, reads the record count and allocates records, then loops over records in reader order. For each index `i`, it allocates a `0x78` item, copies record `i`'s `0x40` bytes into a temporary, and calls `FUN_140eb2470`. The item constructor calls the Qt list-item constructor with the list and type `0`, initializes the embedded record at `+0x30`, then copies those exact `0x40` bytes into that payload. There is no explicit row index passed to the constructor; the list receives each constructor insertion in loop order. Thus the two input rows own separate item payloads, row 0 then row 1.

After the insertion loop, Open obtains the existing QListWidget count and sets the SPRING page checkbox to `count > 0`. It resets the stored current-row field to `-1`; if the count is positive, it selects row 0, retrieves item 0, and explicitly calls `FUN_140ecdee0` for that item. Because the stored row is `-1`, this initial callback does not save a previous item. It loads row 0's two object IDs into the two combo boxes by matching combo item data, loads the two scalar and three direction text controls, and derives the row flag checkbox. Row 1 remains its original item-owned `0x40` payload; it is not copied into the visible controls during Open.

The page checkbox and row flag checkbox are distinct. For two rows, the page checkbox is checked by `count > 0`; each row's own checkbox reflects that row's flag. Both rows' object IDs must be represented in the corresponding combo item-data lists. A missing match leaves the current combo selection unchanged, so it cannot be treated as a valid row selection or assumed to preserve the row's ID.

## Callback: old-row writeback precedes requested-row comparison

At callback entry, a null requested item returns. Otherwise, when the stored current row is not `-1`, the callback retrieves the previously selected item, reads the current controls, and copies a newly populated `0x40` record back to that item's `+0x30` payload. Only after this writeback does it ask the list for the requested item's row and compare it with the stored row.

- If the requested row equals the stored row, callback cleanup/return follows the writeback. This is the Save-time same-row case: visible controls update the current item payload, and no row is reloaded.
- If the requested row differs, the callback copies the requested item payload locally, attempts to select each combo entry by item-data ID, formats five doubles using QString general format with precision 6, sets the row flag checkbox, and records the requested row as current. Thus changing selection from row 0 to row 1 first commits row 0's visible controls into row 0's payload, then loads row 1.

The selection callback's conversion path reparses the visible numeric strings with Qt `toDouble`. Consequently the writeback payload uses the display/parse roundtrip, not necessarily the untouched source double when it has more than six significant digits.

## Save: selected row mutation, then all-row snapshot

Save first checks the SPRING page checkbox and existing list count. If unchecked or count is below one, it removes both spring files. With the page checked and two items, Save obtains the current item and calls `FUN_140ecdee0` before allocating the `count * 0x40` snapshot. That callback saves the visible controls to the current item's payload before the caller starts copying items.

For an Open→Save with no intervening row change, row 0 is still selected. The Save-time same-row call updates row 0 from visible form values and returns; row 1 remains its original item payload. The snapshot loop then copies item 0's payload followed by item 1's payload into a two-record buffer. The writer receives both records in that order. This is the distinction to preserve: **row 0 is mutated from the selected controls; row 1 contributes its still item-owned payload to the snapshot.**

If the user selected row 1 before Save, the earlier row-change callback had already committed row 0 before loading row 1. The Save-time callback then commits row 1, after which the same item-order snapshot contains both resulting payloads. The snapshot is not built from the currently visible row alone.

The CP116 writer report independently agrees that the writer serializes records in buffer order and gives an exact candidate-format two-row/flag-zero example. The CP116 parser report independently qualifies two complete rows with absent `SPRING2.INI`; neither report enables the candidate page/list path or proves an original runtime match. Keep the candidate's positive-row guard closed until the root-owned list state, exact combo bindings, current-row writeback and writer integration are present.

## Remaining boundary

Static evidence covers the two-row Open item loop, selection callback's order, and Save snapshot loop. It does not establish runtime behavior for invalid/missing combo IDs, partial rows, allocation failures, arbitrary edits, or physical writer failures. No source, test, build, GUI or mutable REA state changed.
