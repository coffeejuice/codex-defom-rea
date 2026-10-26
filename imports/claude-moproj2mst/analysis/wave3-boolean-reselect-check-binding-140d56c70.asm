Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D56C70: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D56C75: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D56C7A: 57                 push        rdi
  0000000140D56C7B: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  0000000140D56C82: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D56C8E: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  0000000140D56C96: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140D56C9E: 33 D2              xor         edx,edx
  0000000140D56CA0: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D56CA8: E8 C3 FF 7E FF     call        0000000140546C70
  0000000140D56CAD: 4C 8B C0           mov         r8,rax
  0000000140D56CB0: 48 8B 94 24 78 01  mov         rdx,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140D56CB8: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56CC0: FF 15 D2 ED 0A 01  call        qword ptr [0000000141E05A98h]
  0000000140D56CC6: 90                 nop
  0000000140D56CC7: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56CCF: 48 8D 05 62 7E CC  lea         rax,[0000000142A1EB38h]
                    01
  0000000140D56CD6: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D56CD9: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56CE1: 48 8D 05 18 7E CC  lea         rax,[0000000142A1EB00h]
                    01
  0000000140D56CE8: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D56CEC: E8 EF FE FF FF     call        0000000140D56BE0
  0000000140D56CF1: 33 C0              xor         eax,eax
  0000000140D56CF3: 85 C0              test        eax,eax
  0000000140D56CF5: 75 F5              jne         0000000140D56CEC
  0000000140D56CF7: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0A8h
                    00 00 A8 00 00 00
  0000000140D56D03: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D56D0B: E8 AC 7D 71 00     call        000000014146EABC
  0000000140D56D10: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D56D18: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140D56D21: 74 26              je          0000000140D56D49
  0000000140D56D23: 48 8B BC 24 98 00  mov         rdi,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D56D2B: 33 C0              xor         eax,eax
  0000000140D56D2D: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D56D35: F3 AA              rep stos    byte ptr [rdi]
  0000000140D56D37: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D56D3F: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140D56D47: EB 0C              jmp         0000000140D56D55
  0000000140D56D49: 48 C7 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00 00
  0000000140D56D55: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56D5D: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140D56D65: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  0000000140D56D69: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56D71: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56D79: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56D7D: E8 BE 11 00 00     call        0000000140D57F40
  0000000140D56D82: 83 3D 97 29 02 03  cmp         dword ptr [0000000143D79720h],1
                    01
  0000000140D56D89: 0F 85 90 03 00 00  jne         0000000140D5711F
  0000000140D56D8F: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56D97: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56D9B: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56D9F: FF 15 13 D9 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56DA5: 48 8B D0           mov         rdx,rax
  0000000140D56DA8: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D56DAD: E8 3E 6D 31 FF     call        000000014006DAF0
  0000000140D56DB2: 90                 nop
  0000000140D56DB3: 48 8D 44 24 38     lea         rax,[rsp+38h]
  0000000140D56DB8: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D56DBD: EB 59              jmp         0000000140D56E18
  0000000140D56DBF: 33 C0              xor         eax,eax
  0000000140D56DC1: 83 F8 01           cmp         eax,1
  0000000140D56DC4: 74 0E              je          0000000140D56DD4
  0000000140D56DC6: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  0000000140D56DD2: EB 26              jmp         0000000140D56DFA
  0000000140D56DD4: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56DDC: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56DE0: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56DE4: FF 15 CE D8 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56DEA: 48 8B C8           mov         rcx,rax
  0000000140D56DED: E8 5E DE 42 FF     call        0000000140184C50
  0000000140D56DF2: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  0000000140D56DFA: 48 8B 94 24 00 01  mov         rdx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D56E02: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D56E07: E8 74 55 0A 00     call        0000000140DFC380
  0000000140D56E0C: 48 8B C8           mov         rcx,rax
  0000000140D56E0F: 48 83 C1 10        add         rcx,10h
  0000000140D56E13: E8 E8 29 09 00     call        0000000140DE9800
  0000000140D56E18: 33 C0              xor         eax,eax
  0000000140D56E1A: 83 F8 01           cmp         eax,1
  0000000140D56E1D: 74 0E              je          0000000140D56E2D
  0000000140D56E1F: 48 C7 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],0
                    00 00 00 00 00 00
  0000000140D56E2B: EB 26              jmp         0000000140D56E53
  0000000140D56E2D: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56E35: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56E39: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56E3D: FF 15 75 D8 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56E43: 48 8B C8           mov         rcx,rax
  0000000140D56E46: E8 05 DE 42 FF     call        0000000140184C50
  0000000140D56E4B: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140D56E53: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140D56E5B: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D56E60: E8 1B 55 0A 00     call        0000000140DFC380
  0000000140D56E65: 48 8B C8           mov         rcx,rax
  0000000140D56E68: E8 13 52 0A 00     call        0000000140DFC080
  0000000140D56E6D: 0F B6 C0           movzx       eax,al
  0000000140D56E70: 85 C0              test        eax,eax
  0000000140D56E72: 0F 84 9D 02 00 00  je          0000000140D57115
  0000000140D56E78: 33 C0              xor         eax,eax
  0000000140D56E7A: 83 F8 01           cmp         eax,1
  0000000140D56E7D: 74 0E              je          0000000140D56E8D
  0000000140D56E7F: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0
                    00 00 00 00 00 00
  0000000140D56E8B: EB 26              jmp         0000000140D56EB3
  0000000140D56E8D: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56E95: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56E99: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56E9D: FF 15 15 D8 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56EA3: 48 8B C8           mov         rcx,rax
  0000000140D56EA6: E8 A5 DD 42 FF     call        0000000140184C50
  0000000140D56EAB: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000140D56EB3: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D56EBB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D56EC0: E8 BB 54 0A 00     call        0000000140DFC380
  0000000140D56EC5: 48 8B C8           mov         rcx,rax
  0000000140D56EC8: 48 83 C1 10        add         rcx,10h
  0000000140D56ECC: E8 2F 85 8F FF     call        000000014064F400
  0000000140D56ED1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D56ED4: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D56ED9: EB 70              jmp         0000000140D56F4B
  0000000140D56EDB: 33 C0              xor         eax,eax
  0000000140D56EDD: 83 F8 01           cmp         eax,1
  0000000140D56EE0: 74 0E              je          0000000140D56EF0
  0000000140D56EE2: 48 C7 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],0
                    00 00 00 00 00 00
  0000000140D56EEE: EB 26              jmp         0000000140D56F16
  0000000140D56EF0: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56EF8: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56EFC: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56F00: FF 15 B2 D7 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56F06: 48 8B C8           mov         rcx,rax
  0000000140D56F09: E8 42 DD 42 FF     call        0000000140184C50
  0000000140D56F0E: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000140D56F16: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D56F1E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D56F23: E8 58 54 0A 00     call        0000000140DFC380
  0000000140D56F28: 48 83 C0 08        add         rax,8
  0000000140D56F2C: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140D56F34: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D56F3C: 8B 08              mov         ecx,dword ptr [rax]
  0000000140D56F3E: 83 E9 01           sub         ecx,1
  0000000140D56F41: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D56F49: 89 08              mov         dword ptr [rax],ecx
  0000000140D56F4B: 33 C0              xor         eax,eax
  0000000140D56F4D: 83 F8 01           cmp         eax,1
  0000000140D56F50: 74 0E              je          0000000140D56F60
  0000000140D56F52: 48 C7 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],0
                    00 00 00 00 00 00
  0000000140D56F5E: EB 26              jmp         0000000140D56F86
  0000000140D56F60: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D56F68: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D56F6C: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140D56F70: FF 15 42 D7 0A 01  call        qword ptr [0000000141E046B8h]
  0000000140D56F76: 48 8B C8           mov         rcx,rax
  0000000140D56F79: E8 D2 DC 42 FF     call        0000000140184C50
  0000000140D56F7E: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  0000000140D56F86: 48 8B 94 24 28 01  mov         rdx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140D56F8E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D56F93: E8 E8 53 0A 00     call        0000000140DFC380
  0000000140D56F98: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140D56F9C: 0F 84 6E 01 00 00  je          0000000140D57110
  0000000140D56FA2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D56FA7: E8 04 F5 3C FF     call        00000001401264B0
  0000000140D56FAC: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140D56FB1: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140D56FB7: 0F 84 4E 01 00 00  je          0000000140D5710B
  0000000140D56FBD: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D56FC2: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140D56FC7: FF 15 83 2F 0B 01  call        qword ptr [0000000141E09F50h]
  0000000140D56FCD: 90                 nop
  0000000140D56FCE: 41 B8 18 00 00 00  mov         r8d,18h
  0000000140D56FD4: BA 18 00 00 00     mov         edx,18h
  0000000140D56FD9: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140D56FE1: FF 15 C1 D0 0A 01  call        qword ptr [0000000141E040A8h]
  0000000140D56FE7: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140D56FEF: 45 33 C9           xor         r9d,r9d
  0000000140D56FF2: 4C 8B C0           mov         r8,rax
  0000000140D56FF5: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  0000000140D56FFD: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D57002: FF 15 60 1A 0B 01  call        qword ptr [0000000141E08A68h]
  0000000140D57008: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140D57010: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140D57018: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140D57020: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140D57028: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140D57030: 41 B9 18 00 00 00  mov         r9d,18h
  0000000140D57036: 41 B8 18 00 00 00  mov         r8d,18h
  0000000140D5703C: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  0000000140D57044: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140D5704C: FF 15 F6 2E 0B 01  call        qword ptr [0000000141E09F48h]
  0000000140D57052: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  0000000140D5705A: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140D57062: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000140D5706A: 48 8B 94 24 48 01  mov         rdx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140D57072: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140D5707A: FF 15 58 02 0B 01  call        qword ptr [0000000141E072D8h]
  0000000140D57080: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140D57088: 48 8B 84 24 50 01  mov         rax,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D57090: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  0000000140D57098: 48 8B 94 24 58 01  mov         rdx,qword ptr [rsp+0000000000000158h]
                    00 00
  0000000140D570A0: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140D570A5: FF 15 D5 01 0B 01  call        qword ptr [0000000141E07280h]
  0000000140D570AB: 90                 nop
  0000000140D570AC: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140D570B4: FF 15 E6 45 0B 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D570BA: 90                 nop
  0000000140D570BB: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D570C3: FF 15 0F 4F 0B 01  call        qword ptr [0000000141E0BFD8h]
  0000000140D570C9: 90                 nop
  0000000140D570CA: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140D570D2: FF 15 00 4F 0B 01  call        qword ptr [0000000141E0BFD8h]
  0000000140D570D8: 41 B8 18 00 00 00  mov         r8d,18h
  0000000140D570DE: BA 18 00 00 00     mov         edx,18h
  0000000140D570E3: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140D570EB: FF 15 B7 CF 0A 01  call        qword ptr [0000000141E040A8h]
  0000000140D570F1: 48 8B D0           mov         rdx,rax
  0000000140D570F4: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140D570F9: FF 15 89 01 0B 01  call        qword ptr [0000000141E07288h]
  0000000140D570FF: 90                 nop
  0000000140D57100: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D57105: FF 15 95 45 0B 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D5710B: E9 CB FD FF FF     jmp         0000000140D56EDB
  0000000140D57110: E9 AA FC FF FF     jmp         0000000140D56DBF
  0000000140D57115: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D5711A: E8 01 A4 EF FF     call        0000000140C51520
  0000000140D5711F: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57127: FF 15 2B E9 0A 01  call        qword ptr [0000000141E05A58h]
  0000000140D5712D: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D57132: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D57138: 74 33              je          0000000140D5716D
  0000000140D5713A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D5713F: FF 15 0B 48 0B 01  call        qword ptr [0000000141E0B950h]
  0000000140D57145: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D5714A: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140D57150: 74 1B              je          0000000140D5716D
  0000000140D57152: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D5715A: 45 33 C9           xor         r9d,r9d
  0000000140D5715D: 45 33 C0           xor         r8d,r8d
  0000000140D57160: 33 D2              xor         edx,edx
  0000000140D57162: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D57167: FF 15 3B 42 0B 01  call        qword ptr [0000000141E0B3A8h]
  0000000140D5716D: 48 8D 15 2B 41 CC  lea         rdx,[0000000142A1B29Fh]
                    01
  0000000140D57174: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140D5717C: FF 15 FE D2 0A 01  call        qword ptr [0000000141E04480h]
  0000000140D57182: 90                 nop
  0000000140D57183: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5718B: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5718F: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  0000000140D57197: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  0000000140D5719E: FF 15 5C F7 0A 01  call        qword ptr [0000000141E06900h]
  0000000140D571A4: 90                 nop
  0000000140D571A5: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140D571AD: FF 15 2D E8 0A 01  call        qword ptr [0000000141E059E0h]
  0000000140D571B3: 33 D2              xor         edx,edx
  0000000140D571B5: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D571BD: E8 7E 03 00 00     call        0000000140D57540
  0000000140D571C2: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D571CA: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D571CE: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D571D6: 4C 8D 0D 33 76 CC  lea         r9,[0000000142A1E810h]
                    01
  0000000140D571DD: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D571E5: 48 8D 15 3C 76 CC  lea         rdx,[0000000142A1E828h]
                    01
  0000000140D571EC: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  0000000140D571F0: FF 15 DA CF 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D571F6: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D571FE: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57202: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D5720A: 4C 8D 0D 27 76 CC  lea         r9,[0000000142A1E838h]
                    01
  0000000140D57211: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57219: 48 8D 15 30 76 CC  lea         rdx,[0000000142A1E850h]
                    01
  0000000140D57220: 48 8B 49 70        mov         rcx,qword ptr [rcx+70h]
  0000000140D57224: FF 15 A6 CF 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D5722A: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57232: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57236: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D5723E: 4C 8D 0D 1B 76 CC  lea         r9,[0000000142A1E860h]
                    01
  0000000140D57245: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5724D: 48 8D 15 2C 76 CC  lea         rdx,[0000000142A1E880h]
                    01
  0000000140D57254: 48 8B 49 78        mov         rcx,qword ptr [rcx+78h]
  0000000140D57258: FF 15 72 CF 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D5725E: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57266: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5726A: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D57272: 4C 8D 0D 17 76 CC  lea         r9,[0000000142A1E890h]
                    01
  0000000140D57279: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57281: 48 8D 15 28 76 CC  lea         rdx,[0000000142A1E8B0h]
                    01
  0000000140D57288: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5728C: FF 15 3E CF 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D57292: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5729A: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5729E: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D572A6: 4C 8D 0D 13 76 CC  lea         r9,[0000000142A1E8C0h]
                    01
  0000000140D572AD: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D572B5: 48 8D 15 24 76 CC  lea         rdx,[0000000142A1E8E0h]
                    01
  0000000140D572BC: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  0000000140D572C0: FF 15 0A CF 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D572C6: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D572CE: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D572D2: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D572DA: 4C 8D 0D 0F 76 CC  lea         r9,[0000000142A1E8F0h]
                    01
  0000000140D572E1: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D572E9: 48 8D 15 20 76 CC  lea         rdx,[0000000142A1E910h]
                    01
  0000000140D572F0: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  0000000140D572F4: FF 15 D6 CE 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D572FA: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57302: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57306: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D5730E: 4C 8D 0D 0B 76 CC  lea         r9,[0000000142A1E920h]
                    01
  0000000140D57315: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5731D: 48 8D 15 1C 76 CC  lea         rdx,[0000000142A1E940h]
                    01
  0000000140D57324: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  0000000140D57328: FF 15 A2 CE 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D5732E: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57336: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5733A: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D57342: 4C 8D 0D 07 76 CC  lea         r9,[0000000142A1E950h]
                    01
  0000000140D57349: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57351: 48 8D 15 18 76 CC  lea         rdx,[0000000142A1E970h]
                    01
  0000000140D57358: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D5735C: FF 15 6E CE 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D57362: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5736A: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D5736E: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D57376: 4C 8D 0D 03 76 CC  lea         r9,[0000000142A1E980h]
                    01
  0000000140D5737D: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57385: 48 8D 15 14 76 CC  lea         rdx,[0000000142A1E9A0h]
                    01
  0000000140D5738C: 48 8B 49 58        mov         rcx,qword ptr [rcx+58h]
  0000000140D57390: FF 15 3A CE 0A 01  call        qword ptr [0000000141E041D0h]
  0000000140D57396: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140D5739B: 4C 8D 0D 0E 76 CC  lea         r9,[0000000142A1E9B0h]
                    01
  0000000140D573A2: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D573AA: 48 8D 15 1F 76 CC  lea         rdx,[0000000142A1E9D0h]
                    01
  0000000140D573B1: 48 8D 0D 48 F3 01  lea         rcx,[0000000143D76700h]
                    03
  0000000140D573B8: E8 23 88 57 FF     call        00000001402CFBE0
  0000000140D573BD: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D573C5: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D573C9: B2 01              mov         dl,1
  0000000140D573CB: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  0000000140D573CF: FF 15 BB FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D573D5: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D573DD: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D573E1: B2 01              mov         dl,1
  0000000140D573E3: 48 8B 49 70        mov         rcx,qword ptr [rcx+70h]
  0000000140D573E7: FF 15 A3 FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D573ED: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D573F5: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D573F9: B2 01              mov         dl,1
  0000000140D573FB: 48 8B 49 78        mov         rcx,qword ptr [rcx+78h]
  0000000140D573FF: FF 15 8B FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D57405: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5740D: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57411: B2 01              mov         dl,1
  0000000140D57413: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57417: FF 15 73 FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D5741D: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57425: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57429: B2 01              mov         dl,1
  0000000140D5742B: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  0000000140D5742F: FF 15 5B FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D57435: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5743D: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57441: B2 01              mov         dl,1
  0000000140D57443: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  0000000140D57447: FF 15 43 FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D5744D: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57455: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140D57459: B2 01              mov         dl,1
  0000000140D5745B: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  0000000140D5745F: FF 15 2B FE 0A 01  call        qword ptr [0000000141E07290h]
  0000000140D57465: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D5746D: E8 DE 08 00 00     call        0000000140D57D50
  0000000140D57472: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140D57477: 4C 8D 0D 7A 75 CC  lea         r9,[0000000142A1E9F8h]
                    01
  0000000140D5747E: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D57486: 48 8D 15 83 75 CC  lea         rdx,[0000000142A1EA10h]
                    01
  0000000140D5748D: 48 8D 0D 6C F2 01  lea         rcx,[0000000143D76700h]
                    03
  0000000140D57494: E8 47 87 57 FF     call        00000001402CFBE0
  0000000140D57499: 90                 nop
  0000000140D5749A: 48 8B 84 24 70 01  mov         rax,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D574A2: 48 81 C4 60 01 00  add         rsp,160h
                    00
  0000000140D574A9: 5F                 pop         rdi
  0000000140D574AA: C3                 ret

  Summary

     1E02000 .text
