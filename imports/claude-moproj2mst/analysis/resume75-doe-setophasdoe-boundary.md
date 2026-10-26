# CP75 B121 `SetOprHasDoe` boundary

## Decision

Keep public nonempty `DoeVariables` admission guarded. The native B121 scalar path reaches a synchronous `slotSetOprHasDoe(operationId=1, hasDoe=true)` after committing the variable, keyword, and operation-list prefix. Normal dispatcher completion returns to the parser, which then performs later keyword setters and variable finalization; the dispatcher Boolean does not control parser success. Exceptions/faults can interrupt this sequence, and the recovered code has no rollback. Candidate `SetOprHasDoeSessionBinding` still cannot establish the runtime receiver list/backend state, so neither static evidence nor its unavailable-population refusal resolves public admission.

## Original source-qualified ordering and effects

The evidence is bound to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The bounded native captures and decoded call sites are documented in `analysis/resume50-doe-variable-body-pe.md` and `analysis/resume51-doe-variable-notify.md`.

Immediately before delivery, native `0x14090ee20` has already:

1. Created/appended the variable via manager `vt+0xf8`; append precedes name assignment. It selected the new row and marked all three sample-invalid flags true.
2. Written `varId=1` and `dimension=3`, and copied the nonempty description.
3. Collected `TLOC` keyword attributes and `NominalData` text in locals. The keyword record was constructed/appended with numeric nominal value `0.1`, parameter selector constructor value `-1`, and empty committed nominal text. The variable operation list was merged with operation 1.
4. Entered derived manager `0x1400d25a0`, which adds the keyword through its base helper, then invokes the global messenger for each keyword operation. The actual argument order at the slot boundary is `int operationId` then `bool true`; B121 sends `(1,true)` once.

After successful return from all notifications for that keyword, the parser retrieves the keyword, writes selector `-1` and false function/enum flags, commits the collected nominal text, finishes the remaining child processing, reindexes variables, resolves deferred links, and returns true. B121's scalar-double branch does not call `0x140916530`; it does not establish a direct `temp.KWD` write. The root-level `Operations` sibling is processed by the outer reader only after `DoeVariables` returns.

The native messenger dispatcher (`0x1402cf3c0`) first checks suppression; suppressed delivery returns before flag or map effects. Otherwise it resets notifier `+0x90` to false, obtains/default-inserts the key list, snapshots it, and visits receivers in order. Expired weak guards are skipped/removed from the local snapshot, not shown to be pruned from the persistent registration list. Live slots are invoked synchronously with DirectConnection. The returned `invokeMethod` Boolean is stored but not branched on; an ordinary false result neither stops later receivers nor becomes parser false. Native exceptions/faults may unwind; no rollback is visible.

The identified slot's recovered decision points matter for the minimum probe: null model backend returns before logical-item lookup; with a backend, no matching operation item returns before icon allocation; a matching item with `true` allocates/configures a QIcon and invokes the backend primary-icon setter. The latter needs actual icon/backend ownership evidence. Initial constructor-null backend is not evidence it remains null during lazy DOE parsing. See `resume51-doe-variable-notify.md` for limits on receiver inventory, runtime metadata, QObject liveness, and backend identity.

## Candidate boundary and remaining mismatch

`src/doe/variable_prefix.cpp` currently commits the adapter's staged variable/keyword/operation prefix and then throws `M2M_E_UNSUPPORTED` before dispatch. It retains the pending key, `(operation=1,value=true)`, parsed parameter, and uncommitted nominal text. Its owner retention preserves this diagnostic state through unwind; it does not represent the native parser's normal return or native destructor behavior.

`src/doe/variable_notify.cpp` binds only the existing session notifier owner and actual Project/StudyIo identities. On unsuppressed delivery it records the native flag reset, then refuses because ordered receiver population is unavailable. That is a deliberate model boundary, not an observed original exception. Its explicit test receivers are candidate-controlled bindings, not a qualified native runtime census. Static constructor evidence shows flow-chart registrations exist; their count/order and current backend at B121 execution were not observed.

## Smallest probe to resolve admission

The minimum qualifying observation is one fresh, disposable original B121 Type2 read with exact owned Study/I/O, captured at `DoeVariables` and notifier entry/exit—not a Save round trip. Record a compact receipt containing: original image hash; input XML hash; selected SUB/Study owner identities; notifier suppression/`+0x90`; ordered live/expired receiver identities and bare methods; for each reached receiver, actual model identity, backend-null status, matching item presence for operation 1, whether QIcon/backend branch is entered and its return/exception; then parser return/exception and checkpoints immediately before and after keyword text commit/reindex. This single run discriminates suppressed return, receiver early-return, icon/backend delivery, and unwind, while preserving the pre-notification committed prefix.

Before that run can be scheduled, the existing GUI-control release gate must have a supported release operation and verifiable receipt; retained instructions explicitly prohibit reacquiring control without it. If instrumentation of actual receiver state is unavailable, even a successful visible UI parse is insufficient to generalize the empty/null-backend path: keep public Type2 guarded and add only candidate tests for the bounded branches. Do not infer parser success from the candidate binding's `ReceiverListReturned` outcome or its `Population` exception.

## Scope

This is a read-only source/evidence trace. No source, build, test, GUI, original execution, or mutable REA state was changed or run. It makes no fresh-original parity claim.
