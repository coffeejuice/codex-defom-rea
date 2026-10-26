# CP130 isolated sentinel and map-constructor return: source release

The existing isolated frame now has an explicit resumeSentinel phase that completes the success path through12f1060 returnM. It performs one actual48-byte allocation and the source-selected initialization/publication/self-link/count stores. The8-byte checkpoint remains separately inspectable. No Project binding, enclosing context, native unwind/cleanup, default hook or accepted Close is enabled.

Only the three reserved existing files changed:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/model/feature_crt_runtime.hpp | `053ee825f92374cd7a61b88d83381b2ebefb3fcc2bdcb5aca657c998b8b72903` |
| src/model/feature_crt_runtime.cpp | `cc9c9a72ca0302148615ba51f384a51a291f40bedabda218b6a51dfe9f43e3d7` |
| tests/model/test_feature_crt_runtime.cpp | `c26ca7daa2e91770dcc6526e4bb0be885b5c9f55ebbceca08f6c96a971f441ac` |

## Effects and retained ownership

resumeSentinel requires the same frame at SentinelPending, authentic canonical runtime lease, completed actual producer and prior actual proxy/backpointer publication. It locks owner before call, rereads the current candidate heap slot at this new malloc occurrence and calls actual HeapAlloc(heap,0,48). It does not take an arbitrary handle, report/model result or new owner as allocator authority.

A standard-layout48-byte SentinelHeader has compile-checked links+0/+8/+10, byte28, nil29 and opaque payload/suffix. An explicit empty placement constructor leaves native fields uninitialized. The positive path stores null into+0,+8,+10; byte28=1; nil29=0; records helper returnH; publishes M.head=H; stores nil29=1; reloads actual M.head and stores self-links in+8,+0,+10 order; finally stores count0 and reaches returnedM. All observations read only already initialized fields. Payload18..27 and suffix2a..2f remain unknown and are never inspected/default-zeroed.

The fixed20-event nested sentinel ledger includes the new slot check, actual allocation entry/return, five helper stores, helper return, M.head publication, nil transition, three reload/store pairs, count and returnedM. Per-phase raw arguments/counters remain separate from the original8-byte ledger. All host frame/report/retention storage is prepared before either source allocation; the new kernel phase contains no host allocation or callback. The frame explicitly retains the head allocation even before M.head publication.

Success sets candidateMapConstructorReturned, MapConstructorReturned, headKnown and countKnown with the actual returned identityM. The old static nativeMapConstructed=false declaration was replaced by the honest dynamic candidate-only return flag and a static nativeProjectContextConstructed=false boundary. The next guard is12f10e9/enclosing feature context unavailable. Original-global observation, Project admission, native unwind/cleanup and public Close remain false.

Actual48-byte null return parks at SentinelAllocationFailurePending before malloc's new-mode read1414748ff. It preserves A/M.proxy and does not publish head/count, return operator-new null, retry, invoke a handler, throw host bad_alloc, unwind or free. The declared failure projection is a separate model-origin sentinel report with no actual handle/head/owner. It cannot inject a simulated result into the real frame. Candidate slot read/attempt counters consume the phase so repeated pending calls cannot replay effects or overflow the ledger.

The same explicit process-retained runtime/call owns stable M, A and H. Dropping inspection leases does not free any native block or run a map destructor. The source constructor-EH proxy cleanup only applies if the later native constructor actually throws; a staged guard does not supply that event. No HeapFree, HeapDestroy or automatic native cleanup was added. Final process reclamation stays candidate instrumentation.

## Focused tests and static preflight

The existing runtime executable now has nine cases. The actual map case keeps the earlier8-byte assertions/checkpoint, verifies premature resumeSentinel performs no work, then explicitly executes the new phase. It checks real48/flags0/current candidate heap arguments, one actual result, exact20-event order, initial null links, byte28=1/nil0→1, actual final self-links/head/count/returnedM, unknown payload/suffix and unchanged proxy identities/ledger. Repeated old/new phase calls do not allocate again; dropping/reacquiring local leases preserves M+A+H. Capacity and producer counters remain unchanged.

The new declared sentinel-null case checks the48-byte argument model, first four source events, unavailable head/nil/self-link state and new-mode guard. It is not an actual kernel failure witness. The positive real test requires actual allocation success; environmental failure remains an observed pending state/failing prerequisite rather than being converted into a passing fake result.

Static preflight compared build/wave3-checkpoint129-verified/source.zip, SHA `376863e1dda250b2e8795da4c133d90c6557dda17ac48c43ddb091f57e27e837`. Existing producer constructor/attempt/report, canonical matcher, retaining factory, map preparation,8-byte resume/report and declared producer-projection bodies are identical. Checks additionally verified the exact48-byte call/store order, three actual M.head reloads, failure guard before initialization, fixed20-event capacity, no host allocation in the new phase, no free/destroy/default Project include, resolved quoted includes, balanced delimiter counts, supported Release-active Testkit macros/entrypoint and nine cases. No compiler, CTest or kernel call was executed by this lane.

Root target is m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime. No CMake/standalone/p11 linkage or public model edit was made. Public model.hpp matches CP129 SHA `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d`; Project storage cpp remains `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

## Source evidence and release

Design analysis/resume129-feature-map-next.md SHA `6dbaf98431d7a4c5e81549b149dc31dfeaf5d3121872ea805368af9caf1a8ac9` binds retained original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, sentinel ASM `08e448ba9345cf8a15f02465c0667b3518c520542de75516b576956290c764ef`, complete map-constructor ASM `cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc` and the separate allocator/EH qualifications. No original process or mutable REA capture was needed.

All three source files are frozen and released for root grouped validation/archive. Only this NEW receipt accompanies them. No build, test, live heap, GUI, original target or mutable REA execution occurred. Root owns actual compile/runtime outcomes; default startup/Project/context/Close and explicit resource retirement remain guarded.
