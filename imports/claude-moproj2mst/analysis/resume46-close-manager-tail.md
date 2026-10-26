# Resume46 accepted-Close manager tail

`ModelManager::clear()` already implements the available ordered owner-release/list-clear/current-reset API. Root does not need a second setter or manager registry. It does need an explicit native child-teardown seam before this projection can be called faithful normal Close: Project currently owns data that belongs to later Mo model manager slots.

Scope: static immutable evidence/current source only. No builds, tests, original execution, GUI/app control, mutable REA or shared-source changes. Output ownership is this report alone. Original: retained DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact source-qualified sequence

The following is the accepted path after preceding can-close and receiver effects have completed. It is not a way to bypass them.

| Order | Original instruction evidence | Required transition |
| --- | --- | --- |
| 1 | `029a54` busy setter; `029a61 -> 02aae0`; `029a76 -> c78300` | Set busy, complete null refresh dispatches, then enter actual manager clear. Busy remains set during child destruction. |
| 2 | `c78300`: manager+18 iterator setup; `c784e0..c784f2` loads child's vtable and invokes virtual+18 with flag 1 | Invoke each encountered live child's deleting destructor, rather than drop every reference and assume it ran. Preserve recovered encounter order for the ordinary fixed list. Reentrant insertion/removal/copy-detach behavior is not independently closed here. |
| 3 | Mo deleting wrapper `045350 -> 044040`; `044040@044066 -> 0440c0`, then `044071 -> cda520` | Mo-specific cleanup precedes base cleanup. |
| 4 | `0440c0@0440ce -> 044bf0`, then `0440d8 -> 045000` | Project cleanup/delete precedes the five Mo-owned manager slots. |
| 5 | `044bf0@044bfe` tests model+19c0; `044c14 -> 0982c0`; `044c46 -> 0453e0`; `044c60` stores model+19c0=0 | If Project exists: run `0982c0 -> 0aab30`, delete Project, then clear its model backlink. Null skips this stage. Do not null the backlink before Project destructor callbacks. |
| 6 | Project destructor `08a3c0` binds diagram null before `0a80e0` clears subprojects | Real diagram null-rebind effects belong here, while Project data remains valid. This does not cancel timers or destroy session receiver. |
| 7 | `045000`: slots +19c8, +19d0, +19d8, +19e0, +19e8; deleting calls at `045056`, `0450c6`, `045136`, `0451ac`, `045222`; corresponding zero stores at `045071`, `0450e1`, `045157`, `0451cd`, `045243` | Delete each nonnull slot, then null that slot, in ascending order: operations, objects, materials, equipment, lubricants. Exact row/item destructor effects remain separately qualified. |
| 8 | `044040@044071 -> cda520`; graph cleanup receipt binds `cda520 -> cda6f0` and +1970 deletion before reader | Finish native base-owned managers and members; QObject destruction follows its proper native boundary. Cached graph-factory ordering must remain source-bound. |
| 9 | `c78515` temporary iterator cleanup; `c7852b -> 20db90(manager+18)`; `c78538..c7853e -> cbc920(manager+20,null)` | After the iteration completes: clear manager owning list, then explicitly reset current guard. Do not reset current first. A QPointer may already null when its QObject dies; this final reset is a distinct instruction. |
| 10 | `029a83 -> 01d510`; `029a93 -> 02a0e0` | Post-clear Project lookup must see null current and must not enter lazy Project reconstruction on a retained model. Then actual ProjectChanged/RefreshMainView/DiagEditorFitAll delivery. |
| 11 | Later `029c8d -> 2cf3c0`, then `029c9b -> 2ca9f0(false)` | Busy reset is later than manager clear and another notifier call. This report has not qualified that notifier key/recipients, so a manager-tail implementation must not claim it completed the whole Close return tail. |

Static source qualifications are inherited from `wave3-normal-close-diagram-lifetime-contract.json`, `wave3-close-model-owner-identity-contract.json`, `wave3-close-graph-manager-cleanup-contract.json` and `wave2-doe-model-manager-destructor-amendment.json`. Direct bounded ASM was checked for the manager/Project/slot order above. No original runtime timing, arbitrary reentrant mutation or exception/fault behavior is asserted.

