# CP135: private delivered reaction-rate radio callback

The next bounded state transition is an explicit delivered ACVCOF reaction-rate radio click on the existing bound private editor. Current Fill already projects this callback while initializing Environment; there is no post-Open event API. A new bound method can preserve the same actual SC and owned temporary, without refilling other controls or enabling the public constructor.

Read-only evidence phase: no source/build/GUI/original execution/mutable REA work. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The constructor connects clicked() at UI+e70/e80/e78/e88 to wrappers140ecf7c0/7e0/810/840. Slot-map indices41..44 and bounded DUMPBIN prove these call140eccf10 with constants0,1,2,3. This is an explicitly delivered click, not an automatic response to changing a cached field or programmatic setChecked.

Complete callback `[140eccf10,140ecd2e5)` SHA-256 `9b53369cd493e1d3cad521a65293ab114f46edaa8dabc0e09a8b5c0b6834da38` first selects the four radios and calls setEnabled for the constant edit/unit and definition control. Selection0 enables constant edit/unit and disables definition;1..3 do the opposite. Only at140ecd1b5 does it query current SC. Null returns without the later index or curve stores. Nonnull then writes editor+15c=3 atd1d1. Selection0 writes SC+5b4(ndim)=0 atd211, SC+5c0(var0)=0 atd220, then temporary(editor+c8+3*8)+10=0 atd243. Selections1/2/3 write SC.var0 and temporary.var0 to3/12/1 respectively. They do not set live ndim. No branch changes temporary ndim, axis/count/values storage, numeric text, other curve metadata or files. There is no same-choice early return.

Retained decompile `wave2-save-diffs-300-support-140eccf10.c` SHA `72040eec60e4d1d5513271b0e1625d46441c812644cf56c08cc419d1c52f5ba4`; retained complete assembly `wave3-editor-acvcof-owner-140eccf10.asm` SHA `19561d61b8390d0bd57e7676ad38b30e4c8c1732a7082f566fa36857c454b058`. Wrapper hashes (end-exclusive) are:7c0..7da `16101f471312fab4072ef7d5c4a80d95c9920ed9a0606f4eaf59670d96c4ca1a`;7e0..7fd `f2ee551faef761125541320456350945a0cfb2ce898e7abbebe81f9a94d7d6d1`;810..82d `36e89612c3f24b2cd108a3d5897a53fb8c0bb079fbee10dcd657ccb0f2d1fc3c`;840..85d `844014035c7b80142df10f66c55f44180c02430618ecd8518810288057066322`. Static tool is pinned BuildTools14.44.35207 DUMPBIN, banner14.44.35228.0, SHA `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.

Root shared API:

```cpp
enum class EnvironmentReactionStage { Preflight, ControlsProjected, Complete };
struct EnvironmentReactionReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    EnvironmentReactionStage stage = EnvironmentReactionStage::Preflight;
    bool controlsProjected = false, modelUpdated = false;
};
EnvironmentReactionReport StagedEditor300::reactionRateClicked(Editor300Binding,int selection);
```

Admit selection0..3, same live Project/OPI generation/model/remesh, completed private pages and the existing distinct Environment temporary3 aliased by EnvironmentPage.curves[3]. Model only the existing EnvironmentAcvcofControls value projection: this does not certify physical QWidget enabled bits, setChecked signal emissions or reentrancy. Keep metadata preflight separate from current SC lookup. Project controls, mark ControlsProjected, then query actual simCtl; null/replaced SC or an invalid live/temporary alias relationship returns UnsupportedSnapshot retaining control changes but no curve-index/data stores. Such prerequisite guards are candidate admission, not native failure semantics. Do not dereference stale sourceControl. Propagate later source exceptions unchanged. On valid SC write index3, live ndim only for0, live var then owned temporary var, and return Complete/modelUpdated. No Fill or Apply replay, allocation/free or copy-back.

Existing snapshot tests named environment_acvcof_callback_matrix cover Fill's initial projection and initial owner identity; they are not delivered bound events. New tests should cover all four post-Open choices and0→1→2→3→0; preserve actual table pointers/counts and temporary ndim; restore live/current variable after a same-choice event; preserve unsaved numeric strings; controls-first null/replaced SC refusals with unchanged index/data; wrong identity/selection/temp owner refusal and public/partial factory guard. Use actual owned DYNARY storage, not fabricated pointers.

Exclusive implementation proposal: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_reaction_rate.cpp`, implementation receipt. Root owns owner.hpp declaration; no CMake/lifecycle/public changes. Expected focused target `m2m_mdt_simcontrol300_reaction_rate`, CTest `m2m.mdt.simcontrol300_reaction_rate`.

Operation-name is deferred because its complete source tail traverses current main/model-manager/project and tree update after SC/OPI stores. Unit callbacks require the actual global unit/dialog owner. Frame selection consumes actual picking/combo/tab/stack receivers. The title and NSTART paths are already recovered; neither is re-proposed. Actual public mode300 remains at the selected constructor palette witness/6504abb0 guard, independent of this private event.
