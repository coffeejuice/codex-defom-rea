# Resume47 staged Mo child teardown design

Move ownership of the five Mo manager slots out of Project while preserving existing `p.operations` and item-manager access as borrowed aliases. A root-owned staged child teardown can then destroy Project/subprojects first, clear model+19c0, destroy the five slots in native order, and finally complete base/model-manager cleanup. Reordering Project members alone cannot establish these separate object lifetimes.

This is an implementation design, not implemented or validated behavior. It uses current source and immutable retained original evidence only. No source edits, builds/tests, app control or REA mutation occurred. Original reference: DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. See the prior ordered evidence in `resume46-close-manager-tail.md`; its native-versus-adapter qualifications remain controlling.

## Native owner-to-current-member map

| Native owner/boundary | Existing representation | Required ownership correction |
| --- | --- | --- |
| Mo model+19c0 Project; `044bf0` cleanup/delete, then null store | `ConversionSession::project_` unique_ptr, `DbHandle::moProject` raw backlink | Keep one Project identity. Session can remain an explicit adapter allocation holder, but its deletion must be driven by this actual model's destruction stage. Never clear backlink before Project destructor effects. |
| Project+58 subprojects; diagram null-rebind precedes `0a80e0` clear | `Project::subs` unique_ptr vector; `boundDiagramModel_` separate session pointer | Leave Project-owned. Implement real null-rebind before destroying these objects. Do not move subprojects into Mo slot storage. |
| Mo+19c8 SfMoOprManager; `045056` delete then `045071` zero | `Project::operations` vector<unique_ptr<Operation>> | Move owning container to same Mo model's slot capsule. Project borrows the same container while alive. Operation.Project pointers remain nonowning. |
| Mo+19d0 object manager; delete/zero `0450c6/0450e1` | `Project::objects` ItemManager | Same capsule; second explicit destruction stage. |
| Mo+19d8 material manager; delete/zero `045136/045157` | `Project::materials` ItemManager | Third stage. |
| Mo+19e0 equipment manager; delete/zero `0451ac/0451cd` | `Project::equipment` ItemManager | Fourth stage. |
| Mo+19e8 lubricant manager; delete/zero `045222/045243` | `Project::lubricants` ItemManager | Fifth stage. |
| Model base-owned managers and reader; `044071 -> cda520` after Mo cleanup | `DbHandle::postGraphManager`, reader, slice-plane store and partial model fields | Preserve existing base identity, no duplicate DbHandle. Broader base payload is still guarded. Existing postGraph reset is only a supported prefix. |
| Main-window+2b8 operation-class registry | `Project::registry` map owning `OpClass`; Operation::opClass is raw | Keep registry alive through later operation destruction, independently of Project. Give session a separate registry owner/lease and Project a borrowed view. This is a native session resource, not a sixth Mo slot. |
| Main-window scene/Display | Project::display shared_ptr plus session scene owners/displayLease | Preserve existing session ownership. Do not move Display into Mo capsule. |
| Process remesh/task allocation/notifier state | Current ConversionSession-owned holders | Keep alive across Project and model clear; do not reset as part of slot teardown. |
| Original process globals currently grouped on Project: p06Cog, MultiBlow root, DOE simulation position | Project adapter state | Do not silently claim these are native Project fields or erase process-state qualification. Audit which later destructors read them before changing their lifetime; process-reset parity remains open. |

Today implicit Project destruction releases the five slot representations in reverse declaration order (lubricants/equipment/materials/objects/operations), before its earlier-declared subprojects. The mismatch follows C++ member destruction rules and the retained `0440c0/044bf0/045000` bodies; no runtime crash is claimed.

## Minimal owner capsule

Introduce an internal `MoModelSlots` type holding ownership of the existing five manager representations. Prefer individually owned slot allocations, so each actual manager representation can be destroyed and its slot made null at its distinct native boundary. The operations slot may initially wrap the existing vector, while the four item slots use existing ItemManager objects. Avoid duplicating either rows or model identity.

