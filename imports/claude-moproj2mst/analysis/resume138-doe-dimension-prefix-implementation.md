# CP138 private supplied-owner Variable dimension prefix

Added the isolated per-row dimension stage on actual Study-owned HeaderVariableRecord objects, using an explicitly supplied operation Project. It preserves first-operation selection, missing/equal skips and the dimension store before the keyword boundary. No global/current-manager resolution, prune, I/O, sample mutation or public Save continuation is added.

Evidence contract: `analysis/resume138-doe-dimension-frontier.md`, SHA `C5BBE85DDE71E2038CB63FD2CFB1DD9F06136D5C096CEBC0E08B5EB455F33648`. Original GUI PE SHA is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained0d2b60 body binds first-ID816ae0 -> lookup047dc0 -> dimension05f0f0 -> comparec0df50 -> store0d8040 -> only then item snapshot0d0e60. Setter0d8040 writes only+24; it does not invalidate samples or enter geometry/keyword/file work. The contract records bounded raw hashes and exact sites.

## Exclusive source and API

Only three NEW files were created under `source/apps/moproj2mst`:

- `src/doe/variable_dimension_prefix.hpp`
- `src/doe/variable_dimension_prefix.cpp`
- `tests/doe/test_variable_dimension_prefix.cpp`

No existing owner/parser, public/shared header, Session, Save, CMake, Type3 or CP137 source was edited. The existing source/test globs cover these files.

The private API is `VariableDimensionPrefixResult reconcileHeaderDimensions(Study&, const Project& suppliedOperationProject)`. Its diagnostics distinguish SuppliedRowsCompleted from KeywordItemSnapshotBoundary, with visitedRows, dimensionsStored and optional stoppedAt. `completeForPublicSave` is explicitly false. Native0d2b60 returns void; these results are host diagnostics, never an actual manager/callback receipt.

Before mutation it validates equal borrowed/concrete owner sizes, nonnull owners, each borrowed pointer belonging to a concrete HeaderVariableRecord and distinct aliases. It never dereferences a foreign/null/stale alias to establish ownership. A local borrowed snapshot is then visited in source order. Each variable selects only ops.front(), or-1 when empty. operationById searches only the supplied Project. Missing operation continues without inspecting items; a matched Operation must have that exact Project backlink before opDim uses its registry. There is no fallback to the saved Study's Project or another operation ID.

Existing opDim supplies source-qualified class dimension behavior and default3 when its registry lookup finds no class. Equal dimension skips item inspection. A mismatch stores dimension first; empty items continue, nonempty items immediately return the explicit boundary after the retained store. Later rows remain unvisited. Matched owner/getter refusal propagates at its encounter without undoing earlier stores. No foreign OpClass is fabricated as a native callback-success implementation.

The helper does not copy/reparse keyword records, normalize IDs/indexes, change Description/links/files/selected index, invalidate sample flags, remove Variables or touch output/notification state. Empty-item traversal is a value-effect projection; literal Qt QList snapshot/ref/allocator and first-null-item semantics are not claimed. The supplied stable snapshot does not reconstruct the native pre-global snapshot across unknown global selection activity. No public callsite uses this helper.

## Focused tests and static checks

New target `m2m_doe_variable_dimension_prefix`, CTest `m2m.doe.variable_dimension_prefix`, has seven cases on real Session-owned lazy-imported header records. Ops/items/current-dimension values are explicit private test profiles, not new public Keyword/Geometry reader admission:

1. Empty ops and a missing first ID skip nonempty items; a matching later ID is not tried; absent registry class defaults to3 on the matched row.
2. Existing Boolean2D factory/registry ownership changes3->2, equal dimension skips nonempty items, and missing class changes2->3. IDs/indexes/Description/links/files/sample state and I/O/notifications remain.
3. Earlier dimension changes and the current changed record survive KeywordItemSnapshotBoundary; later rows remain untouched.
4. A later matched Operation with a foreign Project backlink refuses after retaining an earlier completed store.
5. A distinct supplied operation Project controls the lookup/dimension without reselecting or replacing the saved Study owner. The saved Project's same-ID3D operation does not override the supplied2D owner.
6. Five malformed Study owner/alias states reject before any dimension store, including null concrete ownership with a stale borrowed alias.
7. Public Save on an untouched nonempty actual Study still stops at VariableDimensionReconciliation before dimension mutation/prune/XML; old SUB DOM, samples and notifier flag remain, with no later file effects.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_variable_dimension_prefix.cpp` passed. Source-relative private includes resolve; Testkit main/macros are valid. The fixture's rect(const SubProject&,int) signature was checked against the actual interface. All new files passed direct trailing-whitespace/final-newline checks. Registry fixture construction copies the class key before moving its owner, following the existing insertion pattern. No worker build/compiler, executable, CTest/full suite, GUI/original process or mutable REA action ran. Root owns compiled/runtime validation.

## Source freeze

Source hashes sent to root and independent reviewer before this receipt; all source held through grouped validation/archive:

| File | SHA-256 |
| --- | --- |
| `src/doe/variable_dimension_prefix.hpp` | `292FE47DC87730A5CBD8845660483F042820FE5AC05A4CA61F6DDD6EA864619B` |
| `src/doe/variable_dimension_prefix.cpp` | `1922502BFBC94967A29B489DFA562DF79C8B5D12478A54C5940060992C365BC4` |
| `tests/doe/test_variable_dimension_prefix.cpp` | `45A4741F163DD49F33C44A5ED012CD06D8E4FE137B6610CEC54D82AED5734430` |

Public integration still requires the actual Save global/main-window/current-MO/Project/lazy OprManager source occurrence and preserved pre-global variable snapshot. Only after its qualified reconciliation return may public prune precede XML open. Nonempty keyword conversion remains separately guarded after the reached dimension store. The empty-variable public branch retains refresh-key/map/receiver/timer/normal-return dependencies. This private stage closes none of those production owner/lifetime boundaries.
