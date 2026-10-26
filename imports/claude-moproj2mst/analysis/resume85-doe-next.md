# CP85: empty DoeVariables finalization on existing records

The next source-ready active DOE branch is **empty DoeVariables with existing variable records**. Original `0x14090ee20` does not clear that list: at child-list exhaustion it reindexes existing records in order, then returns true. Candidate currently refuses this case. The actual Study and Var.index fields already exist; this increment needs no QWidget, QFileInfo, codec, shared-null count, new variable allocation or notification delivery.

Root confirmed CP84 archive and authorized implementation after this evidence freeze. This investigation changed only this report, using retained original static evidence and bounded PE field/hash reads. No source/build/test/GUI/database action preceded this freeze.

## Current branch reconciliation

- CP83 Operations preserves the normal true/no-payload result; its native QDom traversal/ref/fault caveat is explicit. Do not add a fake traversal loop.
- CP84 outer variable-number finalization now runs after normal child dispatch, with ordered wrapping number selection and separate outer/inner null behavior. It does not reindex.
- Nonempty DoeVariables remains guarded before public variable payload parsing. B121 retained scalar and B122 geometry helpers are separate owner projections and do not prove public parse/notification completion.
- DoeOutput normal populated task/session import still has Report/factory and selected-task dependencies; zero-region/nonpositive-constraint class data is already implemented, and populated payload/application guards remain.
- Type3 optimisation Study ownership and ShapeOptim geometry/file paths remain guarded. The stale study-name folder guard is not the selected independent increment because the native folder route first performs projectDir/path work involving QFileInfo.
- The existing empty DoeVariables reindex refusal is now independent and useful: normal reader completion can reuse CP84's outer ID pass afterward, rather than introducing another isolated owner/helper.

## Original decision, owner and effects

Original GUI SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Retained `wave2-doe-lazy-input-empty-readers-decompile.json` contains completed `0x14090ee20` (same hash qualification as CP83/84). The function initializes local link/deferred-name lists and true status. With no element rows, text/comments/non-element children are skipped. At source exhaustion it calls manager vt+0x1e0 at `0x1409128bc`, then loops the local deferred-link list; that list is empty on this selected branch, so no link lookup/assignment or notification occurs. It returns true. It does not clear manager variables or replace their owners.

Retained `evidence/gui_mo/s1/vtables.tsv` maps both `DefDoeModel` and `SfMoDoeManager` vt+0x1e0 to `0x14091c7a0`. Bounded original DUMPBIN viewed in CP84 confirms this complete 116-byte procedure through RET `0x14091c813`; raw selected bytes SHA-256 `4bbb5259e95301d3a816980bc15e98d5b8d7f5901fdc4fc85181b88043d84777`:

1. `0x14091c7c0` copies the manager+0x38 pointer list into a local list view.
2. Position starts0. Source compares it with the local list count and retrieves each pointer in that original order.
3. `0x14091c800` stores the current signed index into variable+0x18. It does not change number+0x1c, ops, link, scalar/geometry metadata or pointer order.
4. There is no null test before that store. With `[a,null,c]`, a.index has already become0 when the second store would fault. The bounded candidate must refuse at that encounter, retaining a's store and leaving c's prior index untouched.
5. Duplicate pointer aliases are not deduplicated. `[a,a]` stores0 then1 into the same a; the final index is the last position.

Current `Model::allVars` already represents the same normal reindex operation in another query, but dereferences null entries without a guard. Do not change that shared query in this slice or call it after a global prevalidation that loses source partial-store ordering. A private bounded reindex loop in the reader can reuse actual `Study::vars` and preserve the encounter-point unsupported native-fault boundary. No new Var allocation or owner is required.

The existing candidate model/container abstraction is reused. Native QList ref/COW/allocator failure and raw DOM wrapper ownership are not established by this integer-field branch. During this synchronous reindex, the actual list must stay stable; there are no source payload callbacks between stores. A host bookkeeping/temporary allocation must not be narrated as an additional native variable allocation.

## Encounter order relative to CP84

Reindex occurs **inside the DoeVariables child reader**, before later top-level children. CP84 ID repair occurs only after normal completion of the outer child loop. Therefore:

- `<DoeVariables/>` on existing records reindexes, returns true, then outer ID repair may update numbers.
- `<DoeVariables/><RegionsOfInterest><Region/></RegionsOfInterest>` first reindexes; the later region collection is cleared and its existing payload guard throws. IDs are still untouched because the outer pass has not been reached.
- An earlier populated region/constraint/nonempty-variable guard prevents a later empty DoeVariables from reindexing.
- An inner null reindex fault retains earlier index/header stores and prevents both later children and outer number repair.
- Duplicate empty DoeVariables siblings repeat the reindex at their own encounters; there is no one-shot marker or retry ban introduced.

The implementation should keep the current refusal for **any element child** under DoeVariables. A future proof-based unknown-element skip could narrow that guard separately, but no nonempty variable parse, scalar keyword state, deferred LinkTo or sample acquisition is admitted here. Text/comments alone are compatible with the currently selected empty-element branch.

## Reachable actual owner and test boundary

An actual ConversionSession/live Project/active Type2 SubProject owns a Study whose vars aliases can point to existing concrete records retained in its current owner vectors. Operations-only input now handles their outer IDs; empty DoeVariables can independently finalize their indexes on that same owner. A test may explicitly supply such pre-existing plain Var records, then call `parseSubProject` on real SfDoeStudy using RecordingStudyIo. This is a live owner/reader integration fixture, not evidence that public variable payload construction or SetOprHasDoe ran.

Native public Type2 Save is still guarded for a Study with variables: `Type2SaveNextBoundary::VariableDimensionReconciliation` reports `Type2 study Save: InputTable variable dimension reconciliation remains unsupported`. Reindex success must not bypass that guard or reach XML/refresh/GUI ownership for this fixture. Public C ABI active SUB selection is unchanged.

## Exclusive successor files and tests

Root's authorized write set after this report freeze:

- Existing `source/apps/moproj2mst/src/doe/doe_study.cpp`: replace only the empty-list/existing-vars refusal with source-order reindex and encounter-point null guard, leaving nonempty element guard and outer ID pass intact.
- New `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp`: focused owner/order/fault tests.
- Existing `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp`: remove the single obsolete empty-DoeVariables refusal expectation; retain its nonempty variable and region/constraint checks. New focused tests own the new success/ordering expectations.

No header, CMake, shared Session/parser/writer, public ABI, Qt owner or existing native fixture edit is needed. Expected target `m2m_doe_empty_variable_reindex`, CTest `m2m.doe.empty_variable_reindex`.

Focused coverage should check normal index0..n-1 with existing positive IDs/metadata preserved; alias last-position semantics; partial indexes before a null fault; reindex before later region/constraint guard with IDs still old; earlier payload guards preventing reindex; normal combined index-then-ID pass; repeated empty-list encounters; and the actual owned active Study lazy read with ordered sample continuation. A same-owner Save fixture should keep its exact variable-dimension reconciliation guard and no XML effect. All recording probes remain under configured scratch, with explicit declaration headers and active Testkit main/checks. Root owns build/tests after source release.
