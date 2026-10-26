# CP76: keep the native remesh file on the stack

## Current allocation change

CP75's default `saveRemeshAtProjectSave` constructs a `std::function` factory and calls it. The factory uses `std::make_unique<NativeSaveFile>`, and that wrapper contains `CoggingFile` by value. Although `CoggingFile` itself still owns its handle, `FILE*`, and 4 KiB buffer as before, normal Save now adds a guaranteed heap allocation for the wrapper and factory/type-erasure overhead. The original candidate path constructed `CoggingFile` directly on the stack at the Save point.

## Minimal private redesign

Keep the CP75 virtual `RemeshSaveFile` interface and its deterministic fake, but remove the factory from the production call path. Split the implementation into:

1. Existing `saveRemeshAtProjectSave` validation and branching: validate the same ordinary directory, handle disabled `DeleteFileW`, verify known C numeric locale, construct `datafiles::detail::CoggingFile nativeFile(path)` directly on the stack, and preserve the current failed-open unsupported/invalid-parameter guard.
2. A private stack `NativeRemeshSaveFile` adapter that holds `CoggingFile&` and delegates `writeV` to `def::rt::crt::vfprintf`; its `close` delegates to `nativeFile.close()` and reports no checked status. Instantiate it on the stack after successful open.
3. One shared private `writeRemeshSaveBody(state, RemeshSaveFile&)` routine containing the unchanged 31 formatting attempts and ignored close result. Call it with the stack adapter from public Save and with a stack fake from the focused unit test.

The narrow private header can expose only the abstract operation interface and `writeRemeshSaveBody` for tests. Remove `RemeshSaveFileFactory`, `std::function`, `unique_ptr` factory creation, and the public-path call to `saveRemeshAtProjectSaveWithFactory`. The fake test then calls the body directly after its own successful-open precondition. It still observes a failing write followed by the remaining ordered writes and a failed Close return, while making no backing file and no residue claim. No public `include/m2m` or shared converter API changes are needed.

This gives the normal path its prior object-storage shape: stack `CoggingFile` and stack adapter, same underlying native file open, same 4 KiB buffer, same `vfprintf` formatter, same call order, and same ignored write/close outcomes. Formatting may internally allocate temporary strings as it already does through the legacy formatter; this redesign removes the extra wrapper/factory allocation and does not promise allocation-free formatting.

## Failure caveats

- Do not add an explicit flush operation. The qualified original writer has no separate flush call; `fclose` is the flush/close boundary and its result is ignored.
- The injected fake verifies orchestration only. It cannot establish how many bytes a real CRT buffer or filesystem leaves after a failed write/close.
- Preserve the existing failed-open guard. The original null-`FILE` path reaches the CRT invalid-parameter dispatcher and may terminate under the qualified default handler; the adapter must not turn it into the post-open ignored-error case.
- Ensure the default adapter does not call `fclose` twice. `CoggingFile::close()` nulls its member, so explicit close followed by stack destruction remains safe.
- If the existing `vfprintf` wrapper or formatter can throw, stack unwinding still destroys `CoggingFile`; this is adapter cleanup, not proof of original GUI termination behavior for resource exhaustion.

Static-only review of CP75's write path and `analysis/wave3-remesh-save-lifecycle-contract.json`; no source edits, builds, tests, app control, or mutable REA actions were performed.
