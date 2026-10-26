# CP131: isolated empty feature-map destruction prefix

The smallest source-closed cleanup cut after CP130 sentinel completion is the **empty full-range map clear**, stopping before sentinel free12f08b0. It rewrites the actual sentinel links/count even when no nodes exist. It requires no feature provider, optional context field or Project/Close admission. This is an independently callable isolated-map prefix, not an automatic destructor or the enclosing context-constructor lane.

Only this report was written. No source/build/test, GUI/original execution or mutable REA database. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses below use the common14 prefix, e.g.12f0860 means1412f0860. Evidence is retained DUMPBIN14.44.35228.0 assembly corroborated by section-mapped immutable PE byte hashes.

## Exact native order

Standalone map destructor12f0cf0 calls12f0860 at12f0d06, then loads M.proxy and tail-calls14dfc68 at12f0d14. The normal enclosing context destructor12f0d20 instead performs its optional C+1450 work before calling the same12f0860 at12f0d81, then frees M.proxy through its own tail. These caller sequences are distinct; neither is permission to skip the context prefix or enter accepted Project Close.

12f0860 builds begin/end iterator pairs from actual M.proxy=A and M.head=H (begin node H.link0, end node H), then calls12f06b0 at12f08a7. Its full-range path checks nonnull/equal iterator owner A and begin/end matching the actual first/head pointers;12f0715 calls12ee410. This map-clear fast path is selected before the generic equal-iterator empty range path. Do not omit it merely because begin==end on an empty map.

For the actual completed empty sentinel:

1.12ee41f reads M.head;12ee426 reads H.link8 as root. CP130 initialized it to H.
2.12ee42d tests root.nil29. Actual H.nil29=1 takes12ee431 ->12ee453, skipping the entire node/free loop. No key+18 or payload+20 read/deletion is needed.
3.12ee453 reloads M.head;12ee45c writes **H.link8=H**.
4.12ee460 reloads M.head;12ee464 writes **M.count=0**, then12ee46c writes **H.link0=H**.
5.12ee46f reloads M.head;12ee478 writes **H.link10=H**.12ee481 returns.
6.The full-range caller reloads M.head and its link0 at12f071a/71e, then writes the output iterator node at12f0721 and owner A at12f072a before returning. These are local iterator outputs, separate from map fields.
7.12f0860 reloads the sentinel at12f08ac. The proposed prefix stops **before12f08b0 ->14dfc68**. It neither frees H nor executes the later M.head=null/count=0 stores at12f08b8/bc, and it never reaches proxy free.

The count store is between link8 and link0 resets, unlike constructor order where count is last. Empty map destruction is not a no-op and is not equivalent to declaring a container absent. Nonempty node traversal remains guarded: native clear recursively frees right subtrees, saves left, frees the current node and continues left; payloads/borrowed keys are not automatically destroyed by map storage clear.

## Actual isolated owner and proposed API

CP130 already retains a standalone M in FeatureMapProxyCall::Storage, proxy A allocated from the actual candidate runtime heap, and head H recorded separately even before publication. Successful resumeSentinel initializes H's three links, byte28/nil29 and M.head/count, then returns the same M. No arbitrary receipt pointer or imported native address is needed for this empty clear.

A future assigned cut can add an explicit private `FeatureMapProxyCall::resumeEmptyMapCleanupPrefix()` plus an immutable cleanup report/cursor. It should require the actual same retained runtime/call, completed M constructor, live original A/H allocations and actual initialized empty shape. Validate host admission before source effects, retaining the existing owner-before-call lock order. Host readiness guards are not invented native branch tests. During the source phase, use only known fields: nil/root and real iterator owner/head/first identities; never inspect opaque H+18..27/suffix or M's uninitialized bytes.

Record the native empty branch, each actual M.head reload, link/count stores, local output iterator and return, then latch a new boundary before sentinel free. No HeapAlloc, HeapFree, HeapDestroy, provider callback, host allocation or registration is needed within this phase. Keep constructor/proxy/sentinel receipts separately inspectable and unchanged as historical observations. Repeated resumes must not replay stores; premature/incomplete, mismatched or nonempty/unqualified frames refuse before the first effect. A declared invalid/failure projection may test guard ordering but cannot authorize writes to fabricated handles or addresses.

