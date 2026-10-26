Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141486264: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141486269: 57                 push        rdi
  000000014148626A: 48 83 EC 30        sub         rsp,30h
  000000014148626E: 49 8B D8           mov         rbx,r8
  0000000141486271: 48 8B FA           mov         rdi,rdx
  0000000141486274: 48 85 C9           test        rcx,rcx
  0000000141486277: 74 32              je          00000001414862AB
  0000000141486279: 33 D2              xor         edx,edx
  000000014148627B: 48 8D 42 E0        lea         rax,[rdx-20h]
  000000014148627F: 48 F7 F1           div         rax,rcx
  0000000141486282: 48 3B C7           cmp         rax,rdi
  0000000141486285: 73 24              jae         00000001414862AB
  0000000141486287: E8 A8 DE FE FF     call        0000000141474134
  000000014148628C: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141486292: 45 33 C9           xor         r9d,r9d
  0000000141486295: 45 33 C0           xor         r8d,r8d
  0000000141486298: 33 D2              xor         edx,edx
  000000014148629A: 33 C9              xor         ecx,ecx
  000000014148629C: C7 00 0C 00 00 00  mov         dword ptr [rax],0Ch
  00000001414862A2: E8 C1 A2 FE FF     call        0000000141470568
  00000001414862A7: 33 C0              xor         eax,eax
  00000001414862A9: EB 5D              jmp         0000000141486308
  00000001414862AB: 48 0F AF F9        imul        rdi,rcx
  00000001414862AF: B8 01 00 00 00     mov         eax,1
  00000001414862B4: 48 85 FF           test        rdi,rdi
  00000001414862B7: 48 0F 44 F8        cmove       rdi,rax
  00000001414862BB: 33 C0              xor         eax,eax
  00000001414862BD: 48 83 FF E0        cmp         rdi,0FFFFFFFFFFFFFFE0h
  00000001414862C1: 77 18              ja          00000001414862DB
  00000001414862C3: 48 8B 0D 46 A5 27  mov         rcx,qword ptr [0000000144700810h]
                    03
  00000001414862CA: 8D 50 08           lea         edx,[rax+8]
  00000001414862CD: 4C 8B C7           mov         r8,rdi
  00000001414862D0: FF 15 AA D0 97 00  call        qword ptr [0000000141E03380h]
  00000001414862D6: 48 85 C0           test        rax,rax
  00000001414862D9: 75 2D              jne         0000000141486308
  00000001414862DB: 83 3D 76 A5 27 03  cmp         dword ptr [0000000144700858h],0
                    00
  00000001414862E2: 74 19              je          00000001414862FD
  00000001414862E4: 48 8B CF           mov         rcx,rdi
  00000001414862E7: E8 FC B9 FF FF     call        0000000141481CE8
  00000001414862EC: 85 C0              test        eax,eax
  00000001414862EE: 75 CB              jne         00000001414862BB
  00000001414862F0: 48 85 DB           test        rbx,rbx
  00000001414862F3: 74 B2              je          00000001414862A7
  00000001414862F5: C7 03 0C 00 00 00  mov         dword ptr [rbx],0Ch
  00000001414862FB: EB AA              jmp         00000001414862A7
  00000001414862FD: 48 85 DB           test        rbx,rbx
  0000000141486300: 74 06              je          0000000141486308
  0000000141486302: C7 03 0C 00 00 00  mov         dword ptr [rbx],0Ch
  0000000141486308: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000014148630D: 48 83 C4 30        add         rsp,30h
  0000000141486311: 5F                 pop         rdi
  0000000141486312: C3                 ret

  Summary

     1E02000 .text
