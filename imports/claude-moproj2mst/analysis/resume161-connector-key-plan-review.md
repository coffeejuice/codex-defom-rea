# CP161 independent review of ReportGenerate metadata-key proposal

## Disposition

The exact static key is established, but the initializer is **not source-ready for wiring into the candidate’s actual reachable construction path yet**. The candidate currently refuses before constructing prototype one, before a TaskInfo object exists, and well before the later `SfTaskReportGenerate` prototype is reached. Setting `SfTaskReportGenerate` in the factory maps or manufacturing a TaskInfo/name record ahead of that path would pre-seed state before the native constructor/registration effects and would not be a faithful metadata initializer.

A narrow source value fact can be preserved for a future class-specific TaskInfo construction step: once the actual ReportGenerate instance has passed its native prerequisites and its TaskInfo storage is constructed, its metadata name is `SfTaskReportGenerate`. That does not authorize adding an early map entry, returning a synthetic task from the getter, or opening DBLIST/ReportList.

## What CP160 establishes

Proposal hash rechecked: `analysis/resume160-connector-key-next.md`, SHA-256 `6EF0E150C04C3EED77909140BA8BDB6A147F70E1D5AB26F6134F16AEC0880C7E`. Its immutable PE chain binds the ReportGenerate constructor’s post-vptr virtual `+0x60` provider to a QString constructed from the exact `SfTaskReportGenerate` literal and written to `TaskInfo.impl+0x10`. The insertion routine uses that TaskInfo name for both metadata and raw task maps. The lookup-only getter returns the raw pointer at map `+0x10` when present, or null; it does not construct or check the returned vptr.

This closes the key initialization question at the original static level. Conditional on completed construction/registration and no replacement/removal, the map value has the constructor-installed ReportGenerate vptr. CP160 correctly stops short of asserting that this conditional value was returned in the DBLIST occurrence.

## Candidate reachability check

Current [task_factory.cpp] candidate source makes the predecessor block explicit:

- `TaskFactoryRuntime::createTaskBase()` in Native profile calls `initializeNativePrefix()` before any requested-key lookup; this applies even when the request is `SfTaskReportGenerate`.
- `initializeNativePrefix()` commits the factory and data owner, then allocates/records only pending prototype one (`SfTaskReport` at `0x1407b89e0`). It throws before calling the native QObject constructor and before TaskInfo initialization. Its diagnostic states this boundary directly.
- The proposal’s retained prototype order places `0x14079f1e0` (`SfTaskReportGenerate`) later in the 17-prototype sequence. Thus reaching that vptr/name provider depends on first resolving the earlier constructor, TaskInfo/PIMPL, font/metadata/preference effects and every intervening constructor’s applicable state. The value-level ReportGenerate name does not bypass those calls or make a later prototype independently reachable.
- Candidate `Factory::Data` has maps of `qt::String -> const void*` and `qt::String -> SessionTask*`, not an original TaskInfo/PIMPL owner. Pre-inserting a map entry under the now-known key would manufacture the pointer and metadata before the native insertion point. Setting only an unrelated field on the existing DOE task projection would likewise put the name on the wrong object.

The existing guard and construction snapshot should remain intact. The smallest eligible future seam is a ReportGenerate-specific metadata-name assignment inside its actual TaskInfo initialization frame, after the concrete task and TaskInfo owners are built in native order. Until the earlier constructor chain produces those owners, there is no real receiver on which to apply even this now-known value.

## Separate boundaries to keep

- **Getter:** Static lookup semantics are known, but do not fabricate a non-null key result. Require an actual registered map value from the same factory generation; if represented in a later candidate increment, record the concrete pointer identity and vptr conditionally.
- **AppState `+0x1c`:** CP160 corrects the earlier statement that no constructor reset was known: startup construction stores zero to `0x143d766b8+0x1c`. It does not establish the field’s later value when DBLIST reaches its gate. The gate remains guarded pending its later writer/reset/receiver lifecycle.
- **Public DBLIST/ReportList:** Keep the existing refusal. A static key initializer and a conditional cache contract do not prove an actual getter return, the `+0x218` invocation, the gate admission, or successful file-state behavior.

## Exact next work

First close the Native factory’s predecessor constructor boundary sufficiently to reach a real TaskInfo owner (starting with prototype-one QObject/private initialization and the first consumed TaskInfo/font/metadata/preferences, while preserving call order). Then carry the same actual TaskFactory/TaskInfo allocation identity through the prototype sequence to `0x14079f1e0`; install the exact metadata name at its qualified field-write point and register only after constructor completion. Tests should assert the existing factory remains unregistered at earlier guards and that no getter returns a fabricated entry. The DBLIST gate/getter occurrence still needs separate dynamic evidence before public dispatch can be enabled.

This is a read-only source/evidence review. No source, build, GUI, runtime, or mutable REA operation was performed.
