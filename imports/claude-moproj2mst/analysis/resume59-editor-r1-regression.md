# Coordinate R1 enable-delivery regression (checkpoint 59)

Added one focused test to `source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`:
`editor_setup_coordinate_r1_toggle_sets_only_its_group_enabled_state`.

The case advances the isolated setup projection through fresh combo/spin/stopping
profiles, no-active-translator consumed controls, and internal connection
installation. It asserts the source-established initial state (`0x6a0` node
radio checked, `0x6b0` coordinate radio unchecked, `0x6b8` groupBox267 disabled),
then delivers true and false `toggled(bool)` values from `0x6b0`. Each delivery
must produce exactly one ordered route to `0x6b8` and change its requested and
qualified direct enabled state. Radio checked state remains unchanged across
deliveries, so the test does not mistake an event delivery for a click or
`setChecked` call. It makes no claim about descendant effective enabled state
or a physical GUI interaction.

## Frozen inputs and output

- Test file SHA-256: `CB723342D8C453605F803BE974FB86A75A7A17ADE5A0839B3F6E836E15494659`
- Original setup connection contract SHA-256:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`
- Original remesh/radio qualification SHA-256:
  `E14F3F298D1CD4A2B004B9A150DB5DD25D22A53EF277CE2771423AC5122FECC1`
- Existing production handling SHA-256:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`

Static checks passed: balanced braces, no trailing whitespace, and
`git diff --check`. No build or tests were run; root owns build and integration.
No production, shared state, CMake, GUI, or mutable REA files were changed.
Token measurement unavailable.
