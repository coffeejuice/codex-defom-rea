Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414879C4: 48 8B C4           mov         rax,rsp
  00000001414879C7: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000001414879CB: 4C 89 40 18        mov         qword ptr [rax+18h],r8
  00000001414879CF: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001414879D3: 55                 push        rbp
  00000001414879D4: 56                 push        rsi
  00000001414879D5: 57                 push        rdi
  00000001414879D6: 41 54              push        r12
  00000001414879D8: 41 55              push        r13
  00000001414879DA: 41 56              push        r14
  00000001414879DC: 41 57              push        r15
  00000001414879DE: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  00000001414879E5: 33 FF              xor         edi,edi
  00000001414879E7: 41 B4 80           mov         r12b,80h
  00000001414879EA: 41 8B F1           mov         esi,r9d
  00000001414879ED: 48 8B DA           mov         rbx,rdx
  00000001414879F0: C7 40 A8 18 00 00  mov         dword ptr [rax-58h],18h
                    00
  00000001414879F7: 8D 6F 01           lea         ebp,[rdi+1]
  00000001414879FA: 89 78 9C           mov         dword ptr [rax-64h],edi
  00000001414879FD: 40 88 BC 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],dil
                    00 00
  0000000141487A05: 48 89 78 B0        mov         qword ptr [rax-50h],rdi
  0000000141487A09: 45 84 CC           test        r12b,r9b
  0000000141487A0C: 74 08              je          0000000141487A16
  0000000141487A0E: 89 78 B8           mov         dword ptr [rax-48h],edi
  0000000141487A11: 41 B6 10           mov         r14b,10h
  0000000141487A14: EB 07              jmp         0000000141487A1D
  0000000141487A16: 89 6C 24 70        mov         dword ptr [rsp+70h],ebp
  0000000141487A1A: 44 8A F7           mov         r14b,dil
  0000000141487A1D: 48 8D 4C 24 54     lea         rcx,[rsp+54h]
  0000000141487A22: E8 E1 B1 00 00     call        0000000141492C08
  0000000141487A27: 3B C7              cmp         eax,edi
  0000000141487A29: 74 14              je          0000000141487A3F
  0000000141487A2B: 45 33 C9           xor         r9d,r9d
  0000000141487A2E: 45 33 C0           xor         r8d,r8d
  0000000141487A31: 33 D2              xor         edx,edx
  0000000141487A33: 33 C9              xor         ecx,ecx
  0000000141487A35: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000141487A3A: E8 01 8A FE FF     call        0000000141470440
  0000000141487A3F: 0F BA E6 0F        bt          esi,0Fh
  0000000141487A43: 72 15              jb          0000000141487A5A
  0000000141487A45: F7 C6 00 40 07 00  test        esi,74000h
  0000000141487A4B: 75 0A              jne         0000000141487A57
  0000000141487A4D: 81 7C 24 54 00 80  cmp         dword ptr [rsp+54h],8000h
                    00 00
  0000000141487A55: 74 03              je          0000000141487A5A
  0000000141487A57: 45 0A F4           or          r14b,r12b
  0000000141487A5A: 8B CE              mov         ecx,esi
  0000000141487A5C: 41 B8 03 00 00 00  mov         r8d,3
  0000000141487A62: B8 00 00 00 C0     mov         eax,0C0000000h
  0000000141487A67: 41 23 C8           and         ecx,r8d
  0000000141487A6A: BA 00 00 00 80     mov         edx,80000000h
  0000000141487A6F: 3B CF              cmp         ecx,edi
  0000000141487A71: 74 54              je          0000000141487AC7
  0000000141487A73: 2B CD              sub         ecx,ebp
  0000000141487A75: 74 3A              je          0000000141487AB1
  0000000141487A77: 3B CD              cmp         ecx,ebp
  0000000141487A79: 74 31              je          0000000141487AAC
  0000000141487A7B: E8 D4 C6 FE FF     call        0000000141474154
  0000000141487A80: 89 38              mov         dword ptr [rax],edi
  0000000141487A82: 83 0B FF           or          dword ptr [rbx],0FFFFFFFFh
  0000000141487A85: E8 AA C6 FE FF     call        0000000141474134
  0000000141487A8A: BB 16 00 00 00     mov         ebx,16h
  0000000141487A8F: 45 33 C9           xor         r9d,r9d
  0000000141487A92: 45 33 C0           xor         r8d,r8d
  0000000141487A95: 33 D2              xor         edx,edx
  0000000141487A97: 33 C9              xor         ecx,ecx
  0000000141487A99: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000141487A9E: 89 18              mov         dword ptr [rax],ebx
  0000000141487AA0: E8 C3 8A FE FF     call        0000000141470568
  0000000141487AA5: 8B C3              mov         eax,ebx
  0000000141487AA7: E9 FB 06 00 00     jmp         00000001414881A7
  0000000141487AAC: 44 8B F8           mov         r15d,eax
  0000000141487AAF: EB 19              jmp         0000000141487ACA
  0000000141487AB1: 40 F6 C6 08        test        sil,8
  0000000141487AB5: 74 08              je          0000000141487ABF
  0000000141487AB7: F7 C6 00 00 07 00  test        esi,70000h
  0000000141487ABD: 75 ED              jne         0000000141487AAC
  0000000141487ABF: 41 BF 00 00 00 40  mov         r15d,40000000h
  0000000141487AC5: EB 03              jmp         0000000141487ACA
  0000000141487AC7: 44 8B FA           mov         r15d,edx
  0000000141487ACA: 8B 8C 24 E0 00 00  mov         ecx,dword ptr [rsp+00000000000000E0h]
                    00
  0000000141487AD1: 41 BC 02 00 00 00  mov         r12d,2
  0000000141487AD7: 83 E9 10           sub         ecx,10h
  0000000141487ADA: 74 2C              je          0000000141487B08
  0000000141487ADC: 83 E9 10           sub         ecx,10h
  0000000141487ADF: 74 23              je          0000000141487B04
  0000000141487AE1: 83 E9 10           sub         ecx,10h
  0000000141487AE4: 74 19              je          0000000141487AFF
  0000000141487AE6: 83 E9 10           sub         ecx,10h
  0000000141487AE9: 74 0F              je          0000000141487AFA
  0000000141487AEB: 83 F9 40           cmp         ecx,40h
  0000000141487AEE: 75 8B              jne         0000000141487A7B
  0000000141487AF0: 44 3B FA           cmp         r15d,edx
  0000000141487AF3: 8B CF              mov         ecx,edi
  0000000141487AF5: 0F 94 C1           sete        cl
  0000000141487AF8: EB 10              jmp         0000000141487B0A
  0000000141487AFA: 41 8B C8           mov         ecx,r8d
  0000000141487AFD: EB 0B              jmp         0000000141487B0A
  0000000141487AFF: 41 8B CC           mov         ecx,r12d
  0000000141487B02: EB 06              jmp         0000000141487B0A
  0000000141487B04: 8B CD              mov         ecx,ebp
  0000000141487B06: EB 02              jmp         0000000141487B0A
  0000000141487B08: 8B CF              mov         ecx,edi
  0000000141487B0A: 8B C6              mov         eax,esi
  0000000141487B0C: BA 00 07 00 00     mov         edx,700h
  0000000141487B11: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  0000000141487B15: 23 C2              and         eax,edx
  0000000141487B17: 74 46              je          0000000141487B5F
  0000000141487B19: 3D 00 01 00 00     cmp         eax,100h
  0000000141487B1E: 74 38              je          0000000141487B58
  0000000141487B20: 3D 00 02 00 00     cmp         eax,200h
  0000000141487B25: 74 2A              je          0000000141487B51
  0000000141487B27: 3D 00 03 00 00     cmp         eax,300h
  0000000141487B2C: 74 1E              je          0000000141487B4C
  0000000141487B2E: 3D 00 04 00 00     cmp         eax,400h
  0000000141487B33: 74 2A              je          0000000141487B5F
  0000000141487B35: 3D 00 05 00 00     cmp         eax,500h
  0000000141487B3A: 74 26              je          0000000141487B62
  0000000141487B3C: 3D 00 06 00 00     cmp         eax,600h
  0000000141487B41: 74 0E              je          0000000141487B51
  0000000141487B43: 3B C2              cmp         eax,edx
  0000000141487B45: 74 1B              je          0000000141487B62
  0000000141487B47: E9 2F FF FF FF     jmp         0000000141487A7B
  0000000141487B4C: 41 8B EC           mov         ebp,r12d
  0000000141487B4F: EB 11              jmp         0000000141487B62
  0000000141487B51: BD 05 00 00 00     mov         ebp,5
  0000000141487B56: EB 0A              jmp         0000000141487B62
  0000000141487B58: BD 04 00 00 00     mov         ebp,4
  0000000141487B5D: EB 03              jmp         0000000141487B62
  0000000141487B5F: 41 8B E8           mov         ebp,r8d
  0000000141487B62: 0F BA E6 08        bt          esi,8
  0000000141487B66: 41 BD 80 00 00 00  mov         r13d,80h
  0000000141487B6C: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487B71: 73 1E              jae         0000000141487B91
  0000000141487B73: 8A 05 4F 7F 27 03  mov         al,byte ptr [00000001446FFAC8h]
  0000000141487B79: F6 D0              not         al
  0000000141487B7B: 22 84 24 E8 00 00  and         al,byte ptr [rsp+00000000000000E8h]
                    00
  0000000141487B82: A8 80              test        al,80h
  0000000141487B84: 41 8D 45 81        lea         eax,[r13-7Fh]
  0000000141487B88: 44 0F 44 E8        cmove       r13d,eax
  0000000141487B8C: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487B91: 40 F6 C6 40        test        sil,40h
  0000000141487B95: 74 16              je          0000000141487BAD
  0000000141487B97: 41 0F BA ED 1A     bts         r13d,1Ah
  0000000141487B9C: 41 0F BA EF 10     bts         r15d,10h
  0000000141487BA1: 83 C9 04           or          ecx,4
  0000000141487BA4: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BA9: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  0000000141487BAD: 0F BA E6 0C        bt          esi,0Ch
  0000000141487BB1: 73 0A              jae         0000000141487BBD
  0000000141487BB3: 41 0F BA ED 08     bts         r13d,8
  0000000141487BB8: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BBD: 40 F6 C6 20        test        sil,20h
  0000000141487BC1: 74 07              je          0000000141487BCA
  0000000141487BC3: 41 0F BA ED 1B     bts         r13d,1Bh
  0000000141487BC8: EB 0B              jmp         0000000141487BD5
  0000000141487BCA: 40 F6 C6 10        test        sil,10h
  0000000141487BCE: 74 0A              je          0000000141487BDA
  0000000141487BD0: 41 0F BA ED 1C     bts         r13d,1Ch
  0000000141487BD5: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BDA: E8 21 4D 00 00     call        000000014148C900
  0000000141487BDF: 89 03              mov         dword ptr [rbx],eax
  0000000141487BE1: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487BE4: 75 21              jne         0000000141487C07
  0000000141487BE6: E8 69 C5 FE FF     call        0000000141474154
  0000000141487BEB: 89 38              mov         dword ptr [rax],edi
  0000000141487BED: 83 0B FF           or          dword ptr [rbx],0FFFFFFFFh
  0000000141487BF0: E8 3F C5 FE FF     call        0000000141474134
  0000000141487BF5: C7 00 18 00 00 00  mov         dword ptr [rax],18h
  0000000141487BFB: E8 34 C5 FE FF     call        0000000141474134
  0000000141487C00: 8B 00              mov         eax,dword ptr [rax]
  0000000141487C02: E9 A0 05 00 00     jmp         00000001414881A7
  0000000141487C07: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000141487C0F: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141487C14: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000141487C1C: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141487C21: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  0000000141487C26: 41 8B D7           mov         edx,r15d
  0000000141487C29: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  0000000141487C2E: C7 00 01 00 00 00  mov         dword ptr [rax],1
  0000000141487C34: 89 6C 24 20        mov         dword ptr [rsp+20h],ebp
  0000000141487C38: FF 15 D2 B5 97 00  call        qword ptr [0000000141E03210h]
  0000000141487C3E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000141487C43: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487C47: 0F 85 8C 00 00 00  jne         0000000141487CD9
  0000000141487C4D: B9 00 00 00 C0     mov         ecx,0C0000000h
  0000000141487C52: 41 8B C7           mov         eax,r15d
  0000000141487C55: 23 C1              and         eax,ecx
  0000000141487C57: 3B C1              cmp         eax,ecx
  0000000141487C59: 75 43              jne         0000000141487C9E
  0000000141487C5B: B8 01 00 00 00     mov         eax,1
  0000000141487C60: 40 84 F0           test        al,sil
  0000000141487C63: 74 39              je          0000000141487C9E
  0000000141487C65: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141487C6A: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000141487C72: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141487C77: 41 0F BA F7 1F     btr         r15d,1Fh
  0000000141487C7C: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  0000000141487C81: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  0000000141487C86: 41 8B D7           mov         edx,r15d
  0000000141487C89: 89 6C 24 20        mov         dword ptr [rsp+20h],ebp
  0000000141487C8D: FF 15 7D B5 97 00  call        qword ptr [0000000141E03210h]
  0000000141487C93: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000141487C98: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487C9C: 75 3B              jne         0000000141487CD9
  0000000141487C9E: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487CA1: 4C 8D 2D 58 C4 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487CA8: 48 8B C1           mov         rax,rcx
  0000000141487CAB: 83 E1 1F           and         ecx,1Fh
  0000000141487CAE: 48 C1 F8 05        sar         rax,5
  0000000141487CB2: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487CB6: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487CBB: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  0000000141487CC0: FF 15 22 B9 97 00  call        qword ptr [0000000141E035E8h]
  0000000141487CC6: 8B C8              mov         ecx,eax
  0000000141487CC8: E8 A7 C4 FE FF     call        0000000141474174
  0000000141487CCD: E8 62 C4 FE FF     call        0000000141474134
  0000000141487CD2: 8B 38              mov         edi,dword ptr [rax]
  0000000141487CD4: E9 CC 04 00 00     jmp         00000001414881A5
  0000000141487CD9: 48 8B C8           mov         rcx,rax
  0000000141487CDC: FF 15 FE B6 97 00  call        qword ptr [0000000141E033E0h]
  0000000141487CE2: 3B C7              cmp         eax,edi
  0000000141487CE4: 75 4D              jne         0000000141487D33
  0000000141487CE6: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487CE9: 4C 8D 2D 10 C4 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487CF0: 48 8B C1           mov         rax,rcx
  0000000141487CF3: 83 E1 1F           and         ecx,1Fh
  0000000141487CF6: 48 C1 F8 05        sar         rax,5
  0000000141487CFA: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487CFE: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D03: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  0000000141487D08: FF 15 DA B8 97 00  call        qword ptr [0000000141E035E8h]
  0000000141487D0E: 8B C8              mov         ecx,eax
  0000000141487D10: 8B D8              mov         ebx,eax
  0000000141487D12: E8 5D C4 FE FF     call        0000000141474174
  0000000141487D17: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000141487D1C: FF 15 9E B8 97 00  call        qword ptr [0000000141E035C0h]
  0000000141487D22: 3B DF              cmp         ebx,edi
  0000000141487D24: 75 A7              jne         0000000141487CCD
  0000000141487D26: E8 09 C4 FE FF     call        0000000141474134
  0000000141487D2B: C7 00 0D 00 00 00  mov         dword ptr [rax],0Dh
  0000000141487D31: EB 9A              jmp         0000000141487CCD
  0000000141487D33: 41 3B C4           cmp         eax,r12d
  0000000141487D36: 75 06              jne         0000000141487D3E
  0000000141487D38: 41 80 CE 40        or          r14b,40h
  0000000141487D3C: EB 09              jmp         0000000141487D47
  0000000141487D3E: 83 F8 03           cmp         eax,3
  0000000141487D41: 75 04              jne         0000000141487D47
  0000000141487D43: 41 80 CE 08        or          r14b,8
  0000000141487D47: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000141487D4C: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487D4E: E8 FD 48 00 00     call        000000014148C650
  0000000141487D53: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487D56: 4C 8D 2D A3 C3 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487D5D: 48 8B C1           mov         rax,rcx
  0000000141487D60: 83 E1 1F           and         ecx,1Fh
  0000000141487D63: BA 01 00 00 00     mov         edx,1
  0000000141487D68: 48 C1 F8 05        sar         rax,5
  0000000141487D6C: 44 0A F2           or          r14b,dl
  0000000141487D6F: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D74: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487D78: 44 88 74 08 08     mov         byte ptr [rax+rcx+8],r14b
  0000000141487D7D: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487D80: 48 8B C1           mov         rax,rcx
  0000000141487D83: 83 E1 1F           and         ecx,1Fh
  0000000141487D86: 48 C1 F8 05        sar         rax,5
  0000000141487D8A: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487D8E: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D93: 80 64 08 38 80     and         byte ptr [rax+rcx+38h],80h
  0000000141487D98: 41 8A C6           mov         al,r14b
  0000000141487D9B: 24 48              and         al,48h
  0000000141487D9D: 88 44 24 4C        mov         byte ptr [rsp+4Ch],al
  0000000141487DA1: 0F 85 86 00 00 00  jne         0000000141487E2D
  0000000141487DA7: 45 84 F6           test        r14b,r14b
  0000000141487DAA: 0F 89 C2 02 00 00  jns         0000000141488072
  0000000141487DB0: 41 84 F4           test        r12b,sil
  0000000141487DB3: 74 78              je          0000000141487E2D
  0000000141487DB5: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DB7: 45 8B C4           mov         r8d,r12d
  0000000141487DBA: 83 CA FF           or          edx,0FFFFFFFFh
  0000000141487DBD: E8 62 B5 FF FF     call        0000000141483324
  0000000141487DC2: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000141487DC6: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487DC9: 75 19              jne         0000000141487DE4
  0000000141487DCB: E8 84 C3 FE FF     call        0000000141474154
  0000000141487DD0: 81 38 83 00 00 00  cmp         dword ptr [rax],83h
  0000000141487DD6: 74 50              je          0000000141487E28
  0000000141487DD8: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DDA: E8 D5 20 FF FF     call        0000000141479EB4
  0000000141487DDF: E9 E9 FE FF FF     jmp         0000000141487CCD
  0000000141487DE4: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DE6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000141487DEB: 41 B8 01 00 00 00  mov         r8d,1
  0000000141487DF1: 40 88 7C 24 40     mov         byte ptr [rsp+40h],dil
  0000000141487DF6: E8 FD 15 00 00     call        00000001414893F8
  0000000141487DFB: 3B C7              cmp         eax,edi
  0000000141487DFD: 75 18              jne         0000000141487E17
  0000000141487DFF: 80 7C 24 40 1A     cmp         byte ptr [rsp+40h],1Ah
  0000000141487E04: 75 11              jne         0000000141487E17
  0000000141487E06: 48 63 54 24 44     movsxd      rdx,dword ptr [rsp+44h]
  0000000141487E0B: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487E0D: E8 8E AB 00 00     call        00000001414929A0
  0000000141487E12: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487E15: 74 C1              je          0000000141487DD8
  0000000141487E17: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487E19: 45 33 C0           xor         r8d,r8d
  0000000141487E1C: 33 D2              xor         edx,edx
  0000000141487E1E: E8 01 B5 FF FF     call        0000000141483324
  0000000141487E23: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487E26: 74 B0              je          0000000141487DD8
  0000000141487E28: BA 01 00 00 00     mov         edx,1
  0000000141487E2D: 45 84 F6           test        r14b,r14b
  0000000141487E30: 0F 89 3C 02 00 00  jns         0000000141488072
  0000000141487E36: B9 00 40 07 00     mov         ecx,74000h
  0000000141487E3B: 85 F1              test        ecx,esi
  0000000141487E3D: 75 10              jne         0000000141487E4F
  0000000141487E3F: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  0000000141487E43: 23 C1              and         eax,ecx
  0000000141487E45: 75 06              jne         0000000141487E4D
  0000000141487E47: 0F BA EE 0E        bts         esi,0Eh
  0000000141487E4B: EB 02              jmp         0000000141487E4F
  0000000141487E4D: 0B F0              or          esi,eax
  0000000141487E4F: 8B C6              mov         eax,esi
  0000000141487E51: 23 C1              and         eax,ecx
  0000000141487E53: 3D 00 40 00 00     cmp         eax,4000h
  0000000141487E58: 74 4A              je          0000000141487EA4
  0000000141487E5A: 3D 00 00 01 00     cmp         eax,10000h
  0000000141487E5F: 74 2C              je          0000000141487E8D
  0000000141487E61: 3D 00 40 01 00     cmp         eax,14000h
  0000000141487E66: 74 25              je          0000000141487E8D
  0000000141487E68: 3D 00 00 02 00     cmp         eax,20000h
  0000000141487E6D: 74 2B              je          0000000141487E9A
  0000000141487E6F: 3D 00 40 02 00     cmp         eax,24000h
  0000000141487E74: 74 24              je          0000000141487E9A
  0000000141487E76: 3D 00 00 04 00     cmp         eax,40000h
  0000000141487E7B: 74 07              je          0000000141487E84
  0000000141487E7D: 3D 00 40 04 00     cmp         eax,44000h
  0000000141487E82: 75 28              jne         0000000141487EAC
  0000000141487E84: 88 94 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],dl
                    00
  0000000141487E8B: EB 1F              jmp         0000000141487EAC
  0000000141487E8D: B9 01 03 00 00     mov         ecx,301h
  0000000141487E92: 8B C6              mov         eax,esi
  0000000141487E94: 23 C1              and         eax,ecx
  0000000141487E96: 3B C1              cmp         eax,ecx
  0000000141487E98: 75 12              jne         0000000141487EAC
  0000000141487E9A: 44 88 A4 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r12b
                    00 00
  0000000141487EA2: EB 08              jmp         0000000141487EAC
  0000000141487EA4: 40 88 BC 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],dil
                    00 00
  0000000141487EAC: F7 C6 00 00 07 00  test        esi,70000h
  0000000141487EB2: 0F 84 BA 01 00 00  je          0000000141488072
  0000000141487EB8: 89 7C 24 44        mov         dword ptr [rsp+44h],edi
  0000000141487EBC: 41 F6 C6 40        test        r14b,40h
  0000000141487EC0: 0F 85 AC 01 00 00  jne         0000000141488072
  0000000141487EC6: 41 8B C7           mov         eax,r15d
  0000000141487EC9: 25 00 00 00 C0     and         eax,0C0000000h
  0000000141487ECE: 3D 00 00 00 40     cmp         eax,40000000h
  0000000141487ED3: 0F 84 24 01 00 00  je          0000000141487FFD
  0000000141487ED9: 3D 00 00 00 80     cmp         eax,80000000h
  0000000141487EDE: 74 7C              je          0000000141487F5C
  0000000141487EE0: 3D 00 00 00 C0     cmp         eax,0C0000000h
  0000000141487EE5: 0F 85 87 01 00 00  jne         0000000141488072
  0000000141487EEB: 3B EF              cmp         ebp,edi
  0000000141487EED: 0F 86 7F 01 00 00  jbe         0000000141488072
  0000000141487EF3: 41 3B EC           cmp         ebp,r12d
  0000000141487EF6: 76 0E              jbe         0000000141487F06
  0000000141487EF8: 83 FD 04           cmp         ebp,4
  0000000141487EFB: 76 38              jbe         0000000141487F35
  0000000141487EFD: 83 FD 05           cmp         ebp,5
  0000000141487F00: 0F 85 6C 01 00 00  jne         0000000141488072
  0000000141487F06: 0F BE 8C 24 D8 00  movsx       ecx,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000141487F0E: 41 B8 01 00 00 00  mov         r8d,1
  0000000141487F14: 8B EF              mov         ebp,edi
  0000000141487F16: 41 2B C8           sub         ecx,r8d
  0000000141487F19: 0F 84 20 01 00 00  je          000000014148803F
  0000000141487F1F: 41 3B C8           cmp         ecx,r8d
  0000000141487F22: 0F 85 50 01 00 00  jne         0000000141488078
  0000000141487F28: C7 44 24 44 FF FE  mov         dword ptr [rsp+44h],0FEFFh
                    00 00
  0000000141487F30: E9 18 01 00 00     jmp         000000014148804D
  0000000141487F35: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F37: 45 8B C4           mov         r8d,r12d
  0000000141487F3A: 33 D2              xor         edx,edx
  0000000141487F3C: E8 1F 83 00 00     call        0000000141490260
  0000000141487F41: 48 3B C7           cmp         rax,rdi
  0000000141487F44: 74 C0              je          0000000141487F06
  0000000141487F46: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F48: 45 33 C0           xor         r8d,r8d
  0000000141487F4B: 33 D2              xor         edx,edx
  0000000141487F4D: E8 0E 83 00 00     call        0000000141490260
  0000000141487F52: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487F56: 0F 84 7C FE FF FF  je          0000000141487DD8
  0000000141487F5C: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F5E: 48 8D 54 24 44     lea         rdx,[rsp+44h]
  0000000141487F63: 41 B8 03 00 00 00  mov         r8d,3
  0000000141487F69: E8 8A 14 00 00     call        00000001414893F8
  0000000141487F6E: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487F71: 0F 84 61 FE FF FF  je          0000000141487DD8
  0000000141487F77: 41 3B C4           cmp         eax,r12d
  0000000141487F7A: 74 20              je          0000000141487F9C
  0000000141487F7C: 83 F8 03           cmp         eax,3
  0000000141487F7F: 75 6B              jne         0000000141487FEC
  0000000141487F81: 81 7C 24 44 EF BB  cmp         dword ptr [rsp+44h],0BFBBEFh
                    BF 00
  0000000141487F89: 75 11              jne         0000000141487F9C
  0000000141487F8B: 44 8D 40 FE        lea         r8d,[rax-2]
  0000000141487F8F: 44 88 84 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r8b
                    00 00
  0000000141487F97: E9 DC 00 00 00     jmp         0000000141488078
  0000000141487F9C: 0F B7 44 24 44     movzx       eax,word ptr [rsp+44h]
  0000000141487FA1: 3D FE FF 00 00     cmp         eax,0FFFEh
  0000000141487FA6: 75 1A              jne         0000000141487FC2
  0000000141487FA8: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FAA: E8 05 1F FF FF     call        0000000141479EB4
  0000000141487FAF: E8 80 C1 FE FF     call        0000000141474134
  0000000141487FB4: BB 16 00 00 00     mov         ebx,16h
  0000000141487FB9: 89 18              mov         dword ptr [rax],ebx
  0000000141487FBB: 8B FB              mov         edi,ebx
  0000000141487FBD: E9 E3 01 00 00     jmp         00000001414881A5
  0000000141487FC2: 3D FF FE 00 00     cmp         eax,0FEFFh
  0000000141487FC7: 75 23              jne         0000000141487FEC
  0000000141487FC9: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FCB: 45 33 C0           xor         r8d,r8d
  0000000141487FCE: 41 8B D4           mov         edx,r12d
  0000000141487FD1: E8 4E B3 FF FF     call        0000000141483324
  0000000141487FD6: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487FD9: 0F 84 F9 FD FF FF  je          0000000141487DD8
  0000000141487FDF: 44 88 A4 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r12b
                    00 00
  0000000141487FE7: E9 86 00 00 00     jmp         0000000141488072
  0000000141487FEC: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FEE: 45 33 C0           xor         r8d,r8d
  0000000141487FF1: 33 D2              xor         edx,edx
  0000000141487FF3: E8 2C B3 FF FF     call        0000000141483324
  0000000141487FF8: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487FFB: EB 3B              jmp         0000000141488038
  0000000141487FFD: 3B EF              cmp         ebp,edi
  0000000141487FFF: 76 71              jbe         0000000141488072
  0000000141488001: 41 3B EC           cmp         ebp,r12d
  0000000141488004: 0F 86 FC FE FF FF  jbe         0000000141487F06
  000000014148800A: 83 FD 04           cmp         ebp,4
  000000014148800D: 0F 87 EA FE FF FF  ja          0000000141487EFD
  0000000141488013: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141488015: 45 8B C4           mov         r8d,r12d
  0000000141488018: 33 D2              xor         edx,edx
  000000014148801A: E8 41 82 00 00     call        0000000141490260
  000000014148801F: 48 3B C7           cmp         rax,rdi
  0000000141488022: 0F 84 DE FE FF FF  je          0000000141487F06
  0000000141488028: 8B 0B              mov         ecx,dword ptr [rbx]
  000000014148802A: 45 33 C0           xor         r8d,r8d
  000000014148802D: 33 D2              xor         edx,edx
  000000014148802F: E8 2C 82 00 00     call        0000000141490260
  0000000141488034: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141488038: 75 38              jne         0000000141488072
  000000014148803A: E9 99 FD FF FF     jmp         0000000141487DD8
  000000014148803F: C7 44 24 44 EF BB  mov         dword ptr [rsp+44h],0BFBBEFh
                    BF 00
  0000000141488047: 41 BC 03 00 00 00  mov         r12d,3
  000000014148804D: 8B 0B              mov         ecx,dword ptr [rbx]
  000000014148804F: 48 63 C5           movsxd      rax,ebp
  0000000141488052: 45 8B C4           mov         r8d,r12d
  0000000141488055: 48 8D 54 04 44     lea         rdx,[rsp+rax+44h]
  000000014148805A: 44 2B C5           sub         r8d,ebp
  000000014148805D: E8 46 D4 FF FF     call        00000001414854A8
  0000000141488062: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141488065: 0F 84 6D FD FF FF  je          0000000141487DD8
  000000014148806B: 03 E8              add         ebp,eax
  000000014148806D: 44 3B E5           cmp         r12d,ebp
  0000000141488070: 7F DB              jg          000000014148804D
  0000000141488072: 41 B8 01 00 00 00  mov         r8d,1
  0000000141488078: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  000000014148807B: 40 8A AC 24 D8 00  mov         bpl,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000141488083: 48 8B C1           mov         rax,rcx
  0000000141488086: 83 E1 1F           and         ecx,1Fh
  0000000141488089: 40 80 E5 7F        and         bpl,7Fh
  000000014148808D: 48 C1 F8 05        sar         rax,5
  0000000141488091: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141488095: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  000000014148809A: 80 64 08 38 80     and         byte ptr [rax+rcx+38h],80h
  000000014148809F: 40 08 6C 08 38     or          byte ptr [rax+rcx+38h],bpl
  00000001414880A4: 48 63 13           movsxd      rdx,dword ptr [rbx]
  00000001414880A7: 48 8B C2           mov         rax,rdx
  00000001414880AA: 83 E2 1F           and         edx,1Fh
  00000001414880AD: 48 C1 F8 05        sar         rax,5
  00000001414880B1: 48 6B D2 58        imul        rdx,rdx,58h
  00000001414880B5: 49 8B 4C C5 00     mov         rcx,qword ptr [r13+rax*8]
  00000001414880BA: 8B C6              mov         eax,esi
  00000001414880BC: 80 64 11 38 7F     and         byte ptr [rcx+rdx+38h],7Fh
  00000001414880C1: C1 E8 10           shr         eax,10h
  00000001414880C4: C0 E0 07           shl         al,7
  00000001414880C7: 08 44 11 38        or          byte ptr [rcx+rdx+38h],al
  00000001414880CB: 40 38 7C 24 4C     cmp         byte ptr [rsp+4Ch],dil
  00000001414880D0: 75 21              jne         00000001414880F3
  00000001414880D2: 40 F6 C6 08        test        sil,8
  00000001414880D6: 74 1B              je          00000001414880F3
  00000001414880D8: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  00000001414880DB: 48 8B C1           mov         rax,rcx
  00000001414880DE: 83 E1 1F           and         ecx,1Fh
  00000001414880E1: 48 C1 F8 05        sar         rax,5
  00000001414880E5: 48 6B C9 58        imul        rcx,rcx,58h
  00000001414880E9: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  00000001414880EE: 80 4C 08 08 20     or          byte ptr [rax+rcx+8],20h
  00000001414880F3: 41 8B C7           mov         eax,r15d
  00000001414880F6: 25 00 00 00 C0     and         eax,0C0000000h
  00000001414880FB: 3D 00 00 00 C0     cmp         eax,0C0000000h
  0000000141488100: 0F 85 9F 00 00 00  jne         00000001414881A5
  0000000141488106: 41 84 F0           test        r8b,sil
  0000000141488109: 0F 84 96 00 00 00  je          00000001414881A5
  000000014148810F: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000141488114: FF 15 A6 B4 97 00  call        qword ptr [0000000141E035C0h]
  000000014148811A: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014148811E: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141488123: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014148812B: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141488130: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000141488134: 41 0F BA F7 1F     btr         r15d,1Fh
  0000000141488139: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  000000014148813E: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000141488146: 41 8B D7           mov         edx,r15d
  0000000141488149: FF 15 C1 B0 97 00  call        qword ptr [0000000141E03210h]
  000000014148814F: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141488153: 75 36              jne         000000014148818B
  0000000141488155: FF 15 8D B4 97 00  call        qword ptr [0000000141E035E8h]
  000000014148815B: 8B C8              mov         ecx,eax
  000000014148815D: E8 12 C0 FE FF     call        0000000141474174
  0000000141488162: 4C 63 1B           movsxd      r11,dword ptr [rbx]
  0000000141488165: 49 8B C3           mov         rax,r11
  0000000141488168: 41 83 E3 1F        and         r11d,1Fh
  000000014148816C: 48 C1 F8 05        sar         rax,5
  0000000141488170: 4D 6B DB 58        imul        r11,r11,58h
  0000000141488174: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141488179: 42 80 64 18 08 FE  and         byte ptr [rax+r11+8],0FEh
  000000014148817F: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141488181: E8 7A 45 00 00     call        000000014148C700
  0000000141488186: E9 42 FB FF FF     jmp         0000000141487CCD
  000000014148818B: 48 63 13           movsxd      rdx,dword ptr [rbx]
  000000014148818E: 48 8B CA           mov         rcx,rdx
  0000000141488191: 83 E2 1F           and         edx,1Fh
  0000000141488194: 48 C1 F9 05        sar         rcx,5
  0000000141488198: 48 6B D2 58        imul        rdx,rdx,58h
  000000014148819C: 49 8B 4C CD 00     mov         rcx,qword ptr [r13+rcx*8]
  00000001414881A1: 48 89 04 0A        mov         qword ptr [rdx+rcx],rax
  00000001414881A5: 8B C7              mov         eax,edi
  00000001414881A7: 48 8B 9C 24 C8 00  mov         rbx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001414881AF: 48 81 C4 80 00 00  add         rsp,80h
                    00
  00000001414881B6: 41 5F              pop         r15
  00000001414881B8: 41 5E              pop         r14
  00000001414881BA: 41 5D              pop         r13
  00000001414881BC: 41 5C              pop         r12
  00000001414881BE: 5F                 pop         rdi
  00000001414881BF: 5E                 pop         rsi
  00000001414881C0: 5D                 pop         rbp
  00000001414881C1: C3                 ret

  Summary

     1E02000 .text
