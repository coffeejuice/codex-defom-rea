# CP119: Spring item-click transition and combo-label correction

The native handler can support a bounded private row0 -> row1 -> row0 transition, but its combo inputs are **parsed label text**, not QVariant item data. A second correction is equally important: loading a record checks raw flag ==1, not flag !=0. Current canonical numeric-label/flag0 fixtures remain valid; they must not be relabeled full native GUI or public Session acceptance.

## Bounded original proof

Original EXE SHAfd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. New read-only DUMPBIN/import receipts:

- `resume119-spring-label-static.json`, SHAc09ab1c4b09360625f52dba4f2cd1921233fa0ae404a09b9d5e6cd1523325fba, binds the original/tool hashes, commands, imports, literals and two complete helper captures.
- `resume119-spring-combo-label-140ec8da0.asm`, capture SHAe7b577b7ea27cddf656b616346d4cc9f2d3e9a5e036230acd397e95c3ba8ffda; native140ec8da0..140ec8e69 SHA15f8ac6483300ee592eaae43cdfc719f5facd14ac591a36229f9ee42a36c9866.
- `resume119-spring-label-parser-140ec9980.asm`, capture SHA283376436060d0f7ecc918d8ed8e2b482db650206bfce74c2d4831cdcaf7acb5; native140ec9980..140ec9a47 SHA8ee0571c2976dcf9e21b8c604a0dbf50f59276d4417b2db6cb22a39ed1783f07.
- `resume119-spring-label-builder-static.json`, SHAb7f2f88fec0a1f162cc26d45dea873e0c1b787c81cd7d4810e0ae9f76b430ce2; builder capture SHA72b4ce2c2637f91e45022212dc545c9a2205aa4cb1114636730a86ee5287daf4 and native140ec98e0..140ec9970 SHAb2376f94e7b476ea5a1d5d98a6d3326babd96534ff6dc4b17ee7aac6debde0c0.

The PE imports resolve141e0b4a8=currentText(QComboBox),141e0b4a0=count,141e08f50=itemText(int),141e0b4c8=setCurrentIndex(int). No itemData/QVariant is used on this path. Helper140ec8da0 visits indexes in ascending order, parses each itemText through140ec9980, selects the FIRST equal parsed integer and exits; no match leaves the prior current index unchanged. Writeback separately parses currentText through that same helper.

Parser140ec9980 initializes its int0. Empty QString explicitly returns0. Otherwise it converts toLocal8Bit, uses unchecked strcpy into the208-byte stack region rsp+40..rsp+10f, calls sscanf with literal `%d` at142b14f84, ignores scan status and returns the initialized/parsed int. A nonempty failed scan therefore returns0. Do not replace this with full-string integer validation or QVariant conversion.

## Labels, bounds and behavioral differences

Builder140ec98e0 uses literal142b14ff8 `%-3d - ` with the signed object number, then appends the supplied QString name. For example ID10/nameTop Die produces `10  - Top Die` (number width3 includes one pad space, followed by space-hyphen-space). The source combo producer first passes literal142b14ee0 `None` through140f34710 and formats it with ID0, then walks model.object(i+1); kind7 is excluded, and a missing numbered object ends population rather than silently skipping it. It accumulates QString labels and calls QComboBox::insertItems(index0,QStringList) for both controls; this is text-only insertion, with no QVariant item-data argument. Translation/default-codec conditions still qualify the final label bytes.

For canonical short ASCII labels, parsing recovers exactly object(i)+8. Current tests use explicit qualified ID projections or default short names, so their accepted numeric behavior remains valid. The factual correction is their claimed producer/interface meaning: those ints are the parsed IDs of admitted labels, not stored Qt user data. Arbitrary externally supplied item-data lists have never been proven equivalent.

Concrete divergences:

| Label/current state | Native result |
| --- | --- |
|`12suffix`, `12 - die`, `012 name`, `+12name`|`%d` decimal prefix is12; suffix and leading zero are not a full-label rejection.|
|`10 - A`, `10 - B`, current index1, requested ID10|first match selects index0; an ID-only state cannot record the resulting label/index distinction.|
|empty or `bad`|parsed0; an unmatched scan does not keep an unrelated stale integer. A first bad label can match requested0.|
|requested ID99 absent; current text`10 - die`|index/text remain; a later writeback stores10, not99. No source repair/fallback selection occurs.|
|QVariant data99, label`10 - die`|native matches/returns10; modeling data99 can wrongly refuse10 or admit99.|

The recovered layout has name[0x54] at+828. A genuinely NUL-terminated ASCII name within that field has <=83 bytes; the signed-int32 formatter prefix has <=14 bytes, so total local8bit label<=97 bytes plus NUL, fitting208. Layout and native unbounded set_name/strcpy do NOT prove every loaded name is terminated/ASCII/bounded. Multibyte conversion can expand. Review reconciled the distinct stack frames: builder140ec98e0 allocates0x48; parser140ec9980 allocates0x138 and its character region is208 bytes. These are different functions, not conflicting parser sizes. Root selected a more conservative **<=64 ASCII bytes including NUL** admission for generated/explicit label contexts. It is candidate policy, not a native validation limit; a valid97-byte native label is still outside that first candidate profile. Do not truncate or silently raise the cap to207. The checked canonical model/default-name projection is a safe existing subset; arbitrary name/codec/overflow behavior remains unqualified.

## Event, visible controls and flags

