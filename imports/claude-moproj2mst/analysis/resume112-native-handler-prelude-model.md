# CP112: source-qualified default invalid-parameter prelude

This increment recovers the native prelude's conditional state relations as a pure model. It preserves the distinction between original static instructions, caller-supplied hypothetical context/API results, and actual candidate process outcomes. No consumer or endpoint is activated. Actual capture, virtual unwind, process-filter mutation and native-handler execution remain unimplemented.

## Rechecked original identity and instructions

The read-only imported original `evidence/original-binaries/DEF_GUI_MO_64.exe` independently hashes to `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. A standard-library PE section mapper recomputed these half-open ranges, matching CP109/retained disassembly:

| Range | SHA-256 |
| --- | --- |
| `14146da31..14146da5a` | `1c4bdc5062fbc422738f4b0d4c5381d3794667dc063b553a6eefc5744f58b92b` |
| `141470568..1414705ed` | `455022bff2e4016f1b9f4709d08488112c95e19ea191aecf60020fe41b82836c` |
| `1414834f0..1414834f8` | `9b3bff723d01eb2308873bb025da7be6af26ed8173d82f9b28affe6e7d75c7cb` |
| `141470440..141470565` | `056a315910535d2876efecf1b5ff2533dcf7ffb15a514edaa6785bca9eb18041` |

Controlling retained records are `analysis/wave3-remesh-default-invalid-141470440.asm`, its static capture receipt, `analysis/wave3-remesh-save-invalid-141470568.asm`, startup contract `analysis/wave3-remesh-invalid-parameter-startup-contract.json`, CP109 `analysis/resume109-remesh-handler-prelude.md`, and CP110 model/design/review receipts. No mutable REA database was opened. The reverse-engineering skill was read and applied to offline evidence qualification.

The independent PE import walk confirms `141e03318=RtlCaptureContext`, `141e03320=RtlLookupFunctionEntry`, `141e03348=RtlVirtualUnwind`, `141e03340=IsDebuggerPresent`, `141e03310=SetUnhandledExceptionFilter`, `141e03308=UnhandledExceptionFilter`, `141e03188=GetCurrentProcess`, and `141e03580=TerminateProcess`, all imported from KERNEL32.dll. These are static import identities, not observed calls.

## Exact native sequence and modeled relation

| Source instruction boundary | Established static behavior / corresponding model limit |
| --- | --- |
| `14146da3b..55` | First null-FILE fprintf writes errno22/EINVAL before invalid-parameter dispatch. Numeric ERANGE34 is unrelated. The existing CP110 prefix supplies this modeled decision. |
| `14147057f..5d3` | Decode the EXE handler slot `1446ffb40`; custom handler calls are a separate returning-possible branch. Decoded null calls debug leaf `1414834f0`, then default `141470440`. Startup null remains conditional on no later replacement. |
| `141470449..45b` | Zero first record dword and call byte-fill helper for the next 0x94 bytes: total exception-record footprint 0x98. Supplemental bounded read-only dumpbin views at `141471850..18bb` and `1414718c7..18f9` show the repeated-byte qword/tail fill; zero DL and length0x94 take the small non-streaming fill path. The model records zero initialization, not a host memory write. |
| `14147047f..498` | Capture local CONTEXT, take its RIP from `[rsp+208]` (context base `[rsp+110]` plus0xf8), then lookup runtime entry with captured RIP, image-base output and null history table. Model requires explicit captured-RIP and lookup inputs. |
| `1414704a2..4d6` | Entry present: `RtlVirtualUnwind(0,imageBase,capturedRip,functionEntry,&context,&handlerData,&establisherFrame,nullptr)`. It updates a captured CONTEXT; it does not unwind live Save frames or run C++ destructors. Model accepts an explicitly hypothetical two-register result projection; it implements no unwinder. |
| `1414704dd..4f5` | No entry: context RIP becomes qword `[rsp+5e8]`, context RSP becomes address `[rsp+5e8]`. This is the same original function-entry return slot, **without adding8**. The pure fallback preserves exactly this relation. |
| `1414704fd..515` | ExceptionCode=C0000417, ExceptionFlags=1, ExceptionAddress=qword original return slot. On the runtime-entry path ExceptionAddress still uses this original caller value, not the supplied post-unwind RIP. Other record fields retain zero initialization. |
| `14147051d..532` | Record debugger Boolean, clear top-level filter by `SetUnhandledExceptionFilter(nullptr)` (old filter result ignored), call UnhandledExceptionFilter on the local EXCEPTION_POINTERS pair. No restoration is present in this body. Model traces the order only; no host filter is cleared. |
| `141470538..543` | Repeat debug leaf only when filter return is exactly0 and saved debugger result is0. Every nonzero filter value, including negative, bypasses the repeat. This does not undo the earlier unconditional default-prefix clear. |
| `141470548..556` | Both ordinarily returning filter branches request current process and TerminateProcess(C0000417). Pure result contains only a modeled termination argument; no observed process status or successful termination field exists. |
| `14147055c..564` | A native epilogue exists after the termination call. A hypothetical returning API enters that epilogue. Candidate backend-return parking is separate harness policy; it is not this native epilogue. The model labels further caller continuation unrecovered. |

The fresh-default predicate remains startup encoded null plus no later same-module handler replacement. A candidate FreshDefault enum, modeled CONTEXT projection or native-predicted status cannot establish an original handler witness. The existing candidate endpoint and CLI relay intentionally continue skipping this prelude and keep their prior qualification.

## New isolated implementation

Only NEW `src/engine/remesh_default_prelude_model.hpp/.cpp` and NEW `tests/mdt/test_remesh_default_prelude_model.cpp` were added. The model composes the existing CP110 pure prefix without changing it. Unknown/custom/invalid evidence stop at PrefixGuard. Every external input is optional; missing context, lookup, unwind projection, caller data, debugger result or filter result stops at its exact next unimplemented operation/data boundary. Optional false/zero inputs remain distinct from absent inputs. Invalid lookup enums retain the lookup guard.

The finite effect array is bounded to12: one of two mutually exclusive context branches, plus the conditional extra debug clear and adverse native epilogue. Model fields describe supplied or modeled values only. A two-register projection is explicitly not a full CONTEXT. `terminationCallReturned=false` means no return assumption was supplied, never that the process died. `true` is an adverse hypothesis and models the native epilogue without promoting it to a candidate success or executing a continuation.

Six Release-active cases check prefix refusals; ordered input guards and zero/presence distinction; exact no-entry RIP/RSP/ExceptionAddress relation and missing-slot guard; runtime-entry projection versus original exception address; the debugger/filter matrix over0/1/-1/INT_MIN; and adverse-return trace bound plus repeated evaluation without host errno mutation. No endpoint or native API is called by this test.

## Static preflight and frozen files

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_remesh_default_prelude_model.cpp` passed: direct quoted includes resolved, Testkit main/run_all exists, macros are supported and no C assert vanishes in Release. All three files passed masked delimiter balance and trailing-whitespace checks. No compiler, build, test executable, GUI, original launch or mutable REA operation occurred. The two supplemental dumpbin invocations read original PE bytes; they did not execute that target.

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/remesh_default_prelude_model.hpp` | `2783a621840c1a0f92392df1e97b0d04b649be9d3230e5a611194ea3991c226c` |
| `src/engine/remesh_default_prelude_model.cpp` | `4ccd70b1ad233ebc9cdbf8fdf0761b789d072a1ee63d38742f6ae1d61218d01a` |
| `tests/mdt/test_remesh_default_prelude_model.cpp` | `a3c8149475bf26411b5a6e97c3e0d6aa50aa406c0b3c52789939e7ab640fc34f` |

Root integration: existing configure-dependent globs discover these files; no shared CMake edit is needed. Target `m2m_mdt_remesh_default_prelude_model`, CTest `m2m.mdt.remesh_default_prelude_model`. Group with existing invalid-parameter model, handler contract, protocol/safety and real worker regressions, then retain full source-stable acceptance/freeze. There is no model consumer in this increment and no public ABI/default CLI change. Files are frozen pending root build/test/archive. Token usage was unavailable.

## Smallest next executable slice and remaining dependencies

A future separately owned child-only prelude probe could allocate native local EXCEPTION_RECORD/CONTEXT/EXCEPTION_POINTERS, call capture/lookup/virtual-unwind on **its own** x64 frame, record original return-slot and projected context, and exercise the debugger/filter branch through a private per-call service. Test alternative lookup/filter outcomes with explicit scripted inputs before any actual host filter mutation. The child supervisor must retain its current hash, nonce, own-image/job, timeout, sticky failure and no-unwind/lease checks. Never install this in-process into Testkit, the C ABI, default CLI or a user process.

An actual native prelude endpoint needs an owned debug-flag counterpart and exact lifetime; real full CONTEXT and unwind metadata/stack provenance; original caller/return-slot identity versus compiler-generated probe frame differences; per-child debugger/filter/WER side-effect policy; API failure/nonreturn handling; and a distinct durable receipt proving candidate operations occurred before terminal publication. Calling the host APIs would validate that candidate child, not reproduce the original same-module CRT frame or original B133 occurrence. Do not replace the current candidate fixed endpoint until those dependencies and reviewed failure ordering are explicit. An enabled-remesh original one-project witness, same-run slot decode and native termination observation remain separate original acceptance gaps.
