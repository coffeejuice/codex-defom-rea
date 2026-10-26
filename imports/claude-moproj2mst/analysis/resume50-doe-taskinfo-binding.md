# TaskInfo font binding after checkpoint49

The active native Report guard cannot advance with the currently implemented
owners and constructor state. The isolated default-font helper has a precise
future call site, but it does not complete any prerequisite of Report's public
QObject constructor. Keep the guard at its present boundary.

This determination uses current source, immutable original evidence and the
checkpoint49 freeze. This work unit changes only this report. No source, build,
original application, GUI or mutable REA operation was performed.

## Exact integration site of the first-font helper

The native sequence is:

1. First Report allocation enters `1407b89e0 ->14157ead0 ->141403210`.
2. Task base calls public `QObject(parent=null)` at `141403229`. Its return must
   precede the Task vtable write at `14140323c` and TaskInfo wrapper call at
   `141403248`.
3. TaskInfo wrapper `14140e090` writes its vtable, allocates a stable 0x80 private
   object and invokes `14140e910`. Wrapper+8 is not published until `14140e0f6`,
   after the private constructor returns.
4. That private constructor default-constructs QString values at private offsets
   0, 8, 0x10, 0x18 and 0x20 before its default QFont call at `14140e989` with
   receiver `private+0x30`.

`isolatedTaskInfoDefaultFontValue(firstCurrentApplicationFont)` models only the
value result of step4's QFont constructor. The original default QFont body
`651f3560` obtains QApplication::font at `651f3580`, shares its actual FontData
pointer, performs native reference/temporary-lifetime work, then writes the new
QFont's resolve mask to zero at `651f35c6`.

An eventual native integration must obtain the actual current value at that
source call point from the same application/font owner, preserve the shared
FontData identity, and retain the result in the same pending private object. It
must also account for the native reference effects; the isolated shared_ptr value
helper is not evidence that those atomic effects occurred. Do not create another
SessionTask, private TaskInfo owner, or application-font cache to make the call
appear reachable.

The current pending descriptor holds kind/size and a false `qobjectBaseCompleted`
observation. It has no QObject private allocation, TaskInfo wrapper/private frame,
font stage or publication state. The helper has no production caller, by design.

## The second current-font call preserves its own mask

This is a separate call and a different value rule:

| Site | Operation | Required effect |
| --- | --- | --- |
| `14140ea36` | IAT `141e0b960`, QApplication::font() | Obtain the current font again into a temporary |
| `14140ea5c` | IAT `141e078f0`, QFont copy assignment to private+0x30 | Assign the second value's FontData **and its mask** |
| `14140ea68` | IAT `141e0bf28`, temporary QFont destruction | Release the temporary after assignment |

These calls occur after the initial null icon, three QColor initializations and
the true byte at private+0x7c. They occur before the second null-icon assignment,
private-constructor return and wrapper+8 publication. An unavailable second font
therefore must not erase the already reached first-font/icon/color/flag prefix or
publish a completed TaskInfo.

The retained original export table maps these exact imports to:

- QApplication::font(): `6501b330`.
- QFont copy constructor: `652390c0`.
- QFont copy assignment: `651f3060`.
- QFont destructor: `65206b20`.

The no-argument QApplication getter `6501b330` reads its actual global font pointer
at `659c96b8`; its selected null branch can allocate/initialize that global. At
`6501b406` it calls QFont copy constructor `652390c0` on the selected global font.
The copy constructor copies the data pointer at `652390d3` and the source mask at
`652390e1-652390e4`. It does not force that copied mask to zero.

QFont assignment `651f3060` first delegates data-owner replacement to `651f2840`,
then unconditionally copies source+8 to destination+8 at `651f3078-651f3081`.
The data helper has an equal-pointer fast path at `651f2850-651f2853`; taking that
path does **not** skip the later mask store in its caller. On different data,
the native helper increments the incoming nonnull data reference, installs it,
then decrements/releases the old data. Exact runtime counts/cleanup remain a
separate dependency.

Therefore reusing the isolated mask-zero helper for the second call would change
behavior even if both calls return the same FontData pointer. For example, a
second current value with that same data and mask0x1234 must leave private+0x30
with mask0x1234, whereas the first default constructor's local mask is0.
This is a source-derived distinction, not an observed original runtime example.

Do not collapse the two QApplication calls into one startup value. Current
`WidgetRuntimeState::applicationFont()` returns the stored default-font
observation; startup observation is accepted once. That existing API establishes
ownership of an observed value, not invariance through both later source sites or
qualification of the getter's lazy-init/mutex/refcount paths.

## Why the current Report guard stays

The original public QObject constructor initializes its private shared-container
references and backlinks, selects actual current QThreadData through TLS,
increments that thread-data reference, and only then advances the shared QObject
counter. TaskInfo follows the completed call.

Current source still provides only a counter-specific QObject record and the
thread's notify-depth/event queue projection. It does not provide:

- Same-runtime shared-null container identities/refcounts for the public private
  initializer.
- Qualified TLS index/result, actual QThreadData identity and its reference owner.
- A once-only in-place QObject/private constructor frame on the retained Report
  allocation, including preservation of untouched private bits.
