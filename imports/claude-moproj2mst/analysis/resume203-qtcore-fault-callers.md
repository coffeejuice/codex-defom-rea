# CP203: QtCore QListData fault operand and bounded callers

The repeated original crash reports point to the first instruction of QListData::size, which reads the caller-supplied wrapper address in RCX. The copied shipped PE establishes the instruction and the application import path; it does not identify the failing caller or runtime RCX value. No repair to startup, Open, notifier, or converter source follows without a matching exception context.

## Source identity and method

Original DEF_GUI_MO_64.exe reference v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; copied `evidence/original-binaries/QtCore4_64.dll`, version 4.8.4.0 in the crash event, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Both copied-file hashes were recomputed here and match the retained crash identities. These are original shipped references, not reconstructed DEF_PRE instances.

Read-only PE32+ parsing, executable-section byte-pattern searches, exception-directory region mapping, and Microsoft DUMPBIN 14.44.35228.0 bounded disassembly were used. A brief REA `open_binary` of installed QtCore returned identity only; the parent's subsequent no-open instruction was followed by immediate `close_binary` (result null), before any function, database, xref or mutation call. All actual code analysis used copied evidence PEs and DUMPBIN. This report is the sole filesystem write. No original process was run; no GUI/control session, source change, build, or deployment occurred.

## Exact fault instruction

The export `?size@QListData@@QEBAHXZ` maps to RVA `0x32c0`, preferred VA `0x670032c0`. The exact complete leaf bytes, also retained by CP123, are:

```text
RVA     Bytes       Instruction
0032c0  48 8b 01    mov rax,qword ptr [rcx]
0032c3  8b 48 0c    mov ecx,dword ptr [rax+0ch]
0032c6  2b 48 08    sub ecx,dword ptr [rax+8]
0032c9  8b c1       mov eax,ecx
0032cb  c3          ret
```

The x64 member-function argument in RCX must name a readable eight-byte QListData wrapper. That first load obtains its backing-data pointer; the following loads subtract begin from end. A data-read access violation precisely at RVA 32c0 therefore concerns the wrapper address RCX, before the backing-data pointer is dereferenced. A null backing-data pointer with a readable wrapper would normally fault at RVA 32c3, not 32c0. A null RCX, stale/unmapped wrapper, bad owner-plus-offset, or invalid stack/list address are distinct candidate mechanisms; none is established here. WER Event 1000/1001 lacks the exception access-type and accessed-address parameters, registers, and stack, so even the data-read interpretation must remain conditional rather than excluding an execution-protection/mapping anomaly. The DLL fault address alone does not establish a QtCore defect.

The function has no writes, allocation, refcount operation, or detach. `QListData::isEmpty` begins at32d0. The unrelated older RVA12a0e4 crash location is not used.

## Proven application linkage and direct call census

The app PE import descriptor binds QtCore's exact decorated size symbol to IAT preferred VA `0x141e04210` (RVA `0x1e04210`). This is Windows PE IAT linkage, not an ELF PLT. Application wrapper `0x140004ae0..0x140004af9` preserves incoming RCX, calls through that slot at `0x140004aee`, and returns normally via continuation `0x140004af4`. Its bytes hash to the CP123 frozen range hash `638c97280f018b2497d6a1c0d2973edbd25ef35038945a2258b0cd3de6f9e9e7`.

All file-backed executable sections of the copied app were searched for RIP-relative `FF15` calls and `FF25` jumps resolving exactly to this slot. There are 37 call candidates and zero matching jump candidates. Every one of the 37 was then confirmed as a `call qword ptr [141e04210]` instruction by disassembling its containing PE exception-directory range from its entry; 28 ranges are listed below. Ranges are end-exclusive RVAs, and are unwind regions, not a claim of complete logical-function boundaries. Import resolution is static; the actual loaded IAT value at the crash is unknown.

| Containing unwind range (RVA) | Verified call sites (RVA) |
|---|---|
| `0x4ae0..0x4af9` | `0x4aee` |
| `0x4fbe0..0x4fcc3` | `0x4fc07`, `0x4fc35` |
| `0x88be0..0x88cc4` | `0x88c07`, `0x88c35` |
| `0xada30..0xadb19` | `0xada57`, `0xada85` |
| `0xd0ff0..0xd10bb` | `0xd1004`, `0xd1011` |
| `0x15d900..0x15d95f` | `0x15d919` |
| `0x188520..0x18857f` | `0x188539` |
| `0x1f4660..0x1f46bf` | `0x1f4679` |
| `0x3766f0..0x37674f` | `0x376709` |
| `0x376750..0x376838` | `0x376777`, `0x3767a5` |
| `0x410110..0x41016f` | `0x410129` |
| `0x410170..0x410221` | `0x410197` |
| `0x550df0..0x550ed9` | `0x550e17`, `0x550e45` |
| `0x6192d0..0x61932f` | `0x6192e9` |
| `0x619330..0x61938f` | `0x619349` |
| `0x6194b0..0x61950f` | `0x6194c9` |
| `0x619670..0x6196cf` | `0x619689` |
| `0x8188f0..0x81894f` | `0x818909` |
| `0x91fc40..0x91fc9f` | `0x91fc59` |
| `0x91fdc0..0x91fe1f` | `0x91fdd9` |
| `0xd4b8c0..0xd4b9e0` | `0xd4b8ef`, `0xd4b924` |
| `0xf34bc0..0xf34c1f` | `0xf34bd9` |
| `0x11c3390..0x11c33ef` | `0x11c33a9` |
| `0x1232c00..0x1232c5f` | `0x1232c19` |
| `0x1272050..0x1272138` | `0x1272077`, `0x12720a5` |
| `0x13c5290..0x13c536b` | `0x13c52a4`, `0x13c52b1` |
| `0x1582ab0..0x1582b33` | `0x1582ac9` |
| `0x16bf4f0..0x16bf54f` | `0x16bf509` |

