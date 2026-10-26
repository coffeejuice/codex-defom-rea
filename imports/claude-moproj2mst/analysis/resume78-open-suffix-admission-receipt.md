# CP78 full-Save suffix admission receipt

- **Changed source:** `source/apps/moproj2mst/src/core/convert.cpp`
- **SHA-256:** `1FD1592C8BD8623E2D4736AF077CDEF4CD1F530227D87059721F4DBE205F6A30`
- **Ordering:** Relative-path normalization is at `convert.cpp:185-186`; the `M2M_FLAG_WRITE_SAVE`-only suffix guard is at `:190-194`; Session construction begins at `:196-198`; `prepareProjectModel()` is at `:204`. The guard admits `.moproj`, `.DB`, and `.KEY`; other suffixes throw before candidate manager receipt/model construction.
- **Evidence:** `evidence/gui_mo/s1/s1-01/dc/sub0.c` (`FUN_14001ca30`) for native Open dispatcher suffix/admission ordering; `evidence/gui_mo/s1/s1-01/dc/open0.c` (`FUN_14008a9b0`) and `source/apps/moproj2mst/src/moproj/read.cpp:29-57` for accepted suffixes and normalization.
- **Public result:** Unsupported full-Save suffixes retain `M2M_E_PARSE` with `Error on reading project file!`. `.DB`/`.KEY` still proceed to their existing guarded unsupported path. The non-full-Save `checkReadable()` preflight is unchanged.
- **Static preflight:** Source order inspected; trailing-whitespace scan found none. No build or tests run. This increment changed only `convert.cpp`.
