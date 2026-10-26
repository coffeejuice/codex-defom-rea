# CP171 R300 editor300: next source-ready boundary

## Decision

The next native step after CP163 is the actual Stopping step's selected-region length load at **`loader140828060`, RVA `0x140828248`**, reading `S.u2b28[11]`. The zero-length arm writes `S.u26c4[11]=1` at **`0x140828265`** and returns from the loader. That write is not a safe standalone implementation cut: a later invocation can take the already implemented cache-hit getter while the original call has not yet performed the arrays timer checkpoint, assignments to the main objects, node-timer Stop, or reader/outer return.

Therefore the next smallest meaningful source cut is an owner-bound, Stopping-specific pending-call barrier installed **before** the zero-length cache write, followed by cache1 publication and guarded refusal. Until the barrier covers all relevant entry points and model/step lifecycle paths, keep the CP163 refusal immediately after A publication. The length read itself may be recorded as observation, but it cannot advance the getter or justify cache publication.

This is read-only recovery. No source, tests, fixtures, build, executable, GUI/app-control session, live REA state, or mutable analysis database was changed or run. The only intended output is this report. The original is **DEF_GUI_MO_64 v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; no reconstructed executable/deployment was inspected. No public Open/Save reachability or runtime occurrence is claimed.

## Evidence and CP163 binding

The retained native loader interval `[0x140828060,0x1408283d5)` hashes to `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796`; the selected-length/cache branch `[0x140828235,0x140828275)` hashes to `f00c9d46a8a960e005bc09ce4ed3abb11314dc29b9e02bbe7bb27c32d9f628a5`. The latter compares the bounded region index's actual step length to zero; on zero it sets the matching in-step cache element and joins loader return. CP163's report `analysis/resume163-editor-after-query-next.md` has SHA-256 `9d4482e68a589be3632eef32ffcb43a5d81c50d47909e10c66e53da20f359e26` and binds the query, A producer, allocator/diagnostic order and limits. CP164's next-cut report has SHA-256 `0004bac2bb1b31e6d7915f1f5cdafdf29c223fdc7aa85f1992f3f88cccbba698`.

The actual Stopping reader is `[0x14082d7d0,0x14082dc8b)`, native SHA-256 `a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4`. Only after normal loader return does `0x14082d8d6` append `Assign the variable to the object data` to `g_timer_read_arrays`. It then traverses the actual main objects and assignment destinations; the selected Stopping node is not a substitute for those recipients. Node-timer Stop is at `0x14082dc7c`. The retained timer checkpoint `[0x140832370,0x14083242f)` hashes to `4eab6c32d8813886e4dfa2ddd8dc39d0f24f4a8b55c4f1267c3c6b8ff7b18dc6` and has the signed-count boundary described in CP164. These are static, bounded native-byte claims; no called-reader completion or exceptional-continuation claim is added.

Frozen CP163 source hashes, independently rechecked from this subtree:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp`: `31dc5b593077a7bace4a22255d58cf722481b9f90542d38f2f07e82b27324373`.

CP163's validation reports 361 registered tests: 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete. Its focused Stopping test reported 0 of 13 failures. Those results belong to CP163's frozen source; no tests were run for this report.

## Required barrier contract

The barrier must belong to the actual `GuiModel` and bind its current step, selected main object and canonical node set, leased `Project.dataDb`/reader, signed directory index and current physical allocation receipt, selected region 11, queried/post-query version, published A pointer, and observed allocation count. Installation must be no-throw or fail before cache publication. If installation fails, retain the CP163 refusal and cache0.

Every path that can consume or expose the pending step must check the barrier before cached success, low-version shortcuts, reset/reload, or public Save effects. The state must identify at least A-published, cache-committed, arrays-timer-committed, per-recipient assignment progress, and completed reader/outer return. No subsequent invocation may replay a query/allocation/cache/timer stage as though resuming the native stack. Keep unresolved state parked; retire it only with the actual model/step lifecycle, without freeing A separately from its real step owner. Changed project lease, reader, directory generation/receipt, step or node must refuse without dereferencing stale identities. The existing geometry barrier is evidence of a different path only and cannot stand in for this Stopping owner.

After a reviewed barrier exists, the next bounded behavioral cut may read `S.u2b28[11]`; for zero only it can publish cache1, advance the barrier stage and refuse before the arrays checkpoint. Positive and negative lengths stay cache0 and refused pending the DB/body reader. Do not infer body extents from directory sizes. The nonzero branch still requires the actual path/part owner, file handle, producer extents for header/MAR/A, selected span, partial-write/error order and normal-return versus exceptional outcome at `0x140b38d57`.

## Focused validation for a future implementation

Extend `m2m_mdt_simcontrol300_stopping_cached` / CTest `m2m.mdt.simcontrol300_stopping_cached` only after the barrier design and source review. Use the existing owned Project/ProjectScope, actual step and canonical node, current physical directory receipt, and real query/A producer. Cover selected lengths zero, positive and negative. For zero, assert one query and A producer, cache1 committed once, arrays timer byte-identical, no assignment or Stop, and pending stage retained. A fresh-page retry and each relevant model/session entry must refuse before cached success and must not replay query, allocation, cache write or timer work. Exercise reset/destruction, project/reader replacement and receipt retirement for safe refusal and no stale dereference/double free. Preserve CP161/CP163 lower-version, owner/index, header, count, cache-hit and existing-A controls. Then run the focused test with the existing Stopping snapshot/lifecycle regressions; no checks were executed here.

## Remaining blocker

The native zero-length order is resolved statically. The blocker is reconstructed-state ownership and interception: CP163 has no Stopping-owned pending-call state, and no evidence-qualified mechanism currently prevents all consumers from observing cache1 before the rest of the original call completes. This blocks the cache write and any later timer/assignment continuation. Original public acceptance additionally lacks one same-occurrence Open-to-InitialFill witness binding the live companion DB, selected step/node, query/version, loader return and actual reader exit; GUI control remains unavailable under the root workflow's release gate.
