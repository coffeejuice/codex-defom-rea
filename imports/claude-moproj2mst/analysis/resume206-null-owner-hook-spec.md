# CP206: passive null-owner hook specification for one B98 CLI Open

## Purpose and evidence boundary

Determine whether the postOpen notifier's null `P=[O+30h]+38h` was returned null by its producer, or arose after that producer's store through a field clear or holder replacement. This is a **capture specification only**. It neither authorizes nor performs another original run, Frida attach, GUI action, source edit, build, deployment, or mutable REA operation.

The retained CP205 run (`analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`, qualified by `analysis/resume205-original-crash-capture-qualification.md`) used original `DEF_GUI_MO_64.exe` **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its same-run accurate stack binds the fault to `eaf60` via `eb1ec`; the Qt size leaf read at `0x58` with `RCX=0x58`. `analysis/resume205-rea-crash-chain.md` derives `P=0` at the selected accessor. The copied PE at `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed to the same digest for this specification. Bounded Microsoft DUMPBIN 14.44.35228.0 disassembly of preferred-VA ranges `1400eaff0..1400eb025`, `1400eb1c0..1400eb1f0`, `14001d510..14001d540`, and `140045780..1400457f0` confirms the instruction boundaries below. These are static instructions, not a second observed occurrence. App RVAs are relative to the actual module base at runtime; the CP205 base `0x7ff648e40000` must not be reused blindly.

## Four exact instruction hooks

For one owned, identity-validated, suspended original process, install only these four `Interceptor.attach` observers at **app module base + RVA** before its single resume. At an instruction hook, sample registers *before* the addressed instruction executes; do not replace the instruction, alter registers, return values, memory, exceptions, scheduling, or branch flow. Record the raw register values and the indicated guarded pointer reads. `S` in this table is `eaf60`'s current post-prologue `RSP`, not a process-wide stack address.

| Hook RVA | Instruction and exact pre-instruction operands | Log |
|---|---|---|
| `0xeb00f` | `mov qword ptr [rax+38h],r11`; `RAX=H` from `[O+30h]`, `R11=P_producer` copied from `1d510`'s returned `RAX` at `eb000`; `O=[S+170h]` | TID, sequence, `S`, `O`, `H=RAX`, `P_producer=R11`, field address `H+38h`, guarded old `[H+38h]`. This is the producer result at the actual write site. |
| `0xeb013` | Immediately after that store, before `mov dword ptr [rsp+54h],0`; `RAX=H`, `R11=P_producer`, `O=[S+170h]` | Same identifiers, guarded committed `[H+38h]`. A successful read equal to `R11` establishes that the selected store completed and the value was observable at this point. |
| `0xeb1e3` | `mov rcx,qword ptr [rcx+38h]`; `RCX=H` loaded at `eb1db` from `[O+30h]`, `O=[S+170h]`; `EDX` is the loop index loaded at `eb1df` from `[S+74h]` | TID, sequence, `S`, `O`, `H=RCX`, index `EDX`, guarded `[H+38h]` immediately before the load. |
| `0xeb1e7` | `call 0xa74f0`; preceding `eb1e3` has loaded `RCX=P_consumed`; `EDX` retains the index | TID, sequence, `S`, `O=[S+170h]`, current `H=[O+30h]` by guarded read, `P_consumed=RCX`, `EDX`. This is the value actually passed to the accessor; a separate field reread is only diagnostic and must not replace `RCX` as evidence. |

`eb00f` is the earliest sufficient producer-result hook: `eaffb` calls `1d510`, `eb000` copies its return to `R11`, `eb003` reloads `O`, `eb00b` reloads `H`, and `eb00f` stores `R11`. The copied `1d510` body calls `133a0` and `45780`; `45780` has two explicit zero-return arms and otherwise calls `44a90`. A hook at `1d510` entry or `45780` alone would not bind the result to this particular `O,H` store. The consumer-side `eb1d3`, `eb1db`, `eb1df`, `eb1e3`, `eb1e7` instructions bind the later load to the same local `O`; no selected-path instruction replaces `P` between `eb1e3` and the call. Do not infer semantic C++ type, ownership, or lifetime solely from these offsets.

## Correlation, bounds, and interpretation

Use a monotonic in-process event sequence plus PID, process creation identity, TID, and `eaf60` frame `S`. Pair `eb00f` with the immediately following `eb013` for that thread/frame and `O,H`; pair each `eb1e3` with the immediately following `eb1e7` for that thread/frame and loop index. For the causal comparison, select the most recent completed producer store for the **same `O` and `H` in the same `eaf60` invocation**, then the consumer pair that leads to the retained-style `a74f0`/Qt fault. Preserve all raw `O,H` values; matching an `O` alone is insufficient if `[O+30h]` was replaced. If callbacks or recursion make frame identity ambiguous, mark the pair ambiguous rather than merging records. A first-access-violation receipt should retain module identities, RIP, accessed address, `RCX`, and accurate stack so the selected consumer can be tied to the fault. Its handler must return `false`, preserving natural exception delivery.

Cap at **16 events per hook, 64 hook events total**, plus one exception receipt and compact overflow counters. Keep the hooks observational if the cap is reached; stop recording further values and mark causal classification indeterminate when the relevant producer or consumer may have been omitted. Record only scalar pointers, index, identifiers, timestamps/sequence, and guarded-read success or error; do not dump pointed-to objects, scan memory, enumerate other processes, or take stack traces at every hook. Guard Frida memory reads with local exception handling so a failed read is recorded as unreadable rather than converted into an application write or an invented zero. Avoid any callback that blocks for external input.

Interpret a completed, same-frame pair as follows:

| Observed sequence | Supported conclusion |
|---|---|
| `P_producer=0`, committed field `0`, same `H`, `P_consumed=0` | Producer returned null for this selected store; no later change is needed to explain the null. Later transitions are not excluded. |
| `P_producer!=0`, committed field equals that nonzero value, same `H`, `P_consumed=0` | The field became null after the store and before the consumer load. This proves a later state transition, but the four hooks do **not** identify its writer or distinguish an explicit clear from destruction/reuse at the same address. |
| Stored `H` differs from consumer `H` for the same `O` | The holder link `[O+30h]` changed (or its storage was reused) between observations; classify as holder replacement/identity change, and inspect the consumer's own `P`. Do not call it a clear of the prior `H+38h`. |
| `P_producer=0` but post-store field nonzero, or post-store value differs from `R11` | Pairing/race/interposition/invalid-read issue; do not classify without further evidence. |
| `P_consumed` differs from immediately preceding guarded field read | Concurrent transition or observational mismatch around the load; `RCX` at `eb1e7` remains the actual call operand. |

An old field value at `eb00f` is useful context but cannot prove all intervening writes. Matching pointer addresses do not establish object lifetime. These four hooks locate the transition interval; identifying a specific later writer would require a separately scoped observation or static proof. A reproduced crash does not itself establish the public m2m ABI behavior or warrant a converter change.

## Process and fixture ownership

Reuse the CP205 owned-process contract only if a future run is separately approved: exact installed original image hash/version, command `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, cwd `C:\PROBLEM\p202a`, suspended child identity/creation time validated before attach, and the disposable fixture's pre/post manifest digest. Resume once, permit natural exit, verify process absence and Frida detach, and retain raw event and failure receipts. No GUI input or app-control session is needed. If image, arguments, cwd, fixture digest, module identity, hook bytes, or process ownership differs, stop before resume and report the mismatch rather than attaching to another process. Keep generated project paths under `C:\PROBLEM` and under 100 characters. The four hooks instrument executable code in memory as Frida normally does, but specify no application data or control-flow mutation.
