Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412E4B90: 4C 8B DC           mov         r11,rsp
  00000001412E4B93: 53                 push        rbx
  00000001412E4B94: 48 81 EC 60 02 00  sub         rsp,260h
                    00
  00000001412E4B9B: 48 8B 05 8E 94 87  mov         rax,qword ptr [0000000143B5E030h]
                    02
  00000001412E4BA2: 48 33 C4           xor         rax,rsp
  00000001412E4BA5: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  00000001412E4BAD: 4C 8B 81 30 3A 00  mov         r8,qword ptr [rcx+0000000000003A30h]
                    00
  00000001412E4BB4: 48 8B D9           mov         rbx,rcx
  00000001412E4BB7: 4D 85 C0           test        r8,r8
  00000001412E4BBA: 0F 84 66 03 00 00  je          00000001412E4F26
  00000001412E4BC0: 4C 8B 89 38 3A 00  mov         r9,qword ptr [rcx+0000000000003A38h]
                    00
  00000001412E4BC7: 4D 85 C9           test        r9,r9
  00000001412E4BCA: 0F 84 56 03 00 00  je          00000001412E4F26
  00000001412E4BD0: 48 8B 81 40 3A 00  mov         rax,qword ptr [rcx+0000000000003A40h]
                    00
  00000001412E4BD7: 48 85 C0           test        rax,rax
  00000001412E4BDA: 0F 84 46 03 00 00  je          00000001412E4F26
  00000001412E4BE0: 48 8B 89 48 3A 00  mov         rcx,qword ptr [rcx+0000000000003A48h]
                    00
  00000001412E4BE7: 48 85 C9           test        rcx,rcx
  00000001412E4BEA: 0F 84 36 03 00 00  je          00000001412E4F26
  00000001412E4BF0: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  00000001412E4BF5: 49 89 6B 10        mov         qword ptr [r11+10h],rbp
  00000001412E4BF9: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  00000001412E4BFD: 49 8D 8B B8 FE FF  lea         rcx,[r11+FFFFFFFFFFFFFEB8h]
                    FF
  00000001412E4C04: 48 8D 15 D5 FD AC  lea         rdx,[0000000142DB49E0h]
                    01
  00000001412E4C0B: 49 89 7B 20        mov         qword ptr [r11+20h],rdi
  00000001412E4C0F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412E4C14: E8 1F 92 18 00     call        000000014146DE38
  00000001412E4C19: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412E4C21: E8 EA 17 1B 00     call        0000000141496410
  00000001412E4C26: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001412E4C2B: E8 00 1C 02 00     call        0000000141306830
  00000001412E4C30: 8B 83 A4 03 00 00  mov         eax,dword ptr [rbx+00000000000003A4h]
  00000001412E4C36: 33 ED              xor         ebp,ebp
  00000001412E4C38: 48 8D 8B E8 27 00  lea         rcx,[rbx+00000000000027E8h]
                    00
  00000001412E4C3F: C7 44 24 40 03 00  mov         dword ptr [rsp+40h],3
                    00 00
  00000001412E4C47: C7 44 24 44 2D 00  mov         dword ptr [rsp+44h],2Dh
                    00 00
  00000001412E4C4F: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  00000001412E4C53: 89 6C 24 50        mov         dword ptr [rsp+50h],ebp
  00000001412E4C57: C7 44 24 54 B0 00  mov         dword ptr [rsp+54h],0B0h
                    00 00
  00000001412E4C5F: 89 6C 24 60        mov         dword ptr [rsp+60h],ebp
  00000001412E4C63: E8 98 26 02 00     call        0000000141307300
  00000001412E4C68: 4C 8D 4C 24 6C     lea         r9,[rsp+6Ch]
  00000001412E4C6D: 4C 8D 44 24 68     lea         r8,[rsp+68h]
  00000001412E4C72: 48 8D 15 5B FD AC  lea         rdx,[0000000142DB49D4h]
                    01
  00000001412E4C79: 48 8B C8           mov         rcx,rax
  00000001412E4C7C: E8 53 93 18 00     call        000000014146DFD4
  00000001412E4C81: 48 8D 8B E8 27 00  lea         rcx,[rbx+00000000000027E8h]
                    00
  00000001412E4C88: E8 73 26 02 00     call        0000000141307300
  00000001412E4C8D: 4C 8B 8B 38 3A 00  mov         r9,qword ptr [rbx+0000000000003A38h]
                    00
  00000001412E4C94: 4C 8B 83 30 3A 00  mov         r8,qword ptr [rbx+0000000000003A30h]
                    00
  00000001412E4C9B: 48 8B D0           mov         rdx,rax
  00000001412E4C9E: 48 8B 83 40 3A 00  mov         rax,qword ptr [rbx+0000000000003A40h]
                    00
  00000001412E4CA5: 48 8D 8B E8 27 00  lea         rcx,[rbx+00000000000027E8h]
                    00
  00000001412E4CAC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412E4CB1: E8 5A 26 02 00     call        0000000141307310
  00000001412E4CB6: 48 8B F8           mov         rdi,rax
  00000001412E4CB9: 48 85 C0           test        rax,rax
  00000001412E4CBC: 75 45              jne         00000001412E4D03
  00000001412E4CBE: C7 83 58 3A 00 00  mov         dword ptr [rbx+0000000000003A58h],2
                    02 00 00 00
  00000001412E4CC8: C7 83 54 3A 00 00  mov         dword ptr [rbx+0000000000003A54h],10h
                    10 00 00 00
  00000001412E4CD2: 48 8B B4 24 80 02  mov         rsi,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001412E4CDA: 48 8B AC 24 78 02  mov         rbp,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001412E4CE2: 48 8B BC 24 88 02  mov         rdi,qword ptr [rsp+0000000000000288h]
                    00 00
  00000001412E4CEA: 48 8B 8C 24 50 02  mov         rcx,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001412E4CF2: 48 33 CC           xor         rcx,rsp
  00000001412E4CF5: E8 16 92 18 00     call        000000014146DF10
  00000001412E4CFA: 48 81 C4 60 02 00  add         rsp,260h
                    00
  00000001412E4D01: 5B                 pop         rbx
  00000001412E4D02: C3                 ret
  00000001412E4D03: 48 8D 88 A0 00 00  lea         rcx,[rax+00000000000000A0h]
                    00
  00000001412E4D0A: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  00000001412E4D0F: 48 8D 15 B2 F9 AC  lea         rdx,[0000000142DB46C8h]
                    01
  00000001412E4D16: E8 B9 92 18 00     call        000000014146DFD4
  00000001412E4D1B: 48 8D 8F 04 01 00  lea         rcx,[rdi+0000000000000104h]
                    00
  00000001412E4D22: 4C 8D 44 24 74     lea         r8,[rsp+74h]
  00000001412E4D27: 48 8D 15 9A F9 AC  lea         rdx,[0000000142DB46C8h]
                    01
  00000001412E4D2E: E8 A1 92 18 00     call        000000014146DFD4
  00000001412E4D33: 48 8D 8F 68 01 00  lea         rcx,[rdi+0000000000000168h]
                    00
  00000001412E4D3A: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  00000001412E4D3F: 48 8D 15 82 F9 AC  lea         rdx,[0000000142DB46C8h]
                    01
  00000001412E4D46: E8 89 92 18 00     call        000000014146DFD4
  00000001412E4D4B: 48 8D 8B E8 27 00  lea         rcx,[rbx+00000000000027E8h]
                    00
  00000001412E4D52: E8 A9 25 02 00     call        0000000141307300
  00000001412E4D57: 4C 8B 83 48 3A 00  mov         r8,qword ptr [rbx+0000000000003A48h]
                    00
  00000001412E4D5E: 48 8D 8B E8 27 00  lea         rcx,[rbx+00000000000027E8h]
                    00
  00000001412E4D65: 48 8B D0           mov         rdx,rax
  00000001412E4D68: E8 D3 29 02 00     call        0000000141307740
  00000001412E4D6D: 48 85 C0           test        rax,rax
  00000001412E4D70: 75 19              jne         00000001412E4D8B
  00000001412E4D72: C7 83 58 3A 00 00  mov         dword ptr [rbx+0000000000003A58h],2
                    02 00 00 00
  00000001412E4D7C: C7 83 54 3A 00 00  mov         dword ptr [rbx+0000000000003A54h],11h
                    11 00 00 00
  00000001412E4D86: E9 47 FF FF FF     jmp         00000001412E4CD2
  00000001412E4D8B: 4C 8D 44 24 7C     lea         r8,[rsp+7Ch]
  00000001412E4D90: 48 8D 15 31 F9 AC  lea         rdx,[0000000142DB46C8h]
                    01
  00000001412E4D97: 48 8B C8           mov         rcx,rax
  00000001412E4D9A: E8 35 92 18 00     call        000000014146DFD4
  00000001412E4D9F: 8B 83 B8 35 00 00  mov         eax,dword ptr [rbx+00000000000035B8h]
  00000001412E4DA5: 48 8B CB           mov         rcx,rbx
  00000001412E4DA8: 89 AC 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebp
                    00
  00000001412E4DAF: 89 AC 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],ebp
                    00
  00000001412E4DB6: 89 AC 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],ebp
                    00
  00000001412E4DBD: 89 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],eax
                    00
  00000001412E4DC4: 89 AC 24 8C 00 00  mov         dword ptr [rsp+000000000000008Ch],ebp
                    00
  00000001412E4DCB: 89 AC 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],ebp
                    00
  00000001412E4DD2: 89 AC 24 94 00 00  mov         dword ptr [rsp+0000000000000094h],ebp
                    00
  00000001412E4DD9: 89 AC 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],ebp
                    00
  00000001412E4DE0: 89 AC 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],ebp
                    00
  00000001412E4DE7: E8 44 2D 01 00     call        00000001412F7B30
  00000001412E4DEC: 48 85 C0           test        rax,rax
  00000001412E4DEF: 75 1A              jne         00000001412E4E0B
  00000001412E4DF1: 44 8D 40 1F        lea         r8d,[rax+1Fh]
  00000001412E4DF5: 48 8D 15 8A 21 E6  lea         rdx,[0000000142146F86h]
                    00
  00000001412E4DFC: 48 8D 8C 24 BC 00  lea         rcx,[rsp+00000000000000BCh]
                    00 00
  00000001412E4E04: E8 17 B2 18 00     call        0000000141470020
  00000001412E4E09: EB 16              jmp         00000001412E4E21
  00000001412E4E0B: 48 8D 8C 24 BC 00  lea         rcx,[rsp+00000000000000BCh]
                    00 00
  00000001412E4E13: 41 B8 1F 00 00 00  mov         r8d,1Fh
  00000001412E4E19: 48 8B D0           mov         rdx,rax
  00000001412E4E1C: E8 FF B1 18 00     call        0000000141470020
  00000001412E4E21: 8B 83 50 3A 00 00  mov         eax,dword ptr [rbx+0000000000003A50h]
  00000001412E4E27: 48 8D 8B 40 1F 00  lea         rcx,[rbx+0000000000001F40h]
                    00
  00000001412E4E2E: 89 AC 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],ebp
                    00
  00000001412E4E35: 89 44 24 64        mov         dword ptr [rsp+64h],eax
  00000001412E4E39: E8 42 4D 02 00     call        0000000141309B80
  00000001412E4E3E: 4C 8D 8C 24 A4 00  lea         r9,[rsp+00000000000000A4h]
                    00 00
  00000001412E4E46: 48 8B C8           mov         rcx,rax
  00000001412E4E49: 48 8D 84 24 AC 00  lea         rax,[rsp+00000000000000ACh]
                    00 00
  00000001412E4E51: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  00000001412E4E59: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001412E4E5E: 48 8D 84 24 A8 00  lea         rax,[rsp+00000000000000A8h]
                    00 00
  00000001412E4E66: 48 8D 15 5B FB AC  lea         rdx,[0000000142DB49C8h]
                    01
  00000001412E4E6D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412E4E72: E8 5D 91 18 00     call        000000014146DFD4
  00000001412E4E77: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000001412E4E7A: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412E4E7F: 48 8B CB           mov         rcx,rbx
  00000001412E4E82: 41 FF 53 48        call        qword ptr [r11+48h]
  00000001412E4E86: 83 F8 18           cmp         eax,18h
  00000001412E4E89: 75 3C              jne         00000001412E4EC7
  00000001412E4E8B: 48 8B 03           mov         rax,qword ptr [rbx]
  00000001412E4E8E: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001412E4E93: 48 8B CB           mov         rcx,rbx
  00000001412E4E96: FF 50 20           call        qword ptr [rax+20h]
  00000001412E4E99: 3D B8 00 00 00     cmp         eax,0B8h
  00000001412E4E9E: 75 27              jne         00000001412E4EC7
  00000001412E4EA0: 48 8B 03           mov         rax,qword ptr [rbx]
  00000001412E4EA3: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412E4EA8: 48 8B CB           mov         rcx,rbx
  00000001412E4EAB: FF 50 30           call        qword ptr [rax+30h]
  00000001412E4EAE: 83 F8 18           cmp         eax,18h
  00000001412E4EB1: 74 1B              je          00000001412E4ECE
  00000001412E4EB3: C7 83 58 3A 00 00  mov         dword ptr [rbx+0000000000003A58h],2
                    02 00 00 00
  00000001412E4EBD: C7 83 54 3A 00 00  mov         dword ptr [rbx+0000000000003A54h],12h
                    12 00 00 00
  00000001412E4EC7: 33 C0              xor         eax,eax
  00000001412E4EC9: E9 04 FE FF FF     jmp         00000001412E4CD2
  00000001412E4ECE: 48 8B 83 48 3A 00  mov         rax,qword ptr [rbx+0000000000003A48h]
                    00
  00000001412E4ED5: 4C 8B 8B 38 3A 00  mov         r9,qword ptr [rbx+0000000000003A38h]
                    00
  00000001412E4EDC: 4C 8B 83 30 3A 00  mov         r8,qword ptr [rbx+0000000000003A30h]
                    00
  00000001412E4EE3: C7 44 24 30 18 00  mov         dword ptr [rsp+30h],18h
                    00 00
  00000001412E4EEB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001412E4EF0: 48 8B 83 40 3A 00  mov         rax,qword ptr [rbx+0000000000003A40h]
                    00
  00000001412E4EF7: 48 8D 15 72 FA AC  lea         rdx,[0000000142DB4970h]
                    01
  00000001412E4EFE: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412E4F06: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412E4F0B: E8 28 8F 18 00     call        000000014146DE38
  00000001412E4F10: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412E4F18: E8 F3 14 1B 00     call        0000000141496410
  00000001412E4F1D: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  00000001412E4F21: E9 AC FD FF FF     jmp         00000001412E4CD2
  00000001412E4F26: 33 C0              xor         eax,eax
  00000001412E4F28: E9 BD FD FF FF     jmp         00000001412E4CEA
  00000001412E4F2D: CC                 int         3

  Summary

     1E02000 .text
