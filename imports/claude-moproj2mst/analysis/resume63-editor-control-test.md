# NBC angle-group enable regression (checkpoint 63)

Added `editor_setup_nbc_toggle_retains_requested_angle_group_enable_only` to
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`. The setupUi contract
maps `m_chkbxNBC` (`0x14f0`) to `m_grpbxAngle` (`0x14a8`) through one
`toggled(bool) -> setEnabled(bool)` AutoConnection. The receiver has an explicit
initial `setEnabled(false)` at `0x140f16a68`.

The focused test verifies the initial requested false state while leaving the
separate effective enabled state and checkbox checked state unknown. It confirms
that pre-connection delivery is ignored and connection installation does not
synchronize the target. Supplied true and false toggle deliveries each produce
exactly one ordered edge and update only `requestedEnabled`; effective enabled
and checked state remain unknown. This is an isolated retained-route test, not
a simulated GUI click or public editor setup completion.

## Frozen hashes

- Test file SHA-256:
  `347C51C18DC787E9BC7A2A2EB4648BAD2AC68B8541FBA882009DDD3982EE5319`
- Original setup connection contract SHA-256:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`
- Existing candidate `editor_setup.cpp` SHA-256:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`

Static checks passed: balanced braces, no trailing whitespace, unique test
name, and `git diff --check`. No build or tests were run. No production, public
API, CMake, GUI, or mutable REA changes were made. Token measurement unavailable.
