# CP202 requested-visibility observation alternative

Use **original EXE RVA `0x1547e`**, at the start of the straight-line argument-preparation block, as the next candidate. At entry, read the bool from **`[RSP+0x74]`**, the main pointer from **`[RSP+0x250]`**, and the dock pointer from **`[main+0x420]`**. Do not read RDX as the requested bool at this earlier address. The block later loads that exact byte into EDX and calls the dock virtual. This candidate avoids beginning the overwrite window at the indirect call that failed. Static inspection supports a simpler relocation window; **successful Frida interception is still unproved and must pass the gates below**.

This report used the reverse-engineer-anything skill for bounded copied-PE inspection. Only this report was written. No source edits, build, original launch, GUI control, live REA attach/mutation, installed executable change, parent DMO change, or Claude change occurred.

## Identity and retained failure

The copied original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, historical product version **11.0.2**, SHA-256 **`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`**. This audit rehashed all 67,611,136 file bytes. Its PE preferred base is `0x140000000`; SizeOfImage is 87,420,928 (`0x535f000`). The CP202 runtime receipt reports the same file hash and image size. CP202's `originalVersionResource` is null: 11.0.2 is the retained historical product identification, not a newly verified PE version-resource value. No reconstructed DEF_PRE instance is involved.

Read the bounded retained files under `analysis/resume202-passive-oracle-run-2026-10-26T04-28-23-604Z/`: `frida-messages.jsonl`, `summary.json`, `held-handle-helper.jsonl`, `capture-manifest.json`, and the observer. The first module message gives actual base `0x7ff648e40000` and the exact `0x154a4` prefix:

```text
ff 50 58 48 8d 0d 0a 12 d6 03 e8 0d 63 2b 00 0f b6 c0 85 c0 74 5d 45 33
```

`Interceptor.attach` then failed at `0x7ff648e554a4` with `unable to intercept function ...; please file a bug`. The terminal message has zero observations, `installed: []`, and empty readiness. The helper has creation/suspended, terminate-command endpoint and verified cleanup records, with **no resume record**; cleanup reports wait-result 0, exit-code 57857 and handles closed. The helper exited 0. Thus this is an installation failure before original startup, not a negative requested-visibility result or evidence about Save. `summary.json` retains `processAbsenceVerified: false` even though its held-handle exit and separate absence observations are positive; preserve that aggregate qualification. The pre-existing no-fixture-change result is not new evidence of Save absence. This audit did not independently rerun fixture manifests.

The manifest binds `frida-messages.jsonl` to SHA-256 `619d652232fcec6c0f0e1e0bd791ff8fd061caf6137166948c4aefb55859bf8c` and `summary.json` to `be5bfcec8e3e893f948df3c5b25d9b0133175564ca2ca67969062b80582bb7fd`.

## Exact candidate and argument meaning

DUMPBIN 14.44.35228.0, read-only `/DISASM:BYTES /RANGE` on the copied original, decodes:

| Preferred VA | Exact bytes | Instruction |
|---|---|---|
| `0x14001547e` | `48 8b 84 24 50 02 00 00` | `mov rax,[rsp+0x250]` |
| `0x140015486` | `48 8b 80 20 04 00 00` | `mov rax,[rax+0x420]` |
| `0x14001548d` | `48 8b 8c 24 50 02 00 00` | `mov rcx,[rsp+0x250]` |
| `0x140015495` | `48 8b 89 20 04 00 00` | `mov rcx,[rcx+0x420]` |
| `0x14001549c` | `48 8b 00` | `mov rax,[rax]` |
| `0x14001549f` | `0f b6 54 24 74` | `movzx edx,byte [rsp+0x74]` |
| `0x1400154a4` | `ff 50 58` | `call qword [rax+0x58]` |

Candidate file offset is **`0x1487e`**. Its exact 24-byte prehook prefix is:

```text
488b842450020000488b8020040000488b8c245002000048
```

Resolve the hook as **current module base + `0x1547e`**, never a retained ASLR VA. At the retained CP202 base only, it would have been `0x7ff648e5547e`; that is not a future address allocation.

The exception table contains the candidate in unwind region `[RVA 0x151a0, RVA 0x15ae5)`, unwind-info RVA `0x311205c`. Region entry saves RCX at entry-RSP+8, pushes RSI and RDI, then subtracts `0x238` from RSP. Hence the saved original RCX is at current RSP+`0x250` (`8+16+0x238`), consistent with its repeated use as main in the retained CP173 route. Immediately before the candidate, branches at `0x15464` and `0x15477` target `0x1547e`; fallthrough may first clear `[rsp+0x74]` at `0x15479`. The bounded direct-branch scan of this unwind region found no branch into the candidate's later load instructions. This does not prove absence of arbitrary indirect entry.

