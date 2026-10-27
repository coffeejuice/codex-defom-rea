Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919660: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140919664: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140919669: 48 83 EC 28        sub         rsp,28h
  000000014091966D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140919672: E8 29 00 00 00     call        00000001409196A0
  0000000140919677: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014091967B: 83 E0 01           and         eax,1
  000000014091967E: 85 C0              test        eax,eax
  0000000140919680: 74 0A              je          000000014091968C
  0000000140919682: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140919687: E8 DC 65 BC 00     call        00000001414DFC68
  000000014091968C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140919691: 48 83 C4 28        add         rsp,28h
  0000000140919695: C3                 ret

  Summary

     1E02000 .text