Constructor connection UI+1610 is `itemClicked(QListWidgetItem*) -> slotSpringLoadDieSelected(QListWidgetItem*)`, not currentItemChanged. Open resets stored row+118=-1, programmatically selects0 and explicitly calls the helper for item0. Programmatic selection alone must not be treated as a click callback. Stored callback row and actual QList current item are distinct values: Save obtains the actual current item and calls the helper even if a programmatic move did not previously update stored row/controls.

For a nonnull requested item,140ecdee0 first commits the old stored row if it exists: parse both combo currentText values; read five line-edit texts; QString::toDouble(nullptr); derive checkbox0/1; memcpy0x40 to the old item+30. Only then does it compare requested row with stored row. Same row returns after writeback without reloading. A different row loads its payload, performs both first-match combo searches, displays all five doubles with general6, sets the flag checkbox and records its row. Null requested item exits without committing anything.

The five numeric controls map to record+8/+10/+20/+28/+30 through UI+1638/+16a0/+1648/+1690/+1680. Names in setup correspond to stiffness/preload/max-displacement/damping fields; the legacy C++ array name `direction` is not proof these last three are a geometric direction. Retained constructor/slot/automatic-route maps show no separate Spring combo/text callback that immediately commits the item. Visible changes are read at the item handler/Save boundary. Validators are attached later by140eb9090; the handler itself performs no acceptable-input test and passes null to toDouble. A first private transition should retain the existing finite/known-conversion guard rather than invent broad invalid-text conversion support.

**Flag load is exactly1:** at140ece530 cmp[rsp+58],1; the equal branch supplies true to setChecked, every other int supplies false. Save at140ece225 reads isChecked and stores1 atECE232 or0 atECE23c, then copies the old payload atECE257. Native load rangeECE530..ECE56d SHA710d2cf021e7a2df7d885f4c6a1cff99ce39c45e9009283c1d45fb945b1050aa; writebackECE20f..ECE259 SHA2fae71d3f69bbf94552128de88d200c8032593bcda8cf35090988563acb22f6a.

Consequently selected flags2/-1 normalize to0, selected1 stays1 and untouched item flags stay raw. A click0->1->0 commits each left item with this exact normalization. Do not apply a global bool(nonzero) conversion. Earlier reports saying nonzero loads checked are superseded by this body-level proof; flag0 cases are unaffected.

Page checkbox+15f0 is separate: setup explicitly checks it, Open derives it from list count, and duplicate captured toggled->setEnabled routes target group+1600. Preserve duplicate delivery if modeling signals. The row callback does not update page checked state or inspect effective widget enabled ancestry before reading fields; no new enable/validation gate should be inserted. Actual effective enablement and delivery remain outside the private projection.

## Smallest reusable private transition/interface

Keep the current parsed-ID span route explicitly qualified for canonical labels; correcting its comments/parameter meaning need not change accepted behavior or enable public editor. A faithful general click model additionally needs ordered combo labels and current indexes, because IDs alone lose first-match/duplicate/missing-match behavior. Root should reserve a private label context plus stored callback row/current list row rather than treating one integer as both.

A bounded `transitionSpringItem(state, requestedItem, labelContext, reason)` can reuse one ordered item owner and existing visible controls. Reasons distinguish explicit initial load, delivered item click and explicit Save-current-item call. Programmatic list movement changes only the actual list-current index; it does not call this transition. Null requested item is a no-op. For0->1->0, commit the old payload before new label search/load, keep prior combo indexes on missing match, then load texts and flag==1 and record the requested row. Return a reached-stage report so any excluded conversion/label case does not silently roll back an already committed old item. Caller supplies the actual click/current-item qualification; the helper does not emit a native event or infer it from setCurrentRow.

The first implementation can keep bounded complete typed records/canonical ASCII labels/int32 prefix/finite numeric controls and exclude arbitrary edits, non-ASCII/long labels, integer overflow, item removal/reorder and unknown widget events. A label-aware index context is needed before claiming duplicates or missing-ID transitions are implemented; alternatively keep those explicit guards for the first unique matching-label subset. Root owns shared-header/interface decisions; no page edits are authorized by this report itself.

At Save, retain scalar sidecar effects first, then checkbox/count gate, current-item callback, all-item snapshot in list order and writer. Compute the any-nonzero flag predicate AFTER selected-item normalization. For flags2/0 with selected row0 the snapshot becomes0/0 and follows delete; flags0/2 with untouched row1 stays nonzero and follows companion write gate. Per `resume119-spring2-normal-path.md`: any nonzero snapshot flag means primary false skips companion entirely; primary true attempts the raw flags list; all-zero means primary attempt then readable-delete companion regardless main result. Preserve direct partial effects/no rollback and distinguish open success from later stream/Close failure. The current early reconstruction preflight still does not claim native partial effects for excluded profiles.

Tests should cover precision-sensitive0->1->0 and same-row writeback-before-comparison; programmatic movement without implicit callback; labels with suffix/leading zeros/sign; duplicate first match/index; empty/bad parsed0; missing-match retained selection and later rewritten ID; flag0/1/2/-1/INT_MIN selected normalization versus untouched raw flags; exact ordered main/companion bytes; primary-denied skip/write versus all-zero cleanup; companion-denied preserving main; and explicit overlength/non-ASCII/overflow/invalid-edit guards. Keep full Session/native-widget/original same-run acceptance separate.

Only this report and the bounded analysis captures/receipts above were written. No source/header/build/test, GUI, original execution or mutable REA operation occurred. Current public mode300 still refuses at the selected private palette; these findings qualify a private page model, not that public constructor.
