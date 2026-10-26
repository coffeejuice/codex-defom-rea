Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CEC30: 48 83 EC 28        sub         rsp,28h
  00000000650CEC34: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000000650CEC38: 48 85 C9           test        rcx,rcx
  00000000650CEC3B: 74 11              je          00000000650CEC4E
  00000000650CEC3D: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650CEC40: FF 50 68           call        qword ptr [rax+68h]
  00000000650CEC43: 84 C0              test        al,al
  00000000650CEC45: 74 07              je          00000000650CEC4E
  00000000650CEC47: B0 01              mov         al,1
  00000000650CEC49: 48 83 C4 28        add         rsp,28h
  00000000650CEC4D: C3                 ret
  00000000650CEC4E: 32 C0              xor         al,al
  00000000650CEC50: 48 83 C4 28        add         rsp,28h
  00000000650CEC54: C3                 ret

  Summary

      695000 .text
