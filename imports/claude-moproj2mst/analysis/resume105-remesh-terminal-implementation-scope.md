# CP105: smallest isolated remesh terminal implementation scope

Add one opt-in source-private failed-open route inside the remesh writer, consumed only by the separate owned candidate worker. Keep ordinary C ABI1 calls, product CLI, public remesh function signature/report layout and existing unsupported result unchanged. This is a scoped implementation proposal for after root freezes CP104; no source activation is authorized by this report.

## Current source and actual reach

`ConversionCallServices` currently contains only a borrowed `projectOutput` pointer. `convertDetailedWithServices()` forwards that per-call object to `run()`; ordinary `convert`, `convertDetailed` and byte conversion pass null services. The actual native Save branch sets `saved.call.mstWriteAttempted/Returned/Succeeded`, then calls `saveRemeshAtProjectSave(session.remeshState(), actual current directory, KnownC)` before mapping earlier MST/project failures. This is the correct live Session owner and occurrence; no new synthetic RemeshState or early field snapshot is needed.

The remesh writer checks enabled before its KnownC guard, opens its stack `CoggingFile`, and only on failed enabled open returns UnsupportedSnapshot with invalidParameterReached. Successful open uses its existing31-attempt body and ignored Close result. The qualified terminal dispatch belongs inside that failed-open branch, before returning its report or leaving the writer scope. It must not occur after `convert.cpp` receives the report or throws.

The preserved legacy behavior is intentional: ordinary null-service failed open still returns the existing guard and does not silently change caller survival, errno or cleanup. For the admitted owned worker only, dispatch models EINVAL22 (0x16), first format attempt ordinal1 and native expected status0xC0000417. Numeric ERANGE34 is unrelated. The worker endpoint remains explicitly `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`.

## Proposed private signature and ownership

Use NEW `src/engine/simcontrol300_remesh_services.hpp`; leave `include/m2m/engine/simcontrol300_remesh.hpp` unchanged. Proposed declarations in the source-private engine detail namespace are:

- `enum class RemeshTerminalProfile { Unknown, FreshDefault, Custom };`
- a fixed value `RemeshTerminalRequest` carrying source-site enum FirstNullFilePrintf, qualified profile/proof identifier, errno22, expected unsigned status0xC0000417 and attempt ordinal1;
- a borrowed `RemeshTerminalService` exposing the explicitly selected profile and synchronous endpoint dispatch; no pointer is retained in Session, Project, RemeshState or ConversionDetails;
- `RemeshSaveReport saveRemeshAtProjectSaveWithTerminalService(const RemeshState&, const std::string&, NumericLocale, RemeshTerminalService*);`.

The existing public `saveRemeshAtProjectSave(...)` remains an out-of-line wrapper calling the private implementation with nullptr. Both routes share one writer body; do not copy/reimplement the31 calls or duplicate its enabled/locale/path checks. A nonnull service alone does not admit termination: Unknown and Custom must take the ordinary guarded failed-open route without invoking its endpoint or setting the modeled terminal errno. A pure request classifier can test that policy without starting a process.

Keep `SaveCallState` out of the leaf writer request. Root's private synchronous forwarder at the after-MST callsite borrows `saved.call` for this single call and copies it into the full worker protocol event when dispatch is reached. That snapshot records only completed/attempted earlier effects; it must not claim remesh returned, Save returned, normal Close entered or returnedCode. No callback enters Session-owned state or rewrites outputs. Do not preconstruct a stale earlier snapshot.

The exact dispatch interface remains a coordination point with the endpoint owner. A C++ `[[noreturn]]` declaration alone does not enforce an arbitrary virtual backend's contract; returning through it can create undefined behavior. The implementation must define what happens if publication/endpoint unexpectedly fails or returns: publish a distinct HarnessFailure where possible, remain inside the already opted-in worker, and never continue Save, throw/unwind into the ABI, call normal exit/abort, or report ordinary conversion completion. The supervisor watchdog can classify/stop that owned failed worker. Root/backend owner must agree a bounded, testable fail-closed mechanism before activating this branch. An arbitrary returning fake endpoint is not a permitted leaf-test shortcut.

