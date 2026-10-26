# CP158: synchronous free-zero needs the module FLS/PTD owner

**The next missing operation is errno provider `141474134`, not the error mapper.** Current ModuleHeapRole cannot complete a nonnull free whose HeapFree returns0. Installing a success-only deallocator would expose T/B/body32 to an unimplemented synchronous branch. The first scalar remains guarded at `140832adc` before `140832ae9`; the new FH3 body32 cleanup from `resume158-cowvector-publication-owner.md` has the same dependency during unwinding.

New bounded PE observations refine that dependency: lazy PTD storage is **HeapAlloc(flags8,0x2c8)** on the current module heap; resource failure uses no errno-output pointer but can enter live new-mode/handler and Sleep retries. A failed FlsSetValue releases the newly allocated PTD through the very same source free routine, so a failed cleanup free can recursively enter errno provisioning. The shared fallback errno word's retained initial value is **12**, not0. None of these branches was executed here.

Only this report was written. No source/build/test, original process, GUI/control session or mutable REA was used. Read source-qualified CP135/136/138 and the current lower runtime; independently decoded bounded immutable PE bytes with DUMPBIN14.44.35228.0 and existing `tools/doe_static_pe.py`. Original DEF_GUI_MO_64 **v11.0.2**, `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. VAs below are preferred image addresses, not live pointer authority.

## Exact free and errno branches

| Path | Required source effect and continuation |
| --- | --- |
| free(null), `141471b7c -> 471bb8` | Return before heap-slot read, HeapFree, FLS, GetLastError, mapper or errno store. It is not a successful HeapFree. |
| free(nonnull), `471b89..b92` | Read current canonical EXE heap slot `144700810`, call HeapFree(heap,0,pointer). Allocation provenance must match this owner; block identity is consumed once. |
| Raw HeapFree nonzero, `471b9a -> bb3..bb8` | Return without provider/mapper/errno. Source does not explicitly preserve the entry LastError; do not impose that extra contract. |
| Raw HeapFree zero, `471b9c..bb1` | Call provider, save returned errno pointer in RBX, then GetLastError at `471ba4`, map via `1414740ec`, store mapped EAX to that pointer, return. Raw0 is not allocation retirement; callers continue after the full routine returns. |
| Provider `141474134` | Call `1414814e8`; actual nonnull PTD returns PTD+0x10; actual null returns shared module fallback address `143b5e2c8`. It does not call the fatal-on-null wrapper `14148156c`. |

For free-zero, instrumentation/storage must already exist before HeapFree; do not insert logs, allocation, host CRT access or error-changing API work between the failed kernel return and source provider's first GetLastError. Capture thread identity beforehand. A retained historical raw0 report cannot later recover its original error from a new host GetLastError call.

Provider `1414814e8` saves incoming GetLastError at `4814f2`, reads module FLS index `143b5e990` at `4f8`, calls FlsGetValue at `500`, and finally restores the saved error through SetLastError at `558` before returning its PTD/null result. Consequently outer free's later GetLastError obtains the failed HeapFree error despite FLS/allocation/initialization calls in between, assuming those nested routines return. This is same-thread dynamic state continuity, not a stored numeric error supplied by a caller.

## Existing versus missing PTD

**Existing PTD:** actual FlsGetValue nonnull at `48150c` jumps directly to `481556`; SetLastError restores the saved value and returns the same pointer. No allocation, initialization, thread-id query or PTD field read/write occurs in this provider fast path. Errno selection adds0x10; the outer free then writes one int32 there. Other PTD words remain untouched on this branch.

**Missing PTD:** `48150e/511` selects count1/stride0x2c8 and calls `14147e440`. That wrapper calls `141486264` with **R8=0**, a nullable errno-output pointer. Exact count/stride cannot overflow. The allocator computes712, reads current heap `144700810` at `4862c3`, and calls HeapAlloc(heap,**8**,712) at `4862d0`, producing zeroed PTD storage on success. This is distinct from T/B/body32 flags0 storage.

On resource-null, `4862db` reads live new-mode `144700858`; nonzero calls current handler `141481ce8` and retries on a nonzero handler result. With errno-output null, its ordinary resource-failure tail does not call errno provider or store errno12. The overflow validation path does call provider/invalid-parameter machinery, but is unreachable for this fixed1*712 request. Do not generalize that exclusion to arbitrary calloc inputs. The outer `147e440` wrapper also reads live unsigned retry-duration role `1446ffbe4`, can Sleep at `147e485`, increment its retry counter by1000 with saturation, and retry allocation. This slot is outside the file-backed portion recognized by the PE reader; no current/initial runtime value was inferred from absent bytes.

Actual PTD-null allocation return at `481521` proceeds to common SetLastError and returnsnull, selecting the fallback errno word. Actual new PTD nonnull invokes **FlsSetValue(current index,PTD)** at `48152c`, before initialization. Nonzero registration calls `141481434` with RDX0, then GetCurrentThreadId at `481540`, sets PTD+8 to -1 and PTD+0 to the thread ID; common SetLastError returns PTD. Initialization has source side effects beyond errno: PTD+A0 points at role `142f06620`; +1c=1; +c8=1; bytes+174/+1f7='C'; +b8 points at `143b5f000` and its refcount increments under lock13; +c0 selects default locale pointer `143b5f6b0` and invokes `14147c760` under lock12. Those locks and locale/default records need owned lifetimes.

**Registration failure reentry:** raw FlsSetValue0 at `481537` calls source free on the new PTD at **`48154f -> 141471b7c`**, then sets local PTD=null and restores the *outer* saved LastError. If this cleanup free returns HeapFree0, it calls the same errno provider while FLS still has no newly registered PTD. That can allocate/register another PTD and recurse again on another registration/cleanup-free failure. The bytes contain no depth cap, no special host-errno fallback for reentry and no catch that discards the original error. A candidate owner must permit the recovered synchronous call graph and preserve each invocation's saved error; do not hold a non-reentrant bookkeeping mutex through it or invent a recursion cap. This is a static reachable edge, not a claim these failures occurred.

## Process/module and thread ownership

The canonical module runtime must jointly own heap slot `144700810`, FLS index `143b5e990`, callback identity `141481590`, fallback int32 `143b5e2c8`, startup lock table, defaults/locale reference state, writable mapper rows, new-mode/encoded-handler roles and retry-duration role. They are not owned by an individual StateVar/model or frame. Every live body/block needs retained release provenance after the allocation occurrence/model expires; T/B/body32 all dispatch through this same synchronous free owner, with separate block/construction states.

Startup `141481708` initializes encoded roles then locks, calls FlsAlloc(**callback141481590**) at `481723`, publishes its actual result to `143b5e990` at `729`, rejectsFFFFFFFF, allocates/registers/initializes initial PTD, and returns1. Failure cleanup `14148140c` conditionally FlsFree's the index, writesFFFFFFFF, and tears down locks. No current geometry/Feature runtime owns these objects. Borrowing the host UCRT's FLS index/errno or allocating an index with a null callback changes this contract.

Callback `[141481590,1414816c5)` conditionally frees optional PTD buffers at +38/+48/+58/+68/+70/+78/+80; handles +A0 separately against its default role; decrements/releases +B8 under lock13; releases +C0 locale/reference state under lock12; then source-frees the PTD itself at `4816b5`. Even a PTD created solely to write errno has the initializer's default references and callback obligations. A four-byte thread_local integer or partial PTD with a stub callback does not represent those owners. Thread/fiber exit and explicit current-thread detach `1414816c8` must preserve callback/storage lifetime independently of model destruction.

The fallback is **one process/module-global word**, initially PE bytes `0c000000`; it is not a second thread-local errno. PTD-null provider completion returns its address and outer free overwrites it. The initial mapper table has45 ordered pairs and is writable native data. Existing `mapRetainedFeatureWin32Error` correctly models only those retained initial rows; calling it does not observe a live original table or own either errno destination.

## Next useful increment and precise witness

There is **no source-ready live synchronous deallocator increment through the current owners**. `FeatureCrtRuntime` parks raw0 before provider; `ModuleHeapRole` has no free/provider surface; the pure mapper has no live TLS or errno side effects. Repeating an isolated positive HeapFree or saving an error number would not close the newly exposed owner/reentry graph. The smallest coherent implementation increment must construct the module FLS/PTD/lock/default-reference owner and install complete provider/free dispatch together; its source boundary is not merely a new method on Viewer. Additional static recovery is specifically needed for lock startup and locale/default ref helpers plus their failure cleanup, rather than another temp0/B8 candidate pass. Declared provider branch/reentry order models can be tested independently but cannot admit body publication.

A **small passive original provider witness** can reduce the remaining runtime uncertainty without deliberately causing HeapFree failure: observe the next natural call to `141474134 ->1414814e8` on the actual conversion thread. Retain process/module hash/base/thread/fiber generation, entry LastError at `4814f2`, actual current index at `4f8`, FlsGetValue result at `506`, saved-error restoration call/result at `558`, provider return and returned errno address PTD+10/fallback. For the expected initialized fast path, require actual nonnull PTD, no lazy allocation, exact returned address and unchanged relevant PTD/default identity. If the naturally observed branch is null/lazy, retain its actual allocation/registration/init/free order instead; do not declare it the fast path. Also bind startup's index publication and source callback to that same module generation. This witness supplies real owner/continuity inputs; it does not install a candidate backend or prove a failed HeapFree occurred.

For **actual free-zero validation** later, capture a naturally occurring failed free with its real block/allocation provenance, current heap and raw BOOL, then all of provider, restored failed-error, actual mapper rows/input/output, actual errno destination/store and caller continuation. Do not provoke it with invalid handles, corrupted pointers, deliberate double-free or forced OOM. Absence of such an event is an unobserved branch, not a success guarantee. Old paused failures cannot serve as that same-run witness.

**Guard retained:** before `140832ae9`, no first-body/slot publication, T nonnull free, completed leaf or automatic source-payload ownership. No host-errno substitution, destructor early return, hidden leak, mapper-only error completion, suspended C++ destructor or reclassification of old parked errors is admitted. Original parity and full lifecycle remain unclaimed.

## Byte and source receipts

Half-open PE ranges, bytes SHA-256; complete selected function bounds confirmed via `.pdata`. Imports independently decoded from immutable import descriptors: HeapFree `141e032f8`, HeapAlloc `141e03380`, GetCurrentThreadId `141e033e8`, FlsGetValue `141e03430`, FlsSetValue `141e03438`, SetLastError `141e03448`, FlsAlloc `141e03450`, Sleep `141e03590`, GetLastError `141e035e8`, all KERNEL32.dll.

| Range | SHA-256 |
| --- | --- |
| `141471b7c..141471bb9` | `f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696` |
| `141474134..141474154` | `82fd11be9f0076f075cafa1b9e5c1466415a7f017d285cda92bb723d53800482` |
| `1414814e8..14148156c` | `191922765c03256b33eb9d0b8c15ddbbd206a54449906a4f86409a1a11ffebfa` |
| `14147e440..14147e4c3` | `ef1dc429cac12c7d06f5cc0c0efbeeaa15ac58794368e8f3b161a3b79d1dc8de` |
| `141486264..141486313` | `04aa8dc25dfc21828e6bd324bef02bea3351ad527d65ea8d06769f24dd3eca1b` |
| `141481434..1414814e6` | `8fb5d7fc849d3ad80ba97f997e29e3d3308198563bcf4790bfeacbc6f62d42e7` |
| `141481590..1414816c5` | `b3145fdde39ef1c91cff69477eac4211d1e7136fb3f32c22b5b4f6c25de0bc31` |
| `141481708..141481789` | `2e60e5d74214a1cea2203d3f419be2bcc1a2434e8c0e69a15750a091631dda49` |
| `1414740ec..141474133` | `f44e86936fa3b398df977c4dbe968eff2c5fec42fd3c07a7dc4adc02d61bce17` |
| `143b5e160..143b5e2c8` | `dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe` |
| `143b5e2c8..143b5e2cc` fallback initial12 | `42f4aeb81c1ef81f771f3de8abca9dcf66901c575530e7672e4b1146474ae650` |
| `143b5e990..143b5e994` index initialFFFFFFFF | `ad95131bc0b799c0b1af477fb14fcf26a6a9f76079e48bf090acb7e8367bfd0e` |

Source/references rehashed: CP135 next `c5548eb7450556dc2324cf5c7c9665c3770743f10ff0956adb29557c747eda47`; CP136 next `da58f8912165bbe0e105e84053447bdbac0f76227c0a249e5780f99730eb0e6f`; FeatureCrtRuntime header/CPP `a9c07b8e044dcb15e27eb13e75e5443e67137c31477942e384cf02947c10c7a3` / `48434b8eeacccb7e36f00ecbd423536b333696d668a18568756f7946f5183a35`; mapper header/CPP `ec689dd8845c8def3bbb6e3abdcabd663cb4506f83842f596013e067f17b0f92` / `128c9e07d02e589fc1f14d0e7de0a6a1656c9b120ed408135cc034f921f71cd5`; ModuleHeapRole header/CPP `a6fa20a95d835519a9447ee4ecb8792acc4df944e91f85501025d934080a7ac5` / `de8d6748df324473e3276e3dc62fcee29f19fe4a98910ebdb566e9c0ff2a3538`. No executable/deployment or measured efficiency improvement is claimed.
