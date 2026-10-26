Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C9BB80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C9BB85: 48 83 EC 38        sub         rsp,38h
  0000000140C9BB89: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C9BB8E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9BB91: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C9BB96: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140C9BB9C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C9BBA1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C9BBA7: 74 18              je          0000000140C9BBC1
  0000000140C9BBA9: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C9BBAE: 48 83 C1 10        add         rcx,10h
  0000000140C9BBB2: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C9BBB7: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C9BBBB: FF 90 F8 01 00 00  call        qword ptr [rax+00000000000001F8h]
  0000000140C9BBC1: 48 83 C4 38        add         rsp,38h
  0000000140C9BBC5: C3                 ret

  Summary

     1E02000 .text
