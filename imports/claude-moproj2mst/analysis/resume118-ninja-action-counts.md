# CP114–117 Ninja action review

The CP114–115 slow grouped builds include substantially more C++ compilation work than CP116–117. Configure itself stayed near two seconds, so admin/configure tooling does not explain most of the increase. The logs do show the same object targets compiled again across CP114 and CP115, but they do not record Ninja invalidation reasons or per-action elapsed time; a shared-header cause is plausible, not proven.

| Checkpoint/build | Configure | Focused preflight | Full-build command | Preflight actions (compile/link) | Full-build actions (compile/link) |
|---|---:|---:|---:|---:|---:|
| CP114 geometry row | 2.234 s | 9.890 s | 49.344 s | 42 (40 / 2) | 358 (70 / 288) |
| CP115 grouped | 2.157 s | 12.703 s | 55.765 s | 55 (50 / 5) | 371 (83 / 288) |
| CP116 grouped | 2.156 s | 4.766 s | 29.171 s | 15 (11 / 4) | 296 (13 / 283) |
| CP116 final | 2.078 s | 4.172 s | 29.500 s | 10 (8 / 2) | 299 (14 / 285) |
| CP117 close phase | 2.219 s | 1.859 s | 22.828 s | 4 (2 / 2) | 285 (0 / 285) |

The action counts come from `[n/total]` Ninja lines, excluding the `[0/2] Re-checking globbed directories...` line. CP114 and CP115 each link 288 targets in the full build. CP114 compiles 70 object targets; CP115 compiles 83. A path-qualified comparison of `CMakeFiles/<target>.dir/<source>.cpp.obj` finds that all 70 CP114 object identities are present again in CP115, with 13 more objects added. This establishes repeated compilation across those two checkpoint builds, not duplicate compilation within a single build. CP116’s full builds compile only 13–14 objects, and CP117 compiles none, while each still links roughly 283–285 targets.

Focused preflight logs account for 40 and 50 object compiles in CP114/115, predominantly the `m2m_core` target (39 and 46 objects respectively). Full-build logs then compile separate test-target objects and relink the suite. There is no overlap of the same target-qualified object between focused preflight and full build, so these logs do not show the same object compiled twice within a checkpoint. The staged preflight is doing real compile work and is not simply a second execution of the later full-build actions.

The CP114 and CP115 source-tree fingerprints differ, and the build logs do not contain dependency explanations, timestamps for individual actions, CPU utilization, or disk/I/O data. The change from 70 to 83 object compiles may reflect changed source/header dependencies; the logs cannot identify a shared header or separate compile cost from machine contention. CP116–117’s lower compile counts align with shorter full-build phases, but those are later, different source snapshots and therefore not a controlled timing comparison. No target-level elapsed time can be recovered from these logs.

## Recommended measurement

At the next frozen boundary, make a disposable copy of the frozen source/build graph and cache, then capture `ninja -d explain -n` output from that copy alongside the grouped-build receipt. Map each reported dirty object to its dependency explanation and target, compare it with the normal Ninja action list and aggregate full-build time, and record host load. This will show whether shared headers invalidate broad target sets while keeping the candidate cache untouched; it also helps separate compile/action volume from host contention. Make no configuration change until that comparison shows repeatable avoidable work. Keep focused preflight, artifact binding, fixture hashes, and full test coverage unchanged.
