# CP123 — CP122 four-lane acceptance review

## Determination

CP122 is a source-stable candidate build with focused coverage for the four assigned lanes. The receipts establish the stated private effects and guards, not native lifecycle completion or parity. I found no contradiction between the lane reports and the implementation summaries, but the ShapeOptim failure route remains the clearest material gap: the candidate records a failed mkdir and stops at the modal boundary, while the retained original proceeds after its one-OK dialog. The named-DOE path is also still blocked because the study-name owner is unresolved.

## Build, tests, and ABI boundary

The controlling validation is `analysis/wave3-checkpoint122-validation.json`, bound to `analysis/resume122-registry-repair-build.json` and `analysis/resume122-registry-repair-tests.json`. The build reports source tree SHA-256 `ead1572502ec4844c977bb33ecfcfc5aa3de1ace5d8a0824f15980d6ededb913` before and after, and `source_stable_during_build: true`. The test receipt completed with CTest exit 0 and counts 303 ordinary passes, 6 passes with logged gaps, 1 pass with a logged difference, 0 incomplete outputs, 0 skips, and 0 failures. Thus “all green” would be misleading: the run completed with recorded gaps and the overall objective remains incomplete. Historical comparison remains 93 SAME / 5 DIFF, with A55, B120, B121, B122, and B98 still DIFF.

The four focused targets each passed with no per-target gap or diagnostic-difference event: operation-info cleanup 14/14, ShapeOptim study-folder effect 4/4, SPRING reader 2/2, and Project feature storage epoch 7/7. `m2m.p11.xcheck` also passed. These are bounded candidate tests; their pass status does not upgrade the explicit original-runtime qualifications below.

Read-only PE export receipts identify CP122 DLL `build/wave3-checkpoint122-verified/moproj2mst.dll` SHA-256 `628D97EFAE06C8EFF230B07F2CD24BEF448DCF1A6D6B43C7CFBD27AE1BDFE5B9` and CP120 baseline DLL `build/wave3-checkpoint120-verified/moproj2mst.dll` SHA-256 `2722E81D828D8B9BDAC87F83325C8E33CA4DD0042AACE45BF1EBF52B871514B8`. The retained DUMPBIN comparison reports the same six exports in both: ordinals 1–6 / hints 0–5, names `m2m_api_version`, `m2m_convert_utf8`, `m2m_convert_w`, `m2m_set_codepage`, `m2m_set_deform_dir_w`, `m2m_set_gui_unit`; RVAs are respectively `0x3360`, `0x3370`, `0x38D0`, `0x3C50`, `0x3C80`, `0x3DF0`. This is an export-table compatibility check, not proof of behavioral ABI parity or equivalence of all DLL contents.

## Lane findings

### ShapeOptim `mkpath`

The retained native evidence in `analysis/resume122-shapeoptim-mkpath-original.md` qualifies original `DEF_GUI_MO_64.exe` by SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Native `loadData` calls `studyFolder` before its later ROI geometry-list guard. The `mkpath` helper returns true on both success and failure; on failure with this caller’s dialog flag, it executes a one-OK warning and then continues. The actual dialog event/closure was not observed.

Candidate `shapeoptim_load.cpp` attempts and records the immediate directory creation before the later guard when the native Save/Open profile is active. On failure it throws at the unqualified modal boundary, so it does not preserve the native post-dialog continuation. Its real-class test proves the reached successful mkdir; the later-guard persistence case uses a simulated guard, because no source-qualified producer for populated `listF0_` is available. Further, candidate `studyFolder` refuses when a DOE study exists and the name is required, so these tests do not establish the named-study path. This is an explicit guarded frontier, not a false claim of native continuation.

### Project passive epoch

The CP122 feature-storage report and implementation use a typed host-owned bundle behind the existing Project marker shared control block. Weak epoch lifetime and existing Project member ordering remain; the change is passive storage and diagnostics only. No native heap/proxy producer, feature value, registration, native cleanup, constructor completion, or public Close admission is demonstrated. The seven passing cases check host-side ownership, foreign/wrong marker rejection, allocation-exception cleanup, Project scope, retained registered model/slots, and address reuse. They are not evidence that the original CRT heap or unresolved native retirement behavior has been reconstructed.

### OPI cleanup guard

The staged cleanup prefix follows the source-qualified null/nonnull distinction: null Props takes the native skip edge; nonnull Props is stopped before unknown virtual deletion. The separate engine-cache step checks the actual `Operation.engineModel` owner (not borrowed class-model state); null cache reaches the next cleanup boundary without deletion or pointer clearing, and nonnull cache stops before unknown virtual delete. The 14 cases exercise these guards and state identity. No actual native Operation destructor invocation, payload destruction, QObject boundary, or complete cleanup is claimed. The staged candidate limits are consistent with the retained source contract.

### SPRING2 comma parsing

The two passing reader cases include count-sensitive `1,234` behavior: parse prefix 1, retain the comma cursor, then produce zero for subsequent requested fields (`[1]`, `[1,0]`, `[1,0,0]` for counts one through three). The implementation state-gates only this qualified ASCII comma case and rejects comma-leading/repeated malformed forms. The retained evidence supports the C-locale stream behavior described in `analysis/resume122-spring-stream-next.md`; no fresh original-process comparison occurred. It does not establish general locale grouping, arbitrary punctuation, sticky QTextStream status, or unqualified malformed-number behavior.

## Next executable comparison

The best next source-closed comparison is to add an isolated, retained-original-backed SPRING2 comma fixture comparison for the bounded `1,234` count-one/two/three outputs, if a retained original output exists. First check the retained artifacts for those exact inputs and output bytes; if none exists, capture or derive an original output only through the approved original-evidence route. Do not reinterpret the candidate’s focused pass as that witness. In parallel, the next ShapeOptim implementation dependency is source qualification of the study-name owner and the populated `listF0_` producer; until those exist, keep named-study and real-class post-mkdir failure continuation guarded.

No source, build, GUI, original process, or mutable REA state was changed or used in this review. The DUMPBIN executable was not available on PATH during this review attempt; the export details above are carried forward from the retained CP122 ABI audit receipt and should be re-read from its raw capture if this report is used as a release sign-off.
