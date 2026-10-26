Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014084EAA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014084EAA5: 48 83 EC 58        sub         rsp,58h
  000000014084EAA9: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014084EAB2: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014084EABB: B9 28 3F 00 00     mov         ecx,3F28h
  000000014084EAC0: E8 F7 FF C1 00     call        000000014146EABC
  000000014084EAC5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014084EACA: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014084EAD0: 74 1A              je          000000014084EAEC
  000000014084EAD2: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014084EAD7: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  000000014084EADB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014084EAE0: E8 0B 42 FE FF     call        0000000140832CF0
  000000014084EAE5: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014084EAEA: EB 09              jmp         000000014084EAF5
  000000014084EAEC: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  000000014084EAF5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014084EAFA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014084EAFF: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014084EB04: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014084EB09: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014084EB0E: 83 78 10 00        cmp         dword ptr [rax+10h],0
  000000014084EB12: 75 1D              jne         000000014084EB31
  000000014084EB14: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014084EB19: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014084EB1E: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000014084EB22: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014084EB27: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014084EB2C: 48 89 01           mov         qword ptr [rcx],rax
  000000014084EB2F: EB 23              jmp         000000014084EB54
  000000014084EB31: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014084EB36: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014084EB3A: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014084EB3F: 48 89 81 40 39 00  mov         qword ptr [rcx+0000000000003940h],rax
                    00
  000000014084EB46: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014084EB4B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014084EB50: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000014084EB54: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014084EB59: 8B 48 10           mov         ecx,dword ptr [rax+10h]
  000000014084EB5C: 83 C1 01           add         ecx,1
  000000014084EB5F: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014084EB64: 89 48 10           mov         dword ptr [rax+10h],ecx
  000000014084EB67: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014084EB6C: 48 83 C4 58        add         rsp,58h
  000000014084EB70: C3                 ret

  Summary

     1E02000 .text
