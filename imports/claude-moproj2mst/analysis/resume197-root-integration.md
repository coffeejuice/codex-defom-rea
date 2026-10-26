# CP197 root integration

The Session-qualified known-null Report caller now models only the QtCore
`CALL` at RVA `0x131449`. It pushes return RVA `0x13144e` into the live
symbolic `[S]` slot, changes RSP from `S+8` to `S`, and stops at callee entry
RVA `0x2aca0` before its first instruction. The earlier private-initializer
return slot remains historical; distinct live-slot and call-generation state
prevents treating the two returns as concurrent. Synthetic standalone null
refinements stay at the caller boundary.

Independent source review found that the first generation-two occurrence
could self-authenticate against a mutable slot. The repair added a separate
factory-owned issuance anchor and an exact-error regression for coherent
CALL/slot replacement, copy, import and missing anchor. Re-review passed.
Focused factory, Session, Report, Save and ABI tests passed.

The source-stable complete suite reported 361 registered, 354 clean, six
logged gaps, one logged diagnostic-difference suite, zero
failed/skipped/incomplete and zero fixture changes. Historical folder
comparison remains 93 SAME/five DIFF, with no changed logged diagnostics from
CP196. Frozen validation is `analysis/wave3-checkpoint197-validation.json` and
archive is `build/wave3-checkpoint197-verified/`; source SHA-256 is
`c90622460bde430543513cb2e32675006274d3315a7d5d77faeac8ca238ea8f2`
over 3,554 files.

CP198 was selected and started: a reviewed ten-instruction callee/initializer
prefix batch ending before the TLS-index read. No TLS, helper result, return,
task completion or public Open–Save–Close equivalence is claimed. No GUI
control, original app or mutable REA database was used.
