# CP196 known-null connector call investigation

The known-null path supports a structural CALL-only model that stops at the callee entry. It does not yet support completing the call or inventing a current-thread-data return. The nonnull path remains a separate choice for the implementation owner; this report makes no source changes.

## Qualified evidence

Read `resume163-connector-qobject-static.json` before its caller listing, and read `wave2-doe-task-public-qobject-ownership-contract.json` before interpreting the existing callee and initializer captures. These are read-only static disassemblies and source contracts, not native execution witnesses. An unwind-region or bounded window is not by itself a complete logical function. Also read the CP194/195 next, implementation and source-review reports, and current frame/validation definitions.

Rechecked SHA-256:

| Input | SHA-256 |
|---|---|
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `analysis/wave2-doe-task-qobject-current-thread.asm` | `bb4f6c61aa4879b3a4b7b9a7463f690e2f7a78fadf08dd0e7b60c39c45b29151` |
| `analysis/wave2-doe-task-qobject-thread-tls-initializer.asm` | `c06368be2e4a56cb27af21608f344fa868c383eca125e3ffd1838cd1373014b3` |

The last two match the ownership contract's recorded hashes. The imports qualification `wave3-closure-next-close-grid-qobject-child-ownership-static.json` identifies IAT VA `0x671a80a8` as `TlsGetValue` and `0x671a8090` as `TlsSetValue` against the same binary digest.

Bounded, nonmutating DUMPBIN rechecks used the already-installed executable at `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, `/disasm:bytes`, and `/range:` selections `0x6702aca0,0x6702ae00`, `0x6702ae00,0x6702ae50`, and `0x67029600,0x67029670`. Existing qualified full listings supply instruction boundaries where the selected ranges split an instruction. No split-edge byte was interpreted as an independent instruction. The tool currently hashes to `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, different from CP163's historical recorded tool digest; no tool-identity equivalence is claimed. New inspection duplicated retained instruction evidence; no new complete-function, exhaustive-xref, unwind or execution claim is made.

## Exact call boundary and possible increment

With preferred image base `0x67000000`, the qualified caller has:

```text
67131435  48 85 FF          test rdi,rdi
67131438  74 0F             je 67131449
67131449  E8 52 98 EF FF    call 6702ACA0
6713144E  49 89 44 24 40    mov [r12+40h],rax
```

CP195's known-null terminal has phase `BeforeCallerNullParentCall`, path `TakenNullParent`, fully qualified restored RDI parent expression P with P=null and matching TEST/ZF, and RSP `S+8`. S is the existing private-initializer-entry stack origin, not a concrete host address. Require all CP195 identity, owner, task/private backlink, stack lineage, flags, counts and incomplete-state checks before advancing.

A faithful next *structural* increment may execute only `[0x131449,0x13144e)`: record a new call generation with caller RVA `0x131449`, return RVA `0x13144e` and target RVA `0x2aca0`, write the qualified symbolic return address to `[S]`, set RSP to S and cursor to `0x2aca0`, and stop before `push rbx`. CALL itself preserves flags and other registers, private fields, owner/reference counts and completion flags. Do not clear RAX, consume RCX as an invented argument, set private+0x40, or move the cursor directly to the return site.

The new live `[S]` return slot replaces the earlier consumed private-initializer return address in that same physical symbolic slot. Preserve the old call/RET provenance as history rather than requiring both generations to remain live in one slot. This requires explicit frame-generation metadata and corresponding terminal/refusal snapshots; reusing the old initializer call fields unchanged would be incorrect. Default Session unknown-parent admission still stops at `0x131438`; this alternate refinement must not change that behavior.

## Callee entry, ABI and visible return

The retained callee starts:

```text
6702ACA0  40 53                         push rbx
6702ACA2  56                            push rsi
6702ACA3  57                            push rdi
6702ACA4  48 83 EC 50                   sub rsp,50h
6702ACA8  48 C7 44 24 40 FE FF FF FF    mov [rsp+40h],-2
6702ACB1  E8 4A E9 FF FF                call 67029600
6702ACB6  8B 0D 2C 5C 2C 00             mov ecx,[672F08E8]
6702ACBC  FF 15 E6 D3 17 00             call [671A80A8]
6702ACC2  48 8B F0                      mov rsi,rax
6702ACC5  48 85 C0                      test rax,rax
6702ACC8  0F 85 74 01 00 00             jne 6702AE42
...
6702AE42  48 8B C6                      mov rax,rsi
6702AE45  48 83 C4 50                   add rsp,50h
6702AE49  5F                            pop rdi
6702AE4A  5E                            pop rsi
6702AE4B  5B                            pop rbx
6702AE4C  C3                            ret
```

This visible normal path saves/restores RBX, RSI and RDI and returns the selected value through RAX. It reads no caller argument before its first nested call. That observation is not an exported prototype or proof that arbitrary incoming volatile values survive a whole call. RSP after the shown prologue is `S-0x68`; new saved slots are RBX at `S-8`, RSI at `S-0x10`, RDI at `S-0x18`, and the -2 marker at `S-0x28`. These overlap older stack histories and need separate live-slot generations if later modeled. SUB and the epilogue ADD modify flags; preserving CP194 TEST flags through a completed call would be wrong.

The retained initializer begins at `0x67029600`. Its comparison at `0x67029614` reads dword `0x672f08e8`; when not `0xffffffff`, JNE at `0x6702961b` reaches restoration at `0x670296c0` and normal RET at `0x670296ca`. Otherwise additional global initialization, synchronization and allocation-related calls intervene. The caller then reloads the actual index and calls the imported TLS getter. A nonnull getter result is copied into RSI and reaches the visible return tail. A null result takes additional adoption/allocation paths, including TLS writes; no identical-effect shortcut is supported.

Even for the existing-index/nonnull-result fast path, returning to `0x13144e` requires a source-qualified actual thread/index/value observation and owner binding. Then the instruction at `0x13144e` stores that returned pointer into private+0x40, and a later call at `0x131456` changes its reference count. Neither is part of CALL entry or justified by null parenthood alone.

## Missing evidence and safe next action

The smallest missing semantic input is an owner-qualified same-thread TLS profile: the Qt module's actual index at `0x672f08e8` must be initialized, and `TlsGetValue(index)` must resolve to the selected actual QThreadData identity under the existing shared runtime/thread owner. A host test thread ID, a known-null parent, existing notifyDepth, or a newly allocated local token cannot substitute for that identity. A full effect/return contract must also specify the getter's external observable effects and volatile-register/flag observations to the degree the model represents them. Allocation, adoption, initializer slow path and failure/unwind remain guarded until separately qualified.

Safe preparation is to specify (or, in a separately authorized implementation lane, add) the call-generation/return-slot record and CALL-only transition above, with complete before/after snapshot tests, reentry and consistent-rewind checks, refusals for forged target/return/slot generations and premature return/private+0x40/thread-reference effects. This advances to an exact static entry boundary while preserving the TLS guard. It does not complete QObject construction or publish a task. No fresh GUI capture should be acquired under the current unresolved release lifecycle.

Only this report was written. No source/tests/builds, target execution, mutable REA database, GUI control, original evidence or deployment was changed.
