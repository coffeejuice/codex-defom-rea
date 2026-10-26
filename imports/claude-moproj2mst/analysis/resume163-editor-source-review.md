# CP163 editor source review: allocation-only post-query cut

## Decision

**Accept the source cut; block focused-test acceptance until the two timer assertions are corrected.** The reviewed implementation preserves CP161's owned reader, ready/current physical directory receipt, low-version pre-query guard and real four-output query. The only new producer effect follows that query: for the stated profile it calls the existing typed calloc helper, publishes the returned pointer directly to actual `S.u1b78`, and returns `CoordinateSync` before inspecting selected length or publishing cache11. This review is static. It did not build, run tests, control the GUI, run the original, or use live REA.

Reviewed SHA-256: `simcontrol300.cpp` = `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`; `test_simcontrol300_stopping_cached.cpp` = `5410e579e5457a720ee8b17819afbe81bbf37f59c74f609a21cf1f81eba9201b`. These match the implementation receipt. The source judgment applies only to these hashes.

## Blocking test finding

1. `test_simcontrol300_stopping_cached.cpp:340` and `:371` call `globals.timersUnchanged()` after the new fresh-A and guarded-input cases. That helper compares **both** global timers byte-for-byte with snapshots taken before the case (`:59-65`). Every admitted cache0 invocation passes `simcontrol300.cpp:406-409`, which calls node timer `start("Start")` and `checkpoint("ReadSingleAArrayFromDB", nullptr)`. `StepTimer::start` resets the count and `checkpoint` writes labels, subs and clock samples (`source/port/defmodel/src/sdctl/step_timer.cpp:31-49`). Thus the new end-of-case node-timer identity assertion contradicts the intended native prefix. It can fail immediately or pass accidentally if an earlier case left identical count/labels and the clock values happen to coincide. Replace those whole-timer assertions with an arrays-timer byte snapshot and explicit node-timer prefix checks (count 2, labels Start and ReadSingleAArrayFromDB, null subs, no Stop), then let `Globals` restore both timers on destruction. The preexisting CP161 cache0 tests already follow this pattern. This is a test correction; the source timer calls are required.

## Additional acceptance gaps

- The fresh-A case uses `CHECK(step.u1b78)` at `:322`, then dereferences the pointer at `:324`. Use `REQUIRE(step.u1b78)` so an allocation regression reports a failed assertion without a null dereference. Its normal positive-count scenarios should still assert two, three and six zero doubles.
- The new fresh-A case checks getter/dispatch counts but does not retain the existing `Trace` hook to prove dispatch-before-query/producer order. It also omits explicit unchanged `u1b80`, `u1b88`, `u2b24`, `u2b28[11]`, object-coordinate and no-DB-read checks requested by the plan. Add bounded assertions or reuse the CP161 helper's snapshots where the fixture permits. These are coverage gaps, not evidence that the source currently mutates those fields.

## Source assessment and limits

At `simcontrol300.cpp:410-432`, the source takes a strong `Project.dataDb` lease, compares actual handle and step reader identities, checks the current physical receipt and signed index, refuses lower record versions before the query, and calls the actual `step_dir_sizes`. The new `:436-445` guard runs afterward, so a query-admitted profile retains its version effect before any header/count refusal. It requires dimension3, name-state0, version at least6000, positive actual headers, cache11 zero, null A, and a nonnegative `u2b24` no greater than `INT32_MAX-2`. Addition of two remains defined signed32. The published pointer is owned by the actual step; `step_free_contents` frees `u1b78` at `source/port/defmodel/src/step/db_step_data.cpp:194`. No temporary wrapper or rollback is present.

The selected-region length, DB body read, cache11 publication, arrays timer and Stop are not reached in this cut. A retry with nonnull A queries again and refuses, as intended. Source inspection does not establish native CRT heap/new-handler/errno behavior on allocation failure, later reader completion, or public Save/Open reachability. The qualified original remains DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; no reconstructed executable was inspected in this review.

## Repair verification

The subsequent test file hashes to `b5b5df930e42c55298afd81ea87203e7b3f0505958d8ee068303f78b806a9763`, exactly the supplied repair hash. **The blocking timer finding is resolved by static inspection.** The fresh-A case now compares only the arrays timer with its pre-call snapshot (`:323`, `:340`), checks the required two-entry node-timer prefix after the first call and retry (`:341-344`, `:359-361`), and uses `REQUIRE(step.u1b78)` before dereference (`:332`). The guarded-input case likewise compares only the arrays timer and checks the two-entry node prefix (`:386`, `:393-397`). `Globals` still restores both global timers at case exit. The new trace records the real dispatch and confirms its version remains old before the later query effect (`:324-331`); header/MAR/body fields and object coordinates now have explicit unchanged checks (`:316-321`, `:336-348`).

Verdict for this repaired test hash: **accept for root build and focused/full regression**. The absence of DB-body read remains supported by the source stop before that branch, rather than by a direct test-side I/O counter. No source or test file was edited in this verification, and no build or executable test was run here.
