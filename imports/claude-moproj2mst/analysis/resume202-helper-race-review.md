# CP202 helper race independent review

## Scope and evidence

Reviewed the exact-handle termination race handling and its bounded lifecycle tests. No original executable, GUI, or app-control input was used.

Reviewed file hashes:

- `admin/owned_suspended_process.py`: `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`
- `admin/tests/test_owned_suspended_process.py`: `6ba568dc8db2071ad3c13defc0db8e8d8bc203c4f59f4200835d94d469a7017f`
- `analysis/resume202-helper-race-preflight.md`: `33e6e9f04f9655c071b6684a0b9a0a1dfd0e570edc8b6e8b90ef94a9440ccaa9`

## Decision

**Approved for the stated helper-only scope.** The fix preserves fail-closed cleanup and the test set adequately covers the race branch without trying to manufacture a timing-sensitive race against a real process.

## Findings

`OwnedProcess` retains the `CreateProcessW` process handle from creation. On `TerminateProcess` failure it captures the Win32 error and performs a bounded wait on that same handle. Only a signaled handle is treated as evidence that natural exit won the race. Timeout or any other wait result raises; it does not fall back to PID lookup, enumeration, or termination. The ordinary bounded wait and `GetExitCodeProcess` check remain in place, and the receipt records both the failed request and verified exit. This preserves the cleanup fail-closed contract.

The fake-kernel regression deterministically covers the narrow ordering: initial wait times out, termination fails with access denied, the retained handle becomes signaled, and the complete receipt and call sequence are asserted. Existing Windows integration cases use only owned no-window Python children and cover suspended startup, resume, EOF, malformed input, deadline/watchdog, natural exit, hash mismatch, one-shot resume, and handle closure. Together these are appropriate harmless-child tests: the race decision is branch-tested deterministically while real child tests exercise surrounding ownership and cleanup behavior.

I found no required correction. The report correctly limits the claim: a simulated race plus lifecycle integration does not establish the historical target process's exit state or reproduce its exact timing.

## Verification qualification

The worker reports nine Windows tests passing. This review was static and did not rerun the suite; it makes no claim about a fresh test run or target runtime behavior.
