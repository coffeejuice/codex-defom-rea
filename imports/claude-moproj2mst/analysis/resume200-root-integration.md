# CP200 root integration

The frozen CP200 candidate appends an immutable, unresolved control-flow
record for the original QtCore JNE at RVA `0x2961b`. Its two guarded
successors are fallthrough `0x29621` when ZF is set and taken `0x296c0` when
ZF is clear. Both refer to the exact CP199 CMP and privately issued unknown
TLS-index read. The record is default-disabled and stops before either
instruction; it establishes no runtime index, selected edge or TLS effect.

Independent plan and source reviews passed. The initial focused run found one
test lifetime defect: allocation-scope release destroyed the diagnostic frame
before the test snapshot checked it. The test now checks only surviving state
on that route. The reviewer then required semantic equivalence across resumed
cuts and independently varied authority. Revised tests cover all eleven
callee splits, seven earlier cuts, twelve substitution/revocation/reuse cases,
and the ordinary Session driver. Final focused task-factory, Session, Report
and ABI checks passed, including 179 task-factory cases.

The source-stable complete suite reported 361 registered, 354 clean, six
logged gaps, one logged difference, no failed/skipped/incomplete output or
fixture changes. Historical folder comparison remains 93 SAME/five DIFF.
Frozen validation is `analysis/wave3-checkpoint200-validation.json`; source
archive is `build/wave3-checkpoint200-verified/`, source SHA-256
`c9aaa78c42c45ac35c232b0b1ea031c44e24090f0a23f8733a23e8c52574bf5f`
over 3,554 files. This checkpoint does not establish public Open–Save–Close.

CP201 started with two independent read-only successors: original TLS branch
edge mapping and feasibility of a passive owned-process Save-route trace.
The edge map finds no common executed prefix. The passive probe plan must
resolve working-directory and exact-PID cleanup limitations before execution.
GUI control remains gated by the absent supported release/status operation.
