# Folder sweep phase profile

Set `M2M_FOLDER_PROFILE` to any value alongside the existing `M2M_FOLDER_SWEEP` and `M2M_CORPUS` settings to emit one additional TSV row for each case that reaches the copy step. With the variable absent, the existing output rows and diagnostic text are unchanged. Cases rejected before `runCase` copies anything emit no profile row.

Each profile row has exactly 11 tab-separated fields, no header:

| Column | Meaning |
| --- | --- |
| 1 | Literal `M2M_FOLDER_PROFILE` |
| 2 | Case name, such as `A:RS_11_THEXP` or `B:R11` |
| 3 | Status: `compared`, `refused`, or `copy_failed` |
| 4 | `copy_us`: microseconds for removing the unique work folder, recursive copy, and, when profiling, counting regular input files and their sizes |
| 5 | `convert_us`: microseconds for the single `m2m::convert` call |
| 6 | `compare_us`: microseconds for file enumeration, reads, masking, relocation, and comparison classification; excludes printed diagnostics |
| 7 | `cleanup_us`: microseconds for the existing `remove_all(work)` call after conversion |
| 8 | `copy_files`: regular input files included in the copy census, including files ignored by comparison |
| 9 | `copy_bytes`: sum of sizes for those files when `file_size` succeeds |
| 10 | `compared_files`: expected/result file pairs actually read |
| 11 | `read_bytes`: sum of byte lengths of both files in those pairs |

Durations use `std::chrono::steady_clock` and integer microseconds. A status can have zero durations or counters for phases it did not reach. `copy_failed` retains the existing skip behavior, so its cleanup value is zero. The copy census adds filesystem reads and therefore profile timings should be used for bottleneck ranking rather than as an exact measure of an unprofiled run.

For a root-owned build and sweep, check equivalence by running the same executable once with `M2M_FOLDER_PROFILE` unset and once set, with unchanged corpus and oracle paths. Remove only lines beginning `M2M_FOLDER_PROFILE\t` from the profiled stdout, then compare the two stdout streams byte for byte, including `SKIP`, `REFUSED`, `SAME`, `DIFF`, `NOTE`, detail, and summary rows. Check equal exit codes and equal summary tallies. For each profiled case, require one profile row; check nonnegative durations and counters, and require zero conversion/comparison counters after `copy_failed`, zero comparison counters after `refused`. Existing cleanup behavior and comparison classifications should be unchanged. This checkpoint copy has not been built or run here; the root worker owns builds and sweep validation.
