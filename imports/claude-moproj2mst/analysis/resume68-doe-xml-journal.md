# CP68 canonical StudyIo XML journal and recording fault freeze

The actual StudyIo now owns one XML journal and the same recording file store used by XML, PAR and subsequent reads. Its read-only shared alias survives I/O teardown without retaining Session/Project/Study/native devices. Explicit per-instance recording faults run below Text translation and retain exact accepted memory-file spans; native bytes/durability remain unobserved by this journal.

Only include/m2m/doe_io.hpp, src/doe/io.cpp, tests/doe/test_type2_xml_prefix.cpp and this report were edited by this lane. Shared engine.hpp/save_project.cpp/public writer/convert/C ABI, XML device helpers and FileDevice were not edited. Root owns the caller-owned SaveEffects alias and its separate shared-path test, and builds after source freeze. No compiler, build, CTest, GUI or mutable REA ran here.

## Frozen observation and fault APIs

```cpp
std::shared_ptr<const StudyXmlJournal> StudyIo::xmlJournalOwner() const noexcept;
const std::vector<XmlWriteAttempt>& StudyXmlJournal::attempts() const noexcept;
const std::vector<XmlJournalEvent>& StudyXmlJournal::events() const noexcept;
std::optional<std::string> StudyXmlJournal::recordedFile(const qt::String&) const;
std::shared_ptr<StudyIo> makeRecordingStudyIo(std::vector<RecordingXmlFault>);
```

Existing xmlWrites() returns that journal's exact attempts vector, not a copied ledger. Journal data is private and public views are const. The old RecordingStudyIo writes_ map was removed. PAR writeTextFile, XML open/raw writes, readFile and openReadFile all consult the single journal-owned recording map. Unknown optional contents differ from known empty files. PAR baseline data remains visible across a failed XML open; later writes update the same store.

RecordingXmlFault fields are opened, rawWriteReturns, flushReturns, errorAfterClose and label. Profiles are copied into one RecordingStudyIo instance and consumed per XML open in order; exhausted profiles use the original successful simulation. The normal zero-argument factory retains its default success profile. There is no native fault setter, environment switch, C ABI option or public conversion overload. Protected recording methods reject Native mode.

## Canonical ordering and bytes

Each attempt has a stable zero-based index; events() is one append-only chronological sequence linking that index and optional raw-span index. OpenRequested is recorded before the standard native or recording factory opens. OpenResult and openResultObserved distinguish a returned false from an unreturned/unobserved creation path. Recording truncation occurs between request and returned open observation, before DOM serialization.

The StudyIo observer now records StreamWriteRequested **before** delegation and returned acceptance only after delegation returns. A thrown/guarded delegation therefore cannot acquire an invented returned count. Legacy phases and flush/close/error fields remain compatibility views of the same journal mutations. The wrapper forwards aboutToClose from the real lower owner; it does not emit it early.

RecordingRawWriteRequested/Returned events and XmlRecordingRawSpan contain actual post-Text requested bytes, optional returned count and the exact acceptedRecordingBytes appended by the memory-file device. Positive short spans update only those bytes; zero/negative returns append none. No source-byte slicing reconstructs content. A raw fault count outside[-1,request.size] is an unsupported recording-profile error after the request, not a native QFile false.

Each tracked recording close captures recordedBytesAfterClose from the canonical store, including empty/partial files. A failed open has no close snapshot. Snapshot/history remains distinct from current recordedFile state and later attempts. Native attempts have recordingStoreTracked=false, no recorded raw spans or content snapshot; accepted stream bytes remain buffering observations, not physical or durable native contents. Native FileDevice/writeTextMode behavior, close forwarding and post-close QFile.error remain unchanged.

The recording device is an explicit observation simulation, not native Windows fault-residue parity. It is a direct memory-file backend, not a reconstruction of native QFile's internal pending buffers. Its configured flush/file-error observations are independent of stored memory bytes and sticky stream status, which are retained separately. No error code or writer Boolean is used to manufacture memory data or claim native durability.

## Partial/control effects retained

Open false leaves the existing shared store untouched, produces no raw writes or post-close query and preserves the ordinary false/error route to the current refresh guard. Open success truncates immediately; later serializer/owner refusal retains that effect and any returned spans. Native QObject/table/timer ownership is not supplied by the journal.

This journal records I/O observations, not a fabricated completed XML writer or delivered refresh. ErrorAfterClose does not by itself prove the outer helper returned; a later owner guard can still occur. Root must retain actual returned XML result/control state separately when its caller observes it. The current prefix/public guards and writer-false continuation semantics are unchanged.

