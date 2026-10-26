# CP214: reconcile Boolean timing claims after static lifecycle fixes

This read-only reconciliation separates three facts older RBOOL notes often grouped together: the native Boolean constructor's notifier stage, a synchronous zero-slot postLoad add, and a later Apply-origin add during Open. CP102/CP203 close the conditional postLoad branch in original code and candidate tests. They do not close constructor E's notifier occurrence or establish Apply timing during the selected first Open. Any earliest Open-time creation claim must retain its origin label.

## Scope and evidence qualification

Only this report was written. No source, tests, shared state or plans were edited; no build/test, original execution/control, or mutable REA session was used. Retained native evidence is for DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Address abbreviations omit image base `140000000` for EXE RVAs. Disassembly establishes selected static control flow, not a runtime occurrence or exhaustive call graph.

The controlling queue entry is `analysis/takeover-state.json` RBOOL (active, incomplete; required validation includes Open-time creation matching original). CP164 is the direct constructor/Apply reconciliation; CP203 and CP102 qualify the zero-slot branch; CP196's retained-capture census and CP98/101 explain why historical observations do not supply the missing joins.

## Reconciliation: what each path proves

| Path | Established source behavior | What it does not establish |
|---|---|---|
| Boolean constructor E | The Object factory's literal Object branch allocates 0x50 E at `11e065b`, then calls `11e2480(E, OPI)` at `11e0677`. Constructor enters wrapper `130740`; QWidget normal return `377990` precedes wrapper setup. Setup `130830` binds E+38 to OPI, conditionally retrieves the model, binds E+28, then calls canonical notifier `2cf3c0` at `130ad1`, returning at caller continuation `130ad6`. | Static ordering does not prove a Boolean construction reached the notifier, its suppression/N+90 state, or that notification caused creation. Later fill, a Forming capture, eventual editor address, or Session notifier registration does not bind that constructor frame. |
| Boolean postLoad, zero OPI slot records | Native `1411df020` reads the OPI slot-record count at `11df0cb`; zero takes `addObject(OPI)` at `11df0e9` (normal return `11df0ee`), then sets slot 0/kind 5 at `11df0fb`. In the selected normal base-load order, postLoad precedes propsLoad and postLoadViews. Candidate `l5_slots.cpp:booleanPostLoad` implements this prefix and `test_boolean_postload_creation.cpp` distinguishes zero slot records from nonzero records with zero engine objects. | It tests neither engine object count nor `cutter == 0`; the native body does not check add's result before set-kind or normalize Boolean fields. Static predicate/test closure does not show a particular Open OPI reached zero count or attribute this add to Apply/timer delivery. This is already implemented and tested; no new postLoad edit follows. |
| Apply during first Open | Boolean Apply `11e2870` can add through distinct Workpiece and Cutter continuations `11e295a` and `11e2bcb`. An actual nested UpdateData/Apply/add return inside a complete selected Open interval would establish that positive occurrence before Open completion. | The distinct postLoad return `11df0ee`, final object count, file parity, constructor notification, or editor fill cannot stand in for Apply. One positive occurrence does not prove it was first, exclude an earlier occurrence, or identify an Open-registered timer without gap-free coverage and timer/connection ownership. |

The Qt timer, callback, reselection and replacement chain recovered in `wave3-boolean-timer-contract.md` constrains possible timing: it describes a requested 1000 ms timer and current-editor/reselection behavior. Save's selected `processEvents` call can deliver already-due timers but does not wait a second for future timers. This is static lifecycle evidence, not a measured delivery time for a fresh Open, and it does not collapse postLoad into timer/Apply behavior.

## Narrowest original witnesses

