# CP254 loader/property next leaf (revised after review)

## Candidate: MultiBlow save-time PDIE assignment

The bounded owner migration is the final control write in `source/apps/moproj2mst/src/engine/multiblow_load.cpp::multiBlowSaveNames`. Keep the base save-name call, model lookup, and first `simCtl()` presence/admission test. Replace the second raw control dereference with a finite `GuiModel::setMultiBlowPrimaryDie(int)` command that performs one `simCtl()` call, returns if that call yields null, and writes only `pdie1`.

The call site should preserve this current C++ sequence:

```cpp
c.m2m::mst::OpClass::saveNames(p, op, fx);
GuiModel* m = opModel(p, op);
if (!m) return;
if (!m->simCtl()) return;
const int primary = vt120();
m->setMultiBlowPrimaryDie(primary);
```

C++20 evaluates the RHS `vt120()` before the assignment's LHS expression `m->simCtl()`. The retained native `evidence/gui_mo/s1/mdt/L4/d0.c::FUN_140602780` instead resolves/caches the second control pointer before calling virtual +0x120, then stores through the cached pointer. Do not describe the suggested C++ owner call order as the original native order. For the actual registered `MultiBlowClass::v120()`, the provider is the pure constant `return 2`; the observed production outcome is therefore the same. This does not authorize side-effecting/future providers or claim native timing equivalence for them.

The original second dereference is unchecked after the first successful `simCtl()` test. The new owner command's null return is a safe value-command contract, not proof of that unchecked native behavior. This migration is qualified only for the current synchronous path with stable model storage and pure `v120() == 2`; no callback or storage replacement occurs between the first successful check and the second access in the inspected source. If arbitrary providers, reentrant storage replacement, or geometry-barrier changes between accesses enter scope, the command needs a separate reviewed contract. Do not add a C ABI change or expose a native pointer.

## Tests and what they establish

- Add a direct owner command test for ordinary success, parked geometry refusal through the existing `viewer_box` refusal helper with full control/step/effect comparisons, and absent-step no-op/no recreation. Seed unrelated control fields and verify they remain unchanged. The absent-step case validates the new safe owner API only; it does not prove the old unchecked second access.
- Retain `tests/mdt/test_multiblow_save.cpp` as integration coverage. Its `runCase` compares the recorded `save.writes` paths/bytes with historical Save-step files and compares `open.deletes`; it does **not** serialize or compare the full `.moproj`. It also accepts `Result::Skip`, so report executed/pass/skip counts for each 93/93D/94 case. A sentinel variation can set PDIE to a distinct value after opening and before `saveCurrentOperation`, then assert it becomes 2 and the emitted SimCtrl KEY still matches the saved fixture.
- If those optional Oracle B cases are unavailable or all skip, require a narrow direct caller-level test through `multiBlowSaveNames` using a counted pure test `vt120` provider: first-null control path must return before provider evaluation; success path evaluates the provider once and commits its value. Keep it independent of optional Oracle B output files. Confirm configured CTest names before running.

These tests establish current reconstructed owner/hook behavior and recorded Save KEY effects. Final state/output does not dynamically establish intermediate control-pointer/provider ordering; retain source review for that distinction.

## Original qualification and reconciliation

`source/docs/re/moproj2mst/mdt/L4/spec.md` §1 assigns MultiBlow save-current vt+0xd8 `0x140602780` to base save names followed by SC+0x168 = vt+0x120. Retained d0 decompile establishes native control-pointer-before-virtual-call order. L4's historical Oracle B 93/94 saved outcomes and `test_multiblow_save.cpp` are file-output witnesses, not fresh native hook timing evidence. They cover recorded KEY outputs/deletes; do not claim `.moproj` equality from that test.

CP251 prologue command is implemented and complete. CP247's Forming success-order repair is complete while native add/copy failure evidence remains separate. CP248 meshing writeback is complete. CP249's heating-unit query is complete; the broader HeatTreat property projection remains blocked by defaults/read/write layering and missing focused tests. No global loader alias closure is claimed.

No source, test, build, shared state, GUI, original, or REA artifact was changed or run; this is a read-only design.
