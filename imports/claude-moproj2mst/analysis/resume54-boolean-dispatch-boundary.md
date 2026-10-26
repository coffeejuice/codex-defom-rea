# Boolean trace binder to session dispatch boundary

The frozen binder validates an isolated trace mapping; it does not yet establish admission to a particular ConversionSession or implement the original callback effects. Two concrete integration gaps must stay guarded: owner identity is lost when producing DueObservation, and nested native callback frames are flattened into a list. Default fit/click guards currently prevent a public Boolean timing/parity claim.

This review writes only this report. No source edit, build, C++ test execution, original process, GUI control or mutable REA operation was performed. Candidate-only examples below use explicitly synthetic trace fixtures and make no original readiness claim.

## Current validated scope

The root checkpoint53 validation records the new `m2m.mdt.boolean_trace_due` target with0 of10 cases failed, within its173 registered tests and zero failed tests. That supersedes the worker freeze report's earlier pending-build status for that candidate. It validates the tested isolated binder contracts, not original timer timing or a binder-to-session bridge.

The binder checks its borrowed DiagramLifecycleEvents pointer, expected trace/profile identity, source pump occurrence/thread, completeness/error flags, native registration/lifetime mapping to pending logical IDs, callback evidence and delivery ordering. It produces OriginalTrace DueObservation and does not mutate pending timers. ConversionSession owns a separate private Events member, exposes only its pending view, and consumes due IDs through dispatchAt. Default DiagramEffectDriver methods remain unsupported.

## Concrete owner-binding gap before dispatch admission

`BooleanTraceBindingContext.logicalOwner` is checked against the events object passed into the binder. Current DueObservation contains only PumpPoint, Origin, qualification string and numeric due IDs; it carries no owner identity/lifetime. `ConversionSession::dispatchAt` checks nonempty qualification and matching source site/occurrence, then calls its own private events_.deliver(timer). It does not revalidate the binder's logical owner, trace identity or origin. Numerical timer IDs restart per event owner.

Therefore a binder result for a different standalone Events owner can have IDs equal to the session's pending IDs and reach the session's consume-before-fit stage. This is an internal admission gap, not evidence of an external attacker or a claim that the normal public adapter currently supplies such input. A nonempty qualification string and OriginalTrace tag do not preserve the typed owner proof that existed during binding.

There is also no current public/internal borrowed accessor or session-owned binding method exposing the actual session events_ to the frozen binder; `pending()` alone is insufficient because the binder deliberately checks the owner object. Do not create a replacement scheduler, copy pending records into another Events instance or bypass the pointer check to connect it.

The smallest root-owned bridge should bind against the actual live session event owner and preserve a typed, lifetime-qualified owner admission through the call to dispatchAt, validated before any timer is consumed. A narrow session-owned binding/dispatch method or an owner-bearing validated observation can do this. Exact API choice belongs to root/shared source ownership. Do not substitute an address printed into the qualification string; stale owner-address reuse is a lifetime issue, and parsing prose is not the typed contract.

DeclaredAdapterSchedule remains a separate explicitly qualified testing/adapter profile. It must not silently become OriginalTrace. Any bridge must keep source-occurrence validation and missing-versus-explicit-empty distinction before mutation.

## Nested native frames cannot be admitted from a flat list alone

The binder accepts properly nested callback frames and returns only ordered timer IDs. DueObservation contains no parent frame, nested pump occurrence, fit-entry/return point or callback phase. dispatchAt iterates that list sequentially. Native183160, however, calls fit virtual+388 first, then reads the current attached model/item. Fit's zoom notifications can rebind or destroy that attachment, and a nested pump could deliver another timer inside that interval.

Synthetic counterexample:

```text
native callback A enters
  A fit/notification starts
    nested pump: callback B enters, changes current attachment/item, returns
  A fit returns
  A resolves the changed current attachment/item, then clicks
A returns
```

A flat `[A,B]` replay that runs all of A before B can choose a different target for A. Balanced frames and correct entry order alone do not prove effect-order equivalence. Existing session tests cover consume-before-fit and binding resolution after a synthetic rebind, but do not make the flat binder result a complete native nested-pump schedule.

