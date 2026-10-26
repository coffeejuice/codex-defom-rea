# CP218 staged Open directory decisions: independent source review

## Verdict: PASS, limited to candidate-side staged metadata

The increment is placed at the intended Open parser boundary. `parseOperationList` assigns each `<Operation>` its document-order ordinal and passes it to `parseOperation`. In the `<Type>` branch, `id == 0` is first replaced with that ordinal; only after `createOperation` returns does the code call the CP216 collector with this effective ID and `projectDir(p)`. Thus an explicit positive ID and zero-ID fallback qualify, while a negative ID produces no paths. Operations without a successfully constructed Type branch do not contribute.

`openProject` sets `p.path` to the cleaned project file path before reading the XML. `projectDir(p)` returns `cleanPath(absolutePath(p.path))`, so the collector receives the absolute project directory. The collector preserves the documented order `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`, and the integration appends each returned group in parse order. Its `< 1` gate excludes negative IDs.

Partial-parse behavior is coherent for the stated staging contract: entries appended for earlier operations remain in the caller's `Ctx` when a later operation returns false or throws. The test reaches this case by appending a malformed final operation and checks `openProject`'s accepted-suffix return separately from `project.openedOk`. If failure occurs inside `createOperation` before it returns, that operation contributes no staged group; this is a consequence of the chosen post-construction hook and is not tested as a successful construction. Multiple valid operations preserve order and multiplicity, including repeated paths, because the context vector is append-only and no deduplication is performed.

No filesystem or FileEffects behavior is enabled. The new vector is not read by `applyEffects`, and it is not populated into `engine::FileEffects::mkdirs`; the Open test checks an empty effects list and unchanged recursive scratch-tree entries. No writer integration or exported C API declaration changes are present. `Ctx` is a source-visible C++ struct in `include/m2m/common.hpp`, so adding the vector changes its C++ layout for separately compiled consumers; the reviewed change does not establish a stable binary ABI for that internal context type. The public C ABI in `moproj2mst.h` is untouched.

## Test review

`test_staged_operation_directories.cpp` exercises the real `openProject`/XML parser route, not just the CP216 helper. Its fixture copy has a positive-ID operation, an ID-zero operation (ordinal fallback), a repeated positive operation, a negative-ID operation, and a malformed last row. It checks the three expected five-path groups in order, their multiplicity, the rejected negative-ID contribution, parse-failure retention, no generated mkdir effect, and an unchanged scratch tree. The positive operation is copied verbatim, so it also tests duplicate operation-name handling along this path; path expectations remain independent of operation names.

The test source is discovered by the repository's `tests/*/test_*.cpp` convention. I did not build or execute it; the implementation note assigns that validation to the coordinating task. The source-only review therefore does not claim runtime test passage.

## Boundary

This PASS covers candidate source placement, effective-ID handling, project-root selection, path ordering, per-call staging, and the absence of filesystem/public C ABI effects. It does not establish an original runtime occurrence, actual mkdir attempts/results, failure ordering of original filesystem calls, or permission to apply these staged decisions as writes. Keep the public directory-write gate closed pending the separately specified same-occurrence original witness.
