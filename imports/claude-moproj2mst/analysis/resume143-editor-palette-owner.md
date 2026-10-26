# CP143: selected palette owner through constructor and Save

No new source-closed actual Session increment was found. The selected editor component and its Session runtime are joined, but the selected native palette getter/data/atomic occurrence is still missing. Keep `PrivatePaletteUnavailable`, the public `6504abb0` refusal, and the separate `140eb5180` Save refusal. The later private callbacks and solver-6 Apply do not supply an earlier constructor owner.

This lane wrote only this report. It performed bounded current-source/retained-source reads, immutable PE hashing and capture reconciliation. No source/test/CMake change, build/test, original execution, GUI control or mutable REA operation occurred. The original EXE is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable or deployment was inspected.

## Actual owner chain and first missing input

`FormingClass::objectTreeFill` calls the mode300 Open slot before assigning `simControlShown_ = &op`. `formingSimControl300Open` obtains this Operation's actual GuiModel and calls the bound Session's `beginEditor300Factory`. The factory checks Project, Operation.project, Operation.engineModel and current manager model identity, constructs one StagedEditor300, binds its WidgetState to the same Session WidgetRuntimeState, and continues its native-base prefix. The early Operation identity-generation guard rejects expiration and address reuse; it does not execute the later native QPointer assignment.

The selected runtime intentionally has no post-Open `privateConstructionInputs.defaultPalette`. `EditorBaseConstruction::resume` therefore retains its private-allocation/QObject/private-widget prefix and stops at `PrivatePaletteUnavailable`. The Session callsite currently requires exactly that issue. Loader then retains `not_ported(0x6504abb0)`, before setup/page construction. The startup default-palette aliases and SplitView profile are different occurrences; they cannot be copied into the selected editor's inputs.

Native order, retained in `wave3-editor-temporary-ctor-140eb26f0.asm`, `wave3-editor-native-owner-140130740.asm` and the parent captures, is decisive:

1. `140eb2721` calls inherited `130740`; its `130763`/`37798a` path invokes QWidget and the private default-palette constructor.
2. Only after QWidget returns, `130791` initializes editor+38, `130857` assigns the supplied OPI, and `130871..1308a6` resolves/assigns its model.
3. Only after inherited/member construction, `eb2785..eb27ee` allocates/zeros the UI storage and calls setup.

`constructPageState` refuses with `NativeSetupPrerequisite` while an existing base has not returned its application wrappers. Explicit private owners with no native base can exercise recovered page projections; those fixtures do not pass this actual Session gate. Physical setup controls, UI slot generation and source QWidget ownership remain later dependencies.

## What 6504abb0 needs from that same occurrence

The verified native default-palette sequence is:

| Edge | Native effect |
| --- | --- |
| `6504abbe` | Invoke QApplication palette getter. |
| copy `6504a8bd..a8c6` | Load source data D, store D in returned temporary T, retain D once. No detach. |
| `6504abc3..abcb` | Read temporary data and store D in private destination P=selected private+170. |
| `6504abce..abd6` | Deref the temporary's D, then branch on its actual returned bool. |
| zero branch `abd8..ac08` | Destroy 60 brushes/free data before the tail. |
| `6504ac10`, `ac17` | Store destination word0, then retain destination D; this retain's bool is ignored. |

The consumed atomic is raw uint32 at **D+0**, not the palette wrapper word8, a widget counter or a C++ shared-owner count. QtCore ref/deref are raw32 LOCK ADD +/-1 followed by SETNE. A temporary deref of input1 commits zero/destruction-pending; later destination retain cannot justify skipping that branch. Host copies/destruction do not supply native atomic effects.

Existing runtime support already distinguishes Unknown/null/present app_pal and requires the exact QtGui module slot and canonical data control block. Its getter returns one retained native temporary token; retries/host copies must not replay getter+1. The CP81 SplitView partial pointer/token cursor belongs to a different WidgetState and call frame. Its data owner/token cannot stand in for the selected EditorBase receiver. The historical complete-value EditorBase path directly projects a palette before font; feeding it startup or post-return values would skip this ledger.

## Retained witness remains incomplete

Editor004 capture SHA-256 is `e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced`; script SHA-256 is `0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203`. A fresh recursive check still finds twelve references to selected D=`0x3e3edb0`, all with exactly `{data, object, word8}`. The script reads wrapper+0 and wrapper+8, not uint32[D]. The selected private-return snapshot is after this destructor/retain sequence. It does not identify the source wrapper or the current temporary-deref input/outcome.

CP124/130's eight retained editor/splitter captures remain the controlling qualification. Splitter003's app_pal/dataRefs observations are in another process and later consumers. They cannot be joined numerically to editor004. Source-only work through CP142 adds no selected-frame observation. Native address equality, allocation count1, startup aliases/word, later refs166/167/165 and widget counts174/760 do not establish this selected current-sequence atomic or canonical allocation generation.

