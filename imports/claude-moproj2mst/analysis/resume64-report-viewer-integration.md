# CP64 — Report document/viewer ownership versus Project, Session and Close

## Result

The existing `Project` and `ConversionSession` supply plausible lifetimes for parts of Report's native environment, but neither is presently a source-qualified owner for the Report document at `SubProject+0x30` or the Save-time viewer target. The smallest source-backed increment today is therefore **no constructor or Close effect**: retain the current Report Open/Save refusal, and qualify the missing constructor, viewer callee, and cleanup path before adding a live Report owner.

## Candidate owner reconciliation

| Native state | Current candidate | What source establishes | What remains unmatched |
|---|---|---|---|
| Report document shared pointer at SP+0x30 | `SubProject` | Native `0x1400b5ad0` lazily gets/stores a shared object in this slot. Current model already represents per-subproject state and its lifetime. | `SubProject` has no Report document owner field. The native object is created by `0x140643100(raw, mainWindow, type, type != 2, 0)` and parented to the main window. Constructor/vtable effects, field initialization, shared-pointer refcount behavior, and destruction are not recovered for this exact object. |
| Project/main-window 3D display | `Project::display`; `ConversionSession::displayIfCreated()` and `displayLease_` | Current source retains the primary display under the Project and keeps a `displayLease_` across adapter project-storage release. | The Save callee's access to `0x140d0d6f0` is not tied by available function-body evidence to this Display object or its owner. `GuiModel::viewer` is per-model state and is another candidate, not proven equivalent. |
| Normal Close / main-window teardown | `ConversionSession` cleanup and Project destructors | The adapter destructor calls `releaseProjectStorage`, then tears down adapter receiver/dispatcher/runtime owners; comments explicitly say this is not a native normal-Close replay. `SubProject::~SubProject` currently handles only the flow-receiver witness. | No qualified normal Close disposal order for the Report SP+0x30 object, no native parent-destruction ordering, and no evidence that adapter destruction can stand in for accepted Close. |

`Project` contains the subproject vector and the main `Display`; `SubProject` itself has no report document field. `ConversionSession::releaseProjectStorage()` intentionally keeps the display alive through `displayLease_` while dropping the Project. The session destructor explicitly labels this adapter exception/final cleanup, not original Close. These are useful candidate lifetimes, but they do not prove the native Report `QObject`/viewer graph.

## Native order and first downstream states

- Report Open (`0x1412add20`) calls vt+0x78 (`0x1412ae070`) first. That helper obtains the report document via `SP+0x30`, resets/sets its project-derived sibling `.DS` path, then reads/parses the document. Only after vt+0x78 returns does Open display coupon geometry (`0x1412ae3d0`) and fill Report UI (`0x1412b0660`). Current `ReportClass::loadClassData` refuses at the document use, before any of those steps.
- Report Save vt+0x68 (`0x1412adf90`) invokes vt+0x80 (`0x1412aeb30`), then `0x1412af0c0`; only later could the generic operation Save reach Report props vt+0x60 (`0x1412b1f50`, F80/F90/FB0). The captured body of vt+0x80 resolves the subproject Report document and assigns a path derived from the current project's directory and base name. The body of `0x1412af0c0` is absent from retained static captures, so its inputs, model identity, mutations and failure order remain unknown.
- The nearby Open viewer path is not enough to model Save's viewer target. Its retained body uses the operation model and viewer APIs to locate existing geometry files, then updates coupon-related viewer objects; Save's `0x1412af0c0` has only a retained callsite. Do not equate the two based on the `0x140d0d6f0` address noted in the source guard.

## Smallest constructor/Close increment supported now

No live constructor or Close increment is source-supported. In particular, adding a populated `shared_ptr` to `SubProject` would require assuming constructor outputs, main-window parent ownership, and cleanup that are not established. Adding a `Project.display` alias for `0x140d0d6f0` would similarly assume the missing Save callee's receiver/owner. Resetting either candidate during `releaseProjectStorage()` would conflate adapter cleanup with the still-guarded original Close path.

If a structural storage increment is requested later, the bounded design candidate is an **empty, inert** per-`SubProject` Report-document owner slot, with no allocation, path assignment, parsing, native QObject count/refs, or Close action. That would model only the known lazy-slot location and must remain dormant under the public refusal. It is not a Report constructor implementation and does not make the Save path reachable. Before even such a slot is integrated, verify the exact owner lifecycle against the project-member destruction order and `ConversionSession::releaseProjectStorage()`.

## Evidence needed to move past the boundary

1. Recover the exact body of `0x1412af0c0` from the immutable target, including its direct calls, receiver/argument provenance, first mutation, branches and return/failure behavior. This is the smallest missing Save-side viewer query.
2. Recover `0x140643100` and the called QObject base/derived constructor chain for the Report document allocation. Establish its object size/vtable, parent behavior, initialized fields, shared-control block and unwind semantics.
3. Recover the matching Report document destructor/release path and native order among subproject destruction, shared-reference release and main-window parent destruction. The existing main-window/task QObject receipts are for different object instances and cannot substitute.
4. Connect these exact owners to the current `SubProject`, Project display/model and session owner only when types and lifetime ordering match. Then test `.DS` failure partial state, Save callback order and normal Close separately. Keep the F80/F90/FB0 property helper behind both Report-specific Save callbacks.

## Source/evidence references

- Immutable original PE: `imports/claude-moproj2mst/analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
- `evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`: Report `SP+0x30` lazy get/allocate/construct/store order. `FUN_140643100` body is not in the retained function listing.
- `evidence/gui_mo/s1/mdt/L7/s0.c`, `FUN_1412adf90` and `FUN_1412aeb30`: Save vt+0x80 then `0x1412af0c0`, and path assignment through the Report document. No body for `0x1412af0c0` is present.
- `evidence/gui_mo/s1/mdt/L7/d1.c`, `FUN_1412add20` / `FUN_1412ae070`; `evidence/gui_mo/s1/mdt/L7/d2.c`, `FUN_1412ae3d0`: separate Open path and coupon geometry viewer work.
- `source/apps/moproj2mst/include/m2m/model.hpp`: `SubProject`, `Project`, `Project::display`; the former has no Report-document owner field.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp`: project initialization, `ConversionSession::~ConversionSession`, and `releaseProjectStorage`; display lease and explicit adapter-versus-Close qualification.
- `source/apps/moproj2mst/src/model/subproject_lifetime.cpp`: current subproject destruction only records the adapter flow-receiver witness.
- `source/apps/moproj2mst/src/ops/report.cpp`, `src/engine/save_current.cpp`, and `source/docs/re/moproj2mst/mdt/L7/spec.md`: current refusals and qualified slot order.
- Related bounded limits: `analysis/resume51-report-ds-load.md`, `analysis/resume52-report-ds-owner.md`, and `analysis/resume63-report-viewer-owner.md`.

No source, build, GUI, runtime, or mutable REA action was performed.

