# CP214: Close ownership, partial-open cleanup, and process reset

## Result

The ownership evidence is substantially more specific than the older “unknown Close root” notes, but it still does not justify implementing public normal Close or a broad reset. Static original evidence binds the successful path's manager-owned Mo child to its Project and gives conditional destruction order. It does not bind a delivered, accepted Close occurrence to the same concrete owner generations, complete the Project feature callbacks, or establish what a partially opened Project leaves behind on every failure return. The process-global evidence supports several exact stores, not blanket clearing.

No source, tests, build, original process, GUI, mutable REA session, or shared state was touched. This audit writes only this report. Target evidence is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Reconciled ownership facts

- `wave3-close-model-owner-identity-contract.json` identifies the outer `SfModelManager` separately from its ordered child list/current pointer. Each child is one same-object `SfModelBase`/`SfMoModel` identity; the Mo child owns a raw Project pointer at `+0x19c0`. The Project's `+8` points back to that same DB child on the represented normal path. The graph manager at `+0x1970` belongs to the child, not the outer manager.
- `wave3-normal-close-diagram-lifetime-contract.json` qualifies the successful conditional disposal sequence: manager clear invokes each child deleting virtual `+0x18`; the Mo destructor handles its owned Project before its model slots; Project destruction null-rebinds the still main-window-owned diagram view before subproject/member cleanup. This establishes a source order when those calls execute, not that an actual user's Close reached them.
- `wave3-normal-close-guard-contract.json` adds the successful-Open can-close gate: with Project present and process type 5, the default fresh Windows-style answer is Yes and that branch invokes a nested Project Save while still attached. Only the returned answer after that Save can allow subsequent callbacks and teardown. The qualification is static style/default analysis, not an observed original dialog interaction.
- `resume120-close-ownership-order.md` correctly distinguishes Project destruction from later operation-editor removal and from Session exception cleanup. `resume102-close-partial-order-gap.md` remains valid: explicit Save completion/failure does not replace the Close-time nested Save, and neither leaf cleanup tests nor Session destruction establish the native ordering.

## Partial-open failure cleanup

The current converter driver in `source/apps/moproj2mst/src/core/convert.cpp` performs the adapter Open/read/activation and Save sequence inside `run`; it contains no normal-Close dispatch. `ConversionSession::~ConversionSession` in `src/engine/conversion_session.cpp` explicitly identifies itself as adapter exception/final cleanup, then releases Project storage and adapter receiver/dispatcher/main-view/task scopes. That path cannot be used as native partial-Open cleanup or normal Close.

The retained Close receipts primarily describe the successfully opened, nonnull-Mo/Project branch. The current source manager-prefix helpers deliberately stop when owned children remain: `attemptCloseEmptyManagerTail` returns `OwnedChildTeardownUnavailable` for a nonempty population and does not erase child or connection entries. Existing model-manager Open integration also records that lazy Project storage is represented by preallocated adapter storage and does not claim native allocation/destructor callback parity (`wave3-model-manager-open-increment.json`). These guards are appropriate; they do not answer whether each native Open failure path leaves no manager child, a child with null Project, a Project partially initialized, or already-mutated global/file state.

## Process-global reset boundary

`resume62-close-global-reset.md` is still the controlling bounded reset audit. It qualifies the Close busy setter's exact `+9`/counter writes and conditional `+A/+B` values, the earlier post-manager-clear main `+0x3c0` write, the process-type-5 write in the mode-0 callback path, and Project-specific feature cleanup through Project `+0x60`. It also qualifies a fresh global-main lookup for late status reporting. It explicitly finds no evidence for clearing all engine globals, Fortran units/caches, notifier maps, graph registries, remesh storage, or pending events. Absence from those selected direct bodies is not proof that all transitive callbacks are inert.

The adapter's process codec lifetime and mutex in `convert.cpp` are reconstruction mechanisms. They do not establish that original engine globals reset at Project Close, nor that Session destruction should clear them. Keep individually source-qualified writes and preserve unknown process state behind guards.

## Exact unresolved branch and next action

The precise unresolved behavior is the same-owner path from an accepted normal Close after successful Open through the process-type/can-close checks, modal delivery and nested Save result, post-answer callbacks, manager child deletion, Project feature cleanup, and Project/SubProject destruction. For failure cleanup, the missing branch is each native Open/read/activation failure edge correlated with its live manager-child and Project allocation state at return/unwind, plus any prior durable file or process-state effects. Static helper composition and synthetic tests cannot supply these original occurrences.

The smallest next evidence action is a single permitted, source-produced native occurrence capture that records concrete main/manager/Mo/Project identities and allocation generations across one successful Open and accepted default Close, including nested Save return and destructor entry/return. In the same bounded capture contract, record each reached failure return in the loader's post-allocation region only if it can be observed without restarting or broadening the original run; otherwise retain it as a separate unresolved failure-path matrix. Do not add production cleanup or a public Close integration before this owner/effect cursor exists. If original runtime evidence remains unavailable, the smallest implementation action is none: preserve current stop points and report the gate as open.

## Qualification

This is a reconciliation of retained static PE/source contracts and candidate guards, not a fresh original runtime observation. Static ownership/order facts do not prove Close delivery, acceptance, callback completion, destructor return, or process-global reset. Synthetic converter tests establish only the candidate behavior they execute and must not be cited as evidence of original behavior.
