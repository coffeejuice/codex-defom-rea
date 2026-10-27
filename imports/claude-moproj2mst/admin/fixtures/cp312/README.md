# CP312 inert native WORKER fixture

`worker.c` is source for a fixed x64 console WORKER. It can create only the
CP302 exit-only fixture at `C:\PROBLEM\s302\fixture.exe` (108,032 bytes,
SHA-256 `66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28`).
It opens that file without write or delete sharing, hashes it through BCrypt,
and holds the file handle through `CreateProcessW`. It has no original DEFORM,
GUI, hook, network, arbitrary image, or arbitrary argument path. No execution
of this worker or its child is authorized by this source preparation.

## Proposed exact-handle interface (not yet wired)

The supervisor must first create this WORKER suspended in the creation-bound
WORKER job. It must pass only private stdin/stdout anonymous pipe ends through
an explicit STARTUPINFOEX handle list. It must duplicate only the exact CHILD
job handle **noninheritable** into that specific WORKER process after creation
and before resuming it. The supervisor alone owns the WORKER job lifetime
handle; no WORKER duplicate is delivered to or retained by the worker. This
is necessary so a supervisor crash closes the final WORKER handle and its
kill-on-close limit can terminate the worker. The pipe handles are
intentionally inherited; the CHILD job handle is not. Both jobs require
kill-on-close and must lack silent-breakaway permission. The supervisor checks
the WORKER job flags and exact WORKER membership before resuming the worker;
the worker checks CHILD job flags and its nonmembership in CHILD before any
attempted child creation. These checks do not enumerate an ancestor job chain:
the supervisor must inspect and attest every relevant ancestor for
silent-breakaway permission. The supervisor must retain sole ownership of the
input writer and output reader.

The private pipe carries packed, little-endian, one-use structs from `worker.c`:

1. `CP312_INIT`: magic `0x32313343`, version 2, the worker-local CHILD job
   handle value, and a 32-byte occurrence nonce. Version 2 removes the WORKER
   handle from the protocol and must not be decoded as version 1.
2. `CP312_RESULT`: the same magic/version/nonce, whether `CreateProcessW` was
   called, its raw BOOL and immediate `GetLastError`, creation PID/TID and
   worker-local process handle, plus `IsProcessInJob` BOOL/error/value for the
   child against the exact CHILD job handle. The supervisor queries the child
   against its exact WORKER job handle after duplicating the child handle; a
   worker-side WORKER membership claim is deliberately absent. The worker creates suspended
   with `STARTUPINFOEXW` `PROC_THREAD_ATTRIBUTE_JOB_LIST(CHILD)`, disabled
   handle inheritance, and no breakaway flag. It never resumes the child.
3. `CP312_ACK`: magic/version, exact worker-local child process handle, and
   the same nonce. The worker retains the process/thread/CHILD job handles until
   this matching acknowledgment. Invalid or broken protocol leaves it parked
   for supervisor job termination, without speculative cleanup.

The ACK must be sent only after the supervisor duplicates the exact child
process handle into its own table and validates child identity, creation time,
and membership in both exact jobs using supervisor-local handles. If the result
pipe breaks, a report is partial, or
the worker stops before the supervisor receives the handle, child creation and
the worker-local child handle are **unknown**. The supervisor must retain the
exact WORKER and CHILD jobs and terminate/wait on them under the conservative
cleanup protocol; it must not infer that no child was created. A PID alone is
never an ownership proof. This binary
interface is **not** CP311's framed JSON; a reviewed bridge and pipe/worker
identity binding are still required. The supervisor must resolve failure and
timeout retention and terminate both jobs under the existing conservative
cleanup protocol. Native child execution remains **HARD HOLD** until those
pieces, PE receipt, source review, and test gates are complete.

## Static preparation

`python admin/test_cp312_worker_fixture_source.py` checks the pinned source and
build contract without compiling or starting anything. From a Windows x64
MSVC Developer PowerShell, `python admin/build_cp312_worker_fixture.py` would
compile and inspect `C:\PROBLEM\s312\worker.exe`, refusing existing outputs.
The builder does not execute either binary. Its PE gate requires AMD64,
console, only `KERNEL32.DLL` and `BCRYPT.DLL` imports, and no TLS or delay
imports; it writes a receipt at `analysis/cp312-worker-fixture-build.json`.
