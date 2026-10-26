# CP227 editor title command implementation

## Implemented change

Added `GuiModel::copyEditorTitleIfControlMatches` in `source/apps/moproj2mst/include/m2m/engine.hpp` and `src/engine/gui_model.cpp`. The command rejects input longer than the native 80-byte title capacity or bytes outside printable ASCII before consulting model state. It then calls `requireGeometrySyncAvailable()` exactly once, resolves the current nullable control, compares its address to the supplied expected pointer as the legacy identity token, and copies the bytes plus one terminating NUL only on a match. The expected pointer is not dereferenced or claimed to be an opaque/final identity.

`StagedEditor300::titleChanged` still performs its binding/page/event checks, 80-character and printable-ASCII validation, and `qt::toAscii` conversion before the geometry barrier. It delegates the final current-control lookup, identity check, and copy to the named model command. A barrier exception therefore remains at the same point as the old `simCtl()` call: after conversion and before null/replacement rejection or title mutation. False maps to `UnsupportedSnapshot`; true maps to `Completed`.

The C ABI, `simcontrol300.cpp`, Stopping predicates, cache1 storage/publication, and widget construction/order were not changed. No control pointer is returned from the new command.

## Tests and static preflight

Extended `tests/mdt/test_simcontrol300_title_changed.cpp` with a direct model-command test for a matching control and title copy, null expected identity, over-capacity input, non-ASCII input, and no mutation on refusal. Existing title tests already exercise missing-step and replaced-control refusal, preserve the old/replacement titles, and exercise successful editor delivery.

The test follows the file's Testkit entry point and uses `CHECK`, `CHECK_EQ`, and `REQUIRE`; a scan found no C `assert` and no unsupported `REQUIRE_EQ`. `git diff --check` passed. No build or CTest run was performed. The title test fixture does not expose a supported way to install a parked geometry barrier, so barrier refusal is not asserted in this focused file; root should include the existing geometry-barrier and cached-Stopping regressions in integration validation.

## Source state and limitations

Only these four authorized source/test files were modified for CP227. The shared header and `gui_model.cpp` also contain CP226's already-frozen `setDimension` and CP225's `lastObjectNumber` changes in the current checkout; those existing hunks were preserved and are not part of CP227. The working tree is uncommitted and source-stable for root's focused build/CTest integration; no full-suite run or freeze is claimed.

Current SHA-256 values:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `8AEC169587DF42D3AD2BD5617EDB6BACE793FC4CAFD6C5DE1554099645AD75A5`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `28C842A0F6DCDE166C5DA129F5A8A095D99F6AD398F889A0A0B4687126ABBE82`
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: `D7D865527B8252AD1C4BD6D272F51D55002FBDBCFAAB5FDCBDEDE958FF2A534F`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_title_changed.cpp`: `74E3289EDADE355DFEE8F7BEBB29B2A4A07742134EABC074297ADCE69B19CE1A`

