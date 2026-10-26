# CP252 next leaf: correct the secondary-display model's absent-step dimension

`src/engine/scene.cpp::displaySecond` has a distinct, source-ready normal-result discrepancy: with a live main model whose native step is absent, it currently constructs the second model with dimension **0**. Original `0x140cf9990` calls the default-3 getter and then safely skips the main-control copy when that step is absent. Unlike the named-model creator investigated separately, this function has no unchecked main-step-number/title copy before returning.

After CP251's source-stable freeze and ownership release, the smallest implementation is one production expression: use `b->dimensionOr(3)` instead of `guiModelDim(*b)` when constructing the secondary model. Add actual `displaySecond` caller tests in `test_display_selection.cpp`. This reuses CP243's existing value API, changes no common header/model file, and does not reopen the unresolved named-display no-step path.

Only this report is written. No source/test, build, shared recovery state, original process, GUI control or mutable REA database changed; no tests ran.

## Exact production edit

Paths below are relative to `source/apps/moproj2mst/`.

Current `src/engine/scene.cpp::displaySecond`, around lines 53–66:

```cpp
if (dm.second.get()) return dm.second.get();
GuiModel* b = displayMain(d);
if (!b) return nullptr;
auto m = std::make_unique<GuiModel>(nullptr, guiModelDim(*b));
```

Change only the constructor argument:

```cpp
auto m = std::make_unique<GuiModel>(nullptr, b->dimensionOr(3));
```

`GuiModel::dimensionOr(int)` already returns the live integer unchanged, or the supplied fallback only when its step is absent. It is unguarded and `noexcept`. The current `guiModelDim` delegates through viewer `modelDim` to `dimension()`, whose absent-step value is 0. A method-name analogy is not the evidence: this inspected implementation establishes the difference being corrected.

Keep existing-second reuse first, missing-main return before construction, nullable DB input, constructor/backlink/control-copy/set-second/return positions unchanged. Do not replace the dimension with `dimension()==0 ? 3 : dimension()`: present dimension zero is distinct from missing step and must remain zero. Do not add geometry admission, step construction on the main model, identity issuance, new exceptions, or another model lookup. No `displayNamedModel` guard or setup changes belong to this leaf.

## Original full function path, not getter extrapolation

Retained `evidence/gui_mo/s1/mdt/L4/vs/b.c` contains `FUN_140cf9990`. I also independently read the complete original PE unwind region `0x140cf9990..0x140cf9ac8` with read-only DUMPBIN and checked the nullable control getter `0x140c7b2d0..0x140c7b301`.

| Original site | Reached behavior |
|---|---|
| `0x140cf99ae–99b6` | Read the existing secondary guard at display+0x1b8; a nonnull second jumps to the common return. No main query or allocation occurs on this branch. |
| `0x140cf99c1–99c9` | Read main model through `0x140cf94f0`; null jumps to the common return, with no new secondary model. |
| `0x140cf99d4` | Request `0x278` wrapper bytes. Normal-result analysis below assumes allocation/constructor return normally and produce the expected new model/step. |
| `0x140cf99eb–99ff` | Re-query current main, call `0x140c7fee0`, pass its integer result to new-wrapper constructor `0x140c7a6b0`. With a stable live main wrapper/null step, getter returns 3. |
| `0x140cf9a2f` | Publish the new guarded secondary pointer through `0x140cbc920(display+0x1b8,newModel)`. |
| `0x140cf9a4d` | Set the new wrapper's display backlink through `0x140c7ade0`. |
| `0x140cf9a66–9a76` | Query the new model's control, skip later copy if null. |
| `0x140cf9a7d–9a8d` | Re-query current main, call its nullable control getter, and branch to return if that control is null. For a stable absent main step, this is the ordinary no-copy branch. |
| `0x140cf9a94–9aac` | Only after a nonnull main-control test, query it again and copy GEOTYP (`+0x100`) into the new control. |
| `0x140cf9abe–9ac7` | Return the then-current guarded secondary model. |

The control getter first resolves the wrapper's step, tests it, and returns zero at `0x140c7b2fa` when absent; only the present branch adds `0x750`. Thus the no-step main does not lead to the named creator's `step+0x744` fault. The secondary function never performs that step-number or title copy.

This supports a completed stable normal-return branch with a dimension-3 secondary model, existing main step still absent, new backlink and secondary guard established, and no main GEOTYP copy. It is static qualified behavior under successful prefix and stable receiver assumptions, not a recorded original occurrence or a claim that every allocation/reentrant/error path returns normally.

