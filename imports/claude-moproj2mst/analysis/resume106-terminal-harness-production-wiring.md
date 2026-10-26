# CP106 private terminal vertical-cut wiring

The next slice wires the frozen writer service to an explicitly owned conversion worker. Existing convert/C ABI/product CLI calls remain null-service and keep their guarded failure. Preserve all passing v1 safety tests; add an explicitly selected v2 protocol for the real reconstructed remesh boundary. This is an isolated candidate fixed endpoint, not original default-handler-prelude recovery.

## Bound baseline and writer interface

CP105 full receipt is `analysis/resume105-integrated-grouped-repair2-tests.json`, run `build/test-runs/1792906954802152100`. Its terminal safety log records0 of11 failed,2.455s in that full run; executable before/after SHA-256 `df37327a20c4f19f1f13ea921508d8bcf3044929577b8e2a2e92050a7fe4c92d`, log SHA-256 `d08c78474ee1909fa494188e5ade01c6573aa38c5a5fb35102a93f80ca77f088`. Root separately reported a focused2.48s. CP105 freeze source tree is `45f48bb02d844e35b23381dfb758ac92c51d820d8abc1c6894b65e0e3d0d9575`, archive `fcd7eca2cf1993d26d4175c92b77381ce7f6a30f300496e7c11e5bd66e8abf50`. These are retained execution receipts, not executions by this reviewer.

Editor300 has released private `src/engine/simcontrol300_remesh_services.hpp`, SHA-256 `1245df3d36fe7d7e16a40d201f459adaa9a74914eac71d5e826bee6fb619b585`. Its namespace is `m2m::engine::simcontrol300::detail`. The fixed request has site FirstNullFilePrintf, proof NativeFreshDefaultStartup, sourceAddress1400abcf0, expectedProcessStatusC0000417, crtErrno22 and ordinal1. Service methods are profile() noexcept, dispatch(request) noexcept returning RemeshTerminalDispatchFailure, and failClosed(request,failure) noexcept. Private writer sibling takes a borrowed service pointer; public writer delegates nullptr.

On admitted failed open, the writer sets EINVAL22 and calls dispatch while scopes remain live. Any returned tag is normalized, failClosed is called once, then the writer parks in Sleep(INFINITE). Neither callback has a false noreturn promise. The supervisor must stop a parked failure; no throw, return to Save or normal Close is possible on that admitted route. Unknown/Custom/null remain guarded. Do not alter this released interface from the harness lane.

## Exact root-owned core bridge

Root adds a source-private `src/core/remesh_terminal_call_service.hpp` containing `m2m::detail::RemeshTerminalCallService` with the same profile() and two methods accepting the leaf request plus **const engine::SaveCallState&**: dispatch(request,snapshot) noexcept returns the leaf failure enum; failClosed(request,failure,snapshot) noexcept returns void. No public header or result layout changes.

Forward-declare that class and append `RemeshTerminalCallService* remeshTerminal = nullptr` after projectOutput in ConversionCallServices. Existing `{&factory}` aggregate initializers retain their meaning. All public overloads already pass null services; C ABI header/version/exports and normal CLI need no change.

At convert.cpp's existing after-MST remesh call, root constructs a local leaf-service forwarder only when services->remeshTerminal is nonnull. The forwarder holds references to the downstream service and live saved.call. profile forwards directly. On dispatch, it copies live saved.call into an inline value member, then passes that copy to downstream dispatch. failClosed reuses that same captured value. The leaf writer invokes failClosed only after dispatch returned, so the snapshot exists; no pre-Save or after-return snapshot is substituted.

Call the released private writer sibling with &forwarder at precisely the old call position. Keep current Session owner, actual current directory and KnownC arguments. The null route calls the existing public writer. Project/MST false mapping remains after the writer call. Do not fill details.returnedCode or move dispatch to the run catch/result tail: actual termination never reaches those assignments.

