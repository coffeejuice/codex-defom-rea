# Fresh Frida spawn held-handle cleanup guardian

The new `admin/hold_frida_spawned_process.py` provides bounded Windows cleanup for
one PID just returned paused by the same local controller's direct Frida spawn.
No original application, Frida session, GUI control, installed executable,
external project, or reconstructed executable was launched or modified for this
qualification. Only harmless suspended Python children were created.

## Adoption and authority

The controller must launch the helper as its direct child immediately after its
own spawn. Required CLI inputs are `--pid`, `--expected-image` (absolute path),
`--expected-sha256`, `--spawn-token` (fresh unpredictable 32..256 character value),
`--controller-pid`, `--created-after-filetime`, `--created-before-filetime`.
`--adopt-within-ms` defaults to 3000 and cannot exceed 3000; `--max-seconds`
defaults to 20 and must be finite, positive, and at most 20.

Capture the creation window immediately before/after the Frida spawn. Convert
Unix milliseconds to Windows FILETIME with
`(BigInt(milliseconds) + 11644473600000n) * 10000n`; pass decimal strings to avoid
JavaScript Number rounding. The process creation FILETIME must lie in that
window, the window cannot exceed the adoption allowance, and the process age
at handle inspection cannot exceed that allowance. Identity, liveness, and
creation time come from a retained `OpenProcess` handle. The image path is
canonicalized and compared with the handle's queried image; SHA-256 binds the
file at that path. Rejected adoption closes its temporary handle without
terminating the rejected process. A process exiting during adoption is rejected.

This is deliberately a trusted-controller contract. Neither a random token nor
a recent creation timestamp proves Frida origin, parentage, ownership, or pause
state. The helper checks that its declared controller is its own direct parent,
not that the target is a Windows child of that controller (Frida spawning may
involve its own machinery). The controller must assert that the PID came from
its own current paused Frida spawn and must retain responsibility for resume,
detach and the paused lifecycle. A dishonest caller could mislabel another
recent matching process; this helper is not an authorization boundary against
such a caller and does not authorize adopting arbitrary user processes. The
receipt explicitly records `pause_verified: false` and
`ownership_basis: caller_asserted_direct_frida_paused_spawn`.

## Cleanup protocol and verifiable receipt

stdout is JSONL. `ready` includes nested `identity` with PID, creation FILETIME,
canonical image, hash, controller/guardian PIDs, spawn token, receipt ID,
adoption age and diagnostic retained handle value. The handle value is not an
IPC-transferable capability. Preserve these rows in the caller's diagnostic log.
stdin accepts exactly `{"command":"status"}` or `{"command":"terminate"}`.
There is no resume command. `status` reports `alive_pause_unverified` or `exited`.
Terminate, EOF, invalid input and expiry all clean up the adopted handle.

The independent watchdog starts after adoption, before readiness output, and
uses the process-wide monotonic deadline established before adoption. It calls
the same locked exact-handle cleanup even when the stdin reader or stdout is
blocked. A controller that stops reading can lose receipts, but cannot prevent
the watchdog's termination attempt. stdin queue length and line length are
bounded. Termination uses only `TerminateProcess` on the retained handle, waits
up to five seconds, then calls `GetExitCodeProcess`. No process-name search,
enumeration, broad kill, Frida kill, PID reopen, or unrelated-process cleanup
exists. Exit code 259 alone is never treated as liveness: wait status proves exit.

The final `closed` event requires held-handle `wait_result: 0`,
`exit_verified: true`, and successful `CloseHandle` (`handle_closed: true`). It
includes the exact termination attempts, API success/error, requested exit code,
observed exit code, and watchdog errors. An already exited adopted process needs
no termination attempt. A termination racing natural exit retains the failed
API attempt and accepts cleanup only if the handle independently proves exit.
Failure produces a nonzero helper exit and `error`/`cleanup_failed`; a requested
kill without verified exit is never represented as cleanup success. A successful
run requires exit code zero plus a matching final receipt, not merely `ready`.

As with any ordinary subprocess, abrupt helper death bypasses its Python
watchdog and finally block. This is not a kill-on-close job guarantee. The caller
must preserve its separate host deadline and cleanup responsibility. The image
hash is of the resolved file at inspection time, not an attestation of every
mapped code page or filesystem tamper resistance. Windows wall-clock adjustments
can conservatively reject a legitimate adoption. Rejection leaves cleanup with
the direct Frida controller; it must never broaden its authority to compensate.

## Qualification, 2026-10-25

Command run from the converter subtree:

```text
python -m unittest discover -s admin/tests -p test_hold_frida_spawned_process.py -v
```

Result: **13 tests passed, zero skipped, 0.936 seconds**. Tests cover exact-handle
status/terminate/closure; wrong hash and handle image; mismatched creation time;
expired adoption; wide window, short token and self-PID rejection; already exited
child; exit code 259; JSONL status/terminate; EOF; deadline while stdin stays open;
invalid command cleanup; and declared controller mismatch. Every test retains a
separate creator-owned process handle. Successful cleanup is independently
checked through that handle, including expected exit code; rejection cases
verify that the child remains alive. Each fixture's creator performs its own
finally cleanup. Test tokens simulate the caller assertion and do not qualify
Frida suspension, spawn provenance, the original application's lifecycle, blocked
stdout behavior, or abrupt helper-death recovery.

Source SHA-256 at qualification:

- `admin/hold_frida_spawned_process.py`:
  `fb13d8156461e329778f1bdf1328a18ad7380b1ee5ce60681421c538760536e8`
- `admin/tests/test_hold_frida_spawned_process.py`:
  `44996d16b446fbaabff4e77843d3077435803dc3358ab56a3ce07edc64a04b71`

The probe worker received the exact CLI and receipt schema. Original probe
execution remains a separate reviewed integration step. This result qualifies
the guardian's Windows handle path using disposable test children only.
