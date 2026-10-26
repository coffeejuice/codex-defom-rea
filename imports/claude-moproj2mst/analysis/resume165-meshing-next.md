# CP165 meshing controls: next evidence boundary

## Decision

Do not extend the private meshing helper to additional controls or native record fields from the current evidence. The smallest source-ready decision is already integrated: preserve the four-field temporary projection and retain the guarded stateful selection/Save transition. The next valid work item is an **evidence acquisition for one missing same-owner witness**, not another speculative source cut: bind the actual 0x158-byte temporary row through the native Save copy at `0x14085e050`, then trace whether any other controls or side effects are read/written before the row is applied to the live object. The retained CP129 qualification explicitly says this native helper copies only four doubles and does not reconstruct allocation/destruction or unrelated fields. Therefore no additional meshing property is currently source-ready.

## Evidence and limits

The integrated contract is CP129 (`analysis/resume129-editor-meshing-selection.md`, SHA-256 `ac8a95391b93e0106ad62893303f60d8d67631a590475525edfaf41ce97b7192`) as reconciled by CP130 (`analysis/resume130-editor-meshing-selection.md`, SHA-256 `8c3923210a26b86428bcd165d6ccf3a8d5840422fc0f7bd7a9ad513e535de9bd`). It pins original `DEF_GUI_MO_64 v11.0.2`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, callback dispatch/Fill initialization/Save-current-index order, and the four-scalar temporary owner. Its explicit qualification: native `0x14085e050` copies four doubles; the full native 0x158-byte row, allocation/destruction, and unrelated mesh fields are outside the recovered projection.

Current implementation file `source/apps/moproj2mst/src/engine/simcontrol300.cpp` SHA-256 `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`; focused private regression `source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp` SHA-256 `5ae8085a2ab2bc9e9a96833aec4239132eb72f83eedaf4fe9689b60d53146291`. The test is private page/model coverage, not native QWidget delivery or public Editor300 acceptance. It covers selection commit/reload, same-row reload, Save copy ordering, guards, and floating-point edge cases under its admitted profile.

A separate retained contract `analysis/wave2-save-diffs-300-meshing-contract.md` records that only the selected form receives the four-field text/spin roundtrip; unvisited scalar values retain bits. Neither it nor the private tests establish additional form controls, the full native record lifecycle, or the public editor's actual selection event in a fresh Open-to-Save occurrence.

## Exact missing witness and guard

The missing witness is a source-qualified, same-owner native trace from a fresh editor construction through one activation and Save that records: (1) the temporary row allocation and lifetime; (2) each row byte read/written at activation and Save; (3) the live-object destinations; (4) any other control values or side effects; and (5) normal return/early-return ordering. A static helper excerpt, a separate historical callback capture, or the current four-field C++ projection cannot supply that combined witness. Until it exists, do not broaden `MeshObjectSnapshot`, infer extra controls from row size, or promote the private helper to public-editor parity.

If recovered, the smallest candidate source increment is the first separately proven control/side effect and its owner-bound transition, only when its read/write ordering is independently bounded. If no such effect exists, record that the four-field cut is complete for this helper and continue to the next editor lifecycle gate. Keep the public Editor300 factory/palette guard intact meanwhile.

## Focused checks for any later implementation

Before source changes, require a native byte-range/call-path receipt tying the control or side effect to the same row and live object. Then add owner-bound tests for row lifetime/replacement, selected versus unvisited rows, activation and Save ordering, early-return effects, and preservation of unrelated row bytes. Keep tests Release-active with Testkit macros and scratch beneath `M2M_TEST_SCRATCH`; preflight quoted includes and avoid C `assert`. Integration must run focused targets with the full source-stable launcher/freeze boundary; this static lane ran no tests or builds.

## Scope record

Read-only converter investigation only. This report is the sole write. No source, tests, shared plan/state, fixture, build, executable, GUI, app control, live REA, or mutable analysis database was changed or run. No behavior beyond the cited source-qualified evidence is claimed.