Original identity: `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. DUMPBIN banner is 14.44.35228.0 (installed directory 14.44.35207). Native half-open range hashes, recomputed through PE file-backed mapping:

- `140cf9990..140cf9ac8`: `635617aa3ec47b312986217bfea91ee855f820285531448a005b9a1b431218ec`.
- `140c7b2d0..140c7b301`: `2f5b78f5174075403916e79492d514c1eb0aab38b00ffd322bcef44b73bc2ef2`.
- `140cf9930..140cf9959` (separate secondary assignment wrapper): `cff68d5336fcae9dbce7e07c2c151b47bb81f4c1c186bdf1a96f98648f87acec`.
- Default-3 getter `140c7fee0..140c7ff1f`: `e8585aca967e35e147769c8f868d8817892a2e3a509676abc3444b1e62bcca0b`, independently pinned during the adjacent CP252 investigation.

No original instruction was executed and no disassembler database was opened or changed. The read-only disassembly output remains in tool receipts; this report is the only filesystem write.

## Bounded ownership and actual-caller tests

Reserve only `src/engine/scene.cpp` and `tests/mdt/test_display_selection.cpp` after the current freeze. Existing includes already contain the owner API; the test file now includes `defmodel/layout/db_step_data.hpp`, so `main.step.reset()` is resolvable. No new CMake registration or public API is required.

Add a focused case that actually calls `displaySecond`, rather than merely testing the fallback getter:

1. Create a live main `GuiModel`, show it in the actual fixture display, then reset its native step. Confirm the main wrapper remains current and the step is absent.
2. Call `displaySecond(display)`. Require a nonnull returned second with an existing step and exact dimension 3; require its display backlink and `displayModels(display).secondOwned.get()` identify that same model. Confirm the original main remains selected as main and its step remains absent. Do not conflate this with `displayNamedModel`.
3. Call `displaySecond` again and require the same pointer, with no replacement/extra secondary model.
4. With the secondary still alive, let the separately owned main wrapper die so its display guard becomes null; the existing-secondary shortcut must still return the same second. Then drop the second through `displayDropSecond`; with no main and no second, `displaySecond` must return null without recreating storage.

Use a heap-owned main fixture where helpful so its destruction is explicit and no dead pointer is dereferenced. Keep second ownership with the display; do not manually delete its borrowed return. Existing `keepSecond` defaults are false. Existing display guard/backlink tests remain intact.

Retain live-step dimension behavior with a small 2D/3D secondary-creation table: exact input dimension is copied and repeated lookup reuses the same second. Optionally seed main GEOTYP and check ordinary copying; it is surrounding regression, not the selected absent-step fix. A present-dimension-zero creation check is useful only if the existing constructor fixture supports that profile; do not invent a broader dimension-validity promise merely to test fallback selection. CP243 already directly tests that a present zero is not the absent-step fallback.

Existing `selection_named_then_third_then_main_then_second_preserves_other_flags` already exercises the real secondary function, subsequent selection and deletion. The new fixture extends that production entry to the previously untested absent-step branch. These are reconstructed display-model callers, not physical GUI delivery or proof that an original Open path naturally produces a step-less main model.

Expected target/CTest: `m2m_mdt_display_selection` / `m2m.mdt.display_selection`; confirm configured names. Root may include existing `m2m.mdt.l5_loader` because production L5 sites call `displaySecond` (around `l5_slots.cpp:462,549,614,702,1210`), then run the complete source-stable launcher/freeze. No L5 edit is required. Keep Testkit main, `CHECK`/`CHECK_EQ`/`REQUIRE`, no C `assert`/`REQUIRE_EQ`, and preflight the exact includes. No missing-file probe, new GUI fixture, production observer or live ObjectKey is needed.

## Effect-order limitation that must not be hidden

Original secondary publication occurs before backlink/control work (`0x140cf9a2f`). The current converter constructs a local `unique_ptr`, performs backlink/control work, then calls `displaySetSecond`. That is a **separate existing order/exception-prefix discrepancy**, not corrected by this one-line dimension fix. Current `simCtl()` additionally imposes reconstructed geometry admission, absent from the raw native getter. This proposal does not move publication, change parked behavior, translate allocation failures, or claim complete secondary-creation parity.

The selected normal absent-step fixture has no pending geometry barrier; the source-control lookup returns null and skips copying, so it can verify the dimension correction without adjudicating those exceptional prefixes. If root wants a publication-order repair, first specify retention and failure semantics for the actual current display owner, nullable destination construction, later guard failure and ownership handoff. Do not silently bundle that into this fallback change or claim that a normal-result test proves it.

The native outer-allocation-null branch publishes null then attempts backlink work, so it does not establish a safe null allocation return. Constructor/callee exceptions and changing/reentrant main state likewise remain outside this bounded result. No crash, cleanup, leak or fixed exit status is inferred for those cases.

## Reconciliation and remaining witnesses

CP245 reaction-rate, CP246 Mode, CP247 Forming successful-add order, CP248 meshing writeback, CP249 heating-time unit and CP250 named-model live-step read are completed and are not reassigned. CP251's MultiBlow owner command is at freeze. The scalar fallback API is already implemented; this new work corrects one **different reached consumer's** result rather than adding another getter helper.

CP250 explicitly left `displaySecond` unchanged while migrating the named path. The subsequent CP252 named-display investigation and PASS review establish why one cannot repair the named path from getter default alone: its unchecked later main-step loads prevent ordinary safe completion. The full secondary trace above supplies the missing distinction for this different function. Keep the named path's explicit adapter refusal and its runtime-witness blocker untouched.

RGEOM's optional cross-section/mesh owner writer, positive getter/builder occurrence and literal project effect remain unqualified under CP200/203/214. This secondary-model fallback invokes none of those geometry readers. ROPS directory/modal/temporary KEY/Type3 and accepted Close witnesses remain independent. CP225 ObjectKey currentness is neither used nor advanced.

A fresh original occurrence is still needed before claiming natural no-step reachability, Qt callback/reentrancy parity or whole lifecycle success. It is not required to preserve this directly source-bounded normal-result correction in the reconstructed finite function. Report the result narrowly as secondary creation with the original default dimension under the stated stable/unparked/successful-prefix profile, with the separate publication/admission limitations above.
