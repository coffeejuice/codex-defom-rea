# CP119: the pending proxy needs the EXE CRT private heap

A bounded **success-only actual proxy prefix is source-closed**, provided a real process/module CRT-heap owner is bound and the actual allocation returns nonnull. It does not require guessing a new-handler result: that handler is not read on the successful allocation path. The current passive anchor has no such owner, so its default pending guard remains correct. Host `new`, `malloc` or GetProcessHeap is not evidence of the required backing heap, and no declared Boolean may stand in for a successful allocation.

The decisive new fact is the producer for native heap slot **0x144700810**: it is populated by **HeapCreate(0,0x1000,0)**, not GetProcessHeap. This is a process/module-lifetime resource, not a heap created/destroyed per Project or Close. No source was changed in this lane.

## Current anchor and exact pending edge

Frozen CP118 Project metadata reaches `ProxyAllocationPending` from actual initProject after its prepared Mo/slot/epoch validation, before the later adapter f10 write/backlink commit. The anchor preserves its original occurrence/model observation on retry and reports `HeapProfileUnavailable`; its completion/heap/feature/registration/Close traits remain false. Header SHA-256 `5653c60d568ee530be41aa35813d3f87cca418f5b9c94e9f7f34cf7ddefc99c6`; project_owned_storage.cpp `d8a98bcba68e52c8c1197a74df4a56a43874a91594be25f7aedb3dd7f24f69ac`. The actual C++ Project epoch is now available; native allocator/handler/CRT errno state is not.

The pending source call is12f107b→**14146eabc(size8)** inside map constructor12f1060(M), M=featureContext+1458. On normal return, that wrapper returns only a nonzero pointer A. Caller12f1085 writes `[A]=M`;12f108c publishes `M+0=A`; native EH state0 starts at12f108f, just before sentinel helper12edac0. It is a real separately owned8-byte proxy, not the48-byte head or a zeroed context. These pointer writes are after actual allocation return; do not publish them on an unavailable/failing attempt.

## Actual heap producer and successful allocation

The checked direct store to0x144700810 is at141484818 in **141484800**. It calls imported HeapCreate with flags0, initial size4096, maximum0. A null return remains a failed producer. A nonnull handle is stored before calling HeapSetInformation(handle, class0, pointer to DWORD2, length4); the compatibility request's Boolean result is ignored, and the helper returns1 afterward. Rejecting an otherwise live heap merely because that compatibility request returned false would add a native failure branch.

CRT startup **141470c28** calls this producer at141470ca3 and tests its result before later startup phases. Its failed branch enters diagnostics/termination-related helpers; it is not permission to substitute another allocator or silently create a Project-local heap. The producer and slot are tied to this EXE/runtime profile. Static file bytes or a heap handle from another module/process do not establish its current live value.

Wrapper **14146eabc** calls **14147489c**. For the bounded8/48-byte requests, its actual successful path is:

1. Preserve requested size; zero-size normalization/oversize branches are outside these fixed positive requests.
2. Read heap handle0x144700810; missing/null heap takes unresolved fatal/diagnostic work, so the represented prefix must require an actual live owner before that path.
3. Call imported **HeapAlloc(heap, flags0, requested size)** at1414748f1. Flags0 does not request zero-filled blocks. Actual nonnull return goes directly through14147492b to the caller; no new-mode, handler or errno operation occurs on this path.
4. Outer14146eabc sees nonnull and returns it, then map caller commits the actual proxy backpointer/publication.

No malloc/new success witness from a different host allocator can be converted into this receipt. A faithful bounded port may execute this actual HeapAlloc call against its source-profile heap owner and qualify the observed return, while preserving unknown bytes. Successful proxy/sentinel storage alone never supplies default PST feature values.

## Failure inputs and ordering that must remain guarded

The exact live inputs are different at each source occurrence:

| Input | Native source use |
| --- | --- |
|CRT heap slot0x144700810|read before each HeapAlloc/retry; same heap is used by free; owner/module/process generation must stay live|
|malloc new-mode DWORD0x144700858|read **only after** HeapAlloc returnsnull|
|encoded new-handler slot0x1447006e0|read by141481ce8 on each handler occurrence, decoded through1414813ec→DecodePointer|
|decoded handler/size argument/result|nonnull handler is called with the original requested size; nonzero means retry, zero means no retry|
|CRT errno owner1474134|native errno12 writes on malloc failure; not automatically the host UCRT errno slot|
|bad-allocation globals0x1446ffb20/+0x1446ffb38 and native exception runtime|outer operator-new failure after handler returns0; separate lazy construction/throw state|

If new-mode is0 after failed HeapAlloc, malloc calls1474134/stores errno12 at14147491a and then does another1474134/store12 at141474925 before returningnull. If new-mode is nonzero, it first calls the actual handler; a nonzero result retries from the heap-slot read, and zero reaches the latter errno12 store. Outer operator-new14146eabc then calls141481ce8 again after malloc returnednull. With a nonzero malloc new-mode, this can be a **second** handler lookup/call for one failed allocation sequence; do not collapse it into one callback. Outer nonzero handler result retries malloc; zero proceeds to native bad-allocation exception construction/throw. Handler callbacks can change live state, so a frozen startup enum or prior decoded-null observation cannot replace later reads.

