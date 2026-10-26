# CP102: SPRING item and selection body evidence check

## Finding

The reviewed retained artifacts still do not contain the bodies for original functions 0x140eb2470 (spring QListWidget item construction) or 0x140ecdee0 (current-item/selection handling). They contain their call sites and surrounding Open/Save control flow only. No source-qualified positive-count public increment is ready; retain the candidate guard.

This reconciles CP95/CP98: those reports identify the remaining dependency accurately. CP102 found no new retained body capture that closes it.

## Evidence present

- analysis/wave2-save-diffs-300-support-140ec8110.c shows Open clearing the spring list, calling FUN_1410dab40, allocating a 0x78 item object per returned 0x40-byte record, calling FUN_140eb2470, setting the checkbox from list count, selecting row 0, and calling FUN_140ecdee0.
- analysis/wave2-save-diffs-300-support-140ec9040.c shows Save's checkbox/count gate, current-item call to FUN_140ecdee0, copying 0x40 bytes from each QListWidgetItem at +0x30, and passing the snapshot to FUN_1410db030.
- analysis/wave2-save-diffs-300-support-1410db030.c qualifies the subsequent record field formatting and SPRING.INI / SPRING2.INI write/delete decision.
- analysis/wave2-save-diffs-300-slot-map.json has a dispatch/direct-call reference to 0x140ecdee0, but no function body.
- analysis/wave2-doe-sim300-dispatch-support-decompile.json decompiles FUN_140ec9040 and adjacent dispatch support only; its function inventory does not include either missing target.
- Searches of retained .c/.asm/.md/.json captures for both addresses found only the CP95/CP98 analyses, their Open/Save caller decompiles, and the slot-map reference. No item-constructor or selection-callback body capture was found.

## What is known versus unknown

Known: the saved native record is 0x40 bytes; Open passes a copy of it to the item constructor; Save later reads 0x40 bytes at item offset +0x30; both Open's initial row selection and Save's current-item path call the same selection helper. The writer consumes the resulting records and uses record flags to choose companion-file handling.

Unknown: which fields the item constructor copies into item memory versus rendered text/Qt item roles; how offset +0x30 is initialized; what the selection helper writes to line edits, checkboxes, list index, or the item payload; and whether its initial-selection call synchronously transforms the item in a way Save relies on. Function names and call order do not prove those data transformations.

The missing static dependency is body-level disassembly/decompile for both entry addresses, with direct callees and xrefs to item offsets, widget members, constants, and string references. The intended image is the retained DEF_GUI_MO_64.exe SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. For PE-address tooling, the corresponding RVAs are 0xEB2470 and 0xECDEE0. No capture was generated during this task because the permitted lane is retained-static-evidence review and the shared REA database is in use by close_arch.

After those two bodies are recovered, the smallest useful candidate fixture is one valid SPRING.INI record with a missing SPRING2.INI, followed through Open item creation/row-zero selection and Save output. That would still need original-process comparison before claiming runtime parity. A reader-only count-one parser test is insufficient for public behavior.

## Decision and boundaries

Keep positive SPRING counts unsupported. CP94's exact x-CRLF nonpositive-count mapping is unaffected. Do not relax fillExtra3D's nonempty-spring guard, emulate a guessed item structure, or infer callback semantics from item offset +0x30 alone.

No source edits, builds, tests, GUI/original execution, or mutable REA operations occurred.