The forwarding wrapper is not the only application route. For example, region `d0ff0..d10bb` calls size twice on incoming RCX/RDX wrappers, compares the counts, and conditionally continues; region `3766f0..37674f` checks an index against size before further list work. The latter has a negative-index guard but no wrapper-pointer guard before size. These observed patterns demonstrate why assigning all size faults to one high-level model is unsound.

The copied QtCore executable sections contain zero `E8 rel32` byte patterns resolving directly to preferred VA670032c0. This excludes that particular direct-call encoding in those scanned bytes; it does not exclude register calls, aliases, runtime-generated calls, other modules, or external callers. QtGui's callers were not enumerated here. No complete cross-module caller graph is claimed.

## Startup/Open candidates and limits

A bounded direct-call search of native main RVA1410..1e4f, selected GUI Open RVA271b0..28515, constructor/setup RVA130830..130ad7, prepared-visibility vicinity RVA15400..15500, and slot vicinity RVA3a990..3ab20 found no direct `E8` calls into the 28 size-import-calling region entries. This only constrains one call edge; helper, virtual, callback, QtGui and transitive routes remain possible. These bounds are selectors from retained reports, not newly proven whole logical Open boundaries.

Three concrete static routes remain useful when a crash stack becomes available:

1. General wrapper use is widespread. A raw all-section rel32 search found 1952 candidate calls to140004ae0; that count is not a validated instruction census. Restricting to early app RVAs below30000 gives candidates at3b68 (region3b20..3c4e),3ca5 (3c60..3cf6),16418 (15bf0..17261),267c3 (25b60..271a1), and2aca4 (2ab20..2b1e6). CP156's retained decompile separately identifies140025b60 as a main-window Open slot calling File > Open dispatcher14001ca30. Thus267c3 is an Open-related caller candidate, not evidence that CLI -m open used it or that it crashed. The raw candidates were not independently disassembled here and must not be treated as validated call sites.
2. Constructor notification has a proved transitive size path: retained same-address DUMPBIN instructions show140130ad1 calls notifier1402cf3c0. Its unsuppressed branch copies a receiver-list snapshot via1402ef070, then at1402cf475 forms RCX=`rsp+70h` and at1402cf47a calls wrapper140004ae0 (continuation1402cf47f). This is a local stack wrapper, not the original notifier-map address. A direct fault on that particular stack wrapper would require explaining why the stack-derived RCX became unreadable; a corrupt snapshot backing pointer alone would instead affect a later instruction in size. The baseline and instrumented traces do not establish this notifier invocation or stack at the crash. CP203's Boolean constructor same-occurrence prerequisites remain unresolved.
3. OPI cleanup has another proved route: CP123 and re-read DUMPBIN140061220..14006123c show RCX=`incoming OPI+50h`, then call wrapper at140061232. CP124 qualifies the analogous OPI+60 position-count route. These identify possible owner-plus-offset inputs, but neither crash receipt proves cleanup was active. A project-open command or terminating process is not evidence of accepted Close/cleanup.

The incomplete instrumented trace observed prepared visibility and slot entry, then dock events; it does not furnish a complete startup/Open stack. The uninstrumented baseline establishes that the same module/RVA fault occurs without Frida under its recorded launch conditions. Together they exclude the claim that Frida is necessary for this observed fault, while leaving the actual caller, register value, owner lifetime, and environmental cause undecided.

## Required discriminator

A retained matching crash exception context should first identify access type, accessed address and RCX, then resolve the return address at RSP against loaded module bases. Return140004af4 would select the common wrapper; its caller return at the next valid unwound frame would distinguish an OPI accessor, notifier snapshot, Open helper or another path. An immediate app return from one of the 37 sites selects that direct-import route; a QtGui return selects a separately unenumerated module. Validate unwind metadata and stack memory before calling any raw stack value a return address. Collecting such evidence is a separate runtime action and was not performed here.

Sources read: `resume202-qtcore-crash-static.md`, `resume202-baseline-crash-evidence.md`, `resume202-original-probe-attempt2-trace.md`, `resume123-opi-object-list-prefix.md`, `resume124-opi-position-list-prefix.md`, `resume59-close-next.md`, `resume156-cogging-cwd-event-next.md`, `resume203-boolean-open-constructor-static.md`; original copied PE bytes and exact import/export tables. No speculative route here authorizes a source behavior change.
