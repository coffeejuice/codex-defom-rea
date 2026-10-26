# CP110: bounded native invalid-parameter decision model

The smallest honest next model covers the native null-FILE decision up to the first unimplemented default-handler prelude operation. It can gate and explain the existing isolated worker endpoint while leaving that endpoint explicitly a candidate policy. It must not turn a selected FreshDefault enum or matching exit code into an observed original handler or completed native prelude.

## Model boundary

Keep three distinct records: (1) a source-model frame for one first-null-FILE fprintf occurrence; (2) an isolated-worker execution outcome; (3) an optional future original-process observation. They must not share a success Boolean. The current leaf request and root SaveCallState snapshot already locate the source occurrence correctly; preserve their public/private compatibility and wire semantics. The model is consumed at that existing occurrence, never at unrelated startup or after Save unwinds.

The native entry is not a failed-write ordinary return. fprintf14146da0c first tests null FILE, obtains errno storage and writes22 decimal/0x16 before dispatch. Dispatcher141470568 then reads and decodes EXE slot1446ffb40. Known decoded null selects the default branch; known nonnull selects a custom call; unknown cannot select either. Neither selection establishes what the handler later does. The model does not install a host CRT handler, execute DecodePointer on a fabricated address, invoke an original function or modify the embedding process.

## Minimal private value/state contract

Proposed source-private types, with names subject to root ownership agreement:

- `HandlerEvidence`: Unknown; StaticFreshDefaultAssumption; SameOriginalModuleDecodedNull; SameOriginalModuleDecodedCustom. The last two require a separately retained same-call/module witness; today's worker cannot manufacture them. StaticFreshDefaultAssumption expressly means the source-proved startup encoded-null installation plus the unobserved no-later-replacement condition.
- `NativeHandlerFrame`: fixed site0x1400abcf0/ordinal1, native modeled errno22, evidence kind, selected branch, modeled stage, next native operation, optional predicted OS status, and a qualification enum. No native pointer integer is required for the static assumption. It is one occurrence value, not a process registry or a callable handler.
- `NativeHandlerStage`: ErrnoWritten; CustomDispatchPending; DefaultPrefixComplete. These are source-model stages, never labels for actual native execution.
- `NativeHandlerNext`: DecodeHandlerUnavailable; InvokeCustomHandlerUnimplemented; CaptureContextUnimplemented. An absent input/refused edge retains its exact guard.
- a pure `modelFirstNullFilePrintf(HandlerEvidence) noexcept` returning the frame. No allocation, callbacks, host errno writes, stack unwind or OS calls; repeated evaluation has no repeated native side effects.

ErrnoWritten models the native22 store before handler classification even for Unknown/Custom. This is intentionally distinct from the legacy public adapter guard, whose existing host errno behavior remains unchanged. The isolated admitted leaf already sets modeled host errno22 at dispatch. Do not write22 earlier in every legacy call as a side effect of adding this model.

Decision table:

| Evidence | Modeled native prefix | Exact next edge | Status meaning |
| --- | --- | --- | --- |
|Unknown|first FILE check and errno22 store|handler decode unavailable|none |
|StaticFreshDefaultAssumption|errno22, assumed decoded null, debug dword1450540e8 cleared0|CaptureContext unimplemented|predictedC0000417 under the stated static profile; no observation |
|SameOriginalModuleDecodedNull|errno22, witnessed decoded-null branch, modeled debug dword clear0|CaptureContext unimplemented|same source prediction; witness alone does not prove later execution |
|SameOriginalModuleDecodedCustom|errno22 and custom selection|custom call unimplemented|unknown; no forced default status |

DefaultPrefixComplete means only the dispatcher has selected default and its preceding1414834f0 leaf's single0 store has been represented. It does not mean any RtlCaptureContext/filter/termination effect occurred. For current scope, recording that native flag effect belongs to the pure frame; writing an editor-local or host substitute global would invent ownership. The actual original target is module storage1450540e8; a future executable native model needs its real owned counterpart before mutating it.

Invalid evidence enum values should conservatively produce the Unknown decision. A raw custom pointer's numeric zero/nonzero is insufficient without its actual module/sequence ownership. Do not accept stale prior-call witnesses or a host CRT's state as original evidence. Full original module registry/lifetime binding is an independent dependency, not required to express the existing static prediction.

## Stack, prelude and OS status

The source prelude sequence after the bounded prefix is known: initialize exception record; capture a local CONTEXT; RtlLookupFunctionEntry on captured RIP; conditionally RtlVirtualUnwind or adjust the copied context from the caller return; set exception codeC0000417/flags1/address; IsDebuggerPresent; SetUnhandledExceptionFilter(NULL); UnhandledExceptionFilter; possibly repeat the debug0 store; GetCurrentProcess; TerminateProcess(C0000417).

