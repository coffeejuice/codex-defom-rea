# CP188 R300 `contactFlagsFromEngine` plan review

## Decision

**Plan mechanics: pass, with a reconciliation blocker before the next sequential source edit.** The proposed lexical scope is compatible with current behavior and the retained native annotation. However, the opening reconciliation in `resume188-r300-next.md` describes a pre-CP187 source state: current frozen source already has the `saveBcc` scoped callback, and CP187 implementation, root-integration, and freeze records are present. Root must reconcile that mismatch and confirm the two shared-file owners are released before editing. This is a sequencing/identity blocker, not a blocker to the R300 `contactFlagsFromEngine` design.

## Reconciliation resolution before CP188 edit

The implementation owner rehashed both live files against CP187 `source.zip`; both matched the release hashes below. The archive hash also matched its receipt (`d027708fd3dee12a94d6b36ec85e7dc40a3ea5ce7168546574020dc9a1abed12`). The CP187 implementation explicitly releases the pair, root integration confirms the frozen candidate, and root assigned the released files for this CP188 cut. The stale opening in `resume188-r300-next.md` is corrected. The review's sequencing/identity blocker is resolved; the original as-of findings below remain as audit history. Build/regression/freeze remain root-owned.

## Source identity and ownership

The current `source/apps/moproj2mst/src/engine/save_current.cpp` SHA-256 is `46265debeae2d39625cdc4e33e08970eab0bbe8540b6752a1015fa36f3f1ae36`; the current `tests/s/test_save_current.cpp` hash is `0854785340253d8b78d396945a66e75f3cb56bbba2797aefb135e7cdc368e9ee`. Both match the CP187 implementation release. `build/wave3-checkpoint187-verified/receipt.json` records a source archive and tree hash, and `analysis/wave3-checkpoint187-validation.json` records the frozen full-regression result. `analysis/resume187-root-integration.md` states the CP187 source owner released the files and root froze the integrated candidate. These records contradict the R188 note's statements that no CP187 R300 implementation receipt exists and `saveBcc` still has its original function-scope step alias. The current `saveBcc` uses an immediate callback around both `captureKey` writes, with acquisition after the original filters and object/mesh checks.

The target pair remains shared with CP187. The CP187 implementation and root integration indicate release/freeze, but the R188 author/dispatcher should explicitly reconcile current checkout identity and active ownership before assigning edits. This read-only review changed no source or test files and did not build or run tests.

## Borrow, model lookup, and branch order

`contactFlagsFromEngine` currently has one step fetch, `m.step.get()`, and no `opModel` lookup. Its only production caller is `saveInterObject`, which performs one `opModel(p, op)` lookup and enters a synchronous callback with `m ? m->step.get() : nullptr`; that callback rejects null before optional rebuild, flags, path derivation, or writing. `contactFlagsFromEngine` is reached only after this admission. Preserve that caller boundary and avoid introducing a second model lookup or changing null-step behavior. A local synchronous callback can contain the one raw step alias and all reads; keep it non-escaping and do not add a helper-level null policy unsupported by evidence.

The listed algorithm/order matches current code and the native annotation:

1. Clear `icb.flag` first.
2. Read the pair count from `step->inter_obj` at `+0x10`.
3. For zero pairs, enumerate `m.objectCount()`/`m.objectAt(i)` in ascending zero-based index order and flag object ID minus one only when kind at `+0xc` is not 7. This branch is independent of `icb.enable` today.
4. For nonzero pairs, enter the loop only if `icb.enable`; use `pair_nth(..., i + 1)` for `i = 0..pairs-1`, skip null pair records, flag the slave ID minus one for kind 2, the master ID minus one for kind 1, and ignore other kinds.

Do not add range checks, alter indexing, move flag clearing, change the disabled nonzero-pair behavior, or add a new null branch. `saveInterObject`'s existing null refusal remains before rebuild and flag mutation.

## Fixture and qualification

Extending `save_interobject_step_scope` is appropriate. It uses the copied R46 operation-2 project, requires a loaded step and objects, disables lubricant rebuild for the positive probe, and either retains the existing pair list or appends actual-ID kind-2 and reversed kind-1 records. It validates pair kinds, bounded IDs, membership in the loaded object list, computes expected flags using the same kind-specific rule, and checks the result through the actual `saveInterObject` caller. It also checks one expected inter-object write, nonempty bytes, no deletes, pair-count stability, and stable model/step identity. Its detached-step RAII probe calls only `saveInterObject`; it does not invoke the helper with a detached step. This fixture qualifies the positive pair path and helper mechanics, not direct zero-pair enumeration, public Save, native bytes, or helper null behavior. The plan correctly treats a zero-pair branch test as optional and confines any added synthetic list changes to the discarded project.

`source/docs/re/moproj2mst/s/spec.md` §1 places native `0x1400655d0` after optional system-lubricant pair rebuild and before `W_INTEROBJ(step, path)`, identifying the computation as INICTC flags from engine pairs. The source annotation at `0x1400655d0` agrees with the retained pair/object accessors. The existing caller preserves optional rebuild, flag synchronization, path derivation, and writer order. These materials support the bounded order/branch claims; they do not establish fresh original-process comparison, public Save admission, native byte equality, or null behavior inside this helper.

No source, test, build output, GUI/app-control session, REA database, or shared state was changed for this review. Only this report was written.
