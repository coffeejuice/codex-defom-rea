Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714F1C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006714F1C5: 55                 push        rbp
  000000006714F1C6: 56                 push        rsi
  000000006714F1C7: 57                 push        rdi
  000000006714F1C8: 41 54              push        r12
  000000006714F1CA: 41 55              push        r13
  000000006714F1CC: 41 56              push        r14
  000000006714F1CE: 41 57              push        r15
  000000006714F1D0: 48 83 EC 30        sub         rsp,30h
  000000006714F1D4: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714F1DD: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006714F1E2: 49 8B F1           mov         rsi,r9
  000000006714F1E5: 45 8B E0           mov         r12d,r8d
  000000006714F1E8: 48 8B EA           mov         rbp,rdx
  000000006714F1EB: 4C 8B F1           mov         r14,rcx
  000000006714F1EE: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  000000006714F1F9: 8B 9C 24 90 00 00  mov         ebx,dword ptr [rsp+0000000000000090h]
                    00
  000000006714F200: 40 32 FF           xor         dil,dil
  000000006714F203: 45 32 FF           xor         r15b,r15b
  000000006714F206: 45 32 ED           xor         r13b,r13b
  000000006714F209: B9 02 00 00 00     mov         ecx,2
  000000006714F20E: 4D 85 C9           test        r9,r9
  000000006714F211: 74 2B              je          000000006714F23E
  000000006714F213: 41 8B 01           mov         eax,dword ptr [r9]
  000000006714F216: 83 E0 01           and         eax,1
  000000006714F219: 41 0F 95 C5        setne       r13b
  000000006714F21D: 85 DB              test        ebx,ebx
  000000006714F21F: 75 04              jne         000000006714F225
  000000006714F221: 41 8B 59 0C        mov         ebx,dword ptr [r9+0Ch]
  000000006714F225: 41 83 79 04 00     cmp         dword ptr [r9+4],0
  000000006714F22A: 74 08              je          000000006714F234
  000000006714F22C: 40 B7 01           mov         dil,1
  000000006714F22F: 45 0F B6 79 10     movzx       r15d,byte ptr [r9+10h]
  000000006714F234: 45 84 ED           test        r13b,r13b
  000000006714F237: 74 05              je          000000006714F23E
  000000006714F239: 85 DB              test        ebx,ebx
  000000006714F23B: 0F 44 D9           cmove       ebx,ecx
  000000006714F23E: 45 33 C0           xor         r8d,r8d
  000000006714F241: 41 8B D4           mov         edx,r12d
  000000006714F244: 49 8B CE           mov         rcx,r14
  000000006714F247: E8 24 C4 F1 FF     call        000000006706B670
  000000006714F24C: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  000000006714F257: 49 8B 06           mov         rax,qword ptr [r14]
  000000006714F25A: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  000000006714F25E: 45 85 E4           test        r12d,r12d
  000000006714F261: 0F 84 C2 00 00 00  je          000000006714F329
  000000006714F267: 41 BA 00 01 00 00  mov         r10d,100h
  000000006714F26D: 41 BB 00 FF 00 00  mov         r11d,0FF00h
  000000006714F273: 41 B9 FE FF 00 00  mov         r9d,0FFFEh
  000000006714F279: 45 8D 43 FF        lea         r8d,[r11-1]
  000000006714F27D: 0F 1F 00           nop         dword ptr [rax]
  000000006714F280: 41 FF CC           dec         r12d
  000000006714F283: 40 84 FF           test        dil,dil
  000000006714F286: 0F 84 8A 00 00 00  je          000000006714F316
  000000006714F28C: 83 FB 02           cmp         ebx,2
  000000006714F28F: 75 1C              jne         000000006714F2AD
  000000006714F291: 0F B6 45 00        movzx       eax,byte ptr [rbp]
  000000006714F295: 48 FF C5           inc         rbp
  000000006714F298: 0F B6 C0           movzx       eax,al
  000000006714F29B: 66 41 0F AF C2     imul        ax,r10w
  000000006714F2A0: 66 41 23 C3        and         ax,r11w
  000000006714F2A4: 41 0F B6 CF        movzx       ecx,r15b
  000000006714F2A8: 66 03 C1           add         ax,cx
  000000006714F2AB: EB 17              jmp         000000006714F2C4
  000000006714F2AD: 41 0F B6 C7        movzx       eax,r15b
  000000006714F2B1: 66 41 0F AF C2     imul        ax,r10w
  000000006714F2B6: 66 41 23 C3        and         ax,r11w
  000000006714F2BA: 0F B6 4D 00        movzx       ecx,byte ptr [rbp]
  000000006714F2BE: 66 03 C1           add         ax,cx
  000000006714F2C1: 48 FF C5           inc         rbp
  000000006714F2C4: 45 84 ED           test        r13b,r13b
  000000006714F2C7: 75 41              jne         000000006714F30A
  000000006714F2C9: 41 B5 01           mov         r13b,1
  000000006714F2CC: 85 DB              test        ebx,ebx
  000000006714F2CE: 75 2B              jne         000000006714F2FB
  000000006714F2D0: 66 41 3B C1        cmp         ax,r9w
  000000006714F2D4: 75 07              jne         000000006714F2DD
  000000006714F2D6: BB 02 00 00 00     mov         ebx,2
  000000006714F2DB: EB 34              jmp         000000006714F311
  000000006714F2DD: 66 41 3B C0        cmp         ax,r8w
  000000006714F2E1: 75 07              jne         000000006714F2EA
  000000006714F2E3: BB 01 00 00 00     mov         ebx,1
  000000006714F2E8: EB 27              jmp         000000006714F311
  000000006714F2EA: BB 02 00 00 00     mov         ebx,2
  000000006714F2EF: 66 C1 C0 08        rol         ax,8
  000000006714F2F3: 66 89 02           mov         word ptr [rdx],ax
  000000006714F2F6: 48 03 D3           add         rdx,rbx
  000000006714F2F9: EB 16              jmp         000000006714F311
  000000006714F2FB: 66 41 3B C0        cmp         ax,r8w
  000000006714F2FF: 74 10              je          000000006714F311
  000000006714F301: 66 89 02           mov         word ptr [rdx],ax
  000000006714F304: 48 83 C2 02        add         rdx,2
  000000006714F308: EB 07              jmp         000000006714F311
  000000006714F30A: 66 89 02           mov         word ptr [rdx],ax
  000000006714F30D: 48 83 C2 02        add         rdx,2
  000000006714F311: 40 32 FF           xor         dil,dil
  000000006714F314: EB 0A              jmp         000000006714F320
  000000006714F316: 44 8A 7D 00        mov         r15b,byte ptr [rbp]
  000000006714F31A: 48 FF C5           inc         rbp
  000000006714F31D: 40 B7 01           mov         dil,1
  000000006714F320: 45 85 E4           test        r12d,r12d
  000000006714F323: 0F 85 57 FF FF FF  jne         000000006714F280
  000000006714F329: 49 8B 06           mov         rax,qword ptr [r14]
  000000006714F32C: 48 2B 50 10        sub         rdx,qword ptr [rax+10h]
  000000006714F330: 48 D1 FA           sar         rdx,1
  000000006714F333: 49 8B CE           mov         rcx,r14
  000000006714F336: E8 55 53 F2 FF     call        0000000067074690
  000000006714F33B: 48 85 F6           test        rsi,rsi
  000000006714F33E: 74 30              je          000000006714F370
  000000006714F340: 45 84 ED           test        r13b,r13b
  000000006714F343: 74 03              je          000000006714F348
  000000006714F345: 83 0E 01           or          dword ptr [rsi],1
  000000006714F348: 89 5E 0C           mov         dword ptr [rsi+0Ch],ebx
  000000006714F34B: 40 84 FF           test        dil,dil
  000000006714F34E: 74 13              je          000000006714F363
  000000006714F350: C7 46 04 01 00 00  mov         dword ptr [rsi+4],1
                    00
  000000006714F357: 41 0F B6 C7        movzx       eax,r15b
  000000006714F35B: 89 46 10           mov         dword ptr [rsi+10h],eax
  000000006714F35E: 49 8B C6           mov         rax,r14
  000000006714F361: EB 10              jmp         000000006714F373
  000000006714F363: 33 C0              xor         eax,eax
  000000006714F365: 89 46 04           mov         dword ptr [rsi+4],eax
  000000006714F368: 89 46 10           mov         dword ptr [rsi+10h],eax
  000000006714F36B: 49 8B C6           mov         rax,r14
  000000006714F36E: EB 03              jmp         000000006714F373
  000000006714F370: 49 8B C6           mov         rax,r14
  000000006714F373: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  000000006714F378: 48 83 C4 30        add         rsp,30h
  000000006714F37C: 41 5F              pop         r15
  000000006714F37E: 41 5E              pop         r14
  000000006714F380: 41 5D              pop         r13
  000000006714F382: 41 5C              pop         r12
  000000006714F384: 5F                 pop         rdi
  000000006714F385: 5E                 pop         rsi
  000000006714F386: 5D                 pop         rbp
  000000006714F387: C3                 ret
  000000006714F388: CC                 int         3

  Summary

      1A7000 .text
