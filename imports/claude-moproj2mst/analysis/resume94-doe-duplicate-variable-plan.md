# CP94 duplicate-name rows update the reader's last Variable

The smallest next actual-reader advance is a duplicate-name header/Description row on the existing concrete mode0 owner. Native helper0x140919de0 returns the matched object without creating or invalidating anything, but reader0x14090ee20 ignores that return and applies subsequent headers/children to the **last list record**. This unusual distinction is completely visible in the retained original bodies. It must be preserved rather than replaced with a matched-record update or another appended duplicate.

This lane is read-only while CP93 validation/archive is pending. It changed only this report; no source, build, CTest, new native capture, GUI, process or mutable analysis database was used. Implementation awaits CP93 freeze and root's assignment.

## Exact evidence and normal native order

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP50's reader body is15345 bytes/code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`, bound by `resume50-doe-variable-body-pe-receipt.json`. This lane reread its compact instruction stream, SHA-256 `1816C5454A6D81C4BADD706592CA854D7FABB548D9B6DC2AEB38B286EC6C5D02`.

Complete helper0x140919de0..0x14091a07c was reread in `resume91-doe-variable-lifetime2-140919de0.asm`, SHA-256 `57A3E6AEF220FD096F80E58EE806AFE5066BBC644039A29F2B8975EC1AC60C06`, bound by the CP91 lifetime2 receipt. CP93 independently qualified exact same-name lookup0x14091d970, first-match name-index0x14091b5d0, pointer-list snapshot0x1400d7f20, at-index0x14091b950, next-number0x14091da70 and the installed manager vtable. No new Constraint/record analogy is used here.

For each matched DoeVariable the reader first reads name and numSamples, then at0x14090f0ac calls manager vt+f8(name,numSamples,false). The helper calls vt+130 at0x140919e50. If a name exists,0x140919e5b jumps directly to0x140919feb:

1. Copy name as the vt+180 name-index argument.
2. At0x14091a031 invoke name-index; at0x14091a045 fetch manager+38's pointer at that index.
3. Return that matched pointer through the common tail0x14091a052..0x14091a07c.

The entire new-object range0x140919e61..0x140919fe9 is bypassed. Thus a duplicate helper call performs **no new0x48 allocation/constructor/list append, name assignment, next-number query, optional number store, selected-index update, sample invalidation or mode2 design acquisition**. Its numSamples argument is ignored on this branch. Name lookup/copy/ref temporaries still exist natively; the value abstraction does not claim their allocation/ref timing.

Immediately after the helper call,0x14090f0b2 reloads manager,0x14090f0c5 reads count,0x14090f0cb subtracts1, and0x14090f0ef calls vt+1a0(count-1). It never preserves or reads the helper's returned object. The resulting last pointer becomes the target at local rsp+a8. Following optional varId setter0x14090f225, dimension setter0x14090f352, deferred LinkTo and direct children all use that last pointer. Header+Description admission keeps positive explicit ID, exact dimension3 and no link, so those known setters and ordered nonempty Description assignments are the complete implemented continuation. Normal list exhaustion reindexes in order, then the outer reader continues.

## Observable example

Starting from source-qualified rows:

| Input row | Name | Samples | ID | Description |
| --- | --- | --- | --- | --- |
| First | A | 3 | 7 | first |
| Second | B | 4 | 8 | second |
| Duplicate | A | 99 | 9 | replacement |

The final list has **two** records. A stays nameA/samples3/ID7/descriptionfirst. B stays nameB/samples4, but becomes ID9/descriptionreplacement. Selected index stays1 from B's creation. Both indexes become0/1 only when the entire variable traversal completes. Samples99 from the duplicate do not replace either record's count.

To distinguish the skipped helper effects, a separate same-owner read can set explicit sample-invalid flags false and a distinct selected value before processing the duplicate. The duplicate preserves those values. It also leaves existing sample table contents and object identities untouched. This is an explicit value-state fixture, not proof of a real GUI selecting that diagnostic state. A duplicate that matches the last record updates that same last record without allocation; it is a simpler special case, not justification to update a non-last match.

## Bounded production owner and failure contract

Keep CP93's concrete-owner/alias length/order/non-null identity checks before following pointers, mode0 bound, canonical positive samples/ID, exact dimension3, empty LinkTo, and exact direct Description-only/raw-child profile. Scalar/geometry/retained owners, foreign/null/reordered/mismatched aliases, unqualified headers and mixed Keyword/Geometry/other element profiles stay guarded. Do not admit a raw Var because its name matches.

After exact name existence is established on that coherent owner, select `headerVariableOwners.back().get()` for the reader continuation. This does not replace native name lookup with last-match lookup: the existence decision remains exact content equality against all owned names, while the **reader's separate last-object selection** determines the setter target. Require the nonempty owner implied by a real match; no new empty-list fallback. Keep unique-name creation unchanged. The current signed-int32 count bound may remain as a reconstruction admission limit for both branches; no need to synthesize untestable maximum-count behavior.

The duplicate path must not enter concrete allocation/owner push/alias push, assign name or numSamples, calculate a generated ID, reset selection/invalid flags or clear samples. It should share only the subsequent reader header/Description setters with the new-object path. An empty Description skips assignment, preserving the last record's earlier text. A later Description value allocation failure preserves any earlier reached ID/dimension/Description updates on that last record; no current-record or whole-document rollback is justified. Native Qt name/string/list reference/allocator failures remain outside the value seam and must not be simulated as host fault parity.

Normal completion reaches reindex; a later unqualified row or sibling preserves the reached setters. A mixed Description-then-Keyword row still fails at the existing pre-admission guard, not at native Keyword delivery; it remains a reconstruction bound, not a newly claimed native failure order. Save and output do not become supported because record import completes: nonempty-vars dimension reconciliation remains before XML device creation, followed by separate actual refresh/UI dependencies.

## Exact source/test write proposal after CP93 freeze

- `src/doe/variable_record.cpp`: replace duplicate refusal with existing-name branch targeting the last concrete record; share only post-create reader setters.
- `include/m2m/doe_variable_record.hpp`: profile comment only; preserve signature/type fields and concrete ownership.
- New `tests/doe/test_variable_duplicate.cpp`: focused duplicate behavior, state and actual owner/Save checks.
- `tests/doe/test_variable_record.cpp`: its second-row/retry guard fixture uses duplicate valid rows; change only those rows to an unqualified header/payload so its retained-prefix intent still holds.
- `tests/doe/test_variable_description.cpp`: its first partial-effect fixture uses two valid same-name Description rows; make the later row a mixed Keyword profile, preserving the intended first description/index-1 guard check.
- `tests/doe/test_variable_unique_append.cpp`: its explicit duplicate-guard case is obsolete; update it to the source-qualified normal A,B,A last-record result, retaining distinct-name/case/foreign tests.

No Study/reader/Session/activation/shared writer/CMake/C ABI edit is necessary. Root owns build/CTest. Expected new target `m2m_doe_variable_duplicate`, CTest `m2m.doe.variable_duplicate`, alongside existing record/description/unique-append/notifier-binding regressions.

Focused cases should cover A,A with one stable record and ignored samples; A,B,A changing only B's ID/Description; non-last duplicate retaining selected/invalid/sample state across a separate read; empty descriptions retaining last text; duplicate followed by an unqualified matched row retaining pre-reindex updates; exact/case-sensitive name equality; foreign owner refusal; and actual Session-owned lazy duplicate import continuing ordered sample effects before the same Save reconciliation guard. This is a real source-bound public-reader behavior advance without scalar/geometry/link/GUI or public Save completion claims.
