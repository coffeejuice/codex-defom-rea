# CP214: Lubricants directory creator during Open

## Finding

The CP213 fresh valid encoded Open produced one new entry in the disposable project: an empty `Lubricants` directory. The original source fixture remained unchanged. The retained observation reached a normal nonzero outer Open return, but did not observe postOpen, activation, Save, or Close. The directory is an Open side effect in this occurrence; its manifest does not identify which call created it.

The strongest static candidate is original function `0x14008c630`. Its retained DUMPBIN range is `0x14008c630..0x14008c7e0` in `analysis/wave3-earlier-save-14008c630.asm`, captured from original installation v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (PE version resource null). The function makes five successive path values from project-manager helpers and passes each to `0x14081afb0`; the fifth helper is `0x1400a48a0`, statically identified as the `Lubricants` project subdirectory. The function then conditionally creates `DieStress` for the relevant active subproject. Existing qualification says `0x14081afb0` calls QDir `setPath` and `mkpath`; the default-OK modal on failure does not change the caller's continuation.

The retained original spec connects this creator to operation initialization: `0x140050820` sets an operation ID and, for IDs >= 1, calls `0x140051190`; that helper creates the task directory and calls `0x14008c630(P)`. The Open-side static route is `0x14008a9b0` OpenProject -> `0x14008afa0` ReadProjectFile -> `0x14008b410` WalkProject -> Operation_List `0x14008cb50` -> operation parse/factory -> operation-info initialization -> `0x140050820`. The exact relevant source descriptions are in `source/docs/re/moproj2mst/s1-01/spec.md` sections 1–5.3; the function ledger labels `0x14008c630` “mkpath project subdirs”.

This is a statically supported Open-reachable path and a plausible explanation for the CP213 directory. It is not occurrence-level proof that this path ran in CP213: the six-hook capture did not include `0x140050820`, `0x140051190`, `0x14008c630`, or the actual QDir call. The manifest only establishes the side effect and timing window.

## Candidate source comparison

The candidate's `source/apps/moproj2mst/src/model/opinfo.cpp` initializes operation info through `initOperation`. `source/apps/moproj2mst/src/model/operation_rescue.cpp` implements `setOperationId`: it sets the ID and `Operations/TaskNNNNN` relative path, but explicitly comments that `0x140051190`'s task/project mkpath is “not done.” Thus the candidate currently omits the original side effect at the operation-ID setter. The separate `engine::makeFolders` in `source/apps/moproj2mst/src/engine/save_current.cpp` is used by Save and is not evidence that candidate Open creates these folders.

The candidate parse route may still create directories through another path (for example while materializing embedded item blobs). Its source routes must not be conflated with the original's static call chain. CP213's isolated new empty `Lubricants` directory specifically makes the operation-ID/project-directory route a strong lead, but it does not rule out another Open caller of the same QDir helper or directory function.

## Narrow next witness

Use a separately reviewed, source-pinned valid encoded Open on a new disposable project copy, with passive observations for only the following boundaries:

1. Entry/caller of `0x140050820` with the operation ID and thread identity.
2. Entry to `0x140051190` and `0x14008c630`, retaining their project pointer, ordered calls, and return.
3. The five `0x14081afb0` call arguments from `0x14008c630`, especially the fifth `Lubricants` path.

Correlate the call sequence with before/after directory manifests and the already-qualified outer Open boundary. This distinguishes “path is reachable in Open” from “this occurrence caused the directory.” Do not use Save, GUI input, or a second original run until the runner and host release predicates are independently reviewed; this report used retained static/runtime artifacts only and did not acquire app control or alter a REA database.

If that witness confirms the chain, the safe source increment is to represent the original `0x140050820 -> 0x140051190 -> 0x14008c630` effects on the candidate Open operation-ID path, using the existing effect/capture boundary rather than an unconditional directory write. Preserve call order, exact paths, ignored native return behavior, and directory failure notification semantics as separately qualified. First determine how the current `setOperationId` API receives project/file-effect context; do not add global filesystem writes or put the side effect only in Save. Until then, leave source unchanged and record this as an Open fidelity gap with causal attribution unresolved.
