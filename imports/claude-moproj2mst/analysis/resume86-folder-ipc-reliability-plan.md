# CP86 folder-pipeline IPC reliability plan

## Purpose

Propose the smallest diagnostics and recovery change for CP85's generic `Truncated folder IPC` failure without accepting partial/corrupt frames, changing folder results, or reducing fixture/test coverage. This is a plan only; no source or candidate change was made.

## Current producer/consumer contract

The coordinator writes `job.bin`, closes/checks it, rereads its exact bytes as the helper identity, and launches one worker. For each case it writes a `.ready.writing` message, closes/checks it, then renames it to `.ready`. The helper waits for the final path, opens it once with `std::ifstream`, and parses the length-framed schema, ordinal, operation, and exact EOF. It writes each `.done.writing` result, closes/checks, then renames it to `.done`. The coordinator waits for `.done`, opens it once, and verifies the schema, exact echoed job bytes, ordinal, terminal state, tally/output framing, and EOF. The job identity and strict frame checks make stale, unrelated, and malformed messages fail closed.

The publisher's close/check/rename sequence means the final mailbox path should only become visible after the complete temporary file has closed. `waitMailbox()` polls for existence but does not establish that a subsequent open succeeds. Both consumers construct an `ifstream` and parse without first checking `is_open()`. `getNumber()` and `getString()` then use the same `Truncated folder IPC`/`Truncated folder IPC field` messages for any failed read.

CP85's persisted failure mailbox narrows the fault: `job.bin` is complete and its exact bytes are echoed by valid `.done` frames 1–37; `.ready` 38 and 39 are complete valid messages when inspected after failure; no `.done` 38 exists. This is consistent with a one-off failure opening/reading ready 38, but cannot tell whether the stream failed to open or a read returned short at the time.

## Smallest robust change

Add one private mailbox-open helper used at both consumer sites (`.ready` in `folderPostWorker` and `.done` in the coordinator). After `waitMailbox()` observes the final path, attempt to open it for a short, bounded interval (for example, 100 ms total, sleeping a few milliseconds between open attempts). Retry **only while the stream has not opened and no bytes have been consumed**. On exhaustion, report `Cannot open folder IPC mailbox`, the path, and the attempt count. This distinguishes a transient or persistent Windows open failure from a short frame read.

Once an open succeeds, keep the existing length limits, schema, job identity, ordinal, operation, tally, and `requireEnd()` checks unchanged. Do not retry a failed read, truncated length/payload, oversized field, invalid state, identity mismatch, or unexpected tail. Report those as read/framing errors with the path and current byte offset (and expected/actual size where available), then fail the case. A mailbox that opens but is corrupt must never be converted into a successful result by retrying it or accepting a prefix.

This adds no work on the normal path beyond an `is_open()` check. Only a transient open failure takes the bounded delay; a persistent denial still fails with a clear diagnostic. Keep the temporary-name + close/check + rename publication sequence. Apply the same open diagnostic to both message directions so the next incident identifies the failing mailbox side.

## Evidence to retain on recurrence

On failure, preserve the unique helper directory and record the failed mailbox path, message direction, attempt count, native/open state if available, and worker exit code. Keep `.writing`, `.ready`, and `.done` names and sizes, plus their SHA-256 values; record whether the helper had parsed the job and the last completed ordinal. Keep the parent result receipt bound to the exact source and executable hashes, the full fixture snapshots, and full test inventory. Do not log or normalize away frame bytes and do not let a successful retry erase the prior failure.

## Validation plan after approval

Use a scratch candidate built from a frozen source snapshot. First run the current valid 98-case folder sweep and require exact output/tally and fixture equality. Then add a narrow deterministic seam in a private IPC opener to make only the first open attempt fail before opening the valid final `.ready` or `.done` path; verify a later open consumes and validates the complete same message. Add a permanent-open-failure case that exhausts the bound and fails with a distinct open diagnostic. Add malformed/truncated ready and done fixtures that open successfully but fail immediately without retry. Re-run the full folder case and suite, preserve complete before/after fixture hashes, source and all test-binary bindings, and compare all folder outputs/tallies to the existing accepted run. No test may be skipped and no IPC validation may be weakened.

This plan distinguishes transient open failure from bad framing while retaining the contract that only a complete, identity-matching message can contribute a result.
