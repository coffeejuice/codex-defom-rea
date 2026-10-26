# CP222 first-step identity source review

## Decision

PASS for the bounded, observation-only first batch in `resume222-editor-closure-architecture.md`, subject to root-owned build/test validation. No source fix is required by this review. This is not ownership closure, admission authority, or permission to publish R300 cache1. No build or test was run by this reviewer.

## Reviewed behavior

- `StepIdentity` in `source/apps/moproj2mst/include/m2m/engine.hpp:80` has a public inert default constructor and a private owner/generation constructor. Its weak owner and generation are private. Ordinary callers can copy issued observations but cannot mint a bound token through the supported API. There is no pointer/address conversion, native reference, getter, or token resolver. This is ordinary C++ API nonforgeability, not protection against undefined behavior or deliberately redefining an incomplete implementation type.
- `gui_model.cpp:34-60` keeps only a weak control block in a token. Equality uses control-block ownership ordering plus generation, including after owner expiration. A live model at a recycled address gets a fresh control block; the old token keeps its old weak ownership identity. Currentness locks the metadata block and never dereferences the model pointer. The token therefore does not keep the model alive.
- `GuiModel::reset` retains the preexisting geometry rejection and null-step no-op first. For a present step it checks exhaustion, then advances generation before `viewerClear`, mirror-plane clear, `step_clear`, GUI initialization, DOE table reset, and `state100` reset. Existing native effect order is unchanged. The increment is not rolled back if a later effect throws. The max-generation check prevents wrap and rejects before the first destructive effect. It introduces a new exhaustion-only error, consistent with the architecture's overflow rule; no claim that every possible reset has identical error behavior should be made.
- `GuiModel::~GuiModel` retires observations at body entry before auxiliary/display cleanup. It does not reorder those existing calls, alter the native aggregate, clear the model guard early, or change lifetime member declaration order. Guard behavior during unrecovered reentrant destruction remains outside the existing documented contract. Token currentness is deliberately stricter during cleanup than guard lifetime.
- The header explicitly limits invalidation to constructor/reset/destruction and says direct public replacement and destructive loaders bypass it. The legacy reset test confirms that a token can remain current after `model.step.reset()`. Therefore currentness means current instrumented observation generation, not proof of a live native step. No consumer in the reviewed batch uses it as access/publication authority.
- The reviewed patch does not edit `include/moproj2mst.h`, C API exports/layout/version/error mapping, or the R300 implementation. It adds C++ methods/token type and private lifetime metadata; it does not establish a promise of C++ binary ABI compatibility. CP163's cache1 refusal and separate original-witness gate remain controlling.

## Actual test strength and limits

`tests/mdt/test_step_identity.cpp` uses Testkit checks rather than Release-disabled assertions. Its compile-time checks reject the exact bound constructor and implicit pointer/address conversions. Runtime cases cover default/foreign/current equality, unchanged native address across repeated reset, selected reset state defaults, owner expiration without lifetime extension, guaranteed same `optional<GuiModel>` storage reuse, and explicit legacy public-step bypass/null-reset behavior. The existing CMake `CONFIGURE_DEPENDS` test glob should register the new `test_*.cpp`; root must confirm actual registration and execution.

These tests do not dynamically reach generation exhaustion, observe currentness inside destructor cleanup, inject a failure after reset invalidation, test explicit casts/member-address bypass attempts, or prove complete reset/native effect order. Exhaustion and effect ordering here are source-reviewed facts only. No production-only generation setter is warranted just to reach overflow. If future changes make overflow arithmetic or reset cleanup more complex, add a narrow private/test seam or pure transition helper and test max-minus-one/max without exposing mutation through the public token API.

Later loader/replacement/object/key/admission migrations need their own tests; the present cases cannot qualify those routes. Likewise this review does not certify the public legacy step, raw getters, callback captures, shared operation cache, or parked admission as closed.

## Scope

Read the current engine header, model implementation, new identity test, architecture report, and existing seam qualifications. Wrote only this review. No source, build, executable, original process/project, GUI/app-control session, shared recovery ledger, or deployment was changed. Root owns candidate build, full launcher, and source/executable freeze receipts.
