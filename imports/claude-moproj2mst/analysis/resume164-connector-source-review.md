# CP164 connector source and test review

## Decision

**Block CP164 source acceptance pending a root-ownership admission guard and its focused negative test.** The normal-path implementation binds the pending private state to a root-owned QList symbol, adds one symbolic `uint32_t` reference, and stops before RVA `0x131198`. However, a factory can be constructed with an unrelated live `WidgetRuntimeState`; the current guard checks only the symbol's fixed module digest and RVA, so that foreign root is accepted and incremented. This violates the explicit admission contract in `resume164-connector-next.md` and the independent plan review.

This is a static source/test review. No build, tests, GUI, live REA, source, or shared plan/state changed.

## Exact findings

1. **Admission blocker: foreign root passes.** `src/doe/task_factory.cpp:135-148` locks `widgets_`, checks that its `qlistSharedNull()` has the fixed QtCore digest and RVA, then binds and increments it. Every newly constructed `WidgetRuntimeState` creates that same statically qualified record (`include/m2m/engine/view_manager.hpp:489-501, 582`). `TaskFactoryRuntime` accepts an arbitrary allocation store and widget runtime pair (`include/m2m/doe_task_factory.hpp:67-68`), and neither the store nor the pending frame retains an expected root identity. Thus `TaskFactoryRuntime(storeFromScopeA, widgetsFromScopeB)` commits a `private+0x18` binding and delta to B. The production `ConversionSession` currently passes its own shared root and store together (`src/engine/conversion_session.cpp:163-166`), but that call site does not enforce the invariant at the source seam. Bind an expected root/scope identity to the allocation owner or pending frame and compare it before the private write; refuse a mismatch with no binding, delta, or phase advance.

2. **Focused test gap: the required foreign-root case is absent.** `tests/mdt/test_doe_task_factory.cpp:298-339` covers a live root and two factories sharing one root. The existing no-runtime case at lines 265-296 covers absence, not a deliberately foreign runtime. Add the negative case above and check that the pending Report/private identities survive, `sharedNullBinding` remains empty, `sharedNullReferenceCommitted` is false, the root delta is unchanged, and the cursor stays `BeforeFirstSharedQList`. The positive test makes one different-key reentry at lines 316-323; the plan also asks for same-key reentry and a check after each refusal. This is a test-completeness gap rather than a separate demonstrated source defect.

## Accepted behavior within the admitted normal path

`WidgetRuntimeState` owns one stable `QListSharedNullState`; the private frame holds a `weak_ptr<const ...>` and therefore does not own a duplicate symbol or numeric count (`view_manager.hpp:489-501, 544-546, 582`; `doe_task_runtime.hpp:24-39`). `initializeNativePrefix` stores this binding before `commitQListSharedNullReference()` and then advances the frame phase. Reentry skips the `BeforeFirstSharedQList` branch, so the factory does not add a second reference. Unsigned `uint32_t` increment and projected-count subtraction/addition provide modulo-`2^32` arithmetic. The initial private address-point RVA remains `0x1a9a30`; the method throws at the pre-`0x131198` cut without backlinks, a completed QObject, TaskInfo, or factory-map entries (`task_factory.cpp:135-153`). The root's absolute count begins unknown; `observeQListSharedNullCount` stores only a caller-supplied current observation and its delta anchor (`view_manager.cpp:739-749`). These are source properties, not runtime validation.

The retained static source supports only this narrow original instruction window: `0x67131185` loads the preferred-image QList shared-null address, `0x6713118c` stores it at private `+0x18`, `0x67131190` performs the unconditional `lock add ... ,1`, and `0x67131198` begins the later private address-point change. The PE export map names the address `?shared_null@QListData@@2UData@1@A`. The qualified QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the end-exclusive byte window `[RVA 0x131185, 0x131198)` hashes to `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772`. No same-call absolute refcount witness or TaskInfo owner is established.

## SHA-256 receipts

