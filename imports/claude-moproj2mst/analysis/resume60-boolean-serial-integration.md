# CP60: Session-owned serial observation and minimal original capture

No existing Session-owned observation can truthfully supply dispatcher HWND/+a8/+ac today. The smallest integration is an immutable source-stage dispatcher observation bound to the Session's actual runtime/thread control blocks and one native capture epoch. It can initialize/refine the canonical serial-gate projection without treating a wake notification as Boolean timer readiness.

Only this report was written. No source/shared state, build, GUI, original process or mutable REA operation was changed or executed.

## Existing ownership and observation boundary

ConversionSession already owns widgetRuntime_ and exposes widgetRuntimeOwner(); that runtime owns the shared WidgetThreadState returned by threadState(). The same thread object owns WidgetPostEventQueue. WidgetRuntimeValueObservation covers application/widget/value inputs; WidgetConstructorTailObservation covers font/DC/DPI/counters/notify-depth/compatibility. Neither has dispatcher/private/window/serial fields. WidgetThreadState currently represents notify depth and posted queue, not a Win32 dispatcher field store.

The Session's BooleanTraceOwnerScope binds its logical DiagramLifecycleEvents scheduler and trace admission. It proves that candidate logical owner/lifetime, not native HWND/dispatcher identity or readiness. The isolated BooleanSerialGateState represents only private+ac plus explicit native-owner identities. Its per-frame serialAtGate input is an actual staged read, not a persistent +a8 model; it does not store HWND or postedWake and cannot fill them from a timer trace.

Do not extend font/tail values with unrelated guessed fields, identify captured threadData by a C++ pointer cast, copy the queue to a second owner, or interpret equal logical timer IDs as native dispatcher proof.

## Exact observation that could bind the fields

Proposed root-owned **BooleanDispatcherStageObservation** (design only) must carry:

| Field | Required representation/proof |
| --- | --- |
| Actual candidate runtime/thread | Weak links to this Session's actual WidgetRuntimeState and its exact threadState object/control block. Session validates pointer AND ownership equality before binding; aliases/foreign control blocks do not suffice. |
| Native capture namespace | Fresh native session/creation identity, EXE/QtCore/QtGui/script/fixture hashes, action profile and retained receipt. Native aliases are interpreted only within that epoch. |
| Native identity tuple | Observed dispatcher object, private owner and threadData aliases, plus evidence that threadData+20 is this dispatcher and dispatcher+8 is this private. Bind them to the actual candidate runtime/thread through the source-qualified role witness, not numerical address coincidence. |
| Source point | Exact address, occurrence, native thread, entry/after-read/before-store/return phase and enclosing constructor/post/message context. Each field belongs to that phase; no perpetual generic current snapshot. |
| HWND | optional64-bit value at private+98; absent means unknown and explicit0 means observed null. Keep wake's first test and post-CAS reload distinct. Historical numeric handles are comparison evidence, never handles to pass to live Win32 APIs in the candidate. |
| Serial | optional32-bit private+a8 value, phase-qualified. Wake-entry serial is pre-increment; message-gate serial is a different read. Do not seed0 or translate serial deltas into numbers of due timers. |
| Processed comparison | optional32-bit private+ac read at the selected message gate. This is not postedWake. Source updates it before sendPostedEvents. |
| Optional consumer fields | private+b4 postedWake as full32-bit state, private+b0 fallback timer, message kind/full64-bit wParam and relevant actual API/branch outcomes, only when captured/consumed. |
| Prefix witness | For the isolated message-gate slice, proof that this same original invocation reached Core+142949/+142950 after its real application/native-filter/userdata prefix. This witnesses that occurrence; it does not invent empty filter lists or emulate their earlier side effects. |

Keep the raw observation immutable. Session should retain one canonical BooleanSerialGateState per admitted dispatcher/private/thread lifetime, created with observed +ac or unknown; do not maintain another mutable +ac copy in the observation. serialAtGate feeds the matching frame only. HWND/+a8/postWake fields stay staged evidence until their corresponding source continuations exist.

Session must validate its own runtime/thread, source point/profile/epoch and actual role witness before constructing the helper state or setting prefixAdmitted. A different native owner/epoch cannot overwrite live state or replace a pending frame; expire admission and require a new explicitly qualified scope when the original lifetime really changes. Ordinary model/null rebind and Project release do not themselves establish native dispatcher replacement. Escaped frame/state leases must be invalidated at the Session adapter boundary.

The current observeProcessedSerial API only fills unknown/equal state. A later differing native observation cannot silently overwrite a known model store; reproduce the source effect/transit or flag the missing transition. Constructor zero stores at67142665..671426b0 are valid only at that constructor point, not as late message/wake observations.

## Source-stage reads and minimum capture

The retained probe004 decoder skipped HWND/serial/postedWake. No new static PE fact recovers their contents. The smallest future original read sets differ by question:

