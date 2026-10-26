# CP182 staged R300 default contact-pair step closure

Implemented only the owned production/test pair under `source/apps/moproj2mst`; no build, runtime test, freeze, GUI, mutable REA session, deployment or shared-state mutation was performed. This is a source implementation receipt, not a passed checkpoint.

## Production change

`src/engine/save_l3.cpp::feDefaultPairs` now immediately invokes a synchronous void callback whose nullable `DB_STEP_DATA*` argument is acquired at the former early-return point. The empty vector is constructed before the single `opModel(p, op)` lookup. Null returns from the callback and then returns the same empty vector from the function. For a step, the callback clears `inter_obj` before querying object count, iterates objects 1 through n, skips null/kind-7 objects and skips object 1 in dimension 3, then constructs a fresh value `ContactPair` with master i/slave 1/lub 0/default f19. Friction precedes IHTC for each pair, followed by push_back. The step and object pointers remain inside the synchronous callback; returned ContactPair members are integers, doubles and bytes.

No withOpStep, GuiModelAccess::withStep, geometry guard, admission change, exported header change, C ABI change, CP163 cache0/A/after-A change or cache1 change was introduced. Public GuiModel::step and the other outstanding raw-access surfaces remain open. This is not an aggregate parked-model barrier.

## Focused coverage added (execution pending)

`tests/mdt/test_engine_l3.cpp` adds `fe_default_pairs_step_scope` using the admitted one-object 2D `air_convect` corpus operation. It requires a real eligible object, step and common props; seeds a real nonempty inter-object list; checks the single pair including friction, IHTC and flags; and requires the list to be cleared. It then seeds a real nonempty list again, temporarily moves the actual step out, requires an empty result and unchanged detached list count, restores on normal/exceptional exits, and checks model/step/object identity and absent FileEffects.

The existing `heat_resting_2d` case now requires both real 2D objects to be eligible and checks two returned pairs in master order 1, 2 with slave 1, lubricant/default flags and property values, plus cleared list and absent FileEffects. These corpus eligibility requirements have not yet been executed against the fixture.

These are direct internal helper mechanics tests on HeatTransferExpress operations. `classDefaultPairs` dispatches this helper for `SfOprFormingExpress`/`SfOprFormingExpress2D` (save_current.cpp), so they do not establish reached FormingExpress public Save behavior or public null-step Save admission. Existing Save/Express routes still need their required source-stable suites.

## Static audit

A bounded Python audit passed checks for exactly one model lookup, immediate nullable void callback, null check before clear, clear before objectCount, ascending iteration, identical object filters, friction-before-IHTC-before-push order, no added access/geometry guard, and a single value-pair push. Manual inspection confirmed unchanged pair fields and callback-local step/object borrows. `git diff --check` printed no diagnostics, but these imported files are not represented in the parent checkout diff, so that command is not an authoritative patch review. Exact file contents and SHA-256 hashes below identify this candidate.

| File | Before SHA-256 | Candidate SHA-256 |
| --- | --- | --- |
| src/engine/save_l3.cpp | 1a7c6de08abcb97906518549dbdaf0dc1568d444c2167dc70eb69a2f1f1befe0 | 888aa2719cdad44281e6b89159422ad87c5196f64cad4b51313a85f3035908e9 |
| tests/mdt/test_engine_l3.cpp | a9a151d214960df4bfeeb4153ce673651642c3553b3290dfe8cbba0086f8931b | e3bc5449fbb610e5477636f89dc5cefc46a1891d64284853c8175e1edd0b36cf |

Verified unchanged exported `include/moproj2mst.h`: `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`. Verified unchanged `src/engine/simcontrol300.cpp`: `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`.

## Required owner continuation and evidence limit

Build these exact hashes and run `m2m.mdt.engine_l3`, `m2m.s.save_current`, `m2m.mdt.save_l2`, `m2m.mdt.simcontrol300_stopping_cached`, `m2m.g0.abi`; confirm the new assertions execute. Then complete the full source-stable suite and freeze per the next-cut plan. No successful compile or runtime assertion result is claimed here.

Scope and qualifications follow `analysis/resume182-r300-next-cut.md`. The original evidence identified there remains DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this cut adds no same-occurrence reader-tail claim and does not authorize zero-length Stopping cache1. No reconstructed application instance was executed or deployed in this task.

Exclusive ownership of the two implementation files is released with this receipt.

## Compile correction after owner focused build

The owner focused build exposed four incorrect `engine::` qualifications in the new test expectations. The production helper declarations and definitions are in `namespace m2m` (`src/ops/express.hpp` and `.cpp`). Only those four calls in the test file were corrected to explicit `m2m::propsFriction`/`m2m::propsIhtc`; test behavior and production source are unchanged. Bounded static checks confirmed both declarations in the included header, two calls to each correctly qualified helper, and absence of the incorrect qualifications. No rebuild or runtime test was run by this worker; the owner must rerun the focused build/tests. The candidate test hash in the table above reflects this correction. File ownership is released again.