Admission must be exclusive with the context-constructor lane: this isolated destructor attempt cannot be silently interleaved with handing the same M to an enclosing C/Project. Once its cleanup cursor starts, later constructor/context adoption must refuse; neither a pause nor an unavailable report is a native throw/EH occurrence. No shared_ptr destructor, inspection-lease drop, process-retirement hook or Project default constructor may invoke it. The existing process-retained runtime remains responsible for retaining A/H at the free guard.

The minimal future write set is existing `src/model/feature_crt_runtime.hpp`, `src/model/feature_crt_runtime.cpp`, and `tests/model/test_feature_crt_runtime.cpp` beneath source/apps/moproj2mst. These overlap gradient_state's context work, so root must reserve/reconcile after its source freeze before any edit. No source edit is proposed concurrently here; no model.hpp/Project/context fields or public Close callsite is required.

## Tests and later guard

Extend the actual retained-map test or a separately admitted actual frame to verify: premature attempt does no work; completed empty constructor takes the full-range/empty-root branch; exact reset order link8,count,link0,link10 plus actual head reload/output-iterator values; no new allocation/free/provider calls; same M/A/H identities and opaque bytes still unavailable; repeated resume does not replay; dropped/reacquired inspection leases preserve owner; and context adoption/replay is excluded once this cleanup phase starts. Negative shape/ownership tests must be model-origin guards rather than deliberate heap corruption. Root target remains m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime; no tests ran here.

Sentinel free is the next independent effect:14dfc68 forwards to1471b7c, which reads the current CRT heap and calls HeapFree(heap,0,H). A failed free enters errno/GetLastError mapping before returning; an ignored caller return is not proof of success. Only after that call returns does native12f0860 null M.head/count. Later proxy free has no M.proxy=null assignment. Preserve the allocation owner, module/heap lifetime, partial failures and no-replay state before implementing these effects. Do not use automatic delete/free or clear host pointers ahead of source order.

## Qualification receipts and Close limit

End-exclusive original byte hashes:

| Range | SHA-256 |
| --- | --- |
|1412f0cf0..1412f0d19|`4943ebf4417fcb30e864b0b172dad1017561242facc0631e0dc4299717c0275c`|
|1412f0860..1412f08c6|`0e9b657ce10423f951db890b120cf5bc1e090f4e5882d05655cf1b2ba3291fce`|
|1412ee410..1412ee482|`4757db15cae55631c86d285c8d0f85a2686bd9b4202d9dddb11609904e14e097`|
|1412f06b0..1412f0733|`bc65eeede68029abec1e4bf3459a0f690edadd14517d2e60b06e7ad8d7766d41`|

Retained ASM files under analysis/: resume116-feature-map-unwind-tail-1412f0cf0.asm SHA `81a3fcc307651a95473c93dda2ffb73805268a43f0bc0cc9c1c7cc7dcc49d5e7`; resume116-feature-teardown-1412f0860.asm `33c7dff2017500b9262865f4026bce72d2ea3dda460e9a6e1ddd43cdba41b005`; resume116-feature-node-free-1412ee410.asm `43f685cd56cffb282764d08949422ec939b2aeb9e9e6f6b0808ac9bf850ec96a`; resume116-feature-owner-edges-1412f06b0.asm `1a6e0951d126f5c6c728b585e4871be42dfdae7ef64da21957c7c761c29b6849`.

CP130 source release report SHA `42a047df675841bf0734a5e7b2feeb53e14a50cfb4b397318f9667319e3e4aa7`; inspected runtime header/cpp `053ee825f92374cd7a61b88d83381b2ebefb3fcc2bdcb5aca657c998b8b72903` / `cc9c9a72ca0302148615ba51f384a51a291f40bedabda218b6a51dfe9f43e3d7`. Existing resume116-feature-owner-lifetime.md and resume119-feature-context-proxy-allocator.md retain context/EH/provider/free qualifications.

Normal Project destruction still calls context cleanup only after its diagram/SubProject predecessors, and accepted Close still requires the actual application/main/busy/modal/nested Save/manager chain. An isolated empty-M prefix does not supply C+1450, provider registration history, gradient context construction, native EH, Project feature admission or public Close. No reconstructed executable/deployment was inspected or changed.