On every handler occurrence141481ce8 reads the encoded slot, calls actual DecodePointer, skips invocation on decodednull, otherwise invokes it with size and normalizes its return to0/1. An on-disk zero or source initial value is not a decoded-null runtime witness. None of those dynamic states is supplied by CP118. A failing actual allocation must therefore park/refuse at its first unknown next read/effect, retaining the attempt and prior committed effects. It must not fabricate a null-success result, invoke an arbitrary current host new-handler or throw host std::bad_alloc as if the complete native branch had been reproduced.

## Sentinel, unwind and deallocation state

If the proxy prefix succeeds, the smallest next guard is **before**12edac0's48-byte request. No head/cache/feature field has been initialized yet. Native normal sentinel allocation initializes H+0/+8/+10=null, H+28=1,H+29=0, then the map publishes M+30=H, changes H+29=1, writes self links in+8,+0,+10 order, and finally M+38=0. Those later effects are a separate allocation/state slice.

CP114's EH evidence remains exact: a throw from that second request while state0 is active runs141b9c670→12f5e30→14dfc68 on the previously published proxy. It does not null M+0 or initialize any outer context fields. A staged unavailable report is not a native exception and must not trigger that cleanup or replay proxy allocation. Pending proxy ownership must be retained and explicit; later host adapter resource cleanup must not be labeled native constructor unwind or Close.

Free14dfc68 tail-jumps to **141471b7c**. Null returns before a heap read; nonnull calls **HeapFree(the same0x144700810 heap, flags0, pointer)**. If it fails, native code obtains the CRT errno pointer, calls GetLastError, maps it through14740ec and stores errno. The caller's ignored return is not proof that free succeeded. A Project/context destructor using a different heap, automatic delete or unconditional “freed” receipt would violate this ownership edge.

The two direct imported HeapDestroy callsites inspected use **other owners** (global0x144701508 and an object's+8 field), not0x144700810. They do not prove CRT-heap termination or permit destroying this heap at Project release. Indirect/transitive teardown remains unclosed; absence from these direct callsites is not a whole-program claim that the CRT heap is never destroyed. Its actual module lifetime must outlive all proxy/head/node/source-free operations.

## Recommended next concrete integration

Root can assign a bounded process/module CRT-heap owner prefix plus one typed proxy-allocation continuation. The heap owner must execute/bind the actual source producer (HeapCreate and ignored-result compatibility call), retain actual process/module/generation identity and expose a live borrowed heap to the **same Project's** pending context. Default/unbound profiles remain pending; do not read an original address from the candidate process, create a heap per Project, borrow another unrelated process heap or declare startup complete.

The Project continuation should retain the CP118 Project/Mo/epoch binding, execute exactly one actual HeapAlloc8 when admitted, and on observed nonnull return commit the proxy backpointer and M+0 once. It then stops before sentinel allocation with an immutable allocation-owner receipt. Failure advances only to a represented next failure stage; unknown new-mode/handler/errno/exception state guards without retry or rollback. A source attempt cannot be restarted by rebinding a new heap handle/generation, Project or model. A separate typed allocation receipt should distinguish new actual effects from the passive anchor's own historical “establishes no native heap effects” contract; do not turn its static false admission traits into constructor/Close success.

Required tests for that cut: actual private HeapCreate argument/profile and handle binding, compatibility-false still-live owner, two Projects share the same process heap owner but have distinct proxy/epoch identities, actual8-byte allocation/backpointer/publication then sentinel guard, repeated resume does not allocate twice, stale/different heap/process/module generation refuses before allocation, declared allocationnull stops before unknown new-mode with no proxy publication, first success plus unknown sentinel state retains proxy, and Project-storage expiry cannot authorize later writes/free through an escaped receipt. Failure tests must use explicitly declared backend outcomes, not invalid handles or process OOM; qualify them as model/order tests. Actual source-free failure/errno and sentinel-throw EH are subsequent branches, not implied passing cases. No provider, registration, PST zero-default or public Close admission belongs in this increment.

## Evidence binding

Original retained DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. `analysis/resume119-feature-allocator-inputs.json` SHA-256 `ff1ea36ace289559ebca9ef69c0f0811f7a0626f8ed6efd8c6216b3aa008cf51` binds exact import names/IATs, runtime slots, producer arguments, free contract, other-heap distinctions and all8 unique raw ASM hashes. Separate bounded capture receipts retain actual tool commands and .pdata ranges. The DecodePointer thunk's semantic span ends1414813f3; its explicit64-byte capture's later fragments are excluded. Existing CP114/116 constructor/sentinel/EH evidence remains unchanged.

Only this unique report,8 bounded ASM captures,3 compact capture receipts and the inputs metadata were written. No source, CMake, build/test process, GUI/original execution, live heap/handler read or mutable REA operation occurred. This identifies a conditional source-closed actual-success prefix and exact missing runtime-owner/failure inputs; it does not certify that the current candidate has executed it.
