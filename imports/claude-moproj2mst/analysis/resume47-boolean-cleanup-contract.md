# Boolean queue cleanup and constructor dependency contract

The receiver cleanup routine removes queued events before deleting them. It must not be replaced by the single-event destructor helper, and it does not itself deliver a DeferredDelete event or delete its receiver. The existing constructor `PostPending` guard remains necessary.

This is a static analysis receipt only. No source edits, build, app control, original process or mutable REA session was used. Original DLL files were read only. New bounded offline DUMPBIN excerpts are retained inline below, including binary/code hashes and commands. Original instructions and exported symbols establish the contracts; C++ tests have not established runtime parity.

## Receiver destruction reaches the real cleanup function

The export table binds `QObjectPrivate::~QObjectPrivate` to `67131240`, `QObject::deleteLater` to `67129b10`, and `QCoreApplication::removePostedEvents(QObject*,int)` to `6711b500`. These are distinct responsibilities.

At `6713126a..6713128b`, QObjectPrivate destruction first conditionally invokes its thread dispatcher virtual slot `+88`: private flag bit1, nonnull private+40 threadData, and nonnull threadData+20 dispatcher admit this branch. Its dynamic target is not closed by this report. At `67131291..6713129d`, nonzero private+24 posted count causes a call to `6711b500(publicQObject=private+8, type=0)`. Only afterward does `671312a2..671312ab` release its nonnull threadData through `67028610`. The private-owned connection/property/list releases follow. Clearing a C++ weak owner is not a substitute for this ordered destructor path.

`QObject::~QObject` retained at `analysis/wave3-boolean-timer-qobject-destructor.asm` eventually invokes its actual private object's deleting virtual destructor at `6712fd9a..6712fda9`. That virtual target must remain qualified for the actual dynamic private type; the base private body above establishes a dependency, not universal derived-type completion.

`QObject::deleteLater` separately allocates a 0x18 QEvent, constructs raw type `0x34` at `67129b36..67129b3e`, then passes the receiver and event to default-priority postEvent at `67129b47..67129b52`. Its delivery and receiver deletion require additional sendPostedEvents/QObject event-dispatch evidence. The collected-event deletion vector in `6711b500` is also deferred until unlocking, but that is not DeferredDelete delivery. The special raw `0x43` cleanup in this routine is a different event type.

## Exact 6711b500 contract

The record layout is receiver pointer at +0, event pointer at +8, priority/padding at +10. The routine maps requested type 0x46 to 0x43. A nonnull receiver selects its own private+40 threadData; a null receiver selects current threadData. It locks threadData+48. A nonnull receiver with zero private+24 posted count returns after unlocking.

The scan length is the initial active QList end-minus-begin. Each record is matched by receiver (or all receivers for null argument), nonnull event, and requested type (or all types for zero). For a match, `6711b684..6711b68b` decrements that record receiver's private+24 count. Raw0x43 additionally calls `6711a500(receiverPrivate+68)` before clearing the event flag. The helper replaces/releases a shared container; its subordinate allocation/release branches remain qualified by the inline assembly, rather than being reduced to an invented counter reset.

`6711b6a9..6711b6b2` clears event posted bit0. The event pointer is appended to a local deletion vector, initially capacity256 on the stack; reaching capacity calls growth helper67118950. The record event slot is then set null at6711b6ec. No event deleting destructor is invoked in this matching scan.

| threadData+38 | Record handling during scan | After scan |
| --- | --- | --- |
| Nonzero | Matching event slots become tombstones; unmatched records remain in place. | Skip compaction; original active bounds/order remain. |
| Zero | Each unmatched record is swapped into the next survivor position when its scan index differs. | Erase the trailing non-survivor record range, then unlock. |

The semantic name of threadData+38 is not established here; this contract uses its actual zero/nonzero predicate. `6711a5e0` swaps record pointers in the QList, detaching when needed. It does not clone receivers or events. Thus survivor record identities and relative order are preserved. `6711ab90` frees record allocations in its erase range, then calls QListData range removal67048ed0. Those frees are distinct from event destruction. Shared backing may detach through67118ec0; its complete allocation/failure behavior is outside this bounded contract.

For a unique backing, the selected trailing erase has no surviving suffix. `67048ed0` selects the end-shrink branch and reduces end by removed count (an empty range changes nothing); begin remains unchanged for the nonempty-survivor fixture below. Generic middle-range removal can choose another physical relocation and must not be inferred from this tail case.

