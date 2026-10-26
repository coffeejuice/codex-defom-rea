# CP234 DB-loaded step-number owner command

## Change

`decideAndLoad` now delegates its existing `step->u744 = step` write to
`GuiModel::setLoadedStepNumberAfterAdmission(int)`. The method body contains
only that assignment. It adds no barrier check, null fallback, catch, or other
write. The call remains after the existing `m->simCtl()` admission and after
`sc->nstart = ns`, in the original location relative to `tnow3`, `tnow2`, and
operation start-state writes.

The `oracle_b_124_db_step_load` operation-level test now obtains the loaded
operation model, queries `op_first_step` for reader operation index 4, confirms
the fixture query returns `-4`, and asserts the actual loaded `step->u744`
equals that selected number. This verifies the caller path through
`openActiveOperation`/`decideAndLoad`, in addition to the existing reader tests.

## Focused verification

- CMake Release configuration succeeded.
- Candidate target `m2m_mdt_db_load` built successfully.
- Registered CTest `m2m.mdt.db_load` passed (1/1; 0.76 seconds).
- Source remained stable during the focused target/test boundary; source tree
  SHA-256 was `424b2b5ff1e8212a8697dd09353acf521028fca548a8c6a9e3b291fd910522d6`
  (3,563 files).
- An initial attempt used the wrong target name `m2m_mdt_test_db_load`; CMake
  correctly rejected it as unknown. The corrected target above passed.

The helper's `--then-full` path starts the full project build after the focused
preflight; that broader build was not part of this focused verification and no
full-build result is claimed here.

## Source hashes

SHA-256 at report creation:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `f118d0617fcb31dd04d4d74971bbaaee89a9c334a4e3272cc3fee1d1aec062a7` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `2cbc901fb3283f4c71e600d291f63c7c48ef13405e83038f8210f7d5a60cdabf` |
| `source/apps/moproj2mst/src/engine/loader.cpp` | `c71d2a61ce59bde763dd372b9815adbea70f1c6d8408b0685c91de22caa395e3` |
| `source/apps/moproj2mst/tests/mdt/test_db_load.cpp` | `cb88501b2f00077675f3686843cae9e5b26b934b7aa76e1b83c3bb8dcabe561b` |

## Evidence limits

The implementation follows the source-established DB path: the `simCtl()`
admission precedes the `nstart` mutation, and the new command only performs
the unchecked step-number write. The operation-level fixture establishes that
this path loads step `-4` into the model and the test verifies it. This is not
a fresh original-application capture and does not establish concurrent or
reentrant safety; the command is scoped to the synchronous admitted loader
continuation.
