# Boolean timer delivery: input contract, future capture and isolated seam

The smallest safe increment separates wake execution from timer delivery. The immutable wake body closes conditional state transitions; the timer/reselection chain closes what a delivered event does. Neither gives the missing startup dispatcher values or the native schedule that actually delivered Boolean callbacks. Constructor PostPending and actual Open-timing claims remain guarded.

This report changes no source/shared state, creates no probe, launches no process, acquires no GUI/debugger control and runs no build. Runtime input values below remain unknown unless the cited stage-qualified observation exists.

## Exact missing inputs

| Input | Native source/width | Required qualification |
| --- | --- | --- |
| Dispatcher/private owner and call occurrence | Caller threadData+20; dispatcher+8; admitted slot98 route | Same selected receiver/queue/thread and one native wake invocation. A known pointer in a historical process is not a live reconstructed owner. |
| Serial entry value, or explicitly unknown base | Private+a8 dword, Core67141b78 atomic add1 | Native32 wrapping increment happens once before HWND/CAS. Preserve unknown base symbolically; never seed0. Stage-anchor later observations so post-increment state does not silently become an entry value. |
| First HWND load | Private+98 qword at67141b80 | Null skips CAS/message; nonnull permits CAS. This is not dispatcher absence. |
| CAS observation/outcome | Private+b4 dword at67141b91 | Compare0/exchange1 atomically. An earlier snapshot is not the outcome under unqualified concurrent mutation. Preserve nonzero old32-bit value on failure. |
| Second HWND load | Private+98 at67141b9b after successful CAS | Source reloads this field. Preserve its stage separately from the earlier nonnull test; do not force equality without qualification. |
| Required message attempt | PostMessageW call67141bad with reloadHWND,0x401,0,0 | Success-path attempt is required even though its returned boolean is ignored. This body does not roll back postedWake if the API fails. Missing attempt evidence is not an observed failed attempt. |
| Actual timer registration and delivery | Diagram170430/schedule17085f; selected QSingleShotTimer and native timer ID; timerEvent67134ca0 | A1000ms request is not readiness/delivery. Map lifetime and occurrence, including address reuse. Actual callback/pump context is independently required. |
| Current Boolean target and effect sequence | modelUpdated183160, after fit; Boolean factory1411e0510/ctor1411e2480/fill1411e25b0/apply1411e2870 | Resolve then-current attachment/item after fit; initial construction differs from replacing/refilling an existing editor. |

Retained startup004 identifies the queue/dispatcher objects and captures append/count/posted-bit order, but explicitly skipped private HWND/serial/postedWake reads. Equal opaque state descriptors do not fill these inputs. The later editor300 profile and its wake values are a different context and cannot seed Boolean startup. Immutable PE supplies offsets/order/branches, not allocator-private runtime contents.

The native enqueue chain finishes queue mutation and unlock before wake. Normal queued-event delivery clears posted/count/tombstone under lock, unlocks, notifies, then deletes the event. Receiver-wide cleanup collects/deletes without notification. The checkpoint48 removal helper is therefore not a delivery or wake seam.

## Minimal original capture after the GUI release prerequisite is satisfied

Current GUI control is not acquired by this task. Before any future GUI interaction, root must establish a concrete supported release operation plus verifiable receipt/status for that tool surface. A debugger detach, JS reset or owned-process closure does not satisfy it. Inspect the current capability change rather than reusing an old unsupported API assumption. Existing user-owned sessions stay untouched. If control is acquired, end that interaction promptly and verify release before unrelated analysis; retain uncertain outcomes and stop on Escape/tool stop.

Use one fresh owned original process on one disposable, manifest-bound Boolean project under C:\PROBLEM with all generated paths shorter than100 characters. Spawn ownership must retain PID/start identity/executable hash. Never attach to a pre-existing or uncertain user process. Preserve original binaries/source fixtures/settings evidence. Root alone launches/cleans. The observer remains passive: no Qt calls, data/PC changes, synthetic timer callbacks, extra processEvents calls, sleeps to force readiness or injected messages.

