# CP67 single XML effects/result surface and recording faults

Use one canonical XML effects journal owned by the actual StudyIo and expose a read-only reference to that same journal through SaveEffects.project. Do not turn its observations into a second executor or infer file contents from requestedStreamBytes. A separate caller-owned detailed result is required to retain partial diagnostics through public conversion failure; existing SideBytes collection happens too late to do that.

This is a read-only design investigation of CP65 source and its independent review. Only this report was written. No source, build/tests, GUI, target process or mutable REA changed. The design does not qualify DoeTableRefresh receiver/timer ownership or release any Type2/B121/B127/Type3 guard.

## Current concrete gaps

- StudyIo::xmlWrites has per-attempt pre-Text stream requests, accepted **source-byte** counts, phases, flush results and post-close QFile error. RecordingStudyIo separately keeps translated file bytes in private writes_. Both survive a helper refusal only while that I/O owner remains available.
- RecordingBinary accepts every write and returns error0; the standard recording factory has no open/write/post-close failure plan. ScriptIo in test_type2_xml_prefix.cpp supplies declared fault observations, but it is a private test fixture, not a public recording result/profile.
- FileEffects::writes is executable deferred work used by applyEffects and by the SideBytes collector. It is not an appropriate home for already executed native XML attempts.
- saveProjectFileNative still has a local unused FileEffects fallback. A result sink must be passed from its actual caller if observations are to appear in SaveEffects.project.
- convert.cpp408/415 populates SideBytes after Save/MST succeeds. SaveEffects is currently local inside run's try. A refresh guard unwinds before that collector and destroys the local state before catch; merely appending an XML item to saved.project would not expose partial diagnostics through that API.

The existing externally supplied engine::saveProject(...,SaveEffects& out) already provides one useful lifetime: its caller can inspect out after a thrown guard. Native shared writer and convert's detailed result need an equivalent explicit external observation owner. No C ABI status or argument change is required to introduce an optional C++ diagnostics overload.

## One canonical owner, with projections

Proposed new conceptual owner: StudyXmlEffectsSurface, one instance per actual StudyIo/conversion scope. It contains stable attempt IDs, one monotonically ordered XML event journal, derived per-attempt summaries and the recording file-content store/view used by that I/O. It owns observations/simulated values only, never a Project, Study, widget, QTimer or live native file handle.

| Consumer | Relationship to the canonical surface |
| --- | --- |
| StudyIo/device observer | Sole mutation path for XML attempt/events/results |
| RecordingStudyIo reads | Consult the same recorded file store; no second private XML-byte map or end-of-Save replay |
| Existing xmlWrites() | Compatibility view derived from the journal; not an independently mutable second ledger |
| SaveEffects.project | shared_ptr<const Surface> or equivalent lifetime-safe read-only view of that exact owner |
| Detailed conversion output | Caller-owned reference retains journal/results after an unsupported guard and Session destruction |
| Legacy SideBytes | Explicit compatibility projection of simulated file artifacts only on its existing successful-return path |
| Native applyEffects | Does not execute this observational field or replay XML writes |

The current recording map also receives PAR writes. Keep **one** recording file store: either move that map into the surface's backing state or give both I/O paths the same store owner. A PAR write to an XML path must not be hidden by a stale XML-only overlay. Its file version changes on the same store; XML attempts reference the version they actually observed/changed. This is value/result storage, not a new native process/global owner.

Bind SaveEffects.project's observation reference before actual XML open, so truncation and later guarded/false outcomes remain visible through it. Reject rebinding to a different StudyIo/surface owner instead of copying records across sessions. A diagnostic shared surface can survive Project release without treating the native Project/Study or timer as alive. Current XML helper's owner validation remains required and separate.

## Ordered event contract

Use stable attempt IDs plus one journal sequence so nested/faulting attempts cannot be reordered by file name or grouped by outcome. Derived views can group events, but source order remains authoritative. Record actual reached events, not an anticipated full sequence.

