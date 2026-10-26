Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711A200: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006711A205: 57                 push        rdi
  000000006711A206: 48 83 EC 30        sub         rsp,30h
  000000006711A20A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711A213: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006711A218: 48 8B FA           mov         rdi,rdx
  000000006711A21B: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711A21E: 44 8B 00           mov         r8d,dword ptr [rax]
  000000006711A221: 41 83 F8 01        cmp         r8d,1
  000000006711A225: 74 41              je          000000006711A268
  000000006711A227: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  000000006711A22C: 41 B8 01 00 00 00  mov         r8d,1
  000000006711A232: E8 89 EB FF FF     call        0000000067118DC0
  000000006711A237: 48 8B D8           mov         rbx,rax
  000000006711A23A: B9 18 00 00 00     mov         ecx,18h
  000000006711A23F: E8 42 E8 05 00     call        0000000067178A86
  000000006711A244: 48 85 C0           test        rax,rax
  000000006711A247: 74 18              je          000000006711A261
  000000006711A249: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711A24C: 48 89 08           mov         qword ptr [rax],rcx
  000000006711A24F: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006711A253: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000006711A257: 48 8B 4F 10        mov         rcx,qword ptr [rdi+10h]
  000000006711A25B: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006711A25F: EB 02              jmp         000000006711A263
  000000006711A261: 33 C0              xor         eax,eax
  000000006711A263: 48 89 03           mov         qword ptr [rbx],rax
  000000006711A266: EB 34              jmp         000000006711A29C
  000000006711A268: E8 23 E9 F2 FF     call        0000000067048B90
  000000006711A26D: 48 8B D8           mov         rbx,rax
  000000006711A270: B9 18 00 00 00     mov         ecx,18h
  000000006711A275: E8 0C E8 05 00     call        0000000067178A86
  000000006711A27A: 48 85 C0           test        rax,rax
  000000006711A27D: 74 18              je          000000006711A297
  000000006711A27F: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711A282: 48 89 08           mov         qword ptr [rax],rcx
  000000006711A285: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006711A289: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000006711A28D: 48 8B 4F 10        mov         rcx,qword ptr [rdi+10h]
  000000006711A291: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006711A295: EB 02              jmp         000000006711A299
  000000006711A297: 33 C0              xor         eax,eax
  000000006711A299: 48 89 03           mov         qword ptr [rbx],rax
  000000006711A29C: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006711A2A1: 48 83 C4 30        add         rsp,30h
  000000006711A2A5: 5F                 pop         rdi
  000000006711A2A6: C3                 ret

  Summary

      1A7000 .text
