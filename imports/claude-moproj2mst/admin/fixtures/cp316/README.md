# CP316 WORKER-only source preparation

This separate fixture has no child-launch mode and no image-open path. It is
not the CP312 executable. Its packed CP312 v2 INIT/RESULT/ACK sizes remain
48/80/48 bytes. INIT still supplies a positive, noninheritable worker-local
CHILD job handle. The fixture checks kill-on-close, absence of silent
breakaway and its own nonmembership, but never creates a child. Every child
telemetry field in RESULT is zero. It echoes all 32 nonce bytes. ACK must
match magic, version, zero child process handle and all nonce bytes.

The sole command argument is a strict unsigned decimal absolute
GetTickCount64 deadline. The supervisor must compute it BEFORE creating the
worker; the fixture cannot prove when it was computed. A future or remaining
interval over 5000 ms, expired deadline, malformed argument, pipe failure,
invalid INIT or invalid ACK parks the worker for supervisor termination.
One deadline covers startup, INIT, RESULT and ACK; no stage resets it.

Stdin/stdout must be distinct inherited byte-mode named-pipe CLIENT handles
opened FILE_FLAG_OVERLAPPED by the supervisor. The fixture checks client/byte
pipe kind but cannot prove asynchronous handle mode with these documented
Win32 metadata APIs. Therefore native execution remains **HARD HOLD** until a
reviewed supervisor creates and proves the endpoint modes, restricted handle
inheritance, pre-create deadline, retained image identity, creation-time
WORKER job assignment, exact memberships, and exact termination/wait cleanup.
Anonymous/synchronous pipes from the earlier creator are not a usable adapter.
Calling ReadFile/WriteFile on a synchronous handle can block before any wait;
the finite waits here do not make such handles safe.

On proven asynchronous handles, transfers use OVERLAPPED with a manual-reset
event and finite remaining-deadline waits. Partial transfers advance only by
the returned positive byte count. Timeout requests CancelIoEx and parks;
cancellation success is never assumed. OVERLAPPED and frame buffers have static
lifetime, and all exact handles are retained on HOLD. The supervisor alone owns
the WORKER job lifetime and terminates it on failed/uncertain protocol. Parking
is deliberate containment retention, not successful timeout cleanup. A valid
ACK permits closing the unused CHILD handle/event and exiting; it does not
prove external job accounting or authorize another occurrence.

The fixture has been built and statically reviewed, but never executed.
C:\PROBLEM\s316\worker.exe is 104960 bytes with SHA-256
`19886a0821d47611cc82dc44476ae222328e9c169b8e8ba9183cf92c3b655985`.
The checkout-local analysis/cp316-worker-fixture-build.json receipt has SHA-256
`7dbf6936c3261e5faa83b1d29e3fd69e54963ab312670c8b1d9a058d6728e309`.
Its PE inspection records AMD64 console, KERNEL32-only imports and no TLS/delay
imports. The builder refuses existing artifacts. Native execution remains
**HARD HOLD**: this build and static review do not establish runtime deadlines,
pipe behavior, job containment, cleanup, original behavior or converter parity.