Project gets an anchor to that storage before its borrowed aliases are initialized. Its existing spellings can remain `operations`, `objects`, `materials`, `equipment`, `lubricants` as references, reducing the refactor to ownership rather than editing every writer. Alias binding is immutable for the Project lifetime. DbHandle (Mo only) retains the same capsule; no other native model may adopt it. Capsule construction alone must not be reported as evidence that every native manager constructor/lazy factory has executed: allocation/default state qualification remains separate from ownership.

Default-constructed standalone Project fixtures can allocate a private fallback capsule to retain current test ergonomics. Declare the anchor before Project members so the fallback remains alive while subprojects/Project members are released. For normal session paths, bind the SAME anchor to the actual registered Mo child in `prepareProjectModel()`/`commitProjectModelConstruction()`. `projectDb()` must adopt the same anchor when it creates a lazy Mo child in legacy adapters. Reject mismatched/non-Mo adoption. Do not allow changing reference targets after construction.

Reference members change implicit copy/move behavior. Project already contains unique ownership; either explicitly prohibit copy/move (if a bounded full-tree call-site audit confirms none are needed), or implement move construction that rebinds aliases to the destination anchor and repairs all Project backlinks. Never accept an implicit reference-member move that leaves a moved Project borrowing another Project's state. The bounded inspected tests mainly use stable stack Projects; the `p03test::Project` returned by its corpus loader is a separate test helper type and does not justify assuming m2m::Project needs moves. This needs root's full compile validation.

A similar small session registry holder is needed for `registry`; Project borrows it and default fixtures use a fallback owner. At minimum the session must retain the exact registry owner across Project deletion. Moving a map into a temporary after observers already hold it is inferior to stable owner identity from construction. Operation::opClass is a raw pointer; preservation is an architectural requirement even though no destructor dereference fault was demonstrated.

Do NOT simply make DbHandle uniquely own Project while retaining `Project::dataDb` as shared_ptr: manager -> DbHandle -> Project -> shared_ptr to that DbHandle would create a self-cycle. Keeping session as allocation holder with explicit model-driven deletion avoids that new cycle. A later fully native owner refactor would make Project's model reference nonowning and requires its own wider API audit.

## Staged destruction protocol

The coordinator owns an unforgeable/controlled stage record tied to actual session/model identity. The record describes adapter suspension and completed native work; it is not an original exception policy.

1. Require all accepted-Close guard/dialog/nested-Save/display/RemoveGraphItems/HideSecondary/refresh predecessors to have completed. Retain busy=true during this tail.
2. Resolve actual manager encounter and exact Mo model/Project/capsule identity. Extra unrepresented children, mismatch, invalid slot provenance or already destroyed native identity refuse before destructive work.
3. Run Project cleanup `0982c0 -> 0aab30 -> 12f1190(Project+60)` at its real stage. Its unrecovered payload remains guarded; a successful bool shortcut is not evidence.
4. At Project destructor stage, run real diagram null-rebind, destroy actual subprojects and remaining native Project members, then release Project allocation. Capsule and session registry/Display remain alive. Only after Project destruction returns set model.moProject=null. The existing storage helper may become a private substep here, but must not release native Mo-slot owners.
5. Drive operations slot deletion, then null its slot; repeat objects, materials, equipment, lubricants. Do not rely on capsule member destruction order. Slot callbacks may inspect the prior surviving slot identities; preserve that state until each actual boundary. Keep unknown row/Props/Qt payloads guarded.
6. Drive base-owned model cleanup at `cda520` and native QObject lifetime completion. Mark the model's logical native receiver dead independently of surviving diagnostic shared leases. DbHandle currently lacks this independent marker; add one only at the source-qualified boundary, not before reentrant destructor callbacks.
7. Drop this manager-owned child entry only once its staged destruction finishes. After all encountered children finish, run list clear and explicit current reset. Existing `clear()` supplies the available final release/list/current behavior; do not preclear current using an invalid selection call. A staged per-child clear variant must avoid replaying completed child destruction on resumption.
8. Resolve null current Project and execute actual post-clear ProjectChanged/RefreshMainView/DiagEditorFitAll receivers. Complete later Close notifier/busy-reset/return tail only when qualified. Keep terminal session receiver/dispatcher/task-store destruction separate.

