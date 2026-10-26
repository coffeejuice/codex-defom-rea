# CP137 private actual-Study empty-variable pruning

Added an isolated prune stage on actual coherent Study-owned HeaderVariableRecord objects. It collects empty-row names before deletion, then clears incoming links, releases each matching concrete owner/list alias and invalidates all sample profiles. No filesystem operation or public Save continuation is added. The earlier nonempty-variable reconciliation guard remains.

Contract: `analysis/resume136-doe-save-frontier.md`, SHA-256 `9E3C450AEC9EE949AF27A4ADFD9A77DE095E058ABA7FF241DB3EB41524CBE64E`. Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained prune91c820 SHA `15D8F45477D68837C3C663679C3C6B4E9DBF095E3DCD23B780A360647C6281AF` and removal91a090 SHA `31046A65E407AC72CC15E0FE5346262FB824FAE356EAF5801170641B46C0AE8E` qualify the normal mutation order. Earlier0d2b60 reconciliation and later XML open are distinct source stages, as recorded in the contract.

## Source and owner semantics

Only existing `source/apps/moproj2mst/src/doe/doe_study.cpp`, NEW private `src/doe/empty_variable_prune.hpp` and NEW `tests/doe/test_empty_variable_prune.cpp` changed. No shared/public header, CMake, Type3 owner/row/parser, Session or shared Save file changed. The private header forward-declares Study and declares `void pruneEmptyHeaderVariables(Study&)`; no additional shared header was needed.

Private admission requires equal borrowed-list/concrete-owner sizes, nonnull owners, every borrowed pointer belonging to a concrete header owner, distinct aliases and content-unique names. Null/foreign/scalar/geometry-prefix/aliased/duplicate-name host states refuse before the name scan or any mutation. Pointer membership is checked before dereferencing the borrowed variable. This is a bounded reconstruction admission, not native input validation or fault parity. Current admitted header imports create one owner per content name, including null/empty content equality, so native by-name keyword count agrees with that row's items count.

After admission, the helper collects names in original order when BOTH items and files are empty. It completes that collection before removing anything, then looks up each first current matching name and calls the data-only mutation core. numSamples, links, Description, operation mappings and sample state are not eligibility predicates. Header records with admitted zero samples are treated the same as positive samples. Survivors are not reindexed/renumbered and selectedVariableIndex is not adjusted.

The new local `eraseOwnedHeaderVariable` factors the existing CP103 removeVarFiles mutation: find the concrete unique_ptr owner before mutation; clear every incoming Var::link equal to the target (including self/cycles); destroy the concrete HeaderVariableRecord rather than delete through nonvirtual Var; remove its borrowed alias; set all three invalid flags true. Sample tables/counts/mode remain. No whole-Study rollback is added. Name-collection allocation failure precedes removals; a later interruption retains earlier completed removals. No original Qt COW/refcount, allocator failure schedule or QString destructor timing is claimed.

removeVarFiles now delegates only that existing mutation to the same core and then performs its original subsequent GEO/COR path/existence/deletion planning. The private prune does NOT call removeVarFiles and has no path/StudyIo/FileEffects parameter, filesystem lookup or file-effect call. This prevents the otherwise unconditional geometry path probes from entering native91c820's model-only stage.

## Focused tests and static preflight

New target `m2m_doe_empty_variable_prune`, CTest `m2m.doe.empty_variable_prune`, has five cases:

- Real Session/Project/Type2 SUB lazy read retains adjacent actual header records with positive/zero samples, forward/cyclic/self links and Description. Private prune removes all eligible concrete owners/aliases, invalidates samples and preserves table/count/mode/selected state, without StudyIo read/write/directory/XML or notification effects. Repeating on empty state is stable.
- Count-only retained keyword and existing isolated Geometry-row value fixtures keep their actual concrete owners. Adjacent empty targets are removed; only incoming links to deleted targets clear, surviving links/IDs/indexes/Description/row counts remain. A subsequent no-eligible-row pass does not invalidate currently valid flags. These fixtures do not admit Keyword/Geometry parsing or payload I/O.
- An empty coherent owner preserves all sample state.
- Ten unrepresented states (extra/null borrowed row, foreign/scalar/geometry-prefix pointer, duplicate alias/name, null-vs-empty duplicate name, extra owner and null owner) refuse before graph/sample mutation. The null-owner case verifies admission does not dereference its stale borrowed alias.
- Public Save on an unpruned nonempty actual Study still refuses at VariableDimensionReconciliation with graph/sample state and old SUB DOM retained, no XML or later sample writes and no notifier reset. The test does not manually prune first and mislabel the next Save as native reconciliation.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_empty_variable_prune.cpp` passed. Quoted source-relative private headers resolve and Testkit main/macros are valid. The MinimalViews::rect override was checked against `moproj_write.hpp:43` and includes its item-index parameter. Direct whitespace/final-newline checks passed for all three files, including new files; git diff --check reported no errors. Static inspection confirmed no filesystem/effect calls in the prune body. No worker compile/build, executable or CTest/full suite ran. Root owns compiled/runtime validation, including existing variable-link-removal regression after factoring the shared core.

## Source freeze

All source hashes sent to root before this receipt; source held through grouped validation/archive:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `D9DEECAE5E5AF237778E842120B05337DCE37A1AB97F098BB8FFDA7014DC3E2B` |
| `source/apps/moproj2mst/src/doe/empty_variable_prune.hpp` | `111BB74DE9C982E9F40D58514BF383D7FC3DAAFA4C1B8810BF5269A972CD17BE` |
| `source/apps/moproj2mst/tests/doe/test_empty_variable_prune.cpp` | `9C60F718F303740E58BDB8039C3CE753780285E3643C2F08D8C5F6E38DD7E53A` |

This isolated helper cannot certify the preceding global/current-MO/lazy operation-manager stage. Public integration must first close that source-owned reconciliation return, then run prune before XML device open; a later open failure must retain removals. Empty-variable public Save separately retains its actual refresh key/receiver/normal-return guard. No native/public completion claim follows from this private state stage.
