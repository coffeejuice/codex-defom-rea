# CP71 architecture review: project post-open QFile-error seam

Approve a private per-call project-output dependency, with one important qualification: the smallest integrated test can inject a declared post-close error observation while delegating real FileDevice close behavior. That tests continuation after the project writer reports false; it is not an observed native pending-buffer failure or durable partial-byte oracle. Do not add public settings, Session fault state or a global injection switch.

Reviewed proposal: analysis/resume71-project-postopen-fault.md, SHA-256 864cd396c70abbd98ec7344f74c67ab9c5588fa0e4fce6a3bc6f3001de3dc392. Read current project writer, OutputDevice/Stream/FileDevice, conversion coordinator and ConversionDetails source plus the retained wave3-project-close-status-contract.json. No source/build/test/shared-state change, execution, GUI or mutable REA action occurred; only this report was written.

## What the existing code already supplies

mst::OutputDevice provides virtual write, flush, close and opened plus a scoped aboutToClose callback. It has no QFile.error method. mst::FileDevice is final and supplies its own error(). Widening OutputDevice to give every unrelated stream device a fabricated default file-error value is unnecessary.

saveProjectFileNative currently preserves the important order: observation-alias assignment, empty-path early result, appendSuffix, real FileDevice open/truncate, saveProjectFileBody, explicit output close while Stream is alive, Stream destruction, then file.error()==0. Failed open skips the body. Body/owner guards throw rather than become an ordinary false write result.

FileDevice::close first saves its own buffer flush result, then notifies aboutToClose while still open, allowing Stream to flush. It discards pending buffers, closes the handle and only conditionally clears fileError according to the saved initial flush result. A later stream callback error is not automatically persistent. The project writer queries post-close QFile error, not sticky Stream status. These are statically qualified native contracts; existing direct locked/device tests cover important candidate branches but not the desired integrated post-open false case.

convertDetailed currently has no device-factory argument. Its existing observation-only ConversionDetails is sufficient for result assertions but cannot inject this seam. The coordinator records a normal false project result, continues property writers/MST/remesh and maps the project failure to M2M_E_WRITE afterward. A thrown test error would bypass that path and would not test it.

## Minimum private interface

Use a private source header, for example src/moproj/project_output.hpp, with a project-specific subtype:

```cpp
namespace m2m::moproj::detail {
class ProjectOutputDevice : public mst::OutputDevice {
public:
    virtual int error() const = 0;
};
using ProjectOutputFactory =
    std::function<std::unique_ptr<ProjectOutputDevice>(const qt::String&)>;
}
```

The inherited four methods and one QFile-specific query are sufficient. Do not add persistentError setters, raw fault counts, a new global status, or a generic all-writer service registry. A factory is preferable to a preopened-device argument: device construction must remain at the existing normalized-path/open point, after the real input Open has completed, not at test setup time.

Keep the existing installed/public native writer signature as its ordinary production entry. It calls a private implementation with the real adapter factory. The private implementation retains all current early checks, suffix mutation, alias assignment, serialization and post-close query order. Factory output must be nonnull; an invalid factory contract is an infrastructure error, not a synthetic native failed-open result. A normal failed open is represented by a real device object with opened()==false.

The default adapter owns one real FileDevice and delegates all I/O/error calls. Its lower FileDevice aboutToClose callback forwards to the adapter's notifyAboutToClose; the outer Stream is attached to that adapter. close() delegates directly to FileDevice::close and does not notify early or pre-flush. Otherwise initial-flush/error-clearing order changes. Ownership must keep the adapter alive throughout the lower callback, detach callbacks safely during destruction and make repeated close idempotent, matching FileDevice's no-effect second close. Do not close or destroy the device before Stream's scoped callback has unwound.

## Integrated test access without a production hook

For later property/MST continuation, a writer-only factory is insufficient: the normal convertDetailed entry cannot receive it. The narrowest integration is a private call-services argument to core run(), containing only this project-output dependency, plus a private static-library test entry declared under src/core/. All existing convert/convertDetailed/C ABI entries pass the real/default service. The test entry must invoke that same run/coordinator code, not duplicate the Save sequence.

