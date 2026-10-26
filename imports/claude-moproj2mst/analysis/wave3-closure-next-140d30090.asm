Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D30090: F2 0F 11 5C 24 20  movsd       mmword ptr [rsp+20h],xmm3
  0000000140D30096: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D3009B: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D300A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D300A5: 48 83 EC 38        sub         rsp,38h
  0000000140D300A9: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D300AE: 8B 10              mov         edx,dword ptr [rax]
  0000000140D300B0: 48 8D 0D A9 62 04  lea         rcx,[0000000143D76360h]
                    03
  0000000140D300B7: E8 C4 79 5A FF     call        00000001402D7A80
  0000000140D300BC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D300C1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140D300C7: 74 24              je          0000000140D300ED
  0000000140D300C9: 4C 8B 44 24 20     mov         r8,qword ptr [rsp+20h]
  0000000140D300CE: 4D 8B 00           mov         r8,qword ptr [r8]
  0000000140D300D1: F2 0F 10 5C 24 60  movsd       xmm3,mmword ptr [rsp+60h]
  0000000140D300D7: F2 0F 10 54 24 58  movsd       xmm2,mmword ptr [rsp+58h]
  0000000140D300DD: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D300E2: 8B 10              mov         edx,dword ptr [rax]
  0000000140D300E4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D300E9: 41 FF 50 28        call        qword ptr [r8+28h]
  0000000140D300ED: 48 83 C4 38        add         rsp,38h
  0000000140D300F1: C3                 ret

  Summary

     1E02000 .text
