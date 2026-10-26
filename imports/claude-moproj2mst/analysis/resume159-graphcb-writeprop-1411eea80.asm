Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411EEA80: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411EEA85: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411EEA8A: 48 83 EC 18        sub         rsp,18h
  00000001411EEA8E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411EEA93: 48 63 08           movsxd      rcx,dword ptr [rax]
  00000001411EEA96: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411EEA9B: 0F B6 44 08 08     movzx       eax,byte ptr [rax+rcx+8]
  00000001411EEAA0: 88 04 24           mov         byte ptr [rsp],al
  00000001411EEAA3: 0F B6 04 24        movzx       eax,byte ptr [rsp]
  00000001411EEAA7: 85 C0              test        eax,eax
  00000001411EEAA9: 74 13              je          00000001411EEABE
  00000001411EEAAB: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411EEAB0: 83 38 00           cmp         dword ptr [rax],0
  00000001411EEAB3: 75 09              jne         00000001411EEABE
  00000001411EEAB5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411EEABA: C6 40 09 01        mov         byte ptr [rax+9],1
  00000001411EEABE: 0F B6 04 24        movzx       eax,byte ptr [rsp]
  00000001411EEAC2: 85 C0              test        eax,eax
  00000001411EEAC4: 75 13              jne         00000001411EEAD9
  00000001411EEAC6: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411EEACB: 83 38 01           cmp         dword ptr [rax],1
  00000001411EEACE: 75 09              jne         00000001411EEAD9
  00000001411EEAD0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411EEAD5: C6 40 08 00        mov         byte ptr [rax+8],0
  00000001411EEAD9: 48 83 C4 18        add         rsp,18h
  00000001411EEADD: C3                 ret

  Summary

     1E02000 .text
