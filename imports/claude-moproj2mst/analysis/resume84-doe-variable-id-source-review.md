# CP84 independent review: variable-ID finalization

## Review result

The implementation matches the retained source qualification for the outer variable-number finalization path. It preserves the ordering distinction between child-reader completion and the later outer scan, reproduces both null-pointer behaviors, and uses explicit 32-bit wrap semantics without signed-overflow undefined behavior. Public Save/output guards remain in place. This is a static source/test review; no build or test execution was performed.

Reviewed files:

- `source/apps/moproj2mst/src/doe/doe_study.cpp`
- `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp`
- `analysis/resume84-doe-operations-traversal-next.md`

## Source checks

- The pass runs after normal XML child dispatch and before returning accumulated `status`. Thus normally parsed header-only/empty input can return false after IDs update. Malformed XML returns before this tail. Unsupported child branches throw before it.
- It snapshots `s.count()`, iterates indices in order using `s.varAt(index)`, skips null outer results, and invokes the number scan only for a non-null Var with number below 1. Positive IDs are preserved.
- `nextVariableNumber` rescans the current `Study::vars` list for each qualifying entry, so prior assignments affect later rows. Its comparison is signed (`number >= candidate`). Increment is performed in uint32 and bit-cast back to int32, matching native add-eax,1 wrap without C++ signed overflow. The scan does not skip null entries, matching the inner native dereference. Thus a null encountered by the inner scan throws before storing the current outer variable, while earlier child effects remain.
- Aliases remain the same pointers; a later alias sees an earlier assigned number. No index, operation list, vector order, or ownership mutation is introduced.
- The Operations branch still represents normal-return status only; the outer pass is a distinct later InputTable effect. Comments/tests do not claim native internal QDom ownership or fault parity.

Tests encode the retained examples, including `[0,0,5] -> [6,7,5]`, `[-9,-2] -> [0,1]`, and order-dependent wrap `[0,INT_MAX] -> [INT_MIN,INT_MAX]`. They cover false status with normal dispatch, malformed-input early return, alias identity, outer-null skip, inner-null refusal after earlier child effects, and preexisting payload guards.

The active Type2 fixture exercises the pass through `parseSubProject` and the same `ownedDoeStudy` before subsequent sample effects. Its Var instances are manually installed in `scalarPrefixVariableOwners`; this verifies owner-bound lazy-read sequencing and alias preservation, but does not establish that original variable parsing or the native factory produces these records.

## Guard boundary

The source still rejects nonempty `Study::vars` in `writeInputTable`/`saveInputTable`. Type2 Save still stops at `requireRefreshDelivery` after preceding prefix/XML effects. Type3 remains guarded in subproject loading. This increment is reader-side ID finalization only; it does not admit populated variable serialization or public DOE Save completion.

## Disposition

No source correction is indicated. Root can proceed with its planned build/test gate. Retain the limitation that this is static/source-level equivalence, not new native runtime confirmation.
