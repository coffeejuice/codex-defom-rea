# Converter takeover

This subtree is the MOPROJ-to-MST converter project only. The full DEF_GUI_MO GUI and the parent checkout's DMO reconstruction are separate work.

The user authorized importing related materials without modifying Claude's project or its evidence. All implementation, test scratch and analysis outputs for this takeover belong beneath this directory. Do not modify the original Claude checkout, its worktrees, external recovery stores, installed DEFORM programs or original test projects.

Start with TAKEOVER.md, analysis/takeover-state.json and admin/import-receipt.json. The scope discussion is complete and the user explicitly requested implementation. Follow IMPLEMENTATION_PLAN.md. The current host advertises twenty worker slots plus this orchestrator. Target twenty active workers when twenty independent ready tasks exist, with exclusive file ownership; never invent independence to fill slots. Do not edit another worker's assigned files or run competing builds/captures.

Before releasing a new C++ test, its owner checks that it has a Testkit entry
point, resolvable includes, supported assertion macros, and no C `assert`
whose checks vanish in Release. At integration, root runs changed targets as
one focused group before the complete source-stable launcher and freeze.
Resolve every new quoted include against the target's actual include
directories or the source file's directory, using `rg --files` when unsure.
Testkit provides `CHECK_EQ` and `REQUIRE`, but no `REQUIRE_EQ`; use the macros
defined in `source/port/testkit/include/testkit/check.hpp`. Keep missing-file
test probes beneath `M2M_TEST_SCRATCH`, since an overlay miss reaches the host
filesystem. This preflight addresses observed checkpoint58-59 compile and
fixture-isolation rework; it does not replace root's build and tests.

At a source-stable checkpoint, keep every source owner on hold through the
complete test run and `admin/freeze_checkpoint.py` source archive. The runner
binds source and executable hashes before/after CTest; the freeze gate must
match those hashes to the selected build artifacts. Release successor source
owners only after the archive and receipt exist. Checkpoint61 passed tests but
lost this boundary when source edits began before freeze; its run is recorded
as tested, never as a frozen candidate.

The user requires continuous multi-stage implementation without routine decisions or renewed authorization. Run `python admin/check_continuation.py` after resumption and at integration checkpoints. Select and START each checkpoint's successor in the same implementation turn. Reassign finished workers immediately. A locally blocked item releases its worker to ready work or the smallest investigation of that dependency. Do not voluntarily finish an implementation turn while authorized progress is possible. Before any objective-completion claim, `python admin/check_continuation.py --require-complete` must pass; that invariant check does not replace independent scientific acceptance.

Apply the approved model allocation in IMPLEMENTATION_PLAN.md: most bounded worker lanes use GPT-6 Luna Low; use GPT-6.1 Sol High for uncertain native behavior or architecture. The orchestrator uses its actual host-selected model and records it without claiming an in-place switch. Preserve exclusive ownership and escalate conflicting evidence instead of guessing.

Stop only for explicit user pause/cancellation, verified objective completion, or an external constraint preventing ALL remaining authorized progress. A passing build, report, worker batch, local blocker, failing test or original crash is not a stopping condition. Auxiliary messages affect their own activity, not the recovery objective. Answer status questions briefly and resume; incorporate corrections into dependent work. Never ignore an explicit stop or changed constraint, or expand authority to bypass a blocker. Persist a resume cursor before interruption/compaction and resume without renewed authorization; never claim background execution without an active executor.

The user released the session for controlled app testing. Use only owned fresh reference processes and disposable projects; leave user sessions untouched. Close owned test applications after capture and end app-control activity promptly. Escape or an app-control complaint ends that interaction promptly, not source recovery.

Read RESEARCH_CHARTER.md for the controlling research objective. The user selected full headless Open-to-Save scope and explicitly revoked the proposed rollback requirement. Recover original internal architecture, functions, decisions, state, interfaces and external results in modern C++. Derive failure behavior from original code and observations; do not introduce transactional saves, safer error propagation, bug fixes or other redesign into the faithful baseline without an explicit change of research scope. Claude's implementation and notes are hypotheses to audit against the original, not the behavioral oracle.

This is scientific research into REA/Ghidra plus Codex reconstruction capability. Benchmark fidelity against the original app/subsystem. Measure recovery/reconstruction wall time and token usage separately, and seek faster methods without weakening evidence, scope or validation. Record unavailable token measurements as unavailable; output-byte reduction is not measured token savings. Preserve baseline commits, tool versions, source hashes, inputs and held-out evaluation boundaries.

The confirmed lifecycle scope is one project: open A, save A, close A, compared against fresh original application state. Cross-project session effects are outside this stage. Preserve initialization, transitions and cleanup inside the selected boundary; qualify historical session-seeded oracles rather than treating them as fresh-project expectations. Retain the existing m2m C ABI as a thin compatibility adapter.

Relevant dialogs take the original fresh-session default answer. Alternative answers are excluded from this stage. Verify defaults from original evidence. Include DOE/study files produced by Save; separate study execution and simulation running remain outside the lifecycle boundary.

Editable source is source/. Its initial committed baseline is bb869387a85a89193f3fdd78b69442a6ea567866. Build the converter's independent CMake project, not the parent DMO application. Source still contains upstream absolute paths; audit and relocate these before executing build/test/oracle scripts. Use copied fixtures and local scratch.

Working snapshots are alternatives, not automatic overlays. Preserve unmerged changes and reconcile them against the selected baseline by source evidence. history/converter.bundle retains converter branches and their ancestry. Baseline and acquired files have SHA-256 manifests under admin/.

Ghidra/REA files are retained filesystem snapshots. Do not attach to old session endpoints or reuse stale locks. Use a fresh local analysis copy when work resumes. Source qualification and validation limits remain attached to every behavioral claim.

No GitHub operations are required for local recovery. Any later GitHub Git transport must use the configured SSH remote and C:/Windows/System32/OpenSSH/ssh.exe, never HTTPS transport.