The exact missing witness is one selected Project/Operation/editor/private constructor frame, with loaded-module identity and these edges:

- At QtGui+4abb0, require caller+5e8d9 and P==selected private+170.
- Identify the getter's chosen source O and T through getter+16f70/copy+4a8b0, with the selected caller chain. Require D==*O==*T. Full wrapper API admission additionally needs O+8; this private data-only slice does not consume that word.
- At +4abce, require RBX==P and RCX==*T==*P==D. Record current raw uint32[D], already including getter retain, with its mutation sequence.
- At +4abd4, record the actual deref result for this frame/D, with supported execution exclusion or retained relevant atomic/interleaving qualification. An unsynchronized read before the atomic is insufficient by itself.

The observed native D/source occurrence must then map to the **actual** Session/runtime canonical data owner. Historical addresses are evidence labels, not portable owner tokens. A complete allocation/ref-mutation ledger could supply the same proof, but none was found in this retained set. This lane does not request or perform a new runtime observation.

## Save is a separate delivery boundary

Even palette completion alone would not enable public Save. Current `FormingClass::updateDataInWidgetArea` first checks the shown Operation, then refuses mode300 at `140eb5180`. Actual Open currently throws before committing that shown Operation. No production path calls the private all-page Apply adapter from this guard.

Retained `wave2-save-diffs-300-support-140eb5180.c` obtains the current SC, reads the actual navigation currentRow, invokes `140eca080`, then `131110`; it publishes the SC name through the bound OPI, updates project/tree state via `eb5250`, and performs the later control/view tail. The dispatcher capture `wave2-save-diffs-300-support-140eca080.c` additionally establishes current-SC/busy guards, stored-page dispatch (or all pages), temporary cleanup, button projection, unpick/recovery calls and busy-state release. These source excerpts establish order; they do not establish that the candidate actual selected receiver/setup/delivery occurred.

The private `applyInitialEditorPageState` checks current model/SC/remesh identity and uses recovered page Apply order, including partial validation effects. Its completion remains explicitly insufficient for public Save. New private solver-6 parameter stores do not supply native QWidget, UI/navigation ownership, final refresh/publication or the Save tail. No callback or symbol-registration-only patch closes these earlier gates.

## Conditional implementation boundary

No source write set is justified now. After the missing same-owner witness/admission is reviewed, the smallest integrated prefix would retain one actual getter token on EditorBase, store its same canonical data into this WidgetState's partial palette slot once, and stop before unknown temporary deref. It would own `editor_base.hpp/.cpp`, focused `test_editor_base.cpp`, and the narrow Session expected-pending-stage check; runtime changes would be needed only for a reviewed data-only admission API. It must not mark PaletteConstructed or advance font/counters/setup on a partial pointer.

Focused actual-owner cases would reject wrong receiver/runtime/OPI generation/data control block; retain getter+pointer-store once on retry; admit a late count already including getter+1 without replay; and commit zero-deref destruction-pending. Nonzero would permit only the word0/destination-retain tail. Public loader/Save refusal remains until their own predecessors and delivery effects close. Another static scan of this unchanged capture set is not an executable substitute for the missing occurrence.

## Read-only receipt

Six current files were byte-compared with `build/wave3-checkpoint142-verified/source.zip`; all match. Under `source/apps/moproj2mst/`:

| File | SHA-256 |
| --- | --- |
| src/engine/conversion_session.cpp | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| src/engine/editor_base.cpp | `20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817` |
| src/engine/view_manager.cpp | `4f11d60d40ccef3730f4cd7575af53ebf60af1af8581b76fb7d732e64c69e805` |
| src/engine/simcontrol300_owner.cpp | `80a0b704fe3f5610fe2dbf3ccfb84957d02e416dad20352dc74128372f767fba` |
| src/engine/loader_l3.cpp | `5899cd9e9903a1061e46c50d07ff0c43943b184cb85fe20085901f8751d4d939` |
| src/ops/family.cpp | `db40adb7365f1275b5e024d301066c533c1b13c41b195630f988b8348eb72217` |

Original QtGui SHA-256 reverified `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Recomputed half-open PE ranges:

| Range | SHA-256 |
| --- | --- |
| QtGui6504abb0..6504ac26 | `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399` |
| QtGui6504a8b0..6504a8fd | `a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b` |
| QtCore67001aa0..67001aa8 | `7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de` |
| QtCore67001ac0..67001ac8 | `06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6` |

This report reconciles CP120/124/130 with the CP142 source freeze; it claims no new runtime palette value, constructor completion or public acceptance.
