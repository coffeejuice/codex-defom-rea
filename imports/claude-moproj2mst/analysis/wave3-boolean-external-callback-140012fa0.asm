Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140012FA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140012FA5: 48 83 EC 28        sub         rsp,28h
  0000000140012FA9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140012FAE: 48 83 B8 C8 02 00  cmp         qword ptr [rax+00000000000002C8h],0
                    00 00
  0000000140012FB6: 75 0A              jne         0000000140012FC2
  0000000140012FB8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140012FBD: E8 1E 00 00 00     call        0000000140012FE0
  0000000140012FC2: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140012FC7: 48 8B 80 C8 02 00  mov         rax,qword ptr [rax+00000000000002C8h]
                    00
  0000000140012FCE: 48 83 C4 28        add         rsp,28h
  0000000140012FD2: C3                 ret

  Summary

     1E02000 .text
