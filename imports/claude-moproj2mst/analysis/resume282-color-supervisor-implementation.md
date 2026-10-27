# CP282 fixed harmless debugger supervision

This increment establishes a creation-bound supervisor for the fixed Python/native surrogate. Original execution remains **HOLD**. `run_original()` still raises unconditionally; neither CLI accepts an original executable, arbitrary executable, PID, address or attach mode. No original, GUI, Frida or REA session was used. No deployed executable or native fixture file changed.

The supervisor starts a nonresettable three-second monotonic deadline before creating the worker. It retains the worker's creation-owned process handle, queries its PID/image/creation FILETIME, and only then sends a worker-start gate. The same debugger thread creates the fixed child with DEBUG_ONLY_THIS_PROCESS | CREATE_SUSPENDED, checks DebugSetProcessKillOnExit(TRUE), and offers its child process handle and identity. The supervisor DuplicateHandle call uses its owned worker handle as source process, independently queries the duplicate's PID/image/decimal-string FILETIME, compares the offered identity and expected interpreter image, and acknowledges that identity. Only a matching acknowledgement permits the observer's sole ResumeThread, whose previous count must equal one. The creation-to-explicit-kill-policy interval retains the documented default-TRUE qualification; the explicit call cannot protect an interval before it occurs.

At the deadline, the supervisor requests cooperative cleanup, requests termination through the duplicated child handle when available, and allows 350 ms before terminating the retained debugger-worker handle if still running. Separate bounded worker and child waits retain raw status codes. Held entry and EXIT_PROCESS injections deliberately stop servicing cleanup, so the worker termination fallback is exercised. Their uncontinued event snapshots retain the intended disposition and are labeled abandoned by debugger-worker termination; the subsequent event tail and worker-local cleanup stay incomplete. Normal event-held messages are intermediate snapshots; the completed worker event ledger supplies the actual continuation results.

The original CP281 loop receives optional private hooks only; its direct harmless CLI and all twelve tests retain their behavior. New WinAPI calls use explicit ctypes signatures, including GetProcessId and GetExitCodeProcess; inherited ABI functions already declare pointer-width HANDLE arguments. The Popen handle is the worker creation handle, not a PID reopen. Source-before/after hashes bind both implementation files. Logical handle closure is not presented as OS invalid-handle proof.

## Bounded outcomes

- [normal](resume282-color-supervisor-normal.json): 31427 bytes; SHA-256 `2ed71e9684352e5ca94f58d171519ec111f1b4af93fc29a9443411665863028a`. Elapsed 0.25s; worker signaled=True; independently bound child signaled=True; tail `complete`.
- [held-entry](resume282-color-supervisor-held-entry.json): 10842 bytes; SHA-256 `7b0016b3d9d8826b834cbfbe40f2c16ab0002481fb95d0c260aae562bfeedced`. Elapsed 3.39s; worker signaled=True; independently bound child signaled=True; tail `unknown after debugger-worker termination`.
- [held-exit](resume282-color-supervisor-held-exit.json): 11697 bytes; SHA-256 `f1a584a6c2ff6b699e14ad261c5bb28a047e1812e6cbe5134c9327d305de56cb`. Elapsed 3.391s; worker signaled=True; independently bound child signaled=True; tail `unknown after debugger-worker termination`.
- [no-ack](resume282-color-supervisor-no-ack.json): 6970 bytes; SHA-256 `b860f235c1e24f2274df7fdf0f6227fec8dadae21399c8432cef48b7767dca39`. Elapsed 1.641s; worker signaled=True; independently bound child signaled=True; tail `complete`.
- [pre-kill-hang](resume282-color-supervisor-pre-kill-hang.json): 1950 bytes; SHA-256 `21c0d11a89d2dd2fd91f3b0fa122734dacd178247b5d66abf7f5336a8eb810ca`. Elapsed 3.375s; worker signaled=True; independently bound child signaled=False; tail `unknown after debugger-worker termination`.

Normal has a qualified surrogate entry, successful worker cleanup, exact identity acknowledgement, one resume and both independent waits signaled. Missing acknowledgement times out before any ResumeThread and reaches owned cleanup. Held entry retains child wait WAIT_TIMEOUT before worker fallback, then worker/child wait success. Held EXIT_PROCESS may return TerminateProcess failure/error 5 while independent exit wait succeeds; that API failure is preserved, not rewritten as successful termination.

Pre-explicit-policy hang confirms bounded termination and exit of the owned worker. It occurs before child-handle transfer, so independent child exit remains **UNKNOWN**. This is deliberately not a passing default-kill-on-exit proof or a complete creation-aware absence check. Neither the worker's disappearance nor a PID-only assumption substitutes for that missing child handle.

## Validation and exact HOLD

The bounded test receipt is [resume282-color-supervisor-tests.json](resume282-color-supervisor-tests.json), with final source hashes and exact commands/output. Six new tests cover normal acknowledgement/entry, selected-entry hang, held EXIT_PROCESS, missing acknowledgement, pre-explicit-policy hang uncertainty, and arbitrary/original refusal. The existing twelve CP281 tests also pass. Tests and receipts use only the fixed harmless child; no converter or DMO build occurred.

Original readiness is false. Remaining gates include independently verified child exit in the creation-to-handshake interval; crash and pre-acknowledgement hang matrix beyond the current fixed injections; seeded occupied DR state and original initial-create-event arming/readback; original mapped PE/header/section/image-file and dependency identity binding; frozen fresh reservation/clone/command manifests; high-volume event/receipt bounds; injected memory/context/continuation failures; OS-invalid-handle/reuse and independent absence checks; and independent source/policy/neutrality review. This first supervisor increment does not release the original or qualify a full native failure matrix. CP281's primary-thread marker-armed scope and limits remain attached; no full Open, helper return, color effect, Save or Close claim follows.

Final before/after implementation hashes (identical in all five raw receipts):

- `color_original_entry_observer.py`: `def5c760da99860553a1fc6ca4c06149f9b7e114410b63b2e389cde6933fdc25`.
- `color_entry_supervisor.py`: `94ec30292d2df28fbac20ba72206ea174ab00d5f8b76bdb4eb35515ada1e0b55`.

Final tests: six supervisor tests in 12.356s and twelve observer tests in 2.263s, both exit 0. Source and tests frozen after these receipts; source ownership released for independent review.
