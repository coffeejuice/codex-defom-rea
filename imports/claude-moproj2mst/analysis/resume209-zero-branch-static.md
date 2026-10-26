# CP209: B98 producer-zero branch discrimination

CP207 establishes a zero return from the selected `1d510` producer before the same fault-path invocation fails. Static evidence does **not** select either explicit null-return arm in `45780` for B98. Both remain feasible against the retained observations. There is also a downstream allocation-null path in `44a90`, so producer zero alone must not be reported as proof that an early arm ran.

## Evidence qualification

The original is installed DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the installation version is not a version-resource reading. This report reread the matching copied PE offline. No original execution, GUI session, mutable REA database, source/build/deployment change, or external-project write occurred. Only this report was added.

Primary runtime receipt: `analysis/resume207-original-function-owner-2026-10-26T11-51-11-398Z/summary.json`. Its `hookEvidenceQualified=true`, `pairQualification` identifies one unique exact-caller same-creation/thread/invocation pair, and `causalClassification=same_fault_path_producer_returned_zero`. Selected facts are PID 31636, creation FILETIME text `134374890722592589`, TID 31032, invocation `2`, `O=0x3e34320`, producer input `RCX=0x10fe2d0`, exact producer caller return app RVA `eb000`, and `producerP=0`. The contemporaneous pre-store reads succeeded: `H=0x3e1eb00`, `[H+38]=0`. These are pre-store diagnostics, not a witnessed store. The qualified AV remains QtCore RVA `32c0`, read/RCX `58`, with the CP205 caller path. Cleanup and process absence are verified by the receipt.

The static basis is `wave3-closure-next-14001d510.asm`, `wave3-boolean-close-1400133a0.asm`, `wave3-closure-next-140045780.asm`, `wave3-closure-next-140044a90.asm`, `wave2-doe-output-default-task-current-selection.asm`, and `wave3-close-model-owner-identity-contract.json` under `analysis/`. CP209 independently reread RTTI descriptor bytes and DUMPBIN views of `c78ad0..c78b4a` and `c1aef0..c1aefd`. DUMPBIN version 14.44.35228.0 was used. The current-getter unwind region is `140c78ad0..140c78b4b` end exclusive, code SHA-256 `75a3975eaaf6f46c1f07b99f7c1a285e87594ff927fc3583166bc469c4b829e6`; unwind bounds are not proof of a whole logical procedure. Addresses below are app RVAs unless explicitly written as preferred VAs.

## Typed path and possible outcomes

`1d510` calls `133a0`, then passes its result unchanged to `45780`; no later instruction replaces the return before `1d510` returns. `133a0` reads `[input+50]` and uses `146f8fc` with source descriptor preferred VA `1435693f8` (`.?AVSfModelManager@@`), target `143569420` (`.?AVSfMoModelManager@@`), zero displacement and no-throw argument. The retained native CRT qualification identifies this as `__RTDynamicCast`. The second cast in `45780` uses source `143569448` (`.?AVSfModelBase@@`) and target `143569470` (`.?AVSfMoModel@@`), again with zero displacement and no-throw argument. Descriptor names were directly reverified in the copied PE.

| Stage | Actual predicate/effect | Consequence |
|---|---|---|
| `133a0` | Downcast `[input+50]` to concrete Mo manager | Supplies manager `M` to `45780`; no null check before current getter |
| `4578e` | Call `c78ad0(M)`, obtaining current-model result `C` | `C=0` takes `457a0` and returns zero |
| `457c1` | Downcast nonzero `C` from SfModelBase to SfMoModel, obtaining `D` | `D=0` takes `457d3` and returns zero |
| `457dc` | Call `44a90(D)` when both previous results are nonzero | Returns Project field `D+19c0`, including its lazy allocation path |

`c78ad0` is not a raw current-pointer load. `c1aef0` is a direct qword getter; calls with `M+20` read the guarded current cell. `c78ae2` reads it, null takes another read at `c78af5` and return; nonnull takes another read at `c78b05`, then membership-related helper `2178c0` over `M+18`, followed by `4a20`. A zero test result at `c78b2f` explicitly returns zero at `c78b34`; otherwise `c78b41` rereads `M+20` for return. The retained manager-owner qualification identifies the `+18` collection and guarded `+20` current member. Thus the first `45780` null arm can mean absent current selection **or** failure of its collection-membership validation. A single late read of `M+20` would not prove which internal getter arm occurred, and repeated reads must not be assumed atomic in a mutating runtime.

`44a90` returns existing nonzero `D+19c0` immediately. If absent, `44ac8` calls allocator `146eabc`; its explicit zero-result arm at `44ad8 -> 44b55` stores zero through `44b75` into `D+19c0`, which `44bbb` returns. On nonzero allocation it calls Project constructor `89880` and stores its result. This report does not close allocator/constructor semantics or show that allocation failed in B98. The explicit downstream null path remains unexcluded by CP207.

A null first downcast is not itself a clean null-producing branch: `45780` immediately calls `c78ad0`, whose getter dereferences `M+20`. On the ordinary intact no-concurrent-mutation path, normal return from `1d510` excludes that failed-manager-cast route as the direct explanation, because a null manager would attempt a low-address read before returning. This is an instruction-level constraint, not a captured concrete manager identity or lifetime claim.

## Why B98 does not choose the arm

B98's bytes and admitted operation families establish file identity and domain, not the live manager selection, collection membership, or actual dynamic type at the refresh callback. CP207 observed no `133a0`, `c78ad0`, second cast, or `44a90` return. Its two owner entries and selected pair do not supply those operands. The static Mo manager factory relationship in `resume77-close-main-wrapper.md` qualifies the normally constructed manager type; it does not authenticate the field value in this captured occurrence. Likewise CP208's public converter success cannot establish original callback selection state. Therefore neither `C=0` nor `C!=0,D=0` can be excluded, and `C!=0,D!=0,44a90(D)=0` remains a further unexcluded outcome. No code change follows from assigning one of these as B98's cause.

## Exact residual witness

The minimum discriminator for the **two early arms** is the actual `c78ad0` return consumed at `45793`, associated with this same nested `45780` occurrence. `C=0` selects the first arm. If `C!=0`, retain the actual second cast return consumed at `457c6`: `D=0` selects the second arm; `D!=0` proves neither early null arm ran. Then the actual `44a90` return consumed at `457e1` is required to explain a zero producer through the downstream path.

Use occurrence identity `(creation FILETIME text, PID, TID, eaf60 invocation, nested 1d510 invocation)` and exact callers: owner return `eaa7a`, producer return `eb000`, `45780` return `1d52b`, current-getter return `45793`, second dynamic-cast return `457c6`, Project-accessor return `457e1`. Preserve raw inputs/outputs, unreadable versus zero, event order, ambiguity/overflow counters, and the already-qualified first-AV path. These are logical observation coordinates, **not approval or a safe hook-placement prescription**; CP206's divergent instrumentation remains qualified separately. A function-entry/on-leave implementation would need its own reviewed prefix/relocation checks and nested caller pairing.

To distinguish subcauses after `C=0`, additionally retain this occurrence's manager argument, actual `c1aef0` return values at `c78ae7`, `c78afa` or `c78b0a`, `4a20` return at `c78b2f`, and final guarded-current return at `c78b46` when reached. To prove allocation failure after `C,D!=0`, retain accessor input `D`, initial `[D+19c0]`, allocator return at `44acd`, and final accessor return; a zero `44a90` result alone does not establish the allocator branch.

No further original run was performed. CP207 closes the former producer-zero uncertainty; branch selection and the precise selection/type/allocation cause remain runtime witnesses absent from the retained capture.