Only after compaction and queue unlock at `6711b7ba..6711b816` does the routine iterate collected events at6711b830..6711b84f and invoke each actual virtual deleting destructor with flag1, in original match order. A destructor callback therefore sees the completed count/flag/tombstone/compaction mutations and an unlocked queue. Source-required dynamic event destructor effects, callback reentry, allocation failures, invalid inputs and populated raw43 container behavior remain dependencies.

By comparison, `67119970` takes one QEvent pointer, searches the current thread's queue, deletes the first matched event while in its own locked path, tombstones only that record's event slot, and returns. It neither performs receiver-wide scanning nor this deferred deletion-vector/compaction sequence. The earlier contrary receiver interpretation is superseded by `analysis/resume46-boolean-events.json`.

## Comparison with current widget_events code

`WidgetEventState` represents synchronous stack events only. Its `destroyStackEvent()` rejects a posted or active-dispatch event; these objects cannot be transferred to the native queue. `WidgetPostEventQueue` intentionally reports unknown population. `WidgetThreadState` has no represented ordered native records, receiver posted counts, cleanup predicate+38, dispatcher owner or heap-event deleting-destructor contract. `WidgetConstructorTail::resume()` retains its exact stack Create event at PostPending. Existing native-owner invalidation is not receiver-wide cleanup and must not be relabelled as it.

No production cleanup step is justified by adding a fabricated empty queue or by calling destroyStackEvent on heap events. PostPending remains guarded.

## Exact prerequisites for advancing PostPending

1. Bind the same runtime/thread/receiver component and the actual constructor frame. Retain the stack Create through the entire pending call; no replacement event or repeated count increment.
2. Model a separate heap raw0x4a event allocation and its actual deleting-destructor class, separate record allocation, queue ownership transfer and alias identity. Neither stack-event factories nor a second per-editor queue satisfy this prerequisite.
3. Bind the source-selected receiver affinity and the locked threadData owner. Core11ae90 locks private+40's queue, rereads affinity and can unlock/reload/relock. Its locked RDI atCore+11afc6 is authoritative.
4. Retain the complete ordered existing queue, backing/refcount/begin/end, each record/event/receiver identity, incoming priority and actual receiver posted count. Startup004's observed19->20 unique equal-priority append is a qualified fixture, not an empty queue or universal initialization constant. Nonzero posted count may invoke application compression.
5. Execute append before event posted-bit, receiver-count and canWait mutations. Preserve native ordering: insert; mark posted; increment count; set threadData+50 false; unlock; resolve dispatcher from the same threadData+20.
6. Qualify the dispatcher slot+98 route and private HWND+98/serial+a8/postedWake+b4. Admit exact GUI+5e0ca0 thunk only with verified loaded Core+141b70 target. Serial increments before null-HWND/CAS/PostMessage decisions. Unknown branch inputs remain guarded; editor003 values cannot seed startup.
7. After post/wake returns, perform the constructor private+c8 store6506e85d, then stack Create destruction6506e869. Keep later QFrame/QSplitter/style dependencies guarded independently.
8. Before claiming normal lifecycle Close, add source-ordered QObjectPrivate cleanup: actual dynamic private/event destructors, +24 posted count, +38 compaction predicate, raw43 private+68 dependency where reachable, callback reentry and threadData release after cleanup. This is a lifecycle acceptance prerequisite, not permission to hold back already source-closed posting primitives indefinitely.

The minimal root passive startup capture is already specified in `analysis/resume46-boolean-startup-next.json`: retain locked queue-owner snapshots and correct the dispatcher recognizer in a new immutable script revision. No new GUI or event-pumping experiment is required by this report.

## One focused deterministic queue test boundary

Proposed test: **receiver cleanup compacts before unlocked deleting destructors**. This is a future unit contract test, not an executed original comparison or a substitute for fresh lifecycle acceptance.

Use one qualified thread/queue with unique backing, begin5, end9, threadData+38=0, four separately owned plain raw0x4a heap events and distinct native record identities. All posted flags are true; all priorities0. Active records in order are `(A,E1)`, `(B,E2)`, `(A,E3)`, `(C,E4)`. A's posted count2; B and C each1. Invoke receiver-wide cleanup `(A,0)`. This avoids raw43 payload handling, allocation growth and generic middle-range deletion. Heap ownership/deleting-class and shared queue observation APIs must exist before implementing the test.

