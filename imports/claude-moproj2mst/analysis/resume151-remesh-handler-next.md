# CP151 remesh null-`fopen` handler context and terminal path

## Finding

The retained PE closes the **static** null-`FILE*` invocation context more precisely than the earlier frontier: the first null-`fprintf` at `0x14146DA0C` writes `EINVAL` (`22`) and calls this EXE's dispatcher `0x141470568` with all five invalid-parameter arguments null. The dispatcher reads and decodes the module slot at `0x1446FFB40`; a still-null value selects the EXE's default handler. That handler captures a context inside itself, builds an exception-pointer pair, clears the process top-level filter, invokes `UnhandledExceptionFilter`, and then requests `TerminateProcess(..., 0xC0000417)` on every ordinary returning-filter path. This remains a qualified static path, not a witness that the original B133-derived run reached it or that the slot remained null at that time.

## Exact error and slot call

Original binary: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/wave3-remesh-save-writer-14146da0c.asm` records the CRT invalid-parameter entry. At `0x14146DA31–39`, RCX (the FILE argument) is tested. On null, the code calls errno accessor `0x141474134` and stores `0x16` at the returned address (`0x14146DA3B–46`). It then zeros the fifth stack argument and R9/R8/RDX/RCX (`0x14146DA46–53`) before calling dispatcher `0x141470568` at `0x14146DA55`. Thus the dispatcher invocation is precisely:

`expression=null, function=null, file=null, line=0, reserved=null`.

This is distinct from an observed/known CRT handler pointer: the dispatcher at `analysis/wave3-remesh-save-invalid-141470568.asm:0x14147057F–594` loads the actual EXE global `0x1446FFB40`, decodes it through `0x1414813EC`, and branches on the decoded result. A nonnull pointer is called with the same four zero register arguments and fifth null stack argument; if it returns, the dispatcher returns to the CRT's `-1` result path. For decoded null, it calls `0x1414834F0` and then `0x141470440`. The initializer capture `analysis/wave3-remesh-invalid-handler-initializer-14146e9f4.asm` shows startup installing an encoded null into that global before `main`; the aligned setter leaf is a direct store to the same global. Existing scans do not prove no later or computed/foreign replacement, so “default handler” is conditional on that startup value still being current at dispatch.

## Default-handler context and filter call

In `analysis/wave3-remesh-default-invalid-141470440.asm`, the default handler first zeroes a local exception record, then calls `RtlCaptureContext` with the local CONTEXT address `rsp+0x110` (`0x14147047F`). The captured instruction pointer is the return site after that call, `0x141470485`, on the ordinary call path. It passes that RIP to `RtlLookupFunctionEntry` (`0x141470498`). If a runtime-function entry is returned, it invokes `RtlVirtualUnwind` on the **captured local context** (`0x1414704D6`); otherwise its explicit fallback loads the handler's caller return address from `[rsp+0x5E8]`, writes that RIP into the local CONTEXT, and sets local RSP to the same address (`0x1414704DD–F5`). This is report-context construction, not a live unwind of the Save stack or execution of C++ destructors.

The handler sets local `EXCEPTION_RECORD.ExceptionCode=0xC0000417`, `ExceptionFlags=1`, and `ExceptionAddress` from `[rsp+0x5E8]` (`0x1414704FD–515`). Since the prolog is `push rbx; sub rsp,0x5E0`, that stack word is the return address from the dispatcher call into this handler (the dispatcher's `0x1414705D8` return site in the observed null-default call sequence), not the earlier remesh writer's Save caller return address. It then calls `IsDebuggerPresent`, stores its result, calls `SetUnhandledExceptionFilter(NULL)`, and invokes `UnhandledExceptionFilter` with RCX pointing to the local `EXCEPTION_POINTERS` pair at `rsp+0x48` (`0x14147051D–532`). That pair points to the local exception record (`rsp+0x70`) and the captured/virtually projected local CONTEXT (`rsp+0x110`).

Filter return handling is narrow: if `UnhandledExceptionFilter` returns zero and the saved debugger result is also zero, it calls the same debug-flag clearing leaf once more (`0x141470538–543`). Otherwise it skips that second clear. Both returning cases join at `0x141470548`, call `GetCurrentProcess`, load status `0xC0000417`, and call `TerminateProcess` (`0x141470548–556`). The filter return value does not select a normal Save continuation. If the filter never returns, or the termination API unexpectedly returns/fails, this static ordinary-return path does not settle the process outcome; the handler has an epilogue after the API call.

## What is and is not closed

Closed from retained code: first error code and errno write; all-null invalid-parameter arguments; exact module-global slot and decode/branch; the startup null initialization; default-handler local context/record construction; filter call argument identity; filter-return branch; and requested process termination status. Under the conditional still-default profile and ordinary returning filter/Win32 behavior, no later remesh formatted calls, `fclose`, Save return/status mapping, or normal Close occurs. Prior project/property/MST effects remain. The first `fprintf` has been attempted; it is inaccurate to describe the path as zero remesh attempts.

Not closed: the current runtime value of `0x1446FFB40` during a specific Save, whether the original B133 input ever had remesh enabled, Save-time CWD/path identity, actual null `fopen` at the native occurrence, captured runtime context/unwind branch, debugger/filter outcome, or observed child status. Candidate isolated-worker status remains evidence of its own fixed endpoint, not this native handler prelude. Arbitrary failed-open output residue is also not determined; unchanged bytes require a write-denial fixture proven to fail before truncation.

## Smallest decisive witness

The remaining gap is no longer the static caller argument list. It is a same-run original-process witness binding the actual occurrence and dynamic state. The minimal useful trace is a fresh owned original process over a disposable B133-derived input, with a pre-truncation write denial on the resolved `LOCAL_REMESH.DAT`, recording: executable/module identity and hash; successful Open and remesh-enabled state at Save; Save-time CWD/path; actual null fopen and first fprintf; decoded `0x1446FFB40` value at dispatcher entry; handler entry and `EXCEPTION_POINTERS` fields; captured RIP/RSP and selected lookup/unwind branch; debugger/filter return; reach/return behavior of `TerminateProcess`; and external child status. Instrumentation must disclose if it changes debugger/filter observations. Do not replace the module handler or top-level filter just to force the path.

Without that witness, the correct claim remains conditional source prediction. No candidate worker receipt or same numeric exit code can establish the original runtime occurrence.

## Qualification

Read-only comparison of retained original PE disassemblies, startup/slot captures, and CP103/109/116/124 reports. No source edits, build/test, GUI, original process execution, or mutable REA database action occurred.
