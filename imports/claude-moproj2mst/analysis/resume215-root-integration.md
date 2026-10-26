# CP215 integration and continuation

The CP215 implementation increment adds an independently checked borrowed
StudyIo identity to Type2 lazy InputTable parsing. `readLazyStudy` records the
exact I/O object used for byte retrieval; the contextual notifier branch
refuses a different caller I/O object after its committed constructor effects
and before notifier flag mutation. It does not create keys, receivers or
callback results. The source and lifetime review is
`analysis/resume215-doe-io-review.md`.

The focused test initially failed because its direct-source fixture asserted
one project I/O read even though that call bypasses lazy file I/O. The
assertion was corrected to zero reads; the MSVC-configured focused build and
CTest then passed. The complete bound suite ran at
`build/test-runs/1793021939798598700`: 362 registered, 355 clean, six
passing with logged gaps, one passing diagnostic difference, zero failures,
skips, incomplete outputs or fixture changes. Historical comparison remains
93 SAME/five DIFF. The build receipt is `analysis/cp215-doe-io-build.json`;
`analysis/wave3-checkpoint215-validation.json` and
`build/wave3-checkpoint215-verified/` retain the source/binary freeze. Source
fingerprint: `2419f67df80d105650b9725b18d03a4d8715e801f7ec3f444785f0ae5c8385f5`.

Separately, CP215 copied-PE reports map the original Project+0x55 read status
and final diagram/view binding route. The additional original-process
observer worker was stopped by automated cybersecurity filtering before a
qualified source freeze. The draft has all three release guards false; no
CP215 original child or GUI-control session ran. The fresh disposable
`C:\PROBLEM\p215a` clone remains unused. See
`analysis/resume215-observer-interruption.md`. Neither static mapping nor the
disabled draft supplies the missing runtime value or view-binding witness.

The successor is CP216 source-backed recovery and queue reconciliation.
Public active DOE continuation, Save and accepted Close remain guarded.
`python admin/check_continuation.py` still returns `CONTINUE` with 15
workstreams and eight completion gates.
