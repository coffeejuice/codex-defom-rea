# CP165 non-DOE operation/property dispatch and default-dialog boundary

## Decision

There is no new source-ready public operation/property dispatch arm in the bounded retained evidence. Preserve the `propsToEngine` unknown-class refusal and the Report active-Save refusal. The smallest useful **owner evidence cut** is Report Open's publication of the actual document at the selected `SubProject+0x30`, followed by its same-instance reads and first payload/model producers. Only after that owner is represented can Report's existing property callback and later Save/Close calls be tested as one caller path. An isolated call to the three SimCtrl property helpers does not establish that path.

The nearest non-DOE default-dialog dependency is the must-be-last Save warning. Its static Yes result mapping is conditional on an actual answer and event interval. The public `lastOperationCheck` refusal must remain. The separate failed ShapeOptim `mkpath` path has a one-OK modal whose result is ignored and whose native helper returns true; its event interval and return occurrence are also unobserved. The split-DB Open default-Yes branch is already represented in source and has a synthetic caller test, with its modal event interval explicitly outside that test's claim.

This is a converter-only read-only reconciliation. Original target: `DEF_GUI_MO_64.exe` v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The native facts below are conditional static PE/decompile findings, not a fresh original Open→Save occurrence. No reconstructed executable or deployment was inspected; no reconstructed version is asserted.

## Reconciled dispatch status

| Branch | Current status and source qualification |
| --- | --- |
| Base active Save and property `vt+0x60`; step-4 property `vt+0xb8` | Candidate has the bounded base order and the Cogging, Boolean, L6 and Express step-4 families (`resume117-reachable-operation-dispatch.md`; `save_current.cpp`; `convert.cpp`). Do not add a generic no-op for unknown classes. |
| Distortion `vt+0x198` default contacts | **Already integrated.** CP47, CP88 and CP154 say the original body is missing, but CP48 subsequently recovered the complete `0x141364140..0x1413642c4` body from the verified PE. Current `classDefaultPairs` routes Distortion2D/3D to `distortionDefaultPairs`; the helper uses ordered fixtures and workpiece from Distortion pass data, with guarded malformed reconstruction states. `test_distortion_contacts.cpp` contains direct and synthetic Save-path cases. These tests are not fresh-original full-project parity. |
| Report property `vt+0x60` | Retained `0x1412b1f50` maps to F80/F90/FB0 and the candidate already recognizes Report. Active Report Save stops earlier at class `vt+0x68`, and Report Open stops at the document accessor and later viewer state. Thus this mapping is not an omitted public dispatch arm (`resume164-dialog-next.md`). |
| Report class Save `vt+0x68` | Native `0x1412adf90` needs the document `SubProject+0x30`, its `.DS` writer and viewer model. Candidate `report_document_slot.cpp` is a diagnostic null/non-null projection, not an owning producer; `report.cpp` and `save_current.cpp` retain guards (`resume164-close-next.md`). |
| ShapeOptim failed study-folder `mkpath` | Original shared helper `0x14081afb0` attempts `QDir::mkpath`, shows one-OK modal on failure when dialogs enabled, ignores its result, and returns true. Candidate records the attempt and refuses before modal continuation. This is a distinct Open/load boundary, not a property dispatch default (`resume122-shapeoptim-mkpath-original.md`). |

## Exact missing witnesses and guard

**Report owner, first:** statically trace the Report Open call from `0x1412ae070` through the current Project/SubProject lookup, `0x1400b5ad0` document construction, and publication to that same `SUB+0x30`. Bind the accepted QObject parent, document generation, current `+0x20` model, `+0x30` interactive payload, `+0x40` parts and `+0x48` DOM owner as each is produced. Preserve subsequent rebind/destruction and the current slot values before Report Save's document and viewer calls. CP164 Close identifies the same producer and double-reread cleanup; a constructor default or detached `KnownNonNull` observation cannot substitute for its lifetime. This static owner cut can proceed without app control. Source should first model the producer and owning SubProject; only then extend Save or Close.

**Report occurrence, later:** on one copied valid Report project in a fresh original process, retain matching Project/SUB/document generations from Open through Save, the actual default selections and delivered callbacks of any reached dialogs, Report `vt+0x60` and `vt+0x68` entry/normal-return order, `.DS`/viewer effects, and the Save tail. This is required for a public successful-path claim. If no valid Report fixture reaches publication, retain the current refusal and identify that fixture or earlier Open dependency precisely.

