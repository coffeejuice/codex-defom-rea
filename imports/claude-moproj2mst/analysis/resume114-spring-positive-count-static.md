# CP114 SPRING positive-count static recovery

## Result

The two formerly missing bodies are now recovered from the retained original PE using bounded local DUMPBIN disassembly (no REA import/session and no GUI). This closes the item payload ownership and row-selection read/write edge that blocked the CP108 review. A narrow public candidate is now source-closed: one valid ASCII record, absent `SPRING2.INI` (flag zero), and no user edit. This does not qualify allocation failures, malformed rows, unavailable object IDs, or original runtime parity.

Original image: `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Item construction: `FUN_140eb2470`

Capture `analysis/resume114-spring-item-constructor-140eb2470.asm` covers `0x140eb2470–0x140eb251e` including `ret`.

- It calls the imported `QListWidgetItem` constructor with the passed list and row `0`, which inserts the item into the list.
- It sets the custom item vtable, initializes the embedded `0x40`-byte record at item `+0x30` through `FUN_1410da450`, then copies exactly `0x40` bytes from the caller's record into that same item payload.
- It stores the fourth argument (the caller's vtable-derived context pointer) at item `+0x70` and returns the item pointer.

The caller `FUN_140ec8110` allocates `0x78` bytes, copies one `0x40`-byte reader record into a temporary, calls this constructor, and retains the returned item in the spring list. This establishes that Save's later copy from `item + 0x30` reads the original record payload, not a reconstructed or text-only representation. The `+0x70` context is preserved; this capture does not assign it a higher-level semantic name.

## Selection callback: `FUN_140ecdee0`

Capture `analysis/resume114-spring-selection-140ecdee0.asm` covers `0x140ecdee0–0x140ece5b2` including `ret`.

- A null item argument exits before reading list or form state.
- If the stored current-row field at form `+0x118` is not `-1`, the callback fetches the prior current item, reads the current combo/text/checkbox values, converts them into the record fields, and copies the updated `0x40` bytes back to that item's `+0x30` payload. This happens before checking whether the requested row equals the old row.
- It asks the list for the passed item's row. If that equals `+0x118`, it returns after saving the current visible values to that item's payload.
- Otherwise it copies the passed item's `+0x30` record to a local, selects combo entries by matching the two object IDs against combo item data, formats the two scalar and three direction doubles with QString general format/precision 6, derives the checkbox from the record flag, and updates the form. It then stores the passed item's row into `+0x118`.

Open's captured caller resets `+0x118` to `-1` before selecting row zero. Thus the initial selection loads the newly constructed row without first saving another row. Save's captured caller invokes the same helper for the current item before copying every item's `+0x30` payload into the writer snapshot; at that same-row call, current visible values are saved first. This closes the core record owner and ordering contract.

The row-change path has two direct helper boundaries: `FUN_140ec8da0` searches a combo's item data and changes its current index only on an integer match; `FUN_140ec9980` converts a QVariant item-data value to an integer. If there is no matching item, the helper leaves the combo's current selection unchanged. Therefore the bounded one-record candidate must use object IDs present in the populated combos; unsupported/missing-ID fallback must stay guarded.

## Narrow implementation proposal

Implement a one-record public Open→Save candidate under these exact gates:

1. `SPRING.INI` is valid ASCII with count one and a complete row: `10`, `0.5`, `1.5`, `20`, `0`, `1`, `0` in the retained reader's extraction order. `SPRING2.INI` is absent, so the existing source-qualified reader initializes the auxiliary flag to zero.
2. Both object IDs resolve to entries in the two populated object combos. No row-change event or user edit occurs between Open and Save.
3. Initial item construction copies the parsed record as above. Initial selection loads its values into the form; the Save-time same-row callback saves the visible values back to the same record. For double values, the form's `QString::number(..., 'g', 6)` display and subsequent `toDouble` roundtrip must be modeled (so input precision beyond six significant digits is not preserved). Save then snapshots the item payload and uses the already-retained `FUN_1410db030` ordering/formatting contract. Because flag is zero, Save removes/does not write `SPRING2.INI` according to that writer's branch.

Candidate ownership should be split to avoid claiming more than this slice: the native reader and exact one-record tests belong in `simcontrol300_native_io.cpp` and its focused test; record list/page plumbing and writer dispatch must be integrated by the root owner in `simcontrol300_io.hpp/.cpp` and the public IO test. A focused public test should assert the parsed one-record fields, same-row normalization, main-file bytes, absent/removed companion behavior, and retained count/path guards. Keep multi-record selection, missing/foreign combo IDs, edits, partial rows, invalid counts other than already qualified, and allocation failures unsupported.

This is source-qualified but candidate-only until a matched original run compares the same valid one-record fixture. No original execution was performed here.

## Captures and hashes

| Artifact | SHA-256 |
|---|---|
| `analysis/resume114-spring-item-constructor-140eb2470.asm` | `022927C43DD8742634D95A02A5E3C7EFB65A39327F741B9C8295E6EA2256D495` |
| `analysis/resume114-spring-selection-140ecdee0.asm` | `FAE61EECDC45C7E99E6D4ED9A19F8DAE7C4864EEE4B69EF934F1EB4493EDC6C2` |
| `analysis/resume114-spring-editor-setup-140ec8b00.asm` | `262763D0698A3D74C61572F96D89865AEA2C29AC3B16D8A2E59D089914435515` |
| `analysis/resume114-spring-editor-setup-140ec8e70.asm` | `F274CC631D6937614E03B55807172ABB92073A3074A0CA4690189EA4956F23A5` |

The `FUN_140ec8b00` / `FUN_140ec8e70` captures cover combo population and line-edit initialization. The one-record proposal additionally requires known, matching object IDs from those populated lists; it does not infer a fallback when an ID is absent. No source, test, build, GUI, runtime, or mutable REA state changed.
