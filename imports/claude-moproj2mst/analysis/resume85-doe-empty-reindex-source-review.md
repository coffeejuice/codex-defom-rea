# CP85 independent review: empty DoeVariables reindex

## Review result

The frozen source implements the recovered empty-child reindex at the encounter point and leaves the later CP84 number finalization separate. Tests cover the important order and partial-effect distinctions, including duplicate aliases, null fault position, both later and earlier payload guards, repeated empty readers, and the active owned-Study lazy-read fixture. No source correction is indicated. This is a static review; no build or tests were run.

Reviewed:

- `source/apps/moproj2mst/src/doe/doe_study.cpp`
- `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp`
- `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp`
- `analysis/resume85-doe-next.md`

## Branch and ordering checks

The DoeVariables branch rejects any element child before reindexing. For an empty element list it calls `reindexExistingVariables` immediately, then returns child status true. The helper snapshots `count()`, walks `varAt(index)` in encounter order, and stores the index to each returned variable. A null encountered at that exact step throws with the qualified original address. There is no up-front validation, so prior stores remain; a duplicate pointer is stored repeatedly and ends with its last list index.

This operation happens while the outer XML children are being dispatched. Later siblings do not run after an inner null fault. CP84's variable-number pass stays at the outer reader tail, so a later guard or reindex fault prevents it. The tests verify reindex-before-later region/constraint guard while retaining old numbers, earlier guard preventing reindex, and a combined normal pass where reindex precedes final ID repair. Repeated empty DoeVariables siblings reindex at each encounter; number repair still runs once after the child loop.

The adjusted expectation in `test_variable_id_finalize.cpp` is consistent: it no longer expects the now-supported empty reader to refuse, while the nonempty DoeVariables guard and region/constraint partial effects remain covered.

## Owner and guard boundary

The active fixture uses a real ConversionSession, Project, active Type2 SubProject, `ownedDoeStudy`, and its `doe` alias. It supplies concrete plain Var owners explicitly, reads the table through `parseSubProject`, checks preserved Study/list identity and ordered sample continuation. This proves the helper runs on the current owner in the lazy-read path. It does not prove native variable parsing, native allocation/factory behavior, notification delivery, or runtime parity; the variables are manually installed.

Nonempty variable children remain refused before reindex, including unknown element children under this conservative guard. Public `writeInputTable`/`saveInputTable` still reject a nonempty variable list. Type2 Save with those variables still stops at the variable-dimension reconciliation guard and produces no InputTable XML or replacement DOM in the test. The subsequent refresh gate and other public DOE admission constraints are not bypassed.

## Disposition

No correction is indicated. Preserve the implementation's narrow claim: empty DoeVariables reindexes already-owned records in source order, with encounter-point null failure and partial stores; it does not enable variable parsing or populated DOE Save.

