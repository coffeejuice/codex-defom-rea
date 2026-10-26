# CP63 — Report document/viewer owner blocks Report property Save

## Call order and current stop point

The property handler is already known: native `SfOprReportProperties` vt+0x60 (`0x1412b1f50`) calls F80, F90, then FB0. The active blocker is earlier in the operation Save override. Native Report vt+0x68 (`0x1412adf90`) calls its own vt+0x80 (`0x1412aeb30`) and then direct callee `0x1412af0c0`, before the generic base path can reach props vt+0x60. The port keeps an early unsupported guard at this point in `source/apps/moproj2mst/src/engine/save_current.cpp`.

The first predecessor is the Report document owner. Retained `evidence/gui_mo/s1/mdt/L7/s0.c`, `FUN_1412aeb30`, derives a sibling path from the project filename: absolute project directory, separator, cleaned `QFileInfo::baseName`, and a string constant used as the document suffix. It resolves the Report subproject (`FUN_1412b0f70`) and, when present, calls `FUN_140645030(subprojectReportDocument, path, 1)`. The Report subproject document is the lazily created shared object at `SP+0x30`: `FUN_1400b5ad0` in `evidence/gui_mo/s1/mdt/L7/d3.c` reads that owner slot first; on null it allocates `0x70` bytes, obtains the main window, invokes `0x140643100(raw, mainWindow, type, type != 2, 0)`, stores the result in the shared slot, then returns it. This identity is distinct from the DOE TaskFactory's unrelated Report task prototype.

The first required runtime model state for this Save prefix is therefore: `SubProject`-owned Report document identity, its path/DOM/list state and lifetime, plus the project filename used to derive the path. Current `SubProject` in `include/m2m/model.hpp` has no equivalent report-document field. Existing `ReportClass::loadClassData` refuses at the first `.DS` document use, so the normal Open path never creates or populates such a document in the port.

## Viewer predecessor and exact evidence limit

The second direct callee from native Report vt+0x68 is `0x1412af0c0`. The current source guard labels this as the viewer-model branch (`0x140d0d6f0`), and the L7 slot table records the exact `vt+0x68 -> vt+0x80 -> 0x1412af0c0` sequence. The retained `s0.c` capture contains the callsite but no body for `0x1412af0c0`; no independent retained function body for that address was found in the bounded L7 evidence. Thus the viewer operation's arguments, iteration, first mutation, failure behavior, and exact owned model cannot be asserted here.

A nearby, separately retained Report Open path is not a substitute for that Save callee. `FUN_1412add20` calls Report vt+0x78 first, then `FUN_1412ae3d0` to display coupon geometry, then `FUN_1412b0660` to fill Report UI and emit selection notifications. `FUN_1412ae3d0` obtains an engine model and calls viewer-model functions, filtering existing geometry files. This qualifies one Open-time viewer path only; it does not prove that Save's `0x1412af0c0` has the same owner, input set, or side effects.

## Smallest complete helper contract

A complete, isolated Report Save prefix cannot yet be reduced to the F80/F90/FB0 property helper. It must preserve native ordering and hold both Report owners:

1. Resolve or create the subproject's shared Report document owner, preserving its main-window parent and `SP+0x30` lifetime. Reuse the existing owner on repeated calls.
2. Run vt+0x80's project-derived path assignment on that document before any later Report Save effect.
3. Run `0x1412af0c0` against its correctly resolved viewer model, in original order.
4. Only after the Report-specific prefix completes, permit the generic Save path to dispatch the existing Report F80/F90/FB0 helper.

Items 1 and 2 have a qualified path/call skeleton but still lack the `0x140643100` constructor, shared-owner, reset/teardown semantics, and full `0x140645030` effects. Item 3 is blocked on the missing body/disassembly for `0x1412af0c0`. Therefore the smallest safe present action is further bounded static recovery of `0x1412af0c0` plus the constructor/owner path for `0x140643100`; do not implement a placeholder ReportSave helper and do not remove the public Report Open or Save guards.

Acceptance after those prerequisites: a focused native-contract test must verify (a) shared document reuse and project-derived path assignment, (b) the exact Save viewer model selection and ordered effects of `0x1412af0c0`, (c) that Report F80/F90/FB0 runs after both Report-specific callbacks, and (d) failure/partial-state ordering. A direct F80/F90/FB0 unit test alone cannot validate the operation path.

## Bounded retained evidence

- Original PE: `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
- `evidence/gui_mo/s1/mdt/L7/s0.c`, SHA-256 `66149AFCFB363DBF81CD71D86549469D451B5CA2C169DC0CD7EA8A8EF5C2E935`: lines 682–689 establish only the `vt+0x80`, then `0x1412af0c0` call order; lines 711–902 contain vt+0x80 `0x1412aeb30` and its document path/owner calls. The missing `0x1412af0c0` body is an explicit evidence gap.
- `evidence/gui_mo/s1/mdt/L7/d3.c`, function at line 58: Report SP+0x30 shared document lookup, lazy allocation, constructor call and store order.
- `evidence/gui_mo/s1/mdt/L7/d1.c`, functions at lines 463 and 508: Report Open call order and `.DS` loader dispatch.
- `evidence/gui_mo/s1/mdt/L7/d2.c`, function at line 43: Report Open coupon-geometry viewer work, separate from the missing Save callee.
- `evidence/gui_mo/s1/vtables.tsv`, SHA-256 `B5C2274F95BEBD3A0D7F16917937D0974D5F16E1840125F6E314A21787A571F4`: Report vt+0x68 and vt+0x80 targets.
- `source/docs/re/moproj2mst/mdt/L7/spec.md`, sections 2 and 4; `source/apps/moproj2mst/src/ops/report.cpp`; `source/apps/moproj2mst/src/engine/save_current.cpp`: current source qualifications, refusal boundaries and property mapping context.

This audit used retained static PE/decompile/source evidence only. No source, build, GUI, runtime or mutable REA state was changed.
