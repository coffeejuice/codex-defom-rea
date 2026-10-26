# CP103 remesh terminal-outcome architecture audit

The next increment needs an execution contract, not another returned error. In the source-qualified fresh-default profile, enabled remesh Save with a null fopen result terminates the original process with status `0xC0000417`. Returning M2M_E_WRITE, M2M_E_UNSUPPORTED, or a newly named integer after normal cleanup cannot establish that native outcome. Keep the active refusal until the dependencies below are resolved; this report enables no terminal path.

## Evidence and exact boundary

The frozen editor300 report is `resume103-editor300-public-failure-next.md`, SHA-256 `87db55d86b8ae902cb975fe7422c5d58c5facd4d0a606324ae3f9ca854ae9bc6`. Its startup contract is `wave3-remesh-invalid-parameter-startup-contract.json`, SHA-256 `d53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b`; Save lifecycle contract SHA-256 is `dd596017d3cf2b0cde72a666ae7e4046080f2baf3816daa03cd6b066979d28c6`.

I inspected retained writer, fprintf, dispatcher, default-handler and startup records and independently section-mapped the imported original PE, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Additional bounded half-open byte hashes:

| Range | SHA-256 |
| --- | --- |
| `1400abcc0..1400abcf5` | `5912b7bffd5aa69439a8336d5ae40085a25ceb42d81dac9e5a59e7eedde21ca8` |
| `14146da31..14146da5a` | `1c4bdc5062fbc422738f4b0d4c5381d3794667dc063b553a6eefc5744f58b92b` |
| `141470568..1414705ed` | `455022bff2e4016f1b9f4709d08488112c95e19ea191aecf60020fe41b82836c` |
| `141470440..141470565` | `056a315910535d2876efecf1b5ff2533dcf7ffb15a514edaa6785bca9eb18041` |

Enabled writer0abcc0 stores fopen's unchecked result and calls first fprintf at0abcf0. Null FILE takes fprintf's error branch: errno becomes **22 decimal / 0x16 / EINVAL**, then dispatcher141470568 decodes module global1446ffb40. This is distinct from numeric ERANGE34/0x22 recovered in CP98–102. Startup directly installs encoded null before main; under the fresh-default/no-subsequent-handler profile, dispatcher enters141470440. That handler builds exception statusC0000417 and reaches GetCurrentProcess/TerminateProcess at70548..70556. Native process status is unsigned3221226519, equivalently signed-1073740777.

No second through31st fprintf, remesh fclose, Save return/status selection, or later normal Close occurs after successful termination. The first fprintf is attempted and reaches its invalid-parameter gate; it is incorrect to say no format attempt happened. Earlier project/property/MST attempts and completed effects remain. Arbitrary fopen failure does not prove unchanged remesh bytes: some resource failures could follow creation/truncation. Unchanged sentinel bytes are justified only by a fixture whose sharing denial occurs before write-open/truncation.

This is static qualified evidence, not an observed original crash. Setter-reference scans are bounded and do not prove absence of computed writes or foreign handlers. A custom returning handler is a different branch: later calls may remain reachable. Do not infer it from the host CRT's handler configuration or treat it as default termination.

## What the current converter actually does

`saveRemeshAtProjectSave` returns UnsupportedSnapshot plus invalidParameterReached on failed enabled open. `convert.cpp` then throws Error(M2M_E_UNSUPPORTED) before mapping prior project/MST false values. Its catches return Result7; `ConversionSession` and other local scopes unwind. The C ABI copies a message and returns7; CLI prints it and returns7. The C ABI also has catch(...), so an unhandled new C++ terminal-marker exception would become ordinary M2M_E_INPUT after unwinding.

The Session destructor explicitly performs adapter cleanup, including project storage/receiver/runtime release; it is not original accepted Close replay. Current run does not execute the complete normal Close lifecycle. Therefore neither existing cleanup nor a future skip flag may be described as proof of original Close. A terminal design must suppress both any future Close invocation and the ordinary Save/result tail, while avoiding invented destructor equivalence.

## Minimal internal contract

Represent these mutually exclusive dispositions explicitly:

