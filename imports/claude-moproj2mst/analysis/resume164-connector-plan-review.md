# CP164 independent review: first root-owned QList binding

## Decision

**Accept the CP164 source assignment for one bounded normal-path increment**, conditional on enforcing the stated owner and once-only guards in code. The original bytes support binding the existing private allocation's `+0x18` field to the QtCore `QListData::shared_null` identity, recording one symbolic 32-bit reference delta, and stopping immediately before QtCore RVA `0x131198`. They do not establish the global's live absolute count, constructor completion, or a usable TaskInfo. No live REA, GUI, build, or test was run for this review.

## Original evidence and cut

The retained GUI TaskBase disassembly at `0x141403222..0x14140324d` passes null parent to the imported public QObject constructor before its TaskInfo call. The QtCore public QObject disassembly at `0x671313f1..0x6713141c` writes the task address point, allocates `0x88` private bytes, and calls private initializer `0x67131160` only on the displayed non-null branch. The private initializer then writes the initial private address point before the CP163 cut at `0x67131185`.

In `analysis/resume163-connector-qobject-67131160.asm`, `0x67131185` loads preferred VA `0x672f0a50`, `0x6713118c` writes it to private `+0x18`, and `0x67131190` performs `lock add dword ptr [0x672f0a50],1`. `0x67131198` begins the subsequent address-point change. The independently retained PE export map names `0x672f0a50` `?shared_null@QListData@@2UData@1@A`. The end-exclusive 19-byte window `0x67131185..0x67131198` has SHA-256 `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772` in `analysis/resume163-connector-prototype-bytes.json`. These are preferred-image addresses; the model must retain QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and RVAs, not host or preferred-image pointers. The disassembly bounds are unwind regions, not proof of every logical-function path. There is no same-call runtime count witness.

## Actual owner and source seam

`ConversionSession` constructs one `WidgetRuntimeState` and passes that same `shared_ptr` to `TaskFactoryRuntime` (`src/engine/conversion_session.cpp:164-166`). The factory retains a weak reference. Its `initializeNativePrefix` owns the reachable continuation: it retains one pending Report through `TaskAllocationStore`, creates the separately owned sparse private state, and currently refuses at `BeforeFirstSharedQList` (`src/doe/task_factory.cpp:99-139`). `WidgetRuntimeState` already holds session-scope QObject counter state but no QList shared-null identity (`include/m2m/engine/view_manager.hpp:487-589`). A canonical record there and a nonowning binding in `PendingTaskPrototype::SparseQObjectPrivate` are the right ownership placement. An inert `ReportQObjectSharedRefLedger` entry or the separate `QStringPathOwner` cannot replace the root record.

The implementation must test `widgets_.lock()` and the identity of its qualified root record before committing the event. A mere nonnull `WidgetRuntimeState` is insufficient if it belongs to another adapter scope; bind or verify the expected root identity through the factory/session pairing. If the root is absent or foreign, retain the CP163 frame and refuse without a `+0x18` binding, delta, or advanced cursor. On the admitted path, prepare any host-owned state before publication; store the private binding before the root delta; then advance a once-only frame phase to `BeforePrivateFinalAddressPoint`. Reentry with any requested key must encounter that phase and preserve both identities and the one delta. Do not infer native failure-unwind or destructor behavior from this adapter ordering.

An optional absolute count must remain unknown until an actual qualified observation. If a later observation reports the **current** count after this increment, reconcile its base as observed current count minus the already retained delta modulo `2^32`; if it reports the count immediately before this increment, use that value as the base. Record which temporal anchor was observed. PE initializer bytes and the ledger's caller-supplied `10 -> 11` example are not runtime witnesses.

## Minimal acceptance tests

1. With a native factory bound to the actual root, the first refusal retains the same pending Report and private identities, exposes the root-qualified symbol at private `+0x18`, records exactly one `uint32` delta, and stops before `0x131198`.
2. Repeat with the same and a different key. Check the pending-node pointer, private identity, root symbol identity, binding, phase, and delta after **each** refusal. The prototype count remains one and no task is returned.
3. With missing and deliberately foreign root ownership, check that no binding, delta, or cursor advancement is claimed. The inert ledger and isolated QString owner do not satisfy root admission.
4. Check the initial private address-point RVA `0x1a9a30`, absent backlinks and later container/thread/counter effects, false QObject completion, zero SessionTask count, and empty factory maps. A separate two-frame root-state unit test may verify common symbol identity and additive deltas; it must not imply that prototype two is reachable through this factory.

Run the focused `doe_task_factory` and new root-owner tests, then the normal source-stable checkpoint build and regression gate. This review itself establishes source readiness only.

## Evidence limits and receipts

The exact source hashes in `analysis/resume164-connector-next.md` matched the inspected files, including `task_factory.cpp` (`87cd85e2fd08ac2550f89db01ba9092bf6302bf4166597d6781debe7c1a06520`), `doe_task_runtime.hpp` (`d708e5a3e204ea574fa775184669bb357c250b2395dfa0127bd15253717ab936`), and `view_manager.hpp` (`0715b73412eaac42084d3830de6a667d4804777bc76c040137053ca9a4078eff`). The retained private initializer, byte-window, static-capture, and PE-export receipts also matched the plan's SHA-256 values. The source and PE analysis is static; constructor runtime counts, null-allocation behavior, full logical-function coverage, and original positive-path comparison remain unresolved.

This review changed only this analysis note. It did not edit source, tests, shared plan/state, build outputs, GUI state, or a live REA session.