Expected trace from the source:

- Scan E1 then E3 as matches, decrement A2->1->0, clear both posted flags and collect deletion order `[E1,E3]`.
- Move the same B and C record pointers into survivor order `[B,C]` through swaps. Free the two trailing matched record allocations during erase; no event deletion yet. Unique backing begin remains5 and end becomes7.
- Unlock, then call deleting destructors E1 and E3 exactly once in that order. A destructor observer must see Acount0, both matched flags clear, completed survivor order/bounds, unchanged B/C posted counts/flags, and lock-not-held.
- E2/E4 and receiver A remain alive; receiver-wide cleanup deletes queued event objects, not the receiver. A later parent/private destructor stage owns receiver teardown.

This boundary distinguishes wrong immediate destruction under lock, repeated first-match removal, event cloning, survivor reordering and missing compaction. Assert semantic record/event identities and ordered effects, not copied snapshot-descriptor aliases. Keep the nonzero+38 and shared-backing branches guarded until separate tests/evidence admit them.

## Evidence and validation

UTC: 2026-10-24T17:47:54.971863+00:00. Binary `evidence/original-binaries/QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. DUMPBIN SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Export selection:

```text
511  1FE 00131240 ??1QObjectPrivate@@UEAA@XZ
2010  7D9 00129B10 ?deleteLater@QObject@@QEAAXXZ
3935  F5E 00119970 ?removePostedEvent@QCoreApplicationPrivate@@SAXPEAVQEvent@@@Z
3936  F5F 0011C010 ?removePostedEvents@QCoreApplication@@SAXPEAVQObject@@@Z
3937  F60 0011B500 ?removePostedEvents@QCoreApplication@@SAXPEAVQObject@@H@Z
```

| Retained input | SHA-256 |
| --- | --- |
| `analysis/wave3-closure-next-6711b500.asm` | `f6ede102bde09516dfcf7c8b4b73363c298b458c6c0efaabacd5c5df55dd9461` |
| `analysis/wave3-closure-next-67119970.asm` | `ee9cbb5fe13b65d2ad27b1ae3f739b1189e11a10de1e12497589ec0b1986444d` |
| `analysis/wave3-boolean-timer-qobject-destructor.asm` | `f3478033c77b2c1d01c8b374c78f0d9e29ec9365095ac247eb760a4ce3b64a52` |
| `analysis/resume46-boolean-events.json` | `bbf9d9b0064e8c616633cadee6ecdd618fe861c4f138d3dca670100874e78147` |
| `analysis/resume46-boolean-startup-next.json` | `5988ae13a0308db9b033a8fd59448ba455444ed8828c96d2cd8e3d47d4090eb7` |
| `source/apps/moproj2mst/include/m2m/engine/widget_events.hpp` | `ee3dc13813813f316a7389ffd2438d1643708c3d560afb383e283e7606c87b2b` |
| `source/apps/moproj2mst/src/engine/widget_events.cpp` | `9df3a0d055a67c5330981c37248769845a457c58f3d7190e901873cfab39c4ae` |

Six static source-binding assertions passed:

- Actual private destructor calls receiver/type cleanup.
- deleteLater constructs raw34.
- Compaction swaps record pointers.
- Range erase frees records before QListData remove.
- Stack/heap queue distinction and unknown population guard retained.
- PostPending guard retained.

No C++ test or build was run. Proposed test is not counted as passing. Token usage is unavailable; no speed/token-savings claim.

## New bounded inline static evidence

### 67131240..671313cf exclusive

unwind interval; code SHA-256 `11c366a18fd9e4f96751d93e73185a64bf90047881575eb6acc167409cc3db85`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x67131240,0x671313ce evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067131240: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067131245: 57                 push        rdi
  0000000067131246: 48 83 EC 30        sub         rsp,30h
  000000006713124A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067131253: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000067131258: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006713125D: 48 8B D9           mov         rbx,rcx
  0000000067131260: 48 8D 05 A9 8A 07  lea         rax,[00000000671A9D10h]
                    00
  0000000067131267: 48 89 01           mov         qword ptr [rcx],rax
  000000006713126A: F6 41 20 02        test        byte ptr [rcx+20h],2
  000000006713126E: 74 21              je          0000000067131291
  0000000067131270: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  0000000067131274: 48 85 C9           test        rcx,rcx
  0000000067131277: 74 18              je          0000000067131291
  0000000067131279: 48 83 79 20 00     cmp         qword ptr [rcx+20h],0
  000000006713127E: 74 11              je          0000000067131291
  0000000067131280: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  0000000067131284: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067131287: 48 8B 53 08        mov         rdx,qword ptr [rbx+8]
  000000006713128B: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  0000000067131291: 83 7B 24 00        cmp         dword ptr [rbx+24h],0
  0000000067131295: 74 0B              je          00000000671312A2
  0000000067131297: 33 D2              xor         edx,edx
  0000000067131299: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006713129D: E8 5E A2 FE FF     call        000000006711B500
  00000000671312A2: 48 8B 4B 40        mov         rcx,qword ptr [rbx+40h]
  00000000671312A6: 48 85 C9           test        rcx,rcx
  00000000671312A9: 74 05              je          00000000671312B0
  00000000671312AB: E8 60 73 EF FF     call        0000000067028610
  00000000671312B0: 48 8B 43 28        mov         rax,qword ptr [rbx+28h]
  00000000671312B4: 48 85 C0           test        rax,rax
  00000000671312B7: 74 13              je          00000000671312CC
  00000000671312B9: 48 8D 48 F8        lea         rcx,[rax-8]
  00000000671312BD: 48 85 C9           test        rcx,rcx
  00000000671312C0: 74 0A              je          00000000671312CC
  00000000671312C2: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671312C5: BA 01 00 00 00     mov         edx,1
  00000000671312CA: FF 10              call        qword ptr [rax]
  00000000671312CC: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  00000000671312D0: 48 85 C9           test        rcx,rcx
  00000000671312D3: 74 36              je          000000006713130B
  00000000671312D5: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000671312D8: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000671312DC: 48 8D 74 C1 10     lea         rsi,[rcx+rax*8+10h]
  00000000671312E1: 48 8D 79 10        lea         rdi,[rcx+10h]
  00000000671312E5: 48 3B FE           cmp         rdi,rsi
  00000000671312E8: 74 21              je          000000006713130B
  00000000671312EA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  00000000671312F0: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000671312F3: 48 85 C9           test        rcx,rcx
  00000000671312F6: 74 0A              je          0000000067131302
  00000000671312F8: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671312FB: BA 01 00 00 00     mov         edx,1
  0000000067131300: FF 10              call        qword ptr [rax]
  0000000067131302: 48 83 C7 08        add         rdi,8
  0000000067131306: 48 3B FE           cmp         rdi,rsi
  0000000067131309: 75 E5              jne         00000000671312F0
  000000006713130B: 48 8B 7B 38        mov         rdi,qword ptr [rbx+38h]
  000000006713130F: 48 89 7C 24 48     mov         qword ptr [rsp+48h],rdi
  0000000067131314: 48 85 FF           test        rdi,rdi
  0000000067131317: 74 44              je          000000006713135D
  0000000067131319: 48 8D 4F 10        lea         rcx,[rdi+10h]
  000000006713131D: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067131320: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  0000000067131324: 75 09              jne         000000006713132F
  0000000067131326: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067131329: E8 A2 D2 FA FF     call        00000000670DE5D0
  000000006713132E: 90                 nop
  000000006713132F: 48 8D 4F 08        lea         rcx,[rdi+8]
  0000000067131333: E8 38 F3 ED FF     call        0000000067010670
  0000000067131338: 90                 nop
  0000000067131339: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006713133C: 48 85 C9           test        rcx,rcx
  000000006713133F: 74 13              je          0000000067131354
  0000000067131341: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067131345: 75 0D              jne         0000000067131354
  0000000067131347: BA 08 00 00 00     mov         edx,8
  000000006713134C: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006713134F: E8 3C B2 F4 FF     call        000000006707C590
  0000000067131354: 48 8B CF           mov         rcx,rdi
  0000000067131357: E8 FA 75 04 00     call        0000000067178956
  000000006713135C: 90                 nop
  000000006713135D: 48 8D 4B 70        lea         rcx,[rbx+70h]
  0000000067131361: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067131364: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  0000000067131368: 75 09              jne         0000000067131373
  000000006713136A: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006713136D: E8 7E 5C ED FF     call        0000000067006FF0
  0000000067131372: 90                 nop
  0000000067131373: 48 8D 4B 68        lea         rcx,[rbx+68h]
  0000000067131377: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006713137A: 48 85 D2           test        rdx,rdx
  000000006713137D: 74 0F              je          000000006713138E
  000000006713137F: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  0000000067131383: 75 09              jne         000000006713138E
  0000000067131385: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067131388: E8 03 41 ED FF     call        0000000067005490
  000000006713138D: 90                 nop
  000000006713138E: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  0000000067131392: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067131396: 75 0A              jne         00000000671313A2
  0000000067131398: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  000000006713139C: E8 2F ED F3 FF     call        00000000670700D0
  00000000671313A1: 90                 nop
  00000000671313A2: 48 8D 05 87 86 07  lea         rax,[00000000671A9A30h]
                    00
  00000000671313A9: 48 89 03           mov         qword ptr [rbx],rax
  00000000671313AC: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  00000000671313B0: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000671313B4: 75 09              jne         00000000671313BF
  00000000671313B6: 48 8B 4B 18        mov         rcx,qword ptr [rbx+18h]
  00000000671313BA: E8 F1 EF EE FF     call        00000000670203B0
  00000000671313BF: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000671313C4: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000671313C9: 48 83 C4 30        add         rsp,30h
  00000000671313CD: 5F                 pop         rdi
  00000000671313CE: C3                 ret

  Summary

      1A7000 .text
```

