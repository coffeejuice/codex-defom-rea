# CP220 conditional project-directory metadata: independent review

## Verdict: PASS for the bounded staged-metadata increment

The source change matches the static condition and keeps the new path as inert staged metadata. The CP220 full build completed successfully with the source fingerprint unchanged during the build, and the focused CTest run passed all three selected tests, including both relevant model targets. This closes the validation gap noted in the initial review.

## Source findings

- `operation_rescue.cpp` applies the existing `id == 0` ordinal fallback before `createOperation`, then reads `activeSubProject(p)` and passes its type (or `std::nullopt`) with the effective ID to the collector. The insertion is in the `<Type>` branch after operation construction, so malformed operations without a constructed Type do not append decisions. The branch is reached while parsing operation-list rows; it does not derive the predicate from the operation class or row name.
- `operation_project_directories.hpp` preserves the positive-ID gate and the fixed order `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`. It appends the cleaned `projectDir + separator + DieStress` path after those five only when the active type is 4. `projectDir(p)` resolves the project path to an absolute cleaned directory. The test also confirms a negative ID contributes nothing.
- The collector only appends strings. `stagedOperationDirectoryDecisions` is a separate `Ctx` vector, and the staged parser test checks that `FileEffects.mkdirs` stays empty and the scratch tree is unchanged. I found no use of this vector by an I/O path, no exported C ABI change, and no path added to a public directory-write surface.
- Existing parse-failure behavior is consistent: earlier operation groups remain appended if a later operation fails. The staged Open fixture's malformed final row checks retained entries and the accepted-suffix failure state; its three valid groups remain contiguous and preserve duplicates. The hook runs after `createOperation` returns, so a failure before return cannot contribute a group for that row.

## Type-4 parser-test gap

The missing parser-level type-4 fixture is not by itself a source-level blocker for this bounded increment. The helper test independently exercises type 4 and verifies that `DieStress` is sixth, while the Open integration test exercises effective IDs, staging placement, repeated groups, partial failure, and no I/O. Static inspection confirms the parser passes the selected active subproject's type directly to the tested helper. A type-4 Open fixture would still be useful regression coverage for this cross-layer wiring, but the current gap does not change the static conclusion.

## Receipt scope

`analysis/cp220-conditional-directory-full-build.json` records successful CMake configure and full build steps (exit code 0) and `source_stable_during_build: true`; the before/after source-tree SHA-256 values match. `analysis/cp220-focused-ctest.txt` records 3/3 passing tests: `m2m.model.operation_project_directories`, `m2m.model.staged_operation_directories`, and the adjacent `m2m.s.b98_negative_id_native_blobs` regression. The two directory tests pass with no failed cases. Earlier CP216/CP218 receipts remain predecessor context only.
