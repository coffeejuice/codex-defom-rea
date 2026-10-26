# CP154: lifecycle allocator bridge for the actual StateVar body

The smallest viable bridge needs three separate identities: a process/module CRT role, a live geometry allocation occurrence, and the provenance of each body/payload allocation. A GuiModel lease alone is insufficient. A FeatureCrtRuntime lease alone is also insufficient. Once a value pointer enters the actual CowVector, automatic clear/destruction must be able to finish its matching source free, including HeapFree failure and errno continuation. An API that parks after failed HeapFree cannot serve as that live container's deallocator.

There is a source-ready **conditional temporary-only prefix** before an original same-run witness, provided it is integrated into the actual CP152 receiver/frame through a new explicit allocator-role bridge. It must retain the actual temporary and stop before value allocation. There is no source-ready full lifecycle publication through the current APIs, and no original parity claim follows from a new candidate prefix. This refines the CP153 independent suggestion: isolated map-runtime reuse or metadata-only owner registration is not the proposed next checkpoint.

## Evidence and current source boundary

Read AGENTS.md/RESEARCH_CHARTER.md and the CP151–153 geometry receipts, especially `analysis/resume153-geometry-value-owner-next.md` (SHA-256 `83c4fb4f9bbbe7b41aaaf9a4cb005dba9516dd352059a25cd5afe47958e8836b`), the CP153 independent audit, and CP119/134–138 CRT/errno qualifications. Inspected current StateVar, CowVector, GuiModel, collection teardown and dependency files. Used immutable local PE/DUMPBIN reads only; no GUI, original process, build, tests, mutable REA or source edit. Only this report was written. All relative paths resolve under `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst`.

Original DEF_GUI_MO_64 v11.0.2 was independently rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Preferred native addresses below require relocation for a live run. Existing source `@src` comments often name DEF_PRE-family aliases, not the GUI addresses below.

Current source facts:

- `source/apps/moproj2mst/include/m2m/engine.hpp:130` and `src/engine/gui_model.cpp:157` own the actual step/model lifetime but no allocator role. The model's lifetime member is declared before its step so it outlives represented members during teardown. That guard identifies a model; it is not a memory-deallocation capability.
- `source/port/defmodel/src/layout/state_var_base.cpp:137` uses host malloc1 for old count0, then actual CowVector.resize, slot publication and host free. Current CP152 Viewer stops after the existing two setters, without invoking this method again or completing the leaf.
- `source/port/defmodel/include/defmodel/sdctl/cow.hpp` routes clone payload110, insertion137 and push_back295 through host `cow_detail::alloc`; frees payload in destructor122, insertion141, push_back297 and clear329. Default body176 and detached body218 use host new; release349 deletes the last body. Copy/assignment share a body. The hook coverage must include all these routes, not just resize.
- StateVar destructor45 calls clear63, and `members/statevar_collection.cpp:37` deletes actual variables. Deallocation therefore escapes the Viewer invocation and can occur while the model is being destroyed. A borrowed per-frame allocator, thread-local Viewer pointer, or expired model guard cannot supply the required later free.

The known first nonempty native sequence stays unchanged: `0x1409e47e4` requests temporary0, malloc normalizes only HeapAlloc length to1; `0x140832a1d` requests a distinct payload8; native fill/body first-last-end stores precede slot publication `0x1409e486f`; temporary free `0x1409e4935` precedes changed=true/leaf return. CP152/153's independently verified resize/vector/insert unwind flags0 exclude local temporary cleanup on second-allocation throw. No RAII temporary cleanup may be added as native unwind.

## Additional native lifecycle edges checked here

DUMPBIN14.44.35228.0 and section-mapped PE bytes show:

| Native edge | Established order |
| --- | --- |
| Base dtor `0x1409e4090` | reset base vtable, call clear via `0x1409e4150` at40b6, then destroy values handle via `0x1401374e0` at40c5 |
| Clear `0x1409e4170` | dim0 setter4180, vector clear418e -> `0x1408ebee0`, then bound-slot null41a8 |
| Vector clear `0x1408ebee0` | detach `0x1401b2730`, reload body, body clear `0x140b604f0` |
| Body clear `0x140b604f0` | free first via `0x14146d898` at6050c; only after return null first60516, last60523, end60530 |
| Shared detach `0x1401b2760` | old-body ref helper `0x14006bdd0` at277a, operator-new **0x20** at2784, clone ctor `0x1401b2890` at27a3, publish new body27cc |
| Clone payload `0x1401b2890` | signed count branch28bb; positive count*8 allocation28dc; publish first28f0/last290b/end2926, then copy2945 |
| Handle release `0x1401374e0 ->0x140137500` | body ref helper3750e, conditional deleting dtor37540 -> `0x140137560` |
| Body deleting dtor `0x140137560` | body dtor `0x1401375a0` first, then if flag&1 body-storage free through `0x1414dfc68` at37587 |
| Body dtor `0x1401375a0` | free payload through `0x14146d898` at375bc; it does not null the body fields |