| Event | Meaning and important limit |
| --- | --- |
| AttemptBegin | Reached path spelling, I/O mode and selected locale-codec profile; before device creation/open |
| OpenResult | Actual/simulated opened result. Successful recording open truncates the same store immediately; failed open does not modify it |
| StreamWriteRequested/Returned | Locale-encoded bytes before Text translation and accepted source units. Neither is file-content proof |
| RawSpanRequested/Returned | Bytes after Text translation, below the wrapper. For native QFile, acceptance may still be buffered |
| RecordingContentChanged | Exact mutation observed in the memory device/store, with file-version identity; this supplies simulated artifacts |
| StreamFlushResult | The explicit flush asked by QTextStream; do not label it FileDevice's unexposed pre-close flush |
| CloseBegin/AboutToClose/CloseReturned | Actual forwarding order. aboutToClose can cause stream/raw writes between CloseBegin and CloseReturned |
| PostCloseFileError | Actual error() read after successful close sequence; absent on failed open/unreturned serialization |
| LastErrorSet/XmlWriterResult | Exact sticky error and ordinary writer Boolean; stream status is separate |
| DeviceCleanup/GuardedStop/RefreshBoundaryReached | Only observed reached control points. RefreshBoundaryReached is not delivery or a bool argument |

Attempt summaries are projections, e.g. NotOpened, OpenedThenGuarded, WriterReturnedTrue/False, plus separate StreamFailed and optional FileErrorAfterClose. Never default an unreturned writer to false or a missing error query to0. A source-level unsupported owner/serializer guard differs from an ordinary XML false; both retain earlier file/log effects.

The current observer records a write only after its delegate returns. A canonical journal should distinguish Request before delegation from Returned after it, so a refusal during delegation does not fabricate a returned accepted count. Callback/recorder allocation failures retain uncertain observations; this is not an atomic transaction and must never roll back I/O to improve reporting.

## Byte provenance: the non-prefix case matters

Keep explicit provenance on every exported byte value:

- RequestedStreamBytes: pre-Text data passed by Stream; informational only.
- AcceptedIntoDeviceBuffer: a returned count from native QFile/write. It does not prove physical file bytes or persistence.
- ObservedSimulatedFileBytes: actual translated bytes held by the recording memory device/store. These can be exported as declared simulation artifacts, including empty/partial files.
- NativeFileBytesUnobserved: the normal native result when no independent byte observation exists. Leave contents unknown; QFile.error0 or writerSucceeded is not a readback hash.

Do not reconstruct file data by taking requestedStreamBytes.substr(0,acceptedCount). writeTextMode can accept a positive short text span, continue with CRLF and then later spans without retrying the skipped suffix. Resulting bytes can be a **non-prefix** of the requested source stream, and source LF counts differ from physical CRLF bytes. Capture actual accepted spans below Text translation into the recording store.

For native writes, even accepted raw spans can remain in FileDevice buffers and later fail during flush/close. Do not expose them as durable/native final contents. No additional readback/probe should be inserted into the faithful baseline merely to populate a report; an explicit independent acceptance readback is a separate observation with its own receipt. No fsync/power-loss durability is established by these mechanisms.

## Recording fault injection

Extend the actual recording factory with an explicit per-conversion/test-owned fault profile. Do not use environment/global switches or apply recording faults to NativeStudyIo. Default recording remains the declared successful simulation. Profiles match an expected attempt ordinal/path and contain reached observations for open, raw writes/flush and post-close error; record profile identity in the journal.

Open false leaves prior store contents unchanged, records no truncation/body/error-after-close and sets the native open-error string. A successful open creates/truncates that file version before serialization. Raw write faults operate **below** Text translation and update the store only with the bytes the simulated backing device actually accepted. Zero/negative results add no bytes. Positive short results add their exact span and preserve later native Text-wrapper decisions. Later errors never restore prior content.

