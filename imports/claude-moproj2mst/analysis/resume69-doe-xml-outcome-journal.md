# CP69 XML normal return and refresh-boundary journal freeze

The actual StudyIo journal now distinguishes I/O completion, a normally returning XML writer, and the later reached refresh boundary. Empty-variable XML prefix returns record its actual result, sticky stream status and sticky lastError separately. Interrupted serialization/owner paths have no normal writer return. A separate root-callable marker requires the exact latest returned attempt/path and represents **no** messenger delivery, timer stop or UI effect.

Only doe_io.hpp, io.cpp, type2_xml_prefix.cpp, test_type2_xml_prefix.cpp and this report were edited in this lane. Shared engine.hpp/save_project/convert/write.cpp, C ABI and XML-result header were not edited. Root has independently wired the marker in its shared writer and owns grouped validation. Source was frozen before report completion; no build/CTest, candidate execution, GUI or mutable REA was performed here.

## Frozen APIs and fields

```cpp
std::size_t StudyIo::observeInputTableXmlWriterReturn(
    const StudyXmlFile& actualFile, const qt::String& path,
    bool writerSucceeded, bool streamFailed);
void StudyIo::observeInputTableRefreshBoundary(
    std::size_t attemptOrdinal, const qt::String& path);
```

XmlWriteAttempt adds optional<XmlNormalWriterReturn> normalWriterReturn and bool refreshBoundaryReached. XmlNormalWriterReturn stores writerSucceeded, streamFailed and stickyLastError, all independently observed/snapshotted values. New global event kinds are NormalWriterReturn and RefreshBoundaryReached; existing I/O phases remain unchanged.

The journal keeps a private nonowning binding from the actual returned StudyXmlFile wrapper to its attempt ordinal. The binding is created at observation-wrapper construction and erased silently on device teardown. Duplicate open/device binding to one attempt is refused. The journal does not own any file/Session/Project/Study/widget/timer; a surviving const journal alias contains no retained native runtime object. Raw device addresses are compared only while their live wrapper binding exists.

## Normal writer return admission

The observation requires that exact bound file, exact caller path spelling and an observed open result. A duplicate normal return is rejected. For opened=false, success/stream-failure/post-close-error claims are rejected because no stream/error-query path was reached. For opened=true, close and error observations must exist, and writerSucceeded must equal(post-close error==0). Stream failure is independent and may coexist with writerSucceeded=true.

The observer snapshots actual StudyIo.lastError at that point; it neither clears nor recomputes it. Normal-return metadata uses the existing open-false and post-close writer paths, after their native error-string handling and final owner validation. The XML helper records it only immediately before its two normal returns. Source validation or serialization refusal before those return points leaves normalWriterReturn absent even if unwinding closed/truncated the file.

In particular, errorAfterClose by itself is not normal XML return. A standalone device close/error query does not acquire the marker automatically, and a post-error owner guard still leaves no normal return. No false outcome is substituted for an unreturned frame. Metadata allocations are prepared before committing the normal-return/boundary fields; existing physical/simulated I/O is never rolled back for diagnostics.

## Refresh boundary admission

The separate API requires the exact path/ordinal, an existing normal return, no prior boundary, and the latest global journal event being that same NormalWriterReturn. Foreign, stale, mismatched, duplicate or interrupted-attempt markers refuse before journal mutation. Root can copy events().back() by value after the helper returns, check its kind, and pass its ordinal plus xmlResult.path. Device teardown does not append a competing event. Copy the event value rather than holding a vector-element reference across the next journal append.

The boundary API receives no XML-result Boolean and does not inspect/create the notifier. Both ordinary XML true and false can reach it. It marks only that source callsite was reached; it does not set suppression/flag90, deliver a receiver, stop a QTimer, set table+98, enter a widget, write PARs, replace Project DOM or complete Save. The actual receiver/timer/UI owner guard remains after this observation.

## Bound positional recording profiles and chronology

RecordingXmlFault adds optional expectedPath and expectedAttemptOrdinal fixture assertions. Path matching uses the same cleanPath normalization as the recording store; normal-return/boundary path matching remains exact to the actual attempt. Assertions are checked before OpenRequested/open/truncation, and mismatch does not consume the positional fault entry. Profiles remain per-instance, positional, test-only observation simulations; there is no environment/C ABI/native fault switch or generic production schedule-closure claim. Unused raw/flush script steps are not promoted to a native error or writer result.

