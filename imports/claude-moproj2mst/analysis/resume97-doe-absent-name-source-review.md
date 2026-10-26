# CP97 absent-name implementation review

The frozen source implements the qualified absent-attribute branch while retaining the explicit empty-name guard. Static review found no blocker against the retained CP97 native evidence or CP94 duplicate-owner rule. I did not build or run tests.

## Findings

`appendBoundedHeaderVariable` records `hasName` separately from the attribute value. `row.attribute(L("name"))` therefore remains null when absent, and the guard rejects only `(hasName && name.isEmpty())`; an explicit `name=""` remains refused before any owner mutation. The assigned value on a fresh record is still `variable->name = name`, which copies the null QString value instead of synthesizing a label or skipping the field setter. The header comment states the same boundary.

Lookup scans all concrete owners using `owned->name == name`. On the source-qualified Qt content-equality behavior, a repeated absent name finds the prior null-name record. As in CP94, the returned match is not used as the setter target: the code uses `headerVariableOwners.back()`. Thus the test's unnamed, `Named`, unnamed sequence keeps two owners, leaves the first description/name intact, and applies the final ID/description to the last named record. A repeated absent-name row without an intervening owner reuses its only owner, leaves the original sample count unchanged, and applies later fields to it.

The six focused tests cover the key distinctions and surrounding contracts: null name and initialized defaults; repeated null-name rows; a prior null-name match with a different last owner; reached partial state before variable-list and sibling guards; explicit-empty/missing-or-invalid-sample/mixed-child/foreign-alias refusals; and actual Session-owned lazy sample reads followed by the existing Save reconciliation refusal with no XML writes or notifier attempts. The implementation does not change public Save, owner type/alias layout, or shared interfaces.

## Qualification limits

This is a bounded value-semantics implementation, not Qt refcount/allocator or fault-order parity. The retained plan explicitly keeps explicit empty names guarded as a reconstruction limit, not as a claim that native code rejects them. It also keeps missing `numSamples`, arbitrary Qt numeric conversion, mixed Keyword/Geometry payloads, and Save completion outside the admitted profile. The review is static; root owns build/test validation.

Evidence and source scope: `analysis/resume97-doe-absent-name-plan.md`; `source/apps/moproj2mst/src/doe/variable_record.cpp`; `source/apps/moproj2mst/include/m2m/doe_variable_record.hpp`; and `source/apps/moproj2mst/tests/doe/test_variable_absent_name.cpp`.
