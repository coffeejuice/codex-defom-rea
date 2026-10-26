# CP115 SPRING one-row page-admission design

## Decision

The one-record physical reader is ready, but `Extra3DPage` still cannot admit that row safely. Keep populated SPRING disabled until the page carries the native list, combo-ID admission, selected row, and selected editor values through Save. The narrow next increment should model only one unchanged row with both record object IDs present in their respective combo item data; it need not activate the broader editor session or palette.

## Source-qualified row transition

`analysis/resume114-spring-positive-count-static.md` covers the original item constructor and selection callback from the retained PE (`DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`). The constructor inserts a QListWidget item and copies the 0x40-byte `SpringRecord` payload to item offset `+0x30`. Open resets the current-row marker to `-1`, selects row zero, then calls the selection callback; the first selection loads the row into controls. The row callback resolves each record object ID against combo item data and changes selection only on a match, so missing IDs must remain unsupported rather than falling back to a different object.

On Save, the caller invokes the same callback for the current item before taking the payload snapshot. The callback first reads the visible combo/text/checkbox values and writes them back to the current item payload; when the requested item is already the stored current row, it then returns without reloading controls. Save copies the resulting 0x40 bytes from every list item and calls the writer. This puts the current-form-to-item copy before the row snapshot and SPRING writer. The one-row candidate must preserve this order even with no user edits.

The selected editor displays its two scalar and three direction values through `QString::number(value, 'g', 6)`; Save reparses the visible text. Therefore the no-edit result is the numeric `toDouble(number(value,'g',6))` roundtrip, not an unconditional copy of the parsed input double. Object IDs are selected through combo item data. The row flag controls `SPRING2.INI`; separately, the SPRING page checkbox is initialized from whether the list has rows. For the CP114 qualified row, the flag is zero, so the page checkbox is checked while the per-record flag checkbox is not. The existing exact reader record is `10, 0.5, 1.5, 20, 0, 1, 0`, with absent `SPRING2.INI` and zero companion flag (`analysis/resume115-spring-reader.md`).

## Current interface gap

- `Extra3DIo::readSprings(std::vector<SpringRecord>&)` can supply the row, but does not supply either populated combo's item data.
- `fillExtra3D(Extra3DIo&, NumericLocale)` has no model/combo context and returns `nullopt` for any positive spring result.
- `Extra3DPage` has no spring page-checkbox state, list item payloads, current row, selected combo item IDs, or visible spring edit text. Its `supportsPopulatedSprings` constant remains false.
- `applyExtra3D(Extra3DIo&, const Extra3DPage&)` currently removes both `SPRING.INI` and `SPRING2.INI` after the scalar sidecars. It has no state from which to snapshot the current item or choose the writer's flag-dependent companion behavior.
- The lifecycle does have `GuiModel&` at `openInitialEditorInto`, but its Extra3D dispatch currently passes only `io` and numeric locale. `applyInitialEditorPageState` applies Extra3D after the preceding page writers and before the recovery tail; this is the right place to preserve Save order, not a reason to open the full session/palette state.

## Minimal root-owned API shape

Add a small, private `SpringSelectionContext` supplied by the root lifecycle owner at the Extra3D fill call. It should contain the actual item-data IDs from each populated object combo (first and second combo separately); do not substitute a general model-object list unless the combo-population path proves those IDs and ordering match. The current fill call does not receive the model or combo state, and no existing `Extra3DIo` method can report item-data presence. Add to `Extra3DPage` only the state needed for the one-row profile:

- page checkbox state, initialized from `springItems.size() > 0`;
- `std::vector<SpringRecord> springItems` as the list-item payload snapshot;
- current row index (0 for this admitted case);
- two selected combo item-data IDs, which must match the current row IDs and exist in the corresponding context lists;
- five visible numeric text fields (the two scalar values and three direction values), initialized with Qt general format, precision 6;
- the record flag checkbox, initialized from `flag != 0`.

Use an overload such as `fillExtra3D(Extra3DIo&, NumericLocale, const SpringSelectionContext&)` so existing isolated reader/scalar callers retain the empty-row path. A narrow Spring-specific helper may perform the selection transition and the Save-time selected-item writeback. Keep `applyExtra3D`'s scalar sidecar sequence unchanged, then branch at its existing SPRING tail: unchecked/empty retains the current remove-both behavior; the supported one-row checked state parses the five visible texts with the established Qt `toDouble` helper, writes selected combo item-data IDs and the flag back to item zero, snapshots that item, and formats/writes it using a new bounded candidate SPRING writer following the recovered `FUN_1410db030` contract. The current candidate has no such SPRING writer yet. The admitted profile must reject row counts other than one, absent/foreign combo IDs, nonzero companion flags or unsupported locale before any Spring write.

This exposes the exact Open→Save page state without creating `InitialEditorSession`, palette state, row-change behavior, or general user-edit support. The resulting bytes remain candidate behavior until a matched original run; the PE/source evidence qualifies the transition and the six-digit roundtrip, not runtime parity.

## Evidence boundary

This is a read-only architecture report. No source, test, build, GUI or REA state was changed. The next implementation should be owned by the root because it changes shared `Extra3DPage`/lifecycle dispatch and Save integration.
