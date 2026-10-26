# CP108 SPRING positive-count owner audit

## Decision

Keep positive `SPRING.INI` counts behind the existing unsupported guard. The retained evidence closes file parsing and the later writer, but not the first public state transition that gives a parsed record its UI/item ownership. There is no source-closed positive-count Open→Save slice yet.

## Recovered chain and the missing edge

`analysis/wave2-save-diffs-300-support-140ec8110.c` qualifies the Open sequence: clear the spring list, call `FUN_1410dab40`, allocate one `0x78`-byte item for each returned `0x40`-byte record, pass it to `FUN_140eb2470`, set the checkbox from list count, select row zero, then call `FUN_140ecdee0`. `analysis/wave2-save-diffs-300-support-140ec9040.c` qualifies Save: gate on the spring checkbox/count, invoke the same selection helper for the current item, copy `0x40` bytes from item offset `+0x30`, and pass the records to `FUN_1410db030`. The writer body is retained in `analysis/wave2-save-diffs-300-support-1410db030.c`.

The first unclosed edge is `FUN_140eb2470` (RVA `0xEB2470`): its body is absent, so we cannot establish which fields from the parsed record are copied into item payload at `+0x30`, which are only rendered, or how defaults are applied. `FUN_140ecdee0` (RVA `0xECDEE0`) is also absent; its behavior on initial row selection and Save-time selection is unknown. Caller order and the later fixed-size copy do not establish either transformation.

The current candidate stops in `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp` when `readSprings()` produces a nonempty list. The native reader in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` intentionally rejects positive counts after the separately qualified count-first boundary. Removing either guard would admit data into a state model whose ownership and Save readback are not recovered.

## Minimal dependency and safe next step

Recover body-level disassembly/decompilation for both original functions from the retained original image (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), including direct callees and accesses around item offset `+0x30`, widget members, and record fields. Then a single positive record can be modeled through row-zero selection and Save. Until that evidence exists, a reader-only positive-count test cannot qualify public behavior, and a guessed record-to-item copy would risk changing which values Save emits.

The existing candidate state is therefore the correct bounded outcome for this lane: count-first parse facts may be retained, but no item creation, selection emulation, or positive-count support should be added from caller evidence alone. No source, tests, build, GUI, or mutable REA state was changed.