- A TaskInfo wrapper/private frame whose partial fields and publication can be
  distinguished from a completed object.
- Current application-font observations bound to both TaskInfo source call sites.

The queue's recorded RDOE blocker still explicitly identifies unobserved actual
Report TLS/shared refs. Counter commit, event notify depth, matching font alias,
isolated helper success and passing regression do not supply those inputs.

No current active native branch is newly enabled. A future bounded prefix could
first represent the already source-known QObject vtable/private allocation before
the shared-ref initializer, but that requires a stable per-allocation native
identity and private-allocation owner. It must stop before the first unavailable
shared effect. It cannot be achieved by jumping directly to TaskInfo or invoking
the font helper from the existing descriptor.

## Concrete successor and validation boundary

The next active integration dependency is the existing root-owned shared
QObject/TLS/refcount contract and constructor frame. Only after that closes can
the first helper be bound at `14140e989` with a qualified current value.

An independent second-font **value** helper could later copy both supplied
FontData owner and mask, with tests for same-data/different-mask and different
owners, while remaining isolated. That would qualify another value rule, not move
the public Report guard or prove native atomic release behavior. No such helper
is implemented by this report.

Integration tests should separately stop before first and second font reads and
assert the retained constructor prefix, stable Report/private identities,
unpublished wrapper+8 and once-only effects. Native observations must distinguish
both calls; neither a single startup capture nor two matching synthetic fixtures
is acceptance evidence for the actual lifecycle.

## Evidence and offline receipts

The immutable executable is original DEF_GUI_MO_64 v11.0.2, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The QtGui4_64.dll SHA-256 is
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
No DLL was loaded/executed. Retained evidence has its original qualifications.
Only named export entries are used, not table-neighbor inference.

The bounded copy-constructor and data-assignment selections below are retained
inline so this task writes only its owned Markdown report. The PE unwind region
for `651f2840` ends at `651f2855`, before its actual branches finish; the explicit
selection extends through both returns at `651f28a2` and `651f28b0`. An unwind
region is not proof of a complete logical method. Deallocation callees reached
from the unequal-data path remain qualified, not claimed runtime-complete.

<!-- Bounded receipts and SHA-256 table appended by offline verification. -->

Verification UTC: 2026-10-24T18:39:34.250249+00:00.

Static checks passed: source call order, exact named font exports, unchanged candidate49 DOE freeze hashes, retained native Report guard, and absence of a production helper call. Current thread class still has no TLS/QThreadData owner state. These are offline consistency checks; no tests were executed in this work unit.

DUMPBIN SHA-256: `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.

### Inline immutable selection 0x652390c0

Command: `dumpbin.exe /disasm:bytes /range:0x652390c0,0x652390f4 evidence/original-binaries/QtGui4_64.dll`. Exit code0. Raw stdout SHA-256: `14feadf4eb98d3f27a539a0298ffb79a7190223b48f31f2e29fac61634d54d1d`. The block normalizes CRLF to LF; normalized UTF-8 SHA-256: `8b4eb6368aa01a7b1a287a5ae4911cbd84a7c7323e8381d992bf0224511ff3ea`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000652390C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000652390C5: 57                 push        rdi
  00000000652390C6: 48 83 EC 20        sub         rsp,20h
  00000000652390CA: 48 8B D9           mov         rbx,rcx
  00000000652390CD: 48 8B 0A           mov         rcx,qword ptr [rdx]
  00000000652390D0: 48 8B FA           mov         rdi,rdx
  00000000652390D3: 48 89 0B           mov         qword ptr [rbx],rcx
  00000000652390D6: 48 85 C9           test        rcx,rcx
  00000000652390D9: 74 06              je          00000000652390E1
  00000000652390DB: FF 15 DF D7 45 00  call        qword ptr [00000000656968C0h]
  00000000652390E1: 8B 47 08           mov         eax,dword ptr [rdi+8]
  00000000652390E4: 89 43 08           mov         dword ptr [rbx+8],eax
  00000000652390E7: 48 8B C3           mov         rax,rbx
  00000000652390EA: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000652390EF: 48 83 C4 20        add         rsp,20h
  00000000652390F3: 5F                 pop         rdi
  00000000652390F4: C3                 ret

  Summary

      695000 .text
```

### Inline immutable selection 0x651f2840

