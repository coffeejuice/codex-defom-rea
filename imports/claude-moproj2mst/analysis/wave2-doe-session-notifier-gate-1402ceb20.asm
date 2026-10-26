Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CEB20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CEB25: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CEB2A: C6 40 58 00        mov         byte ptr [rax+58h],0
  00000001402CEB2E: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CEB33: 48 C7 40 60 00 00  mov         qword ptr [rax+60h],0
                    00 00
  00000001402CEB3B: C3                 ret
  00000001402CEB3C: CC                 int         3
  00000001402CEB3D: CC                 int         3
  00000001402CEB3E: CC                 int         3
  00000001402CEB3F: CC                 int         3
  00000001402CEB40: CC                 int         3
  00000001402CEB41: CC                 int         3
  00000001402CEB42: CC                 int         3
  00000001402CEB43: CC                 int         3
  00000001402CEB44: CC                 int         3
  00000001402CEB45: CC                 int         3
  00000001402CEB46: CC                 int         3
  00000001402CEB47: CC                 int         3
  00000001402CEB48: CC                 int         3
  00000001402CEB49: CC                 int         3
  00000001402CEB4A: CC                 int         3
  00000001402CEB4B: CC                 int         3
  00000001402CEB4C: CC                 int         3
  00000001402CEB4D: CC                 int         3
  00000001402CEB4E: CC                 int         3
  00000001402CEB4F: CC                 int         3
  00000001402CEB50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CEB55: 48 83 EC 28        sub         rsp,28h
  00000001402CEB59: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CEB5E: 0F B6 40 58        movzx       eax,byte ptr [rax+58h]
  00000001402CEB62: 85 C0              test        eax,eax
  00000001402CEB64: 75 02              jne         00000001402CEB68
  00000001402CEB66: EB 23              jmp         00000001402CEB8B
  00000001402CEB68: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CEB6D: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CEB74: 85 C0              test        eax,eax
  00000001402CEB76: 75 13              jne         00000001402CEB8B
  00000001402CEB78: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001402CEB7D: 48
  00000001402CEB7E: 8B
  00000001402CEB7F: 52                 push        rdx

  Summary

     1E02000 .text
