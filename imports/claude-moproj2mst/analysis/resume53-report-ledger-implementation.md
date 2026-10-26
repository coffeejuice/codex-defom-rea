# Checkpoint 53 Report QObject shared-reference ledger implementation

## Change

Added a standalone `ReportQObjectSharedRefLedger` in `source/apps/moproj2mst/include/m2m/engine/report_qobject_ledger.hpp` and `src/engine/report_qobject_ledger.cpp`, plus `tests/mdt/test_report_qobject_ledger.cpp`. CMake already glob-discovers `src/**/*.cpp` and `tests/*/test_*.cpp`; no build-file edit was needed.

The ledger is dormant and instance-local. A caller supplies a nonzero opaque stable-allocation identity at construction and must repeat it on every observation. Every accepted QtCore observation must carry the qualified QtCore SHA-256. The fixed accepted symbols/helper are only the retained source-qualified RVAs: QListData `0x2f0a50`, QString `0x2f0b30`, QVectorData `0x2f0bb8`, thread-ref helper `0x27630`, and process QObject counter `0x2f4344`.

The ledger accepts, in order: four private shared-ref events (QList, QString, QVector, QList), reciprocal object/private backlinks, initialized-index/non-null TLS identity, selected QThreadData ref-helper increment, and final process counter increment. Each `+1` check uses uint32 arithmetic, so wraparound is retained. It stores the four observed before/after pairs and exact self-attributed deltas `QList=2`, `QString=1`, `QVector=1`; it does not infer or store an absolute base or an absolute resulting global count. A bad observation returns `false` without advancing the phase or mutating the ledger. Duplicate, wrong owner, wrong module/RVA, wrong order, non-fast-path TLS, pointer mismatch and wrong increment are rejected.

The type makes no target-memory writes/calls and has no integration with `WidgetRuntimeState`, `QObjectConstructionRecord`, `PendingTaskPrototype`, TaskFactoryRuntime, or the Report guard. The last counter observation only records its supplied values; it does not invoke the existing runtime counter helper. Completing this in-memory ledger does not complete a native QObject base and does not set `qobjectBaseCompleted`.

## Isolated test coverage added

The new test file covers ordered shared-ref deltas, uint32 wrap, separate QList events, non-adjacent before-values (so unrelated process increments are not assumed absent), wrong module/RVA/frame identity, invalid ordering, invalid backlink pair, missing-index/null TLS rejection, TLS identity equality, helper identity/RVA and increment validation, counter-last ordering, duplicate rejection, and zero allocation-token rejection. These tests check only the ledger's mechanics with synthetic inputs; they do not establish that an original DOE constructor reached these events.

## Status and limits

No build or tests were run, per task instruction. No GUI or mutable REA work was performed. Root review is required before wiring this ledger into runtime ownership. Runtime observations for actual shared-global bases/deltas, the actual TLS fast path, QThreadData identity/refcount, and same-frame Report ownership remain unavailable. The public DOE Report construction guard must remain in place; this change does not claim Report support.
