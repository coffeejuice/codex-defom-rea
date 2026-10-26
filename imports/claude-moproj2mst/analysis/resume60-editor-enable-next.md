# Next editor300 enable route after Coordinate R1 (checkpoint 60)

## Selected edge

The next narrow source-closed edge is MDS Reference 2's node radio to its node
spin box:

`m_rbtnNodeR2 (0x738) --toggled(bool)--> ref2NodeSpnBx (0x740) --setEnabled(bool)`

Original setup evidence identifies the controls, explicitly checks the node
radio at `0x140ef0036`, sets the spin maximum to `999999` at `0x140ef01b1`, and
registers this exact AutoConnection in the internal connection list. Under the
qualified `FreshNativeTree` profile, the direct radio/spin enabled state is
true initially. The current candidate already models the connection and direct
enabled state, but no current `test_editor_setup.cpp` case delivers a toggle
from sender `0x738`; existing deliveries exercise R1 node false, R1 coordinate
true, R2 coordinate true, and unrelated remesh controls.

This is a small test increment after the Coordinate R1 case: verify the R2
node-spin receiver follows supplied true/false toggle deliveries without
changing either radio's checked state. It does not require new production
behavior or establish a real click/signal emission.

## Evidence

- `analysis/wave3-editor-constructor-prefix-contract.json` records
  `{sender_ui: 0x738, receiver_ui: 0x740, signal: 2toggled(bool), slot:
  1setEnabled(bool), connection_type: 0}` and places registration after
  setupUi/retranslation. Connection installation itself does not synchronize
  current state or imply a signal delivery.
- `analysis/wave3-editor-stopping-partial-node-contract.json` retains the
  source-derived fresh MDS radio/spin profile and expressly limits it to the
  qualified native profile; it does not claim full Qt visibility or arbitrary
  constructor effects.
- `source/apps/moproj2mst/src/engine/editor_setup.cpp:247-248` maps `0x738`
  and `0x740` to the control names. Prefix calls at lines 735-736 record the
  checked and maximum setters; connection table line 806 binds the route;
  `bindStoppingControls` around lines 1262-1278 supplies its fresh direct
  enabled state; `receiveInternalToggled` around lines 1366-1378 applies the
  delivered bool only when the receiver's direct state is qualified.
- `source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp` has a stopping fresh
  profile test and a handoff test which delivers `0x6a0` and `0x748`; no test
  currently references `receiveInternalToggled(0x738, ...)`.
- `analysis/wave3-editor-constructor-prefix-contract.json` SHA-256:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`.
  `analysis/wave3-editor-stopping-partial-node-contract.json` SHA-256:
  `7AC3719501898940FBB88B3257B524354EEBA0AFEA18BDB489289E005C73049E`.
  Candidate `editor_setup.cpp` SHA-256:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`.
  Candidate `test_editor_setup.cpp` at audit time SHA-256:
  `CB723342D8C453605F803BE974FB86A75A7A17ADE5A0839B3F6E836E15494659`.

## Focused acceptance contract

In an isolated `EditorSetupState`, bind the fresh combo, spin and stopping
profiles; translate consumed controls with the already qualified
no-active-translator profile; then install internal connections. Assert
`0x738.checked == true`, `0x740.enabled == true`, and `0x740.maximum == 999999`.
Deliver `receiveInternalToggled(0x738, false)`: require exactly one record
`{0x738, 0x740, false}`, set the receiver's `requestedEnabled` and qualified
direct `enabled` values false, and preserve `0x738.checked == true`.
Deliver true: require the same single route with true, restore direct enabled
state, and leave the sender checked. This tests the qualified direct QWidget
state projection only. Do not infer radio-group exclusivity changes, descendant
effective state, GUI click delivery, parent propagation, or full constructor
completion.

## Ownership and bounds

If selected for implementation, assign only
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp` to one editor300 test
owner; root owns build and integration. Suggested target is
`m2m.mdt.editor_setup` plus later combined regression. This report changes no
source or tests. No build, GUI/app-control, or mutable REA action was performed.
The PaintDevice prefix does not establish or alter this independent setupUi
connection contract. Token measurement unavailable.
