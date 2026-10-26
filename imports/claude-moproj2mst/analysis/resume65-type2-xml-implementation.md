# CP65 Type2 immediate InputTable XML output seam freeze

The new Type2 prefix performs immediate error-aware InputTable XML output for the owned empty-variable path and returns at the still-unqualified DoeTableRefresh boundary. It does not gate that boundary on the XML Boolean, deliver a message, produce PAR/main-project bytes or claim full Type2/public Save success. Root alone integrates the shared writer after this freeze.

## Owned writes and API

Existing files changed: include/m2m/doe_io.hpp and src/doe/io.cpp. New files: include/m2m/doe_xml_output_device.hpp, src/doe/xml_output_device.cpp, src/doe/type2_xml_prefix.hpp/.cpp, tests/doe/test_type2_xml_prefix.cpp. Shared moproj writer, convert, model, Session, DOE study planner and CMake were not edited by this lane.

StudyIo::openInputTableXmlFile(path) returns unique_ptr<StudyXmlFile>, an OutputDevice with independent error(). The default unsupported sink refuses rather than fabricate XML support. NativeStudyIo uses existing mst::FileDevice plus existing writeTextMode. RecordingStudyIo immediately clears its same simulated-file map entry at open and appends accepted translated bytes during writes; later readFile/openReadFile observes that same entry. No deferred replacement, rollback, directory creation or extra existence preflight is introduced.

The shared StudyIo.xmlWrites() ledger records path/open result, ordered Open/Write/Flush/Closing/Closed/ErrorAfterClose phases, requested pre-Text stream bytes, accepted source-byte counts, flush results and optional post-close file error. Requests and accepted counts are not durability. Devices/observers borrow their StudyIo; callers must retain that owner until device destruction, as the read-file seam already requires.

Frozen prefix API:

```cpp
Type2InputTableXmlResult writeType2InputTableXmlPrefix(
    engine::ConversionSession&, Project&, SubProject&,
    const Type2SaveNodePrefixResult&, unsigned localeCodepage = 0);
```

Result fields are path, opened, writerSucceeded, streamFailed(diagnostic), fileErrorAfterClose(optional), and guardMessage(). Exact next guard:

```text
Type2 study Save: signalDoeTableRefresh() receiver delivery remains unsupported
```

Locale0 is Windows CP_ACP, the current local codec; an explicit alternative is an externally qualified codec profile, not automatically the user-selected MST code page. Tests supply1252 explicitly. No success/error inference is made from the mutable global locale across unrelated calls.

## Native phase preservation

The prefix validates live Session/Project, actual SUB membership and Study alias/backlink/control block from the cached node prefix, existing I/O identity/control block and the actual Session ModelManager alias/owner. It never reselects activeSubProject or substitutes currentModel. Session identity is checked before Project dereference; membership precedes SUB dereference at revalidation. It refuses nonempty variables before XML open; the current empty-variable dimension/cleanup paths have no list effects.

It constructs the reached Control/DoeInputTable.XML path from the actual Project/Study, opens/truncates immediately, and only then builds/serializes the qualified empty-variable/constraint0 document. The private DOM builder reproduces existing writeInputTable fields without calling its deferred FileEffects writer or pre-encoding via legacy local8. Constraint or newly nonempty-variable serialization refuses after the reached open, retaining truncation. Owner loss during an adapter open likewise retains that open/close prefix and cannot reroute output.

The XML document goes through mst::Stream locale encoding and the one Text-mode wrapper, which performs CRLF translation exactly once. The wrapper forwards aboutToClose from the underlying QFile owner rather than emitting it early: FileDevice's pre-close flush occurs before the text stream's pending flush, preserving existing QFile error-reset behavior independently of sticky stream status. Native QFile close then error() determines writerSucceeded. streamFailed is reported separately and never substituted. Success leaves any previous global error intact.

Open false sets StudyIo.lastError to exact native literalformat Failed to open file, %1 and returnsfalse without DOM serialization or post-close error query. Post-close nonzero error sets Error writing file, %1. Literal VAs1427baf60/1427bb3f0 are selected from the immutable GUI PE; raw open/error ordering is retained in the CP63 writer captures. False is an ordinary writer result, not a thrown I/O error or permission to skip the next message. Unsupported owner/serializer/device boundaries are distinct reached refusals and retain earlier observations.

The function returns with refresh still unperformed after either ordinary true or false. CP65 native audit identifies real SfMoDoeTable/QTimer/UI ownership: the first receiver effect is actual timer stop before UI state. There is no candidate owner for that frame and no Boolean XML-result argument is sent. This lane does not call the empty-notification helper, reset messenger flag90, queue a timer, invent a null population or use the Study as a UI receiver.

## Recording and SaveEffects boundary

Default RecordingStudyIo is explicitly success-only simulation: open succeeds, bytes are accepted, close error0. It is not native failure evidence. Focused failure fixtures override the actual bound I/O/device and supply explicit open/post-close statuses; the production seam preserves those statuses without turning PAR's open-only return into XML success.

