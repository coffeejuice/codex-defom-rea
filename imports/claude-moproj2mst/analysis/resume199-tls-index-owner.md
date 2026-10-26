# CP199 TLS-index producer and exact-owner audit

## Decision

The static producer is identified, but an **initialized TLS-index receipt cannot currently be issued for the exact Session Report frame**. QtCore's PE image contains `ffffffff` at RVA `0x2f08e8`; the runtime initializer replaces it with the EAX result of imported `TlsAlloc`. Neither those initial bytes nor the Session's existing null-parent capability establishes the value at CP198's proposed terminal `0x29614`. Preserve that stop unless a separately reviewed symbolic-unknown global-read increment is deliberately selected. Do not choose the initialized-index JNE path or return from the initializer using the PE bytes.

CP197 is the supplied frozen predecessor. CP198 is a reviewed ten-instruction plan, not an implemented/frozen predecessor in this audit. Its proposed terminal is before CMP, with RSP `S-a8h`, enclosing generation-2 return `[S]=0x13144e`, and generation-3 return `[S-70h]=0x2acb6`. Reconcile any subsequent implementation with that actual source before assigning CP199 execution semantics.

## Evidence and qualification

Read `resume163-connector-qobject-static.json`, `wave2-doe-task-public-qobject-ownership-contract.json`, `wave3-doe-report-qobject-prerequisite-contract.json`, the retained wave2 initializer/current-thread listings, `resume196-nullpath-callee.md`, CP197 integration, and both CP198 plan/review reports. These are original-image/static qualifications, not live Report execution, exhaustive alias analysis, or native source code. Older prerequisite reports' early constructor stops/shared-reference gaps are historical; the current source has advanced those prefixes. Their unclosed TLS evidence requirement remains applicable.

Independent read-only inspection during this audit:

- Original `evidence/original-binaries/QtCore4_64.dll` SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; PE image base `0x67000000`.
- PE section/header mapping puts RVA `0x2f08e8` in file-backed `.data`, raw offset `0x2ee4e8`; its four file bytes are **`ff ff ff ff`**. This is an explicitly initialized image dword, not an inferred zero-fill value.
- Current local MSVC DUMPBIN SHA-256: `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. This differs from the historical tool hash in the old static receipt; it is a new read-only check against the same verified DLL, not a claim to have rerun the historical tool binary.
- Command: the receipt's absolute MSVC `dumpbin.exe /disasm:bytes evidence/original-binaries/QtCore4_64.dll`, piped directly to `Select-String -Pattern '672F08E8h' -Context 2,2`. Only the bounded matching lines were inspected/retained below. A small read-only Python PE parser independently checked raw bytes and resolved the import descriptors/thunks through each null-terminated import table.

No REA session/database, debugger, DLL loading, original process, GUI input, build or test was used. Static decoded direct references do not exclude indirect aliases, external writes, or loader/runtime changes.

## Actual producer, consumers and reset

All addresses in this table are QtCore RVAs. The complete selected direct-reference output had these ten instructions:

| RVA | Selected instruction | Meaning established by static bytes |
|---|---|---|
| `29614` | `83 3d cd 72 2c 00 ff` CMP dword `[2f08e8]`, -1 | First unknown runtime read at the proposed CP198 boundary |
| `2968c` | `89 05 56 72 2c 00` MOV dword `[2f08e8]`, EAX | Runtime index producer store |
| `2a062` | MOV ECX, dword `[2f08e8]` | Load before imported TLS write |
| `2a926` | MOV ECX, dword `[2f08e8]` | Load before imported TLS getter |
| `2acb6` | MOV ECX, dword `[2f08e8]` | Selected helper's post-initializer load |
| `2acf7` | MOV ECX, dword `[2f08e8]` | Helper null-result path, before TLS write |
| `2ad80` | MOV ECX, dword `[2f08e8]` | Another helper null-result path, before TLS write |
| `17db3f` | MOV ECX, dword `[2f08e8]` | Load before imported TLS write |
| `1a6204` | MOV ECX, dword `[2f08e8]` | Teardown candidate load |
| `1a6215` | `c7 05 c9 a6 14 00 ff ff ff ff` MOV dword `[2f08e8]`, -1 | Reset after the selected TlsFree call |

In the retained initializer, the not-equal branch at `2961b` goes to `296c0`. The equal route inspects one-time state at `2f3d28`, uses wrapper/storage at `2f3d20`, and has initialization/synchronization calls and atomic operations before reaching `29683`. The crucial sequence is:

```text
29683 48 0b df             or rbx,rdi
29686 ff 15 ec ec 17 00    call qword [1a8378] ; PE import KERNEL32!TlsAlloc
2968c 89 05 56 72 2c 00    mov dword [2f08e8],eax
```

Thus the file does not encode the allocated index. Its value is produced by the external call at runtime and stored as a 32-bit result. The retained bytes do not justify treating that result as always successful or replacing it with a chosen integer. The caller later loads ECX at `2acb6` and calls IAT `1a80a8` (`KERNEL32!TlsGetValue`) at `2acbc`. The other verified IAT slots are `1a8090` (`TlsSetValue`) and `1a83a8` (`TlsFree`). Teardown candidate `1a6200` compares the loaded index against -1, skips free/reset if equal, otherwise calls `TlsFree` at `1a620f` then writes -1 at `1a6215`. No invocation time or lifecycle completeness follows merely from this candidate's bytes.

The index is a module/process-global slot; its existence alone does not identify a thread's TLS value. Conversely, an adapter thread identity or notify depth does not identify this slot's current value. A non--1 index is only the prerequisite for the selected initializer fast path, not proof that the later getter returns a nonnull QThreadData.

## What the current owners actually certify

`ConversionSession` constructs the one `TaskAllocationStore`, `WidgetRuntimeState`, and `TaskFactoryRuntime`, associates the allocation store with that widget root, and invokes the private Session Report null-caller issuer (`src/engine/conversion_session.cpp`, constructor around lines 163-173). `SessionReportNullBinding` carries the actual receiver, weak allocation/widget owners, exact first Report/private identities and distinct frame/token/private/initializer-call occurrences. The validator in `src/doe/task_factory.cpp` checks shared ownership/control-block identity, source chain, ordinal zero, current allocations and the null-parent argument provenance. This is strong source-backed ownership for the actual modeled Report path; it is not evidence of prior native Qt startup or TLS initialization.

`WidgetRuntimeState` is already the common symbolic QtCore process owner for qualified shared-null identities/deltas and QObject counter state. That owner contains no TLS-index state/producer receipt. Its constructor creates one existing `WidgetThreadState`; the latter currently contains optional notify-depth base/delta and a posted-event queue (`include/m2m/engine/widget_events.hpp:131`), with no native QThreadData identity, index/value binding, TLS lifetime/generation or thread-data reference state. The Session's startup qualification string concerns the allWidgets registry; it cannot qualify every QtCore global. A module hash plus RVA identifies the symbol but supplies no runtime value.

The older `ReportQObjectSharedRefLedger` has a public `ReportQObjectTlsObservation` and `observeTlsFastPath`. It is expressly inert and consumes caller-supplied numbers/pointers. Equality/nonzero checks in that ledger neither produce an observation nor bind it to the Session's shared thread owner or current call occurrence. It also describes a later combined observation including the getter result and private+0x40, which have not executed at CP198. Reusing its default -1, supplying arbitrary matching values, or completing it in a unit test cannot authorize a production fast path.

The retained `wave3-doe-report-qobject-capture-plan.json` remains a plan. Probe004's receipt ends at `selected_editor_first_grid_published`, with startup/grid observations rather than the selected Report constructor/TLS events. Its loaded-module identity, thread ID or successful startup does not establish the value at this later Report occurrence. No selected same-thread initialized-index/getter/QThreadData receipt was found in the inspected retained contracts/reports. This is scoped evidence absence, not proof no such event ever occurred in the original application.

## Smallest next evidence and exact boundary

For **only the initialized-index CMP/JNE path**, obtain a qualified value of this loaded module's dword at `29614` for the selected Report/initializer occurrence. An exact non--1 value at that read is sufficient to determine the static CMP/JNE outcome; a getter result is not required merely to establish this branch. Bind it to module hash and actual loaded instance/base, process/run lifetime, common WidgetRuntime owner, existing WidgetThreadState, exact Session capability plus Report/private/frame occurrences, nested generation-3 call and instruction point. An earlier initialization store alone additionally requires a justified no-reset/no-overwrite lifetime to the read. Do not certify persistence from direct-reference enumeration alone.

For **the subsequent current-thread helper return**, separately qualify the actual index reload/getter argument at `2acb6/2acbc`, same-thread getter result and chosen path, selected QThreadData owner/lifetime, and external-call effects to the represented register/flag/error-state extent. Private+0x40 and thread-ref effects remain later transitions. Index initialization and nonnull thread value require separate receipts even if a future capture records both together.

The smallest non-executing preparation is an immutable, privately issued process-slot observation owner on the existing WidgetRuntime root with no default value; pair it with the existing thread and exact Session frame at consumption. A missing/foreign/copied/stale receipt must refuse before state change. The current source cannot issue a truthful initialized-value instance of that type without the missing original runtime observation or a fully qualified producer/effect/lifetime reconstruction. Host `TlsAlloc`, C++ `thread_local`, guessed startup order, successful widget construction, or a new factory-local thread/index owner are not substitutes.

An alternative bounded **structural** next increment could model CMP as a fresh root-owned unknown 32-bit read expression, with symbolic subtraction flags and no JNE decision, ending at `2961b`. The instruction reads memory and changes flags, so it is not covered by CP198's stack-only batch. It needs a reviewed unknown-memory observation/provenance schema and admission/snapshot tests; it does not create an initialized-index receipt or establish a runtime path. This is a possible next modeling assignment, not an approved implementation from this read-only audit. If the objective is concrete fast-path advancement, the exact blocker remains the unqualified runtime slot value at the selected occurrence.

Existing GUI release restrictions remain: the root lifecycle assessment reports no supported release/status API and `python ../../tools/gui_validation_status.py` returned `release_ready=false`. No GUI session was acquired. A passive observation proposal may be prepared using the existing Report capture plan; do not assume the actual Report path is reachable without further native/GUI actions, acquire control under the unresolved gate, or confuse debugger detach with GUI release.

Only this report was written. No source/test/build, parent application, DMO, Claude, original artifact, deployment or mutable REA state was changed.