## Current API and concrete mismatch

`source/apps/moproj2mst/src/engine/model_manager.cpp::ModelManager::clear()` does:

1. `model.reset()` for every vector member in order.
2. `models_.clear()`.
3. `current_.reset()`.
4. `connections_.clear()` as an adapter disconnection projection.

This preserves the available fixed-list owner-release and explicit final current-reset order. There is no reason to add `setCurrent(nullptr)` or infer clearing from `selectModel(nullptr)`; that selector deliberately retains current when selection is absent. `currentModel()` checks both weak-pointer liveness and membership, so after `clear()` even a surviving external lease is not current. Existing tests cover sole-owner release and external-lease survival, with qualifications.

`ConversionSession::releaseProjectAfterCloseEffects()` only guards a nonnull bound diagram and calls `releaseProjectStorage()`. The latter keeps Display alive, resets adapter ProjectScope, holds a local dataDb lease, destroys Project, then clears dataDb->moProject. It intentionally does NOT clear the manager. A current test expects list size 1/current identity to remain after this storage-only primitive.

The deeper mismatch is `include/m2m/model.hpp::Project`: its `operations`, `objects`, `materials`, `equipment`, `lubricants` explicitly map to native Mo slots+19c8..19e8, but are owned as Project members. There is no custom Project destructor. Reverse member destruction therefore destroys these slot projections during Project deletion in the order lubricants, equipment, materials, objects, operations, and only later destroys the earlier-declared subprojects. Native order is subprojects inside Project deletion, model+19c0=null, then operation/object/material/equipment/lubricant managers. This is established C++ ownership/order, not a reproduced runtime defect.

`DbHandle::~DbHandle()` only has a supported postGraphManager reset prefix; it does not own or delete `moProject`. DbHandle has no native-lifetime marker independent of its shared allocation. `GuiModelGuard` guards a different SfStepData identity; it must not be reused to pretend it guards the SfMoModel/DbHandle child.

## Minimal integration API recommendation

For the **currently supported projection**, use the existing API pair from an explicitly guarded root continuation:

`releaseProjectAfterCloseEffects(); modelManagerOwner()->clear();`

This is sufficient to remove retained manager selection AFTER Project storage release. It must be named/reported as projected owner-list completion, with native child-teardown parity still open. It must not replace storage-only helper semantics or be called from exception cleanup to masquerade as accepted normal Close. No new public ABI is needed.

For the **faithful accepted-Close architecture**, there is no sufficient existing ownership API. Add one root-owned per-child destruction seam at the `c78300` encounter, with phase retention when a recovered effect refuses. Its child identity is the same DbHandle/SfMoModel already in the manager, not a duplicate owner. The seam must drive Project cleanup, real null-rebind/Project destruction, backlink clear, five slot deletions and base cleanup before the manager drops that child's owning entry. Only after all children finish does the existing list/current tail run.

The seam needs a model-owned lifetime for the five slot projections, or an equivalent explicit owner capsule with Project borrowing the same live objects until Project destruction finishes. Simply moving those fields out early is not validated: Project/subproject destructor callbacks may still consult them. Simply leaving Project allocation alive, setting a boolean dead flag and clearing vectors is also not full architecture recovery. Root can stage the owner refactor behind the new seam and keep the unrecovered payload guarded. This is the smallest honest seam, rather than a recommendation for a speculative completed refactor.

Do not add broad notifier/global reset. Do not clear native graph pointer registries as a convenience; they have separate reset versus destructor contracts. Do not make external shared_ptr leases count as native-deletion evidence. For the supported fixed fresh single-project profile, initially guard extra/unrepresented model populations instead of guessing their teardown.

## Focused tests root should add

