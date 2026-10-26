Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714F570: 48 8B C4           mov         rax,rsp
  000000006714F573: 4C 89 48 20        mov         qword ptr [rax+20h],r9
  000000006714F577: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000006714F57B: 55                 push        rbp
  000000006714F57C: 56                 push        rsi
  000000006714F57D: 57                 push        rdi
  000000006714F57E: 41 54              push        r12
  000000006714F580: 41 55              push        r13
  000000006714F582: 41 56              push        r14
  000000006714F584: 41 57              push        r15
  000000006714F586: 48 83 EC 30        sub         rsp,30h
  000000006714F58A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714F593: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006714F597: 49 8B E9           mov         rbp,r9
  000000006714F59A: 49 63 D8           movsxd      rbx,r8d
  000000006714F59D: 4C 8B E2           mov         r12,rdx
  000000006714F5A0: 4C 8B E9           mov         r13,rcx
  000000006714F5A3: C7 40 18 00 00 00  mov         dword ptr [rax+18h],0
                    00
  000000006714F5AA: 8B B4 24 90 00 00  mov         esi,dword ptr [rsp+0000000000000090h]
                    00
  000000006714F5B1: 33 FF              xor         edi,edi
  000000006714F5B3: 45 32 F6           xor         r14b,r14b
  000000006714F5B6: 44 8D 7F 02        lea         r15d,[rdi+2]
  000000006714F5BA: 4D 85 C9           test        r9,r9
  000000006714F5BD: 74 2C              je          000000006714F5EB
  000000006714F5BF: 41 8B 01           mov         eax,dword ptr [r9]
  000000006714F5C2: 83 E0 01           and         eax,1
  000000006714F5C5: 41 0F 95 C6        setne       r14b
  000000006714F5C9: 85 F6              test        esi,esi
  000000006714F5CB: 75 04              jne         000000006714F5D1
  000000006714F5CD: 41 8B 71 0C        mov         esi,dword ptr [r9+0Ch]
  000000006714F5D1: 41 8B 79 04        mov         edi,dword ptr [r9+4]
  000000006714F5D5: 41 8B 41 10        mov         eax,dword ptr [r9+10h]
  000000006714F5D9: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  000000006714F5E0: 45 84 F6           test        r14b,r14b
  000000006714F5E3: 74 06              je          000000006714F5EB
  000000006714F5E5: 85 F6              test        esi,esi
  000000006714F5E7: 41 0F 44 F7        cmove       esi,r15d
  000000006714F5EB: 48 8D 05 3E 15 1A  lea         rax,[00000000672F0B30h]
                    00
  000000006714F5F2: 48 89 01           mov         qword ptr [rcx],rax
  000000006714F5F5: F0 83 05 33 15 1A  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  000000006714F5FD: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  000000006714F608: 8D 14 1F           lea         edx,[rdi+rbx]
  000000006714F60B: D1 FA              sar         edx,1
  000000006714F60D: 83 E2 FE           and         edx,0FFFFFFFEh
  000000006714F610: E8 AB 33 F2 FF     call        00000000670729C0
  000000006714F615: 4D 8B 5D 00        mov         r11,qword ptr [r13]
  000000006714F619: 4D 8B 53 10        mov         r10,qword ptr [r11+10h]
  000000006714F61D: 4D 8D 1C 1C        lea         r11,[r12+rbx]
  000000006714F621: 48 63 CF           movsxd      rcx,edi
  000000006714F624: 4D 3B E3           cmp         r12,r11
  000000006714F627: 0F 83 85 01 00 00  jae         000000006714F7B2
  000000006714F62D: BD 40 28 00 00     mov         ebp,2840h
  000000006714F632: 41 BD FF 03 00 00  mov         r13d,3FFh
  000000006714F638: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  000000006714F640: 41 0F B6 04 24     movzx       eax,byte ptr [r12]
  000000006714F645: 88 84 0C 90 00 00  mov         byte ptr [rsp+rcx+0000000000000090h],al
                    00
  000000006714F64C: FF C7              inc         edi
  000000006714F64E: 48 FF C1           inc         rcx
  000000006714F651: 49 FF C4           inc         r12
  000000006714F654: 8B 9C 24 90 00 00  mov         ebx,dword ptr [rsp+0000000000000090h]
                    00
  000000006714F65B: 48 83 F9 04        cmp         rcx,4
  000000006714F65F: 0F 85 35 01 00 00  jne         000000006714F79A
  000000006714F665: 0F B6 94 24 93 00  movzx       edx,byte ptr [rsp+0000000000000093h]
                    00 00
  000000006714F66D: 44 0F B6 84 24 92  movzx       r8d,byte ptr [rsp+0000000000000092h]
                    00 00 00
  000000006714F676: 44 0F B6 8C 24 91  movzx       r9d,byte ptr [rsp+0000000000000091h]
                    00 00 00
  000000006714F67F: 45 84 F6           test        r14b,r14b
  000000006714F682: 0F 85 9A 00 00 00  jne         000000006714F722
  000000006714F688: 85 F6              test        esi,esi
  000000006714F68A: 75 47              jne         000000006714F6D3
  000000006714F68C: 80 FB FF           cmp         bl,0FFh
  000000006714F68F: 75 1B              jne         000000006714F6AC
  000000006714F691: 41 80 F9 FE        cmp         r9b,0FEh
  000000006714F695: 75 37              jne         000000006714F6CE
  000000006714F697: 45 84 C0           test        r8b,r8b
  000000006714F69A: 75 32              jne         000000006714F6CE
  000000006714F69C: 84 D2              test        dl,dl
  000000006714F69E: 75 2E              jne         000000006714F6CE
  000000006714F6A0: 41 8B F7           mov         esi,r15d
  000000006714F6A3: 33 FF              xor         edi,edi
  000000006714F6A5: 33 C9              xor         ecx,ecx
  000000006714F6A7: E9 EE 00 00 00     jmp         000000006714F79A
  000000006714F6AC: 84 DB              test        bl,bl
  000000006714F6AE: 75 1E              jne         000000006714F6CE
  000000006714F6B0: 45 84 C9           test        r9b,r9b
  000000006714F6B3: 75 19              jne         000000006714F6CE
  000000006714F6B5: 41 80 F8 FE        cmp         r8b,0FEh
  000000006714F6B9: 75 13              jne         000000006714F6CE
  000000006714F6BB: 80 FA FF           cmp         dl,0FFh
  000000006714F6BE: 75 0E              jne         000000006714F6CE
  000000006714F6C0: BE 01 00 00 00     mov         esi,1
  000000006714F6C5: 33 FF              xor         edi,edi
  000000006714F6C7: 33 C9              xor         ecx,ecx
  000000006714F6C9: E9 CC 00 00 00     jmp         000000006714F79A
  000000006714F6CE: 41 8B F7           mov         esi,r15d
  000000006714F6D1: EB 73              jmp         000000006714F746
  000000006714F6D3: 83 FE 01           cmp         esi,1
  000000006714F6D6: 75 1F              jne         000000006714F6F7
  000000006714F6D8: 41 0F B6 C8        movzx       ecx,r8b
  000000006714F6DC: C1 E1 08           shl         ecx,8
  000000006714F6DF: 0F B6 C3           movzx       eax,bl
  000000006714F6E2: C1 E0 18           shl         eax,18h
  000000006714F6E5: 0B C8              or          ecx,eax
  000000006714F6E7: 41 0F B6 C1        movzx       eax,r9b
  000000006714F6EB: C1 E0 10           shl         eax,10h
  000000006714F6EE: 0B C8              or          ecx,eax
  000000006714F6F0: 0F B6 C2           movzx       eax,dl
  000000006714F6F3: 0B C8              or          ecx,eax
  000000006714F6F5: EB 1D              jmp         000000006714F714
  000000006714F6F7: 0F B6 CA           movzx       ecx,dl
  000000006714F6FA: C1 E1 18           shl         ecx,18h
  000000006714F6FD: 41 0F B6 C1        movzx       eax,r9b
  000000006714F701: C1 E0 08           shl         eax,8
  000000006714F704: 0B C8              or          ecx,eax
  000000006714F706: 41 0F B6 C0        movzx       eax,r8b
  000000006714F70A: C1 E0 10           shl         eax,10h
  000000006714F70D: 0B C8              or          ecx,eax
  000000006714F70F: 0F B6 C3           movzx       eax,bl
  000000006714F712: 0B C8              or          ecx,eax
  000000006714F714: 81 F9 FF FE 00 00  cmp         ecx,0FEFFh
  000000006714F71A: 75 06              jne         000000006714F722
  000000006714F71C: 33 FF              xor         edi,edi
  000000006714F71E: 33 C9              xor         ecx,ecx
  000000006714F720: EB 78              jmp         000000006714F79A
  000000006714F722: 83 FE 01           cmp         esi,1
  000000006714F725: 75 1F              jne         000000006714F746
  000000006714F727: 41 0F B6 C8        movzx       ecx,r8b
  000000006714F72B: C1 E1 08           shl         ecx,8
  000000006714F72E: 0F B6 C3           movzx       eax,bl
  000000006714F731: C1 E0 18           shl         eax,18h
  000000006714F734: 0B C8              or          ecx,eax
  000000006714F736: 41 0F B6 C1        movzx       eax,r9b
  000000006714F73A: C1 E0 10           shl         eax,10h
  000000006714F73D: 0B C8              or          ecx,eax
  000000006714F73F: 0F B6 C2           movzx       eax,dl
  000000006714F742: 0B C8              or          ecx,eax
  000000006714F744: EB 1D              jmp         000000006714F763
  000000006714F746: 0F B6 CA           movzx       ecx,dl
  000000006714F749: C1 E1 18           shl         ecx,18h
  000000006714F74C: 41 0F B6 C1        movzx       eax,r9b
  000000006714F750: C1 E0 08           shl         eax,8
  000000006714F753: 0B C8              or          ecx,eax
  000000006714F755: 41 0F B6 C0        movzx       eax,r8b
  000000006714F759: C1 E0 10           shl         eax,10h
  000000006714F75C: 0B C8              or          ecx,eax
  000000006714F75E: 0F B6 C3           movzx       eax,bl
  000000006714F761: 0B C8              or          ecx,eax
  000000006714F763: 81 F9 00 00 01 00  cmp         ecx,10000h
  000000006714F769: 72 23              jb          000000006714F78E
  000000006714F76B: 8B C1              mov         eax,ecx
  000000006714F76D: C1 E8 0A           shr         eax,0Ah
  000000006714F770: 66 2B C5           sub         ax,bp
  000000006714F773: 66 41 89 02        mov         word ptr [r10],ax
  000000006714F777: 66 41 23 CD        and         cx,r13w
  000000006714F77B: B8 00 24 00 00     mov         eax,2400h
  000000006714F780: 66 2B C8           sub         cx,ax
  000000006714F783: 66 41 89 4A 02     mov         word ptr [r10+2],cx
  000000006714F788: 49 83 C2 04        add         r10,4
  000000006714F78C: EB 08              jmp         000000006714F796
  000000006714F78E: 66 41 89 0A        mov         word ptr [r10],cx
  000000006714F792: 49 83 C2 02        add         r10,2
  000000006714F796: 33 FF              xor         edi,edi
  000000006714F798: 33 C9              xor         ecx,ecx
  000000006714F79A: 4D 3B E3           cmp         r12,r11
  000000006714F79D: 0F 82 9D FE FF FF  jb          000000006714F640
  000000006714F7A3: 48 8B AC 24 88 00  mov         rbp,qword ptr [rsp+0000000000000088h]
                    00 00
  000000006714F7AB: 4C 8B 6C 24 70     mov         r13,qword ptr [rsp+70h]
  000000006714F7B0: EB 07              jmp         000000006714F7B9
  000000006714F7B2: 8B 9C 24 90 00 00  mov         ebx,dword ptr [rsp+0000000000000090h]
                    00
  000000006714F7B9: 49 8B 45 00        mov         rax,qword ptr [r13]
  000000006714F7BD: 4C 2B 50 10        sub         r10,qword ptr [rax+10h]
  000000006714F7C1: 49 D1 FA           sar         r10,1
  000000006714F7C4: 41 8B D2           mov         edx,r10d
  000000006714F7C7: 49 8B CD           mov         rcx,r13
  000000006714F7CA: E8 C1 4E F2 FF     call        0000000067074690
  000000006714F7CF: 48 85 ED           test        rbp,rbp
  000000006714F7D2: 74 17              je          000000006714F7EB
  000000006714F7D4: 45 84 F6           test        r14b,r14b
  000000006714F7D7: 74 04              je          000000006714F7DD
  000000006714F7D9: 83 4D 00 01        or          dword ptr [rbp],1
  000000006714F7DD: 89 75 0C           mov         dword ptr [rbp+0Ch],esi
  000000006714F7E0: 89 7D 04           mov         dword ptr [rbp+4],edi
  000000006714F7E3: 89 5D 10           mov         dword ptr [rbp+10h],ebx
  000000006714F7E6: 49 8B C5           mov         rax,r13
  000000006714F7E9: EB 03              jmp         000000006714F7EE
  000000006714F7EB: 49 8B C5           mov         rax,r13
  000000006714F7EE: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  000000006714F7F3: 48 83 C4 30        add         rsp,30h
  000000006714F7F7: 41 5F              pop         r15
  000000006714F7F9: 41 5E              pop         r14
  000000006714F7FB: 41 5D              pop         r13
  000000006714F7FD: 41 5C              pop         r12
  000000006714F7FF: 5F                 pop         rdi
  000000006714F800: 5E                 pop         rsi
  000000006714F801: 5D                 pop         rbp
  000000006714F802: C3                 ret
  000000006714F803: CC                 int         3

  Summary

      1A7000 .text
