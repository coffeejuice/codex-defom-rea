# CP117: filter application, state ownership and terminal ordering

After virtual unwind, the original CRT uses the copied context as exception-report input. It does not install that context into the live thread, restore the previous top-level filter, or convert a filter return into an ordinary successful Save. All ordinarily returning filter branches join the explicit current-process termination call. The retained source does not establish that this entire path executed in an original enabled-remesh run.

Review result: **no concrete discrepancy was found in the existing pure prelude model**. Its source SHA-256 is `4ccd70b1ad233ebc9cdbf8fdf0761b789d072a1ee63d38742f6ae1d61218d01a`. It already models the two-zero extra clear, all supplied returning filter values joining CurrentProcess/Terminate, original caller-slot ExceptionAddress, no native context application/restoration, and adverse termination return. This bounded static continuation of CP116 adds evidence/ownership clarification only; no repair or duplicate helper is proposed.

## Evidence binding

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` independently hashes to `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Read-only PE section mapping recomputed these half-open original byte ranges:

| Range | SHA-256 |
| --- | --- |
| `1414704fd..141470565` | `e2cfeaab19497f83e68a0742deebd9dc4e1abbed13cc07186f8d65170f2e310e` |
| `14147051d..14147055c` | `f366456ee1f6ac4d5df3d094fa6aa445e62ae2149ee6deb73ad81a7b590b5162` |
| `1414834f0..1414834f8` | `9b3bff723d01eb2308873bb025da7be6af26ed8173d82f9b28affe6e7d75c7cb` |

Retained disassembly `analysis/wave3-remesh-default-invalid-141470440.asm` hashes to `04970920ced02bcba5dbce7e464ce91cf631cda9c3636cb59c19a03e0e3946dc`. `analysis/wave3-remesh-invalid-debug-leaf.asm` hashes to `6beb078fc7bcc2c8a21edbb5b2085be2afb51f1dcaaa83bc5672db2b8c4aab22`; only leaf bytes through RET at `1414834f7` are interpreted. The trailing bytes belong to another function.

CP116's pure projection receipt is `analysis/resume116-remesh-default-unwind-projection-implementation.md`, SHA `be9f19023a584b1636ff129319376e2ff4ef9d8b01e6ed180c6bf7127e994fec`. Its numeric caller projection remains a hypothesis from supplied inputs. This lane does not repeat its implementation or claim an observed captured context.

## Copied context is report input, not live continuation

The original default handler builds its local pointer pair at `[rsp+48]`: ExceptionRecord points to `[rsp+70]`, ContextRecord to `[rsp+110]`. RtlCaptureContext supplies the local CONTEXT, then the entry-present path calls RtlVirtualUnwind with that local context. The no-entry path edits its copied RIP/RSP directly. Both join at `1414704fd`.

At the join it reloads the original caller return slot, sets exception code `0xC0000417`, flags1, and ExceptionAddress to that original return value. The record's address is independent of the projected context RIP. It then passes the local pointer pair to UnhandledExceptionFilter at `141470532`.

In the audited body `141470440..565`, no call applies this copied CONTEXT to the running thread: no RtlRestoreContext, SetThreadContext, continuation transfer or exception raise is present. This is a bounded body claim, not an assertion that those imports or mechanisms are absent everywhere in the EXE/OS. The source is a direct filter call with a constructed record; a symbolic filter result must not be interpreted as proof that live exception dispatch resumed execution.

There are also ordinary machine-register restores if TerminateProcess returns: default-handler `add rsp,5e0; pop rbx; ret`, followed by dispatcher's saved-register epilogue. Those restore their own call frames. They are distinct from applying the copied exception CONTEXT, unwinding Save scopes or restoring process-global CRT/filter state.

## Exact filter application order and decision

| Source point | Actual instruction-level decision |
| --- | --- |
| `14147051d` | Call IsDebuggerPresent; save EAX in EBX at `141470525`. |
| `141470523..527` | Set RCX=0; call SetUnhandledExceptionFilter(NULL). The returned previous-filter pointer is not saved. |
| `14147052d..532` | Call UnhandledExceptionFilter with the local EXCEPTION_POINTERS pair. |
| `141470538..53a` | Any nonzero EAX jumps straight to `141470548`. |
| `14147053c..53e` | For EAX=0 only, a nonzero saved debugger result also jumps to `141470548`. |
| `141470540..543` | Only two zero results call `1414834f0(2)`, which writes dword `1450540e8` to0. The leaf ignores the argument in its bounded body. |
| `141470548..556` | All returning cases call GetCurrentProcess and TerminateProcess with status C0000417. |

