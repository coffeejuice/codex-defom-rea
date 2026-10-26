# CP241 L3 non-kind-7 count implementation review

## Verdict: PASS

`GuiModel::countNonType7Objects()` directly delegates to `def::pre::step_count_objects(step.get())`. The native helper reads the stored slot count, queries positions 1 through that count, and increments only for non-null objects that pass `llobj_not_kind7` (kind not 7). The owner method adds no geometry admission, null fallback, or alternate count semantics. It therefore preserves the native helper's unchecked null-step precondition. This remains distinct from `objectCount()`, which returns the stored total slot count.

Both production substitutions change only the helper expression. In `expressBasePostLoad`, the existing model/property/data preconditions and `merge && count > 1` gate remain, and the unsupported refusal text and position are unchanged. In `hteObjectTreeAdd`, the heat-transfer type check, operation slot-count check, model check, `objectCount()`-bounded loop, per-slot count gate, and refusal text/order remain unchanged. Neither caller adds a `step` guard; the owner query is reached with the same preconditions as the original native helper.

The added test builds four linked objects with kinds 1, 7, 0, and 7, verifies the owner result equals the native helper and is 2 while `objectCount()` remains 4, then unlinks the kind-0 node without changing the stored slot count. It establishes that positional lookup 4 is null, checks the total remains 4, and verifies the non-kind-7 count falls to 1 and still matches the helper. This correctly tests a null list position rather than assuming an object-number gap produces one.

The CP241 focused receipt records the `m2m_mdt_engine_l3` target build and the `m2m.mdt.engine_l3` focused CTest as successful, with stable before/after source fingerprints. The implementation report also records `git diff --check` success. The new test covers the scalar owner query, not the two refusal callers; there is no fresh original execution or accepted FormingExpress corpus case claimed.

No defect found.
