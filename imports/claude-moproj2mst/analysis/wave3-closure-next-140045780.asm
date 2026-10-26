Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140045780: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140045785: 48 83 EC 48        sub         rsp,48h
  0000000140045789: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014004578E: E8 3D 33 C3 00     call        0000000140C78AD0
  0000000140045793: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140045798: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014004579E: 75 04              jne         00000001400457A4
  00000001400457A0: 33 C0              xor         eax,eax
  00000001400457A2: EB 3D              jmp         00000001400457E1
  00000001400457A4: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400457AC: 4C 8D 0D BD 3C 52  lea         r9,[0000000143569470h]
                    03
  00000001400457B3: 4C 8D 05 8E 3C 52  lea         r8,[0000000143569448h]
                    03
  00000001400457BA: 33 D2              xor         edx,edx
  00000001400457BC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400457C1: E8 36 A1 42 01     call        000000014146F8FC
  00000001400457C6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400457CB: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400457D1: 75 04              jne         00000001400457D7
  00000001400457D3: 33 C0              xor         eax,eax
  00000001400457D5: EB 0A              jmp         00000001400457E1
  00000001400457D7: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400457DC: E8 AF F2 FF FF     call        0000000140044A90
  00000001400457E1: 48 83 C4 48        add         rsp,48h
  00000001400457E5: C3                 ret

  Summary

     1E02000 .text
