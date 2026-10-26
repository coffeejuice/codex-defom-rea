# CP200: TLS initialization order at first Session Report

## Decision

**No unconditional initialized-index decision is established.** The original static code identifies earlier application paths into the same TLS initializer, but an earlier call is not a successful allocation receipt. At the exact first Session Report CMP/JNE, both `x != 0xffffffff` and `x == 0xffffffff` remain unresolved alternatives of CP199's unknown read. This does not prove that two complete real application executions reaching Report have been witnessed, or that both are feasible under every intended runtime profile.

Keep CP199's concrete execution stop before JNE at QtCore RVA `0x2961b`. Do not infer the value from PE initial -1, the first Report ordinal, successful GUI startup, common Qt behavior, or the existence of preceding QObject constructions. This report concerns the current CP198-frozen/CP199-under-validation boundary supplied by root; it does not issue a CP199 freeze or test receipt.

## Source qualification and method

Read the retained CP163 original-image/prototype qualifications, `wave2-doe-task-public-qobject-ownership-contract.json`, `wave3-doe-report-qobject-prerequisite-contract.json`, the retained TLS initializer/current-thread listings, CP106/107 startup reports, CP196 null-caller qualification and CP199 owner/CMP plan/source review. Their static ranges establish selected instructions and source dependencies, not original execution or exhaustive call/write coverage. Historical earlier constructor guards have been superseded by the frozen prefixes; their missing runtime witness is still missing.

New read-only checks used the copied original `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. DUMPBIN path was `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Commands were bounded `/disasm:bytes /range:START,END` views, `/headers`, selected `/exports` and `/imports`, and one in-memory full disassembly filtered for direct calls to the selected TLS/current-thread/application helpers. A small read-only Python PE parser checked table qwords, raw-byte hashes and selected IAT names. No REA database or runtime was opened. The new evidence is retained below in this sole report rather than separate generated artifacts.

All following addresses are QtCore RVAs unless prefixed with an executable VA. Bounded ranges do not certify complete logical functions. Preliminary views that started inside unrelated instructions are excluded from the reasoning; conclusions use the aligned entries and selected instructions below.

## Module startup reaches cleanup registration, not a success proof

PE `AddressOfEntryPoint=0x179158`. On the selected process-attach route its wrapper jumps at `0x179190` to `0x179020`. The latter calls CRT dispatcher `0x178dc4` at `0x17908b` before its later user-entry helper. The dispatcher's reason-1 route calls `_initterm_e` through thunk `0x179448` with `[0x1a8910,0x1a8920)`, then `_initterm` through thunk `0x179442` with `[0x1a88c8,0x1a8908)` at `0x178fac`, before setting CRT state to 2.

The second range contains these exact qwords in order:

```text
1a88c8 0000000000000000
1a88d0 00000000671a5da0
1a88d8 00000000671a5dc0
1a88e0 00000000671a5de0
1a88e8 00000000671a5df0
1a88f0 00000000671a5e10
1a88f8 00000000671a5e30
1a8900 00000000671a5e60
```

The TLS-related slot `0x1a88e0` points to:

```text
1a5de0 48 8d 0d 19 04 00 00  lea rcx,[QtCore+1a6200]
1a5de7 e9 68 2d fd ff        jmp QtCore+178b54
```

`0x178b54` calls `0x178aa4`. That wrapper either calls imported `_onexit` at IAT `0x1a84c8` or encodes/appends the supplied callback to the module exit table. The CRT detach route walks and calls those decoded entries at `0x178eb9`. Thus this selected initializer registers the reset callback; it does **not** call the TLS allocation producer. Registration return values do not prove eventual callback delivery. Other startup work and indirect calls were not exhaustively closed, so this is not a global absence claim about every possible startup allocation.

Callback `0x1a6200` loads dword `[0x2f08e8]`, compares with -1, skips if equal, otherwise calls imported `TlsFree` at `0x1a620f` and stores -1 at `0x1a6215`. Its placement supports a module-exit cleanup role, but it is not a same-lifetime no-reset proof for the selected Report. The initializer's other registered callback `0x1a61f0` concerns synchronization storage `0x2f3d20`; do not confuse it with this TLS reset.

