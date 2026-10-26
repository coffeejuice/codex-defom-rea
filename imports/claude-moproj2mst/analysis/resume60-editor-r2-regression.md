# Reference 2 node-radio enable regression (checkpoint 60)

Added `editor_setup_node_r2_toggle_sets_only_its_spin_enabled_state` to
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp`. Under the qualified
fresh combo, spin, stopping and no-active-translator setup profile, the case
checks the initial checked R2 node radio, enabled `ref2NodeSpnBx`, and its
source-set maximum `999999`. It supplies false and true toggled values and
requires exactly one ordered delivery from `0x738` to `0x740` each time. The
spin's requested and qualified direct enabled fields follow those values, while
the sender's checked state remains unchanged. It does not claim GUI click
semantics or effective state inherited by descendants.

## Frozen hashes

- Test file SHA-256: `98A6DF578730B848AAE37D02EF781D540DF925BF99F8CA4A0AF520AF41FD4170`
- Setup connection contract SHA-256:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`
- Stopping page contract SHA-256:
  `7AC3719501898940FBB88B3257B524354EEBA0AFEA18BDB489289E005C73049E`
- Existing candidate handling SHA-256:
  `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`

Static checks passed: balanced braces, no trailing whitespace, unique test
name, and `git diff --check`. Build and tests were not run; root owns validation.
No production, shared state, CMake, GUI, or mutable REA files were changed.
Token measurement unavailable.