Thus:

| Filter returns | Saved debugger result | Repeat module-dword clear | Next source call |
| --- | --- | --- | --- |
| 0 | 0 | Yes | GetCurrentProcess, then TerminateProcess |
| 0 | nonzero | No | Same termination path |
| any nonzero, including -1 | any | No | Same termination path |
| does not return | any | Not established downstream | No downstream reach claim |

The repeated clear is in addition to the unconditional clear already performed by the decoded-null dispatcher branch. The current pure prelude model already covers this matrix; another pure Boolean helper would add little evidence.

Windows documents NULL filter installation as selecting default handling and the function's return as the previous filter pointer. The setting affects existing and future process threads. The original discards that return and never restores it in the audited body. See [SetUnhandledExceptionFilter](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-setunhandledexceptionfilter). Default filter behavior also depends on debugger and process error-mode state and may display error UI; this prevents treating a direct call as a bounded, side-effect-free operation. See [UnhandledExceptionFilter](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-unhandledexceptionfilter). These API descriptions explain implications of the static calls; they are not observations of the original OS execution.

## Distinct state owners; no global restoration proved

| State | Owner and established access | Missing runtime fact |
| --- | --- | --- |
| Encoded invalid-parameter handler at `1446ffb40` | This EXE's writable module .data; startup stores encoded null; dispatcher decodes/reads it. This is not the host UCRT's handler or the OS top-level filter. | Current decoded value at this original call and absence of later replacement. |
| Dword `1450540e8` | This EXE's writable module .data; bounded leaf clears it before default entry and conditionally again after filter return. | Full semantic owner/lifetime and any earlier/current value; the inherited label "debug flag" does not recover all users. |
| Top-level exception filter | OS process state changed by SetUnhandledExceptionFilter(NULL). | Previous filter, foreign replacement, OS implementation/filter/WER behavior and debugger/error-mode environment. |
| errno storage | Module CRT getter supplies per-call storage; first null-FILE branch explicitly writes22 before dispatch. | Same-run storage identity and later values; no errno restoration is explicit in the handler body. |
| Exception record/context/pointer pair | Locals owned by this default-handler stack frame. | Actual capture values, stack bytes, complete unwind result, and filter-side changes to those locals. |

PE mapping places both EXE globals in writable `.data` (section characteristics `0xc0000040`) beyond that section's file-backed raw extent. Their mapped initial region is loader zero-fill, not an on-disk initialized current pointer/value. Startup later encodes and stores null into the handler slot. Neither zero-fill nor the pure model establishes current runtime global state.

There is no saved old filter, old dword value or old module CRT handler restored by this path. It would be a behavioral change to introduce a restore-on-return wrapper while claiming a faithful native baseline. Conversely, introducing a host-global replacement merely to resemble the original address would invent ownership and could mutate an unrelated embedding application.

## Termination and implementation boundary

The handler calls TerminateProcess on its own current-process handle. Ordinary successful self-termination is nonreturning; the supplied status is the process exit value, rather than the filter return or an m2m adapter code. This API does not provide normal CRT/Save cleanup. See [TerminateProcess](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminateprocess). The original still contains an epilogue after that call, so an adverse returning call cannot be labeled successful termination. No original process status or filter occurrence was observed here.

There is no source correction justified by this review. The current pure model already captures the source-level application order and its unknown-input guards. The pure metadata projection remains separate and needs no duplication.

The remaining boundary is evidence and actual effect ownership: original captured CONTEXT/lookup outcome, module-owned dword semantics/lifetime, current encoded-handler slot, OS filter/debugger/error-mode state and original process termination are not observed. A future separately reviewed native child application would need those concrete owners and bounded diagnostics; introducing a symbolic flag, another pure helper or a scripted service would not close the original occurrence gap. Existing candidate fixed endpoint/CLI relay qualifications should remain explicit. No such successor implementation is proposed as a repair here.

Only this report was written. No source/test/build/CMake edit, executable launch, GUI control or mutable REA action occurred. Token usage was unavailable.