Command: `dumpbin.exe /disasm:bytes /range:0x651f2840,0x651f28b0 evidence/original-binaries/QtGui4_64.dll`. Exit code0. Raw stdout SHA-256: `429d623a464976215bb78c9795fd4bf4bab28444ff2da623df82ef2a93857649`. The block normalizes CRLF to LF; normalized UTF-8 SHA-256: `6551b2d94412db4e179ee3146d9a09d8b7bbf6083aa497bbe895b05ed069ad2d`.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F2840: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000651F2845: 57                 push        rdi
  00000000651F2846: 48 83 EC 20        sub         rsp,20h
  00000000651F284A: 48 8B FA           mov         rdi,rdx
  00000000651F284D: 48 8B D9           mov         rbx,rcx
  00000000651F2850: 48 3B 11           cmp         rdx,qword ptr [rcx]
  00000000651F2853: 74 4E              je          00000000651F28A3
  00000000651F2855: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  00000000651F285A: 48 85 D2           test        rdx,rdx
  00000000651F285D: 74 09              je          00000000651F2868
  00000000651F285F: 48 8B CA           mov         rcx,rdx
  00000000651F2862: FF 15 58 40 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F2868: 48 8B 33           mov         rsi,qword ptr [rbx]
  00000000651F286B: 48 89 3B           mov         qword ptr [rbx],rdi
  00000000651F286E: 48 85 F6           test        rsi,rsi
  00000000651F2871: 74 1D              je          00000000651F2890
  00000000651F2873: 48 8B CE           mov         rcx,rsi
  00000000651F2876: FF 15 FC 40 4A 00  call        qword ptr [0000000065696978h]
  00000000651F287C: 84 C0              test        al,al
  00000000651F287E: 75 10              jne         00000000651F2890
  00000000651F2880: 48 8B CE           mov         rcx,rsi
  00000000651F2883: E8 B8 F7 FF FF     call        00000000651F2040
  00000000651F2888: 48 8B CE           mov         rcx,rsi
  00000000651F288B: E8 1E E8 3E 00     call        00000000655E10AE
  00000000651F2890: 48 8B C3           mov         rax,rbx
  00000000651F2893: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  00000000651F2898: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000651F289D: 48 83 C4 20        add         rsp,20h
  00000000651F28A1: 5F                 pop         rdi
  00000000651F28A2: C3                 ret
  00000000651F28A3: 48 8B C1           mov         rax,rcx
  00000000651F28A6: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000651F28AB: 48 83 C4 20        add         rsp,20h
  00000000651F28AF: 5F                 pop         rdi
  00000000651F28B0: C3                 ret

  Summary

      695000 .text
```

### Referenced evidence

| Evidence | SHA-256 |
| --- | --- |
| `analysis/resume48-doe-taskinfo-next.md` | `32716851017617077f8ad96e13f9a2b99d0995ec64f9019ae5cb96cd7eb56a8a` |
| `analysis/resume49-doe-default-font-helper.json` | `b652fd2d7ba945826f0afdf9c0d67773ae216f943a54d5c911fc2a2a975cdea6` |
| `analysis/wave2-doe-task-prototype-taskinfo-private-constructor.asm` | `172e1daaf0e6d57fde2153eb64694da2486bc4500d2d840b5f064000b83d980a` |
| `analysis/wave2-doe-task-prototype-taskinfo-ctor.asm` | `7f395fce24a655724492b720cefd36664223c47a84f99b208456e446d40a1ada` |
| `analysis/wave2-doe-task-prototype-qt-default-font.asm` | `1e816c4d5df5edc117db15ae602bc7ad75fbe283c4353614eb2ef356786c81f9` |
| `analysis/wave3-closure-next-6501b330.asm` | `60ae5bc355a02e2d7c3f5e9e1419b2d97c3a12a890712b475a1ee097d7b44130` |
| `analysis/wave3-closure-next-651f3060.asm` | `e10223f224873b6508885795bb924687e304d499141ec7a24070ad50bd5d53f8` |
| `analysis/wave3-closure-next-close-widget-font-same-data-assignment-static.json` | `6b818a4681c8d19c2ca5c92259847670cf5d831050efb95f89ca5864c5d05a2b` |
| `analysis/wave2-doe-task-public-qobject-ownership-contract.json` | `81e05b0f2aee8014da834c2a3db46df6eea09a6a01e6744498e4ee3d9afd9a32` |
| `analysis/wave3-doe-report-qobject-prerequisite-contract.json` | `d132044c91c93b2b6b8017acb11f05451e4ef3b4896f83f158aa055d379a5998` |
| `analysis/wave3-doe-report-qobject-capture-plan.json` | `feb2f1a14fa2216e21aee8a9775bb8bca20cb73ff9811597f585a2aa41b706db` |
| `analysis/wave3-checkpoint49-validation.json` | `6a525fad3b7950d9a3f8e0a207794e8f2958139a5ec9f44848ef30ff93f56b3e` |
| `build/wave3-checkpoint49-verified/receipt.json` | `fbf593fe9d637113807fa90a0c9c86f23c3255842cf19a84d44c2eaa16586945` |

### Current source binding

| Source | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d59535b1771173dfdbdd2bef5ae6637867336544e3487a4d4b683dcab6274ae` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `1d5d13c205660c8f0bf36f8eb28955c5948abaa0f468534308e1f52576c34b7b` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `b41fdc07913a8a15711798324bfcfc7feec5d7abe171e5c342190bc45222369a` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a3c7cff4a57a51b84c8cf4dd17999f8790c20e9448aada9eae70bd8456f4175a` |
| `source/apps/moproj2mst/include/m2m/engine/widget_events.hpp` | `e75a9f7631dab2202a894310e94adfbda0906232074a6b57c8ab1531cd55b4ec` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `69e4b6f1e88644502abb047824db88bab7d511d7dc76c3f3943986f1d0ea66b7` |

Token measurements are unavailable. No speed or token-saving claim is made.