**RtlVirtualUnwind here updates the captured context for the error report. It does not unwind the live Save stack or run C++ destructors.** Do not give a `stackUnwound=true` field to this stage. A future extension needs explicit captured context/RIP, runtime-function lookup result, image base, unwind metadata/stack memory and resulting context before claiming that operation. The absence-of-entry alternate branch still needs the actual caller return/stack inputs. A zero-filled synthetic CONTEXT or invented instruction pointer is not an acceptable witness.

The current model stops before those operations. A separate *plan* can list them, but their effect ledger must remain uncommitted. It should distinguish requested native exception status from observed process status: setting local record.ExceptionCode toC0000417 does not itself terminate a process. The native function has an epilogue if TerminateProcess unexpectedly returns. Source predicts process death under successful ordinary Win32 termination; an actual observed status belongs only to the parent process receipt.

The candidate worker's publication/TerminateProcess call skips the native prelude. On publication failure or endpoint return, its BackendFailure plus parked writer is candidate fail-closed policy, not native default-handler behavior. Preserve sticky supervisor failure/timeout; never upgrade a parked or failed worker to native success because it later has the same code. Actual child statusC0000417 plus valid request proves the existing fixed-endpoint worker occurrence, not the original CONTEXT/filter/debug-global sequence.

## Smallest integration cut and exclusive sets

After root agrees names, a model lane can own NEW `src/engine/remesh_native_handler_model.hpp/.cpp` and NEW `tests/mdt/test_remesh_native_handler_model.cpp`, without altering public interfaces. This is not an executable feature by itself. To make it reviewable in the current worker, a separately reserved worker lane changes `tests/s/test_terminal_remesh_worker.cpp` to consume the model at dispatch and retain its evidence/stage/next-operation qualification in the existing diagnostic ledger before endpoint publication. Do not broaden v2 Boundary meaning or change its stable profile/proof integers merely to carry a modeled prefix. A model result alone can never pass the terminal classifier.

The worker maps its explicit FreshDefault selection only to StaticFreshDefaultAssumption; it must never construct SameOriginalModuleDecodedNull. Existing Unknown/Custom no-dispatch routes remain guards. To expose their pure model results, use pure focused tests or explicitly labeled parent diagnostics rather than calling the refused endpoint. The root live SaveCallState snapshot remains copied once at the existing after-MST occurrence; do not add model evaluation before actual remesh Open/Save or create another remesh owner.

No changes are needed to C ABI exports/status values/API version, product CLI, public RemeshSaveReport, conversion result/details layout, leaf service signature, root forwarder or ordinary null-service path. Source activation of Rtl/context/filter APIs is outside this cut. Existing selected-palette and other public editor guards stay unchanged.

## Focused tests and gaps

Release-active pure cases should cover: Unknown errno-before-decode ordering with no chosen status; static-default prefix/guard/predicted status; witnessed-null requiring distinct provenance while retaining the prelude guard; custom branch with no default flag clear/status; invalid enum refusal; deterministic repeat evaluation with no host errno mutation; no accidental observed-status/unwind/Close flags. Use explicit assertions of modeled versus observed fields rather than only comparing a default object to itself.

Worker regression should retain its unchanged actual denied-remesh occurrence/snapshot and verify the diagnostic says StaticFreshDefaultAssumption/DefaultPrefixComplete/CaptureContextUnimplemented. Require the existing boundary+natural status for fixed-endpoint acceptance and retain no-return/failure cases; diagnostics alone cannot prove a terminal outcome. Unknown/custom guard cases and product C ABI compatibility remain independent existing requirements. No Testkit case should call an admitted endpoint in-process.

Irreducible current gaps: same-original-run decoded handler after Open; actual enabled singleton and Save CWD/null FILE in B133 overlay; original debug-global owner at execution; captured context/unwind metadata/caller-stack values; debugger/filter and external API occurrence/side effects; native default-handler termination observation. CP109 records the exact original witness. None is closed by a candidate symbolic flag, a helper type, a successful build or a matching final fixed-endpoint code.

## Evidence and release

Based on CP109 report SHA387242795bf33d991524879f1c6c6378aeabfabd284c5dd97b6353da331eebb6, its independently recomputed original SHA/ranges/imports, and retained startup contract SHAd53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b. Existing private leaf header SHA1245df3d36fe7d7e16a40d201f459adaa9a74914eac71d5e826bee6fb619b585; root call-service header SHA0f3f3cdd1ed46ab92521a3d15273983fa3d36b8439dbb43d432afdda7d580e81.

Only this analysis report was written. No source/build/test, GUI, original execution or mutable REA operation occurred. This design recovers a bounded decision model and prepares an explicitly qualified worker consumer; it does not claim full native handler or public editor completion.
