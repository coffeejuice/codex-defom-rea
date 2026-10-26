# CP214 connector continuation: next TLS witness

## Decision

CP201’s paired, inert JNE successor descriptors are already integrated in the current connector source. Do not repeat that model increment or add a live branch/effect from it. The next connector action with independent behavioral value is one same-occurrence original witness for CP199’s CMP operand at QtCore RVA `0x29614`, sampled with the resulting flags before the JNE at `0x2961b`. Until that witness exists, retain CP200’s unresolved two-edge state and its guard; no successor instruction is source-closed for execution.

## Reconciled queue and current implementation

The RCONNECT entry in `analysis/takeover-state.json` still lists `resume201-tls-edge-static-next.md` as its latest queued connector item. Its `next_action` explicitly requires the exact same-occurrence CMP dword/ZF witness before choosing a concrete TLS edge, while synthetic `P+8`, TaskInfo, and public DBLIST remain guarded. The note’s proposed inert edge schema is no longer pending: current `doe_task_runtime.hpp` has `TlsIndexJneUnresolved::PotentialEffect`, both edge guards/targets, prospective effect metadata, and unresolved status; `task_factory.cpp` issues and validates the record without executing either edge. Current focused tests include CP201 semantic-value checks (around `test_doe_task_factory.cpp:9695-9700`, `9771-9805`).

This is a source reconciliation, not a new build/test/freeze claim. CP195’s branch refinement is about parent nullness and JE at `0x131438`; it is already implemented, but does not determine the distinct TLS state. Likewise, CP196’s symbolic `P+8` descriptor is not evidence for the TLS initializer’s `x`.

## Smallest original witness

Bind one natural original occurrence to the existing process/module lifetime, primary thread, WidgetRuntime/Session identities, and exact CP198/CP199 stack/call lineage. At the aligned initializer boundary, record:

1. the 32-bit value consumed by `cmp dword ptr [QtCore+0x2f08e8], 0xffffffff` at RVA `0x29614`, tied to that instruction and memory-read occurrence;
2. the resulting ZF (and enough full flags or an independently checked subtraction result to verify it);
3. arrival at JNE RVA `0x2961b` in the same uninterrupted occurrence, with the same read/CMP identity and no intervening flag writer.

Expected branch relation from the recovered bytes: `x == 0xffffffff` gives ZF=1 and fallthrough `0x29621`; otherwise ZF=0 and taken target `0x296c0`. Capture only the CMP/JNE boundary and required owner/stack identities. A later memory read, PE initial sentinel, earlier constructor, guessed initialization order, or a separately sampled ZF cannot substitute for the consumed same-run value. The existing source qualification is a static unwind-region listing; this proposed witness would establish only the sampled occurrence, not universal TLS initialization or all call paths.

If an owned original capture cannot reach this boundary under the available non-GUI route, stop at the first missing prerequisite and preserve the guard. Do not probe a new GUI-control session under the documented lifecycle restriction, and do not turn a candidate-supplied concrete value into production authority.

## Candidate implementation/test seam after witness

The narrow seam is the existing `advanceTlsIndexJneUnresolved` / `validateTlsIndexJneUnresolved` pair in `source/apps/moproj2mst/src/doe/task_factory.cpp`, with the private record and tests in `include/m2m/doe_task_runtime.hpp` and `tests/mdt/test_doe_task_factory.cpp`. A future branch-decision increment must accept only an immutable original-observation token privately issued for the exact existing root/grant, module digest, process/module lifetime, primary thread, CP199 read/CMP occurrence, and CP200 JNE occurrence. It must validate value width, recompute ZF from the 32-bit compare, verify the recorded same-occurrence JNE boundary, and then represent only the selected successor cursor. It must not execute the successor’s first instruction in the same change.

Required focused tests: valid zero/nonzero compare outcomes map to their proper branch; forged/copied/foreign/stale/mismatched read, flags, module, thread, stack frame or JNE occurrence refuse atomically; unobserved/unknown input remains unresolved; reentry is stable and creates no new read; rewind restores the exact CP200 predecessor. Keep all native observations out of synthetic test authority. Root retains source-stable build, full regression and freeze ownership.

## Limits

This lane made no original capture, GUI-control call, build, test, source edit, shared-plan/state edit, or mutable REA operation. The recommendation is a next-evidence contract only; it does not assert that the original CMP value is currently available or that a concrete successor has been observed.