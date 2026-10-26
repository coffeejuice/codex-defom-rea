# CP132: bound private NSTART toggled callback

The next bounded private editor transition is on_chkbxNSTART_toggled(bool). For procType5 and a nonnull OPI, native code writes the operation's given flag immediately, then obtains the current simulation control and refills the step-count page. Current code only stores that flag during Apply; it has no delivered-toggle transition. The callback can be composed through the existing StagedEditor300 identity guard and page owner without opening the public palette constructor.

Only this new report was written. No source/test/shared-header/CMake changes, builds/tests, GUI, original execution or mutable REA occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact native store/refill sequence

The automatic slot map index50 dispatches1410dba96 to140ecf910. The callback has no same-value early return:

| Original address | Ordered effect |
| --- | --- |
|140ecf924..f92c|Read procType from143d766b8 through2cb7a0; other modes return.|
|140ecf933..f943|Resolve OPI through130af0; null returns before SC/model access.|
|140ecf945..f94f|Pass delivered bool to05a2c0.|
|14005a2d3|Store delivered byte into OPI+18; setter then returns.|
|140ecf959|Only AFTER that store, resolve current SC through130f90.|
|140ecf96d|Call140ebb110(editor,currentSC) to refill.|

Getter130f90 reads the editor's actual model through virtual+198, returns null if absent, otherwise calls140c7b2d0. That provider resolves the model's guarded step via c7acd0 and returns step+750 (SC), or null. This corresponds to the existing bound GuiModel::simCtl source, not an unrelated project or copied SC.

Refill140ebb110 starts with SC.nstart. In MO mode it resolves actual OPI/starts-from-key/current class virtual+200. A supported class reads the operation's given flag and, when checked, its saved startStepNo. Then the original positive-value test has its known bug: if the selected source is positive, ADD1/NEG at140ebb360/363 uses SC.nstart, even if positivity came from operation.startStepNo. Keep uint32 wrap and the existing spin clamp.

The refill then sets start spin UI+1718 at140ebb380, steps UI+1740 atb3a4, increment UI+1750 atb3c8, and calls primary-die refill140ebb3f0 atb3d3. The tail uses current eligible object numbers and SC.pdie1, with fallback0. Thus a toggle discards unsaved step-count numeric/primary selections in favor of current source values. Do not refill only the start spin or preserve unrelated unsaved fields as an improvement.

The callback itself writes neither SC.nstart/nstep/stpinc/pdie1 nor operation.startStepNo. Save Apply later commits those values. The immediate operation.given mutation must remain even when a later candidate SC/refill prerequisite is unavailable.

## Checkbox history correction needed for reuse

Existing fillStepCount is a fresh-fill projection with nstartChecked initialized false. Blindly assigning its result inside a stateful callback is wrong for the starts-from-key branch:

- Null OPI or starts-from-key: native hides the checkbox/sets stack page but does NOT set checkbox checked state. Preserve prior delivered checkbox value.
- Non-key, missing/nonwriting class: native blocks signals, sets checkbox false, unblocks false. The operation flag written by the callback remains the delivered value; do not clear it to match the checkbox. Save may later clear it under its own source branch.
- Non-key writing class: native blocks signals and reloads checkbox from operation.given, then uses operation.startStepNo when true.

The private page need not claim visibility, physical signal blocking or QStackedWidget effects. It must preserve these checked-value outcomes. Native unblock(false), not restoration of an arbitrary old block state, is also not a license to add a nicer widget policy.

Small reuse change: retain the existing two-argument fillStepCount entry as a wrapper using prior=false, and add a three-argument overload with priorNstartChecked. Initialize checked from prior; for MO non-key live OPI explicitly set false on nonwriting class, or set from operation flag on writing class. Key/null/non-MO branches preserve prior. Existing fresh calls remain source-compatible and retain their current results. The callback supplies its actual cached checkbox value after a declared delivered toggle.

## Bound private API and ordering

Recommended entry is a StagedEditor300 method, not a free helper with fabricated Operation/model ownership:

```cpp
enum class StepCountToggleStage { Preflight, OperationFlagStored, Complete };
struct StepCountToggleReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    StepCountToggleStage stage = StepCountToggleStage::Preflight;
    bool operationFlagStored = false, pageRefilled = false;
};
StepCountToggleReport StagedEditor300::toggleNstart(
    Editor300Binding, const StepCountContext& currentContext, bool checked);
// Added internal overload; retain the old two-argument function as a wrapper.
std::optional<StepCountPage> fillStepCount(
    const SimControl&, const StepCountContext&, bool priorNstartChecked);
```

Admission is the bounded MO live-operation profile: same current Project/OPI generation/GuiModel/remesh binding, already completed private page state, context.applicationMode5 and the same operation pointer in page/context, with page.nstartChecked equal to the delivered bool. A caller must represent the control's changed value before declaring the delivered event, as with the existing meshing index contract. Missing/partial/native-base-pending pages cannot be created or bypassed by this call. Expired/replaced OPI must fail before dereferencing Operation or changing its flag. Diagnostic expiry is not itself an observed native null-QPointer return; retain that distinction.

After those metadata checks:

1. Store actual binding.operation.nstartGiven as checked0/1. Mark OperationFlagStored.
2. Only now query the bound model's current simCtl and require the same sourceControl identity as the existing private session. Null/replaced source returns UnsupportedSnapshot at OperationFlagStored without dereferencing a stale SC pointer or undoing the flag. Other original/candidate exceptions should propagate unchanged with the earlier flag store retained; do not relabel them success.
3. Refill from that actual SC and the current qualified StepCountContext, passing the current delivered checkbox value to the new overload. Assign only the step-count page. Return Complete/pageRefilled. Other pages/remesh/files stay unchanged.

