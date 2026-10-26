# CP108 SPRING item-constructor static review

## Result

The retained evidence does not close the positive-count SPRING owner transition. The immutable original executable is present and its SHA-256 matches the qualified image (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), but the retained disassembly/decompilation set contains caller evidence rather than bodies for either target. No source-closed positive-record contract can be derived from those artifacts.

## What the retained captures establish

- `wave2-save-diffs-300-support-140ec8110.c`: Open copies `0x40` bytes from each reader record into a temporary and calls `FUN_140eb2470` with a newly allocated `0x78`-byte item, the spring list, the record copy, and a vtable-derived value. It later selects row zero and calls `FUN_140ecdee0`.
- `wave2-save-diffs-300-support-140ec9040.c`: Save calls `FUN_140ecdee0` for the current item, then copies `0x40` bytes from item offset `+0x30` into its snapshot before invoking the spring writer.
- `wave2-save-diffs-300-slot-map.json`: the slot map contains a reference to `FUN_140ecdee0`, not a body.
- `wave2-doe-sim300-dispatch-support-decompile.json`: its captured functions include the Open and Save callers but not either target.

Search of retained analysis captures for both addresses found only those callers/references and prior gap reports. The captured Open code cannot establish the constructor's copy/default/render behavior. The Save caller cannot establish whether selection mutates the `+0x30` payload, refreshes widgets, or leaves it untouched. Therefore the known `0x40` sizes do not authorize modeling the item's contents.

## Exact missing evidence

Need body-level static output for original image RVAs `0xEB2470` (`FUN_140eb2470`) and `0xECDEE0` (`FUN_140ecdee0`), plus any direct callees that read/write the item payload at `+0x30`, widget fields, or source-record offsets. A capture limited to the constructor alone would still leave Save-time current-item behavior unresolved. The smallest useful capture is both function bodies with instruction addresses and direct-call/xref context. This can be acquired later through a read-only PE/disassembly workflow; this lane did not connect to or mutate the shared REA database.

## Candidate implication

Keep the positive-count SPRING guard in place. A reader-only success for one record would not prove which values the UI owns or what Save serializes. No code, tests, build, GUI, or mutable REA state was changed.
