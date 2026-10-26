# CP171: Cogging Save event-pump, CWD, and operation identity audit

## Result

The post-CP156 record still supports a static branch for **possible CWD mutation**, but not a source-bound branch for which project or operation the continuing Save uses. During the single `QCoreApplication::processEvents()` call in the Save prefix, a queued File > Open can reach `OpenProject`, which calls `QDir::setCurrent(projectDir(P))`. After the pump, the synchronous Save path resolves active-operation state and proceeds to Cogging's relative `QFile::exists("MAN_COUPLE.DAT")` check. Therefore the lookup directory can differ from the directory established by the original project's Open.

What is not established: whether a queued Open is delivered during that Save pump, whether that Open is reentrant in the way the retained call path suggests at runtime, and which Project/active-operation identities the continuing Save uses after it returns from the pump. There is no qualifying no-GUI witness in the retained records. The distinction is material: event delivery could mutate CWD, and could also mutate the state used to choose the Save operation; the latter must not be assumed from the former.

## Evidence-qualified contract

- Save enters one `processEvents(flags=0)` poll at `0x140098129` (the containing Save entry is `0x1400980d0`). The existing `SaveEntryEventPrefix` is an adapter boundary: it requires an explicit due-event observation when a dispatcher is present, dispatches the supplied ordered IDs once, and does not infer an empty queue. Its test proves only that candidate adapter behavior.
- Static route: main-window slot `0x140025b60` calls File > Open dispatcher `0x14001ca30`; the `.moproj` route reaches `0x14008a9b0`, which sets CWD to the opened project's directory. This proves a reachable CWD-writing route, not delivery during Save.
- The resumed Save resolves active operation after event pumping, then the Cogging callback reaches `0x140762f30`'s relative existence check. The exact post-pump project/operation identity and native existence result remain unobserved.
- The retained source-qualified contract for `SaveEntryEventPrefix` does not contain a native Qt event queue, File > Open callback, process CWD, Project lifetime, or operation-selection semantics. Extending its candidate test into any such behavioral claim would exceed the evidence.

Primary bounded references: `analysis/resume156-cogging-cwd-event-next.md`, `analysis/resume164-cogging-next.md`, `source/apps/moproj2mst/include/m2m/engine/save_event_prefix.hpp`, `source/apps/moproj2mst/src/engine/save_event_prefix.cpp`, and `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`. Original decompile references and PE receipt are recorded in the CP156 note.

## Smallest resolving witness

The resolving experiment requires an original-process trace; static code and no-GUI candidate execution cannot establish the runtime event dispatch or state identity. In one fresh, controlled original Save run, capture:

1. Before entering and immediately after returning from `processEvents`: process CWD, current Project pointer/identity, and active-operation pointer/identity.
2. Every `QDir::setCurrent` call during the interval, including caller and target, plus whether the queued File > Open handler ran.
3. At the first post-pump active-operation resolution and at entry to `0x140052ec0`: Project and operation identities selected by Save.
4. At `0x140762f30`: CWD and the exact result of `QFile::exists("MAN_COUPLE.DAT")`.

To discriminate the cross-project path, queue Open of a second disposable project whose directory alone contains `MAN_COUPLE.DAT`. Keep original Save and opened-project identities separately labeled; record any Open/Save completion or failure effects. This capture must be authorized and run under the project's controlled-app workflow; it is not part of this read-only audit.

## Focused candidate test, if a native trace becomes available

Do not add a test asserting native behavior now. Once the trace supplies a qualified schedule and identity facts, the narrow candidate regression can inject a single ordered Open event into the existing Save-entry adapter, have that declared callback change a test-local CWD and project/operation token, and assert at the Cogging lookup seam both the resulting relative-lookup directory and the operation token selected by the continuation. The test must mark the event schedule and callback effects as trace-derived inputs, assert the native trace's observed values, and avoid claiming that the adapter reproduces Qt dispatch or Project lifetime. Without those observations, even that test would only demonstrate a made-up candidate scenario.

## Status and limits

Disposition: **static CWD route confirmed; queued delivery and post-pump operation identity unresolved; no-GUI witness unavailable**. Keep public Save bound to explicit supplied evidence and do not substitute `projectDir(p)` for the process-relative lookup. No source, test, build, GUI, live REA, or shared mutable state was changed or used in this audit.
