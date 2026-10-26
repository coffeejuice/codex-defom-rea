# Checkpoint 60: next DoeOutput property-dispatch edge

## Selection

The smallest uncovered operation/property edge adjacent to the Checkpoint 59
active DoeOutput guard is the DoeOutput **properties-load** branch in
`DoeOutputClass::propsLoad` (`source/apps/moproj2mst/src/ops/noop.cpp`). The
method looks up the active sub-project and, when it has a DOE study, refuses
because the two study values are not modelled into the properties object at
`+0x38`. Existing test searches show ShapeOptim properties coverage and a
MultiBlow property-load case, but no DoeOutput-properties assertion. This is a
distinct vtable slot (`vt+0x98`) from the operation class-data/document path
that Checkpoint 59 stops at.

The normal loader calls `ob->propsLoad(p, op)` after operation loading
(`source/apps/moproj2mst/src/engine/loader.cpp`, around line 953). Therefore the
branch is a real operation/property dispatch target. It is **not** reachable
from the unmodified retained 120_B_DOE run today: its first Session Image makes
`openActiveOperation` stop at the QImage decoding guard before the loader can
finish the operation and call properties loading. A focused direct class
dispatch regression can qualify the branch guard without claiming that the
retained project reaches it end-to-end.

## Concrete candidate-only contract

Use a disposable copy of the complete retained
`evidence/gui_mo/oracle_b/120_B_DOE/COG_P1_B1.moproj` case. Bind it to the same
fixture identity already recorded in `analysis/resume54-connector-project-check.md`
(project SHA-256
`60ADDC2722E85F92A088FA07F77E8A966928AB5B934F161DF87DCF6A417ED30E`) and the
Checkpoint 59 DOE document identity (2,013 bytes,
`97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`). Open
through the real parser/registry, activate sub-project index 1, resolve
operation 6 / `SfOprDoeOutput`, then invoke that registered class's `propsLoad`
directly. Keep this separate from `openActiveOperation` and record that this
is a direct property-vtable call, not proof of normal loader sequencing past
the earlier Image refusal.

Assert the selected active sub-project is Type 2 and owns a non-null DOE study;
assert the resolved registered key is `SfOprDoeOutput`; then assert the exact
`M2M_E_UNSUPPORTED` refusal:

```text
SfOprDoeOutputProperties 0x140e36290: DOE study values (0x14091e0b0, 0x14091e0f0) into the props data (+0x38) are not modelled
```

Also assert the copied input tree manifest remains unchanged and no
`FileEffects` writes/deletes occur. Do not assert property values, successful
property completion, operation-load completion, Save, or Close. The method's
available source contract only establishes refusal when the active SP has a
DOE study; it does not establish behavior for a fully modelled property owner.

## Qualification and next evidence

This regression would establish candidate dispatch to the DoeOutput property
override and its current study-present refusal using a complete parsed project
as setup. It is narrower than a full-project lifecycle test and does not turn
the earlier active DoeOutput guard into a passing operation load. The original
decompile attribution (`0x140e36290`, `0x14091e0b0`, `0x14091e0f0`) supports the
branch's existence and read/store intent, but no original execution here
establishes the properties owner's exact state, values, or returned effects.
The original 16-case connector probe is also not relevant evidence for this
property branch: it mocks admission callbacks and does not dispatch operation
properties.

To qualify normal loader reachability later, a separate evidence-backed input
must pass DoeOutput `loadClassData` (including the `.DS` Image/QImage stage) and
reach `propsLoad`, with a matched original/candidate capture. Do not mutate the
retained project or remove its Image under a parity label to manufacture that
path.

Read-only dispatch inventory. No source, build, GUI, fixture, or REA database
was modified.
