# Private project-output integration review

The writer seam and `convertDetailedWithServices` integration preserve the reviewed native Save order and per-call ownership. No blocking source mismatch was found. The service changes only the selected native project output device; it retains the default stack FileDevice path and permits a declared post-close status test without skipping properties, MST or remesh. This report is static review, not a build/test pass or physical-fault/original-runtime acceptance.

Only this report was written. No source, build, GUI, mutable REA or original process changed. Reviewed source hashes below identify the final inspected snapshot, including the paired integration case added during this review.

## Device and callback lifetime

Private `src/moproj/project_output_device.hpp` defines `ProjectOutputDevice : mst::OutputDevice`, adding only the post-close `error() const` query. `ProjectOutputFactory::open(path)` returns unique ownership and must distinguish an actual nonnull failed-open device from a null service configuration. Virtual destruction follows the existing OutputDevice destructor.

`saveProjectFileNativeWithFactory` owns that returned device through the complete synchronous writer call. `finishNativeProjectOutput` first checks actual `opened()`, then calls the unchanged serializer body. Its Stream is local to that body, closes the device while Stream is alive and is destroyed when the body returns. Only afterward does the finish helper query `file.error()==0`. Finally the unique device is destroyed while the factory's caller-owned observation storage is still live. The result/details contain no device or callback.

The writer does not substitute Stream's sticky failure flag for the final QFile-facing error. This matches retained `wave3-project-close-status-contract.json`: native `0x140098dc8` closes, then `0x140098dd3` queries QFile.error; successful close-time buffer recovery may clear earlier file error while QTextStream remains failed. A factory override is sampled at that same candidate decision boundary.

The dedicated writer adapter and root integration adapter both delegate to a real FileDevice. Their lower device callback forwards `notifyAboutToClose()` to the outer Stream only when real FileDevice emits it. They do not pre-flush the outer stream, emit an early synthetic callback or alter FileDevice's saved-initial-flush ordering. The real sequence remains:

```text
FileDevice initial byte-buffer flush (save its result)
  -> aboutToClose while opened -> outer Stream callback flush
  -> native buffer/handle close
  -> serializer Stream destruction and callback detachment
  -> post-close error query
  -> per-call adapter destruction
```

`mst::Stream::~Stream` flushes and clears its outer callback before the adapter dies. Both test adapters close idempotently and detach the lower callback before lower FileDevice destruction. A lower callback captures the adapter, which is still alive during close; its surviving observation reference belongs to the outer test scope. No observed callback points to a destroyed Stream/device in these selected normal and test teardown paths.

The factory contract relies on an implementation honoring OutputDevice semantics. It does not authenticate arbitrary custom devices, their actual path or callback behavior. This is acceptable for a private explicit dependency; the supplied test adapters assert the exact requested project path and delegate physical I/O. No public setting or environment variable selects them.

## Per-call core service lifetime and default route

`src/core/conversion_call_services.hpp` contains one borrowed factory pointer, default null. The private synchronous `detail::convertDetailedWithServices` passes the services address into `run`; no Project, Session, OriginalSettings, public C ABI or ConversionDetails stores it. The service object and factory must outlive that synchronous call, including time waiting on the existing engine mutex. The reviewed tests keep both in that caller scope. No asynchronous callback retains either.

All ordinary conversion wrappers pass null services: `convert`, `convertDetailed` and both underlying `convertToBytes` call routes retain the default behavior. A private call with a null factory also follows the same default branch. Only `nativeSave` with a nonnull explicit factory calls the private native writer. A provided factory is not used for input, DOE XML, property outputs, MST, remesh or recording-only Save.

The default `saveProjectFileNative` still constructs **stack `mst::FileDevice file(p.path)`**, with no factory/adapter allocation. Both entries share the same prelude and finish helper. The prelude assigns the already existing XML journal observation before the null/empty path gate, appends the same project suffix and only then opens output. Failed native open returns false before DOM mutation; normal successful open retains the same DOM/Study side-effect order. Exceptions propagate without rollback.

`include/m2m/moproj_write.hpp` and `include/m2m/convert_details.hpp` were compared byte-for-byte against `build/wave3-checkpoint72-verified/source.zip`: both match. New core quoted includes resolve through their own directory and m2m_core's existing PRIVATE `src` include directory. No new public ABI surface was identified.

Null factory result throws private `std::logic_error`; the outer driver maps this infrastructure error to existing `M2M_E_INPUT`, with project Attempted true and Returned false. It is correctly not reported as the original ordinary failed-open outcome. The service contract requires returning an object with `opened()==false` to represent that outcome. Factory/device exceptions are interruptions, not ordinary returned false; no test should treat their default Succeeded bit as meaningful without Returned.

## Journal, status and continuation order

The reviewed native core order is:

1. Enter Save/current-operation prefix and apply step2 effects.
2. Set `projectWriteAttempted=true`; call the default or explicitly supplied native writer.
3. Assign the returned Boolean to `projectWriteSucceeded`; set `projectWriteReturned=true` only after normal return.
4. Run operation properties/Boolean sidefiles in the existing operation order.
5. Attempt native MST, record its attempted/returned/succeeded states.
6. Run the remesh tail.
7. Map failed MST first, then failed project writer, to adapter `M2M_E_WRITE` after those prior effects.

