# CP75: DBLIST application-global owner and smallest independent state edge

## Original owner chain

The corrected native DoeOutput callsite is `FUN_140650e60`: it recognizes DBLIST literal `0x1424e3cb0` and calls `FUN_1406554a0(part,node,copiedQString)` (not the adjacent ReportList `0x140655df0`). Its sixth-argument byte must be zero before dispatch. It then calls the gate `0x1402cbec0(0x143d766b8)`, which admits only when the global DWORD at `+0x1c` equals zero. This gate precedes DBLIST parsing and any path/manager processing.

The gate address is an application/process-global state object, not a `SessionPart`, Study, TaskInfo, notifier registry, or conversion argument. The DBLIST handler's existing-file route obtains a main-window pointer from `qword[0x143d766b8+0]`, then the ModelManager from `mainwindow+0x50`, then performs manager child lookup/create by resolved DB path. It may append/register a new child while iterating DBNAME nodes, before its warning tail. The incoming part model is not the persistent owner. The first application-global edge is therefore `global app state -> main window -> main-window ModelManager -> owned model list`; the DBLIST node supplies a path request, not the owner.

## Two independent global fields and lifecycles

| Native field | Qualified role and lifecycle | Unresolved part |
| --- | --- | --- |
| `0x143d766b8 + 0x0c` | Process type. `0x1402cb780` stores a DWORD and `0x1402cb7a0` reads it. The retained `FUN_140013d40` mode-0 refresh writes 5 when no current Project exists or the current Project's native-open-success byte `+0x55` is true; it writes 1 when a Project exists but that byte is false. Native Open calls this refresh after the project-load success state is set. Normal Close reaches a later refresh; no unconditional reset to zero is qualified. DBLIST warning code reads it after the DBNAME loop, and value 5 suppresses the warning only; it does not undo manager effects or turn the false result true. | This is not the `+0x1c` admission predicate and not `MoInputMode`. A candidate Session cannot derive it from its identity. |
| `0x143d766b8 + 0x1c` | DBLIST admission state. `0x1402cbec0` returns true exactly when this DWORD is zero; sibling `0x1402cbf00` tests nonzero. `0x1402cbf30` writes 1. It is in the PE `.data` virtual zero-fill tail, so it starts zero on image load. | No retained caller/receiver lifecycle for the `+0x1c=1` setter, no qualified zeroing setter, and no Open/Close reset are established. Static direct E8/E9 scan found no direct code reference to the setter, but indirect/dynamic references remain possible. Initial-image zero does not establish its value at a later DBLIST dispatch in a long-lived process. |

`analysis/resume66-dblist-global-lifecycle.md` already compares the Close/Open order and reaches the same boundary: process type is refreshed, while the admission DWORD's runtime state transition remains unknown. Do not treat the process-type result 5 as DBLIST admission success, or default `+0x1c` to zero because a candidate conversion created a new Session.

## Candidate owner mismatch and the smallest isolated edge

Candidate `engine::ConversionSession` owns its `ModelManager`; `StudyIo` borrows/binds that same manager for a conversion. The manager stores a `ConversionSession* owner_`, its child list, current child, and signal receiver projections. This is a useful Session-level manager, but it is not the original process-global state object or a main-window-owned manager whose lifetime spans project/session transitions. Its `clear()` is Session/manager teardown. Reusing it as the global owner, or copying its current-model pointer into a global gate, would collapse different lifetimes.

The smallest independently testable state edge is an **application-scope state projection**, separate from `ConversionSession`, with explicit `Unavailable / Known(value)` facts for process type `+0x0c` and DBLIST admission `+0x1c`, plus a stable application-instance identity. The only currently closed update rule is the mode-0 process-type refresh (`5` for null or successful current Project; `1` for present unsuccessful Project). The DBLIST gate must remain unavailable until the setter caller/reset contract and actual application owner are found. A unit test can verify two sequential Sessions share the same app-state identity, the qualified process-type refresh changes only `+0x0c`, and DBLIST admission remains unavailable; no public DBLIST dispatch is needed.

This projection should not own a fabricated `ModelManager` or a Boolean `dbListAllowed`. To reach the first DBLIST state effect later, root must separately provide the real application main-window/ModelManager owner and occurrence-qualified `+0x1c` value, then integrate the existing-manager lookup/ensure behavior. The current candidate `ModelManager::prepareProjectModel` is a fresh-empty-manager path, not DBLIST's add-or-find existing child path.

If no actual application-scope owner can yet be represented, the safe next action is evidence-only: retain `+0x1c` as unknown and keep public B127 guarded. There is no closed state edge that can make the public DBLIST predicate safe today. The process-type-only projection is useful and independent, but it must not be marketed as DBLIST admission.

## Evidence scope

Controlling original SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained reports: `resume62-dblist-owner.md` for corrected `0x1406554a0` path/manager effects; `resume65-dblist-global-owner.md` for helper fields and successful Open process type; `resume66-dblist-global-lifecycle.md` for Close/Open transitions; `wave3-normal-close-guard-contract.json` for process-type refresh. Candidate sources reviewed: `include/m2m/engine/model_manager.hpp`, `src/engine/model_manager.cpp`, `src/engine/conversion_session.cpp`, and `src/core/convert.cpp`.

This is a bounded static/source audit. No source, build, test, GUI, original runtime, or mutable REA database changed or ran. No public Type2/B127 path is enabled by this report.
