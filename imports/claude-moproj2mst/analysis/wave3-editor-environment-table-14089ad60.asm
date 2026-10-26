Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089AD60: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089AD65: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089AD6A: 48 83 EC 38        sub         rsp,38h
  000000014089AD6E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD73: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AD78: 8B 00              mov         eax,dword ptr [rax]
  000000014089AD7A: 89 01              mov         dword ptr [rcx],eax
  000000014089AD7C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AD81: 81 38 80 00 00 00  cmp         dword ptr [rax],80h
  000000014089AD87: 75 10              jne         000000014089AD99
  000000014089AD89: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD8E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AD93: 8B 40 58           mov         eax,dword ptr [rax+58h]
  000000014089AD96: 89 41 58           mov         dword ptr [rcx+58h],eax
  000000014089AD99: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AD9E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADA3: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  000000014089ADA8: F2 0F 11 41 08     movsd       mmword ptr [rcx+8],xmm0
  000000014089ADAD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADB2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADB7: 8B 40 54           mov         eax,dword ptr [rax+54h]
  000000014089ADBA: 89 41 54           mov         dword ptr [rcx+54h],eax
  000000014089ADBD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADC2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADC7: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089ADCA: 89 41 04           mov         dword ptr [rcx+4],eax
  000000014089ADCD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADD2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADD7: 8B 40 40           mov         eax,dword ptr [rax+40h]
  000000014089ADDA: 89 41 40           mov         dword ptr [rcx+40h],eax
  000000014089ADDD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089ADE2: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089ADE7: 8B 40 50           mov         eax,dword ptr [rax+50h]
  000000014089ADEA: 89 41 50           mov         dword ptr [rcx+50h],eax
  000000014089ADED: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014089ADF5: EB 0B              jmp         000000014089AE02
  000000014089ADF7: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014089ADFB: 83 C0 01           add         eax,1
  000000014089ADFE: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089AE02: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  000000014089AE07: 7D 3C              jge         000000014089AE45
  000000014089AE09: 4C 63 44 24 20     movsxd      r8,dword ptr [rsp+20h]
  000000014089AE0E: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  000000014089AE13: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE18: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE1D: 42 8B 44 80 10     mov         eax,dword ptr [rax+r8*4+10h]
  000000014089AE22: 89 44 91 10        mov         dword ptr [rcx+rdx*4+10h],eax
  000000014089AE26: 4C 63 44 24 20     movsxd      r8,dword ptr [rsp+20h]
  000000014089AE2B: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  000000014089AE30: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE35: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE3A: 42 8B 44 80 1C     mov         eax,dword ptr [rax+r8*4+1Ch]
  000000014089AE3F: 89 44 91 1C        mov         dword ptr [rcx+rdx*4+1Ch],eax
  000000014089AE43: EB B2              jmp         000000014089ADF7
  000000014089AE45: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AE4A: E8 F1 09 00 00     call        000000014089B840
  000000014089AE4F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014089AE57: EB 0B              jmp         000000014089AE64
  000000014089AE59: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014089AE5D: 83 C0 01           add         eax,1
  000000014089AE60: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089AE64: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AE69: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089AE6C: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014089AE70: 7D 5E              jge         000000014089AED0
  000000014089AE72: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE77: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AE7C: 48 83 7C C8 28 00  cmp         qword ptr [rax+rcx*8+28h],0
  000000014089AE82: 74 12              je          000000014089AE96
  000000014089AE84: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE89: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AE8E: 48 83 7C C8 28 00  cmp         qword ptr [rax+rcx*8+28h],0
  000000014089AE94: 75 02              jne         000000014089AE98
  000000014089AE96: EB 38              jmp         000000014089AED0
  000000014089AE98: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AE9D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AEA2: 4C 63 44 88 1C     movsxd      r8,dword ptr [rax+rcx*4+1Ch]
  000000014089AEA7: 49 C1 E0 03        shl         r8,3
  000000014089AEAB: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089AEB0: 48 63 44 24 20     movsxd      rax,dword ptr [rsp+20h]
  000000014089AEB5: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  000000014089AEBA: 48 8B 54 CA 28     mov         rdx,qword ptr [rdx+rcx*8+28h]
  000000014089AEBF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AEC4: 48 8B 4C C1 28     mov         rcx,qword ptr [rcx+rax*8+28h]
  000000014089AEC9: E8 32 42 BD 00     call        000000014146F100
  000000014089AECE: EB 89              jmp         000000014089AE59
  000000014089AED0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089AED5: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089AEDA: 74 0C              je          000000014089AEE8
  000000014089AEDC: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014089AEE1: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089AEE6: 75 02              jne         000000014089AEEA
  000000014089AEE8: EB 28              jmp         000000014089AF12
  000000014089AEEA: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AEEF: E8 9C 0D 00 00     call        000000014089BC90
  000000014089AEF4: 4C 63 C0           movsxd      r8,eax
  000000014089AEF7: 49 C1 E0 03        shl         r8,3
  000000014089AEFB: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  000000014089AF00: 48 8B 52 48        mov         rdx,qword ptr [rdx+48h]
  000000014089AF04: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089AF09: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014089AF0D: E8 EE 41 BD 00     call        000000014146F100
  000000014089AF12: 48 83 C4 38        add         rsp,38h
  000000014089AF16: C3                 ret

  Summary

     1E02000 .text
