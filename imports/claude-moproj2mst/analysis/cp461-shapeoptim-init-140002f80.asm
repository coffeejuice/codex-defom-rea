Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140002F80: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140002F84: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140002F89: 48 83 EC 28        sub         rsp,28h
  0000000140002F8D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140002F92: FF 15 48 2A E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002F98: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140002F9C: 83 E0 01           and         eax,1
  0000000140002F9F: 85 C0              test        eax,eax
  0000000140002FA1: 74 0A              je          0000000140002FAD
  0000000140002FA3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140002FA8: E8 BB CC 4D 01     call        00000001414DFC68
  0000000140002FAD: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140002FB2: 48 83 C4 28        add         rsp,28h
  0000000140002FB6: C3                 ret

  Summary

     1E02000 .text
