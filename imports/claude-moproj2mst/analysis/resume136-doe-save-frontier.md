# CP136: private empty-variable prune and the earlier Save owner guard

No manager-backed public active Save continuation is source-closed. Empty-variable Study Save reaches the unchanged refresh-key/receiver boundary; nonempty variables stop earlier at operation-dimension reconciliation. The existing candidate Open manager-read receipt is not evidence of the native Save-time global/current-MO/lazy operation manager.

One smallest independent private state cut is source-qualified on the already-owned header Variable profile: native0x14091c820's collect-then-prune operation, including incoming-link cleanup, concrete deletion and sample-invalid stores. It can be implemented without Type3 row files, public Save admission or any file access. It must not call removeVarFiles, whose filesystem work is a different path. This report proposes a private helper only; it does not say the preceding native reconciliation occurred.

Only this NEW report was written. No source/test/build, GUI/original process or mutable REA action occurred. Type3 row/owner files reserved to the other lane were not edited. Current DOE source and bounded retained original captures were read.

## Current ordering after the private INI increment

CP135 extends writeDefDoeIni's buffered private FileEffects plan. It is not a new production callsite; src has no caller of that helper outside its definition. The native post-MST INI stage cannot run ahead of the public XML/refresh/.moproj/MST sequence merely because its text is now known.

For empty Study::vars, the actual Type2 node prefix selects XmlOutputDevice. Immediate XML open/write/close/error (now including known Constraints) produces NormalWriterReturn, then RefreshBoundaryReached. CP131's isolated notifier outcome distinguishes suppressed callee return from UnsuppressedDispatchRequired; public requireRefreshDelivery remains noreturn. Fresh canonical suppression is false and no source producer makes it true for this selected Save. The ordinary next edge is still invocation-bound key QString/codec/data, then actual map/list/receiver/timer/normal return. Later PAR helpers remain behind it. Type3 stack stream/optimizer and supplied-line projections do not close these owners.

For nonempty Study::vars, appendType2SaveNodePrefix still appends the Study element to the pending SUB then selects VariableDimensionReconciliation. It does not prune, open XML or deliver refresh. Keep that behavior. Legacy saveInputTable's own nonempty guard is separate and also remains.

## Three different managers, not an interchangeable Project

Native0x1400d3200 calls0x1400d2b60 at0x1400d3227 before the variable XML writer at0x1400d349c. The latter calls prune0x14091c820 at0x14090a5a9 BEFORE QFile construction/open. Thus correct eventual order is reconciliation -> prune -> XML device -> refresh -> cached-Study PAR sequence.

Retained reconciliation code proves this nonempty entry:

1. Test manager+0x38 variable list for empty. Only empty returns atd31ea before global lookup.
2. Snapshot its variable-pointer list atd2ba6, then call process/main-window getter0x1402c8090 via global0x143d766b8 atd2bb3. Null and failed dynamic-cast branches return after snapshot cleanup.
3. Resolve current MO through0133a0 and Project/backlink045780 atd2c01/d2c09. It can differ from the Project whose Study Save cached the original manager.
4. Call0x1400a58b0 atd2c24, then enter the per-variable operation/dimension loop only if its result is nonnull.

A58b0 obtains the current MO viaA5890 and calls lazy getter044c80. That getter reads Mo+0x19c8; null allocates0x38, obtains a parent through044a90, calls SfMoOprManager constructor045fe0, then publishes the result (including the raw-null result). This is a real owner/constructor stage, before each variable's first operation-id lookup. Empty ops or eventual no matching operation do not bypass it. A supplied false/no-current-model branch cannot be inferred from the saved Study.

Distinguish (a) Session ModelManager, projection of native mainwindow+0x50/current MO collection, (b) actual Study/SfMoDoeManager at selected SUB+0x20, and (c) current MO's lazy SfMoOprManager at+0x19c8. Project::operations is the existing value-level row projection of (c); its presence/emptiness does not prove the actual lazy field was read/constructed or its QObject/item-selection child returned. Calling initOprManager would clear rows and is not that lazy getter.

New SessionModelManagerReadReceipt explicitly records CandidateExistingOwnerRead at fresh Open and declares mainWindowReady=false/nativeFactoryCompleted=false/nativeConcreteMoClassObserved=false. requireManagerFor revalidates a candidate Session manager, not this Save-time global getter/dynamic cast/current MO/Project or Mo+19c8. It must not be repurposed as a reconciliation-return token. A Project/ModelManager equality check also is not a native requirement: selection can differ, so preserve the actual selected current owner rather than force it to the saved Study's Project.

## Source-closed isolated prune

The original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Complete retained prune0x14091c820 has two phases:

- It gets variable count, default-constructs a temporary name list and scans original list order. For each row it obtains name and row pointer, queries keyword count by that name (vt+0x128 ->91a950), and geometry row count on the current pointer (817840). If both counts are below1, append the name to the temporary list. numSamples, incoming/outgoing link, description, operation mappings and sample validity are not eligibility predicates.
- After the complete name collection, for each name in that order, look up the current FIRST matching variable (vt+0x180), get its pointer (+0x1a0), then invoke manager removal+0x100 ->91a090. It does not delete while collecting, sort names or normalize IDs/indexes.

