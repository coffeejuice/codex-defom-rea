# CP240 loader next reachable leaf

## Candidate

Migrate the two live `def::pre::step_count_objects(m->step.get())` calls in `source/apps/moproj2mst/src/engine/loader_l3.cpp` to a named owner query such as `GuiModel::countNonType7Objects()`. This is a value query, distinct from `objectCount()` (which counts all object entries) and from CP237's field writes. The function counts non-null objects whose native kind is not 7; do not substitute `objectCount()`.

Callers are `expressBasePostLoad` near line 414, where the count gates the existing unsupported merge refusal, and `hteObjectTreeAdd` near line 456, where the count gates the existing per-slot unsupported deletion refusal. In both places replace only the native call expression. Preserve the `merge`/loop short-circuit conditions, refusal text/order, `objectCount()` loop bound, and every return/throw boundary.

## Guard and null behavior

`source/port/def_pre/src/menu56_common.cpp::step_count_objects` is a direct native helper: it reads step object count at `+0x1378`, loops positions 1..count, retrieves each object, excludes null and kind-7 objects, and returns the tally. It has no null-step handling. Current L3 callers do not explicitly check `m->step` before invoking it (the containing functions check only `m`); the owner query must not add geometry admission, synthesize zero for absent step, or convert this precondition into a new safe return. Its contract should be explicitly limited to the existing live owner step, preserving direct helper behavior. This is an intentionally unguarded query, not a general public model accessor; prefer internal visibility if available.

## Tests and evidence

No existing test directly exercises either caller/refusal: `rg` finds no `hteObjectTreeAdd` or non-type-7 count case in `tests/mdt`. Add a direct owner-query test to `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp` (or its dedicated Testkit target): build a model with existing `llobj_ctor::get_or_create` fixtures, arrange a mix of ordinary and kind-7 objects plus a missing object number, and compare the command result to the existing `def::pre::step_count_objects(model.step.get())` reference. Assert that `objectCount()` remains the total slot count while the new result excludes null and kind 7. No null-step runtime test; the existing helper itself does not define that case. This test validates the value query only. Since the retained corpus lacks a FormingExpress case, caller dispatch and refusal order remain source-reviewed, not claimed as covered by the test.

Run the registered owner/L3 Testkit target after confirming its configured name; prior reviews indicate `m2m.mdt.engine_l3` for `test_engine_l3.cpp`. No C ABI changes.

Original qualification: `loader_l3.cpp` identifies the native routine as original `0x140c7c1e0 = 0x1401cab30`; `source/port/def_pre/include/def_pre/menu56.hpp` and `source/port/def_pre/src/menu56_common.cpp` preserve that routine and its non-kind-7 loop. The L3 ledger records the caller behaviors as recovered/ported. This is decompile/source-backed evidence, not a fresh original execution or accepted FormingExpress corpus occurrence.

No source, tests, build, shared state, original, GUI, or REA artifacts were changed; no tests were run.
