# CP196 root integration

The exact retained Report constructor chain clears EDX immediately before its
public QObject constructor import call. A private Session-issued binding now
qualifies the first fresh pending Report's parent as null; standalone
diagnostic factories retain unknown or explicit refinement states. Existing
POP, TEST and JE transitions carry the Session token to the known-null stop at
QtCore RVA `0x131449`, before the helper call. No callee, TLS, parent-slot or
public task-completion effects were enabled.

Independent source review first found that a same-address replacement of the
private allocation could retain qualification. The repair adds a separate
allocation-occurrence witness that renews on copy/replacement, includes it in
the immutable binding and snapshots, and tests a deterministic same-storage
lifetime replacement. Re-review passed. Focused factory, Session, Report,
Save and ABI checks passed before and after repair as applicable.

The source-stable complete suite reported 361 registered, 354 clean, six
logged gaps, one logged diagnostic-difference suite, zero
failed/skipped/incomplete and zero fixture changes. Historical folder
comparison remains 93 SAME/five DIFF, with no changed logged diagnostics from
CP195. Frozen validation is `analysis/wave3-checkpoint196-validation.json` and
archive is `build/wave3-checkpoint196-verified/`; source SHA-256 is
`da68d34433002b1e9022a551e075a37d2da4398f3e095954c1d285469ed19de2`
over 3,554 files.

CP197 was selected and started after freeze: the known-null CALL instruction
alone, with distinct call-generation and live return-slot state. Its callee
body, TLS and return effects remain guarded. Separate CP196 audits confirmed
the L5 positive fixture, DOE public Save, Boolean timing, failure ordering,
accepted Close and the five historical differences still need qualified
original action/state witnesses. No GUI control, original app or mutable REA
database was used.
