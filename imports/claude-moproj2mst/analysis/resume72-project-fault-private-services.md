# CP72 implementation plan: private project-output factory and core call services

Implement the integration seam as a private per-call dependency. Keep ordinary saveProjectFileNative on its existing stack FileDevice path, sharing only the path prelude and opened-device serialization/error-query tail with the injected path. This preserves default allocation/open/close behavior and avoids an unnecessary production adapter. The test-only adapter can delegate real FileDevice I/O and inject a declared post-close error observation to exercise continuation; it cannot establish a physical persistent-flush-fault oracle.

This is a design artifact only. No source/build/test, GUI, mutable REA or shared-state change occurred. Baseline inspected: frozen checkpoint 71 source tree 8242e4b524d8ec86c3c93f650183be2370624a8093d93a27aaa4260f142ee951, source ZIP SHA-256 1a77cf43f76f4d3acc27b4d990dd7527d2c4f1f9cd065cb0895b5d5a8f7f2b22, build analysis/resume71-integrated-grouped2-build.json. Existing frozen counts are save_io 70, file_device 7 and native Type2 observer 2. Source successors may differ; root reconciles before assignment.

## File ownership and dependency order

All implementation paths below are relative to the active component root `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst`. The alternative `optimization/test-speed` copy is outside this write set. Receipt/report/build paths are relative to the converter takeover root one level above `source`.

| Owner | Exact write set | Dependency / release condition |
|---|---|---|
| Root interface owner | New src/moproj/project_output_device.hpp and src/core/conversion_call_services.hpp | Freeze signatures below first; no installed header/C ABI changes |
| Writer worker | src/moproj/write.cpp only | Implements private helper against frozen header; preserves default entry |
| Core worker | src/core/convert.cpp only | Implements private services route/entry against frozen headers; all old entries stay default |
| Test worker | tests/s/test_save_io.cpp only | Uses exact private headers/entry and existing Boolean Rig; no production fault policy |
| Root integration | Build/focused/full freeze receipts | Sole build owner, after all source owners release |

Writer/core/test work is independent after the two headers are fixed. No worker edits another lane, CMake, public settings or unrelated device code. Existing CONFIGURE_DEPENDS discovers source changes; no new .cpp/CMake registration is needed. Do not alter public include/m2m/moproj_write.hpp, convert.hpp, convert_details.hpp, mst/stream.hpp, mst/file_device.hpp or src/dll/abi.cpp for this seam.

## Exact private contracts

In src/moproj/project_output_device.hpp, include m2m/moproj_write.hpp, m2m/mst/stream.hpp and memory. The factory is a borrowed virtual interface, avoiding std::function allocation/default initialization on the real path:

```cpp
namespace m2m::moproj::detail {
class ProjectOutputDevice : public mst::OutputDevice {
public:
    virtual int error() const = 0;
};
class ProjectOutputFactory {
public:
    virtual ~ProjectOutputFactory() = default;
    virtual std::unique_ptr<ProjectOutputDevice> open(const qt::String& path) = 0;
};
bool saveProjectFileNativeWithFactory(
    Project& p, bool withBlobs, DiagramViews& views,
    const std::vector<const engine::FileEffects*>& earlier,
    engine::ConversionSession* session, engine::FileEffects* observation,
    ProjectOutputFactory& factory);
}
```

The existing OutputDevice already supplies write/flush/close/opened and scoped aboutToClose; only error is project/QFile-specific. Do not give every OutputDevice a made-up default file error. A valid factory returns a device object even for opened()==false; null is an infrastructure configuration error, not a native failed-open result.

In src/core/conversion_call_services.hpp, include m2m/convert_details.hpp and forward-declare the factory (the test includes its actual private header separately):

```cpp
namespace m2m::moproj::detail { class ProjectOutputFactory; }
namespace m2m::detail {
struct ConversionCallServices {
    moproj::detail::ProjectOutputFactory* projectOutput = nullptr; // borrowed for this call
};
Result convertDetailedWithServices(
    const std::u16string& moproj, const std::u16string& outMst,
    unsigned flags, unsigned codepage, ConversionDetails& details,
    const ConversionCallServices& services,
    int guiUnit = 0, const std::u16string& deformDir = {},
    const OriginalSettings& settings = {});
}
```

