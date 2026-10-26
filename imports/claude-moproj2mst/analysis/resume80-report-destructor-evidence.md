# CP80: Report SubProject destructor/release evidence

## Result

The bounded retained native captures do not identify a Report-document release or destructor edge at `SubProject+0x30`. They establish initialization, lazy construction and post-constructor publication only. The available Close evidence reaches a manager-clear boundary but leaves destruction of nonempty Project/child state unresolved. No source-closed Report release behavior can be implemented; keep Report Open/Save guarded and do not add candidate slot cleanup.

## Retained evidence trace

The original L7 decompile `evidence/gui_mo/s1/mdt/L7/d3.c` contains:

- `FUN_1400b5470` (SubProject constructor): `0x140cbc920(param_1 + 0x30, 0)` initializes the field.
- `FUN_1400b5ad0` (Report document accessor): reads `param_1+0x30` using `0x140c1aef0`; on null, allocates `0x70`, gets the main window and calls `0x140643100`; only after the constructor returns does it call `0x140cbc920(param_1+0x30, result)`, then reads the field again.

The separate retained `evidence/gui_mo/s1/mdt/V/v3.c` body for `0x140c1aef0` is just `return *param_1`. This clarifies the read but not the setter's ownership protocol. The bounded retained evidence reviewed here contains no Report slot clear at Close, no release/deleting call tied to `+0x30`, and no Report document destructor callsite.

The independent Close reports do not fill the gap. `analysis/resume74-close-next-owner.md` qualifies manager list/current clearing for a narrow empty-manager branch and expressly leaves every nonempty child/Project/OPI/slot/base/QObject teardown unresolved. `analysis/resume66-close-lifetime.md` examines status-bar/widget and process-global message state, not the Report document. `analysis/resume50-close-destructor-next.md` traces an operation row/OPI destructor path, but supplies no SubProject `+0x30` field or Report document cleanup edge. Candidate `SubProject::~SubProject()` releases only the adapter flow-receiver witness, not a reconstructed Report document.

## REA availability and scope

I checked the current REA binary session without opening a target. It reports `open=false`, `cleanup_pending=false`; advertised providers are only `rea-artifact-graph` and `rea-dotnet-static`. No native PE provider is advertised. Per root's serialization instruction, I did not create/open a native REA analysis session. This report therefore uses only already-retained disassembly/decompilation receipts and current source; the missing edge is not a claim that every original binary path has been exhaustively searched.

## Exact next evidence producer

The next bounded native query should identify the SubProject destructor from its vtable/function table and follow the normal Project Close caller into it. Then resolve `0x140cbc920` for this field, all writes/clears to `SubProject+0x30`, and the last-reference target. Bind that release order against main-window QObject child teardown and the Report document deleting destructor. If the immutable PE dataflow remains indirect, the missing witness is an original same-instance Open/Close trace carrying the Report document pointer and SubProject slot through slot clearing and destruction. Until that edge is available, do not infer that the main-window parent alone owns or destroys the document.

## Evidence identifiers and hashes

- `evidence/gui_mo/s1/mdt/L7/d3.c`: `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`
- `evidence/gui_mo/s1/mdt/V/v3.c`: `b0a5b9df8fa1dc5f65704f32496098e8341b331aa0e27d8a9e32917a46b6a028`
- `analysis/resume74-close-next-owner.md`: `1f47984d52dcbff0a7db6bfdc632fe5c268c216a947e1e7a734284546aefa7d8`
- `analysis/resume66-close-lifetime.md`: `2a88a75e263eb65fe376176a7f5ddd5b94778f909e682c16e95e99cb48bda1b0`
- `analysis/resume50-close-destructor-next.md`: reviewed for its bounded operation/OPI destructor scope; it contains no Report `SP+0x30` Close edge.
- `source/apps/moproj2mst/src/model/subproject_lifetime.cpp`: adapter flow-receiver witness only; SHA-256 `849873b5f1f4c139bb815bc3a92b897c104a9fc38af016178aa853f89a131f64`.

No native REA target was opened. No source, build, test, GUI, database, or runtime action was performed.
