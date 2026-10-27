Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054E90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054E95: 48 83 EC 78        sub         rsp,78h
  0000000140054E99: 48 8D 0D 18 18 D2  lea         rcx,[0000000143D766B8h]
                    03
  0000000140054EA0: E8 EB 31 27 00     call        00000001402C8090
  0000000140054EA5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140054EAA: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140054EB2: 4C 8D 0D 9F 43 51  lea         r9,[0000000143569258h]
                    03
  0000000140054EB9: 4C 8D 05 C0 43 51  lea         r8,[0000000143569280h]
                    03
  0000000140054EC0: 33 D2              xor         edx,edx
  0000000140054EC2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140054EC7: E8 30 AA 41 01     call        000000014146F8FC
  0000000140054ECC: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140054ED1: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140054ED7: 75 04              jne         0000000140054EDD
  0000000140054ED9: 33 C0              xor         eax,eax
  0000000140054EDB: EB 55              jmp         0000000140054F32
  0000000140054EDD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140054EE2: E8 B9 E6 FB FF     call        00000001400135A0
  0000000140054EE7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140054EEC: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140054EF2: 75 04              jne         0000000140054EF8
  0000000140054EF4: 33 C0              xor         eax,eax
  0000000140054EF6: EB 3A              jmp         0000000140054F32
  0000000140054EF8: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000140054EFD: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140054F02: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140054F07: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140054F0F: E8 EC BD FF FF     call        0000000140050D00
  0000000140054F14: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140054F19: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140054F1E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140054F23: E8 28 B7 01 00     call        0000000140070650
  0000000140054F28: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140054F2D: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140054F32: 48 83 C4 78        add         rsp,78h
  0000000140054F36: C3                 ret

  Summary

     1E02000 .text
