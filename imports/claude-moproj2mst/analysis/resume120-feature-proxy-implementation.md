# CP120 addendum: preserve the existing epoch owner before adding proxy state

The proxy success path remains queued under root's revised instruction. No lifecycle source/API was created and no shared header was edited. The initial cpp-only assignment cannot store one resumable Project-owned allocation through the current read-only anchor interface. There is, however, a narrower existing ownership location than adding another Project field: **the control block of the existing private `Project::slotBorrow_` storage epoch**. This can host private implementation storage without changing Project/marker public field layout, but its construction bridge, stage contract and resource retirement policy require an explicit coordinated design before actual heap effects.

## Existing storage and exact candidate lifetime

`ProjectStorageLifetime` is currently an empty final marker. Project's private `slotBorrow_` is its strong owner; `MoModelSlots::projectStorage_` and `FeatureContextAnchor::storage_` borrow it weakly. Slots bind the exact Project identity at Project shell construction, and the actual prepared Mo later binds its identity. The anchor is a separate diagnostic shared object which borrows epoch/slots/model; escaped anchors cannot retain Project storage.

Current declaration/construction/destruction order is relevant:

1. Project initializes registry/slot anchors, then the strong storage epoch, then its remaining C++ members. Constructor body binds slots to this exact Project/epoch and creates the passive feature anchor.
2. Logical089917 is recorded later from actual initProject only with the prepared same Mo/slot identity, before later adapter fields/backlink commit. It remains pending before any native allocation.
3. Project destructor body first makes the escaped feature anchor's source scope inactive. This occurs before ordinary member destruction.
4. Ordinary later-declared Project members and feature-anchor member ownership are then released. The storage epoch expires at its existing later member boundary, before the earlier slot/registry anchors finish.
5. Mo-owned slots can remain alive through the actual registered ModelManager child after Project storage has expired. Their weak epoch must not prolong or resurrect the Project context. Session release preserves Display, ends thread-local Project scope, destroys Project, then clears the Mo backlink; this remains adapter storage cleanup, not accepted native Close.

CP118's actual release and identical-address reconstruction tests already discriminate this epoch from child/slot/anchor lifetime. Reusing the same raw Project address produces a distinct weak-control-block epoch; an old diagnostic anchor remains inadmissible. A proxy state placed on ModelManager, persistent Mo slots, registry or Session would have a different owner boundary.

## Narrow private ownership mechanism, not an API commitment

Keep `shared_ptr<const ProjectStorageLifetime>` and the empty final marker type unchanged. A future private storage factory can create a private bundle containing that marker and opaque lifecycle implementation storage, returning a shared pointer **to the marker** whose control block owns the bundle. An ordinary private custom deleter can carry the bundle pointer; the same private implementation can recover it with typed `get_deleter` and validate that the actual marker address matches. No inheritance from the final marker, pointer arithmetic/container-of, guessed offsets, public opaque field or process-wide map keyed by raw Project addresses is required.

This is ownership/type erasure through the existing private strong owner, not reuse of unrelated application state. Existing Mo-slot/anchor weak references would still see the same epoch control block and expiry boundary. The private bundle's default implementation must remain passive metadata with no HeapCreate/HeapAlloc/provider effects at shell birth. It must not retain its own epoch strongly and form a cycle; diagnostic/source frames borrow the epoch, and source admission still checks the anchor's earlier destructor-entry invalidation.

The factory/control-block failure path also needs ordinary C++ exception safety: a trivial deleter owns the bundle exactly once if shared-control-block construction fails. Do not simultaneously retain a unique owner and permit the failing shared-pointer constructor to call the same deleter. Typed deleter lookup must fail closed for an old/non-bundle epoch or unavailable type identity, never reinterpret-cast an arbitrary marker. Host C++ RTTI/control-block support is a build gate for this recovery mechanism, not a native provider RTTI witness.

This identifies a **private storage location**, not approval to add the API or native effects now. Root would have to authorize the existing project_owned_storage.cpp epoch-factory/phase bridge and a shared private implementation definition. Existing Project public declarations and mo_model_slots.hpp need no additional payload field merely for the bundle. The supplied project_lifecycle.cpp alone has no access to the private epoch and does not currently exist. Do not replace that missing bridge with a global allocation registry or repurpose overlay, p06Cog, operation registry or Mo-slot payloads.

There is also an existing build constraint: project_owned_storage.cpp is compiled directly into many standalone packet tests without m2m_core. An unconditional new external lifecycle-factory call would require root to supply its object to those targets, or keep the passive factory definition/dependencies within their existing compilation closure. This cannot be hidden behind a runtime branch; the linker still needs the referenced symbol. No CMake change was made here.