The first **15 bytes** are two complete ordinary MOV loads; the first **38 bytes** up to the failed call are only straight-line loads, without relative branches or RIP-relative operands. No instruction in that span overwrites `[rsp+0x74]` or adjusts RSP. Therefore a callback at `0x1547e` sees the exact byte subsequently zero-extended into EDX at `0x1549f`. RCX and RDX at candidate entry need not yet contain the dock and bool. Record raw uint8, and require 0/1 for an admitted bool observation; preserve unexpected values instead of silently normalizing them. The main/dock reads are memory reads only and must preserve read failures.

Use an **onEnter-only** instruction hook and `this.context.rsp`, not `args[]` or an onLeave function-return interpretation. Proposed callback fields are candidate RVA, TID/sequence, RSP, `main=RSP.add(0x250).readPointer()`, `dock=main.add(0x420).readPointer()`, and `requestedRaw=RSP.add(0x74).readU8()`. Do not call native helpers, Qt APIs, event pumps, or change registers, target memory, return values or application control flow. Interceptor itself changes executable code/timing; this is passive application-state observation under that instrumentation qualification, not zero-mutation instrumentation.

Label the record **prepared requested bool before dock call**. A subsequent fault/termination could prevent the virtual call, so this earlier observation alone is not proof of callee entry. CP173 maps the dock virtual+0x58 thunk `0x14013876c` through IAT `0x141e0bed0` to Qt `QWidget::setVisible(bool)`; retain its source qualification. Requested state still does not prove delivered Show, true emission, connected slot delivery, unbusy state, auto-save, writer success, or accepted Close.

## What the failure explains

The failed address begins with a three-byte indirect call, followed by a seven-byte RIP-relative LEA and a relative CALL. An interceptor that needs a larger overwrite window must decode/relocate more than that first instruction. The proposed address instead offers 15 bytes of two simple loads before any such instruction. This is a concrete reason to test the alternative.

It is **not proof of the failure's cause**. The retained error contains no relocator diagnostic, requested patch size, trampoline placement, memory-protection result, Frida/Gum version, or internal error code. Neither a three-byte first instruction nor a RIP-relative operand alone proves unsupported interception. Possible decoding/relocation, trampoline-placement or protection/backend failures remain unseparated. Exact prefix matching rules out a mismatch of those checked bytes at that moment; it does not explain the error. Do not claim that moving the hook fixes it until tested.

## Gates before a future original resume

1. In a separately authorized **harmless owned x64 fixture**, validate the same interceptor backend against the exact copied candidate instructions and stack/main/dock layout. Exercise known 0 and 1 bytes; confirm callback RSP semantics, main/dock correlation and preserved execution/register behavior. Retain Frida/Gum identity, exact script hash, prehook bytes, installed patch length/bytes where observable, callback records, detach/restoration and exact-owned-process cleanup. Do not execute the copied production application or call production Qt functions to obtain this result. This report creates or executes no fixture. A fixture pass establishes only that backend/layout case, not original interception.
2. Rehash the actual original and loaded selected module files; enforce the hash, canonical path, x64 architecture, image size and exact candidate prefix above. Preserve the explicit-cwd, creation-identity and held-process/thread-handle ownership lane. Never attach to an unrelated enumerated user process. Capture all required prefixes before any interceptor modifies overlapping bytes.
3. On a separately authorized **owned original held suspended**, attempt the revised candidate and every other required EXE hook. All EXE listeners, module-observer continuity and script readiness must be positively verified before the one permitted resume. Preserve the Qt late-module readiness gate. The other existing mid-block hooks are not validated merely because this candidate succeeds. An attach error or incomplete readiness means **do not resume**; stop observation, terminate only the exact held owned process, retain wait/exit/handle receipts and independently verify same-device absence. Do not downgrade observation coverage or infer a missing signal.
4. After a fully ready startup capture, retain ordering and dock/main identity across CP173's remaining event, signal, slot, busy-result and auto-save observations. Report each edge separately and compare disposable/source manifests. Keep the accepted-Close blocker independent. No GUI acquisition or app-control release claim follows from this lane.

These are future preflight gates, not actions performed by this read-only audit. The candidate is ready for harmless relocation validation; original runtime behavior remains undecided.