### 67129b10..67129b57 exclusive

unwind interval; code SHA-256 `a48ab202830783c9040efda804760cd197a8e83b4150f331754afacee68fda5c`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x67129b10,0x67129b56 evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067129B10: 40 53              push        rbx
  0000000067129B12: 48 83 EC 30        sub         rsp,30h
  0000000067129B16: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067129B1F: 48 8B D9           mov         rbx,rcx
  0000000067129B22: B9 18 00 00 00     mov         ecx,18h
  0000000067129B27: E8 5A EF 04 00     call        0000000067178A86
  0000000067129B2C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000067129B31: 48 85 C0           test        rax,rax
  0000000067129B34: 74 0F              je          0000000067129B45
  0000000067129B36: BA 34 00 00 00     mov         edx,34h
  0000000067129B3B: 48 8B C8           mov         rcx,rax
  0000000067129B3E: E8 0D 3A FF FF     call        000000006711D550
  0000000067129B43: EB 02              jmp         0000000067129B47
  0000000067129B45: 33 C0              xor         eax,eax
  0000000067129B47: 48 8B D0           mov         rdx,rax
  0000000067129B4A: 48 8B CB           mov         rcx,rbx
  0000000067129B4D: 48 83 C4 30        add         rsp,30h
  0000000067129B51: 5B                 pop         rbx
  0000000067129B52: E9 79 24 FF FF     jmp         000000006711BFD0

  Summary

      1A7000 .text
