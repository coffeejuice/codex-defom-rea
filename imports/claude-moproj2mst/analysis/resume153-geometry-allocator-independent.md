# CP153 independent audit: nonempty StateVar allocator ownership

The current candidate cannot complete the first nonempty normal StateVar leaf with a qualified native allocator owner. Its temporary and vector payload use host CRT allocation; the existing private-heap experiment owns only its isolated Feature map calls. There is no current GuiModel/StateVar-to-runtime lease or compatible private-heap vector deallocation path. Reaching CP152's ordered dimension setters supplies neither fact.

The native success order and local absence of temporary cleanup are independently confirmed. A further immutable disassembly pass is unnecessary for those facts. The smallest useful implementation cut is an explicitly owned, same-live-receiver **temporary-allocation prefix**, after establishing a real candidate process/module CRT-role owner. It must stop before vector allocation and must not install a HeapAlloc pointer in the existing CowVector. Full one-value publication additionally requires a compatible payload ownership interface across clear, growth, clone and destruction, and represented failure/free continuations. No original same-run allocation/heap witness was found in the examined geometry receipts.

## Scope and qualification

Read AGENTS.md, TAKEOVER.md, RESEARCH_CHARTER.md, takeover/import state, CP151/152 geometry design/implementation/source-review receipts, CP119 allocator inputs and retained ASM. Applied the reverse-engineer-anything skill's distinction between immutable native evidence and candidate/source hypotheses. No REA tool, mutable database, original process, GUI, build or test was used. Only this report was written. New DUMPBIN output was inspected in tool results, not saved as a second artifact.

This is an independent static/source audit, not an executed candidate continuation or original parity result. All paths below are relative to `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst`. Native addresses are preferred image addresses in the retained original DEF_GUI_MO_64 v11.0.2; a live observation must relocate them using that run's module base. Source `@src 140301380` and CowVector comments include DEF_PRE-family addresses; the corresponding GUI resize analyzed here is `0x1409e46c0`. Do not identify the source-comment addresses as GUI callsites.

## Independently checked immutable PE facts

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The native receiver remains normal StateVarNode vtable `0x1427eac80`, slot+0x10 -> `0x1409e49d0`, as qualified by `analysis/wave3-editor-mds-node-variable-vtables.json` and its key/zero static receipt. CP152's admitted ordinal0/id1 profile has dimensions1x1x1 after setters, old values count0, unique body with first/last/end null and actual bound slot null. Nothing below broadens that profile.

An in-memory PE section/.pdata parser independently re-read these half-open byte ranges and hashes:

| Range | Meaning | SHA-256 |
| --- | --- | --- |
| `[0x1409e46c0,0x1409e494c)` | StateVar resize | `29f8e0e8332005c0155c0cd835ac67c4f2e85ef4bd9322e3f61cfd048cc0dbaa` |
| `[0x140832790,0x140832b20)` | Vector insertion body | `74b4506c8d610202e662a120cf63f2cca43f507cf83fc7eb74de18c602486c81` |
| `[0x14146eabc,0x14146eb55)` | Throwing new outer wrapper | `b3387d73bf1a5d85ccc99fb9b0fc0d51ce05d655a112500f893bf200afe4a24d` |
| `[0x14147489c,0x141474952)` | Malloc/HeapAlloc wrapper | `a361b9fdb9d9aad45453d7cdcaeb4362e483cfeb1799c4704fc5ba743f0796d4` |
| `[0x141471b7c,0x141471bb9)` | Free/HeapFree wrapper | `f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696` |

