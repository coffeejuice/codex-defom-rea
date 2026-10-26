# CP138: bound Advanced frequency combo change

The next source-closed private transition is delivered frequency currentIndexChanged(int). It composes naturally with the recovered textEdited event: automatic choices immediately store+0 or row0's-1 sentinel, while custom choice only enables the projected input and retains its text/model value. Save must not synthesize either event.

Only this report was written. No source/build/GUI/original execution/REA database change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Evidence is retained decompilation/slot metadata and bounded immutable-PE DUMPBIN, not observed native signal delivery.

## Native order, including the decompile omission

Slot-map indices52..57 reach the six handlers below. **All six query current SC and read SC+32c/compute row×stride before testing the selected index.** The row0 code at140ecfad1 explicitly reads stride, then IMUL0 atfad7; the decompile optimized this apparently unused read away. A custom/no-store choice therefore still requires native SC/stride access. Do not import Environment's no-SC callback behavior here.

After that read/index calculation:

- Row0 choice0 stores positive-double-zero then setEnabled(false); choice1 loads literal1421d3c90 (bytes `000000000000f0bf`, exact double-1) and stores it, then setEnabled(false); choice2 only setEnabled(true).
- Rows1..5 choice0 store positive-double-zero then setEnabled(false); choice1 only setEnabled(true).
- No branch reads/formats/clears the line-edit text, calls toInt, sets the combo index, allocates/repairs the table, or invokes Apply/refresh/I/O. There is no same-index shortcut.
- The values pointer SC+378 is read only by the store branches. Custom choice does not dereference it. Missing values cannot be treated as a prerequisite for that non-store branch.

For row0, SC lookup is140ecfac2, stride readfad1/IMUL0fad7. Choice0 values loadfaef, XORPD+0faf6, storefafa, disablefb14. Choice1 values loadfb2d, literal loadfb34, storefb3c, disablefb56. Choice2 enablefb7a. All enabled calls resolve IAT141e0ba40 QWidget::setEnabled. Other rows use the same source ordering with their stride multiplier1..5 and target line-edit offsets1398/13c0/13f0/1418/1440 (row0 is1370).

| Row | Complete half-open body | SHA-256 |
| --- | --- | --- |
| 0 | 140ecfab0..140ecfb85 | `2854dcec380b5ae36c1cc2ac2b80f176bf9da4c0aa458c61285dc2d7856a7ab2` |
| 1 | 140ecfb90..140ecfc20 | `8cf2bf34943fcee45fff9488a6cf201e3610596b5b2f13f60389552e768e2292` |
| 2 | 140ecfc30..140ecfcc2 | `c160797cdfbcc1b141380d8801f34653bc2f3549b4a7a593bf79791cf21b6533` |
| 3 | 140ecfcd0..140ecfd63 | `1293961485ca8a7153163e956790969469800bf1c77e3fb9ae0225db967e3947` |
| 4 | 140ecfd70..140ecfe03 | `a727b220a33eb931ff6c7d509f7dce205dd85840d4e10c0af83b8663461d07ff` |
| 5 | 140ecfe10..140ecfea3 | `e25553477dcd705f6dcae2d9c52e218c32c0cfc07884a652a28d9f5429e87ef6` |

## Smallest private API and missing page field

Root shared owner.hpp declaration:

```cpp
ApplyResult StagedEditor300::frequencyIndexChanged(
    Editor300Binding, int row, int selection);
```

Root shared AdvancedPage field:

```cpp
// Private projection of the latest reached native setEnabled argument;
// unknown for an otherwise unqualified/unfilled page, not physical QWidget bits.
std::array<std::optional<bool>, 6> frequencyInputEnabled{};
```

The complete initial Advanced Fill explicitly calls setEnabled after each setCurrentIndex, so root should initialize these projected values in fillAdvanced3D's existing frequency loop: row0 enabled iff combo==2, others iff combo==1. No signal occurrence must be inferred for this initialization. Existing exact-int Fill profile guarantees its implied automatic callback would preserve stored bytes; no broader numeric or table allocation admission is proposed. Default nullopt avoids inventing known enabled state for a manually unfilled page.

