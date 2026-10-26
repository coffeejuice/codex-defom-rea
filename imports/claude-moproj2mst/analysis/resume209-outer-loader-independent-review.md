# CP209 outer-loader independent static review

**PASS for the bounded static claim.** In the copied executable's ordinary call/return flow, the direct notifier instruction at RVA `2a720` belongs exclusively to the incoming-null arm of `2a0e0`. Main forwards the `1ca30` result to `2a0e0` without testing that result. This does not establish why a particular loader invocation returned null or independently validate CP205/CP207 runtime capture attribution.

## Evidence and method

Reviewed the copied original `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, and retained DUMPBIN instruction listings. All abbreviated addresses below are RVAs relative to preferred image base `0x140000000`.

Independently parsed PE section mappings with Python, then compared every listed instruction byte, including wrapped continuation bytes, against the copied PE. The callback listing matched 432 instructions / 2,239 bytes; main matched 554 instructions / 3,236 bytes, with zero mismatches. Recomputed listing hashes:

| Retained artifact under analysis/ | SHA-256 |
|---|---|
| wave3-boolean-external-callback-14002a0e0.asm | e3e74166115892e04fe23160955278cf3101abacb6d818cd41746bca94c43f63 |
| wave3-editor-owned-open-entry-140001410.asm | a18e1a6b48e042f2795bf9536d7bcc299d4e9ba8532a9b189912a719a161778e |

Retained Ghidra `wave3-boolean-open-registration-14002a0e0.c` corroborates the null split, but the verdict rests on the byte-matched instructions. Applied the reverse-engineer-anything skill's shipped-artifact evidence discipline. No live REA tool/session, original process, GUI control, build, deployment, fixture change, or mutable analysis database was used. Only this assigned report was written.

## Exact branch and value flow

`2a0e0` saves incoming RDX at entry `[entry_rsp+10h]`. The two pushes (`2a0ea`, `2a0eb`) and `sub rsp,488h` (`2a0ec`) place that saved argument at `[current_rsp+4a8h]`, since `0x10 + 0x10 + 0x488 = 0x4a8`. Incoming RCX is correspondingly at `[rsp+4a0h]`. The later zeroing of EDX at `2a111` prepares a different call and does not change the saved argument.

The controlling instructions are:

```text
2a120  cmp qword ptr [rsp+4a8h],0
2a129  je 2a1a6
```

Nonnull fallthrough occupies `2a12b..2a1a5`. It optionally calls `116ea0`, then calls `a5890` (`2a163`), `133a0` (`2a173`), `c79ee0` (`2a17e`), and `2a9b0` (`2a19c`). Its unconditional jump at `2a1a1` targets `2a94e`, beyond the direct notifier in question. It cannot fall through into the null arm on an ordinary return from `2a9b0`.

The null arm starts at `2a1a6`. Before `2a720`, its only listed conditional branches are `2a1b6 -> 2a1ce`, `2a23a -> 2a28f`, and `2a29f -> 2a2f4`; all reconverge before the notifier. There is no local conditional branch bypassing `2a720` on this arm. The earlier direct notifier at `2a523` is prepared with the literal at VA `142100158`, independently read as `2signalProjectChanged()`.

At `2a712`, RDX receives VA `142100170`; its copied-PE bytes independently decode as `2signalRefreshMainView()`. At `2a719`, RCX receives VA `143d76700`. The direct call at `2a720` is `2cf3c0`, with return address `2a725`. A later direct notifier at `2a91d` uses `2signalDiagEditorFitAll()`. The common virtual-call tail starts at `2a94e`.

Therefore reaching this particular `2a720` instruction through entry `2a0e0` requires the saved incoming argument to compare equal to zero. If all preceding null-arm calls return normally, that arm reaches the instruction. This conclusion concerns the particular direct call site, not every possible refresh event or receiver delivery.

Main's handoff is entirely straight-line after the filename-length gate:

```text
1c7e  test eax,eax
1c80  jle 1cc1                     ; skip Open when length <= 0
1c82  lea rdx,[rsp+1918h]          ; selected path
1c8a  lea rcx,[rsp+40h]            ; same main-window object
1c8f  call 1ca30
1c94  mov [rsp+1920h],rax
1c9c  mov rdx,[rsp+1920h]
1ca4  lea rcx,[rsp+40h]
1ca9  call 2a0e0
1cae  ...                         ; postOpen return address
```

There is no intervening call, comparison, conditional branch, or write to the saved result between `1c94` and `1ca9`. Thus loader RAX, including zero, is forwarded exactly. “Unconditionally” here means after the selected Open route passes its filename gate and the loader returns normally; it does not mean main always invokes Open regardless of arguments or faults.

## Limits and integration consequence

A qualified runtime stack containing this notifier's return `2a725` and main's return `1cae` is consistent with, and under the ordinary flow above implies, a zero loader result forwarded by main. This review did not re-audit those stack captures, measure loader RAX at runtime, establish the cause of a null result, or establish that every B98 launch follows it. The “model pointer” semantic label is an interpretation; the exact static predicate is saved incoming RDX == 0.

Earlier callees may throw, fault, terminate, or alter control flow; notifier internals may suppress delivery. Deeper nonnull-arm callees or common virtual targets may issue other refreshes. Arbitrary interior entry, deliberate return-address changes, stack corruption, and reentrant invocations with their own arguments are outside this ordinary-entry control-flow claim. A reentrant null invocation would be a distinct invocation, not evidence that the outer nonnull arm directly executes this site.

The review passes the CP209 branch distinction and caller handoff only. It does not establish a shipped headless Open-to-Save interface, successful Save, or authority to add this notifier to every successful candidate Open. Recovering the loader's first differing prerequisite remains necessary before mapping the original null outcome onto candidate lifecycle behavior.
