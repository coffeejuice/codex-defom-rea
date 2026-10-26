# Save I/O fixture copy options and benchmark plan

## Fixture shape

The inspected `133_B_DBSTEP_BOOL3_before` tree has 64 regular files totaling 10,619,687 bytes. A single `COG_P1_B1.DB` is 9,216,000 bytes (86.7% of payload). Seven files are 64 KiB–1 MiB (1,291,292 bytes total); six are 4–64 KiB (85,329 bytes); the remaining 50 are under 4 KiB (27,066 bytes). Thus both the dominant database payload and per-file traversal/open overhead matter. These figures came from directory metadata only; file contents were not read for this inspection.

## Copy mechanisms

The current call is `std::filesystem::copy(input, root, copy_options::recursive)`. Microsoft documents the recursive operation as creating destination directories and recursively dispatching entries; regular files are delegated to `copy_file`. That keeps traversal and filesystem semantics in the standard library while still doing one file-level copy per regular file. The available profile’s phase totals cannot tell whether traversal, opening many small files, the 9 MiB database, directory creation, or storage/cache state dominates.

A custom `recursive_directory_iterator` plus `CopyFile2` or `CopyFileExW` would still walk the tree and copy every file. Those Win32 calls are per-file APIs, not recursive tree-copy APIs. They may be useful if a focused benchmark shows the standard library’s per-file path is costly, but they add Windows-specific error handling and require explicit decisions about directories, symlinks, timestamps, attributes, alternate streams, and cleanup after partial failure. `CopyFile2` documents preservation of extended attributes, OLE structured storage, NTFS alternate streams, and file attributes, while stating security attributes are not copied. `CopyFileExW` has callback/cancellation support and documents closely related metadata behavior. Neither API therefore establishes equivalence to the full `std::filesystem::copy` tree operation on its own. [Microsoft: `std::filesystem::copy`](https://learn.microsoft.com/en-us/cpp/standard-library/filesystem-functions?view=msvc-170), [Microsoft: `CopyFile2`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfile2), [Microsoft: `CopyFileExW`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfileexw).

## Bounded benchmark plan

Run after the active hash benchmark is finished so both jobs do not contend for the same disk. Build one small local benchmark harness, outside the test binary, with three variants: (A) current recursive `std::filesystem::copy`; (B) explicit recursive traversal using `std::filesystem::copy_file`; (C) the same traversal using `CopyFile2` (or `CopyFileExW` only if the configured SDK/runtime requires it). Copy into a fresh destination on the same volume as the test scratch directory. Do not write to the source fixture.

1. Do one unmeasured warm-up per variant, then five measured repetitions per variant. Rotate variant order each round (ABC, BCA, CAB, then repeat) to limit ordering bias. This is a warm-cache microbenchmark; report medians and ranges, and do not label it cold-cache performance.
2. Measure whole-tree elapsed time, per-file elapsed time and bytes for the explicit variants, plus directory creation and enumeration time. Record process/OS build, volume, fixture path, destination path, and whether the run shared the machine with other disk work.
3. Compare output inventory and content hashes for every file. Also compare relative paths, object type/symlink target, file attributes, last-write timestamps within filesystem precision, and any alternate data streams present. Check directory attributes/timestamps if tests rely on them. Use metadata inspection and hashing only on the disposable copies; preserve the evidence fixture.
4. Reject any candidate with missing/extra files, mismatched bytes, changed relevant metadata, or less than a repeatable improvement beyond run-to-run spread. If the per-file data shows the 9 MiB DB dominates, separately test only that file using the same five-run rotation before changing the whole-tree implementation. If tiny-file/open overhead dominates, focus on traversal/metadata calls rather than assuming a different bulk-copy API helps.
5. Only after the candidate passes equivalence and timing review, integrate it in the test source and let the root integrator build and run the focused test plus required coverage checks.

No benchmark, build, test, source-tree write, or app-control operation was performed for this report.
