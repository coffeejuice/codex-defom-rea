# CP133 root integration and EOF supervisor repair

The private editor300 NSTART toggle, Type3 BMP whitespace tokenizer, exact
two-row SPRING2 `0xG\r\n` retry, and opt-in empty-map clear are integrated as
bounded source cuts. Public editor/DOE/Project/Close admission guards remain.
The Type3 all-whitespace focused test initially failed because its fixture
passed UTF-8 `U+3000` bytes through the Latin-1 `qt::L` helper; the fixture now
constructs that UTF-16 code unit explicitly. Production tokenizer code did
not change for this repair.

Two successive full parallel runs before the supervisor repair failed in
different `m2m.g0.cli_isolated_worker` early-exit cases. Both observed status
`0xe1050001` and invalid `v2 EOF without final outcome`: the supervisor
classified pipe EOF before Windows reported natural child exit and terminated
the still-live process. A bounded one-second grace now permits that natural
exit while preserving the sticky invalid protocol result and the original
absolute deadline/capture limits. Independent review found and the owner
repaired an exception path in grace capture inspection. Root then guarded
the post-inspection clock subtraction against a crossed deadline. The final
supervisor source SHA-256 is
`30256324d5feb6349e34bb646060ae2a82cf7b01a3a9f5aec33dee11ca4afbd0`;
the root clock repair is independently reviewed in
`analysis/resume133-cli-worker-grace-final-review.md`. The original worker
failure logs and receipts are retained, not normalized away.

The final grouped full build is `analysis/resume133-final3-build.json`,
source SHA-256 `9d3e1f52c5afcbe790880494b0a47eb24065959051991316e20b2d3d8115a1fd`
over 3,474 source files. Six focused CTest targets passed. The previously
flaky isolated-worker target passed 20 additional repetitions. Two separate
full parallel runs of the same source/binaries passed with 318 registered
tests: 311 clean, six with logged gaps, one with a logged difference, zero
failed/skipped, stable source/binaries and no fixture changes. The final
runner receipt is frozen from `analysis/test-relocation-results.json`; raw
launcher logs are `build/logs/resume133-final3-full.stdout.json` and
`build/logs/resume133-final3-full-confirm.stdout.json`. Historical comparison
remains 93 SAME/five DIFF pending freeze reconciliation.

Worker source contracts are `analysis/resume133-editor-nstart-toggle.md`,
`analysis/resume133-type3-tokenizer.md`,
`analysis/resume133-spring-hex-failure.md`,
`analysis/resume133-feature-empty-clear.md`, and
`analysis/resume133-cli-worker-eof-race.md`. The latter predates root's final
clock-boundary source edit and is superseded for that line by this receipt.
No GUI control or original application session was used.
