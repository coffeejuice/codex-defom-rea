# Checkpoint 67: DOE XML effects design review

## Review conclusion

The proposed single StudyIo-owned XML surface is a sound direction for preserving an attempt and exposing the recording sink's actual simulated bytes without scheduling a second write. The current first integration should remain a prefix ending at the reached `DoeTableRefresh` boundary. The design must keep three output lifetimes separate: the StudyIo-owned store/journal, the caller-owned `SaveEffects.project` observation alias, and any public conversion result that must survive a thrown guard.

There is a concrete lifetime/interface conflict for the native/public conversion route. `engine::saveProject(..., SaveEffects& out)` receives caller-owned output, but `run()` in `src/core/convert.cpp` creates `engine::SaveEffects saved` inside its `try` (around line 242). A refresh guard unwinds and destroys it before `catch`. The native path also calls `moproj::saveProjectFileNative(..., ctx.session)` without `saved.project`; that function currently supplies a local `unused` FileEffects to the body. Adding a shared surface pointer only to `SaveEffects.project` will not make native attempts visible to that bucket, nor preserve public-convert partial diagnostics after unwinding.

## Minimal sound CP68 source slice

Implement the observation/recording layer first and keep refresh delivery guarded:

1. Make one surface/store owner per actual `StudyIo`. It owns attempt events and the same recording file store that later `readFile`/`openReadFile` consult. Keep PAR writes on that backing store too, so an XML open cannot clear a stale or competing XML-only map. The surface contains observations/simulated values only; it must not retain Project, Study, Session, timer, or native file-device ownership.
2. Add a `shared_ptr<const Surface>` or equivalent read-only alias to `engine::FileEffects` (and therefore `SaveEffects.project`). Bind that exact surface before XML open. Do not put executed native XML output in `FileEffects::writes`, because `applyEffects` would replay it later. Do not copy event records into a second mutable journal.
3. Give the recording implementation a per-instance, explicit fault profile. Preserve the default success simulation, and let tests configure open, raw-write, flush, and post-close QFile-error observations without global/environment switches. Keep faults below Text translation and mutate the in-memory store only for exact accepted raw spans.
4. First exercise the real `engine::saveProject` shared path with an external `SaveEffects&` and an actual Session/Project/Type2 owner binding. This is the smallest interface whose caller retains the `SaveEffects` after the writer throws at the refresh guard. Do not present that recording path as native disk evidence.
5. If the next slice must observe `saveProjectFileNative`, pass an explicit observation-only sink from its caller; do not rely on its discarded local `unused` bucket. For diagnostics through public `convert.cpp`, add a separate C++ detailed-output handle whose storage is created outside `run()`'s try and retained by the caller. Keep that API work separate from the C ABI and legacy SideBytes projection; it is not needed for the initial source-stable `engine::saveProject` test.

This ordering avoids the shared-interface conflict: root owns the FileEffects/SaveEffects observation surface and public writer/convert interfaces, while an isolated DOE device/profile implementation can own its new helper and focused tests. A worker must not independently modify `engine.hpp`, `moproj_write.hpp`, `write.cpp`, `save_project.cpp`, or `convert.cpp` while root is integrating those shared files.

## Preserve the native failure/control order

The CP64 native evidence in `analysis/resume64-doe-save-effects.md` qualifies `0x1400d3200` as: dimension reconciliation; InputTable XML writer `0x14090a550`; unconditional `DoeTableRefresh`; then the wrapper returns the saved XML Boolean. `0x14090a550` first performs its pre-open variable cleanup, opens QFile WriteOnly|Text, serializes via locale QTextStream, closes, then checks QFile.error. The Type2 public caller ignores that ordinary Boolean and proceeds to the current sample writer and fixed mode calls. Therefore both failed-open and post-close-error results still reach the refresh boundary, and a failed XML Boolean cannot suppress the next message. The receiver/timer phase is independently unqualified by CP66; the candidate must stop there and record only a boundary reached, never a delivered signal or timer/flag result.

PAR remains a separate contract. Native `0x140915360` returns false only on open failure; after successful open it writes and closes, then returns true without checking QFile.error or stream status. Keep that distinction out of the XML result's writer Boolean.

The CP67 proposal correctly rejects reconstruction of file bytes from requested stream bytes and accepted source-unit counts. `writeTextMode` can accept a positive short data span, then proceed to CRLF and later spans without retrying the skipped suffix; raw output can be non-prefix. Native FileDevice may buffer accepted spans, so they are not proof of physical or durable file bytes. For recording, capture exact accepted spans below Text translation as the backing memory store's contents. Label those as simulated bytes only.

## Focused checks for the source slice

Use a real shared `engine::saveProject`/StudyIo binding and caller-owned `SaveEffects`, not only a standalone device test. Keep it a recording simulation and assert journal/store state after the expected refresh guard:

- **Open failure over an existing simulated file:** no truncation, no body writes, no post-close error query, open error recorded, writer false, refresh boundary reached.
- **Open success then non-prefix short write:** inject a short raw span followed by normal CRLF/later spans; assert the exact resulting recording-store bytes, not a prefix reconstructed from requested text. Retain the open truncation and all writes already accepted.
- **Independent writer status:** stream failure with post-close QFile error 0 yields `writerSucceeded=true`; complete accepted body with nonzero post-close error yields false. Record stream status and file error separately.
- **Owner/serialization guard after open:** observe the file version/truncation and partial accepted bytes, mark writer result/error query as absent, and assert no false ordinary result is synthesized. At this stage no refresh is delivered; if the XML writer returned ordinarily false, record the next refresh boundary anyway.
- **Repeated target:** two attempts get distinct IDs/sequences/versions; the second open's reached effect determines the current store view without erasing the first attempt's history.
- **Visibility:** a later read through the same RecordingStudyIo sees exactly the store's current simulated bytes before end-of-Save/SideBytes collection. A failed open leaves the earlier version readable.

Do not assert native partial file content from these checks. For NativeStudyIo, assert reached open/write/flush/close/error observations only; accepted FileDevice bytes may be buffered, and the ordinary error/result is not a readback hash. A separate independent readback can be added only as its own explicit observation, not as part of the faithful Save path.

The design's XML-only event journal is enough for this guarded prefix. Once PAR and final `.moproj` output are enabled, their relative order needs an integrated ordered projection; grouping the XML journal and existing FileEffects buckets after the fact does not establish one global native event order.

## Evidence boundary

Compared `analysis/resume67-doe-xml-effects-design.md`, `analysis/resume66-doe-refresh-prefix.md`, `analysis/resume65-type2-xml-implementation.md`, `analysis/resume64-doe-save-effects.md`, and current `source/apps/moproj2mst/include/m2m/engine.hpp`, `include/m2m/doe_io.hpp`, `src/doe/io.cpp`, `src/doe/xml_output_device.cpp`, `src/doe/type2_xml_prefix.cpp`, `src/moproj/write.cpp`, `src/engine/save_project.cpp`, and `src/core/convert.cpp`. Original XML order/error claims remain bound to the immutable executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` via the retained CP63/64 receipts. This was a read-only audit; no source/build/test/GUI/runtime/mutable REA operation was performed.
