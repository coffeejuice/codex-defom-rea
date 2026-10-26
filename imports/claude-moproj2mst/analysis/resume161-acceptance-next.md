# CP161 acceptance reconciliation after CP160

## Decision

CP160 adds a source-bound SPRING2 parser increment, but the aggregate acceptance ledger did not move: the validation remains 360 registered, 353 clean, six suites with logged gaps, one suite with logged differences, zero failed/skipped/incomplete, and historical output 93 SAME / five DIFF. The receipt records no changed diagnostics and `objective_complete: false`. `admin/check_continuation.py` still returns `CONTINUE`, 15 remaining items, and all eight objective gates pending. This is candidate progress, not full acceptance.

The relevant freeze identity is source tree SHA-256 `043c9578138bcbd768e85b03ed29184550fc285a386aa2f48e956b53794c34c9` (3,551 files) from `analysis/resume160-spring-repair-build.json`; `analysis/wave3-checkpoint160-validation.json` binds the full suite to that build. The preceding CP155 tree was `af5f0b7ceb406844b1b073e143d62694ddc7ecad736582b5f60885ceecb77c38` (3,547 files). The CP160 integration summary and reviewed SPRING source/tests distinguish the new two-row sign/no-digit behavior from comma grouping and other guarded parser cases.

## What changed and what did not

CP158 repaired bounded SPRING cursor/status behavior, CP160 added the recognized ASCII sign/no-digit branch within the stated count-one/two C-locale non-comma profile, and CP159 added a strict empty `KwOperations` scalar prefix. The CP160 full run proves those candidate tests did not regress the aggregate suite. CP159's review explicitly says its tests call a private helper with a null source; they do not yet prove the actual Type2 lazy-reader/notifier bypass or A,B,A reader order. CP159 also created a path-safe B98 comparison capsule, but did not run the original or create files under `C:\PROBLEM`.

The six gap suites and one diagnostic suite remain in the same state. The CP160 receipt lists the same qualification set, has no changed diagnostics, and still labels the full Save-folder comparison diagnostic. In particular:

- `m2m.mdt.mdt_write`: optional imported PACK fixture/header check is still absent (`M2M_INSTALL_FIXTURE` unset); active-cogging refusal is expected behavior, not the gap.
- `m2m.mdt.save_l6`: `87_B_HTFF` still lacks Oracle-B expected output.
- `m2m.moproj.deflate`: external independent vectors are still absent.
- `m2m.moproj.props_l2`: five operation-class registrations still skip (B11_CM, T_CM_DB.x, T_CM_ONLY.x, T_GRP.x, T_GRP2.x).
- `m2m.p07.corpus`: corpus skips remain. The separate CP130 run covers two omitted Oracle-A rows only at P07 header/main/trailer prefix (2/0/0); three A-excluded cases and operation bodies are not covered by that run.
- `m2m.s.save_current`: five project-file comparisons still lack pre-edit snapshots.
- `m2m.s.save_folder`: remains a diagnostic comparison whose logged differences do not fail CTest.

No gap suite was cleared by the CP160 SPRING reader change. The complete six-suite list and limitations are retained in `analysis/resume156-acceptance-queue.md`; CP160's own full validation repeats the same qualification categories.

## The five historical DIFFs remain unchanged

The exact case list, file counts and no-adjustment status in `analysis/wave3-checkpoint160-validation.json` match the earlier 93/5 ledger:

| Case | Still established | CP160 effect |
|---|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One differing `Task00002/SimCtrl.KEY`, no missing/extra. Two bytes depend on the original process reaching the CG radio transition; the old capture does not bind that event/mode. | None. SPRING parsing does not witness the A55 UI transition. |
| `B:120_B_DOE` | One differing `.moproj`, seven missing files; before/after trees represent different DOE action states. | None. CP159 empty-operation prefix is a private partial state effect, not the post-action Save. |
| `B:121_B_DOE_VAR` | One differing `.moproj`, eight missing files; nonempty Type2 state/output is still not completed. | None. The empty childless-operation profile cannot establish the nonempty codec/data-owner path. |
| `B:122_B_DOE_GEO` | One differing `.moproj`, ten missing files; geometry/DOE outputs remain missing. | None. Geometry and DOE writer prefixes do not complete these outputs. |
| `B:98_B_DS3F` | One raw `Gear_blank.MST` difference, no missing/extra; bytes encode different absolute project roots. CP149 source trace says both writers derive `.KEY` from the opened project root. | No source fix indicated. CP159's capsule specifies a matched short root but expressly has not run it; raw parity remains unobserved. |

