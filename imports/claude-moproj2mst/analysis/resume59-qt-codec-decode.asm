Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067072460: 40 53              push        rbx
  0000000067072462: 48 83 EC 30        sub         rsp,30h
  0000000067072466: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006707246B: 4C 8B 11           mov         r10,qword ptr [rcx]
  000000006707246E: 48 8B DA           mov         rbx,rdx
  0000000067072471: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000067072476: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  000000006707247E: 41 FF 52 18        call        qword ptr [r10+18h]
  0000000067072482: 48 8B C3           mov         rax,rbx
  0000000067072485: 48 83 C4 30        add         rsp,30h
  0000000067072489: 5B                 pop         rbx
  000000006707248A: C3                 ret
  000000006707248B: CC                 int         3
  000000006707248C: CC                 int         3
  000000006707248D: CC                 int         3
  000000006707248E: CC                 int         3
  000000006707248F: CC                 int         3
  0000000067072490: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000067072494: 53                 push        rbx
  0000000067072495: 48 83 EC 30        sub         rsp,30h
  0000000067072499: 48 8B D9           mov         rbx,rcx
  000000006707249C: E8 4F DC 0D 00     call        00000000671500F0
  00000000670724A1: 33 D2              xor         edx,edx
  00000000670724A3: 84 C0              test        al,al
  00000000670724A5: 48 0F 45 15 63 19  cmovne      rdx,qword ptr [00000000672F3E10h]
                    28 00
  00000000670724AD: 48 85 D2           test        rdx,rdx
  00000000670724B0: 74 5A              je          000000006707250C
  00000000670724B2: E8 39 DC 0D 00     call        00000000671500F0
  00000000670724B7: 33 C9              xor         ecx,ecx
  00000000670724B9: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  00000000670724BE: 84 C0              test        al,al
  00000000670724C0: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670724C5: 41 B9 01 00 00 00  mov         r9d,1
  00000000670724CB: 48 0F 45 0D 3D 19  cmovne      rcx,qword ptr [00000000672F3E10h]
                    28 00
  00000000670724D3: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000000670724DC: E8 7F FF FF FF     call        0000000067072460
  00000000670724E1: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000670724E4: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000670724E8: 0F B7 08           movzx       ecx,word ptr [rax]
  00000000670724EB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670724F0: 66 89 0B           mov         word ptr [rbx],cx
  00000000670724F3: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670724F7: 75 1B              jne         0000000067072514
  00000000670724F9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670724FE: E8 CD DB FF FF     call        00000000670700D0
  0000000067072503: 48 8B C3           mov         rax,rbx
  0000000067072506: 48 83 C4 30        add         rsp,30h
  000000006707250A: 5B                 pop         rbx
  000000006707250B: C3                 ret
  000000006707250C: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000067072511: 66 89 03           mov         word ptr [rbx],ax
  0000000067072514: 48 8B C3           mov         rax,rbx
  0000000067072517: 48 83 C4 30        add         rsp,30h
  000000006707251B: 5B                 pop         rbx
  000000006707251C: C3                 ret
  000000006707251D: CC                 int         3
  000000006707251E: CC                 int         3
  000000006707251F: CC                 int         3
  0000000067072520: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000067072525: 57                 push        rdi
  0000000067072526: 48 83 EC 30        sub         rsp,30h
  000000006707252A: 0F B6 FA           movzx       edi,dl
  000000006707252D: 48 8B D9           mov         rbx,rcx
  0000000067072530: E8 BB DB 0D 00     call        00000000671500F0
  0000000067072535: 45 33 C0           xor         r8d,r8d
  0000000067072538: 84 C0              test        al,al
  000000006707253A: 4C 0F 45 05 CE 18  cmovne      r8,qword ptr [00000000672F3E10h]
                    28 00
  0000000067072542: 4D 85 C0           test        r8,r8
  0000000067072545: 74 64              je          00000000670725AB
  0000000067072547: 40 88 7C 24 40     mov         byte ptr [rsp+40h],dil
  000000006707254C: E8 9F DB 0D 00     call        00000000671500F0
  0000000067072551: 33 C9              xor         ecx,ecx
  0000000067072553: 84 C0              test        al,al
  0000000067072555: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  000000006707255A: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006707255F: 48 0F 45 0D A9 18  cmovne      rcx,qword ptr [00000000672F3E10h]
                    28 00
  0000000067072567: 41 B9 01 00 00 00  mov         r9d,1
  000000006707256D: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000067072576: E8 E5 FE FF FF     call        0000000067072460
  000000006707257B: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006707257E: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000067072582: 0F B7 08           movzx       ecx,word ptr [rax]
  0000000067072585: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006707258A: 66 89 0B           mov         word ptr [rbx],cx
  000000006707258D: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067072591: 75 1F              jne         00000000670725B2
  0000000067072593: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000067072598: E8 33 DB FF FF     call        00000000670700D0
  000000006707259D: 48 8B C3           mov         rax,rbx
  00000000670725A0: 48

  Summary

      1A7000 .text
