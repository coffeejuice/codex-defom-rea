Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067130B60: 48 85 C9           test        rcx,rcx
  0000000067130B63: 0F 84 92 04 00 00  je          0000000067130FFB
  0000000067130B69: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067130B6E: 53                 push        rbx
  0000000067130B6F: 55                 push        rbp
  0000000067130B70: 56                 push        rsi
  0000000067130B71: 57                 push        rdi
  0000000067130B72: 41 54              push        r12
  0000000067130B74: 41 55              push        r13
  0000000067130B76: 41 56              push        r14
  0000000067130B78: 41 57              push        r15
  0000000067130B7A: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  0000000067130B81: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000067130B8D: 48 8B F9           mov         rdi,rcx
  0000000067130B90: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067130B93: FF 10              call        qword ptr [rax]
  0000000067130B95: 48 8B D8           mov         rbx,rax
  0000000067130B98: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000067130B9D: 48 8D 05 8C FF 1B  lea         rax,[00000000672F0B30h]
                    00
  0000000067130BA4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000067130BA9: F0 83 05 7F FF 1B  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  0000000067130BB1: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  0000000067130BB6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000067130BBB: 48 8B CF           mov         rcx,rdi
  0000000067130BBE: E8 2D BB FF FF     call        000000006712C6F0
  0000000067130BC3: 90                 nop
  0000000067130BC4: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000067130BC9: 49 83 CE FF        or          r14,0FFFFFFFFFFFFFFFFh
  0000000067130BCD: 41 8B C6           mov         eax,r14d
  0000000067130BD0: F0 0F C1 01        lock xadd   dword ptr [rcx],eax
  0000000067130BD4: 41 03 C6           add         eax,r14d
  0000000067130BD7: 75 0A              jne         0000000067130BE3
  0000000067130BD9: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000067130BDE: E8 ED F4 F3 FF     call        00000000670700D0
  0000000067130BE3: 33 ED              xor         ebp,ebp
  0000000067130BE5: 89 AC 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],ebp
                    00
  0000000067130BEC: 48 8B CB           mov         rcx,rbx
  0000000067130BEF: E8 FC D2 FE FF     call        000000006711DEF0
  0000000067130BF4: 85 C0              test        eax,eax
  0000000067130BF6: 0F 8E D2 03 00 00  jle         0000000067130FCE
  0000000067130BFC: 0F 1F 40 00        nop         dword ptr [rax]
  0000000067130C00: 44 8B C5           mov         r8d,ebp
  0000000067130C03: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000067130C0B: 48 8B CB           mov         rcx,rbx
  0000000067130C0E: E8 FD D7 FE FF     call        000000006711E410
  0000000067130C13: 48 8B C8           mov         rcx,rax
  0000000067130C16: E8 85 E3 FE FF     call        000000006711EFA0
  0000000067130C1B: 4C 8B F8           mov         r15,rax
  0000000067130C1E: 80 38 6F           cmp         byte ptr [rax],6Fh
  0000000067130C21: 0F 85 8E 03 00 00  jne         0000000067130FB5
  0000000067130C27: 80 78 01 6E        cmp         byte ptr [rax+1],6Eh
  0000000067130C2B: 0F 85 84 03 00 00  jne         0000000067130FB5
  0000000067130C31: 80 78 02 5F        cmp         byte ptr [rax+2],5Fh
  0000000067130C35: 0F 85 7A 03 00 00  jne         0000000067130FB5
  0000000067130C3B: C7 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],0
                    00 00 00 00 00
  0000000067130C46: 33 D2              xor         edx,edx
  0000000067130C48: 48 89 54 24 58     mov         qword ptr [rsp+58h],rdx
  0000000067130C4D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067130C52: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  0000000067130C55: 2B 41 08           sub         eax,dword ptr [rcx+8]
  0000000067130C58: 85 C0              test        eax,eax
  0000000067130C5A: 0F 8E 27 03 00 00  jle         0000000067130F87
  0000000067130C60: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000067130C64: 48 03 C2           add         rax,rdx
  0000000067130C67: 4C 8B 6C C1 18     mov         r13,qword ptr [rcx+rax*8+18h]
  0000000067130C6C: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000067130C71: 49 8B CD           mov         rcx,r13
  0000000067130C74: E8 77 A1 FF FF     call        000000006712ADF0
  0000000067130C79: 90                 nop
  0000000067130C7A: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  0000000067130C82: 48 8B C8           mov         rcx,rax
  0000000067130C85: E8 E6 D7 F3 FF     call        000000006706E470
  0000000067130C8A: 90                 nop
  0000000067130C8B: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000067130C90: 41 8B CE           mov         ecx,r14d
  0000000067130C93: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067130C97: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067130C9A: 75 0A              jne         0000000067130CA6
  0000000067130C9C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000067130CA1: E8 2A F4 F3 FF     call        00000000670700D0
  0000000067130CA6: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130CAE: 48 8D 50 08        lea         rdx,[rax+8]
  0000000067130CB2: 48 63 3A           movsxd      rdi,dword ptr [rdx]
  0000000067130CB5: 85 FF              test        edi,edi
  0000000067130CB7: 0F 84 01 02 00 00  je          0000000067130EBE
  0000000067130CBD: 8B 08              mov         ecx,dword ptr [rax]
  0000000067130CBF: 83 F9 01           cmp         ecx,1
  0000000067130CC2: 75 12              jne         0000000067130CD6
  0000000067130CC4: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130CCC: 48 8D 48 18        lea         rcx,[rax+18h]
  0000000067130CD0: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  0000000067130CD4: 74 17              je          0000000067130CED
  0000000067130CD6: 8B 12              mov         edx,dword ptr [rdx]
  0000000067130CD8: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000067130CE0: E8 2B CA EF FF     call        000000006702D710
  0000000067130CE5: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130CED: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  0000000067130CF1: 49 8D 4F 03        lea         rcx,[r15+3]
  0000000067130CF5: 48 85 C9           test        rcx,rcx
  0000000067130CF8: 74 1E              je          0000000067130D18
  0000000067130CFA: 48 85 D2           test        rdx,rdx
  0000000067130CFD: 0F 84 BB 01 00 00  je          0000000067130EBE
  0000000067130D03: 44 8B C7           mov         r8d,edi
  0000000067130D06: FF 15 3C 79 07 00  call        qword ptr [00000000671A8648h]
  0000000067130D0C: 8B D0              mov         edx,eax
  0000000067130D0E: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130D16: EB 05              jmp         0000000067130D1D
  0000000067130D18: 48 F7 DA           neg         rdx
  0000000067130D1B: 1B D2              sbb         edx,edx
  0000000067130D1D: 85 D2              test        edx,edx
  0000000067130D1F: 0F 85 99 01 00 00  jne         0000000067130EBE
  0000000067130D25: 42 80 7C 3F 03 5F  cmp         byte ptr [rdi+r15+3],5Fh
  0000000067130D2B: 0F 85 8D 01 00 00  jne         0000000067130EBE
  0000000067130D31: 4A 8D 5C 3F 04     lea         rbx,[rdi+r15+4]
  0000000067130D36: 48 8B D3           mov         rdx,rbx
  0000000067130D39: 49 8B 4D 08        mov         rcx,qword ptr [r13+8]
  0000000067130D3D: E8 5E 91 FF FF     call        0000000067129EA0
  0000000067130D42: 8B F0              mov         esi,eax
  0000000067130D44: 85 C0              test        eax,eax
  0000000067130D46: 0F 89 1C 01 00 00  jns         0000000067130E68
  0000000067130D4C: 49 8B 55 00        mov         rdx,qword ptr [r13]
  0000000067130D50: 49 8B CD           mov         rcx,r13
  0000000067130D53: FF 12              call        qword ptr [rdx]
  0000000067130D55: 4C 8B E0           mov         r12,rax
  0000000067130D58: 48 85 DB           test        rbx,rbx
  0000000067130D5B: 74 12              je          0000000067130D6F
  0000000067130D5D: 33 C0              xor         eax,eax
  0000000067130D5F: 49 8B CE           mov         rcx,r14
  0000000067130D62: 48 8B FB           mov         rdi,rbx
  0000000067130D65: F2 AE              repne scas  byte ptr [rdi]
  0000000067130D67: 48 F7 D1           not         rcx
  0000000067130D6A: 48 FF C9           dec         rcx
  0000000067130D6D: EB 02              jmp         0000000067130D71
  0000000067130D6F: 33 C9              xor         ecx,ecx
  0000000067130D71: 8D 79 FF           lea         edi,[rcx-1]
  0000000067130D74: 33 ED              xor         ebp,ebp
  0000000067130D76: 49 8B 45 00        mov         rax,qword ptr [r13]
  0000000067130D7A: 49 8B CD           mov         rcx,r13
  0000000067130D7D: FF 10              call        qword ptr [rax]
  0000000067130D7F: 48 8B C8           mov         rcx,rax
  0000000067130D82: E8 69 D1 FE FF     call        000000006711DEF0
  0000000067130D87: 85 C0              test        eax,eax
  0000000067130D89: 0F 8E A0 00 00 00  jle         0000000067130E2F
  0000000067130D8F: 90                 nop
  0000000067130D90: 44 8B C5           mov         r8d,ebp
  0000000067130D93: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000067130D98: 49 8B CC           mov         rcx,r12
  0000000067130D9B: E8 70 D6 FE FF     call        000000006711E410
  0000000067130DA0: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000067130DA5: E8 F6 DA FE FF     call        000000006711E8A0
  0000000067130DAA: 83 F8 01           cmp         eax,1
  0000000067130DAD: 75 31              jne         0000000067130DE0
  0000000067130DAF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000067130DB4: E8 E7 E1 FE FF     call        000000006711EFA0
  0000000067130DB9: 48 85 C0           test        rax,rax
  0000000067130DBC: 74 16              je          0000000067130DD4
  0000000067130DBE: 48 85 DB           test        rbx,rbx
  0000000067130DC1: 74 1D              je          0000000067130DE0
  0000000067130DC3: 44 8B C7           mov         r8d,edi
  0000000067130DC6: 48 8B D3           mov         rdx,rbx
  0000000067130DC9: 48 8B C8           mov         rcx,rax
  0000000067130DCC: FF 15 76 78 07 00  call        qword ptr [00000000671A8648h]
  0000000067130DD2: EB 08              jmp         0000000067130DDC
  0000000067130DD4: 48 8B C3           mov         rax,rbx
  0000000067130DD7: 48 F7 D8           neg         rax
  0000000067130DDA: 1B C0              sbb         eax,eax
  0000000067130DDC: 85 C0              test        eax,eax
  0000000067130DDE: 74 19              je          0000000067130DF9
  0000000067130DE0: FF C5              inc         ebp
  0000000067130DE2: 49 8B 45 00        mov         rax,qword ptr [r13]
  0000000067130DE6: 49 8B CD           mov         rcx,r13
  0000000067130DE9: FF 10              call        qword ptr [rax]
  0000000067130DEB: 48 8B C8           mov         rcx,rax
  0000000067130DEE: E8 FD D0 FE FF     call        000000006711DEF0
  0000000067130DF3: 3B E8              cmp         ebp,eax
  0000000067130DF5: 7D 38              jge         0000000067130E2F
  0000000067130DF7: EB 97              jmp         0000000067130D90
  0000000067130DF9: 33 FF              xor         edi,edi
  0000000067130DFB: 33 F6              xor         esi,esi
  0000000067130DFD: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000067130E02: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067130E05: 48 85 C9           test        rcx,rcx
  0000000067130E08: 74 21              je          0000000067130E2B
  0000000067130E0A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  0000000067130E10: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000067130E14: 8B 50 10           mov         edx,dword ptr [rax+10h]
  0000000067130E17: 03 FA              add         edi,edx
  0000000067130E19: 83 38 04           cmp         dword ptr [rax],4
  0000000067130E1C: 7C 03              jl          0000000067130E21
  0000000067130E1E: 8B 50 34           mov         edx,dword ptr [rax+34h]
  0000000067130E21: 03 F2              add         esi,edx
  0000000067130E23: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000067130E26: 48 85 C9           test        rcx,rcx
  0000000067130E29: 75 E5              jne         0000000067130E10
  0000000067130E2B: 2B F7              sub         esi,edi
  0000000067130E2D: 03 F5              add         esi,ebp
  0000000067130E2F: 85 F6              test        esi,esi
  0000000067130E31: 79 2E              jns         0000000067130E61
  0000000067130E33: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130E3B: 41 8B CE           mov         ecx,r14d
  0000000067130E3E: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067130E42: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067130E45: 0F 85 8E 00 00 00  jne         0000000067130ED9
  0000000067130E4B: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130E53: E8 58 F5 EE FF     call        00000000670203B0
  0000000067130E58: 8B AC 24 08 01 00  mov         ebp,dword ptr [rsp+0000000000000108h]
                    00
  0000000067130E5F: EB 7F              jmp         0000000067130EE0
  0000000067130E61: 8B AC 24 08 01 00  mov         ebp,dword ptr [rsp+0000000000000108h]
                    00
  0000000067130E68: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000067130E71: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000067130E79: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000067130E82: 44 8B CD           mov         r9d,ebp
  0000000067130E85: 4C 8B 84 24 00 01  mov         r8,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000067130E8D: 8B D6              mov         edx,esi
  0000000067130E8F: 49 8B CD           mov         rcx,r13
  0000000067130E92: E8 39 F8 FF FF     call        00000000671306D0
  0000000067130E97: 84 C0              test        al,al
  0000000067130E99: 75 76              jne         0000000067130F11
  0000000067130E9B: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130EA3: 41 8B CE           mov         ecx,r14d
  0000000067130EA6: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067130EAA: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067130EAD: 75 31              jne         0000000067130EE0
  0000000067130EAF: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130EB7: E8 F4 F4 EE FF     call        00000000670203B0
  0000000067130EBC: EB 22              jmp         0000000067130EE0
  0000000067130EBE: 41 8B CE           mov         ecx,r14d
  0000000067130EC1: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067130EC5: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067130EC8: 75 16              jne         0000000067130EE0
  0000000067130ECA: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130ED2: E8 D9 F4 EE FF     call        00000000670203B0
  0000000067130ED7: EB 07              jmp         0000000067130EE0
  0000000067130ED9: 8B AC 24 08 01 00  mov         ebp,dword ptr [rsp+0000000000000108h]
                    00
  0000000067130EE0: 8B BC 24 10 01 00  mov         edi,dword ptr [rsp+0000000000000110h]
                    00
  0000000067130EE7: FF C7              inc         edi
  0000000067130EE9: 89 BC 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],edi
                    00
  0000000067130EF0: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000067130EF5: 48 FF C2           inc         rdx
  0000000067130EF8: 48 89 54 24 58     mov         qword ptr [rsp+58h],rdx
  0000000067130EFD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067130F02: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  0000000067130F05: 2B 41 08           sub         eax,dword ptr [rcx+8]
  0000000067130F08: 3B F8              cmp         edi,eax
  0000000067130F0A: 7D 76              jge         0000000067130F82
  0000000067130F0C: E9 4F FD FF FF     jmp         0000000067130C60
  0000000067130F11: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130F19: 41 8B CE           mov         ecx,r14d
  0000000067130F1C: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067130F20: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067130F23: 75 0D              jne         0000000067130F32
  0000000067130F25: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000067130F2D: E8 7E F4 EE FF     call        00000000670203B0
  0000000067130F32: 8D 7D 01           lea         edi,[rbp+1]
  0000000067130F35: 44 8B C7           mov         r8d,edi
  0000000067130F38: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000067130F3D: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  0000000067130F42: 48 8B CB           mov         rcx,rbx
  0000000067130F45: E8 C6 D4 FE FF     call        000000006711E410
  0000000067130F4A: 48 8B C8           mov         rcx,rax
  0000000067130F4D: E8 6E D8 FE FF     call        000000006711E7C0
  0000000067130F52: A8 02              test        al,2
  0000000067130F54: 74 5F              je          0000000067130FB5
  0000000067130F56: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000067130F60: 8B EF              mov         ebp,edi
  0000000067130F62: FF C7              inc         edi
  0000000067130F64: 44 8B C7           mov         r8d,edi
  0000000067130F67: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000067130F6C: 48 8B CB           mov         rcx,rbx
  0000000067130F6F: E8 9C D4 FE FF     call        000000006711E410
  0000000067130F74: 48 8B C8           mov         rcx,rax
  0000000067130F77: E8 44 D8 FE FF     call        000000006711E7C0
  0000000067130F7C: A8 02              test        al,2
  0000000067130F7E: 75 E0              jne         0000000067130F60
  0000000067130F80: EB 33              jmp         0000000067130FB5
  0000000067130F82: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  0000000067130F87: 44 8B C5           mov         r8d,ebp
  0000000067130F8A: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000067130F92: 48 8B CB           mov         rcx,rbx
  0000000067130F95: E8 76 D4 FE FF     call        000000006711E410
  0000000067130F9A: 48 8B C8           mov         rcx,rax
  0000000067130F9D: E8 1E D8 FE FF     call        000000006711E7C0
  0000000067130FA2: A8 02              test        al,2
  0000000067130FA4: 75 0F              jne         0000000067130FB5
  0000000067130FA6: 49 8B D7           mov         rdx,r15
  0000000067130FA9: 48 8D 0D E8 BF 10  lea         rcx,[000000006723CF98h]
                    00
  0000000067130FB0: E8 2B BF EE FF     call        000000006701CEE0
  0000000067130FB5: FF C5              inc         ebp
  0000000067130FB7: 89 AC 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],ebp
                    00
  0000000067130FBE: 48 8B CB           mov         rcx,rbx
  0000000067130FC1: E8 2A CF FE FF     call        000000006711DEF0
  0000000067130FC6: 3B E8              cmp         ebp,eax
  0000000067130FC8: 0F 8C 32 FC FF FF  jl          0000000067130C00
  0000000067130FCE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000067130FD3: F0 44 0F C1 30     lock xadd   dword ptr [rax],r14d
  0000000067130FD8: 41 83 C6 FF        add         r14d,0FFFFFFFFh
  0000000067130FDC: 75 0A              jne         0000000067130FE8
  0000000067130FDE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067130FE3: E8 C8 F3 EE FF     call        00000000670203B0
  0000000067130FE8: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  0000000067130FEF: 41 5F              pop         r15
  0000000067130FF1: 41 5E              pop         r14
  0000000067130FF3: 41 5D              pop         r13
  0000000067130FF5: 41 5C              pop         r12
  0000000067130FF7: 5F                 pop         rdi
  0000000067130FF8: 5E                 pop         rsi
  0000000067130FF9: 5D                 pop         rbp
  0000000067130FFA: 5B                 pop         rbx
  0000000067130FFB: F3 C3              rep ret

  Summary

      1A7000 .text