Keep all five as DIFF. B98's path difference is explained but not byte-identical; the DOE outputs are action-state mismatches rather than demonstrated Save bugs; A55 remains conditional and unexplained without a fresh process witness.

## Smallest useful GUI-independent action

The most direct next candidate acceptance increment is to close CP159's identified integration-test gap in the existing DOE reader test: drive `readLazyStudy` through a real `ConversionSession` with its model-manager notification owner, using an exact childless `KwOperations` row, and assert the bounded scalar state plus no notifier attempt/flag90/key change and no later sample/PAR reads or writes. The current private-helper test passes `nullptr` as source, so it cannot detect the contextual notifier regression. Keep the scalar diagnostic/guard and public nonempty Save refusal; test this as an actual private reader prefix, not a public successful conversion. `analysis/resume159-doe-source-review.md` gives the exact reachability and assertion list. It is headless, adds meaningful lifecycle-route evidence, and does not require an original GUI event.

If fresh original comparison is the required acceptance criterion, there is no qualified substitute: retained original automation uses GUI menu/dialog interaction, and CP159's B98 capsule is only a plan. The repository's GUI release gate still requires a supported control-release API plus a verifiable receipt/status; the capability audit remains `release_ready=false`. Do not acquire app control or claim fresh original parity until that gate is satisfied.

## Public behavior and gate boundary

CP160 records that `g0.abi` permits either `M2M_OK` or `M2M_E_UNSUPPORTED` for a real project. Private parser/reader/helper tests, positive prefixes and expected-refusal tests do not satisfy public Open→Save→Close behavior. Forming Save, native DOE completion, Boolean owner integration, remesh terminal outcome and actual Close admission/dispatch remain guarded or unobserved as described by the continuation queue. The eight gates are still `reachable_scope_accounted`, `architecture_verified`, `lifecycle_integrated`, `no_unexplained_discrepancies_or_refusals`, `fresh_independent_comparison`, `failure_and_close_verified`, `abi_and_regression_verified`, and `reproducible_delivery`.

## Evidence and identity

- `analysis/wave3-checkpoint160-validation.json` SHA-256 `397e11a317d88447408da7c872e857bbf2078cc33362b458b32d4d5d71b20cc5`.
- `analysis/resume160-root-integration.md` SHA-256 `08ba9370d69850be5d117bc1fe4a6cda839543bc00ec6e7c41700f3f618f11b6`.
- Prior queue `analysis/resume156-acceptance-queue.md` SHA-256 `4a0f798aa7551ee5b16cda9b61932cbfcc15f8e0f3d2ef1d607dd17b5af70aec`.
- B98 capsule `analysis/resume159-b98-capsule.md` SHA-256 `e962664f42c183c151041072d207a8ab4ed2f67b0a0356329dc6709256ded801`.
- CP160 source/build/test identities are recorded in `analysis/resume160-spring-repair-build.json`, `analysis/resume160-spring-repair-tests.json`, and the validation receipt. The CP160 sign branch is separately source-reviewed in `analysis/resume160-spring-sign-source-review.md` and `analysis/resume160-spring-sign-repair-review.md`.
- Continuation status was freshly read with `python admin/check_continuation.py`; it reports 15 remaining items and all eight gates pending, with RDOE now specifically requiring the nonempty SetOprHasDoe codec/data-owner witness and preserving the public Save guard.

This audit is read-only: no source, fixture, build, GUI, original executable, live REA or mutable database was changed or run. Report SHA-256 is recorded after writing.
