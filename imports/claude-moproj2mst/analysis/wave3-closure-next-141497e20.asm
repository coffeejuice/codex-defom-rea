Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141497E20: 40 53              push        rbx
  0000000141497E22: 55                 push        rbp
  0000000141497E23: 56                 push        rsi
  0000000141497E24: 57                 push        rdi
  0000000141497E25: 41 54              push        r12
  0000000141497E27: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000141497E2E: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000141497E37: 49 8B F8           mov         rdi,r8
  0000000141497E3A: 4C 8B E2           mov         r12,rdx
  0000000141497E3D: 48 8B F1           mov         rsi,rcx
  0000000141497E40: E8 EB 21 00 00     call        000000014149A030
  0000000141497E45: 48 83 7E 58 00     cmp         qword ptr [rsi+58h],0
  0000000141497E4A: 75 44              jne         0000000141497E90
  0000000141497E4C: B9 18 00 00 00     mov         ecx,18h
  0000000141497E51: E8 66 6C FD FF     call        000000014146EABC
  0000000141497E56: 48 8B D8           mov         rbx,rax
  0000000141497E59: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000141497E61: 48 85 C0           test        rax,rax
  0000000141497E64: 74 24              je          0000000141497E8A
  0000000141497E66: 49 8B D4           mov         rdx,r12
  0000000141497E69: 48 8B C8           mov         rcx,rax
  0000000141497E6C: FF 15 4E C3 96 00  call        qword ptr [0000000141E041C0h]
  0000000141497E72: 4C 8D 1D 7F 04 A7  lea         r11,[0000000142F082F8h]
                    01
  0000000141497E79: 4C 89 1B           mov         qword ptr [rbx],r11
  0000000141497E7C: 48 8B 4E 50        mov         rcx,qword ptr [rsi+50h]
  0000000141497E80: 48 89 4B 10        mov         qword ptr [rbx+10h],rcx
  0000000141497E84: F0 83 01 01        lock add    dword ptr [rcx],1
  0000000141497E88: EB 02              jmp         0000000141497E8C
  0000000141497E8A: 33 DB              xor         ebx,ebx
  0000000141497E8C: 48 89 5E 58        mov         qword ptr [rsi+58h],rbx
  0000000141497E90: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000141497E93: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000141497E96: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  0000000141497E99: 0F 84 72 02 00 00  je          0000000141498111
  0000000141497E9F: 32 DB              xor         bl,bl
  0000000141497EA1: 48 8B D7           mov         rdx,rdi
  0000000141497EA4: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000141497EA9: E8 22 B4 00 00     call        00000001414A32D0
  0000000141497EAE: 90                 nop
  0000000141497EAF: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  0000000141497EB4: 8B 4C 24 68        mov         ecx,dword ptr [rsp+68h]
  0000000141497EB8: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  0000000141497EC0: 8B C1              mov         eax,ecx
  0000000141497EC2: FF C1              inc         ecx
  0000000141497EC4: 89 4C 24 68        mov         dword ptr [rsp+68h],ecx
  0000000141497EC8: 85 C0              test        eax,eax
  0000000141497ECA: 0F 85 1A 02 00 00  jne         00000001414980EA
  0000000141497ED0: 48 3B 54 24 78     cmp         rdx,qword ptr [rsp+78h]
  0000000141497ED5: 0F 84 0F 02 00 00  je          00000001414980EA
  0000000141497EDB: 48 8B 2A           mov         rbp,qword ptr [rdx]
  0000000141497EDE: 66 90              nop
  0000000141497EE0: 85 C9              test        ecx,ecx
  0000000141497EE2: 0F 84 EF 01 00 00  je          00000001414980D7
  0000000141497EE8: 48 8B 05 49 BE 96  mov         rax,qword ptr [0000000141E03D38h]
                    00
  0000000141497EEF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000141497EF4: F0 83 00 01        lock add    dword ptr [rax],1
  0000000141497EF8: 48 8B 05 39 BE 96  mov         rax,qword ptr [0000000141E03D38h]
                    00
  0000000141497EFF: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000141497F04: F0 83 00 01        lock add    dword ptr [rax],1
  0000000141497F08: 4C 8D 46 50        lea         r8,[rsi+50h]
  0000000141497F0C: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000141497F11: 48 8B D5           mov         rdx,rbp
  0000000141497F14: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000141497F1C: E8 3F E6 FF FF     call        0000000141496560
  0000000141497F21: 90                 nop
  0000000141497F22: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141497F2A: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000141497F2E: 75 4B              jne         0000000141497F7B
  0000000141497F30: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000141497F34: 75 0F              jne         0000000141497F45
  0000000141497F36: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141497F3E: FF 15 EC BD 96 00  call        qword ptr [0000000141E03D30h]
  0000000141497F44: 90                 nop
  0000000141497F45: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000141497F4A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000141497F4E: 75 0C              jne         0000000141497F5C
  0000000141497F50: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000141497F55: FF 15 9D C2 96 00  call        qword ptr [0000000141E041F8h]
  0000000141497F5B: 90                 nop
  0000000141497F5C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141497F61: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000141497F65: 0F 85 5D 01 00 00  jne         00000001414980C8
  0000000141497F6B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141497F70: FF 15 82 C2 96 00  call        qword ptr [0000000141E041F8h]
  0000000141497F76: E9 4D 01 00 00     jmp         00000001414980C8
  0000000141497F7B: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  0000000141497F83: 48 8B CD           mov         rcx,rbp
  0000000141497F86: E8 15 54 00 00     call        000000014149D3A0
  0000000141497F8B: 90                 nop
  0000000141497F8C: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  0000000141497F94: 48 8B C8           mov         rcx,rax
  0000000141497F97: FF 15 6B C0 96 00  call        qword ptr [0000000141E04008h]
  0000000141497F9D: 90                 nop
  0000000141497F9E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000141497FA6: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000141497FAA: 75 0E              jne         0000000141497FBA
  0000000141497FAC: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000141497FB4: FF 15 76 BD 96 00  call        qword ptr [0000000141E03D30h]
  0000000141497FBA: 80 7E 48 00        cmp         byte ptr [rsi+48h],0
  0000000141497FBE: 74 66              je          0000000141498026
  0000000141497FC0: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000141497FC5: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000141497FCA: E8 E1 FB FF FF     call        0000000141497BB0
  0000000141497FCF: 48 8B D8           mov         rbx,rax
  0000000141497FD2: 4C 8D 84 24 C0 00  lea         r8,[rsp+00000000000000C0h]
                    00 00
  0000000141497FDA: 48 8D 15 87 0B A7  lea         rdx,[0000000142F08B68h]
                    01
  0000000141497FE1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000141497FE6: E8 E5 E2 FF FF     call        00000001414962D0
  0000000141497FEB: 90                 nop
  0000000141497FEC: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000141497FEF: 4C 8B C3           mov         r8,rbx
  0000000141497FF2: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  0000000141497FF6: 49 8B CC           mov         rcx,r12
  0000000141497FF9: FF 15 29 D5 96 00  call        qword ptr [0000000141E05528h]
  0000000141497FFF: 90                 nop
  0000000141498000: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141498005: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000141498009: 75 0C              jne         0000000141498017
  000000014149800B: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141498010: FF 15 E2 C1 96 00  call        qword ptr [0000000141E041F8h]
  0000000141498016: 90                 nop
  0000000141498017: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014149801C: FF 15 96 D9 96 00  call        qword ptr [0000000141E059B8h]
  0000000141498022: 0F B6 5E 49        movzx       ebx,byte ptr [rsi+49h]
  0000000141498026: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000014149802E: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000141498033: FF 15 67 D9 96 00  call        qword ptr [0000000141E059A0h]
  0000000141498039: 90                 nop
  000000014149803A: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000141498042: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  0000000141498047: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  000000014149804B: 49 8B CC           mov         rcx,r12
  000000014149804E: FF 15 D4 D4 96 00  call        qword ptr [0000000141E05528h]
  0000000141498054: 90                 nop
  0000000141498055: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014149805A: FF 15 58 D9 96 00  call        qword ptr [0000000141E059B8h]
  0000000141498060: 90                 nop
  0000000141498061: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000141498069: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149806D: 75 0F              jne         000000014149807E
  000000014149806F: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000141498077: FF 15 7B C1 96 00  call        qword ptr [0000000141E041F8h]
  000000014149807D: 90                 nop
  000000014149807E: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141498086: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149808A: 75 0F              jne         000000014149809B
  000000014149808C: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141498094: FF 15 96 BC 96 00  call        qword ptr [0000000141E03D30h]
  000000014149809A: 90                 nop
  000000014149809B: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001414980A0: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000001414980A4: 75 0C              jne         00000001414980B2
  00000001414980A6: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001414980AB: FF 15 47 C1 96 00  call        qword ptr [0000000141E041F8h]
  00000001414980B1: 90                 nop
  00000001414980B2: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001414980B7: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000001414980BB: 75 0B              jne         00000001414980C8
  00000001414980BD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001414980C2: FF 15 30 C1 96 00  call        qword ptr [0000000141E041F8h]
  00000001414980C8: 8B 4C 24 68        mov         ecx,dword ptr [rsp+68h]
  00000001414980CC: FF C9              dec         ecx
  00000001414980CE: 89 4C 24 68        mov         dword ptr [rsp+68h],ecx
  00000001414980D2: E9 09 FE FF FF     jmp         0000000141497EE0
  00000001414980D7: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  00000001414980DC: 48 83 C2 08        add         rdx,8
  00000001414980E0: 48 89 54 24 70     mov         qword ptr [rsp+70h],rdx
  00000001414980E5: E9 D6 FD FF FF     jmp         0000000141497EC0
  00000001414980EA: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001414980EF: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000001414980F3: 75 0B              jne         0000000141498100
  00000001414980F5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001414980FA: FF 15 F8 C0 96 00  call        qword ptr [0000000141E041F8h]
  0000000141498100: 84 DB              test        bl,bl
  0000000141498102: 74 0D              je          0000000141498111
  0000000141498104: 48 8B 56 58        mov         rdx,qword ptr [rsi+58h]
  0000000141498108: 49 8B CC           mov         rcx,r12
  000000014149810B: FF 15 F7 D4 96 00  call        qword ptr [0000000141E05608h]
  0000000141498111: 48 81 C4 80 00 00  add         rsp,80h
                    00
  0000000141498118: 41 5C              pop         r12
  000000014149811A: 5F                 pop         rdi
  000000014149811B: 5E                 pop         rsi
  000000014149811C: 5D                 pop         rbp
  000000014149811D: 5B                 pop         rbx
  000000014149811E: C3                 ret

  Summary

     1E02000 .text