Valid-attempt chronology is deliberately retained: OpenRequested, actual simulated map truncation, then OpenResult observation. OpenResult means the observed return/query, not a promise issued before the open's effects. No previous I/O events were reordered to alter this interpretation. New normal-return and boundary events append only at their newly instrumented control points.

Actual translated accepted spans, close snapshots, immediate read-after-write visibility and NativeFileBytes-unobserved semantics are unchanged. Stream requests/counts are not durable bytes; native QFile close/error behavior was not altered. Recording post-close statuses remain declared device observations, not original Windows fault parity.

## Fifteen focused tests and static checks

The existing thirteen cases remain, with outcomes asserted where actual helper returns. Two new cases cover:

- Actual file-device binding, exact path, contradiction with observed file error, foreign I/O, duplicate normal return, wrong/stale ordinal, wrong path, duplicate boundary and snapshot independence from a later lastError change. Device teardown permits the already returned attempt's boundary without introducing a new event. The standalone marker creates no notifier.
- Bound positional fault path/ordinal assertions: mismatched path/ordinal refuses before open or store mutation; the correct retry still receives the unconsumed fault.

Existing profiles now verify normal true/false, stream failure and sticky error separately, followed by the same reached boundary for each outcome. Repeated target attempts reject a stale first ordinal and accept the latest ordinary-false return. Interrupted constraint/owner cases assert no normal return and no valid refresh boundary. Native device-only tests do not gain a normal XML-writer marker.

Static checks: 15 unique Testkit cases, supported main/CHECK/REQUIRE macros, no C assert, 25 quoted includes resolved, final newlines and no trailing whitespace. No compiled or executable test result is claimed. Target remains m2m_doe_type2_xml_prefix / m2m.doe.type2_xml_prefix; root groups it with its shared-writer/owner checks and full Release rebuild.

## Remaining scope

The journal remains XML/I/O and control-point observations. Caller-owned SaveEffects can retain the exact alias through a guard. Native writer/convert's try-local public-result lifetime and legacy SideBytes remain separate contracts; no new convert overload or C ABI publication was added. The source-qualified SfMoDoeTable/QTimer/UI receiver and complete notifier population still determine any actual refresh continuation. No native durability, Type2 success, B121/B127 or Type3 release follows from these markers.

Freeze UTC: 2026-10-24T23:06:33.681084+00:00.

| Owned source/test freeze | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `55e75ac6f3d5c3eaa483690bebc2740fa2789c47111151a5d7d900b843348279` |
| `source/apps/moproj2mst/src/doe/io.cpp` | `b8c71b9a5be6fd13b444af8d998dbafa5a71a38e31adcc72894145d5655e87f1` |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` | `10bd0e9690f62387e394eb603d7f90619aa81304f9fd8127f7fa58c5f14d4d57` |
| `source/apps/moproj2mst/tests/doe/test_type2_xml_prefix.cpp` | `547f1692a295cc1ecd4c6f164f82d7998c133d1330547e0f1791a3b8a1a5ec3f` |

| Controlling evidence/current unchanged helper | SHA-256 |
| --- | --- |
| `analysis/resume68-doe-xml-journal.md` | `7a402a9bcb920c04261782903d55eed4cb668138c8038f77250fe57312601484` |
| `analysis/resume67-doe-effects-independent-review.md` | `cc8f6477898ed3ed279d2902d2feb10e1e75c4580fb800fd9b6b8f67463f9f6b` |
| `analysis/resume65-doe-refresh-owner.md` | `34d23e77aee1693b151479f089fb2baf7c1a1484004cb5f773036ad74bd53855` |
| `analysis/resume66-doe-refresh-ui.md` | `0975503f6ce1b52a47e072ae052725b236328127cd5f6ce01ed2a4ec42ad17b3` |
| `analysis/resume65-type2-xml-implementation.md` | `d0837d50a832e1aa58a662b4ba782ce238334b8d17d9782c2e77ef246a36d5d3` |
| `source/apps/moproj2mst/src/doe/xml_output_device.cpp` | `cd8d9ab766e97f7702f2d4d7ffbeb462d67d62d0d016b8050bf040102d8c6a51` |
| `source/apps/moproj2mst/src/mst/file_device.cpp` | `23a8a7cf973848e09a8fb068d815da2d9a16f4c59db333fb84cf34287a88fca5` |
