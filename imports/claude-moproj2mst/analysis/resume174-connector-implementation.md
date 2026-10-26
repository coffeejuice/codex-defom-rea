# CP174 connector implementation release

CP174 models only QtCore `[0x1311da,0x1311de)`: publish the already retained root QList shared-null address into private `+0x70`, then stop before its second atomic add. Parent supplied CP173 frozen/passed status before this implementation. This report releases the source for independent review and validation; it is not a build/test/freeze acceptance receipt.

## Source effect and ordering

`SparseQObjectPrivate` now contains `secondQListSharedNullBinding` and a false `secondQListReferenceCommitted` marker. `BeforeSecondQListBinding` remains the exact admission phase; success advances to `BeforeSecondQListIncrement` with refusal text at RVA `0x1311de`. The second binding is assigned the root QList record returned by the same qualifier that checks the first binding. Both pointee and control block identity of the first QList, QString and QVector bindings are required, as is the allocation-store/root owner pairing.

Before any CP174 write, admission validates the retained Report ordinal/pointer, native task/private sizes, allocation-time private identity, written task/private address points, earlier reference markers, four observed zeros, qualified QVector binding/marker, incomplete QObject/base and unavailable backlinks. The new binding must have an empty ownership identity, not merely an expired pointee; a previously published but expired binding also refuses. The second marker must remain false. Only binding and cursor change on success. All deltas, optional observations, prior bindings and markers, identities, counts, maps and completion/backlink state are preserved. No absolute count or fixed prior delta is required.

No `view_manager.hpp` edit was needed. `QListSharedNullState::moduleSha256` and `symbolRva` are compile-time constants in the current model; there is no optional QList symbol override to corrupt. Admission checks those actual fields rather than adding an unsupported runtime qualifier. The existing optional QVector qualifier is checked when present. Sparse unknown private bytes remain unmodeled.

## Authored checks and static audit

Added three CP174 cases: actual `ConversionSession` positive pairing with independent nonzero QList history and supplied observation, exact first/second root identity plus same-key/different-key reentry; a retained-predecessor refusal matrix (including missing/expired/foreign/aliasing or wrong-owner prior bindings, uncommitted markers, expired root, malformed QVector qualifier, premature/expired second binding, premature marker, wrong phase/size/address/identity, missing/nonzero zeros and completion/backlinks); and absent/foreign/both aliasing owner-pairing refusals at an explicitly supplied CP173 predecessor. The refusal matrix snapshots the private/frame state, binding ownership, all three deltas, observations, allocation counts and factory map counts before calling the continuation and checks preservation.

Existing ordinary first-call and continuation stops in the owned factory test file were migrated to `BeforeSecondQListIncrement` / `0x1311de`; 38 explicit new-cut refusal strings and zero old-cut refusal strings remain there. Earlier-cut rewind fixtures clear the new binding/marker. CP173 symbolic increment tests still require the QVector delta change once while following through the new CP174 store; they make no second QList increment claim. Static type audit used the declared root record types, optional count/zero types, weak/shared pointer owner comparisons and existing Session owner accessors. No nonexistent mutable QList qualifier was introduced.

## Limits and ownership

No compilation, focused/full tests, original comparison, candidate freeze, GUI, deployment, live mutable REA, version allocation, shared recovery state or shared plan change occurred. Parent must run the authorized source-stable gates. No GUI-control session was acquired. The three owned source/test files and this report are released; `view_manager.hpp` is unchanged.

## SHA-256 release receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `7703139852f2eec98d7da3485ffa0ea2df4b27047876ea3d56c9a9697110a7bb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d9cf855f0c06db319657b682b3442def787ffbedbaf39caf9354892355994a0` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `e21a97c15fefc135b0e498272fd4b3e9beb8c476d019bd33943e97552eff1cc0` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |
