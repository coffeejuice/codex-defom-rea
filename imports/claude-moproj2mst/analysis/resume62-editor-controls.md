# Next editor300 control-enable route (checkpoint 62)

## Selected path

After the R2 node-radio route, the smallest remaining source-closed edge with a
current test gap is the General-page heat-transfer mode checkbox to its owning
group box:

`modeHtTransChckBx (0x78) --toggled(bool)--> groupBox35 (0x80) --setEnabled(bool)`

SetupUi explicitly calls `QWidget::setEnabled(false)` on `0x80` at original
address `0x140edbd8f`. Later in the same setupUi routine it registers the
`0x78` to `0x80` connection at `0x140f219fb`. The retained connection contract
identifies the signature as `toggled(bool) -> setEnabled(bool)`, AutoConnection.
Current `EditorSetupState` records the initial setter as
`requestedEnabled=false` but leaves the separate `enabled` observation unset;
its generic event projection updates the requested value and preserves the
unknown effective enabled state. No existing `test_editor_setup.cpp` test
references this sender or receiver. That provides a compact regression for
remembered setter state without pretending the full widget ancestry is known.

This edge is preferable as the next test-only increment: it needs no new model
or operation fixture and remains below the public editor300 guard. It is not a
claim that a physical click happened or that descendants are enabled/disabled.

## Source and receipt evidence

- Unique bounded capture: `analysis/resume62-editor-setup-toggle-raw.txt`,
  SHA-256 `F233E16A03C4B408F3442DF7D014412DA11173AD446608834B54F97F626E00C8`.
  It preserves source lines 1048-1074 (the `0x80` group-box setter) and
  30660-30675 (the `0x78 -> 0x80` connection) from
  `analysis/wave2-save-diffs-300-setupui.c`, source SHA-256
  `1217ddfefe72fc52f4a963f01cb4f7d10bc3a468d00e7ef1139736979483b8a7`.
- `analysis/wave3-editor-constructor-prefix-contract.json` records this exact
  edge and setupUi/connection ordering. Its SHA-256 is
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`.
- In `source/apps/moproj2mst/src/engine/editor_setup.cpp`, control identities
  at lines 46-47 give `modeHtTransChckBx` and `groupBox35`; prefix call at line
  715 records explicit Enabled(false) at `0x140edbd8f`; internal connection at
  line 803 records the pair. Constructor handling stores the explicit value in
  `requestedEnabled`. `receiveInternalToggled` updates that requested value and
  only changes `enabled` when a qualified enabled profile exists. Source hash:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`.
- Current `source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp` lacks
  `0x78`/`0x80` toggle coverage. Its latest hash at audit time is
  `98A6DF578730B848AAE37D02EF781D540DF925BF99F8CA4A0AF520AF41FD4170`.
- The source contract distinguishes the explicit local requested bit from
  effective enabled state through the parent tree. No inherited descendant
  state is inferred by this proposal.

## Focused acceptance contract

A focused test can bind fresh combo/spin controls, consume the qualified
no-active-translator control prefix, and install the internal-connection
projection. Before any supplied event, assert
`state.control(0x80)->requestedEnabled == false` and
`!state.control(0x80)->enabled`. Deliver
`receiveInternalToggled(0x78, true)` and then false. For each, require exactly
one ordered delivery with sender `0x78`, receiver `0x80`, and matching
`requestedEnabled`. Assert the stored requested value follows the delivery,
while `enabled` remains unset and sender `0x78`'s checked state remains
unresolved. Also assert installation alone did not change `0x80`'s initial
requested value. This checks the original explicit setup setter plus the
remembered internal route; it does not synthesize checkbox checked state, a Qt
signal, parent propagation, or complete native setup.

## Proposed owner and limits

Next implementation owner: one editor300 test worker, write set limited to
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`. Root owns build and
integration. Focused target: `m2m.mdt.editor_setup`. No source or tests changed
for this read-only audit. No GUI/app-control, build, or mutable REA operation
was performed. The PaintDevice prefix is an independent constructor branch and
does not change this setupUi connection contract. Token measurement unavailable.
