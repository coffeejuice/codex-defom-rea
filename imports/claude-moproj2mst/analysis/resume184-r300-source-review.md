# CP184 R300 independent source review

**PASS for the local source cut; no source blockers found.** Focused execution and root source-stable validation remain required. This review ran no build, test, executable, GUI, or REA database operation.

Reviewed `resume184-r300-next.md`, `resume184-r300-fixture-resolution.md`, `resume184-r300-material-fixture.md`, `resume184-r300-implementation.md`, and the current production/test sources. Current SHA-256 matches the implementation release:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `36B3B12693FD5C2B3D4F4BDE9EC486A9B673F7575DA4B6D94903DE19CEB0CF8B`
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `AEC6E922352E31123545270266576F7C9C1E9CD16E9CDF7AE30290B4FB530C37`

## Production cut

`saveMaterials` (lines 639–654) has exactly one `opModel(p, op)` lookup inside an immediately invoked, implicitly void callback. Missing model/step returns before the count read or effects. The count remains the signed 32-bit read at `0x1320`; traversal remains ascending `1..n`; null materials and type 2 at `0x1cc8` remain excluded. Material number at offset zero, `materialDefaultFile`, and synchronous `saveMaterialKey(p, *step, no, path, fx)` are unchanged. The callback exports no pointer or return value. No admission or dispatch behavior was added.

`saveMaterialKey` and `writeTaskKey` synchronously invoke their writer callbacks. The reviewed capture path stores only target path and copied bytes in `FileEffects`, not borrowed step/material pointers. This is reconstructed-source reasoning, retaining the plan's existing native-address qualification; it is not a new original-runtime observation.

## Fixture safety and assertions

`save_materials_step_scope` (lines 500–670) explicitly loads operation 2 and checks class/model/step. It inventories the loaded list before deciding whether an eligible synthetic node is needed; it always adds a uniquely numbered type-2 node. Integer bounds protect both new numbers and list-count increments.

The append guard is noncopyable. Both allocations complete before any original link/count is changed. The constructor's post-allocation mutations do not throw. Material construction initializes `next` to null and the default type to zero; the added nodes retain the real step owner. The guard restores original head, tail, terminal link, list count, and step count before its `unique_ptr` members destroy the synthetic nodes. Its terminal-link restoration to null is justified by the explicit precondition on the original tail. `Material::~Material` does not recursively delete `next`, so member destruction does not double-delete the synthetic chain. Original index arrays remain untouched. The helper's reviewed nth/find and writer lookup paths traverse links rather than these arrays.

`DetachedStep` restores ownership during ordinary exit and explicit exception unwinding. The test checks model/step identity and seeded counts afterward; final checks verify original list restoration after the append guard ends. The append guard itself is not explicitly exception-probed, but its restoration is unconditional and performs only nonthrowing assignments. This is not a blocker.

`FileEffects::writes` is a vector, and `captureKey` appends synchronously, so assertions observe write order as well as exact paths and count. The fixture-resolution note's earlier map characterization is incorrect and is explicitly corrected in the implementation note. The test checks nonempty material KEY bytes/header, excluded paths absent, and no deletes. The synthetic exclusion number cannot collide with a loaded material number. Missing-step execution uses fresh effects and checks no writes/deletes without dereferencing the detached model step.

## Evidence limits

This is helper mechanics coverage. It does not establish public Save dispatch/admission, byte-for-byte native equivalence, null-slot execution, or closure of other Save owners. Header/nonempty-byte checks do not validate complete material payload correctness, and the test does not exercise the native-task-key branch; neither is required for this unchanged-writer borrow cut. The loaded list invariants are fixture assumptions checked sufficiently for this scoped append, not a general malformed-list validator. Root must confirm that the new case actually executes and passes before reporting runtime coverage.
