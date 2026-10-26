# CP211 six-entry Frida window review: static PASS, runtime HOLD

All six current runner prefixes match the exact copied PE. Each covers the complete instructions needed for a hypothetical 14-byte entry redirect, including the repaired 21-byte Project prefix. No displaced instruction in those windows has a relative branch, relative call, or RIP-relative operand. These are conditional static PASS results; all six sites remain HOLD for actual Frida patch/trampoline qualification. Keep both runner guards false. This report does not release an original run or establish six-hook neutrality.

This independent lane applied the reverse-engineer-anything skill, read subtree AGENTS/TAKEOVER, bounded state/import/implementation/charter records, the CP210 outer Frida runner, preparation/review and capture plan, and used read-only exact-PE parsing plus bounded DUMPBIN views. No original process, GUI session, native hook, REA session, fixture change, build or deployment occurred. Only this report was written. The parent recovery-status command was also read; its GUI release restriction was preserved.

## Exact inputs and method

- Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently recomputed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained CP210 qualification identifies original installation v11.0.2 and a null PE version-resource result; installation version is not an independently read executable version. No reconstructed executable is involved.
- Independently hashed second copied PE `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe` has the same SHA-256.
- Reviewed runner: `admin/original_outer_loader_frida_probe.mjs`, SHA-256 `41c2cd035bd023c3728b1dc73bb8ef167d13ae644bd11f4b1ecfcfc61790351b`. Its `ORIGINAL_RUN_REVIEWED` and `HOOK_BYTES_REVIEWED` remain false. It requires package version `17.23.2`; that source-level check does not identify or qualify an emitted native patch.
- Disassembler: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`; output banner `14.44.35228.0`.
- Parsed PE32+ sections to translate RVAs to file offsets and the exception directory to locate the containing `RUNTIME_FUNCTION` entries. Ran `/DISASM:BYTES /RANGE:0x<entry>,0x<entry+48>` for each site. Only complete instructions through each reviewed window are used below; the artificial range end can produce incomplete trailing decode and is not evidence about those trailing bytes.
- Streamed one `/DISASM:NOBYTES` pass across the copied image, filtering its 7,218,152 instruction lines for explicit 16-hex-digit operands into the six minimum whole-instruction 14-byte windows. Exit 0, 6.75 seconds. Retained the counts and selected addresses below in this report, not a full-image transcript. This is a decoded-image direct-reference search, not a complete control-flow or indirect-target proof.

All following coordinates are EXE RVAs; preferred VA is `0x140000000 + RVA`. End coordinates are exclusive.

## Site verdicts

For a five-byte redirect every site has a first instruction of exactly five bytes. The 14-byte column is the smallest whole-instruction displacement covering bytes `[entry, entry+14)`, which may be larger than the actual entry redirect. A trampoline must resume at the whole-instruction end, never in a partially overwritten instruction.

| Site | File offset | Current prefix bytes | Whole window for 14 bytes | Resume RVA | Static verdict | Native runtime verdict |
|---|---:|---:|---:|---:|---|---|
| outer `1ca30` | `1be30` | 17 | 17 | `1ca41` | PASS, conditional 5/14-byte window | HOLD |
| duplicate `c78c10` | `c78010` | 17 | 17 | `c78c21` | PASS, conditional 5/14-byte window | HOLD |
| factory `c79390` | `c78790` | 14 | 14 | `c7939e` | PASS, conditional 5/14-byte window | HOLD |
| shared RTTI/cast `146f8fc` | `146ecfc` | 23 | 15 | `146f90b` | PASS, conditional 5/14-byte window | HOLD, global shared entry |
| Project `44a90` | `43e90` | 21 | 21 | `44aa5` | PASS, conditional 5/14-byte window | HOLD |
| acceptance `8a9b0` | `89db0` | 17 | 17 | `8a9c1` | PASS, conditional 5/14-byte window | HOLD |

Exact byte/instruction qualification follows. Memory operands are RSP-relative stores, not RIP-relative operands. There is no in-window branch or call at any of these sites.

### outer

```text
1ca30  4889542410      mov [rsp+10h],rdx
1ca35  48894c2408      mov [rsp+8],rcx
1ca3a  4881ecd8010000  sub rsp,1d8h
```

A 14-byte redirect intersects the seven-byte SUB; relocate all 17 bytes and resume `1ca41`. Original RSP home-slot stores must execute before the subtraction. No operand needs address fixup when copied to a different address.

### duplicate

```text
c78c10  4889542410      mov [rsp+10h],rdx
c78c15  48894c2408      mov [rsp+8],rcx
c78c1a  4881ecb8000000  sub rsp,b8h
```

The same 17-byte boundary rule applies; resume `c78c21`. No address-dependent operand occurs.

### factory

```text
c79390  4889542410  mov [rsp+10h],rdx
c79395  48894c2408  mov [rsp+8],rcx
c7939a  4883ec48    sub rsp,48h
```

Fourteen bytes ends exactly at the SUB boundary; resume `c7939e`. The next complete instruction is `48c744243000000000`, `mov qword ptr [rsp+30h],0`, ending at `c793a7`. This matters if the actual native backend needs more than 14 bytes: a hypothetical 16-byte redirect needs a 23-byte displacement, beyond the current runner's 14-byte equality check. Do not infer a 14-byte maximum from a generic x64 jump encoding.

### shared RTTI/cast

```text
146f8fc  48895c2410  mov [rsp+10h],rbx
146f901  4889742418  mov [rsp+18h],rsi
146f906  57          push rdi
146f907  4154        push r12
146f909  4155        push r13
146f90b  4156        push r14
146f90d  4157        push r15
146f90f  4883ec60    sub rsp,60h
```

The current 23-byte prefix covers all these instructions. A 14-byte redirect intersects `push r13`; the minimum relocation is **15**, resuming `146f90b`. Relocating only 14 bytes would split the `41 55` instruction. The 15-byte trampoline executes two home-slot stores and three pushes before returning to the original `push r14`; this changes RSP by 24 bytes exactly as original code does. The remaining pushes/SUB stay in original code. A hypothetical 16-byte redirect needs 17 displaced bytes, through `push r14`, also covered by the current prefix. There is no RIP/relative relocation operand in either span.

This is the common RTTI entry, not a selected-call-local patch. The decoded-image search found **1,017** explicit entry calls, including `81d1`, `133ca`, `1c181`, `1c7f6`, selected `1d0b8`, `1d4f2`, `3230b`, and `3af21`. Caller filtering happens only after those other calls have entered patched code and the native listener. It cannot establish their neutrality, and the many unrelated entry calls strengthen the need for a native patch/unwind qualification. The count describes DUMPBIN-recognized references, not an exhaustive runtime caller inventory.

### Project

```text
44a90  48894c2408          mov [rsp+8],rcx
44a95  4881ec88000000      sub rsp,88h
44a9c  48c7442448feffffff  mov qword ptr [rsp+48h],-2
```

The old 12-byte prefix was insufficient. Current 21-byte equality covers the full nine-byte immediate store crossed by a 14-byte redirect, resuming `44aa5`. `-2` is a sign-extended immediate value, not a relative address. All three instructions must preserve their order and RSP relationship in the trampoline.

### acceptance

```text
8a9b0  4889542410      mov [rsp+10h],rdx
8a9b5  48894c2408      mov [rsp+8],rcx
8a9ba  4881ec68010000  sub rsp,168h
```

Relocate 17 bytes for a 14-byte redirect; resume `8a9c1`. No address-dependent operand occurs.

## Incoming targets, caller identity, and unwind boundaries

The streamed operand search found **zero explicit references into the strict interior** of each minimum window above: `(1ca30,1ca41)`, `(c78c10,c78c21)`, `(c79390,c7939e)`, `(146f8fc,146f90b)`, `(44a90,44aa5)`, `(8a9b0,8a9c1)`. This includes decoded branches/calls and any other decoded instruction containing an explicit numeric target in those intervals. It does not rule out computed/indirect jumps, address arithmetic, embedded pointer tables, runtime-generated code, alternate decoding, or references from other modules. No broader absence claim is made.

Explicit entry-reference counts were respectively **4, 2, 2, 1,017, 12, 1**. All matches were `call` instructions. Nonselected entry callers include outer `26f7c/28383/3cc37`, duplicate `c78e98`, factory `c78eb4`, and Project `1c198/1c816/44cbd/44d5d/44dfd/44e9d/44f3d` (only the first eight Project callers were retained). Thus even the non-RTTI hooks have a wider static footprint than their callback caller filters.

The six expected direct callers were independently read as bytes and resolved using signed `rel32 + next RIP`:

| Call RVA | Exact bytes | Target RVA | Continuation |
|---|---|---|---|
| `1c8f` | `e89cad0100` | `1ca30` | `1c94` |
| `1cf0c` | `e8ffbcc500` | `c78c10` | `1cf11` |
| `1d085` | `e806c3c500` | `c79390` | `1d08a` |
| `1d0b8` | `e83f284501` | `146f8fc` | `1d0bd` |
| `1d0d3` | `e8b8790200` | `44a90` | `1d0d8` |
| `1d0f6` | `e8b5d80600` | `8a9b0` | `1d0fb` |

The selected continuation is `1c94`, as the current runner uses. All six entry RVAs coincide with original PE `RUNTIME_FUNCTION` begins, and no second runtime-function begin lies inside these windows. Containing regions and their original UNWIND_INFO headers are:

| Begin..end | Unwind RVA | Version / flags | Original prologue bytes | Unwind slots |
|---|---|---|---:|---:|
| `1ca30..1d435` | `3113ce8` | 1 / 2 | 29 | 2 |
| `c78c10..c78e72` | `33029a4` | 1 / 2 | 26 | 2 |
| `c79390..c79460` | `3302a6c` | 1 / 0 | 14 | 1 |
| `146f8fc..146fa99` | `347bfe4` | 1 / 1 | 23 | 10 |
| `44a90..44bca` | `311d878` | 1 / 2 | 21 | 2 |
| `8a9b0..8af96` | `312e590` | 1 / 2 | 29 | 2 |

These are original unwind regions, not proof of complete logical function boundaries or Frida trampoline unwind support. In particular moving SUB/PUSH instructions outside these ranges does not transfer the original unwind metadata to the generated code.

## Exact remaining runtime patchability limit

The established result is relocation simplicity **conditional on an actual redirect requiring at most 14 entry bytes**, with the whole-instruction spans above. Static bytes alone cannot determine which redirect the pinned backend will emit in a particular address space, where its trampoline/deflector will be allocated, the actual displaced instruction count, its continuation target, native listener effects, or Windows unwind registration. No emitted patch or trampoline was available in this read-only lane. An attempted read of the installed Frida package metadata was denied by filesystem permissions; no escalation or target launch followed, and the report does not claim that local package/native artifact was independently verified.

Before clearing the hook-byte/native-readiness hold, retain an owned harmless **native** qualification for the pinned Frida implementation and executable agent identity that records actual pre/post entry bytes, redirect extent, complete relocated instructions, trampoline/deflector bytes and addresses, continuation target, preserved RSP/register behavior, and exception/unwind behavior across displaced prologues. Cover the shared RTTI push sequence and the Project 21-byte case, plus all six entry shapes. Mock callback fixtures do not supply that evidence. A harmless native pass still does not establish original-specific neutrality: an eventual separately reviewed fixed-child run must retain the actual live six-site patch evidence and required path/exception parity. Any actual emitted displacement beyond the qualified span reopens this review; in particular a 16-byte factory redirect requires prefix qualification beyond the current runner's 14 bytes.

The decoded-image negative search cannot eliminate unknown indirect interior entries. Callback return-address/RSP filtering does not repair an unsafe native patch and cannot exclude effects on unrelated shared RTTI users. For those reasons the final verdict remains **HOLD for original execution at every site**, while the exact 5/14-byte static window checks PASS. No guard or runner edit is made by this report.
