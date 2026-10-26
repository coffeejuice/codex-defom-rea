# CP67 — Report document constructor prefix: blocked at QObject owner effects

No bounded constructor prefix can currently be integrated into actual Report Open without fabricating native QObject state. The constructor entry and class-member initialization are statically mapped, but the ordered effects before and during QObject construction are not executable in the current source model for this actual parented object.

## Exact blocker

Original `FUN_1400b5ad0` lazily allocates a `0x70` Report document, obtains the main-window QObject, calls `0x140643100(document, mainWindow, mode, mode != 2, 0)`, and stores the returned object in the subproject `+0x30` shared slot. `FUN_140643100` then calls the Qt QObject constructor with this non-null parent before initializing document members and registering seven main-window connections.

The existing QObject evidence does not qualify this constructor route:

- The available `analysis/wave3-doe-report-qobject-prerequisite-contract.json` is explicitly static-only and says no QObject/private/thread/counter state changes are closed for execution. It identifies missing shared empty-container ownership, actual TLS index/value, `QThreadData` identity/reference ownership, and ordered counter commit. Its investigated public path uses QObject with a null parent, whereas the Report document passes the main window as a parent. Therefore parent validation, child publication and related parent callbacks are not covered by the null-parent path.
- `include/m2m/engine/report_qobject_ledger.hpp` is an inert observation ledger. It consumes observations supplied by a caller; it does not construct a QObject or own shared Qt/QThreadData state.
- The Report constructor capture qualifies member initialization and seven connection registrations, but not their signal/slot semantics, parent-child effects, exception unwind, or destruction/Close ordering. No corresponding executable Report document owner exists in `Project` or `SubProject`.

Consequently, representing only the post-constructor members or adding an isolated helper would skip the real main-window QObject effects and fail to bind a live returned object into `SubProject+0x30`. There is no safe partial constructor prefix to integrate into public Open. Keep the existing `ReportClass::loadClassData` refusal before document construction and retain the Save guard.

## Minimum prerequisite to reopen this decision

Qualify the actual Report document constructor in a fresh original process, bound to the actual main-window parent identity: shared empty-container increments/backlinks, TLS fast path and selected `QThreadData`/refcount, parent validation and child publication, process QObject counter, then all seven connections. Pair that with a native subproject/Project Close trace establishing slot release and QObject-parent teardown order. The null-parent DOE QObject contract and `ConversionSession` adapter cleanup do not satisfy these requirements.

Read-only evidence review. No source/test files were created, and no build, GUI, runtime capture, or mutable REA action was performed.
