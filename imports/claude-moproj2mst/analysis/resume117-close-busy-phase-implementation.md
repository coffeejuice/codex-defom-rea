# CP117: phase-aware pure main busy decision

Extended the existing pure decision with an explicit main phase. The base-construction phase returns NotBusy without requiring widget/mode input; the derived phase retains the existing currentIndex-then-mode comparison; unknown/unrepresented phase returns NeedsMainPhase. No application/main owner, c99530 execution, phase transition or public Close callsite was introduced.

Only the three exclusively assigned source/test files and this receipt were written. No shared Session/main/interface, CMake or plan edit; no build/test execution, GUI, original launch, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Source and API contract

`resume117-close-canonical-main-construction.md` closes the dynamic table distinction. During selected base construction, table14290ac88+300 resolves140ca44f0, whose eight bytes `48 89 4c 24 08 32 c0 c3` save this, clear AL and return false. SHA-256 of140ca44f0..140ca44f8 is `bda5135adee0e1dc7d938d8d839bae743ef85fbccb23cbad3f3c831b1809efee`. After derived table1420fb108 is installed, its+300 is14001daf0: compare actual currentIndex with2, otherwise compare main DWORD+270 with1. Initial publication of M as W precedes derived/stack readiness.

New `CloseMainBusyPhase` values are Unknown, SfMainWindowBaseConstruction and SfMoMainWindowDerived. The new overload requires all three arguments:

```cpp
evaluateCloseMainBusyDecision(phase, currentIndex, mode270)
```

The phase is an explicit mathematical input, not a constructor/native-liveness witness. Unknown phase remains unavailable even when index/mode values are supplied. Invalid enum values also retain NeedsMainPhase. BaseConstruction ignores the optional values because its native leaf performs neither read. Derived delegates to the unchanged old function and preserves first-read ordering and exact equality semantics.

The old overload and default second argument remain unchanged; existing one/two-argument calls still model the derived predicate. The new overload has no default arguments, preserving even the old single empty-brace call without ambiguity. NeedsMainPhase is appended after the existing enum entries, preserving their ordinal values and all old return behavior.

Neither overload retains a phase or identity. In particular c99530 installs no vtable: the source address cannot by itself select BaseConstruction on an already-derived receiver. Likewise a later global current-main W does not inherit constructor main M's phase. No event argument or automatic initialization/reset API was added.

## Validation prepared for root

Focused target remains `m2m_mdt_close_main_busy_decision`, discovered by the existing source/test globs. Nine Testkit cases retain the original five and add unknown/invalid phase, base false with missing/conflicting values, explicit derived partial-read ordering after a base evaluation, and separate later-current-main inputs that remain unknown instead of borrowing an earlier constructor-main phase. Static noexcept and legacy empty-brace compatibility expressions are included. These are declared pure input cases, not actual Qt owners/events.

Static preflight checked include resolution against m2m_core/testkit roots, Testkit main, supported CHECK/TEST_CASE macros, absence of C assert/REQUIRE_EQ, preserved legacy declaration and result-enum ordering, and whitespace. No compilation or test execution occurred. Root owns the build and focused run:

```text
ctest --test-dir <build> -R '^m2m_mdt_close_main_busy_decision$' --output-on-failure
```

The helper still supplies no global+9/current-main lookup, Qt getter, manager can-close, modal schedule/nested Save, accepted effects or cleanup. NotBusy is only a selected virtual-predicate projection. CP110's Save-failure ordering and later fresh FileCloseAll guards are unchanged. CP117's ATL event registration/selector invocation and real QApplication/main producer remain unresolved.

## Frozen source receipt

Paths below are under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/close_main_busy_decision.hpp|`d988d2e066107e1696dfa50c369206e6702a87ece04d890e140b2dd7e5e1bf61`|
|src/engine/close_main_busy_decision.cpp|`ebcb55dc42a1fa8fd1d54fa7a3c5d93aeb31fa8a5c6811640116d128a1313033`|
|tests/mdt/test_close_main_busy_decision.cpp|`28b56f7312f08c904309f9dfa90cf15b76604947157d65bd708fa15f34ebea04`|

Source qualification report SHA-256 `ffd128c9f46352f1d1b89337634e75ccbe0a4aa7d01bd420c19726f2ad374aa9`. Prior helper receipt: resume112-close-pure-decision-implementation.md. These files are frozen and ownership is released for root integration. This is static preflight, not a tested candidate archive or runtime Close witness.
