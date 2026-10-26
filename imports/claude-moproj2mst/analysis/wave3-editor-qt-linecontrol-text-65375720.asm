Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065375720: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000065375725: 53                 push        rbx
  0000000065375726: 57                 push        rdi
  0000000065375727: 48 83 EC 38        sub         rsp,38h
  000000006537572B: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065375734: 48 8B FA           mov         rdi,rdx
  0000000065375737: 33 DB              xor         ebx,ebx
  0000000065375739: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006537573D: 48 39 99 98 00 00  cmp         qword ptr [rcx+0000000000000098h],rbx
                    00
  0000000065375744: 74 1A              je          0000000065375760
  0000000065375746: 4C 8D 41 10        lea         r8,[rcx+10h]
  000000006537574A: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006537574F: E8 5C F5 04 00     call        00000000653C4CB0
  0000000065375754: 90                 nop
  0000000065375755: BB 01 00 00 00     mov         ebx,1
  000000006537575A: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006537575E: EB 04              jmp         0000000065375764
  0000000065375760: 48 8D 41 10        lea         rax,[rcx+10h]
  0000000065375764: 48 8B D0           mov         rdx,rax
  0000000065375767: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006537576C: FF 15 7E 12 32 00  call        qword ptr [00000000656969F0h]
  0000000065375772: 90                 nop
  0000000065375773: F6 C3 01           test        bl,1
  0000000065375776: 74 12              je          000000006537578A
  0000000065375778: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006537577B: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006537577F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065375784: FF 15 D6 0F 32 00  call        qword ptr [0000000065696760h]
  000000006537578A: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006537578F: FF 15 53 1C 32 00  call        qword ptr [00000000656973E8h]
  0000000065375795: 84 C0              test        al,al
  0000000065375797: 74 20              je          00000000653757B9
  0000000065375799: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  000000006537579D: 48 8D 15 C4 A1 33  lea         rdx,[00000000656AF968h]
                    00
  00000000653757A4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000653757A9: FF 15 F1 10 32 00  call        qword ptr [00000000656968A0h]
  00000000653757AF: 90                 nop
  00000000653757B0: 83 CB 02           or          ebx,2
  00000000653757B3: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  00000000653757B7: EB 05              jmp         00000000653757BE
  00000000653757B9: 48 8D 44 24 60     lea         rax,[rsp+60h]
  00000000653757BE: 48 8B D0           mov         rdx,rax
  00000000653757C1: 48 8B CF           mov         rcx,rdi
  00000000653757C4: FF 15 26 12 32 00  call        qword ptr [00000000656969F0h]
  00000000653757CA: 83 CB 04           or          ebx,4
  00000000653757CD: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  00000000653757D1: F6 C3 02           test        bl,2
  00000000653757D4: 74 13              je          00000000653757E9
  00000000653757D6: 83 E3 FD           and         ebx,0FFFFFFFDh
  00000000653757D9: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  00000000653757DD: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000653757E2: FF 15 78 0F 32 00  call        qword ptr [0000000065696760h]
  00000000653757E8: 90                 nop
  00000000653757E9: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000653757EE: FF 15 6C 0F 32 00  call        qword ptr [0000000065696760h]
  00000000653757F4: 48 8B C7           mov         rax,rdi
  00000000653757F7: 48 83 C4 38        add         rsp,38h
  00000000653757FB: 5F                 pop         rdi
  00000000653757FC: 5B                 pop         rbx
  00000000653757FD: C3                 ret
  00000000653757FE: CC                 int         3

  Summary

      695000 .text
