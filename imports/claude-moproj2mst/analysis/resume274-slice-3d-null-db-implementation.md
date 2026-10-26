# CP274 canonical 3D/null-DB Slice rebind implementation

Implemented the reviewed bounded admission in `source/apps/moproj2mst/src/engine/viewer.cpp::bind`. At the existing post-bind Slice gate, the actual owner must still be `&m` and have a step before dimension access. Dimension 2 remains admitted per CP272; dimension exactly 3 is admitted only when `owner->db == nullptr`. Other dimensions and 3D owners with a database retain the existing `M2M_E_UNSUPPORTED` refusal. The gate remains after object/reps binding and the pending-geometry check; no collection replacement, backlink update, DB synthesis or decision relocation was introduced.

The canonical 3D flag witness is scoped to `GuiModel(nullptr,3)`: the retained native constructor path initializes +0x258 to zero, while the converter constructor has no editor-style flag transition. `analysis/resume274-slice-flag-mapping.md` documents the zero initializer, native getter/setter, and editor counterexample. The converter does not represent the byte, so this supports result and collection-effect parity for this canonical live fixture, not access-order, fault, reentrancy or arbitrary-owner parity. The native helper's earlier gate remains represented by the underlying source qualification; no claim is made about which original early-return gate triggered for other cases.

Updated the existing actual-caller test to make two successful `viewerSync` calls and require the original collection pointer and `viewIdentity` before collection dereferences after each call. Both calls preserve the literal constructor-only collection state and runtime scratch. The CP272 2D success test is unchanged. Testkit entry point and includes resolve through the existing target; assertions use Testkit macros and no C `assert`.

Focused validation succeeded:

- `python -B admin/build_candidate.py --label cp274-slice-3d-null-db --target m2m_mdt_viewer_slice` — exit 0; source stable during build. Receipt: `analysis/cp274-slice-3d-null-db-build.json`.
- `ctest --test-dir build/candidate -R "^m2m\.mdt\.viewer_slice$" --output-on-failure` — 1/1 passed.

No full suite, checkpoint freeze, GUI interaction, original execution or mutable REA operation was performed. Root owns grouped/source-stable integration and freeze. Starting snapshot was checkpoint 273 SSH snapshot `f3af73516fafadf1b03f0a8eeea4bb005d08a19a` (verified by root). Final SHA-256: `viewer.cpp` `708E1BF341170E8A9A4AC6F38E8319BB07254F88EDD7C01C50598E85EF756642`; `test_viewer_slice.cpp` `B25D71635A264F3195A9E9982D1526C8C8C75FA6C9AD3160FC5840AFD84B5695`.
