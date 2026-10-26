# CP164 RIO: remesh terminal status and the next bounded failure-order cut

## Decision

The original remesh terminal outcome remains **guarded**. Retained native code establishes the failure prefix and a conditional default-handler termination request, but no same-run original Save witness binds enabled remesh, the failed writer open, decoded handler slot, filter behavior, and observed child status. The candidate's isolated `0xC0000417` endpoint tests its own boundary and supervision; it does not establish the original handler outcome. Public in-process conversion therefore keeps its existing `M2M_E_UNSUPPORTED` refusal on enabled failed open.

The smallest source-ready CP164 cut is **one candidate-only conjunction regression** in `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp`. Add a second test beside the CP99 disabled-remesh case. It combines three already source-qualified effects in one real `convertDetailedWithServices` call: the project's scripted post-close false result, physical MST write-open denial, and physical enabled-remesh write-open denial. This is useful for checking that a future shared Save edit does not map either earlier failure before reaching remesh. It is not a new RIO behavioral implementation or a native-termination acceptance claim. If CP164 is limited to features that advance original parity, defer this test and retain the RIO guard; there is no smaller unimplemented source-closed terminal effect.

## Exact bounded test recipe

Use the existing copied `133_B_DBSTEP_BOOL3_before` fixture, `Rig`, delegating project output, and Testkit target. Before Open, seed the primary `LOCAL_REMESH.DAT` with `DONE\n` and fallback `LOC_DEN_CTRL.DAT` with `FILTER_DISTANCE 7\nDONE\n`, the pair used by the existing isolated worker to enable remesh. Hold a read-compatible, write-denying handle on the primary before the conversion and through inspection. Verify the lock actually denies write-open before asserting byte retention; a generic failed `fopen` does not prove no prior truncation. In the project `aboutToClose` callback, after actual Open and Save-current, write an MST sentinel and hold a separate read handle with `FILE_SHARE_READ` so the later MST write-open fails. The delegating device closes normally and reports scripted error 7 at its one post-close query.

Assert the actual project open → `aboutToClose` → close returned → error query order; `SaveCallState` project attempted/returned/false and MST attempted/returned/false; prior Boolean BWZ/INI effects; unchanged MST and remesh sentinels; and returned adapter `M2M_E_UNSUPPORTED` with the existing remesh-boundary message. Do not assert an original GUI Save return, original `errno`, original process exit, native Close, or that the candidate worker's fixed endpoint ran. The test exercises the ordinary public guard, not the opt-in child route. Preserve the existing disabled-remesh test and use the same source-stable focused/full validation and freeze gate as any C++ change.

This one-file cut is smaller than adding another isolated worker case. The worker already covers project-false→terminal and MST-false→terminal separately; a triple child case would also require changing its `ChildCase` enum and selection gate. The CP80 proposal correctly called the triple conjunction low priority. No production or shared protocol file needs to change for the ordinary-guard regression.

## Retained source and qualification

Original v11.0.2 `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained bounded PE byte hashes in `resume103-remesh-terminal-outcome-architecture.md` are `[1400abcc0,1400abcf5)` `5912b7bffd5aa69439a8336d5ae40085a25ceb42d81dac9e5a59e7eedde21ca8` for writer/open/first format, `[14146da31,14146da5a)` `1c4bdc5062fbc422738f4b0d4c5381d3794667dc063b553a6eefc5744f58b92b` for null-FILE/errno/dispatch, `[141470568,1414705ed)` `455022bff2e4016f1b9f4709d08488112c95e19ea191aecf60020fe41b82836c` for handler selection, and `[141470440,141470565)` `056a315910535d2876efecf1b5ff2533dcf7ffb15a514edaa6785bca9eb18041` for the default handler. Bounds are end-exclusive. These are static source facts, not a reached runtime trace.

On null `FILE*`, first `fprintf` writes module `errno=22` and calls the invalid-parameter dispatcher with null arguments. Startup initializes the encoded module slot to null; its value at a later Save is not witnessed. If it remains decoded-null, the default handler constructs the exception report, calls the top-level filter, and requests `TerminateProcess(GetCurrentProcess(), 0xC0000417)` on ordinary filter-return paths. Filter nonreturn, custom handler, termination API failure/return, and actual OS status remain separate outcomes. The current pure prelude and unwind models use supplied hypothetical inputs. Their tests do not execute native context capture or the original filter.

Candidate `convert.cpp` completes project/property/MST attempts before calling `saveRemeshAtProjectSave`; it maps remesh refusal before MST/project adapter failures. The writer checks `state.enabled`, uses `CoggingFile` to open the primary, and returns `UnsupportedSnapshot` plus `invalidParameterReached` on failed enabled open when no private terminal service is supplied. CP109's isolated worker case 19 proves candidate project-false→terminal and case 20 proves MST-false→terminal in different controlled children; both use a fixed candidate endpoint and no native handler prelude. The existing CP99 test proves project-false+MST-false+ignored disabled-remesh deletion, not enabled failed-open priority. The proposed test closes only that candidate conjunction.

## Missing original witness and safe resumption

When the GUI-control release gate is actually available, the decisive original experiment remains one fresh owned process on a disposable short-path B133-derived project. In one run bind original image/module, successful Open and remesh-enabled state, Save-time CWD and resolved primary, parent denial-handle lifetime, null `fopen` and first `fprintf`, decoded handler slot and selected branch, filter/debugger result, reached termination call and external child status, plus before/after raw files. Instrumentation must disclose debugger/filter perturbation. A status-only match or candidate child receipt cannot fill those fields. No GUI session or original-process trial is authorized by this report under the current app-control release restriction.

## Verified file identities for this read-only pass

| Converter-relative file | SHA-256 |
| --- | --- |
| `analysis/resume103-remesh-terminal-outcome-architecture.md` | `cc474b1ab6203d2196ae5e79010bb84a6785485125c60a3773c09ec683a53f24` |
| `analysis/resume151-remesh-handler-next.md` | `448a6e4effef3a6e7d00a92ec35208cd34fd7f05a06ae3bc8eac9fd6d993f9f5` |
| `analysis/resume109-worker-failure-order-review.md` | `3ee032d47549c0da1a2ec6671251135b71fba7492d91903c0c8f82325b1250ac` |
| `analysis/resume80-save-failure-order-next.md` | `2e8fa049fd32af7653f2ffcffe01238f84a0c040e53697e695589ab3a211d3a4` |
| `analysis/wave3-remesh-invalid-parameter-startup-contract.json` | `d53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b` |
| `analysis/wave3-remesh-save-writer-14146da0c.asm` | `0ad7a0b58073b4568b9cae8c8d25012f32ec1f6d694dea355cd269edea113baa` |
| `analysis/wave3-remesh-save-invalid-141470568.asm` | `9b4b102e32ad12f18bbdf33e208498d1c91dd96d007a08b1aa24d4fa11405cd7` |
| `analysis/wave3-remesh-default-invalid-141470440.asm` | `04970920ced02bcba5dbce7e464ce91cf631cda9c3636cb59c19a03e0e3946dc` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce` |
| `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp` | `a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228` |
| `source/apps/moproj2mst/tests/s/test_terminal_remesh_worker.cpp` | `5eb9a042204ec78c6e106acd643285c774e23271eb26224ee58eb1d20987b61e` |
| `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp` | `c45745562a68e98a61f08b0fb9e727c393738ef644fac5a6c375564095c11878` |

No source, test, build, shared plan/state, GUI/app control, original process, live REA, or mutable evidence was changed or run in this pass. The report itself is the only write.
