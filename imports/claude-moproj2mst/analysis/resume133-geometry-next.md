# Geometry after node Stop: normal return, not another effect

There is no additional native assignment state or effect between completed node Stop and this reader's normal true return. On the fully admitted CP132 zero-gate profile, the reader return, outer which0 tail and coordinate-pointer return are statically closed. A new metadata-only pre-return helper would add no native progress. The useful next integration is exact-current-frame completion/retirement during the same initial borrow, followed by the existing coordinate/readiness path without replay. Later saved-frame resume and step-generation remain unqualified.

Only this new report was written. No source edits, build/test execution, GUI, original launch or mutable REA operation occurred. The audit used retained normal instruction views, bounded immutable PE bytes, current source and a bounded symbolic walk of the retained outer tail. This is static evidence, not an original runtime or successful candidate test result.

## Reader has no remaining work after Stop

The already reached call at `14082dc7c` stops the actual node timer. Its entire remaining normal path is:

```text
14082dc81  mov al,1
14082dc83  add rsp,98h
14082dc8a  ret
```

No model read/write, object assignment, timer, allocation, callback or cleanup lies between Stop and RET. The stop helper's elapsed result is overwritten by AL=1. CP132 has separately qualified that Stop's report body is empty. Thus `NodeSkipLoopCommitted` can qualify reader success only when its actual admitted loop and Stop really completed; merely having a low post-version, arrays checkpoint or parked frame cannot.

## The already-entered outer caller ignores that result

Outer `14082dca0` initially passed its gate while the actual step version was high, selected which0, set id11/flag1 and called the reader at `14082dd0b`. Return continues at `14082dd10`, which reloads which for the next comparison. It never tests reader AL.

For which0, all later nonzero-key/wildcard branches skip. At `14082e7ba` the outer reads the **current post-query** actual step version, compares it signed against 11000 at `14082e7c3`, and for this below-6000 profile jumps to the common epilogue. It returns at `14082e9df`. No later reader, name rebuild or model mutation occurs on this path.

I re-walked the retained decoded instructions starting at `14082dd10`, rather than relying only on CP70's former >=6000 examples. The constrained walk requires which comparisons to use the register loaded from the original which-pointer stack slot, models actual post-version at the one allowed getter call, and rejects unexpected calls/instructions/comparisons. It permits MOV/LEA/ADD, CMP, JE/JNE/JGE/JL/JMP and RET on the selected path. Results:

| Post-query signed version | Tail instructions | Comparisons | Calls after reader | Return |
| --- | --- | --- | --- | --- |
| 5999, 0, -1, INT32_MIN | 248 each | 81 each | only `140825420` stored-version getter | `14082e9df` |
| 6000, 10999 controls | 248 each | 81 each | same getter only | same RET |

The low-version interval takes the same version branch; no hidden threshold below 11000 changes it. This walk is symbolic control flow over the pinned normal body, not target execution. Starting a **new** outer invocation after the query would instead encounter its now-true low-version entry gate. Do not use that new fast return as a substitute for completing the already-entered reader/timer effects.

## Coordinate getter returns the existing optional-mesh data

Getter `140854af0` has already completed the real key0 COW/storage prefix before calling outer sync at `140854b46`. Once that call returns, `140854b4b..b58` loads the original collection receiver and returns its existing pointer at N+8. There is no further COW, allocation or translation of reader success into a different pointer. The reader operates on main LLOBJ arrays; it has not populated or replaced optional RGEOM coordinates.

The current candidate's cached path already uses `coordinatePrefixCompleted=true` to avoid replaying the first getter, then asks for connectivity. The same completion marker is appropriate only after this newly closed low-record reader/outer return. It would continue into the existing connectivity-null Skip or bounded populated mesh builder. A second getter round explicitly required by the builder remains a distinct native call; do not suppress it merely because the first one completed.

## Smallest useful integration contract

Root can review a viewer.cpp/test_viewer_box.cpp cut with this boundary:

1. Require the exact frame installed and completed by **this** initial callback, with CP132's admitted query, actual MAR table bounds/load, ordered assignment decisions, same-step owners/zero gates and actual node Stop. No generic presence, old stage or new invocation may authorize completion.
2. While the original prepared step borrow is still active, qualify the reader true return and the which0/post-version outer tail. They introduce no new native model effects beyond the closed current-version read. Do not call the full reader or outer helper again.
3. Retire only this exact installed frame, requiring model's current barrier identity to equal the local frame and the model's current step identity to equal the still-borrowed nativeStep. This is an internal same-invocation operation, not a public clear/resume API. Never dereference historical frame->step or grant later pointer authority.
4. Let this invocation continue with its completed coordinate prefix once, preserving current optional coordinates and connectivity. Retain all existing builder/layout/numeric/owner guards. No query/version rollback, cache repair or timer replay is permitted.

The callback's outer model/DB lease, actual View/optional geometry ownership and same-owner zero-gate helper remain live and unchanged through this serialized synchronous path. Its only helper effects are the already recovered table selection and timers; there is no admitted event dispatch, object deletion or optional mesh mutation. This supplies a scoped initial-call argument, not thread safety for external raw mutation.

Any refusal before complete Stop retains the barrier and existing parked behavior. Barrier retirement cannot be inferred from a changed global/version/Viewer/DB on a later poll. No new step-generation scheme is required for the initial borrowed interval; no current scheme would safely authorize a future saved-pointer dereference.

This is broader than adding another pre-return stage: it deliberately admits whole-reader/outer completion and then the existing optional-mesh consumer. If root keeps that completion outside the next checkpoint, there is no smaller native effect to implement after Stop; stop this lane at the report rather than adding another isolated diagnostic helper.

## Focused acceptance if integration is assigned

For fully admitted low-record zero-gate cases, the initial query changes version once, arrays checkpoint occurs once, node timer reaches Stop/count3, coordinate COW occurs once for the first getter, and existing preallocated optional coordinates remain current. Validate both connectivity-null edge-only Skip and populated 2D builder/scene contribution through the actual viewer invocation. Repeated fresh refresh after successful return is a new native invocation: below-6000 sync takes its entry fast return and must not replay this completed query or timers.

Foreign/null main owners, nonzero +0x87c, true assignment state, malformed MAR selectors, negative arrays count and incomplete Stop still park, with earlier effects retained and byte-exact effect-free retries. The broad parked mutation/owner-bypass assertions remain for those incomplete profiles. Do not simply rewrite all former pending expectations to success. A later guard in the optional builder must preserve completed reader/timer effects while remaining a separate failure, rather than reinstating and replaying the old frame.

## Source and byte qualification

Current viewer.cpp SHA-256 is `3ea1b91dcf933def29aca5ab253971b8aee3a4670c2dfef57a77732ca6c6e6fa`. CP132 implementation receipt SHA is `f9325898e1d61fb42391f17fd964d18c5530d065b02cc2f3fe24ad30ad212094`; it records the initial focused fixture failure, test-only setup repair and pending root rerun. This report does not relabel that run as passing.

Original EXE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Direct PE ranges below have exclusive ends:

| Range | SHA-256 |
| --- | --- |
| reader post-Stop `[14082dc81,14082dc8b)` | `abf6edcf823af1d7ea5d32708a4c942450a09b6873f4dbe959a9963193256892` |
| complete outer `[14082dca0,14082e9e0)` | `f985e7ddbd4218a68c500775f28e6f8e231ff0b43e96f557679927a023b10454` |
| complete coordinate getter `[140854af0,140854b59)` | `5457765a83e18b68d8ceda00e7c2d2a7af3517f4b28f510a2339454e7e7aed6b` |
| getter sync/return tail `[140854b1e,140854b59)` | `6967544d9fd118e088af9e2002bef64d3b7152053e43d88457d17d541f76f162` |

Retained outer ASM SHA is `d6f77fd7051cd4d3535e5b0d189fcc72868ae1a38e66aafb70f0092f5ebd17b5`; coordinate getter ASM SHA is `ae6438919d33ce3f1a2c7841fcf37c5afa89ef7c5bba796c453e9c73acd71697`. No runtime count, thread-exclusion or asynchronous generation witness was added.
