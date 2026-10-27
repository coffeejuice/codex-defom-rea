# CP513–515 efficiency review

Three selected parser/model increments used immutable PE audits, independent
source reviews, grouped source-stable focused checks, one complete build and
one bound 387-test launcher each. Full-build steps measured 53.750, 51.234
and 50.547 seconds for CP513, CP514 and CP515. Bound launchers measured
21.984, 22.672 and 22.359 seconds. Focused subproject cases rose from 49
to 50 to 51, with zero failures. Historical Save remained 93 SAME/five DIFF.

The stable launcher timing supports continuing the current jobs=4 grouped
regression gate. Full-build time still dominates these small source edits;
the receipts do not establish a tenfold end-to-end speedup. PE audits and
read-only reviews ran alongside builds without competing source writers,
mutable REA sessions or user GUI control. A reviewer's preliminary CP513
shift-index concern was rechecked and retracted before acceptance.
Token usage remains unavailable in local receipts and cannot be inferred from
report size. Preserve the existing source/binary freeze and next-task release
order. Review again after three more checkpoints or one hour at a safe boundary.
