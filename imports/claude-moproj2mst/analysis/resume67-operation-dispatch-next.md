# Checkpoint 67: next public operation-dispatch branch

## Selected branch

Audit the nested member-writer `current` context in
`source/apps/moproj2mst/src/ops/cycle.cpp`, `CycleClass::writeMst`, especially
the `h->current = it` / `writeMst(...)` / `h->current.restore(saved)` sequence
at lines 151–155. This is a concrete difference at a real operation-dispatch
boundary, is reachable through the public full Save route, and does not
depend on the unresolved DOE `global + 0x1c` gate.

## Why this branch is reachable with an actual Session owner

When `M2M_FLAG_WRITE_SAVE` is set, `src/core/convert.cpp` constructs an
`engine::ConversionSession`, stores its address in `ctx.session`, binds its
manager to the Project's I/O, and keeps the Project in that Session's owned
storage. The public route activates Sub_Project 0, calls
`engine::openActiveOperation`, then later enters `mst::writeMainMst` during
the same conversion. `openActiveOperation` resolves the selected item and
calls the registered class's `loadOperation`; the MST main walk dispatches
the active Cycle item's `writeMst` and then its nested member classes' own
`vt+0x280` writers. This is the real full-Save owner chain, not the direct
Project-only corpus helper used by some focused tests.

The retained `51_MO2_Operation_Cycles2` case is a suitable non-DOE branch
fixture. Existing `tests/mdt/test_l7_load.cpp::cycle51_save_writes_no_task_files`
opens it, selects subproject zero and confirms that the selected operation
is `SfOprCycle`. Its cycle is Type 0 and contains Forming2D members. Therefore
this fixture has no Type 2 DoeOutput session document or DBLIST path; the
CP65/66 DBLIST global state is not an input to this branch.

## Source-qualified difference

The native Cycle writer sequence in
`source/docs/re/moproj2mst/s1-04/spec.md` (section 2, `0x14134dcc0`) iterates
the cycle's member IDs and calls each member's class writer with the member
as the explicit `item` argument. The spec records O-3: nested writers are not
given `curItem` by this Cycle loop. Candidate `cycle.cpp` currently snapshots
the member class's `current` binding, assigns the member, calls its writer,
and restores the snapshot on normal return. That assignment is an explicit
adapter rule; the native no-assignment behavior is source-supported.

This establishes the exact code difference, but not the original value of
`curItem` at every nested writer entry. It may have been set by earlier
model/load/UI work, and a first member model may have established it before
the Cycle writer. The Type 0 candidate fixture can test the effect under a
specified initial owner state, but cannot by itself prove the original
runtime's last-current value or every nested writer's dependence on it. Do
not remove the assignment or claim parity solely from O-3 until that entry
state and the member writers' `current` consumers are closed.

## Acceptance contract for the next focused step

Use a disposable copy of `51_MO2_Operation_Cycles2` and require a full
`M2M_FLAG_WRITE_SAVE` Session path. Capture the Project/Session identity,
active subproject and item, Cycle operation ID and count, member IDs in flow
order, then for each nested dispatch record both the explicit `item` argument
and the class `current` value immediately before and during the member
writer. Also record the cycle-mode/iteration values and resulting writer
bytes/status. The assertions must distinguish “writer got member through its
argument” from “class current was rebound to that member.” Preserve the
normal-return-only restoration/failure ordering already represented by the
source; do not turn exceptions into a new unconditional restore policy.

First use this as a candidate-only instrumented contract to establish what
the actual Session path currently does. For original acceptance, the
required observation is the native class-current value at the same nested
writer boundary after a fresh Open and before/through Save, with the actual
Cycle member dispatch sequence. Static O-3 is not that runtime value. The
retained GUI-control lifecycle gate currently reports `release_unverified`,
so this report does not claim a fresh original comparison. Keep public
behavior unchanged until that evidence exists.

## Existing coverage and limits

* `tests/p13/test_p13_writers.cpp` covers Cycle writer output/context with
  writer doubles, not public Session ownership or native current value.
* `tests/mdt/test_l7_load.cpp` covers the real parser/registry Cycle branch
  with a direct `Project` and `Ctx`; it does not construct the full
  `ConversionSession` from `convert.cpp`.
* CP52 `test_current_operation_binding.cpp` covers the general binding and
  restoration mechanics, including temporary bindings. It preserves the
  Cycle override as an adapter rule and does not qualify native Cycle
  selection.
* The historical Cycle fixture comparison and byte-identical writer tests
  are candidate/retained-output evidence; they are not fresh original
  Open→Save current-state observations.

This is one source-backed next dispatch target, not a claim that Cycle is the
only remaining operation gap or that the converter's full operation/property
surface is closed.

## Evidence hashes

| Evidence | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/ops/cycle.cpp` | `1bb6772156e94cbe33cd14a99045711c874e087db846d466b89ea79266371df1` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `f333209669d87df730849bf2d7962b052a3d9c245ed3c328f268b1567cbd2555` |
| `source/apps/moproj2mst/src/engine/loader.cpp` | `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c` |
| `source/apps/moproj2mst/src/mst/master.cpp` | `7ea30c5d6bc8f335fcb65e30eac285920f2d19c15802e3ea9351ec59af21edee` |
| `source/docs/re/moproj2mst/s1-04/spec.md` | `2e1f9f7b75f49b4e3bcec811755c4c0666e217ccba8864cf2128e58268b93790` |
| `source/apps/moproj2mst/tests/p13/test_p13_writers.cpp` | `33be40ffff0abbddceafbe6928cbb03b6015b421c63614424cd0c557c8ce51fd` |
| `source/apps/moproj2mst/tests/mdt/test_l7_load.cpp` | `d67dbdda0037c6cd5dee1ce89ea24f95771703036675dc329494d1afb472f17a` |
| `analysis/resume52-current-binding-implementation.md` | `1e64c77891bcef8d388eec1086063c87b76517cb52caef1e9dbd1c92c7a6046a` |

Read-only source/receipt audit. No source, build, tests, GUI, project fixture,
or mutable REA database was changed or run.
