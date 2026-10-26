# CP189 R300 `saveMaterials` plan review

**Block the proposed source/test edit as already implemented; redirect the raw-step target.** Current `save_current.cpp` and `test_save_current.cpp` match CP188's released SHA-256 values (`2031229bdecbca9e30a90aae8c4dcde16cf455118fbc73403027e6a5df75eb4f` and `37b8fa328c8605c31faff5559aa4ff8f7955f1431afbe7a1bfb8adc77a50b552`). The CP189 note's `saveMaterials` change is already present, including the required repeated lookups and tests. The note's reference to a final `saveWholeModelKey` call is outside `saveMaterials` and is not inside a callback.

## Reconciliation

CP188's released change is confined to `contactFlagsFromEngine`; its implementation report explicitly releases the same two files after lexical scoping of the contact step borrow. The live hashes match that report. This review does not establish root's integrated build, regression, or freeze receipts.

In the current `saveMaterials` body (lines 643–668), count, nth-material eligibility, and writer each use a distinct `withOpStep` call. The number and eligibility result are copied out of the traversal callback; path derivation occurs between traversal and writer lookup. The writer callback retains its in-place `if (step)` guard and ignores `saveMaterialKey`'s status. No step or material alias escapes the synchronous callbacks. Do not collapse, hoist, or alter these accesses.

The reported raw `m->step` use is instead in `syncOperationFiles` (line 746 for equipment, and the `saveWholeModelKey(p, *m->step, ...)` call at line 751 for restart output). That whole-model call follows `saveMaterials`; it is not its final callback. CP189's stated scope therefore cannot close that raw borrow. The broader closure audit is the appropriate place to select any subsequent cut.

## Existing test coverage and fixture

The assigned test file already contains more than the proposed extension:

- `save_materials_step_scope` uses a copied R46 operation-2 project, restores synthetic material-list links/counts with RAII, checks ordered nonempty material KEY writes and type-2 exclusion, and retains the detached-step zero-effects probe.
- `MaterialAccessorRig` and the repeated-accessor cases use a scripted `OpClass::classModel` seam while keeping both model owners alive. They assert the count/nth/writer lookup sequence and distinguish traversal from writer payload, preserve the initial count when a later model has a larger valid list, and prove continuation when a writer-time number is absent.
- `save_materials_initial_null_step_has_one_count_lookup_and_no_writes` checks the qualified present-model/null-step count result. The existing detached-step case restores ownership; do not invoke a private writer/helper on a detached owner or introduce null-model fault emulation.

These are reconstructed helper-mechanics tests, not public Save reachability or native-byte equivalence. Static inspection remains necessary to establish no borrowed alias escapes. The synthetic accessor tests explicitly keep both owners live, so they do not establish safety under destruction/reentry.

## Native basis and disposition

The retained CP185 source review qualifies the native routine as a runtime-unwind assembly region, not an executed trace or whole logical procedure. It identifies the count-time access, inclusive one-based traversal with per-position model/step reacquisition, number copy before path derivation, writer-time reacquisition, and ignored writer return. It also reconciles the count accessor body: a present wrapper with a null count-time step yields zero. This supports the current repeated-access structure and detached-step outcome; it does not justify changing the already-present implementation.

**Smallest correct action for this review:** no source or test edit. Mark the `saveMaterials` proposal satisfied by the current CP188 source/tests. If the intended next target is the remaining direct `*m->step` whole-model save, revise the next-step plan to name the owning function/call and its separately qualified caller/owner guard before assigning an edit; do not attribute it to `saveMaterials` or broaden this test as if it observes that call. No build, test, GUI/app-control, original-process, or REA operation was performed.