Do not route this through OriginalSettings, ConversionDetails, environment variables, process globals, thread-local setters or a mutable Session factory slot. OriginalSettings represents original behavior, ConversionDetails is reset output-only observation, and Session owns reconstructed runtime state; turning any into a fault selector broadens their contracts. A private source-only per-call dependency avoids a product/runtime control surface and remains isolated to one conversion. No exported C ABI or installed public fault overload is needed.

If a private coordinator dependency is considered outside the approved change, stop at the direct writer test. Pair it with existing open-failure continuation/source evidence, explicitly without claiming empirical post-open integration. Do not imply an unchanged public convertDetailed call can somehow receive the new factory.

## Smallest honest error scenario

Prefer a test-owned adapter/decorator that delegates actual disposable-file writes and exact real close behavior, then returns a scripted nonzero QFile-error observation only when the post-close query occurs. Record whether construction opened successfully, whether real close completed and when error was queried. The production adapter remains unchanged. This models a reported post-close nonzero status, not the physical cause of that status.

A constant error result is not sufficient evidence by itself: require opened()==true, the serializer/body was reached, close happened before the single status query and the injected nonzero value returned normally. Otherwise the test could accidentally exercise the already-covered failed-open branch. The injection must not throw; it must produce the writer's ordinary false result so later stages remain reachable.

If the desired claim is specifically initial native QFile-buffer flush failure persisting through aboutToClose, retain the existing real FileDevice unit evidence or develop a separately qualified lower-native-I/O seam. A wrapper cannot force that physical state merely by overriding error(), and a wholly scripted device that reimplements close does not independently validate FileDevice's native close implementation. Do not call the minimal status-observation integration test native persistent-error parity.

Do not enlarge/alter the project payload merely to make a particular buffer-size assumption true, introduce eager flushes or assert original .moproj partial bytes. The output's exact physical residue may be fully written under the status-decorator model; it is not an original fault-residue oracle.

## Acceptance checks using the existing Boolean Rig

Use tests/s/test_save_io.cpp's known Boolean Rig already used by save_project_open_failure_preserves_project_but_continues_boolean_and_mst. Select only its project output; leave input readers, KEY/BWZ/INI/MST devices on their normal paths. Pin the requested normalized project path and require exactly one factory call. An error-query observer can verify later property outputs are still absent/sentinel at that instant, then the caller checks their actual generated contents after return. This avoids counting stale files as continuation.

Require the following independent outcomes:

- Real input Open succeeded; project device opened and serialized/closed before its nonzero post-close error was returned. Stream status is not substituted for QFile error.
- details.saveCall exists; projectWriteAttempted and projectWriteReturned are true and projectWriteSucceeded is false.
- Later BWZ contains WZD_BOOLEAN_CUTTER_OBJECT_NO and INI contains WZD_MACHDIST_OPERATION_BOOLEAN, with fresh/sentinel setup preventing stale-output passes.
- MST contains END OF MASTER FILE; mstWriteAttempted, mstWriteReturned and mstWriteSucceeded are true. No remesh guard intervened for the qualified Rig.
- The final Result and details.returnedCode are exactly M2M_E_WRITE after continuation, not an injected exception or an allowed unsupported status.
- A default/zero post-close-error control retains normal project success with the same fixture. Keep existing close-clears-error and FileDevice saved-initial-flush tests; they cover a different branch.

Use typed call/result observations and actual artifacts, not sleeps or a racing lock monitor. The existing process-wide conversion mutex remains unchanged. Return-device/journal/trace lifetimes stay per call; any escaped diagnostics must own values, not Project/Session/device callbacks.

## Decision and scope

The proposal is viable with the narrow private subtype/factory and exact callback/lifetime discipline above. Its broader Session-factory suggestion should be replaced by private per-call core dependency if empirical continuation is required. No production test hook or public configuration is justified.

The resulting candidate test can establish continuation after a declared post-close QFile-error result. It cannot establish original physical fault persistence, durable byte residue, arbitrary buffering behavior, public C ABI fault controls or fresh original lifecycle parity. Keep those qualifications with the preserved full source/build/test freeze; no behavior redesign, error propagation change or optional public interface is required by this review.
