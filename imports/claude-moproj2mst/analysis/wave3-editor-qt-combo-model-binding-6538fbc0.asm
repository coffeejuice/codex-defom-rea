Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538FBC0: 40 55              push        rbp
  000000006538FBC2: 56                 push        rsi
  000000006538FBC3: 57                 push        rdi
  000000006538FBC4: 41 54              push        r12
  000000006538FBC6: 41 55              push        r13
  000000006538FBC8: 48 83 EC 50        sub         rsp,50h
  000000006538FBCC: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006538FBD5: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006538FBDD: 48 8B DA           mov         rbx,rdx
  000000006538FBE0: 4C 8B E1           mov         r12,rcx
  000000006538FBE3: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006538FBE7: 48 85 D2           test        rdx,rdx
  000000006538FBEA: 75 12              jne         000000006538FBFE
  000000006538FBEC: 48 8D 0D 2D A2 36  lea         rcx,[00000000656F9E20h]
                    00
  000000006538FBF3: FF 15 9F 6D 30 00  call        qword ptr [0000000065696998h]
  000000006538FBF9: E9 22 04 00 00     jmp         0000000065390020
  000000006538FBFE: 48 8B 8E E8 01 00  mov         rcx,qword ptr [rsi+00000000000001E8h]
                    00
  000000006538FC05: 48 85 C9           test        rcx,rcx
  000000006538FC08: 74 36              je          000000006538FC40
  000000006538FC0A: E8 71 01 03 00     call        00000000653BFD80
  000000006538FC0F: 48 85 C0           test        rax,rax
  000000006538FC12: 74 2C              je          000000006538FC40
  000000006538FC14: 48 8B 8E E8 01 00  mov         rcx,qword ptr [rsi+00000000000001E8h]
                    00
  000000006538FC1B: E8 60 01 03 00     call        00000000653BFD80
  000000006538FC20: 48 3B 86 60 02 00  cmp         rax,qword ptr [rsi+0000000000000260h]
                    00
  000000006538FC27: 75 17              jne         000000006538FC40
  000000006538FC29: 48 8B 8E E8 01 00  mov         rcx,qword ptr [rsi+00000000000001E8h]
                    00
  000000006538FC30: E8 4B 01 03 00     call        00000000653BFD80
  000000006538FC35: 48 8B C8           mov         rcx,rax
  000000006538FC38: 48 8B D3           mov         rdx,rbx
  000000006538FC3B: E8 E0 94 22 00     call        00000000655B9120
  000000006538FC40: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FC47: 48 85 C9           test        rcx,rcx
  000000006538FC4A: 0F 84 12 01 00 00  je          000000006538FD62
  000000006538FC50: 4C 8D 0D 99 A1 36  lea         r9,[00000000656F9DF0h]
                    00
  000000006538FC57: 4D 8B C4           mov         r8,r12
  000000006538FC5A: 48 8D 15 67 A1 36  lea         rdx,[00000000656F9DC8h]
                    00
  000000006538FC61: FF 15 B9 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FC67: 4C 8D 0D 3A A1 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FC6E: 4D 8B C4           mov         r8,r12
  000000006538FC71: 48 8D 15 00 A1 36  lea         rdx,[00000000656F9D78h]
                    00
  000000006538FC78: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FC7F: FF 15 9B 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FC85: 4C 8D 0D C4 A0 36  lea         r9,[00000000656F9D50h]
                    00
  000000006538FC8C: 4D 8B C4           mov         r8,r12
  000000006538FC8F: 48 8D 15 92 A0 36  lea         rdx,[00000000656F9D28h]
                    00
  000000006538FC96: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FC9D: FF 15 7D 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FCA3: 4C 8D 0D FE A0 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FCAA: 4D 8B C4           mov         r8,r12
  000000006538FCAD: 48 8D 15 44 A0 36  lea         rdx,[00000000656F9CF8h]
                    00
  000000006538FCB4: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FCBB: FF 15 5F 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FCC1: 4C 8D 0D 08 A0 36  lea         r9,[00000000656F9CD0h]
                    00
  000000006538FCC8: 4D 8B C4           mov         r8,r12
  000000006538FCCB: 48 8D 15 D6 9F 36  lea         rdx,[00000000656F9CA8h]
                    00
  000000006538FCD2: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FCD9: FF 15 41 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FCDF: 4C 8D 0D 52 8C 36  lea         r9,[00000000656F8938h]
                    00
  000000006538FCE6: 4D 8B C4           mov         r8,r12
  000000006538FCE9: 48 8D 15 18 6D 31  lea         rdx,[00000000656A6A08h]
                    00
  000000006538FCF0: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FCF7: FF 15 23 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FCFD: 4C 8D 0D A4 A0 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FD04: 4D 8B C4           mov         r8,r12
  000000006538FD07: 48 8D 15 82 9F 36  lea         rdx,[00000000656F9C90h]
                    00
  000000006538FD0E: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FD15: FF 15 05 6A 30 00  call        qword ptr [0000000065696720h]
  000000006538FD1B: 4C 8D 0D 56 9F 36  lea         r9,[00000000656F9C78h]
                    00
  000000006538FD22: 4D 8B C4           mov         r8,r12
  000000006538FD25: 48 8D 15 3C 9F 36  lea         rdx,[00000000656F9C68h]
                    00
  000000006538FD2C: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FD33: FF 15 E7 69 30 00  call        qword ptr [0000000065696720h]
  000000006538FD39: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FD40: FF 15 22 6B 30 00  call        qword ptr [0000000065696868h]
  000000006538FD46: 49 3B C4           cmp         rax,r12
  000000006538FD49: 75 17              jne         000000006538FD62
  000000006538FD4B: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FD52: 48 85 C9           test        rcx,rcx
  000000006538FD55: 74 0B              je          000000006538FD62
  000000006538FD57: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006538FD5A: BA 01 00 00 00     mov         edx,1
  000000006538FD5F: FF 50 18           call        qword ptr [rax+18h]
  000000006538FD62: 48 89 9E E0 01 00  mov         qword ptr [rsi+00000000000001E0h],rbx
                    00
  000000006538FD69: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FD71: 4C 8D 0D 78 A0 36  lea         r9,[00000000656F9DF0h]
                    00
  000000006538FD78: 4D 8B C4           mov         r8,r12
  000000006538FD7B: 48 8D 15 46 A0 36  lea         rdx,[00000000656F9DC8h]
                    00
  000000006538FD82: 48 8B CB           mov         rcx,rbx
  000000006538FD85: FF 15 8D 69 30 00  call        qword ptr [0000000065696718h]
  000000006538FD8B: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FD93: 4C 8D 0D 0E A0 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FD9A: 4D 8B C4           mov         r8,r12
  000000006538FD9D: 48 8D 15 D4 9F 36  lea         rdx,[00000000656F9D78h]
                    00
  000000006538FDA4: 48 8B CB           mov         rcx,rbx
  000000006538FDA7: FF 15 6B 69 30 00  call        qword ptr [0000000065696718h]
  000000006538FDAD: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FDB5: 4C 8D 0D 94 9F 36  lea         r9,[00000000656F9D50h]
                    00
  000000006538FDBC: 4D 8B C4           mov         r8,r12
  000000006538FDBF: 48 8D 15 62 9F 36  lea         rdx,[00000000656F9D28h]
                    00
  000000006538FDC6: 48 8B CB           mov         rcx,rbx
  000000006538FDC9: FF 15 49 69 30 00  call        qword ptr [0000000065696718h]
  000000006538FDCF: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FDD7: 4C 8D 0D CA 9F 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FDDE: 4D 8B C4           mov         r8,r12
  000000006538FDE1: 48 8D 15 10 9F 36  lea         rdx,[00000000656F9CF8h]
                    00
  000000006538FDE8: 48 8B CB           mov         rcx,rbx
  000000006538FDEB: FF 15 27 69 30 00  call        qword ptr [0000000065696718h]
  000000006538FDF1: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FDF9: 4C 8D 0D D0 9E 36  lea         r9,[00000000656F9CD0h]
                    00
  000000006538FE00: 4D 8B C4           mov         r8,r12
  000000006538FE03: 48 8D 15 9E 9E 36  lea         rdx,[00000000656F9CA8h]
                    00
  000000006538FE0A: 48 8B CB           mov         rcx,rbx
  000000006538FE0D: FF 15 05 69 30 00  call        qword ptr [0000000065696718h]
  000000006538FE13: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FE1B: 4C 8D 0D 16 8B 36  lea         r9,[00000000656F8938h]
                    00
  000000006538FE22: 4D 8B C4           mov         r8,r12
  000000006538FE25: 48 8D 15 DC 6B 31  lea         rdx,[00000000656A6A08h]
                    00
  000000006538FE2C: 48 8B CB           mov         rcx,rbx
  000000006538FE2F: FF 15 E3 68 30 00  call        qword ptr [0000000065696718h]
  000000006538FE35: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FE3D: 4C 8D 0D 64 9F 36  lea         r9,[00000000656F9DA8h]
                    00
  000000006538FE44: 4D 8B C4           mov         r8,r12
  000000006538FE47: 48 8D 15 42 9E 36  lea         rdx,[00000000656F9C90h]
                    00
  000000006538FE4E: 48 8B CB           mov         rcx,rbx
  000000006538FE51: FF 15 C1 68 30 00  call        qword ptr [0000000065696718h]
  000000006538FE57: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006538FE5F: 4C 8D 0D 12 9E 36  lea         r9,[00000000656F9C78h]
                    00
  000000006538FE66: 4D 8B C4           mov         r8,r12
  000000006538FE69: 48 8D 15 F8 9D 36  lea         rdx,[00000000656F9C68h]
                    00
  000000006538FE70: 48 8B CB           mov         rcx,rbx
  000000006538FE73: FF 15 9F 68 30 00  call        qword ptr [0000000065696718h]
  000000006538FE79: 48 8B 8E F0 01 00  mov         rcx,qword ptr [rsi+00000000000001F0h]
                    00
  000000006538FE80: 48 85 C9           test        rcx,rcx
  000000006538FE83: 74 10              je          000000006538FE95
  000000006538FE85: 48 8B 49 60        mov         rcx,qword ptr [rcx+60h]
  000000006538FE89: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006538FE8C: 48 8B D3           mov         rdx,rbx
  000000006538FE8F: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000006538FE95: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006538FE9A: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FEA1: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538FEA4: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538FEAB: FF 15 9F 89 30 00  call        qword ptr [0000000065698850h]
  000000006538FEB1: 48 8B D0           mov         rdx,rax
  000000006538FEB4: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FEBB: FF 53 68           call        qword ptr [rbx+68h]
  000000006538FEBE: 85 C0              test        eax,eax
  000000006538FEC0: 0F 84 C5 00 00 00  je          000000006538FF8B
  000000006538FEC6: 33 ED              xor         ebp,ebp
  000000006538FEC8: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006538FECD: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FED4: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538FED7: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538FEDE: FF 15 6C 89 30 00  call        qword ptr [0000000065698850h]
  000000006538FEE4: 48 8B D0           mov         rdx,rax
  000000006538FEE7: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FEEE: FF 53 68           call        qword ptr [rbx+68h]
  000000006538FEF1: 85 C0              test        eax,eax
  000000006538FEF3: 0F 8E 92 00 00 00  jle         000000006538FF8B
  000000006538FEF9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006538FF00: 48 8B 86 E0 01 00  mov         rax,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FF07: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538FF0A: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538FF11: FF 15 39 89 30 00  call        qword ptr [0000000065698850h]
  000000006538FF17: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538FF1C: 44 8B 8E 18 02 00  mov         r9d,dword ptr [rsi+0000000000000218h]
                    00
  000000006538FF23: 44 8B C5           mov         r8d,ebp
  000000006538FF26: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538FF2B: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538FF32: FF 53 58           call        qword ptr [rbx+58h]
  000000006538FF35: 90                 nop
  000000006538FF36: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006538FF3E: 48 8B C8           mov         rcx,rax
  000000006538FF41: FF 15 F9 8B 30 00  call        qword ptr [0000000065698B40h]
  000000006538FF47: 8B 08              mov         ecx,dword ptr [rax]
  000000006538FF49: 83 E1 20           and         ecx,20h
  000000006538FF4C: 75 31              jne         000000006538FF7F
  000000006538FF4E: FF C5              inc         ebp
  000000006538FF50: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006538FF55: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FF5C: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538FF5F: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538FF66: FF 15 E4 88 30 00  call        qword ptr [0000000065698850h]
  000000006538FF6C: 48 8B D0           mov         rdx,rax
  000000006538FF6F: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FF76: FF 53 68           call        qword ptr [rbx+68h]
  000000006538FF79: 3B E8              cmp         ebp,eax
  000000006538FF7B: 7C 83              jl          000000006538FF00
  000000006538FF7D: EB 0C              jmp         000000006538FF8B
  000000006538FF7F: 8B D5              mov         edx,ebp
  000000006538FF81: 49 8B CC           mov         rcx,r12
  000000006538FF84: E8 C7 C1 FF FF     call        000000006538C150
  000000006538FF89: EB 4A              jmp         000000006538FFD5
  000000006538FF8B: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006538FF90: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FF97: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538FF9A: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538FFA1: FF 15 A9 88 30 00  call        qword ptr [0000000065698850h]
  000000006538FFA7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538FFAC: 44 8B 8F 18 02 00  mov         r9d,dword ptr [rdi+0000000000000218h]
                    00
  000000006538FFB3: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  000000006538FFB7: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538FFBC: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538FFC3: FF 53 58           call        qword ptr [rbx+58h]
  000000006538FFC6: 90                 nop
  000000006538FFC7: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538FFCC: 48 8B CF           mov         rcx,rdi
  000000006538FFCF: E8 1C AE FF FF     call        000000006538ADF0
  000000006538FFD4: 90                 nop
  000000006538FFD5: 48 8B 5E 08        mov         rbx,qword ptr [rsi+8]
  000000006538FFD9: 83 BE FC 01 00 00  cmp         dword ptr [rsi+00000000000001FCh],0
                    00
  000000006538FFE0: 75 3E              jne         0000000065390020
  000000006538FFE2: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006538FFEA: FF 15 88 67 30 00  call        qword ptr [0000000065696778h]
  000000006538FFF0: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006538FFF3: 48 89 96 28 02 00  mov         qword ptr [rsi+0000000000000228h],rdx
                    00
  000000006538FFFA: 48 8B CE           mov         rcx,rsi
  000000006538FFFD: E8 CE C5 FF FF     call        000000006538C5D0
  0000000065390002: 4C 8B 86 F0 01 00  mov         r8,qword ptr [rsi+00000000000001F0h]
                    00
  0000000065390009: BA 14 00 00 00     mov         edx,14h
  000000006539000E: 48 8D 48 48        lea         rcx,[rax+48h]
  0000000065390012: FF 15 60 6D 30 00  call        qword ptr [0000000065696D78h]
  0000000065390018: 48 8B CB           mov         rcx,rbx
  000000006539001B: E8 70 25 CD FF     call        0000000065062590
  0000000065390020: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000065390028: 48 83 C4 50        add         rsp,50h
  000000006539002C: 41 5D              pop         r13
  000000006539002E: 41 5C              pop         r12
  0000000065390030: 5F                 pop         rdi
  0000000065390031: 5E                 pop         rsi
  0000000065390032: 5D                 pop         rbp
  0000000065390033: C3                 ret

  Summary

      695000 .text
