# CP203 original-runner receipt repair review

## Scope and disposition

Converter-only static review of `admin/frida_original_save_probe.mjs`; the final
reviewed SHA-256 is recorded below. No process,
original application, helper, Frida session, REA operation, or GUI was launched.

**PASS for source preparation only.** The reviewed initial version had one
fail-closed issue, corrected in the follow-up revision described below. The
helper hash and the original-run guard are correct. The same-device absence
result is serialized with a boolean `present` and JSON-safe process details;
query errors set absence false and add a failure. The six hooks and coverage
predicates are unchanged, and `ORIGINAL_RUN_REVIEWED` remains `false`.

## Finding

In the first reviewed revision, `held-handle-helper-raw-receipts.json` was written
through `saveBestEffort` after `result.captureComplete` had already been
calculated. A failed write added a failure without changing the final exit
decision. This was fixed in the follow-up: the runner records the retention result
and recomputes `captureComplete` using both that result and the final failure
count before saving `summary.json` and choosing the exit code.

## Same-device serialization assessment

`sameDevicePresence` rejects non-array results, duplicate rows for the exact PID,
and target rows with missing or malformed PID/name/identifier fields. The receipt
records `deviceId`, `pid`, explicit boolean `present`, and either `process: null`
or `{pid,name,identifier}`. The caller treats rejected/failed enumeration as
unknown (`fridaAbsent = false`) and records the error. Successful empty
enumeration is distinguishable from failure because only the former writes
`same-device-absence.json` with `present: false`.

## Checks

- Reviewed the helper digest in the runner against current helper bytes: exact
  match, `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`.
- Confirmed the original execution guard remains false.
- Confirmed required raw helper receipt retention now gates the final completion
  flag and exit decision, and duplicate/malformed same-device target rows fail
  closed.
- Confirmed the six hook declarations and coverage checks are unchanged within
  the prepared source; no GUI input or target API calls were added by the receipt
  changes.
- No tests or execution were performed by this reviewer.

This review approves the prepared source only. It does not authorize a run; keep
the guard false until the separate launch authorization/review process is
complete.

## Re-review of final receipt schema revision

Re-reviewed exact runner SHA-256
`dd4f1d6e5462f567b5f07895a60956ae2d7333f372fe2c005916b5728d04d10c`.

The same-device receipt carries `schema: "frida-device-presence-v1"` and
`querySucceeded: true` only after enumeration and validation succeed; the error
path writes the same schema with `querySucceeded: false`. Target presence is an
explicit boolean with `process: null` or a validated PID/name/identifier object.
Duplicate matching PIDs and malformed matching target rows throw into the
fail-closed path. The raw-helper-receipt retention result is included in a final
completion recomputation using the post-write failure count, before summary and
exit-code selection. This resolves the prior HOLD finding.

The helper digest still exactly matches the pinned digest. The original run guard
remains false. The six hook declarations and coverage predicate remain unchanged
in this receipt repair. The worker reports `node --check` and focused
source-extracted mocks passed; this reviewer did not rerun them or launch any
process. Disposition is PASS for source preparation only, not authorization to
run the original.
