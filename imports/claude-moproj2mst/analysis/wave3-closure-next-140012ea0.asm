Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140012EA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140012EA5: 48 83 EC 48        sub         rsp,48h
  0000000140012EA9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140012EB2: B9 38 00 00 00     mov         ecx,38h
  0000000140012EB7: E8 00 BC 45 01     call        000000014146EABC
  0000000140012EBC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140012EC1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140012EC7: 74 19              je          0000000140012EE2
  0000000140012EC9: 45 33 C0           xor         r8d,r8d
  0000000140012ECC: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140012ED1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140012ED6: E8 25 65 0D 00     call        00000001400E9400
  0000000140012EDB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140012EE0: EB 09              jmp         0000000140012EEB
  0000000140012EE2: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140012EEB: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140012EF0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140012EF5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140012EFA: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140012EFF: 48 89 81 50 02 00  mov         qword ptr [rcx+0000000000000250h],rax
                    00
  0000000140012F06: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140012F0B: E8 60 7A 00 00     call        000000014001A970
  0000000140012F10: 48 83 C4 48        add         rsp,48h
  0000000140012F14: C3                 ret

  Summary

     1E02000 .text
