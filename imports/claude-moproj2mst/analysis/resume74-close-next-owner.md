# CP74: isolated empty ModelManager clear/current-reset tail

The next bounded cleanup edge independent of the status cache is the **actual Session-owned ModelManager's empty-list tail**: finish the zero-child iteration, clear the owning list, then explicitly reset current. The candidate already owns this manager and exposes its real population. An empty branch avoids every child/Project/OPI/Qt destructor dependency. It must stop before the subsequent post-clear Project lookup/notifications and must not be promoted to completed public Close.

A failed Open after model registration is the decisive negative case. Its actual manager is nonempty even if the Mo child's native moProject field is still null. Null Project makes one can-close guard immediate, but does not make child destruction empty or authorize clearing the list/current first. That failure snapshot must stop before the first owned-child deleting virtual, retaining the model, Project storage and connection metadata.

This report is the sole write. No source/build/tests/GUI/original execution/mutable REA operation occurred. Method: current source and retained native Close/manager captures; bounded offline PE code-hash verification. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable or deployment was inspected or changed.

## Native edge and ordering

`wave3-normal-close-diagram-lifetime-contract.json`, `resume46-close-manager-tail.md` and retained `wave3-boolean-close-140c78300.asm` establish:

1. Native0299a0 must first pass busy/can-close guards, set busy true and run null refreshes.029a76 then calls actual manager c78300. This report does not close those preceding effects or supply an accepted-phase shortcut.
2. c78300 constructs its temporary iterator/copy at c7832f. Its empty-iteration condition takes c783c8 ->c78515 without invoking a child's deleting virtual. Nonempty iteration eventually reaches actual child virtual+18(flag1) at c784f2. No nonempty child is supported by the proposed helper.
3. c7851a destroys the temporary iterator/copy; c7852b calls20db90 on manager+18, the owning list; only afterward c78538..c7853e calls cbc920(manager+20,null), explicitly resetting current.
4. After return,029a83 obtains the current Project via01d510 before029a93's02a0e0 update/notifications. The proposed helper stops before029a83. False-busy/status/ShowMsgInArea are later and remain separately guarded.

This is the semantic list/current owner contract. Qt QList temporary/refcount/allocation behavior and QPointer's internal guard bookkeeping are not newly implemented or certified by an STL projection. In particular, an empty candidate vector is not a captured native QList backing allocation. The selected branch closes absence of child deletion and the explicit state-reset order; physical Qt container equivalence remains qualified.

## Actual candidate owner and existing implementation

Paths below are relative to `source/apps/moproj2mst/`.

- `ConversionSession::ConversionSession` in `src/engine/conversion_session.cpp` constructs `modelManager_ = make_shared<ModelManager>(this)`. `modelManagerOwner()` returns that actual owner. Fresh manager list/connections are empty; there is no need for a caller-supplied `hasModels` fact.
- `ModelManager::clear()` in `src/engine/model_manager.cpp:111` already releases each child in encounter order, clears `models_`, resets `current_`, then clears `connections_` as an adapter QObject-disconnection projection. For the narrowly admitted empty-list/empty-connections branch it invokes no child destructor and erases no connection entry. The existing API is sufficient; no new shared manager interface is required for that branch.
- `currentModel()` validates both weak liveness and list membership. A null result with size0 is not direct proof that the underlying current guard has already received its explicit reset. Call the real clear primitive rather than pretending the instruction is unnecessary or using `selectModel(nullptr)`, which deliberately retains current when selection is absent.
- `prepareProjectModel()` in `conversion_session.cpp:264` constructs the Mo child, adopts the Project's slot owner, sets its path, appends it, registers signals and assigns Project.dataDb. `commitProjectModelConstruction()` stores moProject only later. Therefore size1/moProject-null is a concrete candidate boundary before native Project binding, not an empty manager.
- The core's `src/core/convert.cpp` registers the model before Project initialization/read and maps exceptions after Session unwinding. That final adapter cleanup is not an executed normal-Close path. A new helper must not call Session destruction, `releaseProjectAfterCloseEffects()` or `releaseProjectStorage()` to implement manager clear.

Existing `test_model_manager.cpp` covers ordinary clear, encounter-order adapter release and external shared-lease survival. `test_conversion_session.cpp` covers actual Session registration/selection and explicitly expects manager/current to survive the narrower Project-storage release. Neither establishes the proposed **isolated source-occurrence gate on an actual empty Session manager**, nor a nonempty failed-Open refusal at that gate.

## Exact minimal implementation proposal

Use new exclusive `include/m2m/engine/close_empty_manager_tail.hpp`, `src/engine/close_empty_manager_tail.cpp` and `tests/mdt/test_close_empty_manager_tail.cpp`, with root coordinating integration/build. Do not change public Close, C ABI, ModelManager's shared interface or Session's existing cleanup behavior.

