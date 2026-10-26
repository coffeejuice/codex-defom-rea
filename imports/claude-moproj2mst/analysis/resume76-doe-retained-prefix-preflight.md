# Checkpoint 76: retained prefix before-append preflight

CP75's isolated retained entry can publish an adapter marker before it discovers that no variable can enter the supported prefix. Empty/unknown-only DoeVariables or the existing malformed-header admission cases then leave started=true, variable=null and a retry refusal, despite no native variable append having occurred. This is explicitly diagnostic state, not a native field, but it is unnecessary for the bounded supported profile. The smallest correction is a retained-entry-only first-row admission check before retainOnStudy; valid-row mutation order and later partial retention must remain unchanged.

This is a read-only follow-up during CP75 validation. Only this report was written. No source/public reader, build, executable tests, GUI or mutable REA changed.

## Exact current sequence

In `source/apps/moproj2mst/src/doe/variable_prefix.cpp`:

- `readOwnedB121ScalarPrefix:205–207` calls requireFresh, attaches State via retainOnStudy, then calls read.
- `requireFresh:84–97` checks actual prefix binding, empty manager/projection/retained-state compatibility, mode0, no rows, and DoeVariables root. It does not check for a matching row.
- `read:103` sets State.started before iterating childElements. Unknown elements are skipped. No matching row reaches the empty-list guard at line136.
- For the first DoeVariable, lines106–112 read name/sample count and apply the existing scope checks (nonempty name, sample count>=1, presence of varId and dimension). Failure occurs before the concrete record allocation at113 and append at115, but the retained marker already exists.
- Once appended, name/selected index/sample invalidation occur at116–118. varId/dimension value checks at119–122 and LinkTo/child guards are later and must retain those earlier effects.

The marker is not a dangling alias or a cleanup failure. It safely owns no record and leaks no cycle. The caveat is admission semantics: a no-effect scope refusal becomes a retained attempted state that prevents a later valid isolated call. A direct output claiming a native partial variable for this state would be wrong; CP75's report already distinguishes State existence from append.

## Smallest bounded change after source freeze

Add a private/static helper used by the **owned entry only**, immediately after requireFresh and before retainOnStudy. Conceptual operation:

```text
find the first child ELEMENT whose exact tag is DoeVariable
if absent: refuse the unsupported empty-list/finalization profile
read that row's name and decimal numSamples
apply the existing pre-append scope checks only:
  nonempty name, sample count >= 1, varId present, dimension present
if refused: leave Study's retained owner absent
otherwise attach State and call the existing read path
```

The helper may reuse a tiny pure header-admission predicate with read, preserving error wording, instead of duplicating those four checks. No global parser admission flag, constructor, state reset or public retry API is needed. It is acceptable to leave read's original checks in place; repeated local attribute reads have no native side effect in this bounded immutable DOM input. Do not make a second I/O/read attempt to perform preflight.

Inspect exactly the first matching DoeVariable, not the first *valid* matching row and not all rows. Native walks/skips nonmatching nodes in document order. A malformed first matching row followed by a valid second row must refuse without selecting the second. A valid first row followed by later unknown/unsupported rows still enters the first native prefix and stops at its reached notification; the preflight must not scan/reject later rows early.

Do not preflight keyword subtrees, operation IDs, varId's numeric value, dimension's numeric value, LinkTo, Description, Geometry, nominal text or field setters. Those checks/effects happen after append in the existing sourced prefix. In particular:

- `varId="0"` or present malformed numeric varId must still allocate/append, assign fields and invalidate samples before its existing guard.
- Present non-3D/malformed dimension must preserve the same preceding state.
- Nonempty LinkTo must not be hoisted ahead of append.
- Missing/unsupported Keyword or invalid keyword-operation children must retain the already-appended variable. They need not publish a typed keyword or pending signal that was never reached.

The original native reader does not validate nonempty name or conversion success as the candidate does; varId/dimension are optional natively. Therefore this preflight is explicitly admission to the existing narrow reconstruction profile, not a newly discovered native validation/ordinary false-return rule. Keep M2M_E_UNSUPPORTED. Empty native lists have later finalization/reindex behavior outside this prefix; do not turn that into successful empty parsing here.

After preflight admission, preserve attach-before-allocation so allocation/append and all later guard effects retain their correct concrete owner. Moving attachment after append would create an avoidable unowned alias/unwind gap. Removing the owner on every caught exception would erase precisely the prefix state CP75 was meant to preserve.

## Focused tests

Extend only `tests/doe/test_retained_scalar_prefix.cpp` using the actual Session fixture and no filesystem operations:

1. Empty DoeVariables; comment/text/unknown-only child list; and first matching row with missing name, sample count0/missing/non-numeric, missing varId or missing dimension. Each refuses before owner publication and leaves vars, sample invalid sentinels/mode/tables, I/O and notifier counters unchanged.
2. After each such refusal, pass a valid exact B121 row. It must now reach the retained notification guard once, with one record/alias and its native selected/invalid mutations. This is a fresh admission after zero native mutation, not retrying an interrupted native prefix.
3. Unknown/comment nodes before one valid DoeVariable still admit that row. A malformed first matching row followed by a valid second row refuses without choosing the second. Do not scan a later malformed row to preempt a valid first row's prefix effects.
4. Preserve CP75's varId0 partial-state test. Add dimension2 and nonempty LinkTo cases: each must retain the appended record, index/invalid mutations, and its exact reached missing pending state. This proves no post-append guard was hoisted.
5. Preserve the successful-prefix pending-state, exact allocation retry refusal, no-cycle cleanup, escaped diagnostic observer, detached-driver cleanup and public nonempty reader/Save guard cases unchanged.

Suggested write set after root's freeze release: existing `src/doe/variable_prefix.cpp`, its header only if a private helper declaration is needed, and `tests/doe/test_retained_scalar_prefix.cpp`. No Study data layout, shared Session, public readInputTable/readLazyStudy, CMake or C ABI edit is needed. Root owns grouped build/test/archive.

## Evidence and receipt

Native valid-row order and reconstruction admission distinctions are retained in `analysis/resume50-doe-variable-body-pe.md`, especially manager create/append before header mutation and later child dispatch; original PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This follow-up requires no original capture or runtime experiment.

Inspected CP75 source hashes are unchanged during this review:

| File | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/src/doe/variable_prefix.cpp | 8E4F95D1E48561C81116363F5F0010C903FE31A94BB08285F590AB875E73F95F |
| source/apps/moproj2mst/tests/doe/test_retained_scalar_prefix.cpp | AAB790DF0AC00E32D7FCA24AFCB7A0771C09CAC2BAADDC18D761486664451693 |

No implementation is claimed in CP76. The current CP75 validation remains tied to its frozen source, and the proposed successor must not start editing until root releases that boundary.
