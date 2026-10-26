# CP136 private tokenized `NUM_CON` row projection

Extended the CP135 in-memory Type3 scalar sequence driver to consume positive `NUM_CON` row counts and append complete, already-tokenized rows in order. For the admitted row profile, it reads fields 1–4 as ASCII int, int, finite double, int; derives the classifier result from field 1; and appends through the existing prepared-row helper with a non-null empty QString value. A valid nonpositive count performs no row iterations and resumes top-level line dispatch. If input ends before the declared count, completed rows remain and the sequence ends normally. A later unsupported/surrogate row returns a candidate guard at that line without rolling back earlier scalar updates or row appends. `REGIONS` remains guarded.

The new focused test file covers a complete row followed by ENDDATA, a declared count longer than the available rows, persistence of an earlier scalar and row when a later row is outside the supported numeric profile, and continued refusal at REGIONS after an earlier append. The existing sequence test was updated to use REGIONS for its still-guarded row case, since NUM_CON now has a bounded supported profile. It verifies the empty string field, mapped row fields, and line-boundary outcomes.

## Evidence qualification and limits

The successful native append order and fields are recorded in `analysis/resume131-type3-next.md`; CP136’s immutable-PE inspection resolves the QString constructor pointer `0x1427b9f0f` to a leading NUL byte, so the parser-supplied QString content is empty. The helper still models only prepared row values. The new sequence branch does not open `.OPT`, emulate QFile/QTextStream, process the first-line header probe, construct a native optimizer owner, or alter public Type3/ShapeOptim/DOE guards.

The row profile deliberately admits only ASCII tokens that convert successfully through the candidate Qt numeric helpers, and finite doubles. Short rows, malformed or unsupported numeric forms, surrogate input, out-of-range row-vector result indices, and REGIONS stop with a candidate guard. Those guard results are not claims that the original parser rejects such input. Earlier native effects would persist; the candidate keeps earlier accepted effects for the same order. No native runtime differential is claimed.

## Static preflight

`git diff --check` and static whitespace/newline checks passed for the owned source and new test. No shared header or CMake change was needed: existing recursive source/test globs include both files. No build/test, GUI, original process, mutable REA database, or source outside the Type3 scalar sequence module was touched. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_sequence.hpp`: `4552C72444172220D46F0FC6C6604ED264F4F4A2726CB129D98D11674884A4E2`
- `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_sequence.cpp`: `B4305AFF7678D0F082ED7602C2DB6E9562D6969D53868934CE8977FD17D20E28`
- `source/apps/moproj2mst/tests/doe/test_type3_optimizer_scalar_sequence.cpp`: `326D862981679BE5FD9DFF1497F6349ECF4238AED600F631BA0C2AA92C2C40D1`
- `source/apps/moproj2mst/tests/doe/test_type3_optimizer_numcon_sequence.cpp`: `DB7D3DF210098B6863DA8021AC8B4A568F7A0D5240756DDDBD3310F81A7E0D63`

