# CP217 negative-ID candidate preflight test review

## Verdict: PASS

The repaired test now asserts successful candidate Open and exact copied-tree manifest equality. The updated focused run passes those assertions.

## Verified evidence

- Source SHA-256 matches the report: `f76331f301a8c158e20fde3b1e9a43cd21a6648fbc6b272e1601e2bd2bbfff6c`. Fixture project SHA-256 also matches: `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`.
- The test reads the source fixture, copies its enclosing directory recursively into its configured scratch output, edits the scratch `Gear_blank.moproj`, and compares the complete modified byte string with a separate expected byte string. The edits are exactly two inserted `-` bytes before direct operation IDs 1 and 2. `CHECK(variant.size() == original.size() + 2)` further confirms the size increase. It does not edit the fixture.
- `replaceDirectId` locates the intended operation by name, requires the ID to be within the corresponding `<Operation>`, and checks that the target ID does not occur again within that operation. The expected-byte construction independently inserts minus signs at the corresponding ID tokens, then a full byte comparison catches unintended edits.
- It calls the public `openProject(project, file, context)` directly, captures both its return value and `project.openedOk`, catches exceptions, and snapshots the complete recursive copied-tree manifest before and after. This is candidate Open only; no original application, GUI, hook, or Save is involved.
- `analysis/cp217-negative-id-build.json` records configure and focused-target build exit code 0, and the built executable hash matches the report (`648d68e2cf83bdd5745c03a5ad6d44efb8284dfd676a334090968672aecafe8b`). The checked-in `build/candidate/Testing/Temporary/LastTest.log` records the focused test as passed in 0.06 seconds and prints `candidate_acceptedProject=true candidate_openedOk=true tree_unchanged=true`, with no `tree_delta` rows. These receipts substantiate what happened in that run.

## Repair rerun and receipt

The source now contains `CHECK(acceptedProject)`, `CHECK(openedOk)`, and `CHECK(after == before)` after reporting the outcome, in addition to the input-byte checks. Thus rejection, failure to set `openedOk`, or any added/removed/changed file or directory fails the test. The source hash is `47b3a957e73b8b789a4e5059007d6bb5961b8a721a4f5b4b7b6b499b5b06bcc4`.

`analysis/cp217-negative-id-rerun3-build.json` records configure/build exit code 0. The focused `LastTest.log` records a pass in 0.06 seconds; output reports `candidate_acceptedProject=true candidate_openedOk=true tree_unchanged=true`, no exception, and no delta rows. Its source and fixture hashes match the report. The rebuilt executable's actual SHA-256 is `2351142e6acac7b4859e4d3748eb970444864645d611e729b4c73814b2e9547e`, matching the rerun3 build receipt. The preflight note still lists the earlier executable hash (`648d68e2...`) in its build section, so that artifact hash should be refreshed in the note; it does not change the verified result or this verdict.

The broader interpretation limits in the preflight note are appropriate: candidate acceptance does not establish original-application acceptance or the cause of any original directory creation, and diagram references still use the original operation IDs.