Verified IAT identities: `0x1a8378=KERNEL32!TlsAlloc`, `0x1a83a8=KERNEL32!TlsFree`, `0x1a84c8=MSVCR90!_onexit`, `0x1a84e8=MSVCR90!_initterm`, `0x1a84f0=MSVCR90!_initterm_e`.

## Earlier application call order still leaves the external result unknown

CP106/107 qualify the normal executable startup shell: native main `0x140001410` constructs `SfApplication` at `0x14000147e`; its constructor calls imported QApplication at `0x1400021f1`, resolving to QtGui `0x65028aa0`. That constructor calls the protected QCoreApplication import at `0x65028b04`. The QtCore export table resolves the exact decorated protected constructor to RVA `0x11d3c0`.

New aligned QtCore decoding shows `0x11d3c0` calls protected QObject at `0x11d3d9`, then installs its QCore address point and calls `0x11d010` at `0x11d3ec`. That helper publishes its receiver to global `0x2f4220`, then calls `0x27670` at `0x11d021`. The latter checks qword `0x2f3c98`: if nonzero it returns; if zero it calls `0x26f90`, stores the returned RAX to that qword at `0x27683`, and calls the TLS initializer `0x29600` at `0x2768a`. The earlier protected QObject null-parent path also has a qualified current-thread dependency. These establish real static predecessors on the normal startup shell, not a Session-issued observation that they ran or succeeded.

The selected direct-call scan also finds `0x2a05a -> 0x29600` and the Report helper's `0x2acb1 -> 0x29600`. It is not an exhaustive indirect call graph. At `0x29614` the initializer compares the process-global dword with -1 and JNE skips to `0x296c0`. On the equal route the crucial producer is:

```text
29686 ff 15 ec ec 17 00  call qword [QtCore+1a8378] ; TlsAlloc
2968c 89 05 56 72 2c 00  mov dword [QtCore+2f08e8],eax
```

There is no allocation-result test between this call/store and its normal epilogue. Therefore even a proven earlier normal return from this initializer would only prove that its unknown external result was stored, unless the earlier entry skipped because a separately qualified non--1 value already existed. It would not prove a successful result. The current-thread null-result routes call imported TlsSetValue at `0x2acfd` or `0x2ad86` and continue without testing that call's EAX result. They do not supply an evident local invariant that a later Report can use as an allocation-success receipt. This observation does not close their full failure/unwind/reentry behavior or construct a complete failing original run.

The exact first Report native caller is independently qualified as `0x140137644 -> 0x1407b89e0 -> 0x14157ead0 -> 0x141403210`; TaskBase clears EDX at `0x141403222` and calls public QObject through IAT `0x141e041c0` at `0x141403229`. That determines null parent, not TLS state. Report ordinal zero means first prototype on this factory path, not first QObject or first TLS call in the original process. Neither the retained caller prefix nor the adapter's private Session capability joins the full native startup, intervening callbacks, index writes and this exact read into a success/persistence proof.

## Exact missing witness and appropriate owner

For this JNE alone, capture the dword read at loaded `Bqt+0x29614` for this exact first Report/initializer occurrence, together with the instruction identity. Bind the receipt to original DLL hash, loaded instance/base and process/run lifetime; the common WidgetRuntime root and existing WidgetThreadState; exact Session capability, first Report/private/native-object identities; enclosing generation-2 and nested generation-3 call/return occurrences; and this read occurrence. The observed value determines `ZF=(x==0xffffffff)` and JNE's selected edge. A getter result is not required to decide this branch. A prior TlsAlloc return/store is an alternative only with proven successful value and no intervening reset/overwrite through this read.