This is a private source/static-library entry with no export macro, installed declaration or C ABI overload. The services object and factory outlive the synchronous call; no pointer/callback is stored on Project, Session, Ctx, OriginalSettings or ConversionDetails, or returned through details. ConversionDetails remains reset output-only value observations.

## Writer implementation map

At the existing native writer near write.cpp:581, extract two unnamed-namespace helpers within that translation unit:

1. prepareNativeProjectOutput(Project&, FileEffects*) performs exactly the existing observation.xmlJournal assignment first, empty/null-path false result and appendSuffix mutation, in that order. It opens nothing.
2. A templated finishNativeProjectOutput(Device&, Project&, withBlobs, views, earlier, session) first returns false if !opened(), then constructs the same local unused FileEffects, invokes the unchanged saveProjectFileBody with &device and finally returns device.error()==0. Both concrete FileDevice and ProjectOutputDevice satisfy the five-method contract.

The installed saveProjectFileNative signature stays unchanged. It runs the shared prelude, constructs mst::FileDevice file(p.path) on the stack exactly where it does now, then calls the opened-device tail. It does not construct a factory, wrapper or new heap allocation on the default route. The serializer still calls close while Stream exists; Stream unwinds before post-close error query.

The private WithFactory function runs the same prelude, calls factory.open(p.path) at that output-open point, rejects null with std::logic_error, then runs the same tail on the returned device. Failed-open objects return false without body mutation. A serializer/owner exception still propagates, retaining the same preceding effects. Do not turn exceptions into ordinary project false or introduce rollback/deferred replacement.

Use a single copy of the serializer/body and result rule; do not clone DOM/Type2 logic for tests. The default FileDevice callback path is not interposed, so saved-initial-flush behavior and error clearing remain unchanged.

## Core implementation map

Add one final private argument to run():

```cpp
const detail::ConversionCallServices* services
```

At the existing native project writer call near convert.cpp:335:

- Keep projectWriteAttempted=true at its existing position.
- If services && services->projectOutput, call the private WithFactory function with all the same project/views/earlier/session/observation arguments and that factory.
- Otherwise call the existing saveProjectFileNative exactly as before.
- Assign projectWriteSucceeded from the ordinary Boolean and set projectWriteReturned=true only after a normal return, including false. Preserve thrown-guard behavior.

Add the private convertDetailedWithServices definition beside existing convertDetailed. It creates the same local MST/outPath buffers and invokes the same run() with writeFile=true, supplied details and services. This avoids duplicating Open, Save, mutex, remesh, cleanup or final result mapping.

Explicitly pass nullptr services from every old call site: settings-based convert, ordinary convertDetailed and both convertToBytes implementations. Wrapper overloads/C ABI continue through those unchanged public signatures. The borrowed service is consulted only at native project output; input Open, KEY/property/MST devices and alias/argument guards stay on their existing routes. The test must request M2M_FLAG_WRITE_SAVE and prove one factory call; an unused service is not a successful injected scenario.

Keep later property traversal and MST/remesh control flow untouched. Existing final project-false mapping to M2M_E_WRITE near line 411 remains after those stages. The new private entry does not insert fault logic into OriginalSettings, details, Session, globals, TLS or environment settings.

## Exact bounded test

Add one TEST_CASE to test_save_io.cpp:

```
save_project_reported_postclose_error_continues_properties_and_mst
```

Reuse its existing default Boolean Rig from 133_B_DBSTEP_BOOL3_before. Run two fresh Rig instances inside this one case: default post-close-error delegation and an explicit declared nonzero error 7. Include the two private headers via ../../src/core/conversion_call_services.hpp and ../../src/moproj/project_output_device.hpp, plus m2m/mst/file_device.hpp and required standard memory headers.

