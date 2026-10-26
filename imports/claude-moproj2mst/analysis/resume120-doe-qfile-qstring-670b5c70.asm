Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5C70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670B5C75: 57                 push        rdi
  00000000670B5C76: 48 83 EC 30        sub         rsp,30h
  00000000670B5C7A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B5C83: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670B5C88: 48 8B FA           mov         rdi,rdx
  00000000670B5C8B: 48 8B D9           mov         rbx,rcx
  00000000670B5C8E: B9 30 01 00 00     mov         ecx,130h
  00000000670B5C93: E8 EE 2D 0C 00     call        0000000067178A86
  00000000670B5C98: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000670B5C9D: 48 85 C0           test        rax,rax
  00000000670B5CA0: 74 0A              je          00000000670B5CAC
  00000000670B5CA2: 48 8B C8           mov         rcx,rax
  00000000670B5CA5: E8 86 FE FF FF     call        00000000670B5B30
  00000000670B5CAA: EB 02              jmp         00000000670B5CAE
  00000000670B5CAC: 33 C0              xor         eax,eax
  00000000670B5CAE: 45 33 C0           xor         r8d,r8d
  00000000670B5CB1: 48 8B D0           mov         rdx,rax
  00000000670B5CB4: 48 8B CB           mov         rcx,rbx
  00000000670B5CB7: E8 C4 42 00 00     call        00000000670B9F80
  00000000670B5CBC: 90                 nop
  00000000670B5CBD: 48 8D 05 54 40 17  lea         rax,[0000000067229D18h]
                    00
  00000000670B5CC4: 48 89 03           mov         qword ptr [rbx],rax
  00000000670B5CC7: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000670B5CCB: 48 81 C1 E8 00 00  add         rcx,0E8h
                    00
  00000000670B5CD2: 48 8B D7           mov         rdx,rdi
  00000000670B5CD5: E8 F6 A6 FB FF     call        00000000670703D0
  00000000670B5CDA: 90                 nop
  00000000670B5CDB: 48 8B C3           mov         rax,rbx
  00000000670B5CDE: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670B5CE3: 48 83 C4 30        add         rsp,30h
  00000000670B5CE7: 5F                 pop         rdi
  00000000670B5CE8: C3                 ret

  Summary

      1A7000 .text
