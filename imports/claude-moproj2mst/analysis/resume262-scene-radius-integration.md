# CP262 independent scene-radius integration qualification

The source implementation and focused tests satisfy the independently
PASS-reviewed immutable-original contract in `resume262-scene-radius-next-leaf.md`
and `resume262-scene-radius-next-leaf-review.md`. The final implementation
review is PASS: `resume262-scene-radius-implementation-review.md`. Earlier
focused failures were preserved and repaired before acceptance: a Windows
macro collision, then a named-slot test whose cached model box was still
large. Fresh per-slot stores and explicit scene/all-view prerequisites now
prevent a prior accepted origin from masking a missing radius contribution.

The final focused viewer target built on a stable source tree and passed all
65 internal cases. Root's complete source-stable build receipt is
`cp262-scene-radius-full-build.json`. The bound full launcher registered 369
tests and reported 362 clean passes, six passes with logged gaps, one
diagnostic pass with differences, and zero failed/skipped/incomplete outputs
or fixture changes. Historical Save comparison remains 93 SAME/five DIFF.

This reconstructs the source-qualified independent radius/all-view selection,
camera store predicate and two scene-box queries. Tests are constructed
candidate caller cases; naturally reached original GUI occurrences, hidden
3D unsupported builders, missing-settings logging, exceptional unwinds and
FPU/signaling-NaN behavior are not established. The next selected action is
a bounded disposable original CLI probe design to obtain a missing lifecycle
witness without GUI-control acquisition.
