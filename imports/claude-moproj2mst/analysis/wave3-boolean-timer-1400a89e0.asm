Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A89E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A89E5: 48 83 EC 58        sub         rsp,58h
  00000001400A89E9: 48 8D 0D C8 DC CC  lea         rcx,[0000000143D766B8h]
                    03
  00000001400A89F0: E8 9B F6 21 00     call        00000001402C8090
  00000001400A89F5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400A89FA: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400A8A02: 4C 8D 0D 4F 08 4C  lea         r9,[0000000143569258h]
                    03
  00000001400A8A09: 4C 8D 05 70 08 4C  lea         r8,[0000000143569280h]
                    03
  00000001400A8A10: 33 D2              xor         edx,edx
  00000001400A8A12: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400A8A17: E8 E0 6E 3C 01     call        000000014146F8FC
  00000001400A8A1C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400A8A21: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400A8A27: 75 04              jne         00000001400A8A2D
  00000001400A8A29: 33 C0              xor         eax,eax
  00000001400A8A2B: EB 14              jmp         00000001400A8A41
  00000001400A8A2D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A8A32: E8 69 A5 F6 FF     call        0000000140012FA0
  00000001400A8A37: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400A8A3C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400A8A41: 48 83 C4 58        add         rsp,58h
  00000001400A8A45: C3                 ret

  Summary

     1E02000 .text
