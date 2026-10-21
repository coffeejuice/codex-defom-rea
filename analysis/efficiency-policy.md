# Continuous recovery efficiency

One primary chat owns this work. The user requested recurring bottleneck audits
on 2026-10-21. Standing workers and their coordination schedule remain retired.
This policy changes workflow, not the full reconstruction objective.

## Cadence and scope

Run `python tools/recovery_status.py` at the start of recovery work. Review after
three recovery checkpoints or 60 minutes, at the next safe checkpoint. The count
uses changes to sources, fixtures or contracts; tooling-only commits do not count.
A separate hourly heartbeat checks the same gate. Avoid duplicate reviews by
checking `analysis/efficiency-state.json` first. Honor goal pauses; do not resume
production work, restart workers, or interrupt a live capture/frozen comparison.

Subsequent reviews should take at most five minutes. Inspect only new completed
turns, repeated failures and current blockers. Skip unchanged/non-actionable state.
Choose at most one new measured improvement per review; update its result later
if a fair benchmark is not yet possible. Do not turn efficiency work into another
large project. The initial baseline/setup is recorded separately.

## Measurements

Use `tools/audit_recovery_efficiency.py` on the current chat's local session file,
with a bounded tail and compact report. Keep only timing and usage metadata.
Record output reduction as bytes, not token savings. Separate cached input,
uncached input and generated tokens. Never equate unattributed time with model
reasoning, tool blocking, or an approval delay without evidence. UI item lifetimes
overlap: use interval unions rather than adding nested shell/REA durations.

Compare elapsed time per comparable recovery task, errors/retries, oversized
outputs and validation retained. A faster scalar handler does not prove a faster
scientific-table recovery. Keep a change only if it preserves fidelity and is
useful after setup costs; revert a regression through a normal reviewed patch.

## Adopted defaults

- Read current work through `tools/recovery_status.py`; avoid full plans/ledgers.
- For unknown evidence schemas, use `tools/evidence_body.py FILE --procedure ADDR`.
  It supports standalone, worker and bundle records. Check target identity and
  source qualifications; an absent selected record is not global absence.
- Prefer `tools/evidence_body.py --compact` for bounded body selections. This
  changes JSON whitespace only; identity, qualification pointers and all selected
  lines remain present. Validation records output bytes, not token/time savings.
- For one known field, direct JSON selection remains appropriate. Do not force a
  helper when the exact path is already known. Do not use rg on giant one-line
  pseudocode fields or print whole native session/capability objects.
- Use `tools/capture_keywords.py --compact`. Full PID/run records, transcripts,
  hashes and cleanup checks remain unchanged. Preserve complete REA Evidence on
  disk and print only its ID, operation status, paths and necessary selected text.
- Before the first capture of a changed probe, review its schema against the
  comparator's required fields and run an available JavaScript syntax check.
  Decode fixed-size NUL-terminated text explicitly; fixed-length readCString
  previously included heap padding. Do not add an extra original run routinely.
- Batch independent reads and hash/report preparation. Keep provider-dependent
  queries, mutations and verification ordered. Use one native REA owner.
- After read-only function queries, retain and hash every new complete inline
  Evidence record, keep the immutable baseline, and close REA without another
  cumulative snapshot. Request a snapshot when annotations or other required
  state are not retained separately. Record the release receipt and verify
  retention; `analysis/iteration-evidence-retention-validation.json` is the first
  measured check. Artifact-byte savings do not establish time or token savings.
- Resolve the concrete uncertainty, run the required comparison, inspect a concise
  diff, and proceed. Do not repeatedly speculate about already-passing unchanged
  code or regenerate the same report merely to reassure yourself.
- Run focused checks while editing and the required full regression once per
  completed implementation. Do not weaken tests, freezes, held-out chronology,
  source bindings or API compatibility for speed. Repeat only for a relevant new
  change, failure or unresolved concern.
- For a positive runtime claim, reuse the recovered fixture grammar and record
  the expected successful wrapper status and the intended nonzero effect before
  capture. Compare those expectations with the original, not just C++ with the
  original. Matching error states or zero outputs do not establish positive-path
  coverage. Keep failed fixtures as explicitly labelled error cases.

## Reporting and persistence

Record measured results and remaining blockers in `analysis/efficiency-state.json`
and a compact audit artifact. Report meaningful improvements, regressions and
required decisions; remain quiet on unchanged hourly checks. Do not claim a total
wall-time speedup from output-byte savings. Do not change models, permissions,
dependencies, target behavior or Git transport as a workflow shortcut.

The initial OpenAI Docs reference supports concise, reusable workflows and
source-bound logs, not a guaranteed performance improvement:
[Iterating Development Workflows with Codex](https://developers.openai.com/cookbook/examples/codex/iterating-development-workflows-with-codex).