Prefer the already-qualified native CLI path if it naturally reaches this exact project/action boundary. If a GUI action is needed after the supported release lifecycle is established, record one native Open, one Save and the observed source defaults only. Do not add a row0 click or forced reselection to create the callback being investigated. Native waiting/message-loop time between actions is part of the observed schedule, not a prescribed1000ms timer model. If a native dialog or branch cannot be reached within the declared authority/control lifecycle, preserve the failed prefix.

The minimal hook set has two filters in the same owned process; they may be collected in separate fresh runs if role lifetime/coverage would otherwise be ambiguous:

1. **Constructor posting/wake filter.** Reuse probe004's exact first constructor/post context. Keep Core+11afc6/+11afe8 locked queue snapshots and Core+11b015 same-owner post-bit/count/canWait/dispatcher state. Correct the recognizer to admit exact QtGui+5e0ca0 only with the loaded IAT QtGui+697618 target Core+141b70, or admit the verified direct Core target. At the already-existing Core+141b70 entry/return read actual private+98/+a8/+b4. Add only the source-bound successful-CAS block/call-site read if snapshots cannot uniquely establish the branch or second HWND load. Do not broaden to a memory dump.
2. **Boolean timer filter.** Record selected diagram setModel/schedule contexts; selected singleShot/timer allocation/startTimer return; matching timerEvent entries; modelUpdated entry/return/current target after fit; Boolean factory/constructor/fill/apply nesting; native Open entry/return and Save's actual flags0 pump140098129/Core6711d130 entry/return. Filter application-wide timer sites by the selected diagram registration, actual timer object/generation and native thread. Unknown pointer layouts stay raw/unknown.

Retain a monotonic callback ordinal, native thread, exact source site/occurrence, balanced entry/return, enclosing Open/Save/pump/timer contexts, receiver/model/timer/editor identities, requested interval/native timer ID, result/exception/terminal endpoint and source qualifications. Timestamps diagnose instrumentation but do not substitute for actual timerEvent. Hook overhead and debugger pauses perturb the schedule; label the observed instrumented action sequence and make no uninstrumented latency claim.

Acceptance needs no failed reads/truncation/unmatched selected frame and the actual identity chain registration -> selected timerEvent -> modelUpdated -> Boolean effect. A complete interval without callback supports only “none observed in this interval,” not a global no-delivery claim. A nonzero registration count or final saved bytes cannot fill callback absence. Preserve failures/timeouts and stop further runtime progress when ownership or supported control cleanup becomes uncertain.

Finally keep distinct receipts for: supported GUI release/status if acquired; debugger close/detach status; exact owned-process absence/presence/unknown; project/cwd effect manifests and normal/failed-prefix endpoint. GUI release is not implied by any of the other receipts. Do not terminate or close a user-owned application to clean this capture. The no-GUI design and existing immutable reads do not assert the current overlay is off.

## Safe isolated helper seam available now

Use the existing `DiagramLifecycleEvents` owner and `ConversionSession::dispatchAt` contract rather than introducing a second scheduler. Those components already separate logical timer identity from model/item resolution, consume a delivered timer before callback, retain timers across null rebind, disconnect receivers independently, and make dispatcher destruction terminal. None infers readiness.

A bounded next helper can be a **trace-to-DueObservation binder**, not a clock or application callback runner. It takes an immutable, qualified trace selection plus the existing session's schedule mapping and returns a validated dispatch observation or a diagnostic error. It owns no QObject, queue, native timer, project or alternate timer list; it neither mutates the event owner nor invokes modelUpdated/Boolean apply. Existing dispatchAt remains the sole integration point and DiagramLifecycleEvents::deliver remains the sole logical consumption operation.

Suggested internal inputs/outputs, as a design contract rather than implemented API:

