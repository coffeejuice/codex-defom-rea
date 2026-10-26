# CP201 inert edge-effect descriptor review

## Decision

The proposed descriptor is source-ready as a bounded, immutable extension to CP200, and it adds useful information beyond CP200's existing guard/target pairs. It can be implemented without new runtime evidence if every field remains a static guarded instruction description: no edge choice, operand read, register update, or cursor advance. It is worthwhile only as explicit semantics for the existing unresolved boundary; it must not be presented as additional execution progress.

CP200 already records the unresolved JNE's two guards and targets at `0x29621` and `0x296c0`. The CP201 addition is the qualified first-instruction detail: the fallthrough MOV's potential module-memory read and EAX/RAX overwrite, and the taken MOV's stack source and conditional RBX restore. That distinction is useful because the existing pair identifies control-flow destinations but not their first effects. Keep the current boundary at `0x2961b` and preserve both alternatives.

## Evidence and qualifications

The plan's original-byte basis is internally consistent with the frozen CP200 qualification: the same QtCore hash is recorded in `analysis/resume163-connector-qobject-static.json`, while CP200's schema binds its JNE to the issued CP199 compare/read. The CP201 report records exact raw-byte hashes for the two first-instruction ranges and the preceding RBX spill. The CP198 prefix implementation documents the generation-3 spill at `[S-0x68]`, including its issued producer and call occurrence. These support a static operand/effect description. They do not prove that either edge ran, that module memory contains a particular value, or that runtime flags select one.

The source qualification explicitly limits the retained QtCore captures to bounded/unwind regions rather than complete logical-function or native-execution witnesses. CP200's validation also states that the record is production-disabled and establishes no selected edge or downstream effects. Those limitations are compatible with this inert extension; they prohibit treating the descriptor as execution evidence.

## Ownership and validator/test gate

The exact implementation owner is the existing `TaskFactoryRuntime` / `PendingTaskPrototype::QObjectPrefix` lane:

- Schema: `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`, extending `QObjectPrefix::TlsIndexJneUnresolved::Edge` while retaining the existing guard and target.
- Issuance and validation: `TaskFactoryRuntime::advanceTlsIndexJneUnresolved` and `validateTlsIndexJneUnresolved` in `source/apps/moproj2mst/src/doe/task_factory.cpp`. Keep issuance attached to the exact immutable CP200/JNE and CP199 compare/read chain; preserve the private issued handle and same-owner checks.
- Tests: `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`, in the CP200 JNE schema cases. The frozen CP200 receipt records focused task-factory coverage and the checkpoint suite; root retains the implementation's focused tests, required regression/full-launcher review, and freeze.

The descriptor should carry exact instruction bytes, instruction length, operand width and symbolic address expression, prospective destination/result, unchanged flags/RSP, and prospective next RVA. For the fallthrough edge, represent `[module+0x2f3d28]` as a *potential* width-32 read and zero-extended result; do not issue or count a read or create a y occurrence. For the taken edge, bind `[S-0x68]` to the existing CP198 generation-3 spill occurrence and describe `RBX=B` conditionally; do not mark the spill as newly read or the restore as executed. Keep edge order and guards fixed, and do not add selected-edge/value fields.

Tests must verify exact bytes, operand arithmetic, widths, successor lengths, predecessor identity, both alternatives, and unchanged live registers, flags, RSP, slots, return records, shared state, read counts, phase/cursor, and completion/publication state. Cover copied, foreign-owner, malformed, stale, rewound, expired, and released predecessor/grant state and failure-atomic preparation, plus stable reentry and direct/resumed equality. Test-only arithmetic substitutions may validate expressions but cannot feed the production record or select an edge. The current CP200 suite's pass summary does not itself validate a not-yet-added descriptor.

## Limits and contradiction check

There is no contradiction between describing the two first instructions and retaining the JNE as unresolved, provided descriptor fields are prospective/static. The report's suggestion that a schema can name an unevaluated memory operand is sound only if the API makes the no-read distinction explicit. Avoid an ordinary `read` or completed-effect type that downstream code could mistake for an issued operation.

The fallthrough edge cannot be advanced past its MOV without a separate same-lifetime module-storage read issuer for y. Neither static contents nor purpose inferred from the apparent once flag supplies it. A concrete branch choice still requires the value of CP199's *same* issued read at `0x29614`; re-reading later cannot reconstruct the consumed flags. The taken edge's spill provenance permits a structurally described conditional restore, not branch selection or permission to execute the subsequent ADD/POP/RET. No claim of full unwind, reentry, exception, synchronization, or native behavior follows.

Accordingly, accept this as a small source increment after root review of the CP200 freeze, with the above no-effect contract. If the implementation cannot encode “potential operand, not read” and “guarded effect, not applied” unambiguously, leave CP200 frozen and retain the static report as the contract. No source, build, test, GUI, or runtime work was performed for this review.
