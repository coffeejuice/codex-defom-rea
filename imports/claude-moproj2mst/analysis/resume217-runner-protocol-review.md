# CP217 final runner protocol review

## Decision: PASS for one owned comparison

This is a release-readiness decision for one sequential control/negative-ID
comparison using the owned-process helper. It does not claim that the original
application accepts either project, that the cases are comparable successful
Opens, or that any observed directory change is causal. The final comparison
must retain the runner's own failure gates and report filesystem effects as
inconclusive unless Open comparability is separately established.

The reviewed runner SHA-256 is
`84370534e343d03dee21168a9da3f77d0cd2bbf180fddb3515f7c6e1b9c20467`; it
matched the requested value on two checks before execution. The helper SHA-256
is `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`, matching
the runner pin. The target executable hash, original project hash, 33-entry
source-manifest hash and absence of both `C:\PROBLEM\p217b` and
`C:\PROBLEM\p217n` were also checked against the production constants. The
literal original-launch guard remains `false`. No original process or GUI was
launched or controlled in this review.

Key source locations: launch guard and target/source pins, runner lines 10–20;
physical-path and manifest preflight, lines 29–44 and 80–109; exclusive clone
reservation and byte-delta checks, lines 112–143; created-process identity,
lines 150–158; production PID absence query, lines 210–214; per-case lifecycle
and final success predicate, lines 215–268; harmless enabled and failure
fixtures, lines 288–315. The corresponding helper's retained-handle create,
identity, resume, terminate, and close protocol is at lines 83–189 and its
watchdog/event/cleanup loop at lines 211–291.

## Validation performed

- `node --check` passed.
- `node ... --self-test` passed, including UTF-16 argument encoding, exact
  two-byte negative-ID mutation, exclusive receipt creation, invalid JSONL
  event rejection, and bounded close-wait behavior.
- `node ... --dry-run-harmless` passed with the production `queryPid` function,
  not an injected query. It launched only the harmless Python fixture. The
  enabled control and negative-ID protocol cases both completed, and the
  runner's race-created-root and post-cleanup inventory-failure fixtures were
  correctly rejected and retained.

The independently generated receipts are in
`analysis/cp217-harmless-uxdo42/`. Both enabled cases recorded the ordered
`created → resumed → terminate_command → cleanup` events; `previous_suspend_count`
was 1; observation was 124 ms and 123 ms; helper exit was 0; held-handle exit
was verified; the cleanup wait returned 0; both handles were closed; and there
were no stream or parse errors. The actual PowerShell query receipt for each
was `{"ok":true,"absent":true,"rows":[]}`. Baseline source, executable and
helper integrity passed. Each delta was empty, so the harness correctly
classified the harmless comparison as
`inconclusive_control_did_not_add_lubricants`.

The race fixture failed its exclusive root reservation with `EEXIST` and
verified the pre-existing destination stayed empty. The after-cleanup
inventory-failure fixture retained raw helper output and a result with
`complete:false` and an `ENOENT` inventory error. It did not let the pipeline
turn a missing after-manifest into a successful comparison.

## Protocol assessment

The runner validates the pinned executable and helper, source manifest and
project bytes, target paths and clone structure before launch. It creates each
clone root exclusively and checks that the control clone matches source bytes
and the negative variant differs only by the two specified minus-byte
insertions. Before each launch and after cleanup it rechecks source,
executable and helper integrity.

The helper creates the target suspended and reports identity from its retained
process handle: PID, primary thread ID, creation FILETIME, canonical image,
executable digest, working directory, argv, host and launcher PID. The runner
checks those fields before resuming, requires the prior suspend count to be 1,
and accepts only the explicit terminate-command endpoint after the configured
observation interval. Child/stdin/stdout/stderr errors are recorded; helper
completion is bounded; and the final success predicate requires helper exit,
held-handle exit verification, successful wait, closed handles, no cleanup
error, process absence, intact baselines and a complete after-manifest.

The production PID query now uses `Get-Process -Id` and recognizes only the
specific `NoProcessFoundForGivenId` error as absence; other errors remain
unknown/failing and a returned process remains present. A reused PID can cause
a conservative failure. The retained process handle is the identity-specific
proof of exit; the PID query is a supplementary absence snapshot.

## Scope limit

The dry run verifies runner orchestration, child identity, cleanup, the actual
host absence-query path, artifact capture and negative failure handling. It
does not exercise DEFORM or prove successful/comparable Open. The production
runner's result must therefore be treated as filesystem evidence only. The
interpretation `consistent_with_positive_id_path_but_not_causal_proof` does
not establish that the original accepted the negative-ID project or that the
positive IDs caused a directory to appear.
