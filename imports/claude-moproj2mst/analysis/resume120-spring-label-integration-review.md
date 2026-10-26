# CP120 SPRING actual-label lifecycle integration review

## Result

The frozen candidate producer and row transition preserve the source-qualified sequence for the admitted private profile: create the ID-zero `None` label, append eligible object labels in model order, use the same ordered labels in both combos, and commit the previously displayed item before switching or returning for a same-row request. The candidate does not claim native QWidget construction or unconstrained label conversion. The placeholder is `0   - None`; the earlier bare-`None` statement in `resume119-spring-combo-label-source.md` is superseded by `resume120-spring-placeholder-reconciliation.md`.

## Producer sequence

Original `0x140ec8b00` first clears the two combo widgets, constructs `None`, pairs it with integer zero, and sends that pair through the shared `0x140ec98e0` formatter (`"%-3d - "` plus name). It then visits object numbers 1 through `objectCount`; a null numbered lookup exits before the accumulated label list is inserted. Kind 7 objects are skipped. Other objects contribute a label built from signed object number and the name at `+0x828`. After iteration, the accumulated `QStringList` is inserted at index zero into the first combo and then the second. This is visible text, without QVariant item data.

Candidate `springComboItemLabels` follows that order: it appends `(0, "None")`, requests each 1-based object, returns `nullopt` on null, skips kind 7, reads the object number/name, and appends the formatted label. It then copies the finished ordered labels into `second`, representing insertion into both controls. Returning `nullopt` rather than inserting an incomplete list on null matches the original early return: the native routine abandons the accumulated list before either combo insertion. The 4096-object cap, required NUL within the 0x54-byte name field, printable ASCII restriction, and 64-byte label cap are explicit candidate admission guards; they are not native source limits.

The placeholder is formatted, not inserted as bare `None`: setup stores zero at `[rsp+0x48]`, constructs the literal `None` as a `QString`, then calls `0x140ec98e0` with the zero pointer and name. The builder's `"%-3d - "` literal makes the visible result `0   - None`. Candidate `append(0, "None")` reproduces this for the admitted ASCII profile.

## Transition sequence and combo effects

The retained handler `0x140ecdee0` commits the old stored row before comparing it with the requested row. It parses the current visible text of both combos, reads and converts the five visible numeric fields, converts checkbox state to 0/1, and copies the resulting record back. A same-row request returns after that writeback. On a row change it loads the requested record, searches the first combo labels for the first parsed integer match and applies that index, then does the same for the second combo. Missing matches leave the respective current index unchanged. It then displays the five values, loads checkbox state using `flag == 1`, and updates the stored row.

Candidate `transitionSpringItem` commits using current label indices and visible fields before its same-row check. On a row change it mutates the first combo index before processing the second, then loads visible values/flag and updates `currentRow`. Thus the observable stage order is retained, including a first-combo update before a later second-combo rejection in the candidate's extra guarded failure path. Normal source-qualified labels parse in index order; duplicate parsed IDs select the first match, and a missing ID retains the old index. The old-row writeback likewise parses visible text rather than trusting stored item data.

The candidate additionally requires `requestedRow == listCurrentRow`, bounds label count/length, rejects non-ASCII and overflowing numeric prefixes, and admits only finite successful numeric conversions. These guards narrow the candidate profile. The original event route is `itemClicked(QListWidgetItem*)`, so programmatic list movement alone does not invoke the handler; the separate list-current-row state is appropriate for representing that distinction. This does not establish event delivery for a real Qt list widget.

## Qualification

This is a static sequence review of the retained original captures and current candidate source. It did not run the original GUI or build/run the candidate. Within the stated bounded profile, the combo producer and transition ordering are aligned. Candidate safety caps, event simulation, actual widget insertion, arbitrary name storage/encoding, and conversion/overflow behavior remain separate from native-parity claims.
