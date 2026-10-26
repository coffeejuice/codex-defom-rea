# CP105 — decision on run-scoped Unknown ApplicationState binding

## Decision

The proposed value-owned, default-Unknown object is **C++ lifetime-safe as an inert invocation context** if it is declared before `run()`'s `try`, borrowed by a nonmovable Session, and never used to select a native branch or perform cleanup. It is not a process-lifetime application owner, does not make the detached `ApplicationStateProjection` live, and does not establish native startup/main state.

I do **not** recommend integrating this owner/Session binding before at least one real producer is ready. In the present source it would add a second layer of owner metadata without a consumer or source transition. Tests could prove only that a stack object outlives its Session and stays Unknown; they would not advance Close or DBLIST behavior. If root chooses to stage it anyway, name/document it as an invocation-local context and explicitly scope it to one fresh-app/one-project profile. Do not call it a process-lifetime owner or let it satisfy a native-main/Close prerequisite.

## Why the borrow itself is safe

In `core/convert.cpp::run`, a value declared after the static engine lock and before the `try` would remain alive while the Session local is constructed and destroyed inside the try. On exceptions, the Session is destroyed before entering the catch, while the value remains live until `run()` returns. A noncopyable/nonmovable owner plus a nonowning Session pointer declared before Session-owned members prevents accidental owner duplication and makes the intended lifetime ordering explicit. The existing default `ConversionSession(driver)` can remain unbound and must not allocate a fallback.

The scaffolding stays inert only if these constraints hold:

- default state is entirely `Unavailable`; its constructor performs no native-reset writes, publication, GUI/QObject allocation, I/O, or throwing allocation;
- Session stores only the borrowed C++ relationship; its constructor/destructor and Project release do not mark startup, synthesize null-main, clear busy/counter fields, or change DBLIST admission;
- no Close/Open/Save/DBLIST callsite reads the owner until that callsite has an actual source producer and occurrence-qualified state;
- no `ApplicationStateProjection` value is copied into it or used as a live provider;
- owner scope identity is not a native application identity. Expiry means candidate scope unavailable, not native teardown.

Under these conditions the new link alone should not change public output, but it would still need root-owned static/preflight review and an ownership-only test. Such a test is scaffolding validation, not behavioral evidence.

## Scope mismatch remains

Native `0x143d766b8` is one application-global object through application startup, main publication, Open, explicit Save, Close, and main teardown. Its `+0x1c` DBLIST history and current-main/busy state survive Project-level transitions. The candidate's `run()` function is entered by both CLI and DLL paths for each conversion request, serializes only process-wide engine operations, and creates/destroys a stack Session. The DLL caller may invoke it repeatedly. Therefore a `run()`-scoped object survives exactly one Session/Project attempt, not the native application process or a sequence of host operations.

CP104's selected “one fresh original, one project” profile can choose to treat one `run()` as one simulated application only if that profile is explicitly made part of the public invocation contract. Current CLI/DLL contracts do not declare that equivalence or expose app startup/teardown. A process-static object would have the opposite mismatch: it would survive host calls but no source path would initialize, publish, or clear its facts at the corresponding native phases. A process lock is not a lifetime provider.

## Producer order and safe next slice

The retained native order remains: app-state initializer prefix first; later primary main construction and prepublication effects; publish the same main at `0x140c9957b`; initialize main `+0x50` to null; then create the concrete manager. Open/Save/Close setters and current-main reads follow later. CP101's Ghidra xrefs timed out, and the candidate has none of these application/main producers. Thus no present source location can advance an Unknown run-scoped holder to a known native value.

The next executable implementation should be the first actual producer boundary itself, not just the holder: define a host/application-root lifetime and a main construction/publication owner, then inject that same owner through the calls that must share it. If scope remains the one-shot converter request, first add an explicit internal/public profile that establishes a fresh simulated application per call and then implement its source-ordered startup producer. Until then, the safest action is to defer the binding; keep native app facts unavailable, DBLIST/ReportList refused, and public normal Close guarded.

## Receipts

| Artifact | SHA-256 |
|---|---|
| `analysis/resume104-application-state-insertion.md` | `e76477e6f8a07774e2faf6306365f0e6bf9053784cf5a92e32b2d0adb363fce0` |
| `analysis/resume104-application-owner-integration-review.md` | `1351664e8dba14e984a7964a74b70241bc5156aa50a565c549049ab632f2bb3c` |
| `analysis/resume105-close-producer-trace.md` | `b38da3b7ca608c961adc252ffa9c2ecdaaadb0ccc22e50d4ca8bfef460058e9f` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31` |

Read-only design review. No source, build, GUI, original application, or REA database action was performed.
