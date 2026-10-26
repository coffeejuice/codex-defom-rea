# CP140: bound explicit-solver visibility toggle

The next complete private control cut is the explicit-solver radio toggled(bool) handler. It shows or hides the same eight controls and returns without reading SC or solver parameters. This can be projected on the existing bound Iteration page while preserving the separate SOLMTD6 Fill/Apply refusal. Visibility is not evidence for the four edit values.

Only this report was written. No source/build/GUI/original execution/REA database change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Evidence is retained native source/slot metadata and bounded immutable-PE DUMPBIN, not observed radio delivery.

## Complete native event

Slot-map index65 is on_m_rbtnDefSolExplicit_toggled(bool); dispatch1410dbd0f calls140ed0660 at1410dbd23. Complete native `[140ed0660,140ed0814)` SHA-256 is `c0b4b92840b3561bce23488c66b64ce0d8a63dcdf8c58a8508ee9537d0b952ef`.

At140ed0672..674 it tests the delivered byte. True calls show at068d,06a6,06bf,06d8,06f1,070a,0723,073c; false calls hide at075a,0773,078c,07a5,07be,07d7,07f0,0809. Both use the ordered UI offsets `[b18,b28,b38,b48,b10,b20,b30,b40]`, then return0813. Exact PE imports independently resolve141e0bfb0 to `?show@QWidget@@QEAAXXZ`,141e0bbb0 to `?hide@QWidget@@QEAAXXZ`.

There is no SC/model/dimension query, solver/radio write, parameter count/data read, numeric conversion, default initialization, memory allocation, dialog, viewer or file operation. The callback never populates the four line edits. A same-bool delivered occurrence executes again.

## Minimal private interface and root field

Root owner.hpp method:

```cpp
ApplyResult StagedEditor300::explicitSolverToggled(Editor300Binding, bool checked);
```

Root IterationPage field, appended with an unknown default:

```cpp
// Common last reached show/hide argument for the eight explicit controls;
// private value projection, not physical QWidget visibility/ancestor state.
std::optional<bool> explicitControlsVisible{};
```

One common optional projects the complete normal-return tuple because every body branch applies the same argument to all eight. It does not model partial native exceptions between widgets or independently observed physical bits. Root should initialize it false in fillIteration3D: native140ec19f0 unconditionally hides these eight controls before selecting/formatting radios. Every currently admitted solver is non6, so the completed current Fill profile ends hidden without an inferred native event. Do not enable solver6 Fill merely to initialize this field.

Bound method admission: same live Project/OPI generation/GuiModel/remesh, completed private pages/source-model/remesh tokens and a represented supported selected-radio value. Checked=true requires cached IterationPage.solver6; checked=false requires one of the currently admitted nonexplicit choices0/1/2/4/5. The caller first represents the changed selected radio before declaring toggled delivery. Other solver values/mismatched bool refuse before projection. This is a candidate event-profile guard, not native validation.

Then assign only explicitControlsVisible=checked and return Completed. No pageState/simCtl/sourceControl/dimension/parameter query, no CG normalization, no count4/default assignment, no other control/text change or Apply call. An absent/replaced actual model step must not create an extra prerequisite when the same live model/OPI owner and completed private controls remain. The field records only the existing private value boundary; physical show/hide/layout/events, actual signal delivery and reentrancy stay outside this increment.

## Solver6 remains guarded

Current fillIteration3D excludes solver6; applyIteration3D also excludes it before model stores. Retain both guards. Native solver6 Fill separately formats SC.params0/1 with QString::number(g,6), but formats params2/3 through signed32 CVTTSD2SI then QString::number(int). Native Apply instead reads **all four** edit texts through QString::toDouble, storing each sequentially after count4. Last-two integer formatting does not imply integer-only Save conversion. Unknown/nonfinite/overflow/MXCSR/text/setup-history domains are not established by visibility alone.

For a later finite solver6 Fill/Apply proposal, new explicit edit-text state and its actual population/conversion order would be needed; a post-Open selection from non6 also needs the preexisting setup/remembered texts rather than fabricated defaults. CG normalization is already recovered and is not this event. Public mode300 palette/6504abb0 and completeForPublicSave=false stay unchanged.

## Exclusive files and focused tests

Worker: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_explicit_toggle.cpp`, implementation receipt. Root owns method declaration, IterationPage optional field and single fillIteration3D false assignment; root may clarify the header's old explicit-callback exclusion comment without relaxing SOLMTD6. No CMake/lifecycle/public or worker Apply edit.

Expected target `m2m_mdt_simcontrol300_explicit_toggle`, CTest `m2m.mdt.simcontrol300_explicit_toggle`. Focused cases should check:

- Initial visibility=false for every currently admitted solver, independent of actual later explicit event delivery.
- True/false delivered events project only visibility; selected-radio cache is already changed, while raw live solver/count/four parameter bits, all edit text, other controls/pages/OPI/remesh and I/O stay untouched. Include unconsumed nonfinite parameter bits after initial Fill.
- Cache assignment alone does not deliver; repeated same-bool event reasserts the common projection without populating/normalizing hidden edits.
- Missing/replaced actual step adds no SC/dimension/parameter prerequisite and mutates no old/replacement model state.
- Wrong cached radio/bool/unsupported solver, wrong binding, partial/unconstructed owner and expired OPI generation refuse before projection.
- After a visibility-true event, existing solver6 Apply still returns UnsupportedSnapshot before its model stores; existing solver6 initial Fill remains refused. Qualify this as the retained candidate boundary, not original native solver6 behavior.

Current source has no bound explicit toggle or visibility field. Existing tests guard solver6 and exercise CG Fill/Apply; those are not displaced by this proposal. TIMEID/DB-step tail, definition dialogs, operation-name/tree refresh and selected constructor palette history remain independent guarded work.

## Retained source identity

Iteration Fill decompile140ec19f0 SHA-256 `e9916be99bda0baf03df22d5d9fd3f56d5463292df0f8389910e52d01e086e7b`; Apply decompile140ec3130 `8f49f547a6d720a0047d325140949e530aeb4cdd0099eeabeebbc154cd89526d`. Read-time candidate simcontrol300.cpp `e392269647a84886214eb2d07608ec08fc96699187cafd7f899d163d35914137`; owner.cpp `dbb1962a5524c2e958789c374af1cb8baf9f132dd1a44af2ee19d0c59187d918`.

Bounded DUMPBIN used pinned BuildTools14.44.35207 SHA `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, banner14.44.35228.0. Callback span follows PE unwind bounds/final return. No raw capture outside this report was written; no reconstructed executable/deployment was inspected or produced. Source requires root assignment/shared declaration and frozen-tree release.
