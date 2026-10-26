# Checkpoint 70: caller-owned detailed conversion outcome

The minimal C++ extension retains the actual StudyIo journal before a guarded Save can unwind its Session. It reports the returned conversion status separately from the XML writer's normal return and the reached refresh boundary. It must neither replay effects nor convert partial observations into a successful Save.

This is a read-only contract proposal. Root owns the shared API implementation. The source references below describe an inspected snapshot while root is editing shared files, not a new frozen source candidate.

## Existing lifetime and control flow

`source/apps/moproj2mst/src/core/convert.cpp:148` defines private `run()`. Its engine mutex, Result and Ctx precede the try, but ConversionSession, its Project/StudyIo and `SaveEffects saved` are inside it. `p.doeIo` is assigned at line 193; owner bindings immediately follow. `saved` is created around line 242. At the refresh guard, stack unwinding destroys those try-local objects before the Error catch sets Result. Copying `saved.project.xmlJournal` after that catch cannot work.

The current native branch sets projectWriteAttempted, calls `saveProjectFileNative`, and only then sets projectWriteReturned (lines 326–329). An exception leaves Attempted true and Returned false. The recording shared Save path similarly sets these flags around its project writer. For native normal false, later property/MST/remesh work is attempted before the adapter reports project failure (lines 390–398); an earlier later-stage guard can therefore determine the final Result instead. A project writer Boolean is not the final conversion Result.

The existing SideBytes collector runs near the end of the try (lines 409–419). It projects deferred byte pairs and is not an executed-I/O journal. Preserve its current collection order and failure behavior. Do not fill it from requested XML stream bytes, accepted source-unit counts, or partial recording-store contents.

`engine.hpp:39` FileEffects contains executable effect buckets and callback functions. `SaveEffects:332` also contains `Operation* current`. These are unsuitable as caller-owned diagnostics after Project destruction. The value-only SaveCallState at line 317 is safe to copy; full SaveEffects and raw Project/Study/Session/device pointers must not escape through the detail.

## Proposed C++ contract and exact assignment points

Names are illustrative so root can preserve its in-flight API naming. The required semantic fields are:

| Field | Type/meaning | Assignment point |
| --- | --- | --- |
| XML journal | `shared_ptr<const doe::StudyXmlJournal>` | Copy the exact noncreating `p.doeIo->xmlJournalOwner()` immediately after the actual StudyIo factory assignment, before any following owner binding or Open/Save work. |
| Actual I/O mode | optional `doe::IoMode` | Read from that same actual StudyIo at that assignment. Do not derive mode later from flags. |
| Save call snapshot | optional value `engine::SaveCallState`, or an equivalent value-only public projection | Only valid once a real engine Save call has been entered; capture its current value after normal completion or catch. |
| Returned conversion status | optional integer code, with Result still authoritative for text | Set to the Result code once the catches and normal try-scope cleanup have finished, immediately before returning Result. |

A minimal public type could be `ConversionDetails` declared in a dedicated C++ detailed-result header. An explicit new C++ overload accepts a caller-owned `ConversionDetails&`; existing Result and old overloads remain unchanged and forward no observer. If the new header uses SaveCallState directly, include its definition there; do not enlarge the basic convert.hpp dependencies merely to hide an incomplete value type. No m2m C ABI declaration, struct, calling convention, or legacy SideBytes type changes are required.

Reset the caller's detail once, after run acquires its engine mutex and before effectful initialization. This prevents stale journal/status from a previous call. The caller must not reuse, mutate, or concurrently inspect the same output during conversion. If mutex acquisition itself fails before entry/reset, no fresh-call outcome has been established; an optional completed field should remain an explicitly documented returned-call property, not a general exception handler for the C++ runtime.

For SaveCallState, retain an internal SaveEffects holder outside the try, with the caller receiving only its value call snapshot. Set an independent `saveCallEntered` validity bit immediately before the first actual `engine::saveCurrent` or `engine::saveProject` invocation, including the narrower no-WRITE_SAVE profile. In the common post-catch finalization, copy `saved.call` only when that bit is true. A save-request flag is metadata, not proof that Save ran. Moving the holder outside the try by itself is insufficient: the journal must still be attached early, and full FileEffects callbacks/pointers must never be exposed or invoked after teardown. If a temporary holder cleanup is required, discard the executable buckets without applying them.

The public optional status may instead use an equivalent value projection with per-writer states, avoiding a dependency on engine.hpp. Preserve all validity distinctions below; do not assign a fabricated native status to populate an API field. An unexpected exception escaping the existing catch set can leave the returned-code optional unset while the previously bound shared journal remains observable.

## Status meanings and partial effects

Use Attempted/Returned as the validity gates for the existing success fields:

| Existing writer fields | Reportable state |
| --- | --- |
| `!Attempted` | Not attempted; Succeeded has no observed meaning. |
| `Attempted && !Returned` | Entered but no normal writer return; do not synthesize false. |
| `Attempted && Returned && !Succeeded` | Normal false. |
| `Attempted && Returned && Succeeded` | Normal true. |

This applies independently to project and MST writers. Both Succeeded fields currently default to true, and the default dialog answer is not an observed answer before Save entry. Optional SaveCallState (or explicit validity) prevents these defaults from being presented as reached behavior.

For XML, use the canonical journal rather than a second summary that can diverge:

- A null journal means no actual StudyIo owner was bound to this outcome. A bound empty journal means that owner existed but no XML attempt was recorded.
- NormalWriterReturn records opened-false, or a post-close true/false result, together with stream failure and sticky lastError as separate observations. A serializer/owner exception before the return has no NormalWriterReturn. Opened-false has no post-close error observation.
- RefreshBoundaryReached follows a normal XML return, including false. It proves only that the caller reached the unconditional native continuation boundary. It is not notifier delivery, timer stop, UI refresh, PAR output, final DOM replacement, or Save success.
- The recording journal owns the same simulated file store used for subsequent reads. Exact accepted translated raw spans and optional recording snapshots describe that memory store. Keep them explicitly labeled recording observations.
- Native write requests, returns and stream acceptance do not establish physical or durable file bytes. Missing native byte snapshots remain unknown. Do not issue a readback or flush solely to populate the detail.

The shared const journal must own only value observations/store data, not Project, Study, Session, QObject, QTimer, native file handles, or callbacks into their stack frames. Its shared ownership can outlive Session and StudyIo. Device-to-attempt bindings remain private/nonowning and are erased on wrapper destruction. Const shared ownership does not make mutation during the call thread-safe: inspect the outcome after this synchronous call returns.

The journal has an ordered XML event sequence. It is not a total order over all Open/Save effects. Copying FileEffects groups after the fact cannot establish interleaving with immediate native KEY, PAR, project-file or MST effects. A first slice should expose XML partial effects plus separately valid call status, and leave broader executed-effect ordering unclaimed.

## Native direct-writer observer

The inspected in-flight `moproj_write.hpp:97` signature already has optional `engine::FileEffects* observation`. `write.cpp:583–588` binds its xmlJournal before the empty-path check or main project QFile open, then uses a local unused FileEffects for the body. A direct caller can allocate that observation outside its try, pass its address once, catch the refresh guard, and inspect the retained journal. The observation is read-only diagnostic output; do not apply its effects to manufacture a second XML write.

For convert's native branch, root can pass the caller-retained internal `saved.project` observer to this existing argument. The early detail binding at StudyIo construction remains the more complete lifetime guarantee because errors before native writer invocation can also be reported without manufacturing an I/O owner. Both aliases must refer to the identical canonical journal, not copied attempts or a second store.

This direct observer does not report the main `.moproj` device's opened/truncated state. ProjectWriteAttempted alone cannot distinguish empty path, QFile open failure, or interruption after successful open. A normal false also does not identify which of those occurred. Main-file effects need their own source-qualified observation point in a later increment; do not infer truncation or content from the XML journal or project writer flags.

## Focused validation requirements for root

1. Early failure leaves no stale prior journal; failure after actual StudyIo creation retains a bound empty journal if XML was not reached.
2. Recording XML normal true, opened-false, and post-close error each preserve the exact journal through the refresh guard and final Result. Writer false still reaches RefreshBoundaryReached; the conversion can remain unsupported there.
3. Interruption after XML open retains truncation/accepted recording effects but no normal writer return or refresh-boundary event. Stream failure with post-close error zero remains an independent true writer result.
4. A caller-held alias remains readable after Session teardown, owns no runtime back references, and is identical to any direct-writer/FileEffects alias. Native mode has no fabricated durable-byte snapshot.
5. New detailed and old calls execute the same conversion once, with matching Result/MST/legacy SideBytes and I/O counts. Reusing details resets previous output. Ordinary native false is distinguishable from attempted-unreturned even when a later error becomes final Result.
6. Exercise the optional direct native writer observer with an external holder and the exact reached guard. Keep internal selected-SUB1 Type2 fixtures distinct from public C ABI selection: adding details must not select another SUB or claim a newly reachable public Type2 profile.

No worker build, CTest, app control, mutable REA, or source modification was performed. This report is the only write. Root's next source slice is the detailed C++ API and private run observer lifetime in convert.hpp/new detailed header/convert.cpp, plus its already-owned native observer call wiring and focused integration tests. It does not release the actual refresh receiver or downstream Save guards.

## Inspected source receipts

Paths below are relative to `source/apps/moproj2mst/`. These are inspection hashes, not promises that root's actively edited shared files remain unchanged.

| File | SHA-256 |
| --- | --- |
| include/m2m/convert.hpp | 28D6EFADDE9DF0E8EE8161143969E4B30D1D06614C9ADF512C3F2B0BF954B794 |
| src/core/convert.cpp | F333209669D87DF730849BF2D7962B052A3D9C245ED3C328F268B1567CBD2555 |
| include/m2m/engine.hpp | C086A5530B3828204083D1891BB3F4BE555BD7B822CBDA851FCFA66780F2FD59 |
| include/m2m/doe_io.hpp | 55E75AC6F3D5C3EAA483690BEBC2740FA2789C47111151A5D7D900B843348279 |
| include/m2m/moproj_write.hpp | C78629EF0C232809026A66AFCF1FCC74C3BF4B93391ED36CFC11CB0E42C42069 |
| src/moproj/write.cpp | 6CEE2B06CE406E18C3A74643FE387B14C0434E22025DEBC5D734F55505D259D1 |
| src/engine/save_project.cpp | 90A0DDBF6B4C92B4F4FE8BC106AA2E9248F31C84D2A0981D0EE10EDA0BAF2337 |

Native XML return/refresh order remains qualified by `analysis/resume64-doe-save-effects.md`, `analysis/resume65-doe-refresh-owner.md`, and the CP69 frozen journal implementation report. Lifetime/surface limitations reconcile `analysis/resume67-doe-effects-independent-review.md`. No new original evidence capture was needed for this C++ lifetime contract.
