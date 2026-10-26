# CP128 independent review: isolated FeatureMap allocation prefix

## Result

The frozen candidate models the first bounded native map-constructor prefix through an actual candidate-process HeapAlloc call and the two pointer stores in source order. It correctly stops before sentinel construction and does not present the isolated map as a Project-owned or original-global object. I found no mismatch in the admitted nonnull allocation path. The null-allocation branch is intentionally parked before unqualified CRT new-mode/handler behavior, so it is not a complete native failure result.

## Native call and store comparison

The retained original `DEF_GUI_MO_64.exe` capture `analysis/resume114-feature-constructor-1412f1060.asm` (binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) shows:

1. Constructor argument `M` is saved in RBX.
2. At `0x1412f1076`, it calls operator new with size 8.
3. On a nonnull returned `A`, `0x1412f1085` stores `M` at `[A]` (the proxy backpointer), then `0x1412f108c` stores `A` at `[M]` (the map proxy slot).
4. The next call is the sentinel helper at `0x1412f1092`; only after that returns does the constructor publish its head/count fields.

The bounded allocator capture `analysis/resume119-feature-allocator-14147489c.asm` shows `malloc` reading the current module heap slot, setting `R8` to the requested size and `EDX` to zero for `HeapAlloc`. For this constructor's size 8, that is `HeapAlloc(heap, 0, 8)`. It has size checks and a minimum-one normalization; none changes this positive size. If HeapAlloc returns null, allocator handling continues at `0x1414748ff` into the new-mode/new-handler/errno path rather than immediately returning a constructed map.

Candidate `FeatureMapProxyCall::resume` reads the live candidate heap slot under the owner lock, records that actual candidate handle, calls `HeapAlloc(heap, 0, 8)`, and only on nonnull return placement-constructs `ProxyNode{&map}` before assigning `map.proxy = proxy`. Its effect order is candidate slot read/null test, HeapAlloc entry/return, backpointer store, proxy publication. The tests check the actual returned candidate block's backpointer and the standalone map's proxy slot, call arguments, one-shot behavior and that head/count remain unknown. This faithfully expresses the source prefix for a successful call while keeping candidate and original heap identities distinct.

## Ownership and retention

Preparation requires the genuine retained `FeatureCrtRuntime` lease, an actual published nonnull candidate heap and completed candidate producer state. It cannot synthesize a heap from declared metadata or invoke the producer lazily. It prepares host call/control-block/map storage before the later source-role HeapAlloc. The runtime retains one `FeatureMapProxyCall` strongly; the call owns stable standalone `M` storage and weakly references the runtime. This avoids a strong cycle while retaining `M` and its `A` backpointer target through the process-retained runtime. A second distinct call is refused by a candidate-only experiment-capacity guard before HeapAlloc; resuming the existing call cannot retry the allocation.

The candidate map is not the native inline `Project+0x60+0x1458` context or the `M` pointer supplied at native `0x1412f1060`. It is only a standalone layout-compatible value used to verify the two stores. The project header and Project constructors contain no `FeatureMapProxyCall`/`FeatureCrtRuntime` member or hook. A source search finds the new types only in their private implementation and focused model test; default Project construction therefore does not allocate, retain, or use this map experiment.

## Failure and cleanup boundary

If the candidate heap slot is null, resume stops before HeapAlloc at the separate native null-heap fatal frontier. If actual candidate HeapAlloc returns null, the candidate records an allocation-failure-pending state and stops before `0x1414748ff`'s unqualified CRT handling. It does not synthesize `bad_alloc`, invoke a host handler, retry, return a native operator-new result, or publish a null map proxy. This is the safe boundary supported by retained source; the actual native null path could continue, throw, retry or otherwise mutate CRT state and is not claimed here.

After successful stores the next native operation is the 48-byte sentinel helper. The candidate stops before it and leaves `head` and `count` unknown. The native later constructor stores and state-0 EH cleanup are not reached. The candidate destructor performs no `HeapFree`/`HeapDestroy`; the block remains on its candidate heap, retained until the owned test process exits. That process-exit reclamation is candidate instrumentation, not native CRT teardown or a public Close outcome.

## Qualification

The CP128 source-owner receipt reports static preflight only; no compiler, test, live heap, GUI, or original process was run by that worker or by this review. The retained instruction evidence is static. The positive candidate call, if later run by root, proves a real HeapAlloc call in the candidate process against its candidate heap, not observation of original global `0x144700810` or same-run native parity.

Reviewed against `analysis/resume128-feature-map-prefix.md` and its frozen source hashes:

- `source/apps/moproj2mst/src/model/feature_crt_runtime.hpp`: `2dfd0cad50f484565b71a183fc5fa8d2a1e8084711d1652f523567976d2384a7`
- `source/apps/moproj2mst/src/model/feature_crt_runtime.cpp`: `2d46a0b5a975d7bb345ac63ab590245ccf60944596f75cb6d9cb8c3c89377ff1`
- `source/apps/moproj2mst/tests/model/test_feature_crt_runtime.cpp`: `69aba7a5fd41878e4b4c4e521bcd0649752119ada902e17c717e17086ee9209e`
