# CP105 DB-step Boolean raw test: static review

**Result: no blocking issue found.** The frozen test is a focused public
`WRITE_SAVE` comparison against retained original Save artifacts. It does not
overstate the evidence as fresh-original execution, accepted native Close,
modal parity, or full-folder parity.

## Evidence binding and assertions

The implementation receipt binds the test and all five expected outputs to
SHA-256 values. The independent coverage note traces the fixture to
`mkdbstep2.py`: a deliberately prepared Boolean3D / OprFlag0 / empty-Properties
profile built from a completed3D DB project. The retained `rs_log.tsv` records
the historical Save and a Close question answered No. The prior folder sweep
records SAME for this fixture, but neither artifact proves candidate modal or
native Close behavior. The test correctly asserts only one current public
`convert(..., M2M_FLAG_WRITE_SAVE, 1252)` result and five strict raw files.

The selected outputs, expected lengths, three absent-file preconditions, and
changed initial MST and SimCtrl checks match the evidence note. It compares
the complete MST and four side files without normalization or masks; it makes
no claim about all generated files. On a refusal or byte mismatch, it attempts
to preserve every golden and available actual output, missing markers, code,
message, and disposable project path before failing.

## Scratch and cleanup review

The test copies the full before fixture under a uniquely named direct child of
`C:/PROBLEM`, requires a previously absent leaf, checks the owned-parent and
prefix before recursive deletion, restores the working directory before
explicit cleanup, and checks all copied/generated paths against the 100-unit
bound. A denied write fails in place; there is no relocation or permission
broadening. The diagnostic path is derived through the testkit scratch helper,
and the test requires that diagnostic directory to be fresh before writing.

Minor diagnostic limitation: `read()` asserts that a file opens, but does not
check the stream state after consuming bytes. If an existing output cannot be
read, the test fails at that point before `retainDifference()` runs. This does
not weaken a passing byte comparison, but such an I/O failure may lack the
promised complete binary diagnostic bundle. Root may accept this as a
diagnostic-only caveat or add explicit read-error capture in a later test-only
repair; it is not an oracle/provenance blocker.

The receipt reports a static preflight only; it correctly leaves build/runtime
validation to root. No source, fixture, build, GUI, or original application was
changed or run during this review.
