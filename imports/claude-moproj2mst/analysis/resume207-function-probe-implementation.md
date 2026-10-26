# CP207 two-function owner probe implementation

## Purpose and state

`admin/original_function_owner_probe.mjs` is a new, default-disabled, one-run owned-original probe. Its source SHA-256 is `fdb5e056df5db4d17eded8d72cade418d6e47c6e9adbc656e7fad781af07bab`. `ORIGINAL_RUN_REVIEWED` remains `false`, and the runner still requires the exact `--execute-reviewed-probe` argument. This preparation did not launch, attach to, or resume the original, and did not alter the fixture, converter, helper, or installed executable. The original belongs to the v11.0.2 installation (PE version resource was reported null); its pinned SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Instrumentation and identity gates

The runner retains the CP205/CP206 exact original path, `C:\PROBLEM\p202a\Gear_blank.moproj` command and cwd, project hash, 27-file source/disposable manifest digest, helper hash, Frida 17.23.2 pin, suspended exact-handle ownership, and single resume. Before resume it requires the created PID, primary TID, creation FILETIME text, image, command, cwd, project and fixture identities; the loaded module path and `0x535f000` image size; both exact entry prefixes; and a handler-ready receipt after both `Interceptor.attach` calls. Any failure terminates only the owned child through the helper. No nearby-site fallback exists.

Exactly two function-entry hooks are installed:

| Site | Qualified observation |
| --- | --- |
| EXE RVA `eaf60`, prefix `48 89 4c 24 08 53 48 81 ec 60 01 00 00` | `onEnter` accepts only return EXE+`eaa7a`, records raw `RCX=O`, entry RSP, TID, invocation ID and sequence, and pushes one frame on a per-thread stack. `onLeave` retires only its own top frame. |
| EXE RVA `1d510`, prefix `48 89 4c 24 08 48 83 ec 28 48 8b 4c 24 30` | `onEnter` accepts only return EXE+`eb000` with exactly one active `eaf60` frame on the same TID. It saves that invocation, `O`, input RCX and entry RSP. A normal `onLeave` records raw `retval=P` and guarded `[O+30h]=H`, `[H+38h]` reads with explicit success/error states. These reads are pre-store context, not proof of the `eb00f` store. |

There is no `a74f0` hook. No callback changes registers, return values, target data, or exception disposition. Per-kind event caps are 16 each for owner entry, producer entry and producer return, with a total cap of 48. Overflow and rejected-caller/ambiguity counters are retained. The first AV core includes RIP, module/RVA when available, read operation/address, RCX, creation identity, TID, active invocation IDs, sequence and counter snapshots **before** the accurate backtrace and full register record. The handler returns `false`.

## Host qualification and limits

The host checks sequential event parity against the first-AV snapshot, count/overflow bounds, creation identity, thread and invocation equality, exact caller addresses, unique entry/return pairing, and active invocation at the fault. It separately requires the compact AV and full accurate stack, natural helper/child exit with no termination attempt, handle closure, Frida detach, Windows and same-device Frida PID absence, unchanged fixture digest, and raw receipt retention. A lost core or stack, callback/script error, cap reach, ambiguity, missing delivery, forced cleanup, or incomplete absence leaves the run `INCOMPLETE`.

The CP205 fault-path comparison requires the QtCore4_64.dll RVA `32c0` read at `0x58`, `RCX=0x58`, and contiguous application return RVAs `a7510 -> eb1ec -> eaa7a` from the same first AV. A different or unmapped execute AV is classified as probe-path divergence regardless of hook counts. Only a complete same-path capture with a unique same-invocation producer return may say that this producer returned zero or nonzero before the selected null consumption. A nonzero return would narrow the unresolved change to after the return; this layout cannot establish a committed store, identify a writer, or prove object lifetime. Entry hooks may still change the fault path, and CP206's unmapped execute AV mechanism remains unresolved.

## Static checks

- `node --check admin/original_function_owner_probe.mjs`: passed.
- Extracted injected template with interpolation placeholders replaced by scalar literals, then `node --check` on stdin: passed. This checks generated callback syntax shape without loading Frida or the original.
- No runtime hook delivery, fixture test, target path neutrality, or causal outcome is claimed. A separate owned-original review and explicit guard change would be required before execution.

Sources: `analysis/resume207-function-hook-plan.md`, `analysis/resume205-original-crash-capture-qualification.md`, `analysis/resume206-probe-message-loss.md`, and the CP205/CP206 runner implementations.
