# Next editor300 settings/control-enable increment (checkpoint 59)

## Recommendation

The smallest source-closed control-enable branch is the first MDS reference
coordinate group: native `toggled(bool)` delivery from `m_rbtnCoordR1` (UI
record offset `0x6b0`) to `groupBox267` (`0x6b8`) through `setEnabled(bool)`.
The setup contract gives the receiver an explicit initial disabled state, while
the ordered internal-connection receipt supplies the exact signal, slot, and
connection type. This branch is smaller than resolving any complete effective
enabled state through the QWidget parent tree.

Production behavior already exists in `EditorSetupState`: the FreshNativeTree
profile retains `0x6b8.enabled=false`; the native connection table contains
`{0x6b0,0x6b8}`; and `receiveInternalToggled` applies the delivered bool to the
receiver's direct enabled state only when that direct state is qualified. The
existing tests cover the second reference coordinate group in a page-handoff
case but do not directly assert the first group's ordered delivery and direct
state transition. Therefore the next concrete increment is a focused test in
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`, with no production
change unless that test exposes a mismatch.

## Evidence and source mapping

- `analysis/wave3-editor-constructor-prefix-contract.json` records all 14
  internal `toggled(bool) -> setEnabled(bool)` connections as AutoConnection,
  including `{0x6b0, 0x6b8}`. It also orders connection registration after
  setupUi and retranslation, so registering it does not itself deliver a toggle
  or synchronize the receiver.
- `analysis/wave3-editor-setup-remesh-radio-contract.json` qualifies only the
  two radio groups' checked states, and explicitly disallows inferring
  effective enabled state through the whole parent tree. It does not conflict
  with the direct setter on `0x6b8`.
- `source/apps/moproj2mst/src/engine/editor_setup.cpp:228-231` names the
  controls `m_rbtnNodeR1` (`0x6a0`), `ref1NodeSpnBx` (`0x6a8`),
  `m_rbtnCoordR1` (`0x6b0`), and `groupBox267` (`0x6b8`). Lines 730-732 retain
  the node checked setter, spin maximum, and explicit `Enabled(false)` setter
  at original address `0x140eee4bc`. Lines 800-808 provide the relevant
  connections; line 1262 onward binds the guarded fresh native profile; and
  lines 1366 onward implement event delivery.
- Existing `test_editor_setup.cpp:537-550` checks the explicit fresh-profile
  defaults. Its test `editor_setup_handoff_preserves_reached_toggle_deliveries_without_synthesizing_radio_changes`
  (around lines 644-655) verifies a node-radio delivery and the second
  coordinate-radio delivery; it asserts that a delivered toggle is not itself
  a checked-state mutation. There is no matching direct assertion for the first
  coordinate receiver.
- The original executable qualification and static contract are the retained
  DEF_GUI_MO_64 identity `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
  Original probes 003/004 did not interact with this control, so they do not
  establish a physical user click or complete GUI enablement.

## Exact focused acceptance case

Use `EditorSetupState` with `SetupStoppingProfile::FreshNativeTree`, advance to
`SetupProgress::ConsumedControlsTranslated` using the already required model
and translation profiles, then install the internal-connection projection.
Before delivery assert `0x6a0.checked == true`, `0x6b0.checked == false`,
`0x6a8.enabled == true`, and `0x6b8.enabled == false`. Deliver
`receiveInternalToggled(0x6b0, true)` and require exactly one record
`{sender=0x6b0, receiver=0x6b8, value=true}`; assert both
`0x6b8.requestedEnabled` and its qualified direct `enabled` field are true,
while both radio checked states are unchanged. Deliver false and require the
same single ordered route and the direct receiver bit to return false. No
coordinate descendants' effective enabled states, GUI click, automatic
connection behavior, callback emission, or widget-area completion should be
asserted.

The setup progression is a test prerequisite, not part of this case's claim.
If the intended gate is only the narrow control projection, the existing test
helpers can be reused to reach it without constructing a project or a page.

## Ownership and limits

Proposed sole write owner: editor300 test owner, file
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`. Root owns build and
integration. Focused target: `m2m.mdt.editor_setup`; acceptance is the new case
plus the existing focused target and later root integration. No tests/build
were run for this audit.

This isolates a direct parent-group `setEnabled` call reached through a
source-mapped connection. It does not resolve QWidget inherited enablement,
all control defaults, arbitrary settings, interactive edits, or public
Open-to-Save parity. The PaintDevice cleanup prefix in `editor_base.cpp` is a
separate constructor dependency; it does not supply or change this setupUi
connection contract.

Read-only audit only. No source, build, GUI/app-control, or mutable REA changes
were made. Token measurement unavailable.
