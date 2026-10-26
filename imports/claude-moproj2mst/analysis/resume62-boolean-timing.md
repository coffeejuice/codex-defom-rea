# CP62: Boolean engine object creation versus Open/event/Save timing

Static evidence identifies the mutating leaf and conditional caller order, but does not establish the first runtime phase in which a missing Boolean Workpiece/Cutter is created. Editor construction, posting raw0x4a and requesting a1000ms timer are distinct from an actual engine addObject call. Keep the public timing/PostPending guards.

Only this report and unique raw bounded evidence were written. No source/shared state, application control, original execution, build, tests or mutable REA session was used. Newly retained DUMPBIN unwind intervals are bounded code selections, not proof of complete logical/transitive call graphs.

## New exact native binding

| Native site | Established fact |
| --- | --- |
| Boolean editor ctor1411e2480..1411e2543 | Calls base130740, installs primary vtable142cd9b18, allocates its0x40 UI-private block, calls setupUi1411e2dd0 at1411e2529, then fill1411e25b0 at1411e2533. The UI-private allocation is not engine object creation. |
| Vtable142cd9b18+1b8 / +1c0 | Direct PE pointer reads independently bind fill1411e25b0 and apply1411e2870. This supports the retained current-widget virtual1c0 apply route for the actual Boolean dynamic class. |
| Apply1411e2922..1411e2939 | Reads workpiece combo current text and compares exact ASCII `Add New Object` at142cd9a28. False goes to parsing the existing-object text; true reaches the add branch. |
| Apply1411e2944 /1411e2955 | Snapshot first-free-slot helper140060530, then call actual engine addObject140057ba0 for Workpiece. Store returned object number, name `Workpiece`, then rename the previously captured slot at1411e29c6. |
| Apply1411e2b93..1411e2baa | Independently compares cutter text against `Add New Object` at142cd9a48. |
| Apply1411e2bb5 /1411e2bc6 | Snapshot first-free-slot, then call actual engine addObject140057ba0 for Cutter. Store returned number/name `Cutter`, then rename slot at1411e2c40. Workpiece processing precedes cutter. |
| Fill1411e25b0..1411e2861 | Reads Boolean data/model/slots and fills selections. No direct140057ba0 call is in this selected body. Matching existing text is different from the Add New default; workpiece match takes precedence over cutter match. |

The exact selected literal spans also independently read `Add New Object`, `(%d) %s`, `Workpiece` and `Cutter` from the original PE. The prior decompile/fill/apply contract is now cross-bound to these machine-code call sites and vtable pointers; Claude's current port is not used as the oracle.

The direct constructor/fill observations do **not** prove all transitive Qt setup/base/signal behavior lacks side effects. setupUi1411e2dd0, base130740, external notifier recipients and Qt event/selection descendants remain separately qualified. The original addObject's null/allocation/descendant failure behavior is not broadened into a normal-path timing claim.

## Partial order that static source can establish

**Shown editor during native Save.** Retained base Save5ef510 calls widget-update virtual+d0, then unconditionally propsToEngine0543a0. Widget update5f1e20 queries property-stack index; nonzero emits direct UpdateDataInWidgetArea at5f22af, then propsLoad054350 at5f22c4. The actual current-widget handler140dae340 calls virtual1c0; for Boolean's installed table that target is1411e2870. Thus, with actual shown Boolean editor/valid operation+props/data and an Add New selection, the corresponding engine add call occurs in Save's widget-update stage before propsLoad/propsToEngine and later Save slots. Index0 takes the distinct propsToEngine path; it is not proof of an editor apply.

**Delivered reselection callback.** A nonnull diagram attachment170430 requests1000ms modelUpdated at17085f. Actual selected QSingleShotTimer timerEvent67134ca0 can deliver modelUpdated183160; fit precedes resolving the then-current model/item. The recovered same-current Type0 Boolean Object route reaches5f36d0. With an existing shown property editor (index!=0), it applies the old editor, leaves, creates/fills a replacement. That old apply can execute the same engine add sites **before** the replacement fill. Reusing an old Add New snapshot for subsequent callbacks is wrong; refill can change the next selected text to an existing object.

**Native Save's own pump.** Save0980d0 calls processEvents(flags0) at098129 before current-operation Save052ec0 at0981e1. That pump can dispatch already-ready messages/timers; it does not guarantee a1000ms timer became due. An actual callback during that pump could create an object before the later direct Save widget apply. The user action is Save in both cases, but the causal callback/source boundary differs.

