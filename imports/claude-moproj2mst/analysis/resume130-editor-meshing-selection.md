# CP130: finite stateful private meshing selection and Save

The private meshing page now distinguishes the last loaded form from the actual combo index. Explicit activation/Save commits the old temporary, always reloads a supported requested target even on the same row, and retains a committed old temporary if a later target is unsupported. A distinct mutable Apply copies committed records once before the same-owner remesh global tail. The legacy const first-form adapter remains separate. Full public Editor300 construction/palette is still guarded.

## Source qualification and exclusive scope

Controlling handoff `analysis/resume129-editor-meshing-selection.md` SHA-256 is `ac8a95391b93e0106ad62893303f60d8d67631a590475525edfaf41ce97b7192`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The handoff pins activated dispatch1410db93d, Fill explicit callback140ec1110, complete handler140eceb60..ecf2cd, Save-currentIndex140ec138a/13a0 and the finite/nonfinite absolute-value helper boundary.

Worker edits are only existing src/engine/simcontrol300.cpp, new tests/mdt/test_simcontrol300_meshing_selection.cpp, and this new receipt. Root separately owns shared simcontrol300.hpp fields/declarations and the private lifecycle callsite. No worker shared-header/lifecycle/CMake/public-factory edit occurred.

## Implemented owner and event sequence

MeshingPage.objects remains the sole ordered four-scalar temporary owner. No second remembered vector or alias to live object storage was introduced. Existing first* fields are the current visible controls; names remain compatible. Root-provided storedObjectIndex and comboObjectIndex track native editor+100 and UI+9b8 independently.

Fill retains prior eligibility/sorting and initializes nonempty pages through InitialLoad from stored-1/combo0, ending0/0. Empty pages remain-1/-1. InitialLoad does not read stale controls. Programmatic setMeshingComboCurrentIndex changes only actual combo index, with no implicit activated callback.

transitionMeshingObject3D requires a valid event and requested index equal to the supplied current combo before old effects. A matching negative index is the native no-op before old controls/model queries. Positive calls require the actual supplied3D model, in-range requested/stored indexes and the InitialLoad stage. Positive out-of-range or a fabricated empty-vector/null-native-array equivalence is not admitted.

All old visible numeric conversions and spin bounds are preflighted before mutation. The finite stateful profile uses existing Qt value adapters, so a failed conversion yielding finite0 retains that adapter behavior; it does not borrow Spring's narrower ok guard. Nonfinite old values and an unqualified spin value outside0..1e9 are excluded before old mutation. This guards native nonfinite CRT/FP behavior rather than claiming to implement it.

On an admitted old form, interference absolute value/sign, ratio, absolute tolerance and spin-as-double are stored in their source order to the OLD temporary. Report.previousCommitted becomes true / stage PreviousCommitted. Only then is the target finite profile checked. An unsupported target retains those old values and leaves target controls/stored index unchanged. Unvisited record bits require no all-record finite guard.

Supported target reload applies interference sign radio, abs(interference)/ratio/raw-tolerance g6 texts, CVTTSD2SI plus explicit spin clamp, then stores requested index. Same row always reloads: report.sameRow is diagnostic and never a fast return. Success is Complete with controlsReloaded=true. Old-preflight failure is Preflight with previousCommitted=false; later target failure is PreviousCommitted with previousCommitted=true and controlsReloaded=false. These reports are reconstruction boundaries, not native application errors or complete Qt event/allocation traces.

The new applyMeshingSelection3D validates its existing model/global method admission, invokes SaveCurrentIndex using the actual combo value, copies the committed four-field entries to still-present live objects WITHOUT converting the freshly loaded target a second time, then updates the supplied same RemeshState. Missing live objects skip their own copies and continue. The common copy/global helpers preserve prior field writes and CP128's disabled-global behavior.

Legacy applyMeshing3D retains its original const first-form conversion semantics, including the existing broader value-adapter scope. For nonempty pages it now refuses changed selection context instead of silently writing form0. Existing Fill-created first-form callers have0/0 and remain admitted. New stateful calls use their distinct name; no overload silently changes prior callers. Root has changed the private lifecycle callsite to the mutable name; its sourceModel/sourceRemesh guards and later temporary clear remain root-owned.

## Focused coverage

New target `m2m_mdt_simcontrol300_meshing_selection` / CTest `m2m.mdt.simcontrol300_meshing_selection` contains8 Release-active cases:

- Actual3D model, sorted10-before2 records, initialized0/0, activation0->1->0, previous temporary edits and later control load, no model mutation until Apply, unvisited NaN payload bits retained.
- Same-row activation and Save both commit/reload precision-sensitive fields; negative zero remains in the temporary while radio reload becomes nonnegative, then a later callback commits positive zero.
- Programmatic move alone changes no temporary. Mutable Save commits the prior loaded row, reloads current target, copies target raw fields without premature/double conversion, and preserves disabled-method/global fields.
- Matching negative current index skips poisoned old controls and writeback while Save still copies remembered records.
- Unsupported target after old commit yields PreviousCommitted without target-control/stored-index changes, and prevents later model/global effects.
- Save/delivered-index mismatch, wrong InitialLoad stage, invalid indexes, all old nonfinite/spin controls, wrong dimension and enabled-invalid method remain guarded before old effects.
- Legacy const first-form call keeps its page unchanged and refuses changed context; the explicit mutable path handles that selected row.
- Finite CVTTSD2SI overflow clamps to spin0 on load while the raw temporary remains unchanged until commit; actual removed-object lookup skips to later model copies/global update; empty Fill/Save retains-1/-1 and consumes no controls.

These are direct private page/model calls, not native QWidget/control delivery or full public Session acceptance. They create no file IO or test scratch. No tests were executed by this worker.

## Static preflight and release

Compared the production diff against build/wave3-checkpoint129-verified/source.zip (+108/-25). Inspected the new transition, Fill and both Apply paths; shared signatures match root's header. Checked all new quoted/defmodel/Testkit include targets, valid unique TEST_CASE identifiers, supported CHECK/CHECK_EQ/REQUIRE macros, one main, no C assert/REQUIRE_EQ or raw NUL source, balanced braces and whitespace. Actual object removal uses the existing maintained list API, and deleted pointers are not dereferenced. git diff --check reported no errors.

No compiler/build/CTest, GUI, original execution or mutable REA operation ran. Static preflight is not a compile/test pass. Root owns grouped validation/archive binding. Source/test are frozen for independent review and build:

| Worker-owned file | SHA-256 |
| --- | --- |
|source/apps/moproj2mst/src/engine/simcontrol300.cpp|`a657daa78e1a7b9c3618e37422cf1d8e29a7d28f368fcdcbdae16bc166a9bccb`|
|source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp|`6b7226249296e8e89b70bdcdf5a996dd58fe8ca58b7043241a9c95409e57e6be`|

Read-time root dependencies: simcontrol300.hpp `f014239ace1dc9633dec3033aa3744d177f188c4fd8950196ed6b4b3b234fea7`; simcontrol300_lifecycle.cpp `245fe59467e0852f6a34c540a975dcf5fe7e564922f80cad3a00e6cfdb3d6854`. These are shared-owner identities, not worker edits or a tested candidate. No reconstructed executable/deployment was inspected or produced. Nonfinite native absolute-value/FP effects, full Qt control ownership and public editor construction remain separately guarded.