The factory branch adds no early `if (!projectWriteSucceeded) return/throw`. A returned false project result therefore does not suppress steps4-6. An earlier exception still prevents later stages, as before. A remesh refusal can take precedence over the eventual adapter project-write mapping; the new service does not alter that guard.

`details` is reset at run entry under the existing serialization lock. The existing actual StudyIo journal/ioMode are attached when that owner is created and survive Project destruction by value-only shared observations. The project writer's external observation borrows that same journal, schedules no replayable project write and is not a new physical-output journal. The service introduces no device/factory into the XML journal. `details.saveCall` is copied after normal or exception completion only if Save was entered, and `details.returnedCode` is the final adapter result.

SaveCall's Succeeded bits default true; their meaning still requires both Attempted and Returned. The private service does not weaken the public `ConversionDetails` qualification. Likewise `RefreshBoundaryReached` remains pre-dispatch XML reach, not native refresh completion, unrelated to the new project-status seam.

## Acceptance tests read and their limits

`tests/moproj/test_project_output_device.cpp` uses two fresh disposable copied projects, delegates real open/write/close, then compares real status against declared status7. Its assertions require one factory invocation, correct normalized path, positive serialization, one callback while open, close-before-error order, one status query, adapter destruction before observation inspection and actual serialized DOM bytes. The external observation contains no replayable project writes. This tests writer Boolean selection; it does not test full caller continuation.

The reviewed root case `save_project_reported_postclose_error_continues_properties_mst_and_remesh` now supplies that integration layer through the private detailed-conversion service. Each run has a fresh copied fixture and real conversion/Session. It creates a source-qualified remesh fallback and requires later output paths initially absent. At project error query the adapter observes those later paths still absent; afterward the test requires regenerated Boolean BWZ/INI, complete MST and enabled remesh output, comparing all four contents between control and injected runs.

The test's callback/close/error observations require the same delegated normal close order and real FileDevice error0 in both runs. Control uses that real status, expects project true and `M2M_OK`; injected run reports7, expects Attempted/Returned true with Succeeded false, completed MST flags and final `M2M_E_WRITE` with the project failure message. The output details code must equal Result.code. The explicit remesh artifact prevents inferring tail execution merely from a final status. Source establishes MST-before-remesh order; the test also establishes that both follow the project status query.

These are declared candidate status-injection cases. They do not calibrate a persistent physical QFile failure, predict partial project bytes from a lock or compare against a fresh original fault. The delegated project can be completely written in both runs, which is why the integrated case deliberately does not assert an invented project residue. The standalone case's full DOM-byte equality is a measured delegated successful-write effect, not a physical-fault prediction.

No compiler, configure, CTest, test executable or original capture ran in this review. Root must supply grouped and full source-stable validation/freeze receipts. The factory-null, explicit failed-open service, exception, empty-service and non-Save private-call branches have source-defined behavior but are not new paired post-close acceptance coverage. Existing tests may cover their ordinary default counterparts; no unexecuted coverage is promoted here.

## Reviewed hashes

Paths below are relative to `source/apps/moproj2mst`. This table is a review snapshot, not a build freeze.

| Source | SHA-256 |
| --- | --- |
| `src/moproj/project_output_device.hpp` | `586132f1b480112b5b3d4152b35aca508114fe65a93e786d0b11d2ba8cbec669` |
| `src/moproj/write.cpp` | `70ed65c4c2581ccbafaf123ab70d1fefd0d4747701fd72b17cf6ce8ab1f96717` |
| `src/core/conversion_call_services.hpp` | `8fc9fe687a5a3a187d9f913fc7ba1b5ae62bfdeb97975a9ebcedf1ffdf492a6a` |
| `src/core/convert.cpp` | `1dccfc3e5e99d592e0383a096970c029d061774c367c16e39ea907203d180f84` |
| `include/m2m/moproj_write.hpp` | `c78629ef0c232809026a66afcf1fcc74c3bf4b93391ed36cfc11cb0e42c42069` |
| `include/m2m/convert_details.hpp` | `4db94f52d5bc907bd13a088a876a9b64b6d122789130c4f69b6fea9c0fb75428` |
| `include/m2m/mst/stream.hpp` | `92ba0fc8ceed8bbe69a23bc43bea1184e191c8fea6506e6122026f8111fc319f` |
| `src/mst/file_device.cpp` | `23a8a7cf973848e09a8fb068d815da2d9a16f4c59db333fb84cf34287a88fca5` |
| `tests/moproj/test_project_output_device.cpp` | `8eb2ae13ab0cf60d3509a601129f13debf738b67841f46a2e198468934e47c2c` |
| `tests/s/test_save_io.cpp` | `c6a2f5c36c928c151cce9575c144f0588e337b64d00291b361b03289d5a75997` |

Token measurements are unavailable.