| Question | Minimal selected passive reads |
| --- | --- |
| Constructor PostPending wake branch | Reuse the existing selected raw4a post and locked queue owner. Correct exact QtGui+5e0ca0 thunk recognition by verifying loaded IAT QtGui+697618 -> Core+141b70. At current wake entry/return retain private+98/+a8/+b4 and same dispatcher/private/thread identities. +ac may be recorded there as an additional staged snapshot, but is not yet the later message-gate +ac value. |
| Bind the new serial gate | At Core+142950, immediately after +142949's +a8 load, record actual EAX serial, R8 private owner, private+ac, private+40 threadData, actual messageESI, full wParamRBX and private+b0 when the message is0x113. Retain original window-procedure HWND/current-dispatcher role and balanced source occurrence. At +142959/+142968 retain update-before-dispatch order and actual resulting call context. Addresses here are module RVAs; preferred VAs are67142950/67142959/67142968. |
| Reset postedWake | Only if the reset branch is the uncertainty, retain selected hook67141e40's same private owner/queue-status outcome and the exchange at67141ecb, optionally next site67141ed1. This conditional hook resets+b4; do not clear it from receiver cleanup or from generic message arrival. |
| Distinguish fallback timer from Boolean timer | Capture actual private+b0 and full WM_TIMER wParam at the selected procedure route. Nonzero equality routes to the generic serial gate; it is not a Boolean single-shot delivery. Ordinary registered-timer path delegates67141ff0. |
| Prove Boolean readiness/delivery | For only the selected diagram registration, retain singleShot/startTimer request/object/native ID/lifetime and actual QSingleShotTimer::timerEvent67134ca0, then modelUpdated183160 and source PumpPoint occurrence. A0x401 message, changed serial or fallback0x113 is insufficient. |

RVA notation: Core+142950 above is 0x142950 relative to the preferred0x67000000 module base, yielding0x67142950. Wake141b70 and hook141ecb use their corresponding Core RVAs. New-process ASLR addresses must be resolved from its actual module base; no historical address is a runtime constant.

A hook at the gate can witness that original prefix admission for this bounded slice without logging every application filter. It cannot be used to declare those filters nonexistent or to skip their effects when claiming full public lifecycle parity. Capture the post-prefix values actually consumed by the helper. If other wakes can modify +a8 between passive snapshot and source read, prefer the actual EAX after-read register rather than an earlier field value.

The wake body reads HWND twice and increments+a8 before the first test. Successful CAS requires the message attempt even if PostMessage returnsfalse; actual atomic outcome/reloaded HWND are separate stages. Numeric pre/post snapshots alone may not identify concurrent interleaving. Retain an exact successful-CAS/call-site witness if necessary, without injecting an API call or pump.

Future runtime work stays serialized under root and the existing owned-process/control-release plan. This task acquires no control and asserts no current overlay status. If required supported lifecycle/identity checks cannot be established, retain the blocked capture dependency and continue candidate/static work; do not touch user-owned sessions.

## Safe next candidate action

Root can first implement the Session-owned observation/admission seam described above, with all numeric fields optional and no default startup values. Keep it an internal bounded diagnostic path to the existing isolated serial gate: validate actual runtime/thread control blocks and provenance, create/refine one canonical +ac state, and supply source-phase input to a retained frame. A pending sendPostedEvents request remains unimplemented; it must not call receiver removal, create a DueObservation, execute a timer callback or advance constructor PostPending.

Candidate-only regression can use explicitly synthetic qualified records to show: foreign Session/runtime/thread or control block rejects before state mutation; unknown HWND/a8/ac stays unknown; staged reads cannot be mixed across source occurrences; differing native owner/epoch cannot rebind; and unequal admitted serial retains +ac before the existing pending dispatch requirement. Such tests validate admission/storage, not original readiness. No code or tests were added here.

Keep the Session typed timer binder bridge separate. It already protects logical owner/lifetime at dispatch; it cannot authenticate a dispatcher snapshot or provide native numeric state. Nested callback replay, full fit/click effects and original observation requirements remain guarded.


UTC: 2026-10-24T21:08:52.759466+00:00. Read-only source/retained-evidence audit; no new runtime values or validation results. Token measurements unavailable.

| Reviewed input | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `4e3506ecf7ea8eb253b9055aacff6604dc82958795f9b563e0bbbdc46ac8b283` |
| `source/apps/moproj2mst/include/m2m/engine/widget_events.hpp` | `e75a9f7631dab2202a894310e94adfbda0906232074a6b57c8ab1531cd55b4ec` |
| `source/apps/moproj2mst/include/m2m/engine/boolean_serial_gate.hpp` | `3926e87cc057b03108942c0d6a3ead4d96889bb84f0f668cbd91cc949e490622` |
| `source/apps/moproj2mst/src/engine/boolean_serial_gate.cpp` | `ef8261c3c619920dee9e1ae1f55b8fbcabc52b85c939589124aaed6a9e9ca1ab` |
| `analysis/resume59-boolean-timing-next.md` | `bd7ddd47e660f81254e28f0238c0a1cab29a9e786973339cd47a28718bb80f19` |
| `analysis/resume51-boolean-dispatcher-state.md` | `d32385f367861569551d7146f52dc773d8ed7a89813c100449eeabf6cd476a2e` |
| `analysis/resume57-boolean-session-implementation.md` | `47055d101b13f95a9ee4f86704a0dbde3f34eb6a539d8cfc065e92fb8b075383` |
