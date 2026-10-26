Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409C83B0: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  00000001409C83B4: 48 83 EC 38        sub         rsp,38h
  00000001409C83B8: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001409C83BD: 75 04              jne         00000001409C83C3
  00000001409C83BF: 33 C0              xor         eax,eax
  00000001409C83C1: EB 40              jmp         00000001409C8403
  00000001409C83C3: 48 63 4C 24 40     movsxd      rcx,dword ptr [rsp+40h]
  00000001409C83C8: BA 08 00 00 00     mov         edx,8
  00000001409C83CD: E8 96 AF AA 00     call        0000000141473368
  00000001409C83D2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001409C83D7: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001409C83DD: 75 1F              jne         00000001409C83FE
  00000001409C83DF: 44 8B 4C 24 40     mov         r9d,dword ptr [rsp+40h]
  00000001409C83E4: 4C 8D 05 11 31 EB  lea         r8,[000000014387B4FCh]
                    02
  00000001409C83EB: 48 8D 15 16 31 EB  lea         rdx,[000000014387B508h]
                    02
  00000001409C83F2: 48 8D 0D 1F 31 EB  lea         rcx,[000000014387B518h]
                    02
  00000001409C83F9: E8 22 75 16 00     call        0000000140B2F920
  00000001409C83FE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001409C8403: 48 83 C4 38        add         rsp,38h
  00000001409C8407: C3                 ret

  Summary

     1E02000 .text