The shared-detach ref helper's decrement semantics are supported by existing candidate/evidence mapping; this pass establishes its position before new0x20, not a new full helper proof. The success-only empty-unique first leaf skips detach; it does not authorize the shared branch. Do not replace decrement-before-clone allocation with a stronger exception guarantee during bridge work.

These extra edges expose two requirements that a resize-only fix misses. Clone creates **body storage32 as well as payload**, and body deletion frees those separately. Clear nulls pointers after source free returns, even if kernel free failed and its errno tail then returned normally. Matching a deallocator means executing the right source continuation against the right owner; it does not mean every kernel attempt succeeded.

Half-open PE byte hashes (not textual ASM hashes):

| Range | SHA-256 |
| --- | --- |
| `0x1409e4090..0x1409e40cf` | `78b9312a529380a176defcb256267c3c77520089e63ac7e7ce634af52062620c` |
| `0x1409e4170..0x1409e41b4` | `84d225e8527034eafafae22fd88b657c3133348a46bde8c2acd520fff07a0544` |
| `0x1408ebee0..0x1408ebf05` | `2dcf6fd080c74054750e08d2cfe447b0aa07e04814b910cd66ffe51e59d73c85` |
| `0x140b604f0..0x140b6053d` | `16accdd595941c21eea2c69c821028a0735cbaeab61f4a33e23fc004b2fffcfe` |
| `0x1401b2760..0x1401b27d4` | `1d3c56085667f49d6d5f8de7bc9f6253223513fdf79ba16dca228173bafb723b` |
| `0x140137560..0x140137596` | `446cd47f450fc592271c5a00c0475594701b29ca4e1d2bb8b059fa9d6f798458` |
| `0x1401375a0..0x1401375c6` | `e3b6642a4024654e0b394b5199280e47a2eb8532b5cba78171e4181e86d2250a` |

Bounds were decoded from aligned entries; unrelated trailing fragments in bounded captures were excluded. Clone detail was inspected through its copy call; its exceptional cleanup/complete transitive closure is still a separate qualification requirement before admitting shared-clone failure behavior.

## Proposed bridge: three lifetimes, no object-layout change

Place the narrow platform-neutral role interface and body-provenance dispatch below the app layer, for example under `defrt`/`defmodel`. `defmodel` already depends on defrt; it must not depend on moproj2mst's Feature types. Put the Win32 process/module owner implementation in the converter runtime layer and explicitly connect it to the actual model. Names below describe design roles, not recovered native classes.

1. **Module CRT owner.** One canonical owner for the converter module's reconstruction of original EXE CRT slot `0x144700810`, shared across models/conversions, with real HeapCreate(0,4096,0)/slot publication/ignored compatibility Boolean and a module/process generation. It owns live heap/new-mode/encoded-handler/errno-role state and supplies typed allocation/free occurrences. Creation is module startup work, never per project/reader/StateVar. Do not share FeatureCrtRuntime's private handle, move its retained maps, or treat its encoded-null records as current conversion startup. Reusing already qualified implementation algorithms is appropriate; adopting that experiment's resource is not.
2. **Model/occurrence capability.** The actual GuiModel's lifetime, step, current object/table/owner, installed high-reader frame, actual receiver/id/type, values handle/body generation and u30 slot authorize the next allocation occurrence. Bind through trusted construction/access code, not caller-supplied heap numbers or success flags. Store requested bytes, normalized kernel bytes, original callsite, thread and monotonic occurrence identity. Recheck current ownership at callback-sensitive boundaries. An allocation capability expires with its current occurrence; it cannot replay setters, borrow another frame or resume a repaired parked call.
3. **Body/block provenance.** Out-of-line records retain the allocator owner for the actual body and every live or unresolved block. Keep StateVar0x38, handle8 and body0x20 layouts unchanged. A body record survives handle copies and contains a generation, runtime lease, body-storage provenance and payload provenance. The record follows the body, not a particular handle or GuiModel. A block retains its allocation owner independently of model liveness so destruction can free it when its model is expiring or a shared handle survives elsewhere. Source allocation authority and later release authority are different capabilities.

