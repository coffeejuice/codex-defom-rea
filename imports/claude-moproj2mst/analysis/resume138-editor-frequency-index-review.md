# CP138 independent frequency index callback review

## Result

No source blocker found in the frozen private callback or its tests. The source preserves the native current-SC/stride read before selection branching; automatic value stores precede the requested enabled-state projection; custom choices do not consume the values pointer or table shape. Public Save and physical Qt delivery remain explicitly out of scope.

## Branch and ordering review

The retained contract identifies all six handlers, including row 0's emitted stride read before its `IMUL 0` branch. Candidate `frequencyIndexChanged` resolves the bound current SC, checks its identity, then reads and qualifies the stride before the custom/store split. It admits row 0 choices 0/1/2 and rows 1–5 choices 0/1, with the already-updated cached combo matching the delivered selection.

For custom choices, it sets only the private enabled-argument projection to true and returns before row/component/value-pointer access. For store branches, it requires the existing 6-row/ncomp6/non-null table, stores +0 or row 0's exact -1, then sets the requested enabled argument false. The cache change is caller-owned and refusal is not described as rolling it back. Text is neither parsed nor cleared; a later explicit `frequencyTextEdited` is separately required to commit custom text.

Initial Fill's enabled projection is derived from the same selected combo values and remains optional/unknown until a qualified fill. This models the native explicit setEnabled argument without inferring signal delivery or physical widget bits. Existing signed-int Fill profile remains intact.

## Test review

The nine cases cover initial enabled projection for 0/-1/custom values; automatic selection on all six rows and unchanged unrelated raw bits/text/owners; row 0 sentinel then custom then explicit text edit; all-row automatic/custom/text sequences; repeated same-index delivery; SC/stride guards for every selection including row 0's apparent zero-multiplier branch; table guards on store branches while custom remains independent of table storage; invalid cache/row/selection/binding; and unconstructed/partial/OPI boundaries. The disabled state is asserted as a requested private argument projection, not as an actual QWidget effect.

The receipt's native ordering/source qualification is consistent with the inspected implementation. It does not overstate callback dispatch or Save integration.

## Frozen hash check

The two exclusive worker hashes match `analysis/resume138-editor-frequency-index.md`:

- `src/engine/simcontrol300_owner.cpp`: `c5aa458d0112a9bfc1ad21558ca5d311e0e2633e8a2052a7a7dbb6b932416985`
- `tests/mdt/test_simcontrol300_frequency_index.cpp`: `ca0e8b800f6fabf7bfec1a4db52bc66f3a94972edd79feaa52d9db0436396faa`

Static review only; no build, test execution, GUI, original app or mutable REA access was performed.
