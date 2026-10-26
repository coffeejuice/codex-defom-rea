Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670534B0: 00 85 C9 74 06 48  add         byte ptr [rbp+00000000480674C9h],al
  00000000670534B6: F7 D6              not         esi
  00000000670534B8: 48 FF C6           inc         rsi
  00000000670534BB: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670534C3: 4C 8B 74 24 38     mov         r14,qword ptr [rsp+38h]
  00000000670534C8: 4C 8B 6C 24 40     mov         r13,qword ptr [rsp+40h]
  00000000670534CD: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670534D2: 48 85 C0           test        rax,rax
  00000000670534D5: 74 14              je          00000000670534EB
  00000000670534D7: 85 ED              test        ebp,ebp
  00000000670534D9: 78 05              js          00000000670534E0
  00000000670534DB: 48 FF CF           dec         rdi
  00000000670534DE: EB 08              jmp         00000000670534E8
  00000000670534E0: 48 8B BC 24 80 00  mov         rdi,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670534E8: 48 89 38           mov         qword ptr [rax],rdi
  00000000670534EB: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  00000000670534F0: 4D 85 FF           test        r15,r15
  00000000670534F3: 74 08              je          00000000670534FD
  00000000670534F5: 85 ED              test        ebp,ebp
  00000000670534F7: 0F 9F C1           setg        cl
  00000000670534FA: 41 88 0F           mov         byte ptr [r15],cl
  00000000670534FD: 48 8B C6           mov         rax,rsi
  0000000067053500: 48 83 C4 60        add         rsp,60h
  0000000067053504: 41 5F              pop         r15
  0000000067053506: 5E                 pop         rsi
  0000000067053507: 5D                 pop         rbp
  0000000067053508: C3                 ret
  0000000067053509: CC                 int         3
  000000006705350A: CC                 int         3
  000000006705350B: CC                 int         3
  000000006705350C: CC                 int         3
  000000006705350D: CC                 int         3
  000000006705350E: CC                 int         3
  000000006705350F: CC                 int         3
  0000000067053510: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067053515: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006705351A: 57                 push        rdi
  000000006705351B: 48 83 EC 20        sub         rsp,20h
  000000006705351F: 8B F1              mov         esi,ecx
  0000000067053521: BF 01 00 00 00     mov         edi,1
  0000000067053526: D3 E7              shl         edi,cl
  0000000067053528: 8D 47 FF           lea         eax,[rdi-1]
  000000006705352B: 48 63 C8           movsxd      rcx,eax
  000000006705352E: 48 8D 0C 8D 20 00  lea         rcx,[rcx*4+0000000000000020h]
                    00 00
  0000000067053536: FF 15 D4 50 15 00  call        qword ptr [00000000671A8610h]
  000000006705353C: 48 8B D8           mov         rbx,rax
  000000006705353F: 48
  0000000067053540: 85

  Summary

      1A7000 .text
