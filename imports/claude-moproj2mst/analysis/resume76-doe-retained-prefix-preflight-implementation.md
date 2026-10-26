# Checkpoint 76: retained-prefix admission preflight implementation

The isolated actual-owner B121 entry now checks its first matching row's existing pre-append admission conditions before publishing the Study-owned marker. Empty/unsupported header admission therefore leaves no retained State and does not poison a later fresh valid call. The valid-row allocation/append/name/selected-index/sample-invalidation order and every existing post-append guard are preserved. Public InputTable parsing and Type2 Save remain guarded.

Only `src/doe/variable_prefix.cpp`, `tests/doe/test_retained_scalar_prefix.cpp` and this report changed. The assigned prefix header was inspected but required no edit. Implementation began after root confirmed CP75 frozen. No build, executable test/CTest, shared Session/parser/CMake edit, GUI, original app or mutable REA operation occurred.

## Source change

Two file-local helpers reuse the same narrow existing scope predicate:

- `requireVariableHeaders(node,name,samples)` applies exactly the prior reconstruction checks: nonempty name, decimal-converted sample count>=1, varId attribute present and dimension attribute present. It does not claim native header validation or a native false-return rule.
- `requireRetainableFirstRow(doeVariables)` skips nonmatching child elements, checks exactly the first DoeVariable, then returns without examining later rows/subtrees. Absence refuses the unsupported empty-list finalization branch. A malformed first matching row cannot be skipped in favor of a later valid row.

The retained entry calls this after existing actual-owner/fresh-manager/root admission and before retainOnStudy. The original read path calls the shared header predicate at its original pre-allocation point as well. Those repeated bounded DOM reads create no I/O or native mutation. Once admitted, State remains attached before concrete allocation and borrowed alias publication, preserving ownership through every subsequent exception.

No varId or dimension numeric value, LinkTo, Keyword, operation list, nominal text or later field setter was moved into preflight. Present malformed/zero numeric attributes still reach their existing post-append guard. The existing source mutation sequence, pending SetOprHasDoe guard and retry refusal for genuinely interrupted prefixes remain. Detached-driver cleanup and data-only State lifetime are unchanged from CP75.

## Focused tests

The packet now contains eleven Testkit cases: the eight CP75 cases remain, with three additional cases:

1. A nine-input matrix covers empty list, comment/text/unknown-only list, missing/empty name, missing/zero/nonnumeric sample count, missing varId and missing dimension. Each verifies no State/model/sample/notifier/I/O mutation, then a fresh valid B121 read reaches the normal retained notification guard exactly once.
2. First matching row selection covers ignored unknown/comment nodes before a valid row; a later malformed row cannot preempt that valid prefix; and a malformed first row cannot be skipped to choose a later valid row. Fresh admission remains possible after the no-effect refusal.
3. Non-3D and nonnumeric dimension, nonempty LinkTo and unsupported keyword name still retain their earlier appended variable and reached partial state. Existing varId0 coverage continues to protect its post-append position.

Existing no-cycle/escaped-diagnostics tests, actual owner admission checks, exact pending state, retry refusal, detached alias cleanup and public reader/Save refusal checks remain in the same target. Tests assert the native record's index directly without a Model query that would reindex it first.

These are authored checks, not runtime results. Root should run `m2m_doe_retained_scalar_prefix` and regression `m2m_doe_variable_prefix` as a focused group, then its required complete validation/archive. Existing CMake discovery needs no edit.

## Static preflight and freeze

`git diff --check` on the assigned files had no diagnostics. Direct source scanning also checked new/changed file whitespace and every quoted include against local/actual target roots; no missing include or trailing whitespace was reported. Testkit main/macros/case count were inspected; no C assert or unsupported REQUIRE_EQ was introduced. Manual source order inspection confirms preflight ends before retainOnStudy and post-append checks remain after alias/name/selected/invalid mutations.

Paths below are relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
| --- | --- |
| src/doe/variable_prefix.cpp | 5A716F94257B54ECA63D6EF90EF65BB61ED278A44714867A65333DAD82A398A5 |
| tests/doe/test_retained_scalar_prefix.cpp | D1A4C7CFDDFD6311924097D0442BB4D985B00AC327E87F1D121BE0C8F84FE264 |
| src/doe/variable_prefix.hpp (unchanged) | DDC049B0E824F544AE16063937C24791C6EB0F6DB20493065BF64371DE917F45 |

Source is held unchanged after this receipt for root's grouped build/test/archive. The report's controlling design is `analysis/resume76-doe-retained-prefix-preflight.md`; original valid-row mutation order remains qualified by CP50/51 retained original PE evidence, not by a new runtime comparison.

## Remaining dependency

The isolated retained entry is still not called by public lazy InputTable parsing. Actual SetOprHasDoe population/backend delivery and post-message keyword/selector/text/reindex/root operation effects remain unresolved. No successful read, native cleanup behavior, temp.KWD write or public Type2 Save completion follows from the narrower no-effect admission correction.