Post-close QFile error is supplied independently of sticky stream status. Keep the CP65 distinction: stream failure with error0 can still make XML writertrue; accepted text with a nonzero error makes writerfalse. Fault plans describe declared device observations, not an observed native Windows failure or permission to infer native residue. If native-equivalent buffering/flush-fault residue is needed, qualify/expose the existing FileDevice backing-write/buffer events separately; do not invent it from a high-level count. Simulated file contents are known because they are the state of that memory device, not because the script's error code implies them.

Consumed/unconsumed fault steps and invalid profile values belong to fixture/profile diagnostics. They must not alter native writer result or introduce a production prevalidation pass. A host thrown adapter stop is a GuardedStop, not a native QFile false unless an actual native return was modeled.

| Required profile | Expected observed partial result |
| --- | --- |
| Default success | One Text translation; actual recorded bytes visible after writes/close; error0; writertrue; refresh boundary |
| Failed open over existing file | Existing bytes untouched; writerfalse/open lastError; no post-close query; same refresh boundary |
| Raw short span followed by CRLF/later span | Store matches exact non-prefix accepted spans, never reconstructed source prefix |
| Write/flush failure plus error7 | Earlier simulated bytes retained; writerfalse from error; stream status separate; refresh boundary |
| Same stream failure plus error0 | Earlier bytes retained; writertrue despite StreamFailed; no fake complete-body claim |
| Full accepted body plus error7 | Nonempty stored bytes with writerfalse and write-error string |
| Constraint/owner refusal after open | Truncated/partial file retained; writer outcome unreturned; no refresh-reached event |
| Two attempts on one target | Distinct IDs/events and versions; first result/history retained, second actual effect determines current view |

All cases should use the real shared Save/StudyIo binding, not only a standalone scripted device. Actual message/timer delivery stays guarded after either ordinary XML true or false.

## Public result and compatibility bridge

Add an observational XML-surface reference to FileEffects/SaveEffects, separate from executable writes. The shared recording writer already receives FileEffects&; link it to the actual I/O surface before XML open. Extend the native writer with an optional **observation-only** output reference/pointer so convert can pass saved.project instead of relying on its local unused fallback. The device continues to execute once at the reached source phase.

For public conversion partial diagnostics, prefer an explicit caller-owned detailed C++ result/output handle allocated outside run's try, or an overload that takes SaveEffects/Artifacts by reference. The surface must be retained before the first XML open and survive catch. Leave existing C ABI and successful SideBytes overload behavior intact; do not silently publish partial file pairs through an API that currently returns none on failure. engine::saveProject's external out can be the first integrated detailed test path.

On an overall successful recording conversion, a compatibility collector may export each reached simulated artifact/version in the appropriate Save phase, with its actual observed bytes. Failed-open attempts do not become newly written files. Opened-but-empty files and partial outcomes remain explicit in the detailed result. If the legacy pair-only SideBytes projection cannot convey a required status, retain that status in the detailed surface rather than relabeling bytes as successful complete output.

Do not append native XML receipt bytes into FileEffects.writes: current applyEffects would perform another, later write and change failure/visibility order. Do not defer recording-store mutation until SideBytes collection. Ensure the collector visits the XML project-phase projection in its reached order relative to the final .moproj artifact and future PAR effects; the journal is XML-scoped and is not proof that all existing non-XML FileEffects buckets already share a global event order.

Root's Type2 caller continues to throw the refresh guard irrespective of writerSucceeded. Preserve that behavior and its saved Boolean separately. No Surface::RefreshBoundaryReached may be promoted to notification delivered, timer stopped, table+98 set, PAR written, project DOM replaced or overall Save succeeded.

## Smallest implementation assignment

First implement the canonical surface/recording store view and observation-only SaveEffects alias, plus the explicit recording fault profile. Keep the public refresh guard and test partial engine::saveProject output through a caller-owned SaveEffects. This closes the current result/fault gap without requiring the missing widget/timer runtime. Add the detailed convert overload/legacy SideBytes projection only after its partial-result lifetime/API contract is selected.