The first adoption can attach only to an actual unique empty existing body after the exact CP152 recheck. Its **already-existing host-allocated body storage remains host-owned**; only future temp/payload blocks use the source-role backend. Record that mixed backing explicitly, without claiming the native earlier body allocation was reproduced. Future bound clone bodies need source-role request32 plus placement construction and matching explicit body destruction/free; no plain delete on a HeapAlloc body. Alternatively, keep all clone bodies host-backed as an explicitly translated intermediate, but that cannot claim native clone body-allocation fidelity. The preferred eventual lifecycle bridge represents request32 as well.

Avoid a process-wide global operator-new override or a transient TLS allocator scope: many other CowVectors and unrelated host bookkeeping coexist, and a shared body can be mutated/destroyed after the scope ends. Preserve current host alloc/free for unbound bodies. A role-owned block must never fall back to host free because its registry entry or model lease is missing. Dispatch relies on retained provenance established before publication; missing provenance is an internal invariant failure, not evidence of an original failure branch.

Prepare registry/call storage before source effects. An append/growth failure must stop before the native allocation/refcount mutation it would describe, and be labeled a reconstruction bookkeeping guard. Registry registration/retirement and receipts must not allocate during destructors or after freeing a block. Store block tokens after successful free as opaque history; never dereference or query them. Do not hold a registry mutex through a new-handler callback: native callbacks may change runtime state, so retries must reread current roles at their actual occurrences.

## Container routing and failure requirements

| Operation | Required routing |
| --- | --- |
| StateVar temporary | same canonical role as its actual bound values body; request0 retained separately from kernel length1; raw T retained in the occurrence, no RAII source-free on a later throw |
| insert/grow/push_back | select backend from body; allocate/fill/copy through existing real algorithm; free old payload before original pointer stores; retire only the actual old-block receipt |
| copy/assignment | share the actual body and its existing provenance; assignment releases the displaced body's owner, never rebases its data onto the destination model's allocator |
| detach/clone | clone provenance from source body, prepare a new record, preserve native refcount/allocation/publication order; metadata body32 and payload are separately owned |
| clear | real detach if shared, then matching payload source-free; null pointers only when that routine returns; binding of the empty body remains for future regrowth |
| last-body destructor | payload free exactly once, then body-storage release according to its own provenance; no required live Viewer/frame; any retained failed-free block becomes opaque unresolved history |

The bridge needs two execution surfaces, not one ambiguous Boolean allocator callback. An explicit geometry continuation may return **Pending** before unknown new-mode/handler/errno effects, retaining its actual partial state. Ordinary C++ container allocation/destruction must run a qualified **complete synchronous routine** or remain outside admission. `CowVectorBody::~CowVectorBody`, `StateVarBase::~StateVarBase` and deleting destructors have no pending-return protocol. Returning from them after a partial free, throwing an arbitrary host exception from noexcept destruction, leaking a role-owned pointer by switching to host cleanup, or silently treating a raw BOOL0 as success all change behavior.

This is the main blocker for value publication today. The existing Feature free methods intentionally stop on HeapFree0 before errno and retain their map backing. That policy works for an explicit experiment; it cannot be installed as the actual payload destructor. Even a correct allocation/normal-free success test cannot prove that failure is unreachable.

A full free backend must execute `0x141471b9c ->0x141474134 ->0x1414814e8`, preserving same-thread LastError through its provider, then GetLastError at`0x141471ba4`, mapper`0x1414740ec`, errno store`0x141471bb1`, and return. After that return native clear still nulls pointers on raw free0. Retain an unreleased-block record; do not automatically retry or report it freed. The runtime's failed-free history may therefore outlive body deletion. Successful source HeapFree retires ownership before caller stores and prohibits later pointer reads.

## What is genuinely missing from original evidence

**Already established statically:** temp request0/HeapAlloc1 and separate value8, successful order, native private heap producer, occurrence-sensitive new-mode/handler loops, same source heap free, local absent temporary unwind, plus the lifecycle edges above. These do not need another discovery pass before writing the bridge interface.

**Missing original same-run success witness:** one original high-reader occurrence binding G/step/current O/actual id1 receiver/body/slot and dimensions, then actual heap-slot reads and nonnull T/B returns at`0x1409e47e4`/`0x140832a1d`, ordered body/slot publication, actual temporary free arguments/result and leaf return. Retain actual process/module/base/thread/frame/occurrence generation. CP153's recipe is the exact request; a remote pointer is evidence, never candidate memory authority.