Before production admission, either restrict this bounded profile to non-overlapping selected delivery frames with qualified absence/coverage of relevant nested pumps, or retain per-delivery parent/pump/phase structure and replay nested observations at the exact driver callback boundary. Merely accepting the native nested trace and flattening it cannot establish parity. If a driver independently replays B during A, the outer due list must not also pretend that B is an independent outer-pump delivery; stale second consumption would return NotPending and misdescribe the native schedule.

No source change is made here; the binder's current nested-acceptance test is an isolated qualification test and now has this integration limitation explicitly attached.

## Exact unbound native effect fields and branches

| Boundary | Established from source | Still required before an actual public effect |
| --- | --- | --- |
| Native readiness/delivery | Each nonnull diagram170430 attachment requests a distinct1000ms timer; delivery consumes it before callback. | Actual timer object/generation/registration ID, receiver/thread/dispatcher, delivered occurrence and native pump context. Request interval, elapsed tool time, HWND wake success or final Save bytes do not establish readiness. |
| Startup posting/wake | Queue append/post/count/canWait/unlock/wake order and Core67141b70 branch are source-bound. | Actual same-process HWND test/reload, serial and CAS/postedWake inputs/outcome; real posting continuation. Queue-removal helper is not wake or delivery. |
| Fit183160+388 ->174120 | Fit precedes model inspection. Selected body queries the view behind this+28/private+8, has a rectangle/predicate branch, consumes private+128/+12c dwords on the fallback path, updates geometry/transform-derived state, writes private+54 and emits18d2c0/18d310 zoom notifications. | Actual dynamic virtual target/view/private owners; initialized rectangle/predicate/offset values; Qt geometry/transform results; private+54 state; actual notification recipients/arguments/order and any reentrant binding/pump effects. Offsets are source-consumed fields, not invented width/height/zero values. A recorded return or a ShellDriver log does not execute this work. |
| Current model/item after fit |183187..1831c1 reads this+28/private+18 model and model virtual+100 current item; null model/item exits after fit. | Source-qualified mapping of the actual then-current native model/item to the same live session SubProject/FlowItem. Registration-time snapshots or only the pre-fit active index are insufficient. |
| Diagram click dispatch | modelUpdated emits signalDiagItemClicked, then UpdateMainView, through recovered notifier registrations. | Complete admitted recipient population/order, actual main-window receiver/thread, project/loading guard and intervening callbacks. Fixed registration scans do not prove absence of dynamic extra receivers. |
| Selected Boolean leaf | Active Type0 subproject, same-current Boolean class, loaded engine/step, shown Object editor index!=0 supports old apply -> Leave -> factory/new fill. | Actual operation/project/class/current pointers and ID agreement, editor index/lifetime, data/model/step and current object. Changed-operation activation, unshown/index0 editor, Type2/3 and other tree-item kinds remain separate branches. |
| Surrounding notifications/effects | Retained reselection contract identifies DOE refresh, CurObjNoChanged, tree checks and widget show order. | Actual admitted UI/loading/action/title/checkbox/visibility and display-kind state/effects, plus any reachable recipient reentry. Calling applyBooleanSelection alone intentionally omits the full click/UpdateMainView chain. |

The default fit guard must occur even for a null-bound or released project: the native function fits first. Only after a qualified fit returns may null binding skip selection. The existing session code preserves this order. A future effect implementation must not replace the null-model branch with an early pre-fit no-op.

## One candidate-only regression to add now

Proposed regression: **an observation bound to another events owner must be rejected before session timer consumption**. This is a source-predicted failing integration contract on current code, not a test run or an established external bug.