| Owner/value | Lifetime |
| --- | --- |
| run's Session and saved.call | Enclose the writer call; remain live during dispatch/park |
| local core forwarder and inline snapshot | Enclose the synchronous private writer call |
| worker endpoint service | Constructed before convertDetailedWithServices and survives it/park |
| inherited protocol handle | Worker-owned until exit; only bounded endpoint records use it |
| parent prepared fixture/denial handles | Retained until child stopped and manifests captured |
| child job/process ownership | Established before resume; stopped before fixture lease release |

No pointer from this chain enters Project, Session, RemeshState, returned details or wire records. Endpoint code does not reenter conversion, invoke GUI callbacks or mutate model state.

## Explicit v2 wire layout

Keep v1 Frame40/Collector/SelfTestSite1 behavior unchanged. The supervisor selects an expected protocol profile before launch; never auto-upgrade from incoming bytes. Proposed RemeshV2 frame is64 bytes, little-endian, with the same magic and version2:

| Offset | Width | Meaning |
| --- | ---: | --- |
|0|4|magic3548544d |
|4|4|version2 |
|8|4|kind: Started1, Returned2, Boundary3, BackendFailure4 |
|12|4|site2 = reconstructed remesh first-null-FILE printf; never self-test1 |
|16|8|per-run nonce |
|24|4|Boundary expected statusC0000417; Returned M2M0..7; otherwise0 |
|28|4|Boundary EINVAL22; failure stable failure tag; otherwise0 |
|32|4|Started child PID; Boundary ordinal1; otherwise0 |
|36|4|Boundary wire profile1 FreshDefault; otherwise0 |
|40|4|Boundary proof1 NativeFreshDefaultStartup; otherwise0 |
|44|4|Save snapshot flags described below |
|48|4|checkedSubprojects |
|52|4|raw DialogAnswer answer |
|56|4|raw DialogAnswer pendingDefault |
|60|4|checksum over bytes0..59, same FNV convention as v1 |

Save flags: bit0 snapshot-present; bit1 opened; bit2 validationComplete; bit3 lastOperationModalPending; bits4/5/6 project attempted/returned/succeeded; bits7/8/9 MST attempted/returned/succeeded. Reject all higher bits. Preserve succeeded values as recorded, but interpret only when attempted and returned. Do not reinterpret native DialogAnswer::No(10000h) as adapter failure; copy raw answer values and validate membership in the current enum. Empty snapshot means all snapshot words0, not fabricated default succeeded=true.

Started has no snapshot. Boundary requires one, both project/MST attempted+returned, known site/profile/proof/status/errno/ordinal, and a coherent snapshot. Fixture assertions additionally require opened/validation complete and no pending last-operation modal; preserve exact checked-subproject count/answers from the selected fixture/control. Returned may carry a snapshot if detailed conversion returned with one; legacy ABI returned cases have none. BackendFailure carries an explicitly mapped nonzero PublicationFailed/EndpointReturned/InvalidFailureCode tag, not a cast of enum ordinals or an errno substitute. Its other source/snapshot words are0. Allow at most Started,Boundary,BackendFailure (or Started,Returned); cap bytes/frames and reject v1/v2 mixing, wrong site, stale nonce, duplicate/conflicting outcomes and reserved bits.

Parent accepts CandidateProcessTerminal only from valid v2 Boundary + exact natural child statusC0000417 + no Returned/failure/supervisor stop. It remains distinct from v1 SelfTestTerminal. Failure/timeout is sticky even after a terminal-looking exit. Keep qualification `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`; the original default remains a static source prediction.

## Endpoint dispatch and failure

The worker endpoint implements the root core service. profile is the case's explicit private selection, never the host CRT handler. dispatch validates the fixed leaf request, serializes the supplied snapshot into one bounded v2 Boundary, publishes it, then calls the fixed own-process endpoint. It uses stack values/arrays, no exceptions or allocating formatting inside noexcept dispatch.

Publication failure returns PublicationFailed. If the process endpoint unexpectedly returns, return EndpointReturned. failClosed emits at most one best-effort BackendFailure record and returns to the writer's already-defined parked state. It must not call the expected terminal status after a failure, because that would obscure failure precedence. The parent stops that owned child promptly on a valid failure record, otherwise on the existing deadline. There is no ordinary Result or Close attempt from the failed backend.