```

### 6711a500..6711a5d7 exclusive

unwind interval; code SHA-256 `4d6a7b7fed05d99b3a83271755ce3cac922549299a1c996b66029040bb1f608d`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x6711a500,0x6711a5d6 evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711A500: 40 57              push        rdi
  000000006711A502: 48 83 EC 30        sub         rsp,30h
  000000006711A506: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711A50F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006711A514: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  000000006711A519: 48 8B D9           mov         rbx,rcx
  000000006711A51C: 48 8D 35 95 66 1D  lea         rsi,[00000000672F0BB8h]
                    00
  000000006711A523: 48 89 74 24 40     mov         qword ptr [rsp+40h],rsi
  000000006711A528: F0 83 05 88 66 1D  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006711A530: F0 83 05 80 66 1D  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006711A538: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006711A53B: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000006711A53F: 75 08              jne         000000006711A549
  000000006711A541: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006711A544: E8 47 AF EE FF     call        0000000067005490
  000000006711A549: 48 89 33           mov         qword ptr [rbx],rsi
  000000006711A54C: F6 05 71 66 1D 00  test        byte ptr [00000000672F0BC4h],1
                    01
  000000006711A553: 75 16              jne         000000006711A56B
  000000006711A555: 44 8B 05 60 66 1D  mov         r8d,dword ptr [00000000672F0BBCh]
                    00
  000000006711A55C: 8B 15 5E 66 1D 00  mov         edx,dword ptr [00000000672F0BC0h]
  000000006711A562: 48 8B CB           mov         rcx,rbx
  000000006711A565: E8 16 B0 EE FF     call        0000000067005580
  000000006711A56A: 90                 nop
  000000006711A56B: F0 83 05 45 66 1D  lock add    dword ptr [00000000672F0BB8h],0FFFFFFFFh
                    00 FF
  000000006711A573: 75 52              jne         000000006711A5C7
  000000006711A575: 48 63 1D 44 66 1D  movsxd      rbx,dword ptr [00000000672F0BC0h]
                    00
  000000006711A57C: 48 C1 E3 04        shl         rbx,4
  000000006711A580: 48 8D 3D 41 66 1D  lea         rdi,[00000000672F0BC8h]
                    00
  000000006711A587: 48 03 DF           add         rbx,rdi
  000000006711A58A: 48 3B DF           cmp         rbx,rdi
  000000006711A58D: 74 2B              je          000000006711A5BA
  000000006711A58F: 90                 nop
  000000006711A590: 48 83 EB 10        sub         rbx,10h
  000000006711A594: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711A597: 48 85 C9           test        rcx,rcx
  000000006711A59A: 74 19              je          000000006711A5B5
  000000006711A59C: F0 83 41 08 FF     lock add    dword ptr [rcx+8],0FFFFFFFFh
  000000006711A5A1: 75 12              jne         000000006711A5B5
  000000006711A5A3: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711A5A6: 48 85 C9           test        rcx,rcx
  000000006711A5A9: 74 0A              je          000000006711A5B5
  000000006711A5AB: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711A5AE: BA 01 00 00 00     mov         edx,1
  000000006711A5B3: FF 10              call        qword ptr [rax]
  000000006711A5B5: 48 3B DF           cmp         rbx,rdi
  000000006711A5B8: 75 D6              jne         000000006711A590
  000000006711A5BA: BA 08 00 00 00     mov         edx,8
  000000006711A5BF: 48 8B CE           mov         rcx,rsi
  000000006711A5C2: E8 C9 1F F6 FF     call        000000006707C590
  000000006711A5C7: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006711A5CC: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  000000006711A5D1: 48 83 C4 30        add         rsp,30h
  000000006711A5D5: 5F                 pop         rdi
  000000006711A5D6: C3                 ret

  Summary

      1A7000 .text
```

