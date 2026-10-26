# CP201 TLS JNE edge static audit and smallest successor

## Decision

There is no nonempty common executed prefix after the unresolved CP200 JNE. The two first instructions access different locations and write different registers. Static evidence supports a paired, inert description of their guarded effects. It does not support executing either instruction, merging the paths at the epilogue, or issuing a concrete branch decision.

The smallest useful implementation proposal is one immutable successor-edge semantics record attached to CP200's same issued JNE/CMP/read. Keep the current unresolved boundary at RVA `0x2961b`; describe each first instruction without claiming a memory read occurred. This is a schema proposal for root review after CP200 freeze, not source release or execution approval.

## Qualification and method

Used the reverse-engineer-anything skill's static evidence discipline. Read CP200 TLS initialization/order and schema reports, CP196 nullpath and CP198 prefix reports, and the source qualifications `resume163-connector-qobject-static.json` and `wave2-doe-task-public-qobject-ownership-contract.json`. Their bounded/unwind captures are static observations, not complete logical-function or native execution witnesses. Historical native-instruction qualifications do not authenticate a later adapter occurrence by themselves.

New read-only DUMPBIN inspection used copied original `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Tool: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`; printed version `14.44.35228.0`. Command: `/disasm:bytes /range:0x67029600,0x670296d0 evidence/original-binaries/QtCore4_64.dll`. Interpretation begins at the aligned retained initializer entry. The trailing partial byte at `0x296d0` is excluded. An in-memory Python PE section mapper rechecked original-file raw bytes and hashes; it wrote no evidence sidecars.

No REA database, open_binary, native process, GUI control, build or tests were used. Source was read only to check existing stack provenance and ownership, not as an oracle for original bytes. This report alone was written.

## Both edges, in aligned instruction order

All addresses below are RVAs. Let `x` be CP199's same issued 32-bit read at `0x29614`, `ZF=(x==0xffffffff)`, and S the existing symbolic private-initializer-entry stack origin. CP200 has RSP=`S-0xa8`; B is its already qualified task-RBX token. These are model expressions, not observed original register values.

```text
2961b  0f 85 9f 00 00 00     jne 296c0

; fallthrough, guarded by ZF set / x==0xffffffff
29621  8b 05 01 a7 2c 00     mov eax,dword [QtCore+2f3d28]
29627  48 8d 1d f2 a6 2c 00  lea rbx,[QtCore+2f3d20]
2962e  a8 01                 test al,1
29630  75 20                 jne 29652

; taken, guarded by ZF clear / x!=0xffffffff
296c0  48 8b 5c 24 40        mov rbx,qword [rsp+40]
296c5  48 83 c4 30           add rsp,30h
296c9  5f                    pop rdi
296ca  c3                    ret
```

The fallthrough first instruction requires a new 32-bit read of process/module storage at `+0x2f3d28`. Call that potential read y; it is distinct from x at `+0x2f08e8`. If this edge executes, EAX=y and RAX=zero_extend_64(y); MOV preserves arithmetic flags and RSP. The following LEA computes a module-relative address, without reading storage at `+0x2f3d20`; TEST then replaces arithmetic flags based on `low8(y)&1` before another conditional branch. Initial file contents, apparent once-flag purpose, or prior helper calls cannot supply y for this occurrence. This audit assigns no synchronization-type or concurrency guarantees to that storage.

The taken first instruction reads `[S-0x68]`, exactly the generation-3 spill written by `0x2960f`, bytes `48 89 5c 24 40`. Under the unchanged qualified prologue/stack history, its conditional result is RBX=B. It preserves flags, RSP and both live return slots. The current source validator already authenticates slot 7 as generation 3, producer `0x2960f`, width 64, offset `-0x68`, TaskRbx value with matching caller/call occurrence. This supports a conditional restore expression; it does not authorize marking the restore executed when the edge is unresolved.

The taken ADD at `0x296c5` would change flags and RSP to `S-0x78`; POP would read the saved RDI slot and advance to `S-0x70`; RET would consume the generation-3 return. None is common prefix work. Fallthrough has intervening state reads/writes, calls, atomics and another TLS allocation producer before its visible normal paths join `0x296c0`. A shared later epilogue is not permission to skip those effects. This bounded audit makes no complete unwind/reentry/exception-path closure claim.

## Smallest useful next cut

