Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140004A20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140004A25: 48 83 EC 18        sub         rsp,18h
  0000000140004A29: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140004A2E: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140004A31: 85 C0              test        eax,eax
  0000000140004A33: 74 0B              je          0000000140004A40
  0000000140004A35: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140004A3A: 48 89 04 24        mov         qword ptr [rsp],rax
  0000000140004A3E: EB 08              jmp         0000000140004A48
  0000000140004A40: 48 C7 04 24 00 00  mov         qword ptr [rsp],0
                    00 00
  0000000140004A48: 48 8B 04 24        mov         rax,qword ptr [rsp]
  0000000140004A4C: 48 83 C4 18        add         rsp,18h
  0000000140004A50: C3                 ret

  Summary

     1E02000 .text