### 6711a5e0..6711a64b exclusive

unwind interval; code SHA-256 `54a6ef9a3d1eb6019a832a862c117d707a72a1a396bde7ab996b4cdc522a6cdd`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x6711a5e0,0x6711a64a evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711A5E0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006711A5E5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006711A5EA: 57                 push        rdi
  000000006711A5EB: 48 83 EC 20        sub         rsp,20h
  000000006711A5EF: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711A5F2: 49 63 F8           movsxd      rdi,r8d
  000000006711A5F5: 48 63 F2           movsxd      rsi,edx
  000000006711A5F8: 44 8B 08           mov         r9d,dword ptr [rax]
  000000006711A5FB: 48 8B D9           mov         rbx,rcx
  000000006711A5FE: 41 83 F9 01        cmp         r9d,1
  000000006711A602: 74 0B              je          000000006711A60F
  000000006711A604: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711A607: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711A60A: E8 B1 E8 FF FF     call        0000000067118EC0
  000000006711A60F: 4C 8B 13           mov         r10,qword ptr [rbx]
  000000006711A612: 49 63 42 08        movsxd      rax,dword ptr [r10+8]
  000000006711A616: 4C 8D 04 30        lea         r8,[rax+rsi]
  000000006711A61A: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006711A61F: 48 03 C7           add         rax,rdi
  000000006711A622: 49 8B 4C C2 18     mov         rcx,qword ptr [r10+rax*8+18h]
  000000006711A627: 4F 8B 4C C2 18     mov         r9,qword ptr [r10+r8*8+18h]
  000000006711A62C: 4B 89 4C C2 18     mov         qword ptr [r10+r8*8+18h],rcx
  000000006711A631: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711A634: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006711A639: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  000000006711A63D: 48 03 C7           add         rax,rdi
  000000006711A640: 4C 89 4C C1 18     mov         qword ptr [rcx+rax*8+18h],r9
  000000006711A645: 48 83 C4 20        add         rsp,20h
  000000006711A649: 5F                 pop         rdi
  000000006711A64A: C3                 ret

  Summary

      1A7000 .text
