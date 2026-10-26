# CP203 original-runner receipt repair

Prepared the converter-only runner at
`C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\admin\frida_original_save_probe.mjs`.
This is source preparation only. No original application, helper, harmless child,
Frida session, GUI session, or REA operation was launched.

The runner now pins `owned_suspended_process.py` to SHA-256
`8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d`, matching
the current helper bytes. Same-device absence receipts now record `present` as an
explicit boolean, `schema: "frida-device-presence-v1"`, and
`querySucceeded: true`; a matching process is a JSON-safe `{pid, name,
identifier}` object. Duplicate matching PIDs or malformed process identity
fields fail closed. Absent PIDs retain `present: false` and `process: null`.

Cleanup after-state observations are factored into a retryable best-effort path,
including when an unexpected cleanup exception occurs. Receipt-file write
failures are recorded without preventing the other after-state query. Complete
helper JSONL lines are also buffered and saved as
`held-handle-helper-raw-receipts.json`; an unterminated final line is retained in
that raw copy as well. Failure to retain this raw copy now forces
`captureComplete: false`; the retained-helper flag is applied after the write
attempt. The existing JSONL stream and capture manifest remain.

The execution gate remains `ORIGINAL_RUN_REVIEWED = false`; the six-hook list
and coverage checks were not changed. Focused checks passed: `node --check`,
mocked same-device serialization cases for present/absent PIDs plus duplicate
and malformed target-row rejection, a six-hook/gate assertion, and an
independent helper SHA-256 check. Three mocked completion cases verify that
failed raw-receipt persistence or any failure forces incomplete status. A
source-order assertion confirms the raw receipt persistence result gates final
capture completeness. Updated runner SHA-256:
`dd4f1d6e5462f567b5f07895a60956ae2d7333f372fe2c005916b5728d04d10c`.

The independent reviewer requested duplicate/malformed-row rejection, explicit
query/schema fields, and fail-closed capture status when the raw helper receipt
cannot be retained. The exact final runner hash above was re-reviewed and passed
for source preparation only. Review record:
`C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\resume203-original-runner-receipt-review.md`
(SHA-256 `c6d3c03ba2122677c6ae55fc092c9706126b3145e5783c1fbac68d80c6bde48a`).
The review does not authorize execution. Do not enable the execution gate or run
the original based on these syntax and mock checks alone.
