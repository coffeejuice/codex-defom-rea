# CP202 remaining five hook sites: static relocation audit

Prioritize `auto_save_call` and `show_signal_call`: both start with a five-byte relative CALL. The Qt site immediately follows it with a relative JMP and has a nearby independently reached block. `busy_return` is less exposed for a short patch but encounters a conditional branch after 11 bytes. The two function-entry hooks have ordinary prologues. These are static risk rankings, not predictions of Frida success or a diagnosis of the prior interception failure. No site was intercepted in this audit.

Used the reverse-engineer-anything skill for copied-PE inspection. Only this report was written. No original launch, GUI session, REA attach, mutable analysis database, runner/source/build/deployment change, parent DMO change, or Claude change occurred. The revised visibility site `0x1547e` is excluded.

## Evidence and method

Rehashed `evidence/original-binaries/DEF_GUI_MO_64.exe`: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`. Rehashed `evidence/original-binaries/QtGui4_64.dll`: SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, preferred base `0x65000000`. PE section mapping and exception-directory entries were read with Python `struct`; DUMPBIN 14.44.35228.0 `/DISASM:BYTES /RANGE` decoded bounded copied-file ranges. Complete instructions quoted below were inside those bounds; truncated trailing decode output was discarded.

All five runner 24-byte prefixes matched the copied files exactly. Function-region entries `EXE [0x3a990,0x3ae33)` (unwind RVA `0x311a028`) and `QtGui [0x39ffa0,0x3a03db)` (unwind RVA `0x86449c`) anchor sequential decoding to the hook boundaries. Unwind ranges alone do not establish complete logical functions or exclude indirect entry. Use live module base plus RVA in any future probe; preferred addresses here are static coordinates.

Read source qualifications in `analysis/resume173-headless-oracle-next.md`, `analysis/resume201-passive-oracle-probe-plan.md`, `analysis/resume202-original-save-probe-source-review.md`, and `analysis/resume202-visibility-hook-alternative.md`. CP173's Qt export mapping identifies the event and signal bodies; its conditional Show/Hide route and busy/auto-save meanings remain static capability evidence, not observed startup behavior or writer success.

## Exact current sites

| Priority | Site, RVA / file offset | Exact 24-byte prefix | Decoded initial window and risk |
|---|---|---|---|
| 1 | EXE `auto_save_call` `0x3aaa2` / `0x39ea2` | `e8d960fdff884424640fb644246485c0750dc78424c00100` | `e8d960fdff` CALL EXE+`0x10b80` (5); `88442464` MOV `[rsp+0x64],al` (4); `0fb6442464` MOVZX EAX (5); `85c0` TEST (2); `750d` JNE (2). Any overwrite starts by relocating a relative CALL; a larger window later encounters a branch. |
| 2 | QtGui `show_signal_call` `0x3a0084` / `0x39f484` | `e877d8ffffe946020000488bcee88ae2ffffe93902000032` | `e877d8ffff` CALL QtGui+`0x39d900` (5); `e946020000` JMP +`0x3a02d4` (5). At +`0x3a008e`, `488bce` MOV RCX,RSI then `e88ae2ffff` CALL +`0x39e320`. Five-byte window ends cleanly at +`0x3a0089`; a 14-byte window crosses the unconditional JMP into a different dispatch block. |
| 3 | EXE `busy_return` `0x3aa86` / `0x39e86` | `884424620fb644246285c07407c644246301eb3e488b8c24` | `88442462` MOV `[rsp+0x62],al` (4); `0fb6442462` MOVZX EAX (5); `85c0` TEST (2); `7407` JE +`0x3aa9a` (2); `c644246301` MOV `[rsp+0x63],1` (5); `eb3e` JMP +`0x3aad8` (2). Nine complete ordinary bytes satisfy a five-byte minimum. A 14-byte minimum requires 18 complete bytes and relocates the JE. |
| 4 | EXE `slot_entry` `0x3a990` / `0x39d90` | `8854241048894c240856574881ecd8010000c64424600048` | MOV `[rsp+0x10],dl` (4), MOV `[rsp+8],rcx` (5), PUSH RSI (1), PUSH RDI (1), SUB RSP,`0x1d8` (7). First 18 bytes are complete ordinary prologue instructions; no relative CALL, branch or RIP-relative operand. |
| 5 | QtGui `dock_event` `0x39ffa0` / `0x39f3a0` | `405355565741544881eca000000048c7442440feffffff4c` | PUSH RBX (2), PUSH RBP (1), PUSH RSI (1), PUSH RDI (1), PUSH R12 (2), SUB RSP,`0xa0` (7). First 14 bytes are complete ordinary prologue instructions. The next nine-byte MOV stores -2 at `[rsp+0x40]`; no relative operand in those 23 bytes. |

The five-byte and 14-byte windows are illustrative patch-size cases, not an assertion about the backend's chosen overwrite length. Relative instructions can be supported by relocators. Their presence alone does not prove rejection; decoding, trampoline placement, protections and backend behavior remain unseparated. None of these five starts with a RIP-relative memory operand. RIP-relative operands occur nearby (for example the busy call's preceding LEA), so simply moving a few bytes backward is not automatically an improvement.

## Candidate replacements and semantic constraints

### Auto-save

Earlier execution candidate EXE RVA `0x3aa9a` / file `0x39e9a` begins `48 8b 8c 24 f0 01 00 00` (`mov rcx,[rsp+0x1f0]`), followed by the existing CALL at `0x3aaa2`. Exact 24-byte prefix:

```text
488b8c24f0010000e8d960fdff884424640fb644246485c0
```

This provides eight ordinary bytes for a short patch, and is the JE destination when busy is false. Read main from `[RSP+0x1f0]`, not entry RCX, which has not yet been loaded. It records preparation immediately before the call; it cannot prove callee entry. A larger overwrite still includes the CALL and therefore does not remove the main relocation issue.

Stronger relocation candidate: actual callee entry EXE RVA `0x10b80` / file `0xff80`, exact prefix:

```text
48894c24084883ec7848c7442440feffffffc64424280148
```

Its first 18 bytes are MOV `[rsp+8],rcx` (5), SUB RSP,`0x78` (4), MOV qword `[rsp+0x40],-2` (9), with no relative operands. An onEnter hook sees the main in RCX. Filter its saved incoming return address to **EXE base+`0x3aaa7`**, and correlate the active slot/TID, to isolate precisely the old caller. This observes actual callee entry rather than the call instruction; retain that event-label distinction. The function has other callers. Merely logging every entry would change coverage semantics.

### Show signal

There is no earlier single straight-line block with both the final bool and a long ordinary overwrite span: either negative position check jumps to `0x3a007f` (`33 d2`, XOR EDX,EDX), while nonnegative checks use `0x3a007b` (`b2 01`, MOV DL,1) then `0x3a007d` (`eb02`, JMP `0x3a0081`). At `0x3a0081`, `48 8b cd` loads RCX from RBP, leaving only three ordinary bytes before the existing CALL. Moving to `0x3a007b` would miss false emissions; moving before the checks would observe an undecided bool. Those are not equivalent replacements.

Use the lower-RVA actual signal entry QtGui RVA `0x39d900` / file `0x39cd00` as the relocation-friendly candidate, exact prefix:

```text
885424104883ec38488d4424484c8d4c2420488d1567fc35
```

The first 17 bytes are MOV `[rsp+0x10],dl` (4), SUB RSP,`0x38` (4), LEA RAX,`[rsp+0x48]` (5), LEA R9,`[rsp+0x20]` (4). The next instruction, at +`0x11`, is a RIP-relative LEA: keep the candidate's long-window qualification bounded to these first 17 bytes. At entry, RCX is dock and DL is the exact bool. Filter incoming return address to **QtGui base+`0x3a0089`** to preserve the Show-branch-only coverage; Hide calls this same body from `0x3a003f`, returning at `0x3a0044`. Other callers also exist. Lower RVA does not mean earlier execution: this is signal-body entry after the call executes. It demonstrates entry, not successful `QMetaObject::activate`, connected-slot delivery, or writer success.

### Busy result

The current post-call location is the earliest caller instruction at which the returned AL exists. No earlier caller point can preserve that return observation: the preceding CALL is at `0x3aa81`, preceded by RIP-relative LEA `48 8d 0d 37 bc d3 03` at `0x3aa7a`. A pre-call move must not read a stale AL as busy.

Alternative: hook actual busy function entry EXE RVA `0x2ca920` / file `0x2c9d20`, exact prefix:

```text
8854241048894c24084883ec38c644242000488b4424400f
```

First 18 bytes: MOV `[rsp+0x10],dl` (4), MOV `[rsp+8],rcx` (5), SUB RSP,`0x38` (4), MOV byte `[rsp+0x20],0` (5), without relative operands. On entry save whether incoming return address equals **EXE base+`0x3aa86`** and the slot/TID identity; onLeave record retval low byte only for that invocation. The bounded body returns via `8a442420 4883c438 c3` at `0x2ca987..0x2ca98f`, confirming returned AL. Do not attach onLeave to a CALL instruction or treat the entry value as the result. The candidate is an earlier function entry with deferred return observation; a fault/non-return means no busy-return record. Correlation and return-address filtering need a harmless fixture test before claiming equivalence under Frida.

### Entry hooks

Keep `slot_entry` and `dock_event` as the first static candidates. They already begin at PE unwind-region entries with ordinary prologues; moving later would complicate argument and stack semantics without an established benefit. Slot entry's RCX/main and DL/value are intact before stores/pushes; entry onLeave can maintain the slot stack. Dock event entry's RCX/dock and RDX/event are intact; its QEvent type read still requires the separately qualified QtCore layout/prefix. Ordinary prologues do not establish actual interceptability or safe unwinding under instrumentation.

## Review implications

Validate these exact instruction layouts using the authorized harmless owned fixture lane, then preserve the original held-suspended readiness gate. EXE auto-save entry and busy callee entry can be installed with the other EXE hooks before resume; the Qt signal entry remains subject to qualified late module loading. Capture all prefixes before any patch; if changing sites also update callback semantics, caller filters, expected hook coverage, and review receipts together. This report authorizes and performs no such edits or execution.

There is no evidence here that any of the remaining five hooks actually fails, or that any replacement succeeds. Do not weaken the required six-observation coverage after an installation failure. Do not infer a missing Show, false busy, auto-save absence, writer success, accepted Close, or GUI-control release from static code or a future partial installation.
