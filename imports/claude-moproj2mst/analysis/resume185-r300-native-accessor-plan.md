# CP185 saveMaterials native accessor repair plan

Restore the native repeated lookup structure as a separate fidelity change. Do not remove the detached-step test: a non-null model with a null step produces zero materials in the native count accessor. CP184 correctly identified a caller-level difference, but that difference alone does not imply a null-step outcome mismatch. No retained same-occurrence public Save observation establishes model replacement or a null model at this helper.

This is a read-only investigation using the reverse-engineer-anything skill and retained artifacts. No source/test/shared state edits, builds, GUI control, original execution or REA database operations occurred. Only this report was written. Paths below are relative to this converter checkout. The required recovery status was read; its parent GUI lifecycle block does not affect these static reads.

## Original evidence and a necessary correction

The target is original `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Rehashed here. `analysis/wave3-native-key-140058cc0.asm` rehashes to `e66f7860e75c9377f5de55821c2f090b591da06be38f12bbe8b6091afb80d896`, matching `analysis/wave3-native-key-wrapper-static.json`. Its qualification remains “runtime unwind region, not proof of whole logical procedure.” It includes the relevant entry, loop and normal return; this is static evidence, not an executed trace.

Supporting Packet S qualification is `source/docs/re/moproj2mst/s/spec.md:3–10`: retained annotated `out_s/s1c.c` through `s9c.c`; writer twins rely on matching records, with size/callee matching for ambiguous signatures. Its prose shorthand `materialCount(step)` at line 87 omits wrapper detail. Prefer the actual retained count body over that shorthand:

- `evidence/gui_mo/out_s/s1c.c:780`, `0x140c7cae0`: call `0x140c7acd0(model)`; if the returned step is null, return zero; otherwise call that step accessor again and call `0x1408303b0(step)`.
- Unannotated counterpart `out_s/s1.c:1174` agrees. `out_s/s2.c:2144` defines `0x1408303b0` as the int32 load at step `+0x1320`.
- L0 qualification `evidence/gui_mo/s1/mdt/L0/spec_A.md:3–10` identifies the original and static headless Ghidra copy, and gives assembly precedence over argument-drop artifacts. `L0/A/a0a.c:407` defines `0x140c7acd0` as holder access at model `+0xe0`; `L0/A/b0.c:1` defines lazy `0x14005f820`, and line 36 defines nullable operation-step accessor `0x14005f7a0`. `s1/mdt/cur_d4.c:168` supplies the forwarding `0x14005f770 -> 0x14005f820`.

Consequently, a present wrapper whose first count-time step access is null follows native `n = 0`, and never enters material traversal. The existing early `!m->step` return has the same material/file outcome under stable model/step ownership. It omits accessor calls and manager acquisition, so it is not an architectural equivalence proof. Null wrapper is different: there is no caller check before `0x140c7cae0`; its step getter forms wrapper `+0xe0`. The retained holder body (`s1/mdt/L2/d3a.c:446`, `0x140c1aef0`) is a direct dereference. A null-wrapper chain is an invalid access in this static model, not an observed public Save crash. Do not introduce deliberate C++ undefined behavior to imitate it.

The original outer sequence is unambiguous in the assembly:

1. Resolve material manager once at `0x140058ce6`.
2. Resolve operation model at `0x140058cf8`, acquire and snapshot signed count at `0x140058d00`.
3. For each position `1..n`, reacquire operation step through `0x14005f7a0` at `0x140058d34`, then nth material at `0x140058d40`. Null material/type 2 skips everything remaining in that iteration.
4. Copy the material number before manager virtual `+0x178` path generation at `0x140058d8a`; convert that path to local 8-bit bytes.
5. Reacquire operation model at `0x140058dc6`, get its step at `0x140058dce`, and call material writer at `0x140058df3`. Ignore writer status and continue.

Thus total operation-model calls within this helper are `1 + visited positions + eligible positions`. Step-holder accesses include the count wrapper's one-or-two accesses; these are distinct from operation-model calls.

Retained hashes newly checked: `out_s/s1c.c` = `ced227e5400eb85c544c347571e54058b9ca6c62045ceaf02f9c7aeb1bd8bf1c`; `out_s/s1.c` = `e4e78f5ef1c94d103e50466632dffc363dc6389cbb9e1ff9bba10ea4abe75f27`; `out_s/s2.c` = `3f59385ca2fffc3886cc91c140580255dde9e308c0698f5e33d9f1d793ecd0c1`; `L0/A/a0a.c` = `15f0b16099c5f0e841600b1773d1c88ac0ddccdefdd26d33140bf59245bc9f41`.

## Public Save reachability and limits

Current production hash is `36b3b12693fd5c2b3d4f4bde9ec486a9b673f7575da4b6d94903de19ceb0cf8b`. `save_current.cpp:639–654` caches one step. It therefore differs structurally even when its output agrees.

Both production call sites are on Save paths:

- `core/convert.cpp:334–347` configures native Save and calls `engine::saveCurrent`; `save_project.cpp:44–69` checks opened state and last-operation rules, chooses active operation, then dispatches `saveCurrentOperation`.
- `save_current.cpp:749` checks class/current/project and dispatches class Save work. Base/express file synchronization reaches `syncOperationFiles:711–747`, whose initial `opModel` null guard precedes folders, SimCtrl, interobject, object, mesh/BCC/property/equipment work, and finally `saveMaterials:734`. The retained original `out_s/s1.c:1–91`, `0x140057370`, likewise guards its initial model before these calls.
- Convert2Dto3D's `l5_slots.cpp:1032–1081` has an initial current-operation/model guard and a subsequent step guard before conversion-file work and `saveMaterials:1075`.

These guards establish a prior successful lookup, not successful lookup at every later occurrence. They do not prove immutability across intervening calls. However, the only production `classModel` override found in `src` is `ops/cycle.cpp:86`; Cycle and Group explicitly skip task-file synchronization (`save_current.cpp:825–828`). The public C adapter's native-write callback (`convert.cpp:267`) only compares output aliases and throws. It does not replace models/steps or deliver an external caller callback. No public ABI model-replacement facility was found on this bounded path. No asynchronous mutation or native event-loop reentry is established by this investigation.

Therefore public Save reaches the repeated-lookup helper, but input-driven replacement/null-wrapper divergence has **not** been demonstrated. A direct test callback that swaps `op.engineModel` is useful mechanics coverage and must be labelled synthetic; it is not evidence that a caller of the public C API can do so. An absent class fails earlier dispatch/model gates; simply constructing an absent-class helper test does not establish public reachability.

## Smallest faithful source and test change

Implement only the supported repeated-access structure in `saveMaterials`; keep this distinct from a null-wrapper policy decision and the InterObject borrow-only cut. Do not redesign public admission or add rollback.

1. Snapshot count from one freshly resolved model, applying the native present-wrapper/null-step zero-count rule. Do not retain that model/step borrow for later iterations. Existing project material-manager access has no separate effectful lookup equivalent; do not invent new manager effects.
2. At each position, freshly resolve model/step, retrieve nth material, and copy only filter result and number out of a synchronous local scope. No material/step pointer should survive path construction or another model resolution.
3. Generate the default path, then freshly resolve model/step and synchronously invoke the existing writer with that current step. End this borrow when the writer returns. Preserve the original saved count, position order, duplicates, type-2 exclusion and ignored write failure status.
4. Keep null-wrapper and null-step-during-a-positive-loop handling explicitly qualified. Retaining a silent no-effect guard there is a deliberate unresolved divergence; replacing it with an unsupported error is also a policy difference until justified. Do not claim either choice recovers native failure behavior. The supported stable-owner path can be restored independently; no new undefined dereference is required.

For tests, use the existing `mst::OpClass` subclass/registry fixture seam demonstrated by `tests/s/test_save_dispatch.cpp:12–74`, with a `classModel` override counting calls and retaining owned model A/B. Do not add a production callback just for testing. Directly call `saveMaterials` to isolate the helper's exact call count and ownership:

- Stable model containing eligible and type-2 materials: assert `1 + n + eligible` operation-model calls, exact ordered paths, nonempty KEY bytes and unchanged exclusions. Preserve CP184's detached-step test; assert one model call and no writes when initial count-time step is absent. This checks the recovered count-zero branch, not public null admission.
- Script count from A, nth position from A, writer from B with the same number and distinguishable payload; keep both owners alive throughout. Assert B's payload is written. This distinguishes writer reacquisition from traversal reacquisition and avoids a dangling pointer.
- Use two positions and return B for the second nth lookup; give B a smaller count but valid second linked position. Assert traversal obeys initial A count, consults B's nth position, and copies the number before the next lookup. If fixture invariants prohibit count/list disagreement, instead give B the same count with different ordered numbers; that still proves reacquisition without malformed state.
- With `nativeTaskKeys`, make writer-time B lack the selected number, seed a sentinel target under `M2M_TEST_SCRATCH`, and assert no callback/open/truncation for that iteration; then assert a later eligible material is attempted. The existing `saveMaterialKey:620–634` and retained writer ordering resolve number before opening and ignore return at the loop. Do not assert writer-time B's type filter: filtering belongs to traversal-time material A, while writer selection is by number in B.

The writer's synchronous `beforeNativeKeyWrite` runs **after** number resolution and before file open. Reacquisition once per writer does not protect a step/material destroyed inside that callback. Test owners must outlive the whole writer; do not reset `unique_ptr<DB_STEP_DATA>` during active serialization, pin all Save models, or claim lexical lambdas prove reentrant ownership safety. The production public alias callback does not destroy ownership. A separate same-occurrence lifetime investigation is required before widening that assumption.

Use Testkit-supported assertions, scratch paths and existing constructor/list ownership rules. Focused target is `m2m.s.save_current` (plus dispatch target only if changed). Root owns build, grouped checks, full source-stable suite and freeze. No test was executed here.

## Exact remaining evidence

A public behavioral mismatch claim needs one Open–Save–Close occurrence with caller identity, Save-stage boundaries, model identities and step identities at `0x140058cf8`, every `0x140058d34` and eligible `0x140058dc6`, plus a demonstrated in-boundary cause of replacement/null before that occurrence. For null-step failure claims, distinguish initial zero-count null from later null after a positive saved count. Record which earlier Save files already changed and whether project/properties/MST stages execute afterward. A detached-step helper fixture, multiple separate reference runs, or source-level synthetic override cannot supply this missing same-occurrence evidence.

Architecture restoration is justified statically now. Native public null-model/fault equivalence and reentrant lifetime closure remain unresolved; do not claim them as repaired by repeated calls or pointer scoping.