All paths below are relative to this converter checkout. The six source receipts match `resume164-connector-implementation.md`.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f4a6d985a7f68864348087f7cdeb3dde1407dcde57da859b61df6246e99f47ba` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `f573396d9d3a7c86ec4817efecbc375e8fbb6f325ceca4d04b272bf8ed137735` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `b9da4de9994c89abffc5ba7d2565c77fff54d0b60aa1c7ff92c0f6167ab2ed3a` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `3accb5c22e9c4a3aa80613476931629957b8797075846b303db13c6199927321` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `88406917fe165126567c84b0f4af586c36940ff65d22ef89f1a4b150f4f6f562` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bbdc8e7d7fa18a51269c4c060d950d92c382` |
| `analysis/resume163-connector-prototype-bytes.json` | `b38ce1278e3ef032cb2f1a8e125c22c73357bc176dbc3e46f3a411e320f381c3` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/wave3-closure-next-close-splitter-locale-equality-body-static.json` | `a0683413a66fc6a14925aab8e5e9d8647610c8024ef2071de2f0fbb027533012` |

After repairing the admission guard and tests, run the focused factory/root-owner tests and the normal checkpoint build/regression gate. This review supplies no execution result.

## Re-review after root-admission repair

**Accept the repaired CP164 source and focused-test design for integration.** This verdict applies only to the five hashes below and supersedes the earlier source block. It is a static review; focused tests, checkpoint build/regression, and candidate freeze remain unverified here.

`ConversionSession` now explicitly associates its own `taskAllocationStore_` with its `widgetRuntime_` at the start of its constructor body (`src/engine/conversion_session.cpp:170-172`), before the factory is used. `TaskAllocationStore::associateWidgetRuntime` accepts the first live pairing only; `ownsWidgetRuntime` compares `shared_ptr` ownership identities and requires a live adapter scope (`src/doe/task_runtime.cpp:22-35`). Comparing control-block ownership avoids equating a different owner merely because a host address is reused. At the CP164 cursor, `initializeNativePrefix` requires that exact association before writing `private+0x18` or committing the root's `+1` (`src/doe/task_factory.cpp:135-148`). An unpaired, expired, or foreign root therefore retains the existing pending Report/private frame at `BeforeFirstSharedQList` with no binding or delta. The root association stays outside the factory constructor, so constructing a factory with a foreign widget cannot appoint it as the allocation store's root.

The positive focused test now checks the same pending/private identities, binding, phase, and unchanged delta after both same-key and different-key reentry (`tests/mdt/test_doe_task_factory.cpp:299-330`). The actual-Session test checks production pairing and one admitted increment (`:333-347`). The foreign-first test checks refusal before any association and again after association with a different root, preserving identities and zero deltas (`:349-395`). A separate test uses two actual `ConversionSession` roots and checks that the first Session's store rejects the second Session's widget on the first factory call (`:376-396`). The two-frame unit test still checks a shared symbol and additive symbolic deltas without claiming a second prototype is reachable through one native factory (`:398-413`). The added test uses a Testkit `TEST_CASE`, supported `CHECK`/`REQUIRE` macros, and a resolvable `m2m/engine/conversion_session.hpp` include; no C `assert` was introduced.

The accepted boundary remains one root-owned symbolic `uint32_t +1`, after a nonowning `private+0x18` binding, followed by `BeforePrivateFinalAddressPoint`. The private address point remains RVA `0x1a9a30`; the method refuses before RVA `0x131198`. No absolute reference count, later container reference, TaskInfo, or native failure/unwind behavior is claimed. The retained original binary and byte-window qualifications in the preceding review remain unchanged.

### Re-reviewed SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `44a2616e575e489052d0d6db850dfb9ddb0e8cc192576950a318504a7fed0c54` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5f8cb8cdb27189f06155524642e504b544736f6db24e3fe0dda749b6c16810bb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3caf344a07b946c396d5b3c4d645e3eec698eb779799973da67fa0ec4b1e41f0` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `e75573b6264abd07afc082472f1d532e9a0467ff262a458e15bc9242f3cd9d7d` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `bac15e6b20def8846dadba8c9653046272a0576166b009d992705baba0626f41` |

This re-review changed only this analysis note. No source, tests, build outputs, GUI, live REA session, or shared plan/state were edited.
