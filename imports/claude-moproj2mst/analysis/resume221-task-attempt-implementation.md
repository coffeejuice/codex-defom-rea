# CP221 task-directory attempt implementation

Implemented the internal ordered Open attempt metadata in the converter subtree.

- Added `StagedOperationDirectoryAttempt` with cleaned UTF-16 path and explicit `ReturnIgnored` policy.
- In `parseOperation`, after `createOperation` returns, eligible IDs (`id >= 1`) stage `taskDirAbs(*item)` first, followed by the existing project paths in source order. The existing `stagedOperationDirectoryDecisions` project-only shape is unchanged.
- Negative IDs stage no task/project attempt. Metadata does not enter `FileEffects`, invoke filesystem APIs, or alter the C ABI.
- Extended the staged parser test to check positive, ordinal-zero, repeated positive-ID, and negative-ID cases; order, effective IDs, ignored-return policy, project-only vector shape, malformed later operation prefix, and scratch-tree preservation.

Focused build attempted with `cmake --build build/baseline --target m2m_model_staged_operation_directories`. It failed before reaching this target's sources because the configured MSVC compiler could not find standard headers (`cstdint`, `cstddef`), indicating a broken/incomplete compiler environment. The test was not executed. The fixture has no demonstrated Type4-active variation in this test, so conditional `DieStress` sequencing remains covered by the existing project-only tests/source review rather than added here.
