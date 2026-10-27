Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140E23A00: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140E23A05: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140E23A0A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140E23A0F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140E23A14: 48 81 EC 28 02 00  sub         rsp,228h
                    00
  0000000140E23A1B: 48 C7 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140E23A27: 48 C7 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],0
                    00 00 00 00 00 00
  0000000140E23A33: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140E23A3C: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140E23A44: E8 37 8A 3F 00     call        000000014121C480
  0000000140E23A49: 90                 nop
  0000000140E23A4A: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140E23A52: E8 29 8A 3F 00     call        000000014121C480
  0000000140E23A57: 90                 nop
  0000000140E23A58: 48 8B 8C 24 38 02  mov         rcx,qword ptr [rsp+0000000000000238h]
                    00 00
  0000000140E23A60: E8 AB DF 3C 00     call        00000001411F1A10
  0000000140E23A65: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140E23A6D: C7 84 24 F8 01 00  mov         dword ptr [rsp+00000000000001F8h],1
                    00 01 00 00 00
  0000000140E23A78: 48 8D 94 24 F8 01  lea         rdx,[rsp+00000000000001F8h]
                    00 00
  0000000140E23A80: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140E23A88: E8 B3 AF 3C 00     call        00000001411EEA40
  0000000140E23A8D: 0F B6 C0           movzx       eax,al
  0000000140E23A90: 85 C0              test        eax,eax
  0000000140E23A92: 74 4E              je          0000000140E23AE2
  0000000140E23A94: C7 84 24 FC 01 00  mov         dword ptr [rsp+00000000000001FCh],2
                    00 02 00 00 00
  0000000140E23A9F: 48 8D 94 24 FC 01  lea         rdx,[rsp+00000000000001FCh]
                    00 00
  0000000140E23AA7: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140E23AAF: E8 8C AF 3C 00     call        00000001411EEA40
  0000000140E23AB4: 0F B6 C0           movzx       eax,al
  0000000140E23AB7: 85 C0              test        eax,eax
  0000000140E23AB9: 75 47              jne         0000000140E23B02
  0000000140E23ABB: C7 84 24 00 02 00  mov         dword ptr [rsp+0000000000000200h],3
                    00 03 00 00 00
  0000000140E23AC6: 48 8D 94 24 00 02  lea         rdx,[rsp+0000000000000200h]
                    00 00
  0000000140E23ACE: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140E23AD6: E8 65 AF 3C 00     call        00000001411EEA40
  0000000140E23ADB: 0F B6 C0           movzx       eax,al
  0000000140E23ADE: 85 C0              test        eax,eax
  0000000140E23AE0: 75 20              jne         0000000140E23B02
  0000000140E23AE2: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140E23AEA: E8 81 8A 3F 00     call        000000014121C570
  0000000140E23AEF: 90                 nop
  0000000140E23AF0: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140E23AF8: E8 73 8A 3F 00     call        000000014121C570
  0000000140E23AFD: E9 6C 02 00 00     jmp         0000000140E23D6E
  0000000140E23B02: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  0000000140E23B0A: 48 05 48 03 00 00  add         rax,348h
  0000000140E23B10: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140E23B15: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140E23B1A: E8 01 59 42 00     call        0000000141249420
  0000000140E23B1F: 44 8B D8           mov         r11d,eax
  0000000140E23B22: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140E23B2A: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140E23B32: E8 E9 21 B2 FF     call        0000000140945D20
  0000000140E23B37: 4C 8B 84 24 50 02  mov         r8,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140E23B3F: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140E23B47: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140E23B4F: E8 4C 31 B2 FF     call        0000000140946CA0
  0000000140E23B54: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140E23B5C: 83 38 00           cmp         dword ptr [rax],0
  0000000140E23B5F: 75 0D              jne         0000000140E23B6E
  0000000140E23B61: C6 84 24 68 01 00  mov         byte ptr [rsp+0000000000000168h],0
                    00 00
  0000000140E23B69: E9 5A 01 00 00     jmp         0000000140E23CC8
  0000000140E23B6E: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140E23B76: 8B 00              mov         eax,dword ptr [rax]
  0000000140E23B78: 83 E8 01           sub         eax,1
  0000000140E23B7B: 48 63 C8           movsxd      rcx,eax
  0000000140E23B7E: 48 69 C9 08 01 00  imul        rcx,rcx,108h
                    00
  0000000140E23B85: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140E23B8A: 48 03 C1           add         rax,rcx
  0000000140E23B8D: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140E23B95: 48 83 BC 24 B0 01  cmp         qword ptr [rsp+00000000000001B0h],0
                    00 00 00
  0000000140E23B9E: 75 20              jne         0000000140E23BC0
  0000000140E23BA0: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140E23BA8: E8 C3 89 3F 00     call        000000014121C570
  0000000140E23BAD: 90                 nop
  0000000140E23BAE: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140E23BB6: E8 B5 89 3F 00     call        000000014121C570
  0000000140E23BBB: E9 AE 01 00 00     jmp         0000000140E23D6E
  0000000140E23BC0: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140E23BC4: F2 0F 11 84 24 E0  movsd       mmword ptr [rsp+00000000000001E0h],xmm0
                    01 00 00
  0000000140E23BCD: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140E23BD1: F2 0F 11 84 24 E8  movsd       mmword ptr [rsp+00000000000001E8h],xmm0
                    01 00 00
  0000000140E23BDA: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140E23BDE: F2 0F 11 84 24 F0  movsd       mmword ptr [rsp+00000000000001F0h],xmm0
                    01 00 00
  0000000140E23BE7: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  0000000140E23BEF: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140E23BF7: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140E23BFF: E8 0C 11 B2 FF     call        0000000140944D10
  0000000140E23C04: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140E23C0C: 48 83 C1 40        add         rcx,40h
  0000000140E23C10: 48 8D 94 24 E0 01  lea         rdx,[rsp+00000000000001E0h]
                    00 00
  0000000140E23C18: E8 A3 28 B2 FF     call        00000001409464C0
  0000000140E23C1D: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140E23C25: 48 83 C1 40        add         rcx,40h
  0000000140E23C29: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140E23C31: E8 8A 28 B2 FF     call        00000001409464C0
  0000000140E23C36: 4C 8B 84 24 48 02  mov         r8,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140E23C3E: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140E23C46: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  0000000140E23C4E: E8 CD 12 B2 FF     call        0000000140944F20
  0000000140E23C53: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140E23C5B: E8 30 42 42 00     call        0000000141247E90
  0000000140E23C60: 0F B6 C0           movzx       eax,al
  0000000140E23C63: 85 C0              test        eax,eax
  0000000140E23C65: 75 0D              jne         0000000140E23C74
  0000000140E23C67: C7 84 24 10 02 00  mov         dword ptr [rsp+0000000000000210h],1
                    00 01 00 00 00
  0000000140E23C72: EB 0B              jmp         0000000140E23C7F
  0000000140E23C74: C7 84 24 10 02 00  mov         dword ptr [rsp+0000000000000210h],0
                    00 00 00 00 00
  0000000140E23C7F: 0F B6 84 24 10 02  movzx       eax,byte ptr [rsp+0000000000000210h]
                    00 00
  0000000140E23C87: 88 84 24 68 01 00  mov         byte ptr [rsp+0000000000000168h],al
                    00
  0000000140E23C8E: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140E23C96: 48 83 C2 40        add         rdx,40h
  0000000140E23C9A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140E23C9F: E8 CC 21 B2 FF     call        0000000140945E70
  0000000140E23CA4: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000140E23CAC: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140E23CB1: E8 8A 23 B2 FF     call        0000000140946040
  0000000140E23CB6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140E23CBB: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140E23CC3: E8 A8 21 B2 FF     call        0000000140945E70
  0000000140E23CC8: 48 8D 84 24 C8 00  lea         rax,[rsp+00000000000000C8h]
                    00 00
  0000000140E23CD0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140E23CD5: 4C 8D 8C 24 70 01  lea         r9,[rsp+0000000000000170h]
                    00 00
  0000000140E23CDD: 44 0F B6 84 24 68  movzx       r8d,byte ptr [rsp+0000000000000168h]
                    01 00 00
  0000000140E23CE6: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000140E23CEE: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140E23CF3: E8 38 5A 42 00     call        0000000141249730
  0000000140E23CF8: 89 84 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],eax
                    00
  0000000140E23CFF: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  0000000140E23D07: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140E23D0C: E8 AF 57 42 00     call        00000001412494C0
  0000000140E23D11: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140E23D16: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140E23D1C: 74 1A              je          0000000140E23D38
  0000000140E23D1E: 4C 8B 84 24 50 02  mov         r8,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140E23D26: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140E23D2E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140E23D33: E8 68 3E 42 00     call        0000000141247BA0
  0000000140E23D38: 4C 8D 84 24 C0 00  lea         r8,[rsp+00000000000000C0h]
                    00 00
  0000000140E23D40: 48 8B 94 24 40 02  mov         rdx,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140E23D48: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140E23D4D: E8 2E 62 42 00     call        0000000141249F80
  0000000140E23D52: 90                 nop
  0000000140E23D53: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140E23D5B: E8 10 88 3F 00     call        000000014121C570
  0000000140E23D60: 90                 nop
  0000000140E23D61: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140E23D69: E8 02 88 3F 00     call        000000014121C570
  0000000140E23D6E: 48 81 C4 28 02 00  add         rsp,228h
                    00
  0000000140E23D75: C3                 ret

  Summary

     1E02000 .text
