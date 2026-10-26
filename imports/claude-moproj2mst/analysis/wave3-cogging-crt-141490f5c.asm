Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141490F5C: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141490F61: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000141490F66: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000141490F6B: 57                 push        rdi
  0000000141490F6C: 48 83 EC 50        sub         rsp,50h
  0000000141490F70: 33 ED              xor         ebp,ebp
  0000000141490F72: 49 8B D8           mov         rbx,r8
  0000000141490F75: 48 8B F2           mov         rsi,rdx
  0000000141490F78: 48 8B F9           mov         rdi,rcx
  0000000141490F7B: 4C 3B C5           cmp         r8,rbp
  0000000141490F7E: 75 07              jne         0000000141490F87
  0000000141490F80: 33 C0              xor         eax,eax
  0000000141490F82: E9 26 01 00 00     jmp         00000001414910AD
  0000000141490F87: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000141490F8C: 49 8B D1           mov         rdx,r9
  0000000141490F8F: E8 88 26 FE FF     call        000000014147361C
  0000000141490F94: 4C 8B 5C 24 38     mov         r11,qword ptr [rsp+38h]
  0000000141490F99: 41 39 6B 08        cmp         dword ptr [r11+8],ebp
  0000000141490F9D: 75 13              jne         0000000141490FB2
  0000000141490F9F: 4C 8B C3           mov         r8,rbx
  0000000141490FA2: 48 8B D6           mov         rdx,rsi
  0000000141490FA5: 48 8B CF           mov         rcx,rdi
  0000000141490FA8: E8 D3 F3 FD FF     call        0000000141470380
  0000000141490FAD: E9 E8 00 00 00     jmp         000000014149109A
  0000000141490FB2: 48 3B FD           cmp         rdi,rbp
  0000000141490FB5: 75 3C              jne         0000000141490FF3
  0000000141490FB7: E8 78 31 FE FF     call        0000000141474134
  0000000141490FBC: 45 33 C9           xor         r9d,r9d
  0000000141490FBF: 45 33 C0           xor         r8d,r8d
  0000000141490FC2: 33 D2              xor         edx,edx
  0000000141490FC4: 33 C9              xor         ecx,ecx
  0000000141490FC6: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  0000000141490FCB: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  0000000141490FD1: E8 92 F5 FD FF     call        0000000141470568
  0000000141490FD6: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  0000000141490FDB: 74 0C              je          0000000141490FE9
  0000000141490FDD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141490FE2: 83 A0 C8 00 00 00  and         dword ptr [rax+00000000000000C8h],0FFFFFFFDh
                    FD
  0000000141490FE9: B8 FF FF FF 7F     mov         eax,7FFFFFFFh
  0000000141490FEE: E9 BA 00 00 00     jmp         00000001414910AD
  0000000141490FF3: 48 3B F5           cmp         rsi,rbp
  0000000141490FF6: 74 BF              je          0000000141490FB7
  0000000141490FF8: 41 B0 04           mov         r8b,4
  0000000141490FFB: 0F B6 0F           movzx       ecx,byte ptr [rdi]
  0000000141490FFE: 48 FF CB           dec         rbx
  0000000141491001: 48 FF C7           inc         rdi
  0000000141491004: 46 84 44 19 1D     test        byte ptr [rcx+r11+1Dh],r8b
  0000000141491009: 74 2D              je          0000000141491038
  000000014149100B: 48 3B DD           cmp         rbx,rbp
  000000014149100E: 75 11              jne         0000000141491021
  0000000141491010: 0F B6 06           movzx       eax,byte ptr [rsi]
  0000000141491013: 8B CD              mov         ecx,ebp
  0000000141491015: 46 84 44 18 1D     test        byte ptr [rax+r11+1Dh],r8b
  000000014149101A: 75 5B              jne         0000000141491077
  000000014149101C: 66 8B D0           mov         dx,ax
  000000014149101F: EB 47              jmp         0000000141491068
  0000000141491021: 40 38 2F           cmp         byte ptr [rdi],bpl
  0000000141491024: 75 05              jne         000000014149102B
  0000000141491026: 66 8B CD           mov         cx,bp
  0000000141491029: EB 0D              jmp         0000000141491038
  000000014149102B: 0F B6 07           movzx       eax,byte ptr [rdi]
  000000014149102E: 66 C1 E1 08        shl         cx,8
  0000000141491032: 66 0B C8           or          cx,ax
  0000000141491035: 48 FF C7           inc         rdi
  0000000141491038: 0F B6 16           movzx       edx,byte ptr [rsi]
  000000014149103B: 48 FF C6           inc         rsi
  000000014149103E: 46 84 44 1A 1D     test        byte ptr [rdx+r11+1Dh],r8b
  0000000141491043: 74 23              je          0000000141491068
  0000000141491045: 48 3B DD           cmp         rbx,rbp
  0000000141491048: 75 04              jne         000000014149104E
  000000014149104A: 8B D5              mov         edx,ebp
  000000014149104C: EB 1A              jmp         0000000141491068
  000000014149104E: 48 FF CB           dec         rbx
  0000000141491051: 40 38 2E           cmp         byte ptr [rsi],bpl
  0000000141491054: 75 05              jne         000000014149105B
  0000000141491056: 66 8B D5           mov         dx,bp
  0000000141491059: EB 0D              jmp         0000000141491068
  000000014149105B: 0F B6 06           movzx       eax,byte ptr [rsi]
  000000014149105E: 66 C1 E2 08        shl         dx,8
  0000000141491062: 66 0B D0           or          dx,ax
  0000000141491065: 48 FF C6           inc         rsi
  0000000141491068: 66 3B D1           cmp         dx,cx
  000000014149106B: 75 26              jne         0000000141491093
  000000014149106D: 66 3B CD           cmp         cx,bp
  0000000141491070: 74 05              je          0000000141491077
  0000000141491072: 48 3B DD           cmp         rbx,rbp
  0000000141491075: 75 84              jne         0000000141490FFB
  0000000141491077: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  000000014149107C: 0F 84 FE FE FF FF  je          0000000141490F80
  0000000141491082: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141491087: 83 A0 C8 00 00 00  and         dword ptr [rax+00000000000000C8h],0FFFFFFFDh
                    FD
  000000014149108E: E9 ED FE FF FF     jmp         0000000141490F80
  0000000141491093: 1B C0              sbb         eax,eax
  0000000141491095: 83 E0 02           and         eax,2
  0000000141491098: FF C8              dec         eax
  000000014149109A: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  000000014149109F: 74 0C              je          00000001414910AD
  00000001414910A1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001414910A6: 83 A1 C8 00 00 00  and         dword ptr [rcx+00000000000000C8h],0FFFFFFFDh
                    FD
  00000001414910AD: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  00000001414910B2: 48 8B 6C 24 68     mov         rbp,qword ptr [rsp+68h]
  00000001414910B7: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  00000001414910BC: 48 83 C4 50        add         rsp,50h
  00000001414910C0: 5F                 pop         rdi
  00000001414910C1: C3                 ret

  Summary

     1E02000 .text
