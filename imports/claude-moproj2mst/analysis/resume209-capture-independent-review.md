# CP209 independent review of the CP207 two-function capture

**PASS for the bounded same-path producer-return claim. HOLD for any committed-store, specific null-arm, or converter acceptance claim.** This audit reads retained evidence only. It performed no original run, attach, GUI operation, source change, build, or shared-state edit.

The reviewed run is `analysis/resume207-original-function-owner-2026-10-26T11-51-11-398Z/`. Its target is original `DEF_GUI_MO_64.exe` from the v11.0.2 installation, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The version resource is null; 11.0.2 is the installation version. No reconstructed application is assessed.

## Independent checks

The target, command (`-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`), cwd (`C:\PROBLEM\p202a`), and fixture digest exactly match qualified CP205 run `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/`. The CP207 fixture-before and fixture-after files are byte-identical; their parsed manifest also equals CP205's before manifest. The 27-file digest is `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`.

The four raw helper JSONL receipts independently retain suspended creation, one resume with previous suspend count 1, target-exited endpoint, and cleanup. Parsed raw receipts match the summary's retained raw receipt strings. PID is 31636, primary/fault TID 31032, launcher PID 39068, host Sim2. The exact creation FILETIME is **134374890722592589**, preserved as raw JSON integer and as text in the hook/core records. The summary's JavaScript-parsed numeric creation field rounds to 134374890722592590; it must not be used as the exact identity. This rounding does not break this run's text-based correlation.

The retained generated script hashes to the recorded `6db92418e8ee7dd1e66ce7d59ac3dc3ab1b5c1c61e9dcc3d6843debdf36b44ad`. It checks the loaded image path, size and entry bytes, installs only the planned `eaf60` and `1d510` entry hooks, qualifies return callers `eaa7a` and `eb000`, and captures `1d510`'s normal return through its function `onLeave`. Handler-ready precedes resume in the receipts. The first-AV compact core precedes full backtrace enrichment; the handler returns false. There is no optional third consumer hook.

The first AV is a read of `0x58`, `RCX=0x58`, at `QtCore4_64.dll` RVA `0x32c0`. The full accurate-stack module-offset sequence equals CP205's sequence, including app `4af4 -> a7377 -> a7510 -> eb1ec -> eaa7a -> dafad`. Thus the planned CP205 same-fault-path gate passes, unlike CP206's divergent unmapped-execute occurrence. Module path/base/RVA establish identity; inconsistent symbol labels alone do not.

The six hook records are consecutively numbered 1 through 6, with two owner entries, two producer entries and two normal producer returns. The compact-core snapshot agrees with all six received records; overflow is zero and counts are below both caps. There are no hook/script/handler errors. Forty-four unrelated producer callers are explicitly rejected, not silently paired. `noUniqueOwner` and `ownerLeaveMismatch` are zero.

At the first AV the active invocation list is solely `2`. Its owner/entry/return sequence is **4/5/6**, all with the exact creation text, PID 31636, TID 31032 and invocation ID `2`. The owner is `O=0x3e34320`, caller `base+eaa7a`; producer input is `0x10fe2d0`, caller `base+eb000`, and entry RSP `0x10fd368` agrees between entry and return records. The raw normal return is **`producerP=0x0`**. Guarded pre-store reads successfully report `H=0x3e1eb00` and `[H+38h]=0`. Invocation `1` also returned zero, but it is not active at the AV and must not be substituted for invocation `2`. The unique selected pairing therefore passes independent inspection.

Cleanup is complete for the owned process: exact-handle exit `3221225477` (`0xC0000005`), closed handles, zero termination attempts, helper exit 0, detached Frida session, and independent successful Windows and same-device Frida absence receipts for PID 31636. Script unload is correctly recorded as not applicable after natural target exit. No GUI control was acquired, so no GUI release is claimed. The runner's current original-run guard is false.

## Exact justified claim and limits

**In this owned, two-hook original v11.0.2 B98 CLI Open occurrence, the exact caller-qualified `1d510` call in fault-active `eaf60` invocation 2 normally returned zero before the CP205-style null-consumption fault.** The qualified stack plus copied-code chain supports null consumption at the selected downstream accessor, as explained in `analysis/resume205-rea-crash-chain.md`; this is a static-chain inference combined with runtime fault evidence, not a third-hook observation.

The successful `[H+38h]` read is pre-store context. It does not witness `eb00f` committing, prove the same holder remained installed at consumption, exclude intervening writes, identify object lifetime, or select either internal `45780` zero arm. Those distinctions follow the controlling `analysis/resume207-function-hook-plan.md` and `analysis/resume207-null-producer-static.md`. Same-path parity in this one run does not prove universal instrumentation neutrality. The capture establishes a producer return that CP205 lacked; it does not establish a complete causal account of every intervening state transition.

CLI Open still faults before completion; this run supplies no Save or accepted Close evidence and no converter acceptance pass. No behavior change is justified merely by the summary's `CAPTURE_COMPLETE` label. Source qualifications, immutable fixture identity, raw receipts, and the above limitations remain attached to this PASS.
