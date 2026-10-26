# CP216 operation project directory helper review

## Verdict: PASS, limited to the pure helper and its unit tests

The helper in `source/apps/moproj2mst/src/model/operation_project_directories.hpp` matches the narrow CP216 design. It appends exactly five cleaned child paths in the retained order: `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants`. It returns without modifying the vector for every ID below one. Each entry is formed from the caller-supplied project directory, the platform separator, and the child name, then passed through `qt::cleanPath`.

The retained operation specification states the original `id >= 1` gate and names the same five subdirectories in order (`source/docs/re/moproj2mst/s1-01/spec.md:315-318`). Candidate `projectDir` uses `cleanPath(absolutePath(p.path))`, and `listDir` provides the same separator-plus-child then clean-path construction (`src/model/project.cpp`; `src/moproj/read.cpp`). The helper does not independently make its input absolute; that is consistent with the documented caller contract that supplies the already-resolved project directory.

The tests assert fixed expected paths in the required order, including normalization of an input containing `/./`; they check zero-ID no-op behavior, repeated-call multiplicity and ordering, and that collecting against an existing scratch directory leaves its entries unchanged. The Testkit header and `TEST_CASE`/`REQUIRE`/`CHECK` forms are used. The tests do not assert negative IDs, alternate separator forms, or paths with trailing separators, so those spellings are not independently pinned. The implementation's `< 1` guard covers negative IDs by inspection.

This is not connected to `setOperationId`, Open, `FileEffects`, or a filesystem writer. No public integration or CP213 Lubricants-causation claim is supported. No focused test build was rerun in this review; the retained collector note records an earlier build attempt that stopped during unrelated MSVC dependency compilation before the target was reached.
