# CP201 root integration

The frozen CP201 candidate adds inert first-instruction effect descriptions
for both successors of the unresolved QtCore TLS JNE. The fallthrough
alternative describes a potential 32-bit module-memory read into EAX/RAX;
the taken alternative describes a potential RBX restore from the exact
generation-3 stack spill. Neither instruction executes. The live boundary
remains RVA `0x2961b`, with no selected edge, new read, register write or
return consumption.

Independent static source review passed after authenticating the original
QtCore bytes and the CP200 archive. Focused task-factory, Session, Report
and ABI tests passed, including 181 task-factory cases. The source-stable
complete suite reported 361 registered, 354 clean, six logged gaps, one
logged difference, zero failed/skipped/incomplete output and zero fixture
changes. Historical folder comparison remains 93 SAME/five DIFF. Frozen
validation is `analysis/wave3-checkpoint201-validation.json`; archive is
`build/wave3-checkpoint201-verified/`, source SHA-256
`2e90ced71ce9e116259a45104df0ceffb2bdd8b94d2a0167f10c0b39636df1de`
over 3,554 files. Public Open–Save–Close remains incomplete.

An independent CP201 lane built a converter-owned Windows suspended-launch
helper for a possible passive original Save-route trace. Seven harmless
no-window child tests passed. Its source review found a watchdog termination
receipt defect, so no original process run is admitted until repair, re-review
and exact-owned-process cleanup validation. No GUI control, original launch
or mutable REA database was used.
