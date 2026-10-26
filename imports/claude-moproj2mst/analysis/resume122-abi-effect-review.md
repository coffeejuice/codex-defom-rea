# CP122 ABI and ShapeOptim effect review

## ABI and extraction verification

The CP121 feature-anchor change preserves the frozen CP120 C export table. I compared `build/wave3-checkpoint120-verified/moproj2mst.dll` (SHA-256 `2722e81d828d8b9bdac87f83325c8e33ca4dd0042aace45bf1ebf52b871514b8`) with `build/candidate/bin/moproj2mst.dll` (SHA-256 `2025f28c022e963f291b4b68c16d21ec3a0b563a4239d038b064db36c301863c`) using DUMPBIN `/exports`. Both report six functions and six names, ordinal base 1, with identical names, ordinals, hints and RVAs:

| Ordinal | Hint | RVA | Export |
|---:|---:|---:|---|
| 1 | 0 | 0x3360 | `m2m_api_version` |
| 2 | 1 | 0x3370 | `m2m_convert_utf8` |
| 3 | 2 | 0x38D0 | `m2m_convert_w` |
| 4 | 3 | 0x3C50 | `m2m_set_codepage` |
| 5 | 4 | 0x3C80 | `m2m_set_deform_dir_w` |
| 6 | 5 | 0x3DF0 | `m2m_set_gui_unit` |

The CP121 build receipt binds the candidate to source tree SHA-256 `cbe2be05aa260e907e6a17d9785fc95dfbb1f44e75f3a1e926218150fea950ce`; its full test receipt reports 308 registered tests and exit code 0. The focused preflight included the anchor test, opaque-header test and `m2m_p11_xcheck`. The private extraction retains the same `Project::FeatureContext*` nested identities, int-backed enums and existing `shared_ptr` member; `project_owned_storage.cpp` includes the private definition before namespace scopes, so the textual p11 `m2m`→`m2m_xc` substitution also reaches the new header. The opaque test verifies incomplete Anchor/Report use and opaque shared ownership. These build/test and export receipts support the extraction boundary; they do not measure C++ object layout or prove external C++ source compatibility for callers that depended on definitions transiting through `model.hpp`.

## ShapeOptim mkpath effect

The retained original contract in `analysis/resume122-shapeoptim-mkpath-original.md` establishes that `loadData` calls `studyFolder` and `QDir::mkpath(path, true)` before the later ROI `.GEO` list guard. A failed mkpath displays a one-OK modal, ignores its result, and returns true; ShapeOptim continues after the dialog. This is distinct from successful directory creation and from a returned failure.

The candidate's immediate path is at `src/engine/shapeoptim_load.cpp:109-120`. For the enabled native-directory profile it consumes the queued mkdir, calls `nativeMkpathAbsolute`, and records `{path, succeeded}` before the later geometry guard. Thus a successful immediate attempt persists if that later guard throws. The failed attempt is also preserved in `FileEffects::directoryAttempts` before the helper throws `M2M_E_UNSUPPORTED` at lines 46-48. The current seam therefore preserves the fact and result of the failed filesystem attempt; it deliberately does **not** preserve native continuation past the modal. Treat this as a guarded boundary, not as a successful native failure-path match. A follow-on that needs continuation must represent the modal/event phase explicitly and preserve the recorded attempt; silently returning success would misstate the unrecovered dialog path.

The named-study route is still not integrated. `studyFolder` at `src/engine/shapeoptim_load.cpp:150-156` refuses when `sp->doe` exists because its actual study name is not modeled; when it is absent, the current candidate builds a path with an empty study component. The existing focused `test_shapeoptim_study_folder_effect.cpp` calls the private effect seam directly with a synthetic path. It verifies attempt recording, success persistence before a simulated later guard, and failed-attempt recording plus the modal refusal boundary. It does not establish the real DOE study-name owner, a full `loadData` path, or native dialog delivery.

**Actionable finding:** keep the named-study ShapeOptim route and public Open admission guarded until the exact study-name owner is bound. The immediate helper can preserve failed-attempt evidence, but cannot claim native continuation after failure; preserve its explicit modal stop unless a source-qualified modal/event bridge is added. No test should interpret `directoryAttempts.push_back({path, false})` as proof that the native `loadData` call returned and reached the later geometry guard.

## Limits

Read-only DUMPBIN and source/receipt inspection only. No source edits, build, tests, GUI interaction, original execution or mutable REA operation occurred in this review.

