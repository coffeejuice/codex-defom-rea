# CP185 R300 repeated accessor source review

**PASS for the bounded source change; no blocking defect found.** The current implementation and synthetic fixtures satisfy the approved repeated-accessor plan. This is a static source review, not a build/test receipt or native public Save equivalence result.

Reviewed `resume185-r300-native-accessor-plan.md`, `resume185-r300-accessor-plan-review.md`, and `resume185-r300-accessor-implementation.md` against the current implementation and tests. Current SHA-256 values match the implementation report:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `dfc90650f638389644301fca786b447a581f75ac749de444653eb0b8e5bedd54`.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `4bebfa62f75f939d3940f2da3627269ac11e53f475239b1944a7fa9995a62acf`.

## Source acceptance

`saveMaterials` at lines 639–664 makes exactly one count-time `withOpStep` call, one traversal call for each saved position, and one writer call for each eligible traversal result. `engine_internal.hpp:68–74` resolves `opModel` exactly once per invocation; `loader.cpp:168–180` consults `classModel` on every such resolution. This establishes `1 + n + eligible` model lookups for a completed positive-count loop, excluding model-holder accesses inside the original native count wrapper. Nonpositive initial count makes only the initial lookup.

The signed count is copied once before the loop. Each iteration copies the number and eligibility while the traversal borrow is local; null material and type 2 skip path generation and writer resolution. The copied number feeds `materialDefaultFile` before the writer model is reacquired. The writer receives its freshly resolved step and the copied number. There is no traversal pointer captured across either operation. The existing writer result remains ignored, so a returned failure does not stop the loop.

The initial present-model/null-step case leaves count zero and performs no material effects. This agrees with the plan's retained count-body qualification. Initial null-wrapper and later-null-step guards remain explicitly qualified adapter divergences. The implementation does not assert native fault equivalence for them.

`saveMaterialKey:621–634` resolves the number before entering `writeTaskKey` in native mode. Missing number returns 3 before the callback, file open or overlay erasure. `writeTaskKey:92–110` confirms this order and the continuation case tests it. No writer-time type filter was introduced.

## Fixture acceptance

The five direct-helper cases at `test_save_current.cpp:722–831` distinguish the relevant mechanics:

- Stable four-position list: exact eight model calls, ordered numbers 7/7/9, three nonempty KEY effects and type-2 exclusion. Duplicate number lookup intentionally serializes the first matching number twice, as the existing number-based writer does.
- Initial detached step: one lookup, no effects, and retained material ownership. The earlier loaded-operation detached-step case remains present.
- A traversal/B writer: three calls and B's distinguishable payload at A's selected number. B's type 2 also verifies that filtering belongs to traversal.
- A count/list of two and B count/list of three: five calls and only two paths, with B's second payload/number 42. This follows the plan review's refinement and proves both saved-count traversal and nth reacquisition without inconsistent count metadata.
- Writer B missing A's selected first number: unchanged on-disk sentinel and overlay, only the later callback, later B payload, and five calls. This establishes continuation after the ignored missing-number status.

`MaterialAccessorClass` owns both models for each whole case and returns them through the existing virtual seam. The fixture never destroys an active writer's model/step. `material_list_add` constructs owned nodes and links them; `add` synchronizes `step.n_materials` with list count, so the count-snapshot fixture is structurally valid. Scratch destinations are checked through the existing `scratchOutput` helper. Includes resolve to existing sources, supported Testkit assertions are used, and the Testkit main remains present. No test-only production hook was added.

## Limits and integration

Pointer scope prevents a traversal borrow escaping into path generation or a later model resolution. It does not guarantee writer lifetime if a callback destroys the active owner; both the plan and implementation report correctly retain that limitation. These synthetic fixtures do not establish public C-ABI replacement reachability or original application byte parity. The count accessor's native one-or-two holder reads are not reproduced as separate observable model calls, and no claim otherwise is warranted.

Packet S and L0 source qualifications were read: retained annotated decompiles, static headless analysis, and assembly precedence over argument-drop artifacts. The plan's native assembly qualification remains a runtime-unwind region containing the relevant sequence, not a whole logical procedure or executed trace. This review validates conformance to that qualified approved plan; it adds no runtime evidence.

Only this report was written. No source changes, build/test execution, GUI control, original execution or REA database operations were performed. Root still owns focused validation, grouped/full gates and source freeze before candidate acceptance.