The phase machine must stop on the first unknown effect without executing later phases. It cannot resume inside a partially completed arbitrary native destructor by restarting that destructor. Place guards before unresolved payloads or represent their own continuation boundaries. Adapter exception cleanup remains a separately named path; it must release resources safely without claiming accepted native Close.

## File ownership and integration boundaries for root

- `include/m2m/model.hpp`: declare slot/registry anchors and borrowed Project aliases; document native owner separately from adapter allocation holder; explicitly settle Project copy/move. Keep user-facing ABI unchanged.
- New `include/m2m/engine/mo_model_slots.hpp` and `src/engine/mo_model_slots.cpp` (or a focused model-owned equivalent): exact five-slot ownership, slot identity/access, ordered stage transitions and adapter fallback cleanup. Keep it independent of ViewManager/Qt constructor recovery.
- `include/m2m/engine.hpp`, `src/engine/db_load.cpp`: Mo-only capsule binding and logical native-lifetime state; lazy `projectDb()` adopts same storage. Base child remains a separate kind. Existing graph-manager/reader cleanup order must survive.
- `include/m2m/engine/conversion_session.hpp`, `src/engine/conversion_session.cpp`: session registry retention, Project/model/capsule identity binding and guarded destruction driver. Preserve existing `releaseProjectAfterCloseEffects()` as storage-only compatibility primitive, or rename it with all callsites/tests explicitly qualified; do not silently change its meaning.
- `include/m2m/engine/model_manager.hpp`, `src/engine/model_manager.cpp`: add a focused staged per-child clear seam only if needed; retain current final list/current ordering and existing adapter destructor semantics. Root should not convert ordinary `clear()` into a throwing destructor path.
- `CMakeLists.txt`: register only the new focused source/test target membership. Keep shared test ordering/build ownership with root.
- Tests: extend `tests/mdt/test_conversion_session.cpp` and `test_model_manager.cpp`, plus a narrow `test_mo_model_slots.cpp` if needed. Readers/writers should continue through the same p.field aliases; any broad callsite churn indicates the owner refactor escaped its intended boundary.

No source files are assigned to this worker. Root must serialize shared-interface edits and build freeze.

## Focused test acceptance

- Identity: addresses of p.operations/item managers equal actual capsule slot representations before and after DbHandle binding; a second child cannot adopt them. Type4/base child does not share the Mo capsule accidentally.
- Lifetime: destroy Project while retaining actual Mo child. Subproject/diagram owned objects expire; all five manager representations and session registry remain alive. Then explicit slot stages expire actual objects in operation/object/material/equipment/lubricant order. Observe real weak lifetimes/custom destructors, not only stage counters.
- Backlink: Project destructor effects observe moProject nonnull; post-Project slot effects observe null. Selection stays source-correct during permitted earlier callbacks, and final manager.current becomes null after child/list completion.
- External leases: a retained DbHandle allocation does not make the logical native model dispatchable after its completed QObject boundary. Existing adapter clear tests retain their explicitly weaker shared_ptr semantics.
- Suspension: stop at each unknown stage, assert later slot objects/current were not changed, then resume without double destruction or repeated earlier callbacks. Wrong model/session stage tokens refuse.
- Legacy fallback: default Project and lazy projectDb still feed the same readers/writers and clean fallback ownership once, including failure before model commit. Stack Project destructors remain nonthrowing adapter cleanup.
- Regression: registry class pointer identities, loaded Operation/GuiModel/db identities, active-subproject access, Save serialization and public m2m ABI results do not change merely from relocating ownership. Run appropriate existing model/session/Save tests and root-required regression once integrated.
- Session survivors: pending diagram timer/receiver, scene/Display, remesh, notification owner and task/slice process stores survive native Project/model cleanup. Explicit post-Close due schedule fits first and sees null binding.

These checks establish staged ownership only. They do not substitute for original-first fresh Open/Save/Close acceptance, unknown slot/Qt destructor payload recovery or actual default-dialog timing.
