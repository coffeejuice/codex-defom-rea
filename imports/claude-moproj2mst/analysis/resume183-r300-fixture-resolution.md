# CP183 R300 fixture resolution

## Decision

Resolve the reachability blocker with an explicitly synthetic helper-level test on a real loaded operation. Do not claim that the existing oracle Save cases reach `baseDefaultPairs`, and do not use the synthetic case as public Save or native equivalence evidence.

Keep the reserved test path `source/apps/moproj2mst/tests/s/test_save_current.cpp`. Add `TEST_CASE(base_default_pairs_step_scope)` there. The existing `main` calls `testkit::run_all()`; the CMake tests/<packet>/test_*.cpp registration maps this source to target `m2m_s_save_current` and CTest name `m2m.s.save_current`, linked with `m2m_core` and Testkit. No new test file or registration is required.

## Exact fixture and isolation contract

Use the configured copied corpus path:

```text
<M2M_CORPUS>/46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj
```

Require this fixture to exist and load; do not skip this new case when the fixture is unavailable. Open the project using the same initProject/makeRegistry/openProject/activateSubProject pattern as the existing MDT L3 `openCorpus` helper. Select operation ID 2 with `operationById`. Load its real model using the established `loadOp` sequence in `tests/mdt/test_engine_l3.cpp`: installLoaderSlots, opBase, opModel, reset, assign ob->current, loadOperation. Require every pointer before use. The existing `heat_resting_2d` case documents two loaded objects and step dimension 2 for this exact operation.

Require operation class `SfOprHeatTransferExpress2D`, nonnull props/common, model/step, exactly two objects, and step type at +0x1c equal to 2. Do not silently accept fixture drift. This operation's classDefaultPairs branch currently falls through to baseDefaultPairs, but this new case calls the helper directly and does not test admission or public dispatch.

Make the test predicate explicit rather than guessing the corpus's original object kinds: in this test's private in-memory model, set object 1 ID (+8) to 1 and kind (+0xc) to 1; set object 2 ID to 2 and kind to 0. Neither is kind 5 or 7. This gives exactly two defined objects, exactly one candidate, and selected candidate ID 1. These controlled fields are synthetic inputs, not observations of the source fixture. Record original values and restore them with a scoped guard if they are reused by any subsequent assertions; the project otherwise dies at case exit. Never save these changes to the corpus.

Set the private operation's common friction to 0.375 and common IHTC text to `11.0 N/sec/mm/C`. Record `expectedIhtc = propsIhtc(*op->props)` and require it to be positive/nonzero; assert `propsFriction(*op->props) == 0.375`. This tests scalar propagation and avoids the express zero-IHTC fallback branch, whose mesh dependence would obscure this borrow test. Keep classKey and operation/model linkage intact.

Use the existing internal includes from MDT L3, resolved relative to this test directory: `../../src/engine/engine_internal.hpp`, `../../src/ops/express.hpp`, plus public `m2m/data/common.hpp`, `m2m/props.hpp` and the needed defmodel step/inter_object_ops layouts. `baseDefaultPairs` has external linkage in namespace `m2m::engine` in save_current.cpp; add a test-local declaration `std::vector<ContactPair> baseDefaultPairs(Project&, Operation&);` after including the internal header. The local loader likewise may declare `m2m::mst::installLoaderSlots()` as the existing MDT L3 test does. No production header change, alternate helper implementation, or source-file inclusion is needed.

## Present-step assertions

Save model, step and both object pointer identities. Seed `m->step->inter_obj` with `def::model::pair_append`; require a nonnull result and positive count immediately before the helper call.

Call `engine::baseDefaultPairs(p, *op)` exactly once. Require size 2 before indexing, then assert the full ordered scalar result:

| index | master | slave | lub | f19 | frc | ihtc |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 1 | 0 | 1 | 0.375 | expectedIhtc |
| 1 | 2 | 1 | 0 | 1 | 0.375 | expectedIhtc |

Require attached inter-object count to be zero afterward, and check model/step/object identities unchanged. The included self pair is intentional: step type 2 does not apply the type-3 self-pair exclusion. The vector is scalar-only; the existing inter-object list is cleared, not populated by this helper.

Optionally add a second present-step subcase with object 2 kind 1 (two candidates): reseed a positive list, call the helper, assert empty output and cleared list. Restore kind 0 before the null-step subcase. This is useful rejection-predicate coverage but is not required to resolve the blocker.

## Null-step RAII contract

Reseed a real pair in the attached step and save its positive count. Use a noncopyable test-local guard that owns the moved `std::unique_ptr<engine::DB_STEP_DATA>` and a reference to the original model's step owner:

```cpp
struct DetachedStep {
    std::unique_ptr<engine::DB_STEP_DATA>& owner;
    std::unique_ptr<engine::DB_STEP_DATA> saved;
    explicit DetachedStep(engine::GuiModel& model)
        : owner(model.step), saved(std::move(model.step)) {}
    DetachedStep(const DetachedStep&) = delete;
    DetachedStep& operator=(const DetachedStep&) = delete;
    ~DetachedStep() noexcept { owner = std::move(saved); }
};
```

Create the guard in a nested scope. Require `m->step == nullptr` and `guard.saved.get() == originalStep`; call the direct helper and assert its vector is empty and the detached step's list count is still the saved positive count. On leaving the scope, require the original model identity, exact original step pointer, both object identities and saved positive count. Use CHECK/REQUIRE, never C assert. The guard restores on CHECK continuation, Testkit's REQUIRE Abort exception, and any helper exception; do not rely on a trailing manual move or a catch limited to one exception type.

This scoped test deliberately makes the model step unavailable only for a synchronous internal call. Do not invoke Save, class dispatch, loader, objectAt, model reset, operation switching or any other invariant-dependent operation while detached. Access the detached list only through the guard's owned step. After restoration, check FileEffects has no writes/deletes. This is null-pointer branch mechanics, not a naturally admitted public null-step state.

A small explicit throw/catch inside a separate detach scope can additionally prove the guard restores on exceptional exit; require restored identities and count outside the catch. Never manufacture a failed Testkit assertion to test cleanup, since it would fail the suite.

## Integration gate and bounded claims

The implementer must run the actual new case through `m2m.s.save_current` against the copied corpus; compilation alone does not close this blocker. Root retains grouped integration tests, complete source-stable tests and freeze. This report itself executes no test and establishes no runtime result.

Before implementation, record current hashes of the two reserved files. Production review still requires one opModel lookup, null return before effects, inter_obj clear before objectCount, original loop/filter/selection/read order, no escaped pointer and immediate synchronous callback invocation. `withOpStep` itself performs opModel lookup, so a transformation that also retains an outer opModel lookup must not accidentally perform a second one; choose the callback shape accordingly and inspect the final source.

Source qualification remains the existing SfOprBase vt+0x198 annotation and review of the current reconstruction. This resolution adds no native-runtime observation, public Save equivalence, parked-model barrier, cache1 authority, or same-occurrence reader-tail claim.

