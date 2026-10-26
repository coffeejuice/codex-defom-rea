# Case 005 short-path correction (design only)

## Required outcome

Keep BILLET and every path presented to the original shorter than 100 characters and under `C:\PROBLEM`. Preserve one owned original process, a separate candidate copy/cwd, raw manifests, original-first chronology, and local evidence in the converter checkout. Do not use case004's long paths or silently claim an alias is equivalent.

## Feasibility finding

The current harness cannot directly produce this layout. `harness.py` fixes `ROOT` to its parent checkout and `STORE` to `ROOT/build/reference-harness`; `prepare`, `case_dir`, `copy_tree`, and `record` enforce containment below that store. The planned full project path under checkout is 151 characters. A mere `subst`/junction is not a fix: harness resolves paths and explicitly rejects links, and the app would see a changed path spelling. Do not bypass those guards.

Recommended correction: add an explicit, validated `--runtime-root` to the harness while leaving case/evidence storage under `STORE`. The flag chooses only two disposable runtime trees:

- `C:\PROBLEM\m2m005\original\project\BILLET.moproj` (48 characters)
- `C:\PROBLEM\m2m005\original\cwd` (30 characters)
- `C:\PROBLEM\m2m005\candidate\project\BILLET.moproj` (49 characters)
- `C:\PROBLEM\m2m005\candidate\cwd` (31 characters)

All are under `C:\PROBLEM`, and comfortably below 100. Keep frozen inputs, binary snapshots, observations, receipts and comparisons in the checkout's append-only `build/reference-harness/billet-training-005` directory. At runtime-root setup, copy the prepared `inputs/` fixture into each isolated project directory, create separate empty cwd directories, and write a before-manifest for each role into the case evidence store. Do not hardlink project files between roles. Keep the exact original path shown to File>Open in the observation.

The harness change must validate that runtime root equals or is a strict descendant of `C:\PROBLEM`, both role trees are distinct and link-free, and no runtime project/cwd overlaps source inputs, candidate binaries, or another case. Record absolute runtime paths in `case.json`; `record` must verify the observed cwd against that recorded role path and collect manifests from the external role tree while writing receipts only under `STORE`. `compare` then compares these external role snapshots through captured case-local manifests/snapshots. Preserve current freeze, chronology, digest, no-overwrite, and raw-vs-qualified rules. Add focused harness tests for valid short external roots, outside-root rejection, original/candidate overlap, symlink/reparse rejection, missing paths, cwd mismatch, raw manifest completeness, and case-local receipt containment. These are required before capture because the current harness's exact-cwd assertion assumes `<case>/sandboxes/<role>/cwd`.

This is the smallest clean design that keeps evidence under the takeover subtree and the actual test project under `C:\PROBLEM`. A separate copied harness with its own `ROOT` under `C:\PROBLEM` is a fallback only if runtime-root support cannot be implemented; it moves all case receipts/source snapshots outside the checkout and weakens the current one-store audit chain, so it requires an explicit storage decision and independent freeze checks.

## Case inputs and process isolation

Prepare case id `billet-training-005`, split `training`, from the immutable fixture `evidence/gui_mo/s1/corpus/06_BILLET`, project `BILLET.moproj`. Recheck fixture/input hashes from the copied `inputs.json`; case004 anchors are project SHA-256 `650b448be5e765c0eaf1e32ef70bf4f7280173b10c8767b023b12e11762a29c1` and `BILLET.MST` SHA-256 `62829836c3b9c8c1e710c066a8dea0d14872874546e46ba2580d19a4cf99e3de`. Bind original EXE `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, expected SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. For historical candidate reproduction, bind case004's frozen EXE `dea5f846e2117e02ab8917ddb08b2331feca10b644b73232502de270334fa89d`, DLL `facb8b34cc8e2d5f94cd41b0307c6f9f9f93788e786bbd14f8209c055ecaf980`, and `deformdb.dll` `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`; reuse the verified `analysis/wave2-geometry-v1-build.json` receipt only if its source/binary binding still passes. A newer candidate needs its own complete build receipt and a new freeze; do not build during capture.

Before acquiring any GUI control, retain the supported app-control release receipt/status required by the recovery workflow. Immediately before original start, recheck installed EXE hash, module identities, licensing/readiness, active process absence, settings/locale/code page and current session. Start one owned PID in session 1 only after confirming no existing original process or open project. Never attach to an uncertain or pre-existing PID. Use the short backslash path from the beginning. Do not clear persisted settings; record them as observed.

Observe Startup → selected File>Open → visible-open → stable post-open → Save → Close. Capture actual dialog text/default and select only the observed default. A rendered project title is `visible_open`, not proof all Open work completed. On crash/refusal/timeout, preserve the last phase and partial writes; stop sending input. Wait for the owned PID's writes to quiesce, verify its window/PID is absent after normal close or crash, then release app control through the supported operation and retain its status. Do not terminate user applications or treat a crash as release proof. Record original first; candidate may run only afterward in its separate runtime tree with `--write-save`.

## Receipt and decision

The case-local `original-observation.json` keeps schema `m2m-reference-harness-v1` and fields: role/outcome; session start/end, PID, argv, EXE hash, cwd, fresh-state/previous-project declaration, exit status, locale/codepage, module evidence, settings; ordered action/status/evidence references; dialog text/default/selection; artifacts and qualifications. Add the recorded runtime root and original/candidate before/after manifest IDs in case metadata, binding hashes for each manifest. Raw comparison must enumerate every project/cwd added, removed and changed file plus metadata deltas; outside-project write coverage and intermediate ordering stay explicit limitations. Candidate start must postdate original receipt.

A full comparison is eligible only when original Open, Save and Close are each positively observed and candidate is isolated, frozen, and recorded later. Otherwise keep an original failed-prefix receipt and label candidate output diagnostic-only; it cannot fill missing original stages.

**Current status:** design only. Requires harness/runtime-root implementation, focused tests, fresh preflight, and a verifiable app-control release status before any capture. No path, case, harness source, app state, or REA database was changed in this task.



