# Responsive coordination

The user authorized recurring coordination and continuous workflow improvement.
This chat is the dispatcher and integration decision owner. Long implementation,
benchmarking, detailed audits and integration execution belong to bounded workers.
The main full-app implementation goal remains paused; scheduling does not resume it.

## Each coordination pass

1. Read the compact coordination-state.json. If disabled or explicitly paused, stop.
2. Use one wait_threads snapshot for A/B/C/Bottleneck, with recorded cursors and
   timeoutMs0. Inspect only newly completed/blocked work; read a focused artifact
   or latest turn if a decision needs it. Do not reread full histories or large evidence.
3. Record new cursors and statuses. Treat partial snapshots as partial: an omitted
   worker has unknown current status, not completion. Check that worker only if needed.
4. On completion, verify deliverable existence and scope, then assign independent
   ready work from the queue. Full scientific review may run separately. Never label
   static or unexecuted assertions as runtime validation.
5. Before sending, record task ID, worker and pending dispatch. After confirmation,
   record destination turn/receipt and mark sent. On uncertain outcomes inspect the
   destination before retrying. Never resend on every heartbeat.
   After dispatch, await a new turn beyond await_new_turn_after; an old completed
   turn must not be mistaken for completion of the newly assigned task. Record the
   new turn ID when observed and clear that guard.
6. Preserve one native REA owner; require confirmed release before transfer. If blocked,
   assign useful retained-evidence work instead of creating session contention.
7. Stop the pass after bounded dispatch and state updates. If the queue is exhausted,
   report once and pause/disable coordination rather than inventing indefinite work.

A one-minute scheduled check is a requested cadence, not a latency guarantee. Tool,
approval, scheduling and model delays remain possible. The machine and app must be
running for local work. Do not remove approval/sandbox boundaries for speed.

## Worker reports

At a natural checkpoint report task ID, status, exact artifact paths/hashes,
validation performed, remaining gaps, resource release and recommended next task.
Keep the completion summary short; detailed evidence belongs in files. Completion
and blocker reports to the coordinator are within the user's coordination request.
Do not send peer tasks directly; the coordinator avoids duplicate assignments.

Each completion may include at most three efficiency observations: operation,
elapsed measurement if actually available, output bytes/token counts if provided
by tools, repeat count and cause. Unknown measurements remain unknown. Do not
interrupt research or perform a separate benchmark just to fill this report.

## Continuous improvement

The Bottleneck chat reviews new evidence hourly. Skip unchanged input and skip
starting another cycle while its current pilot or independent review is pending.
Choose at most one bounded improvement each cycle, measure against a fair focused
baseline, validate evidence fidelity and measure setup/amortization costs. Token
savings require measured token usage; output bytes are only a proxy.

The coordinator queues independent review, then integrates only the reviewed
tooling files in a separate commit and sends exact revision, commands, scope and
limitations to workers at safe checkpoints. Workers acknowledge adoption; rollback
or discontinue an optimization if fidelity, task time or total effort regresses.
One source-bound known-record lookup may be cheaper without the slice helper.

The Bottleneck worker owns tools/evidence_*.py, tools/benchmark_evidence_*.py,
tests/test_evidence_*.py and bottleneck-specific analysis artifacts. The coordinator
alone edits coordination-state.json and this workflow. Workers keep their original
worktree scopes. No blanket staging, unsolicited push or production API changes.

Measure completion-to-next-assignment time, worker idle time where observable,
repeated tool calls, truncated outputs and available per-task token use. Compare
before/after equivalent tasks; do not optimize local command time at the expense
of larger context or more approval rounds.

No event-driven service is being installed in this phase. Reconsider it only if
scheduled coordination still leaves material measured idle time.
