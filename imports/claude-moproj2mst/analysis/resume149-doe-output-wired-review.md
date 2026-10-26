# CP149 review — wired DOE Output Save prefix

## Result

The CP149 implementation follows the reviewed prefix order for a bounded candidate Save call:

1. Existing `saveCurrentOperation` dispatch preconditions (`opBase`, exact `ob->current`, nonnull operation project) remain first.
2. The DoeOutput branch verifies that the operation belongs to the supplied Project and that an active SubProject/owned Study is present. `attemptOutputRegionsMkdirPrefix` then enforces active Type2 identity, Study aliases/backlink, nonempty name, and immediate-directory profile before calling native mkdir. The attempt is retained in `FileEffects::directoryAttempts`.
3. A physically failed attempt stops at an explicit unsupported warning-modal-return guard. A successful attempt proceeds to `opModel(p, op)` for the same active operation.
4. A null `opModel` return follows native whole-callback early return. A nonnull model is not substituted with the later display ROI model; the branch stops before Regions QDir construction/enumeration and any cleanup.

This preserves the source order and records the directory side effect before any subsequent refusal. It does not claim successful public Save, Regions cleanup, Common_Mesh removal, geometry, `.DS` write, or later project-file Save. `saveProject` cannot reach its XML write after this deliberately partial operation branch returns/throws.

## Failure and guard honesty

The failed-mkdir branch intentionally differs from native continuation: native `0x14081afb0(..., true)` displays a one-button warning and returns true after dismissal, so its caller continues. The candidate cannot reproduce modal delivery/dismissal and therefore throws at a named warning-modal-return guard after retaining the physical failed attempt. That is an honest candidate stopping boundary, not a faithful final native outcome. This caveat must remain explicit in the test/report and must not be described as native abort behavior.

The successful-nonnull branch similarly stops at Regions enumeration. The helper makes no deletion attempt and the test sentinel remains. The successful-null path returns from the callback without cleanup/DS work; however the focused tests avoid manufacturing a null model, consistent with CP149's proposal not to inject an unreachable null profile. Existing real model and existing lazy producer paths are both exercised.

## Test review

`tests/doe/test_output_save_prefix.cpp` uses the real `engine::saveProject` dispatch, registered DoeOutput class/current pointer, active flow item, active Type2 SubProject and owned Study, and caller-owned SaveEffects. The four cases cover:

- Existing model: one successful mkdir, same engineModel pointer retained, cleanup guard reached, sentinel preserved, no project/XML journal or later effects.
- Failed mkdir: blocker is a regular file at exact Regions target; one failed attempt retained; no lazy engineModel was created; blocker survives; warning-modal guard reached.
- Lazy model: successful mkdir calls the registered existing `opModel` lazy producer for the same operation; candidate reaches cleanup guard; later ROI model marker remains zero.
- Owner/profile guards: broken Study backlink and non-immediate directory profile refuse before directory/model effects.

The assertions of `validationComplete` and no project-write attempt reflect the candidate flow; they do not establish native modal return or complete Save behavior. The `Scratch` object is declared before Project, so reverse destruction tears down Project/model/Study owners before removing its unique `M2M_TEST_SCRATCH` directory. Testkit `REQUIRE`/`CHECK` are used, and includes directly cover filesystem, file output, project/model/DOE, writer, path and Testkit APIs. No CMake edit is required under the existing `tests/*/test_*.cpp` configure-dependent glob. No source, Testkit or include defect was identified by static inspection.

## Receipt and remaining qualification

`analysis/resume149-doe-output-wired-next.md` was available for review. A separate implementation receipt was not present in `analysis/` at review time, so exact frozen hashes/preflight/build status are not independently verified here. This review did not run preflight or a build, per assignment.

Files inspected: `src/engine/save_current.cpp`, `src/engine/save_project.cpp`, `src/engine/loader.cpp`, `src/doe/output_regions_mkpath_prefix.cpp`, `tests/doe/test_output_save_prefix.cpp`, and `analysis/resume149-doe-output-wired-next.md`. Static source review only; no edits/build/CTest/GUI/REA operation occurred.
