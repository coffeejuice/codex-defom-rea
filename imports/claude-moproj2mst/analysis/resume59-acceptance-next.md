# Checkpoint 59 acceptance gap inventory

## Current acceptance position

The 98-case Save comparison remains **93 historical comparator matches and 5
DIFFs**, with zero refusals in that particular sweep. This is a retained
candidate-versus-Oracle-B folder comparison, not 98 fresh original-session
comparisons and not raw-byte equality for all files. The five differences are
still the same cases reported by checkpoint57:

| Case | Recorded delta | Reconciliation |
|---|---|---|
| `A:RS_55_Multiple_Operations_Example1` | One file differs; no missing/extra files. `Operations/Task00002/SimCtrl.KEY` differs at two bytes: CG parameters are `0 0 0 0` in candidate vs `0 1 0 1` in the retained original. | Real historical state difference. The retained original source chain explains the mutation through the enabled CG radio toggle on the editor300 iteration fill. Whether fresh Open reaches that slot remains unobserved; do not make the historical row a fresh-session expected result. |
| `B:120_B_DOE` | One `.moproj` difference and 7 missing DOE side files. | The input is the pre-interaction project; expected Oracle-B is after DOE study creation and Save. This is not isolated Open–Save equivalence. |
| `B:121_B_DOE_VAR` | One `.moproj` difference and 8 missing paths, including DOE study files and `temp.KWD`. | The retained expected state is after creating the study and a variable. The exact `.moproj` byte delta is not retained. It must not be described as a byte-level defect or fixed by synthesizing absent UI-created state. |
| `B:122_B_DOE_GEO` | One `.moproj` difference and 10 missing DOE paths. | Also a post-interaction DOE state comparison, not a fresh equivalent-state Open–Save pair. Preserve the missing-path inventory and do not count it as a pure writer mismatch. |
| `B:98_B_DS3F` | One differing MST line, no missing/extra paths. | Raw bytes differ because the `KFREAD` line contains different absolute project roots. The retained candidate-only reproduction shows exact equality after substituting only the complete case-root path. Raw historical result remains DIFF; the path explanation does not prove fresh original parity. |

The overall result therefore has no blanket “five code defects” interpretation,
but it also cannot be upgraded to acceptance: A55 is unresolved, B98 is only
environment-explained, and the three DOE cases compare different interaction
states. The 98-case denominator itself should remain labeled historical and
diagnostic.

## Latest source/build and changed-path limits

Checkpoint57 is still the last verified integrated candidate. Its validation
records 181 registered tests (174 clean, 6 gap-qualified, 1 diagnostic
difference), 93/5 historical comparisons, and `objective_complete: false`.
The diagnostic folder sweep does not fail on differences/refusals, so its
green test status cannot replace the five-item inventory above.

Checkpoint58 did not produce a new validated freeze. The recorded full build
failed with exit 2 at `m2m_p08_emit_a.exe` after an unresolved
`m2m::SubProject::~SubProject()` link reference; the later build attempt was
cancelled/interrupted. Checkpoint59's registration audit confirms the two new
PPV targets are discovered, and `ctest -N` sees 186 `m2m` tests, but no
checkpoint58 CTest receipt exists. Registration and partial linking do not
establish that these tests ran. Keep all checkpoint58 source changes outside
the verified-candidate claim until compile/link and full regression succeed.

The separate `resume59-validation-audit.md` also records a hermeticity issue in
the new PPV file-reader test: its “missing file” case probes hard-coded
`C:/scratch/missing.PPV` after an overlay miss. That path is outside the test
scratch root and could read an unrelated file if present. Repair that path or
inject the byte source before using the test as a reliable gate. The root-only
PPV test is an in-memory `FileEffects` characterization over a read-only
Oracle-B input; it asserts KEY output/no PPV output for the source-qualified
local-empty native gate. It is neither physical file verification nor full
public PPV Save acceptance.

## Original GUI release gate

`python tools/gui_validation_status.py` currently reports `overall=blocked`,
`gui_control=release_unverified`, and `release_ready=false`. Its first blocker
is explicit: the documented Windows control surface has no release operation
paired with a verifiable release receipt/status. The retained current-package
assessment and subsequent package review likewise find no such API/status.
Therefore no new original GUI control session can begin under the standing
project instruction. Existing historical GUI captures and the fact that some
original runtime work was captured earlier do not establish that the current
control lease has been released. This audit did not acquire control or claim a
current control-state observation.

This blocks fresh original-first acceptance, including replaying B98 or A55
against the original. It does not block independent headless candidate work.

## Next independent comparison without app control

The next useful comparison that needs no original application or GUI control
is a **new candidate-only B98 raw-output run** from the last verified
checkpoint57 CLI, against the immutable retained `98_B_DS3F` Oracle-B output:

1. Copy the B98 `_before` project tree to a fresh short disposable directory
   under `C:\PROBLEM`; record input paths and a pre-run file/hash manifest.
2. Run the already frozen checkpoint57 candidate from the converter CLI with
   the same absolute project path as the previous B98 raw reproduction.
3. Preserve the raw generated MST and all project/side-file hashes before
   cleanup. Compare bytes first to the retained Oracle-B MST, then separately
   report the exact complete key-path substitution and require all remaining
   bytes to match.
4. Record candidate executable/source freeze hashes, argv/cwd, output hashes,
   and post-run input manifest. Do not normalize the raw artifact or modify the
   retained Oracle-B tree.

This independently refreshes the candidate half of the B98 path explanation
from the verified checkpoint57 artifact and can test repeatability without
building or controlling the original app. It still does **not** close the
`fresh_independent_comparison` gate: the expected original bytes remain a
historical capture. After the CP58 compile/test issue is repaired and a new
candidate is fully frozen, the same headless comparison may be rerun against
that freeze; do not use the currently failed CP58 build as its executable.

## Evidence boundary

Reviewed checkpoint57 validation/integration, `resume58-combined-full-build`,
`resume58-reader-test-fix-build`, checkpoint58/59 audits, the five historical
diff reports, and the current GUI release assessment/status script. No source,
CMake, build, fixture, GUI, or mutable REA state was changed by this audit.
