# Efficiency review after checkpoints 92–94

The complete source-bound test launcher took 15.078 seconds for 256 tests at
CP92, 13.953 seconds for 257 tests at CP93, and 13.844 seconds for 258 tests
on the CP94 freeze candidate. All three runs used threaded fixture hashing,
recorded stable source/binary binding, and found no fixture changes. There
were no full-suite retries in this interval. These runs are not a controlled
speed experiment: the candidate and test inventory changed each time.

The grouped configure/focused/full build phases took 2.078/6.360/31.454
seconds at CP92, 2.157/6.281/32.312 at CP93, and 2.109/6.547/32.734
at CP94. New DOE record paths and focused tests made the full build the
largest measured integration cost. The no-op and test-only fast paths remain
available from earlier checkpoints; repeating them here would not resolve a
specific risk.

Using exclusive source lanes and one grouped build per source-stable candidate
avoided build races. Static PE audits selected small original-backed cuts,
while independent source review preserved guards and caught ordering issues
in prior checkpoints. The remaining objective-level bottleneck is evidence
for selected GUI state, accepted Close and active DOE Save, rather than the
15-second regression launcher. No 10× end-to-end reconstruction claim is
supported. Continue grouped builds, focused tests and a single full suite
after source review; keep raw historical differences and fixture hashes.
