# CP131 root integration

Five disjoint private source cuts were integrated: exact one-row `0xA0`
SPRING2 extraction, prepared-value Type3 `NUM_CON`/`REGIONS` row appends,
suppressed DOE notifier callee return, conditional masked-invalid editor300
node count, and the post-query low-version geometry arrays-timer checkpoint.
The public Open/Save/Close guards for unqualified branches remain in place.

Focused builds and CTests passed for
`m2m.mdt.simcontrol300_spring_reader`,
`m2m.doe.type3_optimizer_scalar_prefix`,
`m2m.doe.refresh_notifier_prefix`,
`m2m.mdt.simcontrol300_meshing_selection`, and `m2m.mdt.viewer_box`.
The grouped full build is `analysis/resume131-final-build.json` with stable
source SHA-256 `49356dbe662cd5fa46f69d179d38e8dd8027eb5a6ca0cb747705bd93f10533ba`
over 3,472 source files. The full runner recorded 316 registered tests:
309 clean passes, six passed with logged gaps, one passed with a logged
difference, and none failed or skipped. Source and binary bindings were stable
and no fixtures changed. Historical comparison remains subject to freeze
reconciliation; CP130 had 93 SAME/five DIFF.

Source contracts are `analysis/resume131-spring-hex.md`,
`analysis/resume131-type3-prepared-rows.md`,
`analysis/resume131-doe-suppressed-prefix.md`,
`analysis/resume131-editor-nonfinite.md`, and
`analysis/resume131-geometry-arrays-timer.md`. Independent reviews of the
Spring, Type3, DOE and editor evidence are retained in their CP131 review
reports. The editor nonfinite branch is conditional on actual candidate MXCSR;
the original selected MXCSR has not been observed. No GUI control or original
application session was used.