Immediate simulated read-after-write visibility and ordered XML attempts are present. This lane does **not** add a SaveEffects.project/SideBytes entry. Root integration must retain/expose the actual XML receipt and recording output in the caller's ordered result rather than discard the native writer's local unused bucket, duplicate a native write or copy requested bytes as proved durable bytes. Until that result bridge is supplied, public recorded Save output completeness remains unclaimed. XML effects survive a later refresh guard in the I/O/file owner, with no later PAR or .moproj commit.

## Eight focused tests and static checks

- Actual Session/Study recording prefix: locale bytes/one CRLF translation, immediate same-I/O read, preserved previous lastError, ordered close-time flush, no refresh/PAR effects.
- Open false preserves old bytes, skips even the constraint serializer and post-close error query, sets exact lastError and returns the refresh guard.
- Nonzero post-close QFile error makes resultfalse despite stream success, retaining accepted bytes.
- Zero post-close QFile error can make resulttrue despite sticky stream failure; accepted counts and stream status remain diagnostic.
- Constraint refusal after open retains immediate simulated truncation and closes on unwinding without inventing a normal error query/result.
- I/O-owner replacement during open refuses after retaining the old device's truncation/close prefix.
- Nonempty variables refuse before open.
- Disposable candidate-native device fixture truncates an existing scratch file and writes locale1252 plus one CRLF translation, checks post-close error and actual bytes. This test was authored, not run in this lane.

All 30 quoted includes resolve across the owned files. The new test has eight unique TEST_CASE names, Testkit main, supported CHECK/REQUIRE macros and no C assert. New files have final newlines and no trailing whitespace. Static inspection checks callback lifetime/close forwarding and the independent QName device status. No compiler, build, CTest, candidate executable or GUI was run by this lane; root owns execution.

Exact suggested target: m2m_doe_type2_xml_prefix; CTest name: m2m.doe.type2_xml_prefix. Root must add BOTH new cpp files(xml_output_device.cpp,type2_xml_prefix.cpp) to core and link the test, with the same support/test_paths setup as other scratch-file tests. Changes to StudyIo's virtual interface require a complete source-stable rebuild.

Next root callsite shape: retain the existing nonempty-variable node guard; call this XML prefix for its XmlOutputDevice stage; retain writerSucceeded separately; then unconditionally stop at result.guardMessage() until actual refresh owner/population/timer effects close. Do not continue PARs on an invented refresh success. Public B121/DBLIST/Type3 and later Type2 arms remain guarded.

Freeze UTC: 2026-10-24T22:21:36.195169+00:00.

| Owned source/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `9d176e7a8cfcb44c3ac592a9167f1e1cf6c13ffcc6b2465ae564058dcff7f3b2` |
| `source/apps/moproj2mst/src/doe/io.cpp` | `0aeb221dfafc52cafb9e4165053eb0e38aeaea09845f9fba3b8896b5d59af976` |
| `source/apps/moproj2mst/include/m2m/doe_xml_output_device.hpp` | `f8bf11d2703a5a6d0aca96df8e7adcf2e263fe376a1108616fa8763f6458c8e6` |
| `source/apps/moproj2mst/src/doe/xml_output_device.cpp` | `cd8d9ab766e97f7702f2d4d7ffbeb462d67d62d0d016b8050bf040102d8c6a51` |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.hpp` | `0a92bd02b0db6500291fe6d81e92b2f3bcc941b57f4449fc5da5b8b22d0acb91` |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` | `1db2d6bd3c3a78773a31e505775bb4c91cbee2e480b7567c6523d9a4337050aa` |
| `source/apps/moproj2mst/tests/doe/test_type2_xml_prefix.cpp` | `f4934ebac889a6e88e1ce7813a3147983419ecccbc1172c548e7ad35f3cda91e` |

| Controlling source qualification | SHA-256 |
| --- | --- |
| `analysis/resume64-type2-node-prefix-implementation.md` | `7c927c17f80dc89aee56f3bb1b761eb0f20519d50f27a303f4c53b1336fe0595` |
| `analysis/resume63-doe-backend-writer-entry.asm` | `bf013324bf1bf4d55bb45985cfa0c63035875bfcd2bdf2416531f89bcce6a299` |
| `analysis/resume63-doe-backend-writer-tail.asm` | `86df025e429d37b0bc5dea475a1273bd2ba4b382ce22da932c38ba685fcc5466` |
| `analysis/resume63-doe-backend-save-wrapper.asm` | `e128d25c7007ff290e59ea51ee5a00462b54aefa832486ed70c918999eaedcec` |
| `analysis/resume65-doe-refresh-owner.md` | `34d23e77aee1693b151479f089fb2baf7c1a1484004cb5f773036ad74bd53855` |
| `analysis/resume65-type2-xml-independent-review.md` | `d092ab0897f9da6734f2772b07e16d6cf535905823c5e399c633fbb669289b12` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `988b9599de5d47426bef8fd34554b436636e0af2932ecbd1f43e1458629532e4` |
| `source/apps/moproj2mst/src/mst/file_device.cpp` | `23a8a7cf973848e09a8fb068d815da2d9a16f4c59db333fb84cf34287a88fca5` |
| `source/apps/moproj2mst/src/mst/stream.cpp` | `f3d507e8e10076e11f24085ab9fc950638b4baf09784245c8f8c64ef6726f92f` |
