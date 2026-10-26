# Checkpoint 62: DOE sample I/O and Save-order release boundary

## Result

The smallest source-backed public-path slice is the existing Type=2 lazy-study sample initialization on Open. It is already wired into the normal model path: `convert.cpp` activates subproject zero; `activateSubProject` calls `parseSubProject`; the `SfDoeStudy` Type=2 child invokes `readLazyStudy` in `model/subproject.cpp`. That path does not construct the TaskFactory Report prototype. It is a source-level integration, not a fresh-original acceptance result, and its focused tests use an internal activation seam.

There is no additional bounded sample **Save** release ready. The sample writers exist as helpers, but the current public Save path has no call site for `saveCurrentSamples`/`saveSampleTable`, the project writer rejects Type=2/3 study elements, and the active DOE Output Save branch remains guarded before its viewer/document effects. The DOE temporary MST mode is also rejected at the conversion entry.

## Existing Open sample sequence

`readLazyStudy` in `src/doe/activation.cpp` performs this source order:

1. `readStudyElement` updates the study header/name/flags.
2. It reads `Control/DoeInputTable.XML`; a failed open stores the existing error text, while success passes bytes to `readInputTable` and ignores that parser's boolean result.
3. It calls the immediate-I/O `loadSampleTable` for `Samples_LHS.par`, `Samples_FF.par`, and `Samples_USR.par` in mode order 0, 1, 2. Each valid mode writes its mode header plus CRLF before applying the original table-state rule; write results are ignored by the caller, so later modes continue.
4. It reads `Control/DEF_DOE.PAR` last and, on a successful read, parses current mode/sample rows and updates that mode's invalid/count state.

This is an active source call chain through `makeSubProject`/`parseSubProject`, not just an isolated helper. `tests/mdt/test_doe_activation.cpp` covers the fixed order and native-versus-recording file effects, but its activation fixture is explicitly internal rather than full default C ABI Open. The sample I/O contract notes the source qualification is a retained static reconstruction; it is not fresh-original runtime equivalence. Preserve that limitation in any public status claim.

## Why Save is not the next small release

The retained static Save graph has two calls to the current-sample writer. During active Type=2 subproject serialization, original `0x140099040` emits the study element, writes `DoeInputTable.XML`, saves current `DEF_DOE.PAR`, then writes the three per-design sample files. Later the DOE Save stage writes `DEF_DOE.INI`, calls the current-sample writer again, and continues its directory/design work. Their ordering and ignored helper results are part of the caller contract; one isolated sample helper call cannot stand in for the sequence.

Current code does not reach this graph. `src/moproj/write.cpp::subProjectElement` throws for Type=2/3 before appending the DOE study element. `src/core/convert.cpp` rejects `M2M_FLAG_DOE_TEMP`. If the active operation is `SfOprDoeOutput`, `src/engine/save_current.cpp::saveCurrentOperation` throws before the viewer sample-object and output-document effects. In addition, `saveCurrentSamples` and `saveSampleTable` in `doe_study.cpp` are not called by production source. Removing only one guard or wiring only one helper would misstate write order and leave required state such as design tables/manager ownership incomplete.

The separate `0x1400d7a80` empty-table writer during lazy Open (`loadSampleTable` in `sample_io.cpp`) must not be confused with the Save table writers. Likewise, the DOE Output `.DS` writer and TaskFactory's parentless Report `QObject` are separate contracts. None closes the other path.

## Bounded recommendation

- Treat existing Type=2 lazy-study sample Open I/O as the current smallest integration slice. Do not add new sample Save calls in this lane; the live Open–Save entry and its owner/order contract are absent.
- If the next milestone must be a public Save release, recover one coherent existing-study Type=2 Save sequence first: owner state, the two current-sample save positions, three per-design sample outputs, preceding/following `DoeInputTable.XML` and `DEF_DOE.INI` results, and exact partial-failure behavior. It must be reached through the normal converter lifecycle, not by calling helpers directly.
- Keep Type=2 MOPROJ serialization, DOE Output effects, `DOE_TEMP`, and native Report/task initialization guards unchanged until their own dependencies close. No build, tests, GUI, mutable REA work, or source changes were made in this read-only audit.
