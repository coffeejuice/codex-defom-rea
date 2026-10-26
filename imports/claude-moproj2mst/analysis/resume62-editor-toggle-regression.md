# Heat-transfer mode toggle regression (checkpoint 62)

Added `editor_setup_heat_transfer_toggle_retains_requested_group_enable_only`
to `source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`. The focused case
uses the fresh default combo-model and no-active-translator prefix required to
install the recovered internal connection. It verifies the explicit initial
`groupBox35.requestedEnabled == false` from setupUi, and separately requires
its effective `enabled` and the checkbox checked state to remain unknown.

Before the connection projection is installed, a supplied toggle produces no
delivery. Installing the projection does not synchronize the receiver. After
installation, supplied true and false events each produce exactly one ordered
`0x78 -> 0x80` delivery and update only `requestedEnabled`; the effective
parent-tree enabled state and checkbox checked state remain unknown. The test
does not represent a physical checkbox click or Qt signal emission.

## Frozen hashes

- Test file SHA-256:
  `33C545B3DF8C95B5D0CAD39756048A6A15F072D810BA4E1A28A1736954ED935C`
- Bounded raw setupUi excerpt SHA-256:
  `F233E16A03C4B408F3442DF7D014412DA11173AD446608834B54F97F626E00C8`
- Original connection contract SHA-256:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`
- Existing candidate `editor_setup.cpp` SHA-256:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`

Static checks passed: balanced braces, no trailing whitespace, unique test
name, and `git diff --check`. No build/CTest was run per assignment; no source,
shared, CMake, GUI, or mutable REA files were changed. Token measurement
unavailable.