1. **Existing primitive preserved:** current selected model survives storage-only release, moProject becomes null, manager size remains 1. Keep the present assertion unchanged.
2. **Projected tail:** after an explicit accepted-phase fixture and storage release, call actual `clear()`. Assert list size 0, current null/ID -1, modelAt/modelWithId absent and signal connections cleared. Retain a lease and confirm it cannot be reselected; label lease survival adapter-only.
3. **No accidental lazy reconstruction:** after tail, `currentSubProjectDbOwner(*manager)` returns null because current is absent. Compare this with storage-only release, where the retained Mo current and null backlink currently refuse a missing lazy Project. This directly catches the integration gap without manufacturing native callbacks.
4. **Native child seam order, once implemented:** use actual session model plus controlled effects that inspect real owner fields at each boundary. During Project cleanup, model backlink/current remain bound; during post-Project slot destruction backlink is null; slot destruction order is operations/objects/materials/equipment/lubricants; base cleanup follows; final current-reset follows every child. Counters alone cannot establish object lifetime: observe actual object identities/weak guards or destructors.
5. **Refusal after a completed phase:** refuse the first unresolved null-rebind, slot destructor or base cleanup payload. Verify no later phase, manager list clear/current reset or post-clear ProjectChanged is executed. Resume must not repeat a completed side effect. Adapter refusal is not an original application error policy.
6. **Session survivors:** pending timer/receiver, primary and secondary scenes/Displays, remesh, notification owner and process allocation stores survive manager clear. Timer dispatch after real null binding uses explicit due observations, performs fit first and does not click freed project data. Session terminal destruction remains separate.
7. **Negative entry:** busy failure, can-close Cancel or uncompleted display/RemoveGraphItems/HideSecondary/refresh phases must not enter the child seam. A qualified phase token should originate in the coordinator, not arbitrary callers setting `accepted=true`.

No tests were run in this lane. Existing tests demonstrate adapter behavior only; they do not validate native QObject invalidation timing, Project/slot destructors or accepted normal Close.

## Prior dependencies remain explicit

Before the production normal-Close coordinator may reach this tail: actual busy/can-close ownership, fresh default question/modal delivery, nested Save's independent integer return, non-Cancel display callback, RemoveGraphItems with both actual scene lookups, complete HideSecondary recipients/Qt effects, Tree/Property/Scene refresh recipients and real diagram null-rebind must be closed. The View_Default/Primary aliases remain guarded. `0aab30 -> 12f1190(project+60)` payload and several destructor callbacks remain unexpanded. Root can integrate a guarded continuation and its supported projection now; end-to-end accepted Close remains incomplete.

## Frozen references

- `wave3-boolean-close-140c78300.asm`: `f4f560ad36054ac0b16d03f4a542db5aec4d366eb6ef75a15b1291fd11790445`
- `wave3-boolean-close-1400440c0.asm`: `329c7dd97d4205ab649199c29b0803e5835c89a2cb76dd094092d5bf66de640b`
- `wave3-boolean-close-140044bf0.asm`: `e0f190d0dbe7bc2697dc030ed3733cd5e1ad386e02b2b6f1692fee65e543caf6`
- `wave3-boolean-close-140045000.asm`: `c8352a4f6c7a5188e21abb6c935f097e7d78280e322008e6d036e68daad2fd4a`
- `source/apps/moproj2mst/include/m2m/model.hpp`: `5a7af0a1e6e6263fa72970d8f84cc18a55dd3385c46813e3be453dc61f671846`
- `source/apps/moproj2mst/include/m2m/engine/model_manager.hpp`: `7c55be276bc833f1681d81ebccbc87612d9e3f61e7d486c9388e43dda7202e59`
- `source/apps/moproj2mst/src/engine/model_manager.cpp`: `69f426a43ffead038991178fe7106c3fd3f4b46b93fc4968b48d0dbd2f94972d`
- `source/apps/moproj2mst/src/engine/conversion_session.cpp`: `0224745a5136a0e5b2870c5d4859638268b741959887508f7bcf16fce4fc1d14`

ASM paths are beneath `analysis/`. Current source may advance after these inspection hashes. No build or deployment occurred.