The bound method requires same live Project/OPI generation/model/remesh and completed private page metadata/source-model/remesh tokens, row0..5 and valid choice(row0:0..2;others:0..1). The cached frequencyCombo[row] must already equal the delivered selection, representing the actual changed combo before event delivery, as in the meshing and title event contracts.

Then query actual current SC, require original sourceControl identity and read its column stride. Keep stride1 as the bounded candidate profile; do not use pageState early or make the custom branch independent of current SC. On custom choice, project enabled=true and return Completed without consuming rows/component counts/values pointer, numeric text or old value. On store choice, additionally require the existing six-row/ncomp6/non-null actual values profile, store+0 or-1, then project enabled=false and return Completed.

Missing/replaced SC, unrecovered stride, or unsupported store-table state returns UnsupportedSnapshot **before** changing the previous enabled projection or model data. The cached combo has already been changed by the caller and is not rolled back. These are candidate admission guards, not native graceful-null/failure semantics. The completed store has no later candidate guard that could erase its source-ordered effect. Later source exceptions propagate before enablement; physical QWidget failure/emissions/reentrancy remain outside the private value projection.

Do not clear/replace populatedFrequencyText or call frequencyTextEdited. Thus auto→custom enables the retained old text while the live frequency remains0/-1 until an actual subsequent textEdited event. An explicit repeated same-index occurrence reasserts the model store/requested argument rather than taking a shortcut. No numeric locale argument is needed: only fixed binary+0/-1 values are stored.

## Exclusive implementation and focused contract

Worker files: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_frequency_index.cpp`, implementation receipt. Root owns owner.hpp, the AdvancedPage field in simcontrol300.hpp and the single derived-enabled assignment in existing fillAdvanced3D(simcontrol300.cpp). No lifecycle/CMake/public changes. Expected target `m2m_mdt_simcontrol300_frequency_index` / CTest `m2m.mdt.simcontrol300_frequency_index`.

Focused cases should verify:

1. Initial Fill's enabled values for0, row0-1 and custom positive/negative accepted model values under existing profile.
2. All6 automatic0 choices overwrite only addressed model double with positive-zero bits before enabled=false projection, preserving line text and other row bits/owners.
3. Row0 sentinel choice1 stores exact-1 and disables; row0 custom2/other custom1 enable without changing model/text. Compose sentinel/auto→custom→explicit textEdited to prove retained text does not write automatically.
4. Repeated same-index occurrence restores an externally changed frequency value and requested enabled state; same-index cannot bypass explicit delivery.
5. Missing/replaced SC and stride mismatch guard every choice before enabled projection, including custom; row0's apparently redundant stride read is preserved.
6. Store branches with missing/wrong table shape retain old enabled/model values, but custom branches still enable with null values/wrong row/component shape after valid SC/stride. Restore test-owned metadata before teardown; no fabricated pointer probes.
7. Wrong index/selection/cache delivery, wrong binding/OPI generation and partial/unconstructed owner guard before effects. Public constructor remains unsupported.

The current candidate models initial combo and populated text only, with no delivered-index method or requested-enabled field. This is not a duplicate of textEdited: combo changes may replace model data with automatic sentinels while preserving text, and the later edit callback performs conversion. Existing Advanced Apply deliberately consumes neither control; its behavior and prior numeric admissions stay unchanged.

Public palette/6504abb0 and actual widget receiver/disabled-bit semantics remain guarded. TIMEID rebuilding/DB-step tail, operation-name/tree refresh, unit dialog and picking/frame selection retain independent owner dependencies. This proposal closes a private event/model ordering only.

## Capture qualification

Bounded DUMPBIN used pinned BuildTools14.44.35207 SHA `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, banner14.44.35228.0. The original literal was read directly from section-mapped PE bytes; end-exclusive body spans follow unwind bounds and final returns. Reused Fill decompile140ec5bc0 SHA is `34fc6d60edcb5cc4c50e99db08b2a4d2b265ef578d26fa65860df072279710ea`. No raw capture outside this report was written and no reconstructed executable/deployment was inspected or produced.
