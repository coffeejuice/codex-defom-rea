Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141490D68: 48 8B C4           mov         rax,rsp
  0000000141490D6B: 48 89 58 08        mov         qword ptr [rax+8],rbx
  0000000141490D6F: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  0000000141490D73: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  0000000141490D77: 57                 push        rdi
  0000000141490D78: 48 83 EC 50        sub         rsp,50h
  0000000141490D7C: 48 8B F9           mov         rdi,rcx
  0000000141490D7F: 48 8B F2           mov         rsi,rdx
  0000000141490D82: 48 8D 48 D8        lea         rcx,[rax-28h]
  0000000141490D86: 49 8B D1           mov         rdx,r9
  0000000141490D89: 49 8B D8           mov         rbx,r8
  0000000141490D8C: E8 8B 28 FE FF     call        000000014147361C
  0000000141490D91: 33 ED              xor         ebp,ebp
  0000000141490D93: 48 3B DD           cmp         rbx,rbp
  0000000141490D96: 75 1A              jne         0000000141490DB2
  0000000141490D98: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  0000000141490D9D: 74 0C              je          0000000141490DAB
  0000000141490D9F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141490DA4: 83 A0 C8 00 00 00  and         dword ptr [rax+00000000000000C8h],0FFFFFFFDh
                    FD
  0000000141490DAB: 33 C0              xor         eax,eax
  0000000141490DAD: E9 8B 01 00 00     jmp         0000000141490F3D
  0000000141490DB2: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000141490DB7: 39 69 08           cmp         dword ptr [rcx+8],ebp
  0000000141490DBA: 75 13              jne         0000000141490DCF
  0000000141490DBC: 4C 8B C3           mov         r8,rbx
  0000000141490DBF: 48 8B D6           mov         rdx,rsi
  0000000141490DC2: 48 8B CF           mov         rcx,rdi
  0000000141490DC5: E8 06 2D 00 00     call        0000000141493AD0
  0000000141490DCA: E9 5B 01 00 00     jmp         0000000141490F2A
  0000000141490DCF: 48 3B FD           cmp         rdi,rbp
  0000000141490DD2: 75 3C              jne         0000000141490E10
  0000000141490DD4: E8 5B 33 FE FF     call        0000000141474134
  0000000141490DD9: 45 33 C9           xor         r9d,r9d
  0000000141490DDC: 45 33 C0           xor         r8d,r8d
  0000000141490DDF: 33 D2              xor         edx,edx
  0000000141490DE1: 33 C9              xor         ecx,ecx
  0000000141490DE3: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  0000000141490DE8: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  0000000141490DEE: E8 75 F7 FD FF     call        0000000141470568
  0000000141490DF3: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  0000000141490DF8: 74 0C              je          0000000141490E06
  0000000141490DFA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141490DFF: 83 A0 C8 00 00 00  and         dword ptr [rax+00000000000000C8h],0FFFFFFFDh
                    FD
  0000000141490E06: B8 FF FF FF 7F     mov         eax,7FFFFFFFh
  0000000141490E0B: E9 2D 01 00 00     jmp         0000000141490F3D
  0000000141490E10: 48 3B F5           cmp         rsi,rbp
  0000000141490E13: 74 BF              je          0000000141490DD4
  0000000141490E15: 41 B1 04           mov         r9b,4
  0000000141490E18: 0F B6 17           movzx       edx,byte ptr [rdi]
  0000000141490E1B: 48 FF CB           dec         rbx
  0000000141490E1E: 48 FF C7           inc         rdi
  0000000141490E21: 44 84 4C 0A 1D     test        byte ptr [rdx+rcx+1Dh],r9b
  0000000141490E26: 74 55              je          0000000141490E7D
  0000000141490E28: 48 3B DD           cmp         rbx,rbp
  0000000141490E2B: 75 16              jne         0000000141490E43
  0000000141490E2D: 0F B6 06           movzx       eax,byte ptr [rsi]
  0000000141490E30: 66 8B D5           mov         dx,bp
  0000000141490E33: 44 84 4C 08 1D     test        byte ptr [rax+rcx+1Dh],r9b
  0000000141490E38: 0F 85 5A FF FF FF  jne         0000000141490D98
  0000000141490E3E: E9 C4 00 00 00     jmp         0000000141490F07
  0000000141490E43: 40 38 2F           cmp         byte ptr [rdi],bpl
  0000000141490E46: 75 04              jne         0000000141490E4C
  0000000141490E48: 8B D5              mov         edx,ebp
  0000000141490E4A: EB 43              jmp         0000000141490E8F
  0000000141490E4C: 0F B6 07           movzx       eax,byte ptr [rdi]
  0000000141490E4F: 66 C1 E2 08        shl         dx,8
  0000000141490E53: 48 FF C7           inc         rdi
  0000000141490E56: 66 0B D0           or          dx,ax
  0000000141490E59: 66 3B 51 10        cmp         dx,word ptr [rcx+10h]
  0000000141490E5D: 72 0C              jb          0000000141490E6B
  0000000141490E5F: 66 3B 51 12        cmp         dx,word ptr [rcx+12h]
  0000000141490E63: 77 06              ja          0000000141490E6B
  0000000141490E65: 66 03 51 14        add         dx,word ptr [rcx+14h]
  0000000141490E69: EB 24              jmp         0000000141490E8F
  0000000141490E6B: 66 3B 51 16        cmp         dx,word ptr [rcx+16h]
  0000000141490E6F: 72 1E              jb          0000000141490E8F
  0000000141490E71: 66 3B 51 18        cmp         dx,word ptr [rcx+18h]
  0000000141490E75: 77 18              ja          0000000141490E8F
  0000000141490E77: 66 03 51 1A        add         dx,word ptr [rcx+1Ah]
  0000000141490E7B: EB 12              jmp         0000000141490E8F
  0000000141490E7D: 0F B7 C2           movzx       eax,dx
  0000000141490E80: F6 44 08 1D 10     test        byte ptr [rax+rcx+1Dh],10h
  0000000141490E85: 74 08              je          0000000141490E8F
  0000000141490E87: 0F B6 94 08 1D 01  movzx       edx,byte ptr [rax+rcx+000000000000011Dh]
                    00 00
  0000000141490E8F: 44 0F B6 06        movzx       r8d,byte ptr [rsi]
  0000000141490E93: 48 FF C6           inc         rsi
  0000000141490E96: 49 8B C0           mov         rax,r8
  0000000141490E99: 45 84 4C 08 1D     test        byte ptr [r8+rcx+1Dh],r9b
  0000000141490E9E: 74 4E              je          0000000141490EEE
  0000000141490EA0: 48 3B DD           cmp         rbx,rbp
  0000000141490EA3: 75 04              jne         0000000141490EA9
  0000000141490EA5: 8B C5              mov         eax,ebp
  0000000141490EA7: EB 5E              jmp         0000000141490F07
  0000000141490EA9: 48 FF CB           dec         rbx
  0000000141490EAC: 40 38 2E           cmp         byte ptr [rsi],bpl
  0000000141490EAF: 75 05              jne         0000000141490EB6
  0000000141490EB1: 66 8B C5           mov         ax,bp
  0000000141490EB4: EB 51              jmp         0000000141490F07
  0000000141490EB6: 0F B6 06           movzx       eax,byte ptr [rsi]
  0000000141490EB9: 66 41 C1 E0 08     shl         r8w,8
  0000000141490EBE: 48 FF C6           inc         rsi
  0000000141490EC1: 66 44 0B C0        or          r8w,ax
  0000000141490EC5: 66 41 8B C0        mov         ax,r8w
  0000000141490EC9: 66 44 3B 41 10     cmp         r8w,word ptr [rcx+10h]
  0000000141490ECE: 72 0C              jb          0000000141490EDC
  0000000141490ED0: 66 3B 41 12        cmp         ax,word ptr [rcx+12h]
  0000000141490ED4: 77 06              ja          0000000141490EDC
  0000000141490ED6: 66 03 41 14        add         ax,word ptr [rcx+14h]
  0000000141490EDA: EB 2B              jmp         0000000141490F07
  0000000141490EDC: 66 3B 41 16        cmp         ax,word ptr [rcx+16h]
  0000000141490EE0: 72 25              jb          0000000141490F07
  0000000141490EE2: 66 3B 41 18        cmp         ax,word ptr [rcx+18h]
  0000000141490EE6: 77 1F              ja          0000000141490F07
  0000000141490EE8: 66 03 41 1A        add         ax,word ptr [rcx+1Ah]
  0000000141490EEC: EB 19              jmp         0000000141490F07
  0000000141490EEE: 41 0F B7 C0        movzx       eax,r8w
  0000000141490EF2: F6 44 08 1D 10     test        byte ptr [rax+rcx+1Dh],10h
  0000000141490EF7: 74 0A              je          0000000141490F03
  0000000141490EF9: 0F B6 84 08 1D 01  movzx       eax,byte ptr [rax+rcx+000000000000011Dh]
                    00 00
  0000000141490F01: EB 04              jmp         0000000141490F07
  0000000141490F03: 66 41 8B C0        mov         ax,r8w
  0000000141490F07: 66 3B C2           cmp         ax,dx
  0000000141490F0A: 75 17              jne         0000000141490F23
  0000000141490F0C: 66 3B D5           cmp         dx,bp
  0000000141490F0F: 0F 84 83 FE FF FF  je          0000000141490D98
  0000000141490F15: 48 3B DD           cmp         rbx,rbp
  0000000141490F18: 0F 85 FA FE FF FF  jne         0000000141490E18
  0000000141490F1E: E9 75 FE FF FF     jmp         0000000141490D98
  0000000141490F23: 1B C0              sbb         eax,eax
  0000000141490F25: 83 E0 02           and         eax,2
  0000000141490F28: FF C8              dec         eax
  0000000141490F2A: 40 38 6C 24 48     cmp         byte ptr [rsp+48h],bpl
  0000000141490F2F: 74 0C              je          0000000141490F3D
  0000000141490F31: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141490F36: 83 A1 C8 00 00 00  and         dword ptr [rcx+00000000000000C8h],0FFFFFFFDh
                    FD
  0000000141490F3D: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000141490F42: 48 8B 6C 24 68     mov         rbp,qword ptr [rsp+68h]
  0000000141490F47: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  0000000141490F4C: 48 83 C4 50        add         rsp,50h
  0000000141490F50: 5F                 pop         rdi
  0000000141490F51: C3                 ret

  Summary

     1E02000 .text
