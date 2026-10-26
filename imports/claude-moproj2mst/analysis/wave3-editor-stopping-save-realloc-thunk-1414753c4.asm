Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414753C4: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001414753C9: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000001414753CE: 57                 push        rdi
  00000001414753CF: 48 83 EC 20        sub         rsp,20h
  00000001414753D3: 48 8B DA           mov         rbx,rdx
  00000001414753D6: 48 8B F9           mov         rdi,rcx
  00000001414753D9: 48 85 C9           test        rcx,rcx
  00000001414753DC: 75 0A              jne         00000001414753E8
  00000001414753DE: 48 8B CA           mov         rcx,rdx
  00000001414753E1: E8 B6 F4 FF FF     call        000000014147489C
  00000001414753E6: EB 6A              jmp         0000000141475452
  00000001414753E8: 48 85 D2           test        rdx,rdx
  00000001414753EB: 75 07              jne         00000001414753F4
  00000001414753ED: E8 8A C7 FF FF     call        0000000141471B7C
  00000001414753F2: EB 5C              jmp         0000000141475450
  00000001414753F4: 48 83 FA E0        cmp         rdx,0FFFFFFFFFFFFFFE0h
  00000001414753F8: 77 43              ja          000000014147543D
  00000001414753FA: 48 8B 0D 0F B4 28  mov         rcx,qword ptr [0000000144700810h]
                    03
  0000000141475401: B8 01 00 00 00     mov         eax,1
  0000000141475406: 48 85 DB           test        rbx,rbx
  0000000141475409: 48 0F 44 D8        cmove       rbx,rax
  000000014147540D: 4C 8B C7           mov         r8,rdi
  0000000141475410: 33 D2              xor         edx,edx
  0000000141475412: 4C 8B CB           mov         r9,rbx
  0000000141475415: FF 15 7D DF 98 00  call        qword ptr [0000000141E03398h]
  000000014147541B: 48 8B F0           mov         rsi,rax
  000000014147541E: 48 85 C0           test        rax,rax
  0000000141475421: 75 6F              jne         0000000141475492
  0000000141475423: 39 05 2F B4 28 03  cmp         dword ptr [0000000144700858h],eax
  0000000141475429: 74 50              je          000000014147547B
  000000014147542B: 48 8B CB           mov         rcx,rbx
  000000014147542E: E8 B5 C8 00 00     call        0000000141481CE8
  0000000141475433: 85 C0              test        eax,eax
  0000000141475435: 74 2B              je          0000000141475462
  0000000141475437: 48 83 FB E0        cmp         rbx,0FFFFFFFFFFFFFFE0h
  000000014147543B: 76 BD              jbe         00000001414753FA
  000000014147543D: 48 8B CB           mov         rcx,rbx
  0000000141475440: E8 A3 C8 00 00     call        0000000141481CE8
  0000000141475445: E8 EA EC FF FF     call        0000000141474134
  000000014147544A: C7 00 0C 00 00 00  mov         dword ptr [rax],0Ch
  0000000141475450: 33 C0              xor         eax,eax
  0000000141475452: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141475457: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000014147545C: 48 83 C4 20        add         rsp,20h
  0000000141475460: 5F                 pop         rdi
  0000000141475461: C3                 ret
  0000000141475462: E8 CD EC FF FF     call        0000000141474134
  0000000141475467: 48 8B D8           mov         rbx,rax
  000000014147546A: FF 15 78 E1 98 00  call        qword ptr [0000000141E035E8h]
  0000000141475470: 8B C8              mov         ecx,eax
  0000000141475472: E8 75 EC FF FF     call        00000001414740EC
  0000000141475477: 89 03              mov         dword ptr [rbx],eax
  0000000141475479: EB D5              jmp         0000000141475450
  000000014147547B: E8 B4 EC FF FF     call        0000000141474134
  0000000141475480: 48 8B D8           mov         rbx,rax
  0000000141475483: FF 15 5F E1 98 00  call        qword ptr [0000000141E035E8h]
  0000000141475489: 8B C8              mov         ecx,eax
  000000014147548B: E8 5C EC FF FF     call        00000001414740EC
  0000000141475490: 89 03              mov         dword ptr [rbx],eax
  0000000141475492: 48 8B C6           mov         rax,rsi
  0000000141475495: EB BB              jmp         0000000141475452

  Summary

     1E02000 .text