Owner: the existing TaskFactoryRuntime/QObjectPrefix source lane should prepare an inert edge-effect record only after CP200 is frozen and independently reviewed. Link exact object/control-block ownership to its issued CP200 JNE and CP199 CMP/read; retain the existing WidgetRuntimeState module identity and thread execution context. Do not create a factory-local TLS/once-global owner.

Record two alternatives in CP200's existing order:

- ZF set: instruction `0x29621`, six exact bytes, width-32 module-memory operand `+0x2f3d28`, prospective EAX/RAX overwrite with 32-to-64 zero extension, flags/RSP unchanged, prospective next boundary `0x29627`.
- ZF clear: instruction `0x296c0`, five exact bytes, width-64 stack operand `RSP+0x40 = S-0x68`, reference to CP198's exact issued generation-3 spill, prospective RBX=B, flags/RSP unchanged, prospective next boundary `0x296c5`.

These are semantic alternatives, not issued read expressions, completed effects, selected cursors or witnesses. Keep current registers, flags, stack, globals, read counts, live returns and completion state unchanged; retain the unresolved cursor at `0x2961b`. Do not mint a dummy y occurrence merely to populate a record. A schema can name an unevaluated memory operand before any read issuer exists. If the frame contract cannot make that distinction explicit, retain CP200 unchanged and use this report as the successor contract.

To actually execute conditional branches in parallel symbolically would require a separate guarded-state model, including guard-scoped reads, stack/register liveness, branch-local flags and joins. That is larger than this cut and is not approved by this report. An ITE merged into one live register state is insufficient because the unresolved paths have different instruction positions and read histories.

## Exact missing witness for concrete progress

For a real choice of successor, preserve CP200's missing witness: the value of CP199's same read at loaded `Bqt+0x29614`, tied to instruction identity, original hash, module/base/process lifetime, WidgetRuntime and existing thread, Session capability, exact Report/private/native identities, generation-2/3 call occurrences, and that read occurrence. Do not perform a later memory read and pretend it supplies the flags already consumed by JNE. A qualified prior successful TlsAlloc store plus proven absence of intervening writes is an alternative, not established here.

If that witness selects fallthrough, advancing its first MOV additionally needs a distinct same-lifetime module-storage read issuer/unknown expression for y at `Bqt+0x2f3d28`, bound to the selected edge occurrence. A concrete y is needed to decide its later TEST/JNE; it is not required merely to retain the MOV result as an explicitly unknown symbolic read. If the witness selects taken, existing issued spill provenance is enough for the first MOV's structural restore, subject to the normal lifetime/stack-history validations. Further epilogue instructions require their own flags/stack/return modeling.

## Test and release gate

No tests were changed or run in this read-only lane. Before any proposed schema is released, its owner should verify exact bytes, widths, address expressions and successor lengths; mandatory association with the exact issued JNE/CMP/read; both alternatives present with no selected edge; explicit no-read/no-effect semantics; unchanged RAX/RBX/flags/RSP/slots/returns/shared state and native-publication state; stable terminal reentry without new occurrences or reads; direct/resumed equality; failure-atomic record preparation; and refusal of absent, copied, foreign-owner, stale, rewound, expired or released predecessor/grant state. Test substitutions may check instruction arithmetic but cannot become production branch witnesses. Root retains source-stable focused-test/full-launcher/freeze ownership.

GUI release remains governed by the existing unsupported-release assessment and CP200 capability report; no GUI session was acquired here. No claim that app control was released is made.

## Raw-byte receipts

Ranges are half-open; SHA-256 covers original mapped raw file bytes, not formatted DUMPBIN text.

| RVA range | SHA-256 |
|---|---|
| `29600..296cb` aligned initializer selection | `1472c224b2d4209935fcedc1136de1796679022a624fec472c16abaaffe7993c` |
| `2961b..29621` JNE | `fcf8c4c0bf2c94487c8ff3657651dda575de9b367b35a718dc3db25c940d92c5` |
| `29621..29630` fallthrough first MOV/LEA/TEST | `71ab22a456f7cc81da6a092b1f1c9c08721495a77aa44376c883f0739e798289` |
| `296c0..296cb` taken epilogue | `982f73888551fef02d4bbf732d171fb51de3162a5fa4c1061cbd9465b119cb21` |
| `2960f..29614` prior RBX spill | `17d9a84e9c2fe45922b7e86df970c2b7333291d61a92ee1a987e74c14b4abf8e` |

No source/build/test, original binary, mutable REA, installed application, parent DMO or Claude tree was changed.