An isolated progress object binds the actual Session and weak actual ModelManager identity, plus the exact occurrence `0x140029a76:0x140c78300`. A provenance string is required but explicitly is not authentication or proof that prior accepted-Close phases ran. Stages should distinguish entry inspection, blocked owned-child teardown, tail committed and post-clear lookup guard. There is no public Boolean Close-success return or API to mark prior child effects complete.

On its first attempt:

1. Obtain `session.modelManagerOwner()` internally. Require the same canonical manager/control block, `ownerSession()==&session`, matching occurrence/progress and nonblank provenance. Mismatch/expired binding throws `M2M_E_UNSUPPORTED` before mutation.
2. Inspect the real population. Nonempty latches `OwnedChildTeardownUnavailable` at c784f2, retaining the first encountered child's diagnostic identity/ordinal without dereferencing it on later retries. No list/current/connection/Project mutation occurs. A later external clear cannot turn that blocked occurrence into the empty branch; a population change must refuse rather than skip the blocked child phase.
3. Require actual empty connections for this narrow projection. If entries exist, stop before `clear()` because its extra connection erasure is not supplied by a child-free native tail. The current API normally reaches size0 with empty connections, but the helper should keep the boundary explicit rather than silently widening it.
4. For empty list/connections, invoke this same actual manager's existing `clear()` once. Mark the semantic owning-list/current-reset projection complete and return `BeforePostClearProjectLookup`, next site029a83. Do not call `currentSubProjectDbOwner()`, lazy Project reconstruction,02a0e0 or a notifier.
5. On retry after commit, revalidate the same Session/manager and require it remains empty. Return the retained guard without clearing again. If a new model was appended, refuse before mutation; do not clear that later population under the old occurrence.

This uses the native empty-population branch, not an `accepted=true` override. It does not permit a nonempty manager to be dropped just because an external adapter lease would keep the child allocation alive. Production entry still requires the coordinator to reach c78300 through the actual prior guards/effects. The helper's isolated entry is useful for source-bound staging and focused tests only.

## Focused test and failure contract

Proposed automatically discovered target: `m2m_mdt_close_empty_manager_tail`; use Release-active Testkit main/includes and CHECK/REQUIRE. Root builds/runs it with the coordinated candidate. No target or test file was created here.

1. **Actual fresh Session:** require manager.ownerSession points at that Session, size0 and empty connections. Attempt reaches the post-clear guard once. Retain the same Project, slot owner/next slot, scenes/Displays, MainView, WidgetRuntime, notification owner, remesh owner/value and diagram receiver/pending metadata. No driver callback or notifier occurs. Do not label this fresh candidate state a captured original Close.
2. **Failed Open before Project binding:** call actual `prepareProjectModel`, retain its actual child and signal registrations, and verify moProject is null. Attempt must stop at child teardown, preserving list size/child identity, connections, Project.dataDb and slots. This is a declared candidate failure-boundary fixture; it is not a claim about an original modal/default schedule.
3. **After Project binding/selection:** prepare/init/commit construction/select as in existing Session tests. The same nonempty guard preserves actual current identity and moProject backlink; it must not reset current before child deletion. This independently catches misuse of `currentModel()==nullptr` or moProject-null as empty-list admission.
4. **Blocked retry cannot bypass:** after the nonempty guard, externally clear/change the population in the fixture. Retrying the old progress refuses before effects; it cannot claim skipped native child teardown. Also test an empty successful occurrence followed by appending/selecting a new child: retry must preserve that new child/current/connections.
5. **Ownership/mismatch:** foreign Session, replaced manager identity/control block, missing qualification and wrong source occurrence refuse before mutation. Qualification labels do not intern unrelated owners or certify native execution.
6. **Pending events survive:** a declared Session event fixture can verify this primitive does not cancel/coalesce timers, destroy receiver/dispatcher or consume due events. It must not be called a full native diagram-null rebind or normal Close replay.

There is no rollback after a completed native/projection phase. Adapter refusal before child teardown is not an original error return. No can-close Cancel or modal branch is reinterpreted, and the empty helper does not turn a conversion failure into completed accepted Close.

## Receipt and remaining uncertainty

Retained ASM artifact `wave3-boolean-close-140c78300.asm` SHA-256 remains `f4f560ad36054ac0b16d03f4a542db5aec4d366eb6ef75a15b1291fd11790445`. Independent PE/.pdata verification in this lane gives direct function bounds140c78300..140c7854b and code-byte SHA-256 `b2c665e2182ec5050431091d4859aafd7d86e1ca1ca2e4e7d14a36c5bf7b1487`. No new raw evidence file was written.

Unresolved dependencies remain prior busy/null-refresh effects and phase reachability; every nonempty model child/Project/OPI/slot/base/QObject teardown; physical Qt iterator/list/guard effects; post-clear main/Project lookup and notifier population; reentrancy outside the selected stable empty branch; and full fresh original one-project acceptance. The status-item cache is not involved in this isolated reset edge and is not implicitly initialized or cleaned by it.
