# Checkpoint 65: Cogging partial-write and exception boundary

## Result

When a two-Reheat Save reaches the first one-past load (`0x14076cb80`, established in [resume64-cogging-memory-bound.md](resume64-cogging-memory-bound.md)), the only file effect that can be stated as a definite pre-read side effect is this: if the Cogging `fopen("wt")` succeeds, the native open path uses `CREATE_ALWAYS`, so the destination is created or truncated before serialization starts. The writer has also issued its preceding formatted-output calls to the CRT stream, including `Reheat 2` immediately before the load. Their exact persistence to the file is not statically determined because the stream is buffered and the normal close path is later.

If the load faults and propagates, the normal Cogging writer path does not reach its later `fclose`, and outer Save does not reach its later MST writer. Static evidence does not determine whether another exception handler intercepts the fault, whether the CRT stream is flushed/closed during process teardown, or the bytes/length left on disk. No crash dialog, exit code, or guaranteed partial-file contents can be claimed.

## Ordered native effects

The outer Save decompilation (`evidence/gui_mo/s1/mdt/save_d0.c`, `FUN_1400980d0`) calls the MOPROJ writer (`FUN_1400983c0`) before walking operation property writers (`FUN_1400982e0`), and invokes MST output (`FUN_1400a20d0`) after that walk. Therefore, if a Cogging property callback is reached, the earlier MOPROJ writer call has returned. This is a call-order result; this audit makes no claim about storage-device durability.

The Cogging property writer (`0x1407617e0`) converts the path, calls the native `fopen` wrapper with mode `wt`, then calls master writer `0x14076a8c0`. The retained native I/O receipt `analysis/wave3-cogging-native-io-contract.md` ties the successful normal regular-file path to `CreateFileA(..., CREATE_ALWAYS, ...)` and a CRT text stream using a 4 KiB buffer. The writer's explicit CRT close wrapper (`0x14146fb28`) occurs only after master serialization and Process Data output, at `0x140761c46`; its normal return follows at `0x140761c84`.

For two reheats, the master writer first runs the SIMLST loop, writes the `NoReheats` record, then calls the record writer for valid element 1 as `Reheat 1`. On the next iteration it calls `0x14076cb50` with the one-past pointer. That callee calls the banner formatter (`0x14076b450`) before loading the NoPass dword at `0x14076cb80`. Thus all prior sections and the first reheat have reached their formatter calls, and the second-reheat banner formatter has been entered, before the first invalid model-data read. If that load faults, the NoPass formatter and subsequent Heat/Pass output are not reached on that call.

These are formatter-call and API-order facts. The CRT stream is buffered. The native CRT flush path emits pending bytes when invoked, marks an I/O error on a short/error result, then resets the pending count/pointer. The writer ignores formatter and close return values on its normal path. The earlier calls may have caused automatic flushes as the buffer filled, but the exact flushed prefix depends on actual field values and buffer occupancy. No explicit flush precedes the invalid load. Because the `fclose` site is later, static source does not prove that it executes after a fault.

## Exception and unwind boundary

The original PE unwind table marks `0x1407617e0` with `UHANDLER` and handler RVA `0x0146E20C`; the master writer at `0x14076a8c0` has `EHANDLER UHANDLER` and handler RVA `0x0146FDE4`; the record writer at `0x14076cb50` has no handler flags. The bounded capture includes these rows, the master handler thunk, and the writer's unwind target body. The thunk has no visible branch to the post-read continuation; it delegates to a helper and returns 1. This metadata and helper code do not establish an application recovery path that resumes after the load, closes the `FILE*`, or flushes pending bytes. Nor do they prove that a first-chance access violation necessarily escapes the process: higher-level SEH/runtime handling and the actual heap mapping are not observed here.

So the smallest defensible fault-order statement is conditional: **if** the one-past load raises an access violation that propagates out of this call, the normal writer close and outer MST continuation are bypassed. Whether it raises, is handled, or leaves particular bytes requires an original runtime capture. Even then, partial bytes are run-specific because the one-past address may be readable in one allocation layout and not another.

## Evidence and limits

The unique bounded capture is [resume65-cogging-partial-order-capture.txt](resume65-cogging-partial-order-capture.txt), SHA-256 `b0d16c1a4888819c3368adccadeb6ae0a4ec913dd33688db2d6c879d7bf7bdc4`. It uses immutable `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and contains bounded writer/caller/callee/CRT/unwind slices. Existing I/O evidence is cited rather than duplicated. The allocation-vs-readable-memory distinction is in CP64; this receipt adds only surrounding file and unwind order.

No source, build, test, GUI, or mutable REA state was accessed or changed.