The test factory records the exact expected normalized project path and call count, then returns a test-only ProjectOutputDevice owning a real FileDevice. Forward write/flush/opened to that FileDevice. Install the lower FileDevice aboutToClose callback to record that it arrived while the file is still open and invoke the adapter's protected notifyAboutToClose. Adapter close delegates to real close; it never emits the callback early, pre-flushes or invents an initial-flush event. Teardown closes idempotently and detaches its lower callback before the adapter disappears. Keep recorded observations in factory-owned per-call state so they survive destruction of the returned device; do not inspect a dangling device pointer after conversion. I/O callback code records facts for later Testkit assertions rather than throwing test assertions into destructor paths.

Its const error() records a single query after real close through mutable observation state or the factory-owned record. Record the ordered trace `factory/open → aboutToClose while opened → close returned → error query`; writes can occur before close or from the forwarded stream-flush callback. Do not assume every payload first writes during close. The control returns the real error. The injected variant returns 7 as an explicitly declared QFile post-close status observation, while separately recording the real device error. No throw, native FileDevice setter, environment flag or forced pending-buffer algorithm is used. Require the real device actually opened; otherwise the already-covered failed-open route could falsely satisfy the final error result.

Before each call REQUIRE BWZ/INI/new MST paths absent. At the error query, record whether those later outputs are still absent, then verify generated output after return. Use the existing private services entry with flags WRITE_SAVE/codepage 1252 and an external ConversionDetails:

```cpp
m2m::detail::ConversionCallServices services{&factory};
ConversionDetails details;
const auto result = m2m::detail::convertDetailedWithServices(
    r.project.u16string(), r.mst.u16string(),
    M2M_FLAG_WRITE_SAVE, 1252, details, services);
```

Required assertions after each run:

- Exactly one factory call at the normalized project output path; actual opened true, serialization write observed, aboutToClose forwarded while open, real close completed and exactly one post-close error query. No assertions of an unobserved private initial-flush call.
- details.saveCall/returnedCode have values. projectWriteAttempted/Returned true; Succeeded true for control and false for declared nonzero status.
- BWZ contains WZD_BOOLEAN_CUTTER_OBJECT_NO, INI contains WZD_MACHDIST_OPERATION_BOOLEAN, MST contains END OF MASTER FILE; they were not stale inputs and were absent at the project error query.
- mstWriteAttempted/Returned/Succeeded are true. Control Result/details code exactly M2M_OK; injected Result/details code exactly M2M_E_WRITE with project-write error message, after later outputs. No OK-or-unsupported allowance and no intervening remesh guard.
- No exact .moproj fault-residue assertion. Real delegated writes may complete normally despite the declared status; preserve whatever physical bytes occurred as diagnostic artifacts rather than presenting them as original persistent-fault residue.

This exercises one ordinary false-return integration path plus its control, without changing the default production writer behavior. It does not claim a physically caused native flush error or original parity.

## Validation and release

Static preflight: quoted includes resolve, no raw C assert/unsupported Testkit aggregate printing, factory lifetime is per-call, every old run caller supplies nullptr and public headers/C ABI signatures remain unchanged. Root then groups m2m_s_save_io, m2m_mdt_file_device and m2m_doe_type2_native_open_guard. Against frozen CP71, expected case counts after only this change are 71/7/2, respectively; reconcile other active edits before using those as actual expected totals.

Retain focused outputs and preserve existing project-open-failure continuation, close-clears-error and saved-initial-flush FileDevice controls. Then run the complete required optimized launcher, all source/binary/fixture bindings and immutable freeze before source release. No new CTest registration/CMake edit is required. Do not claim an empirical integrated post-open false result until the new private entry/test actually compile and pass.

The result's precise scope is candidate continuation after a declared post-close QFile error while real close ordering is delegated unchanged. Actual native persistent-error causes, durable partial bytes, allocation/callback failures and fresh original acceptance remain separately qualified. No production fault control or behavior redesign is introduced by this plan.
