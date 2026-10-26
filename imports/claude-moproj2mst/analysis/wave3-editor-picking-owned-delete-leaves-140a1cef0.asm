Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140A1CEF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140A1CEF5: 48 83 EC 58        sub         rsp,58h
  0000000140A1CEF9: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140A1CEFE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140A1CF01: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140A1CF06: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140A1CF0C: 74 5D              je          0000000140A1CF6B
  0000000140A1CF0E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140A1CF13: 48 8B 80 40 08 00  mov         rax,qword ptr [rax+0000000000000840h]
                    00
  0000000140A1CF1A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140A1CF1F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140A1CF24: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140A1CF29: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140A1CF2E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140A1CF33: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140A1CF39: 74 1B              je          0000000140A1CF56
  0000000140A1CF3B: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140A1CF40: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140A1CF43: BA 01 00 00 00     mov         edx,1
  0000000140A1CF48: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140A1CF4D: FF 10              call        qword ptr [rax]
  0000000140A1CF4F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140A1CF54: EB 09              jmp         0000000140A1CF5F
  0000000140A1CF56: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140A1CF5F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140A1CF64: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140A1CF69: EB 9B              jmp         0000000140A1CF06
  0000000140A1CF6B: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140A1CF70: 48 C7 40 08 00 00  mov         qword ptr [rax+8],0
                    00 00
  0000000140A1CF78: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140A1CF7D: 48 C7 00 00 00 00  mov         qword ptr [rax],0
                    00
  0000000140A1CF84: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140A1CF89: C7 40 14 00 00 00  mov         dword ptr [rax+14h],0
                    00
  0000000140A1CF90: 48 83 C4 58        add         rsp,58h
  0000000140A1CF94: C3                 ret

  Summary

     1E02000 .text
