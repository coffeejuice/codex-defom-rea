Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140834FA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140834FA5: 48 83 EC 18        sub         rsp,18h
  0000000140834FA9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140834FAE: 83 78 0C 07        cmp         dword ptr [rax+0Ch],7
  0000000140834FB2: 74 09              je          0000000140834FBD
  0000000140834FB4: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  0000000140834FBB: EB 07              jmp         0000000140834FC4
  0000000140834FBD: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140834FC4: 8A 04 24           mov         al,byte ptr [rsp]
  0000000140834FC7: 48 83 C4 18        add         rsp,18h
  0000000140834FCB: C3                 ret

  Summary

     1E02000 .text
