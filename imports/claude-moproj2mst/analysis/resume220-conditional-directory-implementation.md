# CP220 conditional staged project-directory decision

## Change

The Open-side decision collector now receives the active subproject type and, after the five fixed project directories, appends the cleaned `DieStress` path only when the effective operation ID is positive and the active type is 4. A missing active subproject is represented by `std::nullopt`. The helper remains pure: it only appends path descriptions to the staged vector and does not create directories or populate `FileEffects`.

The parser obtains the active subproject at the existing `<Type>` hook, after operation construction and effective-ID fallback, then appends the collector's results to `Ctx::stagedOperationDirectoryDecisions`. This keeps each operation's decisions contiguous and preserves repeats.

## Verification

The focused helper test covers type 4 (six paths, with `DieStress` sixth), type 0 and no active subproject (five paths), and a negative operation ID with type 4 (no paths). Existing operation-order, path-cleaning, multiplicity, and no-filesystem checks remain. The Open integration test exercises the parser staging position, effective IDs, repeated operations, negative-ID exclusion, partial-parse retention, and absence of filesystem effects.

Built `m2m_model_operation_project_directories` and `m2m_model_staged_operation_directories` in `build/candidate` with the configured MSVC x64 environment. Focused CTest run passed both tests:

```text
2/2 Passed
```

## Limits

The integration fixture's active subproject is type 0 during the operation-list parse, so it validates the parser hook and the five-path Open staging behavior. The type-4 sixth-path branch is validated directly through the pure collector test. These staged records remain inert metadata and do not establish original runtime occurrence or authorize filesystem writes.
