# CP88 independent review: DOE Constraint dispatch

## Result

The frozen implementation matches the retained native reader decision. The existing constraint-count clear occurs before row dispatch. Only a direct element tagged exactly Constraint reaches the retained unsupported boundary; nonmatching direct tags, case variants, raw nodes, and nested rows under unknown wrappers are ignored. No source correction is indicated. This review was read-only; no build or tests were run.

## Order and partial effects

- Empty/no-matching-row lists complete normally after clearing the count projection. They do not reindex variables; the outer variable-number pass still occurs only after normal completion of all top-level readers.
- The first exact direct Constraint throws after the count clear. Prior top-level mutations remain, while later siblings and the outer ID pass are not reached.
- A prior variable guard or malformed XML prevents reaching the Constraints branch and its clear. A later variable guard preserves the constraint clear and prior region-name list but prevents ID finalization.
- Repeated no-row Constraints lists clear at each encounter; the outer number pass remains once. An outer number-scan null fault happens after the constraint clear.
- No constraint record is parsed or created. The integer count remains a projection and is not a substitute for native record storage.

The focused cases cover those boundaries, including exact direct match after ignored siblings, nested/case/raw omissions, a no-row case with existing variables proving IDs can change without index reindexing, and owner-bound sample continuation. The active ConversionSession fixture checks the same ownedStudy alias, zeroed count, preserved region names, ordered LHS/FF/USR writes and current PAR read, with no Session notification attempts.

The same-owner Save fixture reaches immediate XML normal return and records the refresh boundary, then throws at the existing main-window+458 owner guard. The old Sub_Project DOM remains, no replayable project write occurs, and ignored constraint metadata is not promoted into output. This is a partial effect only; it does not establish completed Save or durable-file parity. Existing writer-false continuation and geometry/output guards were not changed.

## Evidence boundary

The original-body receipt identifies the exact Constraint tag literal and confirms count clear before direct-child traversal. Candidate filtering reproduces ordinary state/order through the parsed-DOM abstraction, not Qt DOM/container reference counts, allocator failures or deletion/fault timing. Count 7 in the test fixture is a synthetic projection value, not evidence of seven actual native records.

The next matched-row increment still requires concrete constraint record storage/ownership and the native manager append/type-field contract. Keep the current exact-row refusal until that owner is qualified.

