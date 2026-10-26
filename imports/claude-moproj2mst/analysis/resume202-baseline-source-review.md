# Resume 202 uninstrumented baseline source review

## Current runner review

**PASS for one guarded, owned-process baseline attempt.** Current runner SHA-256: `ae2e3b2ee74590221782d93f563d3958f1fd48e6873aa785ead0968a6e59adba`. Pinned helper SHA-256: `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`.

The CIM query now emits a strict JSON envelope with `ConvertTo-Json`. A successful zero-row query is the only absence case accepted. Any row for the exact PID is treated as present, without inferring PID reuse from a rounded CIM timestamp; query errors, malformed JSON, and incomplete row identity fail closed. This avoids the earlier false-absence risk.

The four self-test cases are statically present and cover an explicit zero-row envelope, a present-PID envelope, a query-error envelope, and rejection of legacy plain text. They exercise PowerShell JSON serialization and envelope parsing; they are mocks, not a live CIM integration check. I did not run them.

The previous gates remain intact: pinned executable and helper hashes; pinned source and disposable manifest hashes and roots; suspended/live state plus complete identity checks before resume; five-second observation with the 20-second helper watchdog; exact held-handle cleanup; retained raw/parsed failure evidence; and source, disposable, and executable before/after checks. The 5-second observation and same-desktop launch are accepted documented limitations for this discriminator. The plan now correctly distinguishes the observation window from the watchdog.

## Qualification of the retained original baseline result

The retained one-attempt run is `analysis/resume202-uninstrumented-baseline-2026-10-26T04-59-10-181Z/`. Its helper event stream records PID 37824 created suspended, resumed once, then naturally exited (`endpoint: target_exited`). The held process handle verified exit code `3221225477` (`0xC0000005`), `termination_attempts` is empty, and the helper reports both `exit_verified: true` and `handles_closed: true`. The run records the pinned original executable SHA-256, `instrumentation: false`, `frida: false`, `guiInput: false`, a five-second observation setting, and unchanged executable, disposable, and source snapshots.

Windows Application Error and WER evidence in `analysis/resume202-baseline-crash-evidence.md` associates PID 37824 with an access violation in `QtCore4_64.dll` at offset `0x32c0`, matching the reported instrumented PID 20712 fault location. This is evidence that the recorded QtCore fault occurred in a no-Frida startup under the retained conditions. It does not establish the root cause or prove instrumentation caused the matching instrumented failure.

**The retained runner attempt is still incomplete under its own acceptance gates.** Its `result.json` has `processAbsence: null` and an `absenceError` because the then-current parser received PowerShell’s plain text `ok absent` rather than JSON. The cleanup handle proves that the owned process exited, and the CIM-serialization-fix note reports a separate coordinator check of PID absence; neither retroactively replaces the failed absence field in this immutable run. Keep the crash observation as qualified evidence, but do not label the retained attempt a fully accepted baseline result. The new serializer fix is for a future authorized attempt and does not rerun or repair this old receipt.

## Review boundary

Static review only. No runner self-test was run and no new original-process run was made. The runner guard remains false. The prior retained run is described above as existing evidence; I did not launch it or interact with its GUI.