These hashes cover PE bytes, not the textual ASM hashes in CP119. Inspection used existing DUMPBIN `14.44.35228.0`, path `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, with `/disasm:bytes /range:...`. Bounded ranges included `0x1409e47b4..0x1409e4875`, `0x140832a07..0x140832b1f`, `0x1408329b9..0x140832a26`, and `0x1409e4926..0x1409e494b`. The first insertion capture began inside an instruction and the first resize capture ended inside one: the fragments before `0x140832a0f` and after `0x1409e486f` were excluded; the aligned `0x1408329b9` recapture supplies the growth/allocation prefix.

At `0x1409e47b4` actual value_data is acquired, then `0x1409e47c6` actual count. `0x1409e47cb..0x1409e47e4` forms oldcount*8 with overflow clamp and calls `0x14146c25c -> 0x14146eabc -> 0x14147489c`. Oldcount0 is a real **operator-new request0**. Malloc `0x1414748b4..0x1414748bc` normalizes only HeapAlloc length to1; `0x1414748c0` reads heap slot `0x144700810`, and `0x1414748f1` calls imported HeapAlloc with flags0. Returned temporary is stored at resize `rsp+0x68` then `rsp+0x48`; its byte is uninitialized. The zero-count copy at `0x1409e4816` has no byte effect under CP152's retained memcpy zero guards.

`0x1409e483b` calls vector resize1/fill0.0. The unique empty body takes `0x1408329b9`, computes0+max(0,1)=1 and at `0x140832a1d` invokes the same operator-new with **request8**. This second block is distinct from the temporary. `0x140832a94..0x140832a98` writes one0.0. The old payload free at `0x140832ad7` receives null and returns before any heap read. Body first/last/end stores occur at `0x140832ae9`, `0x140832afa`, `0x140832b14`; the bound owner's slot is published later at `0x1409e486f`. With remap=false, `0x1409e4935` frees the temporary, then `0x1409e493a` sets changed=true. No whole-leaf receipt can be issued at allocation or slot publication alone.

The independently parsed .pdata entries and complete aligned unwind records are:

| Function range | Unwind VA | Raw record / flags |
| --- | --- | --- |
| `[0x1409e46c0,0x1409e494c)` | `0x1432c7fe4` | `0111020011f20a30` /0 |
| `[0x1408325c0,0x14083264e)` | `0x143401288` | `0114020014321030` /0 |
| `[0x140832700,0x140832788)` | `0x14334a0e8` | `0118010018620000` /0 |
| `[0x140832790,0x140832b20)` | `0x14315e880` | `011b02001b011100` /0 |

There is no EH/UH/chain handler in those scopes. In this exact empty-capacity profile, throwing second allocation does not execute a local temporary-free handler. This does not exclude an enclosing catch or eventual module termination. Adding scoped RAII that frees the temporary on vector throw changes this local native behavior. A parked/unavailable continuation is also not a thrown native exception and must not manufacture unwind.

## Why the existing candidate allocator cannot be relabeled

`source/port/defmodel/src/layout/state_var_base.cpp:137` performs the broad order but calls `std::malloc(bytes ? bytes : 1)` at155, throws host `std::bad_alloc` at156, calls existing `values.resize` at159, publishes through u30, and `std::free(tmp)` at174. Its raw temporary is not RAII-owned, so the local second-allocation-throw leak shape is preserved, but that coincidence does not supply native handlers, errno, heap identity or exception type.

`source/port/defmodel/include/defmodel/sdctl/cow.hpp:38` allocates payload with host `std::malloc`; at42..43 it converts null directly to host `std::bad_alloc`. The body's insert path allocates at137, frees old first at141, then publishes pointers142..144. Its destructor at122, push_back297 and clear329 also use `std::free`. Handle construction176 and detach218 use host `new Body`; body sharing/release349..352 can lead to a later delete. `StateVarBase::clear` at67 reaches that actual `values.clear`, and the StateVar destructor44 calls clear. Thus a private-heap payload cannot be attached by direct body-pointer stores or an ad hoc Viewer allocation: even a successful first scalar leaf leaves later clear/destruction using the wrong allocator. A success-only temporary can be separately retained without attaching it to CowVector; a successful value payload cannot.

`source/apps/moproj2mst/src/engine/gui_model.cpp:157` constructs DB_STEP_DATA with host make_unique; there is no FeatureCrtRuntime hookup in this constructor, viewer.cpp, gui_model_access.hpp or the inspected StateVar/CowVector paths. CP152 helper `viewer.cpp:899` still ends at `ScalarDimensionsCommitted` after936, whole-leaf count0. The actual frozen Viewer hash is `8777ad9d05f6f357f34f82e18b1fa366d7cecf057f603a89b6858052d5c09669`.

`source/apps/moproj2mst/src/model/feature_crt_runtime.hpp:429` describes an **explicit owned-test-process** owner and no default conversion hook. CPP84 stores its candidateHeapSlot;391 retains one private factory owner, pins its containing candidate module and records process identity. `attemptProducer`908 executes HeapCreate(0,4096,0), stores the candidate role, then invokes compatibility class0/DWORD2/length4 with ignored Boolean result. Existing `FeatureMapProxyCall::resume`440 uses this owner to HeapAlloc8 for a specific map proxy and publishes a map backlink; sentinel resume allocates48. Its private heap is not a generic allocation interface and its map/proxy receipt cannot stand for a geometry temporary or values body. Matching the shared_ptr control block at404 proves that test owner, not membership of an actual GuiModel/StateVar. Its destructor389 explicitly performs host metadata cleanup only, never HeapDestroy.

Original private heap creation/liveness belongs to the module/process CRT role: producer `0x141484800`, HeapCreate at `0x141484812`, slot publication `0x141484818`; startup caller `0x141470ca3`. Candidate role mapping can execute an equivalent owner producer in its own module. It must not reuse an original process handle or read preferred original addresses in the candidate. A fresh original runtime witness is needed for original-path validation, not as a transferable allocation capability for the candidate.

## Exact missing runtime witness and failure state

No examined CP151/152 geometry artifact captures the following **same original run and same allocation occurrences**. A small future successful witness should bind:

1. Original image hash, process/module/base/generation and live heap slot; same caller frame/current GuiModel/step/object/ordinal0 receiver, vtable/id, receiver+0x28 handle/body/refcount1, first/last/end null, actual u30 and bound slot null; current/previous dimensions and D1 facts at entry.
2. Temporary callsite `0x1409e47e4`: request0, HeapAlloc occurrence `0x1414748f1` with the then-current slot handle, flags0/length1, nonnull return T; count0 memcpy and retention of T in the same resize frame.
3. Value callsite `0x140832a1d`: request8, separate current heap-slot read and actual HeapAlloc return V; fill V=0.0, null-old-free path, three ordered body publications, then actual bound-slot publication of V. Do not infer these identities from a separate map allocation or an earlier run.
4. `0x1409e4935`: actual free argument T; `0x141471b89` current source heap-slot read, HeapFree(heap,0,T) at `0x141471b92`, return/result branch and return to changed=true/leaf/caller. HeapFree failure requires its actual error continuation before claiming free-helper return; ignored return never proves T was freed.

Successful allocations do not read new-mode/handler/errno and therefore do not require speculative handler values. On actual null, the next required source input is malloc new-mode `0x144700858` at `0x1414748ff`; handler occurrences each read encoded slot `0x1447006e0` through `0x141481ce8` and DecodePointer. Original request0 remains handler argument0 despite HeapAlloc length1. New-mode0 calls errno provider `0x141474134` twice (stores at `0x14147491a` and `0x141474925`); new-mode nonzero can invoke a handler inside malloc, followed by a distinct outer-new handler call after malloc returns null. Retry rereads the heap slot. Native bad-allocation globals `0x1446ffb20`/`0x1446ffb38` and exception machinery remain separate requirements. Existing initialized candidate encoded-null role receipts do not prove all those inputs or current occurrence bindings.

For free failure, source first obtains errno pointer `0x141471b9c`, then GetLastError `0x141471ba4`, maps through `0x1414740ec` at `0x141471bac`, stores errno `0x141471bb1`, and only then returns. Capture/order tests must preserve that sequence rather than assume host errno/GetLastError effects are equivalent. The flags0 unwind proof already closes the **local absence** of temp cleanup; an original failure observation is needed to validate any claimed enclosing exception/return behavior, not to rediscover that local fact.

## Smallest source-ready next cut

Do not merely call unrestricted StateVar resize from CP152 or reinterpret a Feature map report. Assign a typed process/module CRT-role allocation call and a same-live-geometry owner lease as explicit source work. Keep the current production guard until that binding exists. The smallest experimental continuation is request0/HeapAlloc1 only, under the existing owned-test-process opt-in and same current frame/receiver/body/slot checks. It can reuse the qualified owner-producer algorithm; it cannot reuse map-specific storage/backlink effects. Require a separately retained owner capability and immutable requested-vs-normalized size facts, not a public Boolean or externally supplied heap number.

Prepare host bookkeeping before source allocation. After CP152 setters in the still-current invocation, revalidate the real registry/receiver/body and owner lease, perform actual value_data/count0 and one request0 allocation, record actual nonnull T, observe zero-copy, then stop immediately before `0x1409e483b`/value allocation. Keep body/slot empty and whole-leaf count0. Retain T and its live owner in a dedicated pending call, with no reallocation/replay on another refresh, no automatic source-free on metadata destruction, and no claim of native unwind. On actual HeapAlloc null, park before new-mode; do not report operator-new returnednull or fabricate host bad_alloc. This extends an explicit test experiment; it does not by itself validate or release normal conversion.

Before integrating request8/publication, design one compatible allocator/deallocator binding for this actual values body and every operation that can consume/share/release its payload. Preserve original object/body layouts and existing compatibility ABI; adding fields to StateVarBase/CowVectorBody would violate their established layouts. An out-of-line owner/call registry is a possible implementation choice, not a recovered native class. Establish its lifetime and sharing semantics from the current body, and make its extra host bookkeeping explicit. Binding must precede the first payload allocation and remain valid through actual clear/reset/destruction; a later ad hoc free interception is insufficient. Full first-leaf success also requires actual temporary free return or an explicit free-error pending stage. Default/unbound profiles remain parked.

Discriminating declared-order tests should separately cover temp allocationnull before new-mode, temp success/data allocation pending, second allocation failure retaining T with no local unwind, value/body/slot publication before normal temp free, free failure parked before errno, and stale/foreign runtime/model/frame/body/slot leases refusing before another native allocation. Wrong shared_ptr control block, real body sharing, changed actual table membership and repaired parked input must not permit replay. Use declared backend outcomes; do not force OOM or invalid heap handles. Actual candidate HeapAlloc/HeapFree success tests establish candidate kernel effects only. Fresh original parity stays pending the same-run witness above.

## Source/evidence identities at audit

| File | SHA-256 |
| --- | --- |
| `source/port/defmodel/src/layout/state_var_base.cpp` | `6bd7f9cf4ff638994311f888f573a079ec0558a7ce725df81329aa0719ff535d` |
| `source/port/defmodel/include/defmodel/sdctl/cow.hpp` | `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983` |
| `source/apps/moproj2mst/src/model/feature_crt_runtime.hpp` | `a9c07b8e044dcb15e27eb13e75e5443e67137c31477942e384cf02947c10c7a3` |
| `source/apps/moproj2mst/src/model/feature_crt_runtime.cpp` | `48434b8eeacccb7e36f00ecbd423536b333696d668a18568756f7946f5183a35` |
| `analysis/resume152-geometry-nonempty-next.md` | `a10a372122662e29bcff9c62596c794315ea5055b359eee0696b27144dcf61b1` |
| `analysis/resume119-feature-allocator-inputs.json` | `ff1ea36ace289559ebca9ef69c0f0811f7a0626f8ed6efd8c6216b3aa008cf51` |

No executable was produced or deployed. No measured speed/token improvement, full leaf completion, original failure parity or headless Open-to-Save completion follows from this audit.
