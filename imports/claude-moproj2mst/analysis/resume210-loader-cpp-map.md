# CP210: modeled C++ path versus original outer loader `0x14001ca30`

## Scope and evidence boundary

Read-only mapping from the CP209 headless-entry report, retained original `evidence/gui_mo/s1/s1-05/sub0_s101.c`, and current converter source. No code, shared plan, build, GUI, original process, or evidence was changed. This compares the inspected original v11.0.2 `DEF_GUI_MO_64.exe` loader with the converter's `M2M_FLAG_WRITE_SAVE` path; it does not claim the paths are behaviorally equivalent.

## Mapping

`0x14001ca30` is the outer application loader. CP209 establishes the CLI caller passes the selected path and actual main-window object, stores the returned pointer, and unconditionally passes that pointer to `0x14002a0e0` (`postOpen`). In the captured failure occurrence, the refresh callback is in the null-result arm. The retained s1 pseudocode describes a multi-stage loader with suffix admission followed by model/factory/owner prerequisites and subordinate load work; the subordinate project reader is `0x14008a9b0`. CP209 explicitly does not identify the exact `0x14001ca30` null-return condition.

The closest converter correspondence is `source/apps/moproj2mst/src/core/convert.cpp` `run()` (around lines 183–249):

1. For `M2M_FLAG_WRITE_SAVE`, it skips the legacy `checkReadable` preflight, resolves the selected path, and applies a suffix gate for `moproj`, `.DB`, or `.KEY` before constructing the session.
2. It derives the `.DB` path, calls `ConversionSession::prepareProjectModel`, initializes the candidate `Project`, and commits model construction.
3. It binds session services, builds the registry, and calls `openProject(p,file,ctx)`, which models the subordinate project-reader work (`0x14008a9b0`) rather than the outer `0x14001ca30` loader.
4. It rejects `!acceptedProject`; for legacy mode it also rejects `!p.openedOk`. In full-save mode, accepted suffix admission is allowed to continue even if XML reading left `openedOk` false, to preserve later Save decision behavior.
5. It commits candidate model selection, then calls `activateSubProject(p,0,ctx)` and enters Save.

The converter therefore has source-ready analogues for path admission, project-model preparation/construction, subordinate project parsing, selection and activation. `ConversionSession::commitProjectModelSelection()` verifies the constructed model identity is present in the manager and derives its list ordinal. The native outer loader's returned pointer and its `postOpen` consumer are not represented as explicit values or phases in `run()`.

## Exact known guards and current branches

- **Known native caller guard:** CLI main only enters the `-m open` arm and calls `0x14001ca30` when the converted `-f` filename is nonempty. After the loader returns, it calls `postOpen` with the returned value without a nonnull gate.
- **Known native `postOpen` branch:** `0x14002a0e0` tests the saved incoming project pointer. Nonnull follows the project path; null follows the path that directly dispatches `signalProjectChanged`, then `signalRefreshMainView`, then `signalDiagEditorFitAll`. This is not a general successful-open callback sequence.
- **Known converter suffix branch:** full-save mode admits suffixes `moproj`, `.DB`, `.KEY`; other suffixes throw parse error before session construction.
- **Known converter parser branch:** `openProject` can return false for unsupported suffix; after accepted suffix it parses/loads. Full-save mode requires accepted suffix but intentionally does not gate on `p.openedOk`; legacy mode does.
- **Known converter continuation:** after the acceptance check, session selection commit and activation of subproject 0 occur before Save. No postOpen-like synchronous notification dispatch is present in this path.

## Missing prerequisites and evidence gaps

1. **Outer loader stage-by-stage closure:** CP209 says the native loader may return null after model/factory/owner prerequisites or load refusal, but does not enumerate a complete, qualified set of the prerequisites and guards in `0x14001ca30`. Retained s1 pseudocode must be traced branch by branch and qualified against instructions before naming exact native conditions.
2. **Return-value boundary:** the observed null is derived from the captured `postOpen` null arm plus callback path, not a directly captured `RAX` at loader return. A same-occurrence `0x14001ca30` return observation would establish that boundary directly; CP209 labels it derived.
3. **State correspondence:** no evidence maps native loader's model/factory/owner objects or returned pointer to `ConversionSession`, `Project`, `ModelManager`, `Project::dataDb`, or `openedOk`. Do not infer that a successful `openProject` or selected candidate Project means the outer loader would return nonnull.
4. **Input-domain correspondence:** the candidate `M2M_FLAG_WRITE_SAVE` path is a converter lifecycle, while CP209's original CLI path is GUI startup Open on the actual main window. There is no established headless original Open-to-Save API equivalent. The exact original fixture/profile to candidate call arguments and process prerequisites have not been paired.
5. **PostOpen effect correspondence:** the converter does not model the null-arm synchronous notifications/callback set. The existing diagram lifecycle event model is a separate timer/model-update representation. However, CP209 proves the cited refresh call is conditional on a null outer-loader result, so its omission alone cannot explain differing loader outcomes.
6. **Failure semantics after subordinate reader:** the converter's full-save acceptance rule intentionally proceeds when `openedOk` is false; whether the outer native loader returns null on the corresponding native read outcome is unresolved. The candidate's continued Save path cannot answer that native guard question.

## Source-ready actions versus evidence-gated actions

**Source-ready, read-only next work:** annotate the outer native function's suffix checks, object construction/registration, owner establishment, subordinate reader call, success return, and each null return from the retained `s1-05` pseudocode and bounded instruction captures; link each branch to its source qualification and distinguish firm facts from decompiler interpretation. On the C++ side, build a small mapping table from those confirmed native stages to current `run()`/`ConversionSession`/`openProject` stages, marking unmatched stages without changing code.

**Evidence-gated:** do not add a native-loader success/null emulation guard, manufacture a `postOpen` event, equate native owner fields with session owners, or change the full-save `openedOk` policy yet. Those require (a) branch-complete, instruction-qualified loader conditions, (b) evidence of the relevant native object/state correspondence to the public converter profile, and (c) a paired original/candidate observation at loader return or the earliest divergent prerequisite. The native null return must not be normalized to successful conversion based only on accepted suffix/parser completion.

## Consulted files

- `analysis/resume209-headless-entry.md` (CP209; original executable identity/hash and static/runtime qualifications).
- `analysis/resume206-cpp-postopen-gap.md` (prior representation-gap audit).
- `evidence/gui_mo/s1/s1-05/sub0_s101.c` (`FUN_14001ca30` and related retained loader pseudocode).
- `source/apps/moproj2mst/src/core/convert.cpp` (`run`, full-save path gates and ordering).
- `source/apps/moproj2mst/src/engine/conversion_session.cpp` and `include/m2m/engine/conversion_session.hpp` (model construction/selection commits).
- `source/apps/moproj2mst/src/moproj/read.cpp` (`openProject`, subordinate reader analogue).