**Constructor notifier witness.** For one naturally reached constructor invocation, correlate one process/module profile and TID; the successful Object-factory branch and allocation lifetime generation for E; `11e2480(E, incoming OPI)`; same-E QWidget normal return at `377990`; same-invocation setup/guard normal returns through `130830`; actual E+38 OPI resolution; `05f770(OPI)` model return only if that branch is taken; `377b20(E,S)` return for E+28 while preserving nullable S; and notifier entry `130ad1` identified by caller continuation `130ad6`. Retain canonical notifier N identity, its N+141 suppression read, and N+90 before/after. E+28 and E+38 are different guarded fields. This is the narrowest evidence for the constructor gate/reset; it does not by itself admit posted-event delivery, Apply, or object creation.

**Positive Apply-origin Open witness.** Select one complete fresh Open interval: GUI handler `271b0..28514`, or selected CLI startup branch through `1e4e`. Within that same process/thread and nested frame, retain current-widget UpdateData `dae340` -> Boolean Apply `11e2870` on E, resolve E+38 to OPI O, and capture normal `addObject(O)` return at `11e295a` or `11e2bcb`, including returned object/number and owner lifetimes. Independently label zero-slot postLoad continuation `11df0ee`; do not treat it as the Apply witness. Loader return `1c94` and postOpen return `1cae` alone are not whole-Open completion boundaries. A positive nested chain proves one Apply-origin add before completion; “first”/absence or timer attribution needs gap-free selected coverage, reentrancy accounting and (for a timer claim) actual timer/connection ownership.

These are separate witnesses. They should not be combined into one broad memory snapshot or inferred from end-state parity.

## Retained evidence and candidate seam

Editor003/004 observed a Forming constructor (`eb26f0`), not Boolean Object factory/constructor `11e2480`; Editor004's QWidget/notifier observations lack Boolean caller continuation `130ad6`. The retained capture audit found no qualifying Boolean Apply/add chain or same-invocation constructor frame. Its negative result means the retained set does not cover the required witness, not that the original path never runs. The offline Boolean Open checker is a structural preflight and does not authenticate a new capture; its retained Forming input is not acceptance evidence.

Candidate seam for later work is bounded:

- Keep the existing postLoad zero-slot behavior and regression as-is; they already cover the static conditional prefix and candidate behavior.
- For constructor work, only after the same-E notifier receipt, integrate the gate/reset through the existing Project/Session notifier owner and stop at that admitted effect. Do not reset from ordinary selection fill, manufacture constructor progress, or claim E/H posted-event readiness.
- For first-Open timing, preserve public Apply/constructor/PostPending guards until the nested original Apply/add receipt exists. Then compare exact native order and failure prefix against the existing Apply/editor snapshot seam; do not move Apply earlier based only on timer metadata or file equality.

The CP203/164 lineage records unsupported/unverified app-control release; subtree instructions bar acquiring GUI control until a supported release operation and verifiable receipt exist. Thus the next admissible action is offline selector/coverage preflight or static candidate seam review. No original timing capture is licensed by this report. Preserve unknown timing and failed/missing returns as unknown rather than converting them into no-ops.

## Source references

- `analysis/resume164-boolean-next.md`: constructor E missing returns and separate first-Open Apply/add witness.
- `analysis/resume203-boolean-open-constructor-static.md`: CP203 zero-slot predicate and retained-capture limits.
- `analysis/resume102-boolean-postload-creation-bound.md` and `analysis/resume102-boolean-postload-test-implementation.md`: native branch/base-load order and candidate regression.
- `analysis/resume159-boolean-frame.md` and `analysis/resume156-boolean-constructor-notifier.md`: constructor owner and notifier call chain qualifications.
- `analysis/resume103-boolean-creation-census.md`: distinct native creation continuations.
- `analysis/resume196-bool-witness-audit.md`: bounded retained-capture coverage.
- `analysis/wave3-boolean-timer-contract.md` and `analysis/wave3-boolean-external-callback-contract.json`: static timer/dispatch lifecycle and runtime delivery limits.
- `analysis/takeover-state.json` RBOOL: active item and acceptance requirements.

No timing-dependent production change or new native-runtime claim is supported by this reconciliation.
