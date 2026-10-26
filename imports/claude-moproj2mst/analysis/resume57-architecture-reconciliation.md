# Checkpoint 57 architecture reconciliation

## Decision

The latest helpers are useful, source-bounded pieces, but they do not close
their queue items' lifecycle gates. In particular, the DOE flow-receiver
*witness* is not a production registration integration. Likewise, the PPV
reader's binding to model lists is not public Open/Save integration, and the
populated mesh rectangle helper is not evidence that a real 3D project reaches
that data path. Keep all four queue items partial and their public guards in
place.

## Queue reconciliation

| Queue | Latest evidence | What is genuinely integrated | What remains for the lifecycle gate |
|---|---|---|---|
| **R300** | `resume56-style24-cleanup-integration.md`; `resume56-paint-runtime-binding.md`; `resume49-editor-controls.md` | The editor's retained texture-opacity continuation now sequences the image-data boundary, returned QPaintDevice word/class prefix, and a runtime-owned cleanup helper. Its helper refuses without the actual qualified global owner. | This is a selected internal editor continuation, not completed public mode300 construction, setup, active-copy behavior, Save ordering, or fresh original comparison. The checkpoint56 validation still reports historical diffs and says public editor guards remain. Do not label the selected cleanup-tail path full editor lifecycle integration. |
| **RDOE** | `resume56-doe-receiver-witness.md`, `resume56-doe-receiver-population.md`, `resume57-ppv-open-binding.md` | The DOE witness stores explicitly supplied normal/copy construction events, ordered history, and explicit death; the PPV binding maps parser point/flag effects onto existing Operation/Project list owners for isolated calls. | The witness has no production constructor caller, does not install notifier registrations, does not establish QObject death/backend state, and cannot authorize B121 delivery. PPV remains behind `loadSimCtrl`'s public guard: slot `ppvCount`, native QFile/QTextStream decoding, and the paired root-list Save effect/guard are absent. Calling these helpers “DOE lifecycle integration” would overstate them. The actual active-study/report TLS and thread/refcount blockers remain recorded in the queue. |
| **RGEOM** | `resume56-geometry3d-increment.md`, `resume56-geometry3d-node-owner.md` | `geometryMeshRect` consumes a populated geometry-list mesh's two-dimensional node/element arrays under explicit count, width, pointer, connectivity, and finite-coordinate guards. A destructor-compatible synthetic quad tests the candidate seam. | No retained real 3D project fixture demonstrates the positive counts and prepopulated arrays required by that branch. The original no-owner getter path works only when arrays already exist; missing coordinates with null dimension owner are not qualified. Thus candidate scene-box support for a supplied populated mesh is not real-project geometry lifecycle/parity evidence. |
| **RINHERIT** | `resume56-ppv-owned-binding.md` and `resume57-ppv-open-binding.md`; queue still points to owner-binding work | PPV local points bind to operation `recoveryB`; predecessor points/flags bind to existing root and operation vectors, with call-local snapshots and partial list effects. | The model binding does not prove native object deletion/aliasing, public loader count propagation, Save pairing, or that imported inherited corrections survive the full Open→Save→Close path. Keep `RINHERIT` partial; list-owner correspondence alone does not satisfy its imported-correction/shared-function-equivalence gate. |

The checkpoint56 run (179 registered; 172 clean, 6 gap-qualified, 1 diagnostic-difference; no failed tests) is useful regression evidence, not a lifecycle verdict. Its validation explicitly says the historical oracles are not fresh original-session acceptance; the historical set remains 93 `SAME` and 5 `DIFF`. `takeover-state.json` also keeps every objective gate false. These receipts do not support upgrading a queue item based on helper tests or a green aggregate.

## Next executable owner/effect edge

Advance **RDOE** at the first production ownership edge: connect an actual normal `SubProject`/flow-model construction event to the already-existing session notifier's ordered receiver population, then retain the receiver's actual death. The static source path is normal SUB constructor `0x1400b5470` → flow-model constructor `0x140070f10` → registration call at `0x140071166`; the copy constructor and its registration at `0x140071a7f` must remain distinct. The current `FlowReceiverWitnessRegistry` only accepts explicit diagnostic identity tokens and has no production caller, so it is not yet this edge.

The concrete effect to qualify is notifier dispatch reaching that same live model in registration order during B121, with the already-established `(operation id, bool)` invocation order and existing Project/SubProject/diagram-membership guards. Do not bridge straight from constructor to public parser: first bind actual model identity and death, establish population completeness for the selected session, then bind current backend state at delivery. The source evidence says the constructor's initial backend null does not prove a later current null. If this edge cannot be safely wired without those identities/lifetimes, the immediate executable action is a production-adapter design/test seam that refuses until actual constructor and destruction hooks are available—not another isolated witness success case.

This is independent of GUI control and is narrower than enabling the guarded DOE path. It advances a real owner-to-effect link while preserving unsupported backend, QObject, TLS/thread, PPV loader, and Save boundaries.

## Receipt scope

This report reconciles existing receipts and source descriptions only. It does not alter historical receipts or source, run a build/test, use GUI control, or mutate REA state. Checkpoint57 PPV Open remains explicitly guarded; no full Open–Save–Close or fresh original acceptance is claimed.