```

### 6711ab90..6711ac64 exclusive

unwind interval; code SHA-256 `c9d98620e2f476081edbcbc5ed15e92cdda60642d210688e665ad6a0445b0b0e`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x6711ab90,0x6711ac63 evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711AB90: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006711AB95: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006711AB9A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006711AB9F: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000006711ABA4: 41 54              push        r12
  000000006711ABA6: 48 83 EC 20        sub         rsp,20h
  000000006711ABAA: 49 8B 18           mov         rbx,qword ptr [r8]
  000000006711ABAD: 49 8B F1           mov         rsi,r9
  000000006711ABB0: 4D 8B E0           mov         r12,r8
  000000006711ABB3: 48 8B EA           mov         rbp,rdx
  000000006711ABB6: 48 8B F9           mov         rdi,rcx
  000000006711ABB9: 49 3B 19           cmp         rbx,qword ptr [r9]
  000000006711ABBC: 73 13              jae         000000006711ABD1
  000000006711ABBE: 66 90              nop
  000000006711ABC0: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711ABC3: E8 8E DD 05 00     call        0000000067178956
  000000006711ABC8: 48 83 C3 08        add         rbx,8
  000000006711ABCC: 48 3B 1E           cmp         rbx,qword ptr [rsi]
  000000006711ABCF: 72 EF              jb          000000006711ABC0
  000000006711ABD1: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711ABD4: 8B 08              mov         ecx,dword ptr [rax]
  000000006711ABD6: 83 F9 01           cmp         ecx,1
  000000006711ABD9: 74 0E              je          000000006711ABE9
  000000006711ABDB: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711ABDE: 48 8B CF           mov         rcx,rdi
  000000006711ABE1: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711ABE4: E8 D7 E2 FF FF     call        0000000067118EC0
  000000006711ABE9: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711ABEC: 4C 8B 06           mov         r8,qword ptr [rsi]
  000000006711ABEF: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  000000006711ABF3: 48 8D 44 C1 18     lea         rax,[rcx+rax*8+18h]
  000000006711ABF8: 49 8B 0C 24        mov         rcx,qword ptr [r12]
  000000006711ABFC: 48 8B D9           mov         rbx,rcx
  000000006711ABFF: 4C 2B C1           sub         r8,rcx
  000000006711AC02: 48 8B CF           mov         rcx,rdi
  000000006711AC05: 48 2B D8           sub         rbx,rax
  000000006711AC08: 49 C1 F8 03        sar         r8,3
  000000006711AC0C: 48 C1 FB 03        sar         rbx,3
  000000006711AC10: 8B D3              mov         edx,ebx
  000000006711AC12: E8 B9 E2 F2 FF     call        0000000067048ED0
  000000006711AC17: 4C 8B 1F           mov         r11,qword ptr [rdi]
  000000006711AC1A: 41 8B 03           mov         eax,dword ptr [r11]
  000000006711AC1D: 83 F8 01           cmp         eax,1
  000000006711AC20: 74 0E              je          000000006711AC30
  000000006711AC22: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711AC25: 48 8B CF           mov         rcx,rdi
  000000006711AC28: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711AC2B: E8 90 E2 FF FF     call        0000000067118EC0
  000000006711AC30: 48 8B 17           mov         rdx,qword ptr [rdi]
  000000006711AC33: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006711AC38: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  000000006711AC3D: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  000000006711AC41: 48 63 C3           movsxd      rax,ebx
  000000006711AC44: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006711AC49: 48 03 C8           add         rcx,rax
  000000006711AC4C: 48 8D 44 CA 18     lea         rax,[rdx+rcx*8+18h]
  000000006711AC51: 48 89 45 00        mov         qword ptr [rbp],rax
  000000006711AC55: 48 8B C5           mov         rax,rbp
  000000006711AC58: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  000000006711AC5D: 48 83 C4 20        add         rsp,20h
  000000006711AC61: 41 5C              pop         r12
  000000006711AC63: C3                 ret

  Summary

      1A7000 .text
```

### 67048ed0..67048f6f exclusive