Only Started plus at most two small records are written on the normal/failure route, well below the dedicated pipe capacity; retain the parent deadline/available-byte reader, file captures/caps, suspended job ownership and restricted inheritance from CP105. Do not move IPC into the writer or reset the failure state during cleanup.

## Supervisor preparation and worker role

The passing supervisor currently creates an exclusive empty leaf and rejects an existing directory. A preseeded project cannot simply be passed to that API. Add a harness-only bounded preparation phase after canonical leaf creation and before launch: prepareOwnedCase(leaf) creates the copied project/config and returns a parent-owned PreparedCase lease containing denial handles and case metadata. It cannot change the executable, worker role or allowed root. Hold that lease until confirmed child stop and post-run manifest capture. Declare it before the child ownership guard or explicitly stop/wait before reset, so an exception cannot release write denial while a live child is still saving.

Keep the self-test role unchanged. Add a distinct explicit remesh-conversion worker role, expected RemeshV2 decoder and finite case enum; each still requires termination opt-in, own-image/hash verification and nonce/PID handshake. Use fixed fixture-relative names or a bounded job file, never shell-built user commands. A new `tests/s/test_terminal_remesh_worker.cpp` can contain both Testkit parent and explicit child entry. It links the core and the private root service; no product CLI change is needed.

For real binary ABI compatibility checks, the child can load the pinned adjacent candidate DLL with restricted DLL search and resolve the existing wide/UTF8 exports. Record DLL/deformdb hashes before/after and the actual loaded DLL path; do not claim DLL compatibility from statically linked wrappers alone. These legacy cases supply no terminal service and must return7 normally on denied remesh. Parent expected artifact identities must come from root's build receipt; measured own-image hash alone is not independent source binding.

## Fixture and focused acceptance

Prepare a copied supported project with a valid primary KEYWORD_DRIVEN remesh file, supported fields and DONE. A parent read handle with read sharing but no write sharing permits Open and blocks later write-open before truncation. Validate that the selected fixture has no earlier writer of this path and that actual Session Open enabled this owner; record that reach with v2's source boundary plus earlier Save snapshot, not a fabricated state. Do not use an invalid sentinel or a lock that makes Open fall back/disable remesh.

The vertical cut needs: unblocked normal control; denied-remesh terminal case with earlier project/MST returned success; denied remesh after earlier project/MST returned failures; Unknown/Custom guarded7 without Boundary; publication/endpoint-return fail-closed child cases; and wide/UTF8 legacy DLL returned7 controls. Require pre/post output manifests, exact unchanged remesh sentinel only for this pre-truncation denial, and reached attempt/success bits. A project-output service can provide a scripted earlier project false if labeled core-only; physical project/MST denial must retain read compatibility and source ordering. A no-Boundary same-status crash never passes.

Extend protocol tests for exact full snapshot round trip, reserved bits, stale proof/profile/site, v1/v2 mismatch, malformed/truncated v2, failure after Boundary and timeout after complete Boundary. Rerun all11 v1 safety cases unchanged, writer contract tests,31-write/ignored-close regression, numeric/errno targets, required full source-stable launcher and freeze. A helper marker proves only helper-scope behavior, not Session destructor or completed normal Close parity.

## Exclusive ownership and release cut

Editor300 owns/releases remesh.cpp, simcontrol300_remesh_services.hpp and its writer contract test; harness work must not edit them. Root owns the new core call-service header, conversion_call_services.hpp, convert.cpp and artifact/build registration. The harness lane may exclusively own terminal_harness_protocol.*, terminal_harness_supervisor.*, a new v2 protocol/helper if kept separate, and the new conversion worker test. Keep the existing safety test on hold unless root explicitly grants a needed compatibility-test amendment. Agree names and v2 enums before parallel edits.

This report is the design release, not source activation. No source/test edits, builds/tests, GUI, original/native target execution or REA mutation occurred. Current writer prefix is isolated until root's core bridge and the owned worker consumer are integrated and validated together; public C ABI/product CLI remain guarded throughout.