**Missing runtime failure/lifetime owner closure:** native module FLS index`0x143b5e990`, startup locks, callback`0x141481590`, PTD allocation/initialization/lifetime, errno fallback`0x143b5e2c8`, current new-mode/encoded-handler state and native exception construction/throw. CP135/136 qualify FLS startup at`0x141481723`/index publication1729/PTD0x2c8 allocation173e/FlsSetValue1754. The current runtime owns none of those. `feature_win32_error_map` is an explicit retained-initial-table model only; table`0x143b5e160` is writable, not a current original table observation. It cannot complete a native failed free by itself. Old parked failures did not preserve same-thread error continuity and cannot be promoted later using a fresh host LastError read.

**Missing wider lifecycle validation:** same original body through shared detach/clone, nonempty regrowth/clear/destruction and representative failures, including clone body32/payload unwind. The native calls discovered above support the bridge routing but do not establish complete exception semantics for those branches. Whole-module heap teardown remains unclosed; do not invent HeapDestroy at model/Project release.

Candidate changes needed to represent those roles are not substitutes for these observations. Conversely, an original trace does not install a safe candidate allocator. Treat source readiness, actual candidate kernel effects, and original parity as three separate acceptance statements.

## Source-ready prefix and integration sequence

A conditional actual prefix can precede the missing same-run witness because immutable bytes establish its successful operations. It requires **new coordinated bridge work**, not calling current unrestricted resize or adding a Viewer flag. Establish the canonical actual conversion runtime owner and bind the actual live CP152 body/occurrence. In the same current reader invocation, after the existing setters, run actual data/count0 and request0/HeapAlloc1. Nonnull T is retained in that occurrence; copy0 has no byte effect. Stop immediately before value request8 with body/slot empty, whole-leaf count0, public barrier installed and no setter/lookup/allocation replay. Actual null parks before new-mode and must not be called operator-new returnednull or host bad_alloc.

The pending call must retain its runtime/T through return to the host and model cleanup; that is reconstruction retention of a suspended native frame, not native unwind/Close. Do not automatically free T in a metadata destructor or destroy its heap. This prefix establishes actual bounded effects, not a complete lifecycle allocator. It is acceptable only if it is the first integrated occurrence of the planned bridge and its next dependency is explicit. If the next checkpoint must complete a value leaf, keep CP152 unchanged until the synchronous destructor/free backend is ready; an isolated owner or empty metadata stage does not meet that goal.

For full publication, first implement/qualify complete free-error ownership and route all listed body/payload paths. Then execute request8, fill and actual typed body/slot stores through the real container implementation, temp source-free, and leaf completion. Do not repeat virtual setters to enter that continuation. Keep all-zero recipients/public scene guards unchanged until the surrounding loop/copy/Stop is separately admitted.

Suggested source ownership is one coordinated bridge lane: new lower-layer role/provenance interface and implementation, CowVector header, StateVar resize implementation/header only if a private typed continuation requires it, actual GuiModel/access ownership and Viewer integration, CMake registration, and focused tests. Preserve the m2m C ABI. Avoid parallel owners editing this shared interface; root integrates focused targets then full stable validation/freeze.

Tests must distinguish actual Win32 effects from declared failure/order models:

- Actual two live GuiModels share one module role, with distinct body/occurrence generations; foreign shared_ptr/control block, replaced current table/receiver/body/slot and expired allocation occurrence refuse before allocation. Deallocation still works during owning model destruction.
- Actual temp0 yields kernel length1 and distinct data8; retained byte contents remain unknown until the actual fill; ordered body/slot stores and temporary free follow the native sites. No replay or repeated setters after parking.
- Clear/regrow, push_back growth, copy assignment releasing an old owner, shared detach, last-reference destruction and address reuse preserve body/payload provenance and exactly-once deallocation. Mixed existing host body/source payload and new source body32 are verified separately.
- Declared first/second allocation failures retain the exact partial state and temp leak/unwind distinction. Declared free0 checks provider-before-GetLastError, errno mapping/store before caller pointer stores, no false freed receipt and no retry. Failure in shared clone preserves qualified native refcount/order, not an invented strong guarantee.
- Layout/static ABI checks and actual existing StateVar/CowVector/all-zero geometry regression targets remain required. No process OOM, invalid handles or double-free injection. A positive kernel result is asserted as positive, never substituted with declared success.

At audit, CowVector SHA-256 remains `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983`, StateVar CPP `6bd7f9cf4ff638994311f888f573a079ec0558a7ce725df81329aa0719ff535d`, engine.hpp `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f`, gui_model.cpp `26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45`. No executable/deployment or measured speed/token improvement is claimed.