| Root/shared files | Needed responsibility |
| --- | --- |
| include/m2m/engine.hpp | Observational surface reference in FileEffects/SaveEffects, not executable write |
| include/m2m/doe_io.hpp, src/doe/io.cpp | Single actual surface/store owner, recording factory profile and reads/views |
| include/m2m/doe_xml_output_device.hpp, src/doe/xml_output_device.cpp | Before/after requests, translated raw-span observation, explicit simulated faults |
| src/doe/type2_xml_prefix.hpp/.cpp | Record reached ordinary result/error/cleanup/refresh boundary without changing callback admission |
| include/m2m/moproj_write.hpp, src/moproj/write.cpp | Bind observation sink before XML and preserve native single execution |
| src/engine/save_project.cpp, src/core/convert.cpp | Pass actual external result owner; keep partial artifacts alive across guards |
| include/m2m/convert.hpp, if later detailed overload chosen | Explicit C++ artifact/result contract; no C ABI change implied |
| New exclusive surface/profile helper and focused test files | Canonical ordering/replay views and real shared-writer fault matrix |

No source ownership has been exercised by this read-only report. Current native/candidate error and codec port qualifications remain attached; a diagnostic surface does not establish fresh-original parity or complete lifecycle fidelity.

Freeze UTC: 2026-10-24T22:35:48.312547+00:00.

| Evidence/current source snapshot | SHA-256 |
| --- | --- |
| `analysis/resume65-type2-xml-implementation.md` | `d0837d50a832e1aa58a662b4ba782ce238334b8d17d9782c2e77ef246a36d5d3` |
| `analysis/resume65-type2-xml-independent-review.md` | `d092ab0897f9da6734f2772b07e16d6cf535905823c5e399c633fbb669289b12` |
| `analysis/resume66-doe-refresh-prefix.md` | `12225248c242c4f1e4a3527fde8f9b158d116c7cbceb9781b951eb9adbcf3c1c` |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `9d176e7a8cfcb44c3ac592a9167f1e1cf6c13ffcc6b2465ae564058dcff7f3b2` |
| `source/apps/moproj2mst/src/doe/io.cpp` | `0aeb221dfafc52cafb9e4165053eb0e38aeaea09845f9fba3b8896b5d59af976` |
| `source/apps/moproj2mst/include/m2m/doe_xml_output_device.hpp` | `f8bf11d2703a5a6d0aca96df8e7adcf2e263fe376a1108616fa8763f6458c8e6` |
| `source/apps/moproj2mst/src/doe/xml_output_device.cpp` | `cd8d9ab766e97f7702f2d4d7ffbeb462d67d62d0d016b8050bf040102d8c6a51` |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` | `1db2d6bd3c3a78773a31e505775bb4c91cbee2e480b7567c6523d9a4337050aa` |
| `source/apps/moproj2mst/src/mst/file_device.cpp` | `23a8a7cf973848e09a8fb068d815da2d9a16f4c59db333fb84cf34287a88fca5` |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `c0849247fb275e299c306b2613b5dbb60daa46f1b6ddea26257ed5949d5fb415` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `f333209669d87df730849bf2d7962b052a3d9c245ed3c328f268b1567cbd2555` |
| `source/apps/moproj2mst/include/m2m/convert.hpp` | `28d6efadde9df0e8ee8161143969e4b30d1d06614c9adf512c3f2b0bf954b794` |
| `source/apps/moproj2mst/src/engine/save_project.cpp` | `f4093cb75d92f47e83bc22f6679e8276709e3fe68494448ea1f0ba2c47e769cb` |
| `source/apps/moproj2mst/src/moproj/write.cpp` | `31fa67461d534b99ced922cf9361b1497054d291d064ccb55e9ed198488e4a0d` |
