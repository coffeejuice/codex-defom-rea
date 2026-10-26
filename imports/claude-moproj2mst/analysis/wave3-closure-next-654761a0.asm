Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654761A0: 40 57              push        rdi
  00000000654761A2: 48 83 EC 40        sub         rsp,40h
  00000000654761A6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654761AF: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000654761B4: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000654761B9: 8B F2              mov         esi,edx
  00000000654761BB: 48 8B D9           mov         rbx,rcx
  00000000654761BE: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000654761C2: 4C 8B C1           mov         r8,rcx
  00000000654761C5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000654761CA: E8 E1 DF FF FF     call        00000000654741B0
  00000000654761CF: 90                 nop
  00000000654761D0: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  00000000654761D4: 48 8B D0           mov         rdx,rax
  00000000654761D7: 48 8B 8B 50 02 00  mov         rcx,qword ptr [rbx+0000000000000250h]
                    00
  00000000654761DE: E8 0D FD F3 FF     call        00000000653B5EF0
  00000000654761E3: 48 8B CB           mov         rcx,rbx
  00000000654761E6: E8 45 E4 FF FF     call        0000000065474630
  00000000654761EB: C7 83 48 02 00 00  mov         dword ptr [rbx+0000000000000248h],0
                    00 00 00 00
  00000000654761F5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000654761FA: E8 21 87 C5 FF     call        00000000650CE920
  00000000654761FF: 89 B7 48 02 00 00  mov         dword ptr [rdi+0000000000000248h],esi
  0000000065476205: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006547620A: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006547620F: 48 83 C4 40        add         rsp,40h
  0000000065476213: 5F                 pop         rdi
  0000000065476214: C3                 ret

  Summary

      695000 .text
