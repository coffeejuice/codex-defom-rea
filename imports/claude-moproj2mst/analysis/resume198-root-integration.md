# CP198 root integration

The Session-qualified Report path now executes a reviewed ten-instruction
structural prefix across QtCore RVA ranges `0x2aca0..0x2acb6` and
`0x29600..0x29614`. It records eight exact stack writes, an active outer
return and nested initializer return with separate generations, and symbolic
current/historical SUB flags. It stops before the TLS-index CMP at `0x29614`.
No runtime TLS index, branch result, helper return or public task completion
is inferred.

Independent source review passed against the CP197 archive and qualified
instruction bytes. It noted that individual corruption tests do not cover
every nested-call tuple field separately; the corresponding validators were
inspected and no source defect was found. Focused factory, Session, Report,
Save and ABI tests passed.

The source-stable complete suite reported 361 registered, 354 clean, six
logged gaps, one logged diagnostic-difference suite, zero
failed/skipped/incomplete and zero fixture changes. Historical folder
comparison remains 93 SAME/five DIFF, with no changed logged diagnostics from
CP197. Frozen validation is `analysis/wave3-checkpoint198-validation.json` and
archive is `build/wave3-checkpoint198-verified/`; source SHA-256 is
`2ddf0b0ad3d2922f19e555fa9e75385ecebdefc3eff0653ec49274522163015e`
over 3,554 files.

CP199 was selected and started: one symbolic CMP read of the QtCore TLS-index
cell, privately tied to the Session root, stopping before JNE. PE initial
bytes and static TlsAlloc/TlsFree stores do not establish the live index.
No GUI control, original app or mutable REA database was used.
