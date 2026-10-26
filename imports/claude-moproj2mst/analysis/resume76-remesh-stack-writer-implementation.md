# CP76 stack-backed native remesh writer

## Implementation

Applied the CP76 stack-allocation design in the frozen remesh source lane. The public `saveRemeshAtProjectSave` now constructs `datafiles::detail::CoggingFile` directly as a local stack object after path, enabled-state, and locale checks. Its failed-open report and invalid-parameter guard remain in place. A small stack adapter references that `CoggingFile` and forwards variadic-format arguments through the existing legacy `vfprintf` implementation.

The new private `detail::writeRemeshSaveBody` is the sole owner of the 31 ordered format attempts and ignored close result. The public path calls it with the stack adapter. The focused unit test supplies a stack fake directly to this body, failing write 6 and the close result while recording all calls. The test asserts writes 1–31 followed by close, confirms later writes continue after the failure, and makes no physical file-residue claim.

Removed the `std::function` factory, `unique_ptr<NativeSaveFile>`, and helper that created the native writer through the heap. Public headers and converter/Save interfaces are unchanged. The default path retains the actual `CoggingFile` constructor, its native `FILE*`, 4 KiB buffer, formatter, ordered formats, and explicit close behavior. The 31-write source count was checked statically. Targeted whitespace scan found no trailing whitespace.

## Validation status

Static preflight only; no build or tests were run. The fake exercises the post-open body and does not model CRT/file buffering or establish partial bytes. Root should run the focused remesh test, including literal-derived default-byte comparisons, before treating this as validated. Failed-open behavior is unchanged and remains a guarded adapter boundary, not a claim of original returned status.

## Frozen SHA-256

- `src/engine/simcontrol300_remesh.cpp`: `2D02E06BDAF1F01308507D1C033DE0119D8EA43BCA1E6D34A2F5F793B4245FAE`
- `src/engine/simcontrol300_remesh_internal.hpp`: `9B01F3EA0745E2B421D65BD6FB5E3BA0953F29382931DE9731D8B68A7586FDCF`
- `tests/mdt/test_simcontrol300_remesh.cpp`: `A45371490851E4D62CBE195E8FBDE7A579D001F2F46695B6B253B8D72F2E715D`
