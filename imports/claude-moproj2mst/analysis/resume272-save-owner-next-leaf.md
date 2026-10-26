# CP272 Save owner next leaf

The next finite internal owner migration is the four-field commit at the end of `save_current.cpp::storeSimNames`. It can proceed without new original execution, GUI control or Frida. This is a source-backed owner boundary, not a new Save parity claim. CP247's equipment ordering and CP256's MultiBlow PDIE command are already consumed; neither should be repeated.

## Proposed command and preserved boundary

Add a narrowly named `GuiModel::storeOperationSimulationNames(const qt::String& operationName, int flow, const qt::String& simulationName, int simulation)` (final naming subject to ordinary review). It must:

1. Call `simCtl()` exactly once and return if absent. Keep throwing admission behavior; do not declare noexcept.
2. Execute `strncpy(sc->oprnam, local8(operationName).c_str(), 0x40)`.
3. Store `sc->cursim[1] = flow`.
4. Execute `strncpy(sc->simnam, local8(simulationName).c_str(), 0x50)`.
5. Store `sc->cursim[0] = simulation`.

Do not pre-encode both strings in the caller or before admission: that moves conversions before the null gate and before the first partial commit. Do not use terminating string helpers, zero whole arrays, change locale conversion, batch/rollback the writes or return a control pointer. The references are synchronous inputs; the command stores no references. Only these four destinations change.

`storeSimNames` retains active-subproject lookup, copied operation name, flow number, complete `mst::simEntries` construction, first matching entry selection, unnamed-entry refusal, and model lookup in their present order. Only its final immediate lambda/control-pointer dereference becomes `if (m) m->storeOperationSimulationNames(name, no, first->name, first->sim)`. There is no added control admission and no retained raw alias. Keep operation metadata and sim-list ownership outside the model.

This is distinct from CP256: PDIE uses two successful admissions surrounding a provider, while this site has one admission, no provider callback and a complete finite input tuple already selected by the caller. CP233 rejected generic writer/callback extraction and lacked a qualified one-value consumer; this design instead uses the explicit four-field native command and adds the previously missing actual Save caller assertion.

## Source qualification

`source/docs/re/moproj2mst/s/spec.md` section 1 identifies native `0x140054540` and its exact destinations/copy lengths. The retained original decompile `evidence/gui_mo/out_scope/opsave3.c`, lines 45–78, establishes first-entry selection, single control resolution, non-null gate, conversion/copy of operation name, flow store, conversion/copy of simulation name, simulation store. SHA-256 inspected here: `3815a4d7c22943db590786fae008231cbe4f3ced9c299d40770965c767399361`. `opsave2.c` retains base save-name dispatch into this helper. These are retained static qualifications, not a fresh original runtime observation. Preserve the current candidate model barrier; no claim is made that null-model or parked-geometry exceptions reproduce native faults.

## Required focused checks, including actual Save

Retain `tests/s/test_save_current.cpp::store_sim_names_step_scope`, which already checks both names, integer fields, sentinel bytes immediately beyond each copied region, repeated calls, owner/step identity and detached-step no-op. It explicitly calls only the helper, so it is insufficient by itself for this leaf.

Add direct owner cases for short names, exactly-bound names and overlong names, using independently assembled expected byte arrays. Compare the complete pre/post step snapshot with only the four expected changes; verify no terminator appended at indices 0x40/0x50, null-step no recreation, and the existing parked-geometry refusal invariant. Do not manufacture an exception hook merely to observe each store.

Extend the existing real caller in `tests/mdt/test_multiblow_save.cpp::runCase`, immediately around its existing `engine::saveCurrentOperation(p, *op, ctx, save)`. For each already executed 93/93D/94 corpus case, obtain the active subproject and independently select the first matching named `mst::simEntries` entry using `flowNumber(*op)`. Require that entry, seed both bounded name regions and both CURSIM fields to distinct wrong sentinels, then run the actual Save call. Assert exact expected name arrays and both selected integer values afterward, alongside the existing PDIE assertion and unchanged historical KEY/deletion comparisons. Do not rename the operation or change expected fixture bytes: seed only stale model fields which Save restores. Current MultiBlow Save invokes baseSaveNames before the CP256 PDIE command; unlike the CP256 synthetic provider fixture (which intentionally has empty `current`), this real caller reaches the base-name segment.

Before implementation acceptance, confirm all three fixture cases execute without skip and their selected entries exist; a skipped Oracle case is not caller coverage. Build through `admin/build_candidate.py` and run the exact configured CTest entries for Save-current, MultiBlow Save and viewer-box; preserve required root full regression/freeze separately. These checks are proposed, not executed in this read-only lane. Final saved bytes can establish the caller's resulting state, but do not dynamically prove intermediate conversion timing or every native interruption prefix; order remains source-established.

## RIO, RACCEPT and retained differences

`takeover-state.json` RIO remains active with the per-stage open/write/close fault matrix and later-stage retained effects incomplete. Its CP214 next action already consumes the CP164 candidate denial conjunction. This owner migration closes none of that original failure witness and must not trigger repeated denial permutations.

The exact remaining remesh fopen witness is one original Save occurrence with held denial and resolved path/CWD, failed primary open, first null FILE passed to fprintf, decoded handler slot from the same module, filter/default-handler/termination reach and final process status, bound to process/module/load identity and before/after raw files. CP247's separate equipment witness concerns the fixed `Equipment (N).KEY` source (not the Qt fallback QTemporaryFile), manager add result including -1, both manager path lookups, ID stores, removal attempts and second writer attempt. Native wrapper return hides the inner writer's failed-fopen status 3. Neither witness is supplied by a new owner command.

RACCEPT remains dependent on the unfinished recovery workstreams, including RIO and RCLOSE. Historical `93 SAME / 5 DIFF` remains qualified: A55 is conditional editor-fill/CG timing; B120/B121/B122 compare pre-action trees against GUI-created DOE state/outputs; B98 is the isolated project-root path difference. The CP256 historical audit provides exact paths and limitations. This command changes none of those classifications, supplies no fresh Open → Save → Close acceptance, and must not be described as resolving a historical DIFF.

Only this report was written. No source/test/shared state, build, executable, original process, GUI or mutable REA operation was changed or run.
