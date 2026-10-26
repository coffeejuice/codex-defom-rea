# CP188 R300 next raw-step owner closure

## Reconciliation and selected successor

The earlier as-of audit described the pre-CP187 source and is superseded by this CP188 reconciliation. Before editing, the two live files were rehashed against their entries in `build/wave3-checkpoint187-verified/source.zip`: `save_current.cpp` matched SHA-256 `46265debeae2d39625cdc4e33e08970eab0bbe8540b6752a1015fa36f3f1ae36`, and `test_save_current.cpp` matched `0854785340253d8b78d396945a66e75f3cb56bbba2797aefb135e7cdc368e9ee`. The archive itself rehashed to `d027708fd3dee12a94d6b36ec85e7dc40a3ea5ce7168546574020dc9a1abed12`, matching its receipt. `analysis/resume187-r300-bcc-implementation.md` releases both files; `analysis/resume187-root-integration.md` and the CP187 validation/freeze records confirm integration. Current frozen `saveBcc` already uses the immediate scoped callback. CP187 is closed for this sequential source cut; root assigned the released pair to the CP188 implementation owner.

Select `contactFlagsFromEngine(Operation&, GuiModel&)` in `save_current.cpp`, extending `save_interobject_step_scope` in `tests/s/test_save_current.cpp` (`m2m.s.save_current`). This function-disjoint successor reads the inter-object list and either enumerates engine objects or walks contact pairs. Its only caller, `saveInterObject`, already confines its own step to a callback and checks it before calling this helper synchronously. The implementation owner holds both files during this cut; root retains build, regression and freeze ownership.

## Exact behavior and scope

Keep `icb.flag.fill(0)` first. Then confine the existing `m.step.get()` borrow and all subsequent reads to a synchronous local callback. Preserve the present order and branches exactly:

1. Read pair count from `step->inter_obj` offset `0x10`.
2. When the count is zero, read `m.objectCount()`, visit `m.objectAt(i)` for `i = 0..n-1`, and set the flag at object ID minus one only when object kind at `+0xc` is not 7.
3. Otherwise enter the pair branch only when `icb.enable` is true; traverse indices `1..pairs` with `pair_nth`, skip null records, set the slave ID flag (`+0x16c`) for kind 2, set the master ID flag (`+0x168`) for kind 1, and ignore other kinds.

Do not add range checks, change object-index conventions, alter the no-pair behavior when `icb.enable` is false, or move flag clearing. Do not add a new null-step policy: the only current caller, `saveInterObject`, has already admitted a non-null step. A defensive early return would introduce behavior for a direct internal call that is not established by the native evidence. Keep the callback synchronous and return no pointer or reference to the step.

## Fixture and guards

Extend the existing `save_interobject_step_scope` fixture rather than creating another project setup. It opens and activates the copied `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, loads operation 2, records model/step identity, and ensures the inter-object list has valid object IDs with kind-2 and kind-1 entries. Its existing expected-flag calculation and assertion after `saveInterObject` directly exercise the positive pair branch through the actual internal caller. It also asserts the one inter-object write, nonempty bytes, no deletes, unchanged pair count, and stable model/step identity.

The same test detaches the step under a local RAII owner guard and confirms `saveInterObject` refuses before effects or flag mutation, then verifies restoration. Do not call `contactFlagsFromEngine` while detached: it has no separately qualified null behavior. Runtime assertions establish the helper's input/output path and state effects; they cannot establish absence of pointer escape, so static review must verify that the local callback contains every step use and that no alias survives its closing brace. No additional fixture appears necessary unless the owner wants a direct assertion for the zero-pair object enumeration branch; such a branch test must restore the original pair list and keep all synthetic mutations in the discarded project.

## Native qualification and limits

Read `source/docs/re/moproj2mst/s/spec.md` §1. It places native `0x1400655d0` after the optional system-lubricant pair rebuild and before `W_INTEROBJ(step, path)`, and identifies it as computing INICTC flags from engine pairs. The current source annotates the reconstructed helper with that address and uses the pair/object accessors described above. The existing test is an internal-helper mechanics test on a reconstructed loaded operation; it is not a fresh original-process comparison, native byte comparison, public Save admission proof, or evidence for null-step behavior inside the helper.

Also inspect the current `saveInterObject` body before integration: it performs the null check before the optional rebuild, flag derivation, path and writer effects. Keep that caller boundary unchanged. Do not use Save-level acceptance as a substitute for the focused helper assertions. Root retains source-stable build, focused/full regression and freeze ownership.

The original planning pass changed only this note. CP188 implementation later reconciled its stale opening against CP187 before source edits; see `analysis/resume188-r300-contact-flags-implementation.md` for the resulting change and file release.
