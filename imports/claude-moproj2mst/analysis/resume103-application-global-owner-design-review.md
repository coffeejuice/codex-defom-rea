# CP103 — common application owner for DBLIST and Close state

## Finding

The native evidence now supports one common **lifetime scope** for the two problems: an application-instance owner that survives individual project/conversion Sessions and owns the current main-window identity plus app-global fields. It does not support wiring either feature to a `ConversionSession`. However, no candidate production owner can yet be bound to the corresponding live native state without fabrication. The current `ApplicationStateProjection` is intentionally detached and only models the process-type observation; it is not a main-window, manager, busy-state, or `+0x1c` owner.

The smallest defensible production design is therefore a future process-lifetime application owner injected into the public Open/Save/Close driver, with explicit per-field `Unavailable/Known` observations and a guarded main-window identity. It must begin unavailable unless the candidate application's actual construction/publication producer is implemented. It should not own or synthesize a `ModelManager`, initialize DBLIST admission to zero, copy a Session address into the main slot, or treat `+0x0c=5` as DBLIST admission. This is a design boundary, not a recommendation to add that owner now.

## Reconciled native ownership and state

The CP101 static chain improves the owner identity story. Original application construction initializes state at global `0x143d766b8`; later the application creates a main window and publishes it through that same singleton before initializing the main window's `+0x50` manager field. Open, explicit Save, and Close paths use this same singleton/main lineage. Main teardown clears the published main pointer after the application event loop exits. Project Close is not main-window teardown.

This establishes an application-lifetime boundary that can be shared by Session-like work over time. It does **not** make all fields one interchangeable status:

- `+0x0c` is process type. Open/Close refreshes select 5 or 1 from current-Project presence/open success. DBLIST's warning-only tail reads this value; 5 suppresses the warning without changing manager effects or the handler's false result.
- `+0x1c` is the DBLIST admission gate. Its predicate admits only zero; the known helper writes one, but its runtime caller/receiver and reset path remain unknown.
- The app-global qword at `+0` publishes current main. Busy checks also use global `+9`, and the main virtual busy predicate reads `main+0x480` current index and `main+0x270`; setter paths use additional `+0xA/+0xB/+0x274` and tree effects. These are live callback-sensitive Close state, not a scalar `+0x0c/+0x1c` policy projection.
- The main-window `+0x50` ModelManager is initialized after publication and owns the DBLIST model list. It is not the Session's existing per-conversion manager.

CP101's three xref requests for the application/global entry and busy setter providers timed out during Ghidra import; the retained export contains zero successful evidence records. The report's separate DUMPBIN/static receipts identify constructor/publication order and bounded stores, but do not provide dynamic callback-state coverage or a candidate main-window producer.

## Candidate mismatch

`ApplicationStateProjection` is not referenced by current production conversion code. It is constructed explicitly in its test; its only modeled transition consumes caller-supplied current-Project/open facts and updates `+0x0c`. `+0x1c` remains unavailable by construction. `core/convert.cpp::run` serializes process-wide engine conversions with a mutex, then creates a stack `ConversionSession`; it has no long-lived application/main owner and no normal-Close coordinator. `ConversionSession` owns the candidate `ModelManager` and destroys its own adapter resources. The Doe parser still refuses DBLIST/ReportList.

Thus a shared owner type could unify the **future interface**, but an owner allocation or singleton alone would only move unknown fields into one place. It would not bind the main-window producer, the app-global `+0x1c` setter occurrence, actual Open/Save busy-state writes, or callback-delivered mutations.

## Exact proof gap and interface implication

To implement a common production owner, the next source/evidence composition must provide:

1. One candidate application-lifetime construction/publication site that spans public Open, explicit Save, and a later Close attempt; it must bind a stable application identity and the actual candidate main/manager owner. A per-call stack Session is insufficient.
2. Writer hooks at each recovered Open/Save/Close occurrence that update the **same** owner in native order, plus callback-sensitive reads that can alter the live Close busy predicate. Initializer literals alone cannot stand in for a later current stack/page value.
3. For DBLIST admission specifically, a native occurrence for `0x1402cbf30` with the `rcx` receiver proven equal to global `0x143d766b8`, and the relevant writes/resets to that same receiver across startup/Open/Close. CP101 does not close this; it repeats the retained CP98 static blocker.
4. A main-window publication/clear lifetime and owner guard; `WidgetAreaPrimaryWindow`, MainView, notifier state, `SessionNotificationState`, and `ConversionSession::modelManager()` are distinct candidate objects and cannot alias it by naming or nullness.

Until those producers are present, the safe interface remains: keep `ApplicationStateProjection` as a detached value object; leave DBLIST admission unavailable; keep public DBLIST/ReportList and public normal-Close integration guarded. An eventual common owner can contain the projection and main/current-state subobjects, but it must receive only occurrence-qualified facts and must preserve their different meanings/lifetimes.

## Evidence receipts and limits

| Artifact | SHA-256 |
|---|---|
| Original `DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `analysis/resume101-close-live-owner-chain.md` | `8d893447af81cedd885b112609f7a7de709c1e2ab9b9f54a456746c4e2c7f793` |
| `analysis/resume101-close-pe-writer-chain.json` | `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1` |
| `analysis/resume66-dblist-global-lifecycle.md` | `e57b855d498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e` |
| `analysis/resume75-doe-application-state-projection.md` | `80b46a39c465a4c946271ddbe296a309844cbd0f61bf4665488aaeb344ff0908` |
| `source/apps/moproj2mst/src/doe/application_state_projection.hpp` | `25d236d51f278ab7e57e3773d8f6e12df68c57900879d150cf3deba7d9cc597c` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| `source/apps/moproj2mst/src/doe/session_records.cpp` | `8e8410f7fb4596bc62e752b02969208e51c9cfcfab402ba72727885bb3f50794` |

Read-only retained-evidence/source audit only. No REA retry, GUI, original execution, source edit, build, or mutable database operation was performed.