| Disposition | Meaning | Permitted next work |
| --- | --- | --- |
| Returned | A qualified Save/call returned | Existing adapter status and later qualified Close logic |
| Guarded terminal prediction | Known native terminal intent, execution policy not admitted | Existing explicit refusal; adapter cleanup remains qualified |
| Native process terminal | Qualified handler dispatch reached a nonreturning process endpoint | No Save mapping, Result/message return, Close, unwind or normal completion |

A private terminal record needs source occurrence (`0abcf0:first-null-FILE-fprintf`), native handler profile/proof identifier, process status uint32C0000417, CRT errno EINVAL22, first format attempt ordinal1, and completed earlier SaveCallState observations. It must not claim remeshReturned, SaveReturned, CloseEntered or returnedCode. A returned guard may have returnedCode7 plus a clearly labeled *prediction*; those fields must never describe actual observed process termination.

Do not add terminal as ApplyResult::Completed or ordinary writer false. A tagged private remesh result/request can classify the failed-open branch, but merely returning that request through stack cleanup is not faithful terminal execution. The admitted nonreturning endpoint must be dispatched at the first invalid-parameter boundary, before unwinding writer/session scopes or reaching run's catches. Set the modeled EINVAL at that point. The public writer's successful stack FileDevice/CoggingFile path and31-call body remain unchanged.

Use a private per-call execution service, not a global environment test switch, host invalid-parameter handler mutation, nullptr fprintf, exit(), abort(), std::terminate(), longjmp or a throw caught at the ABI. A process endpoint must not return to Save. Unexpected backend return/failure is outside the admitted Win32 termination profile and must be detected as a failed contract; it cannot resume the31 writes. Any test observer is borrowed/value-only and must finish a bounded durable receipt before dispatch, without entering Session-owned state or changing output files.

Direct TerminateProcess can test fixed process-status/no-unwind behavior but does not reproduce the entire native default handler. The retained handler also captures context, consults debugger/exception machinery, clears its recorded flag, calls SetUnhandledExceptionFilter(NULL) and UnhandledExceptionFilter before termination. Those prefix effects must be ported or explicitly left qualified; matching only the exit code is not full handler-architecture parity.

## Public C ABI and CLI policy dependency

The old ABI1 functions return int0..7 and promise message behavior; they have no terminal-result carrier. No integer can simultaneously mean “this caller process died and did not return.” Preserve those signatures/status values and their present failed-open guard until root selects an outward execution contract.

The least disruptive next route is an explicitly isolated conversion endpoint/supervisor: it owns a fresh worker process, runs the terminal-capable core there, and reports a **process outcome** to its surviving caller. The parent record distinguishes ordinary M2M return, native process termination, adapter launch/failure, timeout and interrupted harness. A new public isolated API/CLI route and version/documentation policy are root-owned design choices; this worker must not silently add a flag, enable killing DLL embedders, or pretend the worker's terminal status is an M2M return. Existing wide/UTF8 calls remain guarded during that stage.

If root later chooses process-faithful behavior for the existing ABI itself, the documented C ABI call may not return on this WRITE_SAVE branch. That is a caller-survival behavior change even with identical signatures. Both wide/UTF8 implementations and CLI must bypass all result/message printing and catch mapping via the same nonreturning core endpoint. It must be explicitly selected and tested in owned children; this evidence-only task does not authorize that activation. A safe legacy guard and an isolated process-terminal receipt are useful distinct contracts, not native parity for the old in-process function.

## Close and effect invariants

Terminal dominates earlier **returned** project/MST failure statuses because the original reaches remesh before reporting those locals. Do not map MST/project M2M_E_WRITE first. Do not roll back, delete or regenerate prior output; do not flush all CRT streams or run atexit/DLL-detach/destructors as a substitute for native termination. Operating-system handle release on process death is separate from fclose and accepted project Close.

Any future lifecycle driver must branch on the disposition before its Close entry, Save-answer callbacks, notifications, diagram rebind, model destruction or global-reset effects. A terminal disposition supplies no normal Close success/cancellation result. A child test may prove that its post-call Close-entry marker is absent, but complete Close implementation remains separately guarded and cannot be claimed from this absence alone.

