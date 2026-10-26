# CP112 — default-engine and DBLIST boundary

## Result

There is no source-closed DBLIST admission transition to add or test at the public parser boundary. The smallest still-missing item is native evidence binding the `0x1402cbf30` setter call's receiver to the gate object at `0x143d766b8`, followed by a same-instance write/reset lifecycle. CP108's CRT initializer binding does not provide that evidence: it identifies the pre-`main` AppState constructor occurrence, while its retained field prefix does not establish a `+0x1c` store.

The candidate already has a conservative contract: `ApplicationStateProjection::dbListAdmission()` defaults to `Unavailable` and exposes no mutation API, while `importPart` rejects direct `DBLIST` and `ReportList` children. A new helper/test that supplies a guessed fresh-zero/default-engine fact would only restate a fabricated assumption; a test of `Unavailable` is already present and would add no discrimination.

## Source and native boundary

The original DoeOutput path evaluates the application-global DWORD at `0x143d766b8+0x1c` before calling the DBLIST handler; admission occurs only when it is zero. The known helper `0x1402cbf30` writes one through its receiver-relative `+0x1c`, but its caller and receiver occurrence are not closed. Direct-call/pointer scans are qualified as incomplete for indirect calls, aliases, and virtual dispatch. Fresh-image zero-fill and the CRT constructor's known resets establish earlier startup facts only. The separate `+0x0c` process-type refresh is used for missing-file warning suppression and does not decide admission.

Current source keeps these boundaries explicit:

* `source/apps/moproj2mst/src/doe/application_state_projection.hpp` documents DBLIST admission as unavailable; the corresponding projection test confirms it remains unknown after the modeled process-type transition.
* `source/apps/moproj2mst/src/doe/session_records.cpp` refuses DBLIST/ReportList rather than entering an unqualified handler.
* `source/apps/moproj2mst/src/core/convert.cpp::run` uses a function-static `engineMutex` to serialize conversions because the DEF_PRE engine/Fortran units/KEY reader/model globals are process-wide. This is engine-call serialization, not an application-state producer: it creates no native AppState/main owner, does not initialize `+0x1c`, and cannot imply a reset between calls.

No other source-backed shared-engine edge in this review supplies the missing DBLIST owner or admission value. The mutex behavior is already an explicit source contract; expanding it would not advance the native gate qualification.

## Exact next executable action

After the retained-analysis database is available to its owner, perform a bounded original-PE xref/dataflow pass for setter `0x1402cbf30` and global `0x143d766b8`, including indirect/vtable call sites and stores through aliases. Accept a result only when a setter occurrence has `rcx` proven to be the gate object and the same object's relevant reset/write order is sufficient to determine its value at later DoeOutput import. If static analysis cannot establish that, the remaining witness is a same-instance original application trace recording caller, receiver identity, and `+0x1c` across startup, Open, DBLIST dispatch, Close, and subsequent Open.

Until then, keep the public refusal and projection unavailable. No helper or test patch is justified by the current facts.

## Evidence scope

Read-only reconciliation of current source and retained static reports. Original PE identity: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Controlling receipts include `analysis/resume108-dblist-admission-writer.md`, `analysis/resume102-dblist-admission-beyond-cp98.md`, and `analysis/resume81-dblist-setter-pe.md`. No source, build, test, GUI, original execution, or mutable REA database was changed or run.