**Must-be-last warning:** on one original Save invocation reaching `0x1400b8fb0`, retain timer connection/readiness and delivery identities separately for Save's `processEvents(0)` poll and the warning's show, nested loop, completion and unwind; record actual activating input, `done(int)`, mapped answer, callbacks through Save return and writer-entry coverage. The static offered Yes (`0x4000`) with unavailable requested No default supports a conditional early-return rule, but it does not prove that Yes occurred or that the modal interval was empty (`resume160-dialog-default-next.md`). Keep `save_project.cpp::lastOperationCheck` guarded; its `pendingDefault=Yes` is not a returned answer. The current `test_save_answer.cpp` asserts that distinction.

The independent GUI validation status currently reports `release_ready=false`: the supported Windows control surface provides no verified release operation/receipt. Under the parent recovery rule, do not start a GUI-control capture to obtain either runtime witness until that capability is established. No GUI-control session was acquired here.

## Tests for the future owner cut

After the Report producer is source-bound, test same-SUB publication and generation-specific current rereads, null and nonnull document branches, payload/model production, and ordered Report Save entry under its actual active operation. Preserve the generic unknown-class and existing Report refusal tests until the owner and caller route are admitted. For the warning, first use a private validated-result/profile seam to test conditional result mapping and early return; require an original occurrence before allowing public continuation. A candidate-only helper test or a synthetic split-DB default test is not original modal or full-lifecycle acceptance.

No build or test was run for this report. Existing CP48 Distortion tests are source coverage, and the CP164 Report/dialog reports provide guard evidence, not a new green gate.

## Read receipts and limits

All hashes below were recomputed from this checkout. Paths are relative to this converter root.

| Artifact | SHA-256 |
| --- | --- |
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `analysis/resume48-distortion-vt198-rea.json` | `f02654480fe2be5ab2a9c9e99e13ed19e1ea502c55054d3ac8245f9168e8e571` |
| `analysis/resume164-dialog-next.md` | `549e17c9e698f88e699d27270aa3818a182ca775cef243fd7378fb4e52439156` |
| `analysis/resume164-close-next.md` | `13308e5ba842a4cfcc4399bbb60969ad4761c5eb0f06c9852d23a740f6b5352f` |
| `analysis/resume160-dialog-default-next.md` | `282c61d24d7e936f4f56293db9f6a4fb6549eed238643cebafaaa6607021630b` |
| `analysis/resume122-shapeoptim-mkpath-original.md` | `2d201ce1158b6c0197eab9ecde7db01884527258cb2cd61066485f2d83ff945e` |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129` |
| `source/apps/moproj2mst/src/engine/distortion_contacts.cpp` | `20b70b643998a6e83a63750d8bd00fe48d8f06c70c8473aabd8dc2b5ffda685c` |
| `source/apps/moproj2mst/src/engine/save_project.cpp` | `90a0ddbf6b4c92b4f4fe8bc106aa2e9248f31c84d2a0981d0ee10eda0baf2337` |
| `source/apps/moproj2mst/src/engine/shapeoptim_load.cpp` | `1428fd84e341ccba66d1203243e1836a32a0292ffa44898d927378faadcb1367` |
| `source/apps/moproj2mst/src/ops/report.cpp` | `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a` |
| `source/apps/moproj2mst/src/engine/report_document_slot.cpp` | `1f004f5f797fc0abeed34b0a90a0a58059c7763e9ea61576eaa0a7c4fb7251b4` |
| `source/apps/moproj2mst/tests/s/test_save_answer.cpp` | `551ddc47ef883897dad01717b2ad768ab5a0cc37b307cc72e68c47487829479f` |
| `source/apps/moproj2mst/tests/s/test_distortion_contacts.cpp` | `e3e7f8225dbd23d2e800e34aca0ff8b8fa0bf63766033e39ef2b8e94e2855d3f` |

No source, tests, shared plan/state, build, GUI, live REA, or original executable was changed or run. Retained static reports have selected byte/call-path coverage only; absence of a selected occurrence is not proof that the original branch never runs.
