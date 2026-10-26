# CP136 DBLIST generated receiver and slot frontier

## Result

CP131's unresolved *slot target* is now partly narrowed by the retained vtable and slot-body captures: if `slotGetTaskBase("SfTaskReportGenerate")` returns an object with that dynamic type, its handler dispatch at `SfTaskBase+0x218` lands at `SfTaskReportGenerate` slot 67, `0x1407a0150`. The native selector and main-window slot metadata are also retained. The missing edge is still the concrete return from the signal/Qt meta-call. No candidate-private dispatch cut can safely consume DBLIST or ReportList from these facts alone; keep the public guard.

## What the later artifacts close

- `analysis/resume60-dblist-signal-next.md` ties the existing connection setup call `0x1402cfbe0(0x143d76700, signalGetTaskBase, mainWindow, slotGetTaskBase, ...)` to the main-window constructor. The retained metadata names `slotGetTaskBase(QString)` and `SfTaskBase*`. This narrows the return handler owner to the main window, but does not retain its method body or show the concrete task returned for the `SfTaskReportGenerate` string.
- `analysis/resume131-dblist-metaobject-next.md` traces the actual dynamic call argument: `0x1402cf740` obtains a current item from the wrapper's `+0x88` list and uses that item's `+8` object pointer as the `QMetaObject::invokeMethod` receiver. The receiver is not directly the wrapper global nor the AppState singleton. That record/item is not statically bound to the main-window object or a specific task instance by this capture.
- `evidence/gui_mo/s1/vtables.tsv` identifies `SfTaskReportGenerate` primary vftable slot `+0x218` as `0x1407a0150` (index 67). `analysis/resume63-reportgenerate-slot67.asm` disassembles that function through its return (capture SHA-256 `1BB1996696A22FAD159005A87AED7A19BE5C6BAD89D5C694E5FD609098C489B7`). It walks child elements of its DOM input, and only for a direct `OutputList` tag calls the same object's slot `+0x240` at `0x1407a0242`.
- Slot `+0x240` is `0x1407a0500`, as captured in `analysis/resume60-dblist-reportgenerate-slot72.asm` (SHA-256 `C2C84E3CB5B49A6CD9887A7EFB346A3762ACD7D043B546F4ADE21E0C806E59F3`). That body allocates DOM-derived records and traverses object-associated data; it is not a source-closed no-op when an `OutputList` exists. The retained B127 ReportList shape has no direct `OutputList`, so on the ReportGenerate-type hypothesis the delegation does not occur for that fixture. This is only a static shape branch, not proof that the getter returned this type.

The captures are tied to original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The presence of a class string or vtable entry does not establish the returned object's vptr.

## Candidate boundary and no safe private integration

Candidate `src/doe/session_records.cpp::importPart` still stops on direct `DBLIST` or `ReportList` before constructing or mutating the corresponding task owner. `src/doe/task_factory.cpp::TaskFactoryRuntime::createTaskBase` explicitly refuses all registered non-DOE task construction in the bounded parsed profile; that constructor/factory route is separate from `signalGetTaskBase` and cannot supply its returned task. `src/doe/task_lists.cpp::readTaskLists` covers OprList/ObjList scalar projections and refuses populated `OutputList`; it is not the native ReportGenerate `+0x218`/`+0x240` object path. Existing tests for empty and populated OutputList therefore do not close the dynamic slot owner.

For B127's DBLIST node, absence of `OutputList` means the known ReportGenerate candidate slot has no visible list-child delegation after entry. It does **not** prove that the slot has no other effects, that the actual returned task is ReportGenerate, or that a DBLIST manager child exists. A no-op adapter keyed only by the selector would assume the unresolved signal return and could silently drop a non-ReportGenerate `SfTaskBase` implementation. Separately, DBLIST admission still depends on AppState `0x143d766b8+0x1c`; CP131/CP116 establish startup zero and an unbound one-writer, not the value at the public Open occurrence.

## Smallest missing witness

The next decisive retained artifact is the implementation/Qt metacall path for `slotGetTaskBase(QString)` through result storage. It must bind the invoked receiver to the current DBLIST call's owner, show lookup of the supplied `SfTaskReportGenerate` selector, and expose the returned object's actual vptr/type (or prove null). If it returns ReportGenerate, that binds the already captured slot 67 target; if not, capture the selected dynamic `SfTaskBase` vtable's `+0x218` target instead. Public DBLIST release additionally still requires the same-run `+0x1c` writer/reset and its ordering before a later gate read.

No new build, source edit, GUI session, original runtime execution, or mutable REA action was used. This is a bounded read-only reconciliation of CP60/63/131 with the retained vtable and slot captures and current candidate guards.