The bounded actual header-record admission creates one concrete owner per content name. Duplicate input rows update existing/last records instead of creating duplicate named owners. A first implementation can require coherent one-to-one header ownership/borrowed vars and content-unique names (including null/empty content equality), refusing null/foreign/alias/duplicate-owner host states before mutation. This keeps the native by-name keyword lookup equal to that current owned record's items count. Do not broaden to arbitrary mixed scalar/geometry-prefix owners from type similarity.

Removal91a090 clears ALL incoming Var+0x40 links equal to the target, destroys that concrete variable, removes its borrowed list pointer, then sets all three sample-invalid flags true. The existing CP103 removeVarFiles has this bounded concrete-owner value mutation, but subsequently computes/probes three GEO/COR paths regardless of target row emptiness. Calling it from prune would add file access/deletion that native91c820 does not perform.

Native prune does not reindex survivors, renumber IDs, adjust selectedVariableIndex, clear retained sample table/count payloads, or decide based on numSamples. It returns its initialized true byte on normal completion, including no eligible rows; that does not mean XML or Save succeeded. Native QString/QList COW/ref/allocator/destruction timings are not represented by this host value cut.

## Proposed exclusive implementation and tests

After root assignment, reserve existing `src/doe/doe_study.cpp`, NEW private `src/doe/empty_variable_prune.hpp` and NEW `tests/doe/test_empty_variable_prune.cpp`. A narrow API such as `void pruneEmptyHeaderVariables(Study&)` names an isolated stage and accepts the real Study owner. It must not accept a supplied reconciliation-success flag, add a public Save capability or call the public native writer.

Factor the existing concrete record/link/sample removal into a shared local data-only routine used by removeVarFiles and the private prune. Keep removeVarFiles' existing subsequent file-effect behavior unchanged. Prune first validates the bounded coherent domain and collects names from items/files emptiness, then re-finds/removes by name in collected order through the pure mutation routine. It has no FileEffects/path/StudyIo parameter and no file-existence/removal calls. Unsupported model admission has no removals; failures while collecting names precede removals. Once removals begin, retain each completed removal/invalid store if a later admitted step fails; do not create whole-Study rollback or infer native allocator fault schedules.

Focused tests should lazy-read actual Session-owned header records with positive and admitted zero samples, deferred forward/cyclic/self links and descriptions, then explicitly invoke only the private prune. Check all-empty records are removed regardless of sample count; incoming links are cleared on surviving owned records; a caller-supplied supported keyword/geometry count fixture retains its row; sample invalid flags change only when removal occurs while tables/counts/mode remain; survivor IDs/indexes/selected index remain unchanged; repeated no-op prune leaves state stable. Count-preserving test fixtures are value projections, not new Keyword/Geometry input admission. Foreign/scalar/geometry-prefix/null/alias/duplicate-name owner states should refuse before graph mutation. No filesystem or planned-file effects belong to this API.

A separate actual public Save test on an UNPRUNED nonempty fixture must still stop at VariableDimensionReconciliation after the pending-node prefix, before removals/XML/sample effects. Do not manually prune then claim that a later successful empty-variable branch proves native reconciliation. Existing removeVarFiles ownership/link tests should remain valid; root can run them without changing their contract. No Type3 file or shared header/CMake edit is required.

## Exact missing input for eventual public integration

Before wiring prune into active Save, root needs a source-owned Save occurrence that binds the process main-window lookup and its dynamic type, selected current MO/Project, and the lazy Mo+19c8 field/constructor result (or the actual earlier null-return discriminator). Only after that qualified reconciliation return may public pruning happen. A fresh Open manager receipt, saved Project pointer, empty operation list or constructor label cannot stand in for this occurrence. The first uncertain branch is global main-window/current-MO ownership, and on its nonnull branch the first unmodeled mutation is lazy operation-manager creation/publication. No further static function-name scan is necessary to state that boundary.

Later XML open failure must retain already-completed prune removals, because prune precedes the device. Later key/callback refusal retains prune plus XML/flag state. Current production guards prevent those effects from being reordered; keep them while the helper is isolated. The separate CP130 process codec/read/data witness is still needed on the empty-variable unsuppressed refresh branch.

## Inspection anchors

Retained captures: reconcileD2B60 SHA `4E5A81664574951B2A327A00723BC13C1FF0153292BE67613F21C1F0598A59A2`; prune91C820 `15D8F45477D68837C3C663679C3C6B4E9DBF095E3DCD23B780A360647C6281AF`; removal91A090 `31046A65E407AC72CC15E0FE5346262FB824FAE356EAF5801170641B46C0AE8E`; lazy044C80 `A2D2711054AF6D91B5DF59890377CE57D3104751A96808AD3F6142D13A70B179`. These instruction-qualified bodies and CP103/112 reports control native order; historical reports are not rewritten.

Current-source inspection: doe_study.cpp SHA `00B2E7AED3D47728F6A54D2EB70BF09B3B93076711FB29C8731FB591394D7423`; SessionModelManagerReadReceipt header `F9D8C014B7F6FC4BD14E0795CB7B9C3AD1E4317D3A7BDF6D08771DBCCA698750`. This report claims no source release, runtime test pass or public completion.