## Storage placement does not close teardown

The bundle must not turn a shared-pointer/custom-deleter destructor into a native feature destructor. Native source order from CP114/116/119 remains:

| Boundary | Required effect and unresolved dependency |
| --- | --- |
|First proxy request|Actual CRT private-heap owner; HeapAlloc8 flags0; only actual nonnull return permits `[proxy]=mapIdentity` then map proxy publication. Unknown/failure handler/errno state remains guarded.|
|Sentinel request|Separate HeapAlloc48 plus nil/self-link publication; a guard before it is not a native throw.|
|Native second-allocation exception|The map constructor EH action frees the already-published proxy through the same native heap/free path; it does not null the map slot or initialize later fields. A paused candidate call cannot pretend this unwind occurred.|
|Accepted Project Close|Per-feature release can insert a null node; nonnull node erase precedes provider deleting virtual; later success alone clears the flag. No generic map ownership replacement.|
|Later context destruction|Optional context+1450 uses global0x1446efe90 before node clear, sentinel free, head/size zero and proxy free. Provider payloads/borrowed keys are not deleted by container clear.|
|CRT heap lifetime|Process/module private HeapCreate owner must outlive all source allocation/free operations. Project/epoch release is not proof of the original CRT heap-destruction boundary.|

A pending successful allocation record needs a defined actual resource-retention policy if adapter Project storage ends before native constructor/unwind is reproduced. Simply dropping a raw pointer loses the allocation owner; auto-delete/HeapDestroy in the epoch deleter invents source teardown. A separately qualified process/runtime allocation-retention owner can retain actual blocks and expired Project provenance while native continuation remains permanently refused, but that resource policy must be selected explicitly and kept distinct from host adapter cleanup. It must not become another guessed native owner or enable unbounded public allocations without a closed retirement contract. Current default guard performs no allocation and avoids this ambiguity.

The existing public anchor report also names `ProxyAllocationPending` and the first unknown heap site, with static false heap/completion traits. A new private continuation cannot silently advance actual proxy state while leaving that report apparently authoritative for the same current stage. Root must settle whether it remains a historical passive-entry receipt or delegates to a new private authoritative cursor, and keep defaults/public Close guarded. This is a report-semantics issue independent of where bytes are stored; it is not justification to casually widen Project's public interface.

## Next concrete step and test gates

The smallest next authorized increment would be **ownership-only private epoch bundling**, with no kernel/provider work, after root agrees its private bridge/build closure. Verify with the existing actual Project/Session path that weak epochs expire at the same boundary, escaped anchors do not retain the bundle, two Projects/ABA reuse remain distinct, normal Open behavior and Mo-slot/registry/Display lifetime remain unchanged, and the private deleter runs once as host storage cleanup without emitting a native free/Close claim. Legacy/non-bundle tokens must stay pending safely.

Only after that and the resource-retirement/report contract should the source-closed success-only proxy continuation be assigned: actual shared process/module heap binding, actual HeapAlloc8, exact backpointer/publication, immutable receipt and sentinel guard. Add actual success/no-replay/same-owner tests plus declared-failure ordering tests; unknown first heap/new-mode/handler/errno/teardown facts must never yield success. Do not infer any PST byte, registration, constructor completion or public Close admission.

## Source/evidence binding and release

Read source hashes remain unchanged from CP118: model.hpp `5653c60d568ee530be41aa35813d3f87cca418f5b9c94e9f7f34cf7ddefc99c6`; project_owned_storage.cpp `d8a98bcba68e52c8c1197a74df4a56a43874a91594be25f7aedb3dd7f24f69ac`; project.cpp `00778dfa9e2d05acf091aad5d459d46b7f54f11720a8cd41e6057910497e5868`; mo_model_slots.hpp `0486d2a65638576a3544d3cf7732a64e84d5f808bdd88202daf393deb463264c`; conversion_session.cpp `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`; focused CP118 test `cb84c0926c0a6ed7b3290bf5fd906793201324c37a96bd10ce37afdcfbe947ee`.

Native evidence is unchanged: CP119 allocator report SHA `dedc4f677a1f1bbcadde4b4dc55fc34663fcdc70ad8678b72e1407bb57e3dedb`, CP116 lifetime report `8a24844a96af8d6897c96c4720635a3a646081548b376e7890520d8486013871`; both bind retained DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No executable instance/deployment was inspected. This addendum is the sole write; no cpp/header/test/API, build, GUI, original execution or mutable REA work occurred. The proxy implementation is queued, not reported complete.