## Safe bounded regression plan after the policy decision

1. First add a pure private classifier/dispatch-contract test: fresh-default plus enabled failed open selects terminal request; unknown/custom-handler profiles remain explicit guards. Keep returning-custom-handler execution unsupported until its repeated null-FILE/fclose route is independently recovered. This test must not install a host handler or kill its Testkit process.
2. Preserve current in-process C ABI guard tests for both wide/UTF8 while that remains the selected policy. Add only explicitly labeled terminal-prediction observations. Continue existing31-attempt post-open write/close-failure coverage; it is a different branch.
3. Run each active endpoint only in an owned fresh child, explicit executable/DLL paths and hashes, with all disposable inputs, receipts and output beneath a unique build scratch directory. Bind source/build hashes before/after. Parent owns a bounded20-second wait and child-only job/process handle cleanup; timeout/other status is a failure, never expected terminal success. Normalize GetExitCodeProcess to32 bits and require exactlyC0000417.
4. A real public-call fixture can avoid a new timing barrier: prepare a valid KEYWORD_DRIVEN primary with enabled input and hold a parent read handle permitting read sharing but denying writes. Qualified project Open can read it; later remesh write-open fails before truncation. Confirm the chosen copied fixture has no earlier writer of that path. Record sentinel hash, project/property/MST manifests and child entry. A normal unblocked control must complete. Wide/UTF8 child calls need post-call/normal-Close markers that must remain absent on the terminal route.
5. For status dominance, add a bounded child case with earlier physical project/MST write-open denials that still permit their required reads, plus remesh denial. Qualify chosen paths first. Alternatively reuse existing project-output service in a private core test, explicitly distinguishing that from a public ABI test. Require reached prior-attempt metadata and terminal status; do not assign a returned Save code. The existing CP74 callback fixture already supplies a simpler core-only predecessor.
6. A private preterminal observer may emit a versioned fixed-size pipe event containing reached site, handler profile and completed SaveCallState. The parent must require both that event and process status; a pipe event alone proves reach only. Do not use stdio buffering, postmortem in-process details, absence of a file alone, or an arbitrary same-exit-code helper as proof. Static placement before unwind plus real endpoint execution is required; independently measured destructor absence would need a separately reviewed private observation seam.

No original process execution is required by this candidate regression plan. It remains source-qualified until a separately authorized original acceptance strategy exists. No new baseline byte hashes should be invented: compare selected earlier closed outputs against retained/control manifests, preserving raw differences and fixture-specific qualifications.

## Exact dependencies and ownership

Before enabling the path, root must resolve: (1) fresh-default native handler admission and custom/unknown rejection; (2) legacy versus isolated outward policy; (3) typed disposition/returnedCode invariants; (4) in-boundary nonreturning dispatch and default-handler prelude qualifications; (5) lifecycle Close suppression; (6) owned-child supervisor/status protocol, deterministic fixture, focused/full frozen validation. A local type or symbol without those integrations is not a recovered public feature.

Root must reserve shared `convert.hpp`, `convert_details.hpp`, `convert.cpp`, `conversion_call_services.hpp`, C ABI header/implementation, CLI and any lifecycle dispatcher together. A separate worker can own a new source-private terminal contract/backend plus dedicated classifier/child test only after those signatures are agreed. Editor300 retains remesh source ownership until explicit release. The parent harness/runner and CMake edits stay root-owned. Other independent lanes can continue while this edge stays refused.

Reviewed source hashes: convert.cpp `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30`; abi.cpp `6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848`; CLI `b382032ec09bd7ed910a87e48f823924efc1ced57fda847db4d9cf2fc5476c5e`; Session `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`; remesh `39249f84faa6bef97e53504b7fb22f632a539d42cc7b5270a7bd3c553321814f`. These bind this source review, not a new executable acceptance.

Only this report was written. No source/test edit, build/test execution, GUI, original execution or REA mutation occurred. The blocker is an explicit terminal execution/API contract with the listed dependencies; no additional numeric patch closes it.