```text
Inputs:
  TraceIdentity(binary/script/fixture hashes, action-profile identity)
  ScheduleMap[(native receiver lifetime, timer object lifetime, registration ordinal)
              -> existing DiagramLifecycleEvents receiver/timer IDs]
  CompletePumpInterval(source site, occurrence, native thread, balanced boundaries)
  OrderedSelectedDeliveries(actual timerEvent evidence rows, callback context)

Output:
  DueObservation{
    point = exact native/source PumpPoint,
    origin = OriginalTrace,
    qualification = trace receipt + complete interval + mapping receipt,
    dueInOrder = mapped existing logical timer IDs in observed order
  }
```

Admit only a complete interval with module/profile match, bijective live registration mapping and matching source occurrence. Reject unknown timer registrations, duplicate delivered IDs in one interval, reused addresses without lifetime generations, mismatched receiver/project profile, unmatched callbacks, truncation and mapping to an unrelated session's IDs. A native startTimer0/no-dispatcher result is not a delivered logical timer; preserve that explicit source branch/guard and do not silently map it to a pending ID.

Unavailable/incomplete evidence returns a diagnostic/unavailable result, never an empty due list. A complete explicitly observed interval with no selected deliveries may return an explicit empty list qualified only to that profile/occurrence. `DeclaredAdapterSchedule` remains a distinct origin and cannot be relabelled `OriginalTrace`. Binding native delivery order does not establish complete fit/click effects; the existing effect driver retains its guards.

This helper can be independently tested with clearly declared synthetic contract fixtures: unavailable versus complete-empty interval; two registered timers mapping to two observed deliveries in order; duplicate/unregistered delivery rejection; source-occurrence mismatch; pointer reuse with different lifetimes; and no mutation of existing pending timers until dispatchAt consumes them. These tests would validate the binder, not original timer timing. No tests or source increment are delivered by this report.

A separate source-qualified Win32 wake prefix may later retain the serial mutation and pending HWND/CAS/message stages on the actual same dispatcher owner, as described in resume52. It cannot supply DueObservation: queued wake-message posting and timer readiness/delivery remain different facts. Neither proposed helper should move public PostPending merely because isolated tests pass.

## Next dependency

Root can prepare/review a new narrow passive script and the immutable trace schema now. Original numeric/runtime evidence remains gated by the supported session lifecycle and exact owned process. After a qualified capture, the binder can translate the observed schedule for existing dispatchAt, while the independent posting/wake continuation consumes its actual source-stage inputs. Preserve the public constructor and timer effect guards until those integrations pass their relevant comparisons.


UTC: 2026-10-24T19:23:02.753471+00:00. Static design only; no uncaptured values assigned, source changes, tests, builds or runtime work. Token measurements unavailable.

| Input | SHA-256 |
| --- | --- |
| `analysis/resume51-boolean-dispatcher-state.md` | `d32385f367861569551d7146f52dc773d8ed7a89813c100449eeabf6cd476a2e` |
| `analysis/resume52-boolean-wake-prefix.md` | `5266a1879e1f80cc3d8b5e769f40318d47858ab261425853dc158e6b8f921626` |
| `analysis/resume50-boolean-passive-hook-plan.md` | `32aaf2939feada6992b6d154ad42d724e1a2d1226e2a033bb75303852bddbda6` |
| `analysis/wave3-boolean-timer-contract.json` | `63ee9c2bca39e6b14ba2866a0ffc7a8caf5b389f49d4b41530d84e4f6145cdb0` |
| `analysis/wave3-boolean-reselection-contract.json` | `c6ab537765ad84cb9d14d6f2900d970a92b4500cdcf80e348a7c8fece0504268` |
| `source/apps/moproj2mst/include/m2m/engine/lifecycle_events.hpp` | `61c574811261d54da39c989f4326ca82c17613564e48f1aee2f75096c373fe09` |
| `source/apps/moproj2mst/src/engine/lifecycle_events.cpp` | `ebc7758e61132b211d6713e48133e45bf5d132f99c92cb9b16d199177b5dd4e1` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `f6d4a46270b7d421e8a78f96db11f990de2e0d61482c01cff2d3662fbe695de4` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `39f1141d2ba59c3d624f7f2be55c50ba0e0e0c15af523b3e9b3430dae15ffbd7` |
