# Save-entry pump and nested warning: static order bound

## Order established by retained native evidence

The retained `analysis/wave3-boolean-timer-contract.json` binds this Save sequence for original hash `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:

1. Save `0x1400980d0` performs its open check, then calls Qt `processEvents(QFlags)` at `0x140098129` with flags `0`.
2. The IAT resolves to the non-time-limit overload, which calls the current thread's dispatcher once when present. The traced Win32 dispatcher route dispatches native messages and polls with zero timeout; the blocking wait path requires flag `4`, absent here. Thus this Save-entry point can dispatch already-due timer messages but does not wait for a not-yet-due 1000 ms diagram timer.
3. Only after that pump does Save run per-subproject validation, calling last-operation checker `0x1400b8fb0`; its normal current-operation Save call is later at `0x1400981e1`.
4. If a must-be-last operation is found before another chain item, `0x1400b8fb0` invokes the single-Yes `QMessageBox::warning` at `0x1400b94b3`. The warning enters a distinct `QDialog::exec` nested event loop, which repeatedly dispatches the current-thread dispatcher until exit. The source-derived result is Yes (`0x4000`); Save immediately returns it before current-operation Save or output writers.
5. Normal Close replaces its answer with nested Save's integer and maps exactly Yes to Cancel. On this warning branch, Close therefore retains the project and skips accepted-Close callbacks.

The Save-entry processEvents call cannot consume the warning's future Yes answer: warning construction and its modal `exec` occur later. It can, however, consume any diagram timers already due. The warning loop is its own later opportunity to consume remaining eligible events. The two intervals cannot be represented as one pump or one answer event.

## Static implementation boundary

The contracts support implementing these *control-flow facts* without GUI capture: an explicit Save-entry single-poll boundary before validation; a distinct modal boundary when the warning is raised; the Yes-only warning result; Save's early return; and the Close Yes-to-Cancel mapping. They also support modeling a diagram timer callback conditionally when an evidence-backed due timer is supplied, consuming it before callback and resolving the then-current receiver/model attachment.

They do not close the event schedule. No retained source or historical capture states which timers were due at Save entry, which were consumed there, or which remained due/arrived before the nested dialog returned. Modal default derivation does not establish a zero-event interval. Therefore no fixed timer list, empty schedule, timer expiry inference from 1000 ms, or no-callback assertion can be implemented as original behavior.

## Candidate adapter implications

Keep two named schedule inputs:

- `SaveEntryProcessEvents(0x140098129)`: one dispatcher poll, with explicit due IDs that are eligible at this point; it is nonblocking.
- `LastOperationWarningExec(0x1400b94b3)`: a separate ordered modal event schedule, ending with the Yes result.

An event consumed at Save entry is removed and cannot reappear in the warning schedule. If either interval lacks a qualified schedule, preserve the existing unsupported boundary. A static helper may test that dispatch calls occur before each interval's subsequent source-bound branch, but that unit test must not pretend a constructed schedule is an original observation.

## Smallest remaining evidence

The single unresolved input is the per-process event partition across these two intervals on a fresh original case reaching the warning. Once the app-control release gate clears, capture one owned disposable fixture, record timer registrations and ordered timer callbacks before and inside the warning modal interval, then record the Yes result, nested Save return, Close mapping, project attachment and writes. If no callback occurs in an interval, record evidence for an explicit empty set. The static code itself needs no further broadening for this bounded order; the capture supplies schedule data only.

## Qualification

Basis: `wave3-boolean-timer-contract.json`, `wave3-directory-failure-dialog-contract.json`, `wave3-normal-close-guard-contract.json`, and `wave3-close-save-return-contract.json`. These are static PE/Qt analyses and do not report original runtime GUI execution, timer readiness, or event order. No GUI, source, build, test, or mutable REA operation was performed.
