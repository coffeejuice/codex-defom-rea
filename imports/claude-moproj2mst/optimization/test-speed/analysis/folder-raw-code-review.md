# Raw folder receipt code review

The raw receipt implementation captures generated regular-file hashes before cleanup in both serial and pipeline modes, with complete comparator vectors and explicit run/case identities. Its publication protocol prevents a half-written case or unfinished capture from appearing complete. The successful-warning omission found in the initial review is corrected in the follow-up revision. No further blocking completion-marker defect was established statically.

Reviewed revision of `optimization/test-speed/source/apps/moproj2mst/tests/s/test_save_folder.cpp`: SHA-256 `A11455853BC9D7926DF4420B2515BC291C04224CD532509928F0D66C8BB6E8DE`. References below identify this revision, before any subsequent correction. Review was read-only; no build, test, fixture writes or application execution occurred. Only this report was written.

Follow-up reviewed revision: SHA-256 `614AD40D668BCB1150AD331251D13F261D23848DE9DD6E07D42694659A22FDE3`. The correction and completion-marker follow-up below identify this revision. No build/tests were run for the follow-up.

## Initial finding, corrected: successful Result messages were lost

`compareCase` writes the compared receipt with `M2M_OK` and `""` at line 532. This is not necessarily the converter's actual successful message: `src/core/convert.cpp:422` assigns `r.message = joinWarnings(ctx)` on its successful path. `runCase` preserves non-OK messages for refusals but discards the successful message when dispatching comparison. Pipeline requests carry profile data and the compare flag, not the successful Result message.

Consequently, serial and pipeline receipts can both contain an empty message even if their converter warnings differ. This prevents those receipts from establishing the design's exact raw status/message equivalence. Pass the actual successful message through the serial comparison and pipeline request, or publish authenticated coordinator metadata joined by ordinal. Keep human diagnostic behavior unchanged if the baseline did not print success warnings. This finding was sent to root immediately.

The follow-up source now forwards `r.code`/`r.message` to serial `compareCase` at line 625 and to `publishPost` at line 621. The request schema is `M2M_FOLDER_POST_READY_2`, encoding the exact message after the profile and status. The helper requires the schema, validates ordinal and status width, requires `M2M_OK` for comparison, reads the complete string and requires end-of-message before forwarding both fields to `compareCase` at lines 840–842. `writeRawCase` receives those actual values at line 534. This resolves the identified omission in both paths; no human diagnostic changes were needed. Noncomparison requests retain default metadata because copy/refusal receipts are already written by the coordinator.

## Per-case coverage

`runCase` adds a raw identity after eligibility checks and ordinal selection, before copying. Copy failures write `copy_failed` with `converter_called:false`, error text and any regular files remaining in the partial work tree. Refusals write `refused` with full status/message and generated/retained regular files before the synchronous removal attempt. Successful cases invoke `writeRawCase` from `compareCase` before cleanup.

In serial mode that comparison happens in the coordinator. In pipeline mode the manifest passes the raw root to the single post helper, which checks scratch containment, adopts pipeline mode, and writes the same case ordinal. Copy failures/refusals remain coordinator-written; compared cases are helper-written. Each ordinal therefore has one writer in the normal state machine. The coordinator records all expected identities and finishes the run only after `runPipeline` has collected every result, verified helper exit, and checked helper stderr. Sharded conversion explicitly refuses raw mode rather than creating incomplete mixed-mode receipts.

Eligible case count intentionally excludes entries that preflight ignores for absent `.moproj`/expected directories, notes for out-of-scope cases, and summary events. It includes copy failures and refusals. It is not the entire directory-discovery denominator. The manifest preserves case order, ordinal, expected path, work path, receipt filename and receipt SHA-256, plus coordinator PID, executable path/hash and mode. Per-case records add writer PID/role, terminal state, converter-called flag and work existence.

Generated inventory is independent of the comparator's exclusions: it walks every regular file and records exact relative spelling, size and SHA-256, including files the human comparator ignores. Comparator details preserve each matched/missing/extra decision stage, full difference vectors before/after older-window adjustment, masking outcome indicators and banner count. Thus the new raw records are substantially stronger than truncated human rows.

The receipt does not record input-root/selected-`.moproj` identity or expected-file hashes. Those require the frozen discovery manifest and fixture snapshots or a separate evidence record. Do not describe this receipt alone as a fully self-contained input/oracle freeze. File inventory covers regular-file content, not empty directory structure, reparse-point metadata, alternate streams, timestamps or other filesystem metadata.

## Publication and completion

The run root is newly created under scratch with coordinator PID, mode and a monotonic timestamp. Per-case writes reject preexisting `.writing`/`.json` paths, stream into an ordinal-specific `.writing` file, close/check the stream and rename only after complete serialization. A hash/enumeration/write exception leaves an incomplete artifact and aborts; it does not publish a complete case. Different writers use different ordinal files in the normal protocol.

`finishRawRun` rejects duplicate expected ordinals, unexpected files, nonregular entries, missing receipts and leftover `.writing` files. It hashes the exact case receipt bytes into an ordered complete manifest. That manifest is itself closed/checked and renamed from `complete.writing` to `complete.json`. An exception before completion leaves no complete run marker. This is publication atomicity through same-directory rename, not a claim of crash-durable storage or an atomic filesystem content snapshot.

Completion validates filename membership and hashes bytes; it does not parse each case JSON to cross-check embedded ordinal/name/mode/work/state or reconcile decisions against its inventory. A downstream verifier should validate those embedded identities against `complete.json` before accepting parity. The normal writers receive identities from the coordinator's manifest, so no concrete current misassociation was found in this pass.

The follow-up edge-case audit found no path publishing `complete.json` after a helper failure, malformed result, failed replay or raw-file hashing error: `runPipeline` must return before `finishRawRun` at line 1364. A failure after every case receipt exists but before that return still leaves no run-complete marker. An interruption during manifest serialization leaves only `complete.writing`; rename publishes the completed document. A second completion attempt on the same root rejects the already present extra marker rather than silently accepting it.

An empty expected-case set does produce a complete manifest with `eligible_cases:0`. This is complete capture of an empty eligible stream, not sufficient evaluation coverage. Consumers must require the frozen expected denominator and exact ordered case list. Likewise, the marker is published before the final ignored-error scratch removal attempt at line 1365. It establishes receipt capture completeness, not successful cleanup, absence of diagnostic differences, full CTest completion or launcher acceptance. These distinctions are consistent with the diagnostic nature of the underlying sweep and should remain explicit in the parity verifier.

## Hashing and scientific limits

SHA-256 is obtained through dynamically loaded system BCrypt functions, with API-status checks and cleanup on exceptions. The reader checks opening, EOF, observed byte count and file size before/after. This rejects truncated reads and observed size changes. It cannot detect every concurrent same-size write; the phrase “changing raw inventory file” in its error does not imply all races are detected. Expected and generated files must remain quiescent during capture.

Raw mode adds enumeration, hashing, provider loading and allocations before later conversions. In serial mode that work runs in the converter process; pipeline compared-case hashing runs in a helper. Raw mode therefore changes inter-call allocation/I/O history differently across modes. Repeated raw parity is necessary evidence, but it must be combined with uninstrumented ordered diagnostics and focused captures rather than assumed to prove identical uninstrumented execution.

Literal hashes of generated files containing PID-specific absolute work paths can differ across separate runs. A parity verifier must explicitly classify such bytes using the established relocation rules, preserving original relocation targets and raw hashes; it must not silently widen normalization to make new numeric/content differences disappear. This review did not run such a verifier or accept the optimization.