1. In an isolated candidate test create ConversionSession with the default unavailable DiagramEffectDriver, one owned test subproject and a committed nonnull diagram binding; retain its pending timer ID and pending count. Use no fixture files or original process.
2. Create a separate standalone DiagramLifecycleEvents whose first receiver/timer IDs intentionally collide numerically with the session IDs. Bind an explicitly synthetic complete native trace to that foreign owner using the actual frozen binder; assert its OriginalTrace observation is structurally accepted for that owner and the foreign timer is still pending.
3. Attempt to submit that observation at the same PumpPoint to the session. Required admission behavior: reject the owner mismatch before fit and before removing any session pending timer. Both owners' pending state must remain unchanged.
4. Current dispatchAt loses the owner proof, so source inspection predicts it will consume the colliding session timer, then throw from the unavailable fit driver. The default effect guard prevents Boolean mutation but does not prevent this consumed-timer mismatch. Capture this failure as a regression, then fix the root-owned typed bridge/admission without replacing either event owner.

This regression directly tests the new seam boundary, unlike another isolated binder test. It needs no source-qualified runtime due values: all metadata/IDs are declared synthetic contract inputs. It must not be counted as original timing acceptance. Root owns its source/test integration; this worker made no test edits and ran no build.

After the owner bridge is in place, a valid same-session binder result with two observed flat deliveries should reach dispatchAt: unavailable fit consumes only the first delivered ID before throwing; the second stays pending and no click/Boolean effect occurs. That existing consume-before-fit rule must remain intact while fixing pre-dispatch owner validation. Rejected/incomplete/mismatched-point observations must still consume none.

## Admission decision

Keep public Boolean timer effects and constructor PostPending guarded. The immediate candidate work is a typed same-session bridge plus the owner-mismatch regression, and a policy/representation for nested pump timing. Fresh native runtime inputs and the fit/click effect driver remain separate dependencies. An isolated passing binder target does not close these boundaries.


UTC: 2026-10-24T19:41:13.773729+00:00. Root checkpoint53 outcomes are quoted from its retained validation receipt; no new tests were run by this worker. Token measurements unavailable.

| Reviewed source/evidence | SHA-256 |
| --- | --- |
| `analysis/resume53-boolean-trace-binder.md` | `01f1aac39b446254abb5fbc593ec1ba1bdaeacc75d2f06f58747528efdda808f` |
| `analysis/wave3-checkpoint53-validation.json` | `de28e6a1ae87ce14149cf580395b65f387dee5613a309de87a33ae0d0c9fb402` |
| `source/apps/moproj2mst/include/m2m/engine/boolean_trace_due.hpp` | `4fa1cf07a80452114806494f00a8048da5527ba8acf530d4a48670975255a4b6` |
| `source/apps/moproj2mst/src/engine/boolean_trace_due.cpp` | `abb52b8c042117c6ba252f1b99d815b84bed37a8af6f5f3a7266bd8c8456ab8d` |
| `source/apps/moproj2mst/tests/mdt/test_boolean_trace_due.cpp` | `462cc262d52fb018650d3fa94bf37a5eea6f1f134dc5a37353a58fa3d1857257` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `f6d4a46270b7d421e8a78f96db11f990de2e0d61482c01cff2d3662fbe695de4` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `39f1141d2ba59c3d624f7f2be55c50ba0e0e0c15af523b3e9b3430dae15ffbd7` |
| `source/apps/moproj2mst/tests/mdt/test_conversion_session.cpp` | `20b57b6be635894cef20d1b694ec8ea3a98ec552ddb57160695dfacd3020c0b2` |
| `source/apps/moproj2mst/src/engine/lifecycle_events.cpp` | `ebc7758e61132b211d6713e48133e45bf5d132f99c92cb9b16d199177b5dd4e1` |
| `analysis/wave3-boolean-open-model-140183160.asm` | `370093dcd43955e18064af45980ef7523c81779e929abf83949622caeb0eec98` |
| `analysis/wave3-closure-next-140174120.asm` | `11ca3a1fefc1f060a1512d5e0a8a1d96148a0d1ddc9470c44dd28c733524093d` |
| `analysis/wave3-boolean-timer-contract.json` | `63ee9c2bca39e6b14ba2866a0ffc7a8caf5b389f49d4b41530d84e4f6145cdb0` |
| `analysis/wave3-boolean-reselection-contract.json` | `c6ab537765ad84cb9d14d6f2900d970a92b4500cdcf80e348a7c8fece0504268` |