Do NOT call pageState() as the early admission helper: it queries model.simCtl before returning. That would move the SC prerequisite before the source's operation store. Inspect the existing owner's metadata/page-complete/sourceModel tokens first, without a simCtl call, then query SC at step2. No constructor-stage completion flag changes follow.

currentContext.primaryDieObjects is the existing explicitly qualified current eligibility-result profile, not a newly recovered QObject/label producer. It must be current for this callback, rather than blindly reusing the initial vector after model changes. The private value method models its supplied result and scalar/ID projection; actual tail model enumeration, labels, signals and reentrant widget effects remain outside this admitted component. Native non5/null-OPI no-op branches are source-known, but the proposed bound method need not infer them from unrelated context or expired diagnostic metadata.

## Exclusive source/test proposal

Worker source set after root release:

- existing src/engine/simcontrol300.cpp: prior-checkbox fill overload/wrapper only;
- existing src/engine/simcontrol300_owner.cpp: guarded bound toggle/store/current-SC/refill method;
- NEW tests/mdt/test_simcontrol300_nstart_toggle.cpp;
- new implementation receipt.

Root/shared owner: include/m2m/engine/simcontrol300.hpp overload declaration and include/m2m/engine/simcontrol300_owner.hpp method/report declarations. No CMake, public loader/factory, native Qt runtime, IO, Spring or meshing edits are needed. The old C ABI and public PrivatePaletteUnavailable/6504abb0 gate remain intact. The actual public Session factory has no completed pages, so this method must refuse it rather than create settings.

## Focused test contract

1. Existing actual Project/Operation/GuiModel/StagedEditor300 private-page fixture with a qualified writable class. Set cached checkbox true as delivered, SC.nstart=-7, operation.startStepNo=10 and unsaved page numerics. Toggle stores given1 immediately, leaves startStepNo10 and SC unchanged, refills start6 (source bug), steps/increment from current SC and primary die from the supplied current eligible list. Toggle false stores0 and refills from SC; other page/file effects stay absent.
2. Same bool on a new explicit callback occurrence still refills current SC; do not add an equality shortcut. Include INT32 wrap/clamp and a changed current primary list with fallback0.
3. Starts-from-key retains delivered checkbox history, rather than fresh-fill false. Missing/nonwriting class forces projected checkbox false while leaving the just-stored operation flag true. Existing fresh two-argument Fill results remain unchanged. Save's later given/startStepNo behavior is not moved into the callback.
4. Retain the actual private page pointer while model/OPI remain live, then make current SC unavailable through the owned model step lifetime. Callback must commit the operation flag first, stop at OperationFlagStored, and leave cached page values unchanged without touching a stale SC. This is a candidate prerequisite guard, not a claim that native null-SC dereference gracefully returns.
5. Wrong binding, replaced/expired OPI generation, missing private pages, partial construction and mismatched delivered-checkbox/context fail before flag/model/file effects. Public base-pending factory remains guarded. Reuse existing OperationInfoGuard fixtures, not synthetic native-owner observations.

No implementation or test execution occurred in this lane.

## Read-only evidence receipt

- `[140ecf910,140ecf977)` callback SHA-256 `c42e04dbe9cf19df81308a1844308f42e0b1fdb2c35f4c10adad87bfeeed8916`.
- `[14005a2c0,14005a2d7)` byte setter SHA-256 `5c8db3fb96328f5fe89483750946d171aba51735f2a339639c3d628b4cd50439`.
- `[140ebb110,140ebb3dd)` complete refill SHA-256 `f7f7cb6e239a1437f6930d385b4534c5f62beeb0ebc181e305efb23c889788c5`.
- `[140130f90,140130fd5)` current-model SC getter SHA-256 `f85c714b94efce9d22960b1d2321e0621f827cd1a4b31a59b4f43a08e72b220f`.
- `[140c7b2d0,140c7b301)` guarded step+750 provider SHA-256 `2f5b78f5174075403916e79492d514c1eb0aab38b00ffd322bcef44b73bc2ef2`.
- Retained ebb110 decompile `412fed1d88d31618ce9a4f8c1b5194d228e978876f5d89eab086afc89e16cbbf`; ebb3f0 tail decompile `5be00876daf83e238d4a1700a34449b6b51398faa178049e55736bfb7e9e5754`.

Bounded DUMPBIN used the pinned BuildTools14.44.35207 tool, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Only analysis tools executed; no raw artifact outside this report was written.

Read-time identities: simcontrol300.cpp `5c61460e70552a8e58ec7c5e1a3ce0aed98a45308b5948f0d1efc60626ec4a3a`; owner.cpp `aca6874a8cc8d77b1469d4a4791d6504308a753e83139a8aad6ddd63c455e9d7`; owner.hpp `6f24f1165f65996a7f4d89c76b6006e8ca4802aab7230bae6f50cf0778653b33`; existing MO tests `9ad474e7512ed120f45e0a8399a7414a72ba3595252aa363090034c6c0ba57aa`; owner tests `c2b32ede8d343108bbbbf30ac8ec51e4370d9195ee031f87253f8afaabd81e58`. No reconstructed executable/deployment was inspected or produced.
