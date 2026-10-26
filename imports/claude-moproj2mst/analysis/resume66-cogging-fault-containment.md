# Checkpoint 66: Cogging two-Reheat fault containment

## Qualified static outcome

CP64 proves that, with two allocated Reheat records, the faulty second writer iteration passes the one-past pointer to `0x14076cb50`, whose first model-data access is a four-byte load at `0x14076cb80`. This checkpoint closes the immediate native exception-dispatch chain around that instruction.

The record writer (`0x14076cb50`) has no unwind handler flags. Its caller, master writer `0x14076a8c0`, has both `EHANDLER` and `UHANDLER`; the exception-handler RVA points to `0x14146fde4`. That thunk calls `0x14146fd80`, then sets `EAX=1` and returns. The Windows `EXCEPTION_DISPOSITION` enum assigns 1 to `ExceptionContinueSearch` (local MSVC `excpt.h`). So, if the one-past read raises an access violation and reaches this frame's handler, the master writer's handler continues exception search; it does not locally catch and resume the post-load writer path.

The next frames in the known Save chain have no exception-dispatch handler: `0x1407617e0` (Cogging property writer) is marked `UHANDLER` only; `0x1407616d0` is also `UHANDLER` only; operation iteration `0x1400982e0` and property dispatch `0x140760400` have no handler flags. The outer Save entry `0x1400980d0` is `UHANDLER` only. `UHANDLER` participates in unwind; it does not designate an exception-dispatch handler. Thus no frame in this traced chain catches the fault and returns normally. The master frame's handler selects continued search, then the outer frames have no `EHANDLER` to stop it.

This is stronger than saying only “no recovery was seen”: **conditional on an access violation being raised at the load and normal Windows frame-based dispatch, it propagates past the known Save/Cogging writer chain.** It does not establish whether the load faults at runtime. A vectored or process-level handler above this chain, unhandled-exception behavior, and any resulting dialog/exit status are outside the retained evidence.

## Consequence for observable writes

If the successful Cogging `fopen("wt")` reaches `CREATE_ALWAYS`, the destination is created or truncated before serialization. Earlier formatter calls are issued in order, and the `Reheat 2` banner formatter runs before the one-past load. The normal explicit `fclose` occurs after master serialization and Process Data output; normal Save reaches MST output only after the operation property loop. If the access violation propagates as above, the code does not return normally to either continuation.

The CRT stream is buffered (4 KiB in the qualified normal allocation profile). Earlier formatter calls may already have caused automatic flushes, but their exact prefix depends on the actual model strings/values and buffer occupancy. The unwind metadata proves no local exception recovery to the normal close site; it does not prove whether an unwind callback or process teardown flushes/closes CRT state. So the only unconditional file-content claim remains the successful-open create/truncate effect. No specific partial byte range, final file length, MST effect, GUI dialog, or process status follows from static code.

## Evidence

The bounded unwind excerpt and the handler thunk are in [resume66-cogging-fault-containment-capture.txt](resume66-cogging-fault-containment-capture.txt), tied to immutable `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. For the exact allocation and read site, see [resume64-cogging-memory-bound.md](resume64-cogging-memory-bound.md). For file-open, buffer, close, and surrounding call order, see [resume65-cogging-partial-order.md](resume65-cogging-partial-order.md). Those earlier bounds are referenced, not re-derived here.

Retain the candidate two-Reheat refusal guard. The native static invariant is the propagated invalid access if it faults; it supplies no deterministic two-Reheat serialization result or original status mapping.

No source, build, test, GUI, or mutable REA state was accessed or changed.