Recording store/journal observers capture their shared observation owner, so diagnostics can outlive StudyIo without retaining native objects. The existing caller requirement to keep its I/O context alive through helper/device use remains. Metadata allocation/callback failures are not made transactional and no earlier file effect is rolled back for reporting.

## Thirteen focused cases (authored, not executed here)

The existing eight CP65 cases remain. Native device coverage now uses NativeStudyIo so the test can verify actual scratch-file bytes independently while asserting the journal exposes no fabricated recorded/native byte snapshot.

Five added cases cover:

- Standard recording failed-open profile over an existing PAR/interface store value; exact canonical alias and request/result ordering, no truncation/body/snapshot.
- Positive short raw span followed by CRLF and later text. Source abcd\nZ records actual simulated ab\r\nZ, accepted source units4 versus physical bytes5, and matching later readFile/openReadFile results. This non-prefix case detects reconstruction from requestedStreamBytes.
- Common recording profiles for rejected write/error7, rejected write/error0, flushfalse/error7 and full body/error7. Stream status, post-close error, source result and exact stored accepted spans stay independent; no refresh is delivered.
- Repeated attempts on one target retain distinct labels/history, first close snapshot and failed-second-open current contents; another I/O instance does not inherit profiles or their cursor.
- A read-only journal remains inspectable after actual I/O/Session teardown and contains only observation/value ownership.

All 18 quoted includes in owned files resolve. There are 13 unique Testkit cases, main, supported CHECK/REQUIRE assertions, no C assert, final newlines and no trailing whitespace. Static inspection confirms no old xmlWrites_/writes_ storage remains and no worker edits to shared interfaces. These are static checks, not compiled/executed validation.

Existing exact target remains m2m_doe_type2_xml_prefix / m2m.doe.type2_xml_prefix; no new source file or test target is required. Root should group it with the changed shared SaveEffects alias/guard test and full Release rebuild. Ownership/layout changes in StudyIo require a source-stable rebuild.

## Integration boundary

Root may alias the same shared const journal in caller-owned FileEffects/SaveEffects before XML open; do not put executed XML data in executable writes or replay it through applyEffects. This lane does not solve native writer's local unused sink or run()'s try-local public result lifetime. The first sound observer remains the external SaveEffects passed to engine::saveProject, as the independent CP67 review specifies. No new convert overload/C ABI change/legacy SideBytes publication was made.

The next real delivery boundary remains actual SfMoDoeTable/QTimer/UI ownership after XML/error effects. All public Type2/B121/B127/Type3 success and end-to-end acceptance claims remain withheld.

Freeze UTC: 2026-10-24T22:50:24.366485+00:00.

| Owned source/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `10b6d59385f073e49eb0c2baf11a591ca97d82e83ba807dd7746c516fb074548` |
| `source/apps/moproj2mst/src/doe/io.cpp` | `b89ad8a85f0500123163d7f18f05657aaa7c224155a46cdf01f1b410fa5a8fd3` |
| `source/apps/moproj2mst/tests/doe/test_type2_xml_prefix.cpp` | `031c102250972d0d8c0569ea3c7c4bac405098e3213aebb9dcf15507f2d85709` |

| Controlling evidence/current unchanged helper | SHA-256 |
| --- | --- |
| `analysis/resume67-doe-effects-independent-review.md` | `cc8f6477898ed3ed279d2902d2feb10e1e75c4580fb800fd9b6b8f67463f9f6b` |
| `analysis/resume67-doe-xml-effects-design.md` | `02175eaf1f3f40fb92097be0043c47517a12a3d8fbf144a1bd41e68258cac1f3` |
| `analysis/resume65-type2-xml-independent-review.md` | `d092ab0897f9da6734f2772b07e16d6cf535905823c5e399c633fbb669289b12` |
| `analysis/resume65-type2-xml-implementation.md` | `d0837d50a832e1aa58a662b4ba782ce238334b8d17d9782c2e77ef246a36d5d3` |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` | `1db2d6bd3c3a78773a31e505775bb4c91cbee2e480b7567c6523d9a4337050aa` |
| `source/apps/moproj2mst/src/doe/xml_output_device.cpp` | `cd8d9ab766e97f7702f2d4d7ffbeb462d67d62d0d016b8050bf040102d8c6a51` |
| `source/apps/moproj2mst/src/mst/file_device.cpp` | `23a8a7cf973848e09a8fb068d815da2d9a16f4c59db333fb84cf34287a88fca5` |