unwind interval; code SHA-256 `3c7c062f95e4647697c53e487eda8d5df05a1fda9b006990dd0ededd601ea635`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x67048ed0,0x67048f6e evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067048ED0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067048ED5: 57                 push        rdi
  0000000067048ED6: 48 83 EC 20        sub         rsp,20h
  0000000067048EDA: 4C 8B 19           mov         r11,qword ptr [rcx]
  0000000067048EDD: 49 63 D8           movsxd      rbx,r8d
  0000000067048EE0: 48 8B F9           mov         rdi,rcx
  0000000067048EE3: 45 8B 4B 08        mov         r9d,dword ptr [r11+8]
  0000000067048EE7: 45 8B 43 0C        mov         r8d,dword ptr [r11+0Ch]
  0000000067048EEB: 8B C3              mov         eax,ebx
  0000000067048EED: 45 8D 14 11        lea         r10d,[r9+rdx]
  0000000067048EF1: 99                 cdq
  0000000067048EF2: 41 8B C8           mov         ecx,r8d
  0000000067048EF5: 2B C2              sub         eax,edx
  0000000067048EF7: D1 F8              sar         eax,1
  0000000067048EF9: 41 03 C2           add         eax,r10d
  0000000067048EFC: 8B D0              mov         edx,eax
  0000000067048EFE: 2B C8              sub         ecx,eax
  0000000067048F00: 41 2B D1           sub         edx,r9d
  0000000067048F03: 3B D1              cmp         edx,ecx
  0000000067048F05: 7D 32              jge         0000000067048F39
  0000000067048F07: 49 63 C1           movsxd      rax,r9d
  0000000067048F0A: 44 2B D0           sub         r10d,eax
  0000000067048F0D: 49 8D 54 C3 18     lea         rdx,[r11+rax*8+18h]
  0000000067048F12: 48 03 C3           add         rax,rbx
  0000000067048F15: 4D 63 C2           movsxd      r8,r10d
  0000000067048F18: 49 8D 4C C3 18     lea         rcx,[r11+rax*8+18h]
  0000000067048F1D: 49 C1 E0 03        shl         r8,3
  0000000067048F21: FF 15 81 F6 15 00  call        qword ptr [00000000671A85A8h]
  0000000067048F27: 4C 8B 1F           mov         r11,qword ptr [rdi]
  0000000067048F2A: 41 01 5B 08        add         dword ptr [r11+8],ebx
  0000000067048F2E: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067048F33: 48 83 C4 20        add         rsp,20h
  0000000067048F37: 5F                 pop         rdi
  0000000067048F38: C3                 ret
  0000000067048F39: 45 2B C2           sub         r8d,r10d
  0000000067048F3C: 49 63 CA           movsxd      rcx,r10d
  0000000067048F3F: 48 8D 04 19        lea         rax,[rcx+rbx]
  0000000067048F43: 44 2B C3           sub         r8d,ebx
  0000000067048F46: 49 8D 4C CB 18     lea         rcx,[r11+rcx*8+18h]
  0000000067048F4B: 4D 63 C0           movsxd      r8,r8d
  0000000067048F4E: 49 8D 54 C3 18     lea         rdx,[r11+rax*8+18h]
  0000000067048F53: 49 C1 E0 03        shl         r8,3
  0000000067048F57: FF 15 4B F6 15 00  call        qword ptr [00000000671A85A8h]
  0000000067048F5D: 4C 8B 1F           mov         r11,qword ptr [rdi]
  0000000067048F60: 41 29 5B 0C        sub         dword ptr [r11+0Ch],ebx
  0000000067048F64: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067048F69: 48 83 C4 20        add         rsp,20h
  0000000067048F6D: 5F                 pop         rdi
  0000000067048F6E: C3                 ret

  Summary

      1A7000 .text
```

### 6711c010..6711c017 exclusive

explicit 7-byte wrapper interval; code SHA-256 `ecbc9dcdf3cc5e77369080b1d5818eede13d2f0aca85c070fbf7945c796bd132`. A PE unwind interval is a bounded capture, not proof of a complete logical call graph. Command: `/disasm:bytes /range:0x6711c010,0x6711c016 evidence/original-binaries/QtCore4_64.dll`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711C010: 33 D2              xor         edx,edx
  000000006711C012: E9 E9 F4 FF FF     jmp         000000006711B500

  Summary

      1A7000 .text
```

