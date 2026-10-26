# CP164 connector implementation: root-owned QList shared-null binding

## Implemented boundary

The native factory retains the same pending Report allocation and sparse private allocation from CP163. When its constructor-bound `WidgetRuntimeState` is live and exposes the qualified QtCore root symbol, the continuation stores a weak, nonowning binding from private `+0x18` to that root-owned `QListData::shared_null` record, commits one symbolic `uint32` reference delta, and advances the pending frame to `BeforePrivateFinalAddressPoint`. It then refuses before RVA `0x131198`.

The root record is owned by `WidgetRuntimeState`, qualified by QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and RVA `0x2f0a50`. Its absolute observed count remains absent until a caller supplies a qualified count observation; the observation retains the absolute count and the delta at which it was observed. The projected count uses modulo-`2^32` arithmetic. No PE initialized value or fixture count is treated as a runtime observation.

The private initial address-point RVA remains `0x1a9a30`. The continuation does not create backlinks, later container references, a TaskInfo, a completed QObject, or factory cache entries. Missing root ownership refuses at the CP163 cut without a binding, delta, or phase advance. Reentry after a successful binding sees the advanced phase and does not commit another increment.

Added focused test cases cover the admitted first event, same/different-key reentry, retained identities and phase, absent completion/cache effects, and two pending frames sharing one root symbol while accumulating two symbolic deltas. The existing no-runtime refusal tests retain the CP163 boundary expectation.

## Checks and limits

`git diff --check` passed. No build or tests were run, per the worker assignment. The complete focused factory/root-owner tests and checkpoint build/regression remain for root integration. No GUI or live REA session was used.

Static evidence retained by the CP164 plan/review: QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; byte window `[RVA 0x131185, RVA 0x131198)` SHA-256 `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772`; CP163 source/byte receipt `analysis/resume163-connector-prototype-bytes.json` SHA-256 `b38ce1278e3ef032cb2f1a8e125c22c73357bc176dbc3e46f3a411e320f381c3`.

## Source SHA-256 receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `f4a6d985a7f68864348087f7cdeb3dde1407dcde57da859b61df6246e99f47ba` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` (unchanged) | `f573396d9d3a7c86ec4817efecbc375e8fbb6f325ceca4d04b272bf8ed137735` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `b9da4de9994c89abffc5ba7d2565c77fff54d0b60aa1c7ff92c0f6167ab2ed3a` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `3accb5c22e9c4a3aa80613476931629957b8797075846b303db13c6199927321` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `88406917fe165126567c84b0f4af586c36940ff65d22ef89f1a4b150f4f6f562` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d1ac4321eef560232771f175c1d2bbdc8e7d7fa18a51269c4c060d950d92c382` |

## Release

CP164 source ownership is released for these six files. This is an implementation handoff, not a tested or frozen checkpoint. Shared plan/state, CMake, generated outputs, builds, full tests, GUI, and live REA were untouched by this worker.