## Exact proposed exclusive files

After CP104 source/archive freeze and signature agreement, reserve these for the editor300 writer lane:

1. existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`;
2. NEW `source/apps/moproj2mst/src/engine/simcontrol300_remesh_services.hpp`;
3. NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_terminal_contract.cpp`;
4. a unique implementation receipt under analysis.

This set contains no public header, CMake, ABI or CLI edits. Its focused Testkit target is discovered by the existing mdt glob. The test file must exercise only pure profile/request classification and ordinary guarded/disabled/success controls; no in-process test dispatches an admitted nonreturning endpoint. Actual admitted dispatch belongs in the isolated process suite.

Root separately owns the shared integration set: `src/core/conversion_call_services.hpp`, `src/core/convert.cpp`, CMake worker registration and parent harness/build-receipt wiring. Add a nullable terminal service member after the existing projectOutput member so current aggregate initializers such as `{&factory}` retain their meaning. Internal conversion services are source-private; do not change `Result`, `ConversionDetails`, `RemeshSaveReport`, ABI export lists, API version or status enums merely to transport a result which cannot return. Root should reserve any new private wrapper/header needed for the full worker event before endpoint code is written.

The terminal/backend lane owns NEW `tests/tools/remesh_terminal_worker.cpp` (outside the ordinary test glob), its new private protocol/backend files and classifier tests, with root-confirmed names before writes. IPC, handle inheritance, job/process supervision and fixed Win32 endpoint stay outside the writer. The parent script proposed in CP104 remains `admin/test_remesh_terminal_route.py`; root must coordinate it with the trusted hash/frozen-build launcher rather than weakening hash binding. No concurrent lane edits the editor-owned writer files.

## Acceptance cut and guards

The minimum integrated slice comprises private classifier/dispatch placement, live SaveCallState forwarding, the dedicated worker endpoint, bounded supervisor/protocol, unblocked control and denied-open terminal observation. A writer-only service/symbol without that consumer is a source prefix, not public behavior recovery. Existing product entry points remain guarded throughout.

Retain all CP104 focused cases: unknown/custom classification without endpoint; normal worker completion; denied remesh after actual Open with earlier Save attempt snapshot; earlier returned project/MST failure dominated by terminal dispatch; legacy wide and UTF8 returned7 compatibility. Require Started/TerminalBoundary with correct nonce/site/profile, exact child statusC0000417, no Returned record, unchanged remesh sentinel only under the proven pre-truncation sharing denial, and prior artifact/control manifests. The parent should hold a read-share/write-denial handle on valid primary KEYWORD_DRIVEN input, not on a malformed sentinel; Open must read it and establish this Session's enabled owner before Save fails. Inheritance must exclude that denial handle. Incomplete/conflicting protocol, same status without boundary, boundary with ordinary exit, backend failure and timeout are failures rather than terminal success.

No post-call marker is weaker than a destructor observation. Assert placement before unwind from source plus real endpoint/status; do not claim full native default-handler prelude, Session destructor parity, accepted normal Close or public C ABI termination. Existing31-write/Close ignored-error regression and all CP98–102 reader/errno admissions must remain unchanged. Selected palette/public editor construction, native nonregular/resource paths, wider raw spill and custom-returning invalid-parameter handler remain guarded.

## Evidence qualification and release

Reviewed CP104 harness design `resume104-remesh-isolated-terminal-harness-design.md` SHA400e282d2d5f7cdab1c8421b21c53d0c7f38eeb0dd5646b46c483f2cb71388b2, CP103 architecture SHA cc474b1ab6203d2196ae5e79010bb84a6785485125c60a3773c09ec683a53f24, and native startup contract SHA d53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b. The native default-handler evidence is static under fresh-default/no-subsequent-handler qualification, not observed original termination. Current remesh source remains CP102 SHA39249f84faa6bef97e53504b7fb22f632a539d42cc7b5270a7bd3c553321814f.

Only this report was written. No production/test source, build/test execution, GUI, original execution or mutable REA changes occurred. CP104 remains under root validation; the exclusive write set above is a proposal until root grants it after freeze.