A future concrete-value issuer belongs to the existing process/module symbol owner on `WidgetRuntimeState`, with the existing thread attached as execution context. CP199 already has a privately issued unknown symbol/read/grant there. Do not put a guessed index in the thread, factory constructor, public observation ledger, or new startup flag. Any decision receipt must authenticate the **same** issued CMP read, not perform a fresh read after CMP: the actual JNE consumes existing ZF even if memory changes afterward. Runtime module lifetime and adapter lifetime must remain distinct. Later reload/getter/QThreadData effects require separate evidence.

No such runtime witness was created here. GUI release is still unverified (`release_ready=false` from the required status tool); no GUI control was acquired. The assessment's missing supported release/status API remains a gate, and no debugger detach or application close is offered as a substitute.

## Safest structurally useful next increment

After root freezes CP199, review one **symbolic JNE control-flow record**, authenticating its current CMP/read rather than choosing an initialization profile. Bytes at `0x2961b` are `0f 85 9f 00 00 00`; next sequential address is `0x29621`, taken destination `0x296c0`. Preserve the predicate `!ZF`, equivalently `x!=0xffffffff`, and the two guarded edges tied to that same unknown read. Stop before either successor's first instruction. This needs an explicitly unresolved-successor phase/representation; a single concrete cursor pointing at the fast-path epilogue would falsely select a branch. If the current frame contract cannot represent unresolved control flow, retain the existing stop and prepare that inert schema first.

This is a review proposal, not an implementation approval or physical execution claim. It must not allocate TLS, read the equal-route once state, execute the fast-path epilogue, pop either return, call the getter, complete QObject, or publish a task. Useful tests would require: exact root/session/read/CMP identity and predicate/edge bytes; unchanged flags/registers/stack/private/shared state; direct-versus-resumed equality; stable terminal reentry without another read; failure-atomic refusal for missing/foreign/copied/stale/rewound CMP and grant state; release/expiry refusal; and explicit absence of a selected edge/concrete value. Test-only substitutions of values can check predicate arithmetic, but cannot issue a production branch witness.

## Reproducible raw-byte receipts

Each range is half-open and SHA-256 covers original mapped file bytes, not formatted disassembly. These are selected ranges, not claims of complete semantic closure.

| RVA range | SHA-256 |
|---|---|
| `179158..179195` entry | `eaf2f780bc7e98af36303ed23b4a11ce8797e2206308de739c97937587c81209` |
| `179020..179155` wrapper | `32368509d98841fcfc5c78fe2e90f083d65241c804c42e19ecb5ba1f7dc892f0` |
| `178dc4..17901d` CRT dispatch | `80f3f42f0b210a416daab1cf9ab0a78da5b148aa679f980e89a005653c0e99c5` |
| `1a88c8..1a8908` initializer table | `86c3b800d46a8dce5bad1f5c68c06966342a8d7b7283f005a0549e5bf2708de5` |
| `1a5de0..1a5dec` TLS exit registration | `8794a7d76bfd169c59757ce8385066f13b76ca20c8bc2b582f7b2dace8da64da` |
| `178aa4..178b6b` registration wrappers | `5899b562ceeb10196561c477c9e61ed4aba114066b441a22a43e09ee09118215` |
| `11d3c0..11d3fb` protected QCore ctor | `dde5cf6333931d3c7cc82ad87d84f0758f378a02afb3ab888ee0e444188c7a1b` |
| `11d010..11d026` app-init prefix | `cc11e9231f5eb487d61e0ef5391219e71b72f80403ffd6c341e02b7c759c880c` |
| `27670..27694` earlier helper | `0869ecea7c555604864c83afaf23c57ad3337bd19ad106772c6a343030a97b6a` |
| `29600..296cb` TLS initializer | `1472c224b2d4209935fcedc1136de1796679022a624fec472c16abaaffe7993c` |
| `1a6200..1a6224` TLS reset | `0d39774c5081a10bb22556d2e4cb1bbd7e922d4e9ea8f50178edf9c19c447b8e` |

Only this report was written. No source/test/build, GUI, original process/artifact, parent/DMO/Claude tree, deployment or mutable REA database was changed.