**Open and queued events.** Normal selected Open bindings request distinct timers; requests are not delivery. Retained Open/model168 selections do not establish a direct Boolean apply in those selected bodies, but the earlier first-editor/selection and transitive nested-loop dependencies are not fully closed. Startup raw0x4a is a different posted event from QSingleShotTimer's raw timer event. Generic Win32 wake/serial/sendPostedEvents activity does not by itself identify a Boolean apply or a ready single-shot timer.

## What remains undecided without runtime

The first missing-object creation may occur through an already-existing editor's apply during a synchronous/reentrant Open callback, through a delivered timer after Open return, within a later native pump such as Save's entry pump, or at the direct shown-editor Save apply. These are unresolved possibilities within selected call-chain limits, not an exhaustive proof of every application route.

The exact discriminators are runtime facts: first/current property editor index and dynamic type; actual combo snapshot after fill; current model/item/operation and loading/selection guards; whether a selected timer was registered and delivered; native pump/parent callback nesting; and prior apply/refill history in the same fresh-project interval. Even if two timers are requested, an initial editor index0 versus an already-shown Add New editor changes which delivered selection can first reach apply. Static interval constants and final saved bytes cannot choose that branch.

Historical “opened” screenshots showing a Cutter do not locate creation before native Open return: they can include idle event-loop work. Nor does the final saved Cutter prove direct Save created it; an earlier callback could have done so. No new original runtime observation was added here.

The smallest remaining static investigation, if chosen before runtime, is the first-editor publication path: setupUi/base automatic callbacks and the source-qualified current-index/initial explicit row0 click route ending in5f36d0. It may close more conditional edges, but absence of a direct call cannot become whole-program proof that Open never applies. The current timing guard should not be relaxed from this report.

## Safe next action

Preserve the source-proven conditional Save/reselection implementation and refill semantics. Do not add unconditional postLoad apply, infer due IDs from serial/timer request counts, treat raw0x4a as Boolean delivery, or replay stale Add New data. Candidate-only tests may validate these conditional partial orders, but this task runs/adds none and they would not locate original first creation.

When a supported owned-session observation is available, the minimal discriminating hooks are apply1411e2870 plus its two add call sites1411e2955/1411e2bc6, Boolean ctor/fill, native Open entry/return, Save entry/pump/direct widget-update contexts and selected timerEvent/modelUpdated nesting. Read actual current combo selections/returned object identity only at source-qualified sites. That directly attributes engine creation to its causal context; a general screenshot or message count is weaker. Existing control-release/ownership gates remain unchanged.

## Raw evidence and checks

UTC: 2026-10-24T21:34:09.014163+00:00. Six static identity/order checks passed; no C++ tests or original execution. Token measurements unavailable.

- constructor_directly_calls_fill: passed.
- apply_has_two_engine_add_sites: passed.
- selected_fill_has_no_direct_engine_add: passed.
- vtable_fill_apply_bound: passed.
- exact_workpiece_then_cutter_calls: passed.
- public_postpending_guard_retained: passed.

| Evidence | SHA-256 |
| --- | --- |
| `analysis/resume62-boolean-timing-static.json` | `1a4135eeed4433789f4f588b75b7c46d85e459ed3ed7e03a46f28867a85d36de` |
| `analysis/resume62-boolean-timing-1411e2480.asm` | `6561e7e4b7e5c2918e657a4bcca4d52eb5981a6a5583b9ffc46d3c5ba8639a2f` |
| `analysis/resume62-boolean-timing-1411e25b0.asm` | `9bd8c120fd83bca7de996a5f129f3d3f8a10d00b9fe4aa4a898a94f1418248f8` |
| `analysis/resume62-boolean-timing-1411e2870.asm` | `36cae45274444b3654c0f8b88c62053c4bec5b471587384a2fd062eae40608c8` |
| `analysis/resume62-boolean-timing-vtable.json` | `5aa88263b17d518c9cf67c24cc7debe00c147ce43142294d43be5f35b75a7720` |
| `analysis/wave2-db133-boolean-contract.json` | `1862dbf63c609a9aa943751becb5417e3688897893bce5821bd16d5ab8dbea72` |
| `analysis/wave2-db133-open-lifecycle.json` | `82ff5ead28846fad5dd98df56eb397879e3747466a8f8f78f70cf12e0e8e3983` |
| `analysis/wave3-save-widget-order-contract.json` | `ed07d99c64007a1cfe2c0d9fda5d60f72893b6fe30a79212cf9e0e5f7c6f009e` |
| `analysis/wave3-boolean-reselection-contract.json` | `c6ab537765ad84cb9d14d6f2900d970a92b4500cdcf80e348a7c8fece0504268` |
| `analysis/wave3-boolean-timer-contract.json` | `63ee9c2bca39e6b14ba2866a0ffc7a8caf5b389f49d4b41530d84e4f6145cdb0` |
