Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC74E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC74E5: 48 83 EC 38        sub         rsp,38h
  0000000140CC74E9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CC74EE: 48 83 C1 08        add         rcx,8
  0000000140CC74F2: E8 59 15 41 FF     call        00000001400D8A50
  0000000140CC74F7: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140CC74FC: 74 19              je          0000000140CC7517
  0000000140CC74FE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CC7503: 48 83 C1 08        add         rcx,8
  0000000140CC7507: E8 44 15 41 FF     call        00000001400D8A50
  0000000140CC750C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140CC7510: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC7515: EB 0C              jmp         0000000140CC7523
  0000000140CC7517: 48 8D 05 52 B1 9E  lea         rax,[00000001446B2670h]
                    03
  0000000140CC751E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC7523: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CC7528: 48 83 C4 38        add         rsp,38h
  0000000140CC752C: C3                 ret

  Summary

     1E02000 .text
