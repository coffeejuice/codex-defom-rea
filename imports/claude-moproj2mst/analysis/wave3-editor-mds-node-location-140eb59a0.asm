Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB59A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140EB59A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140EB59AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140EB59AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB59B3: 48 83 EC 48        sub         rsp,48h
  0000000140EB59B7: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB59BC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140EB59BF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140EB59C4: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140EB59CA: 8B 54 24 58        mov         edx,dword ptr [rsp+58h]
  0000000140EB59CE: 48 8B C8           mov         rcx,rax
  0000000140EB59D1: E8 AA 54 DC FF     call        0000000140C7AE80
  0000000140EB59D6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140EB59DB: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140EB59E1: 75 07              jne         0000000140EB59EA
  0000000140EB59E3: 32 C0              xor         al,al
  0000000140EB59E5: E9 08 01 00 00     jmp         0000000140EB5AF2
  0000000140EB59EA: 83 7C 24 60 00     cmp         dword ptr [rsp+60h],0
  0000000140EB59EF: 7E 11              jle         0000000140EB5A02
  0000000140EB59F1: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140EB59F6: 8B 80 80 08 00 00  mov         eax,dword ptr [rax+0000000000000880h]
  0000000140EB59FC: 39 44 24 60        cmp         dword ptr [rsp+60h],eax
  0000000140EB5A00: 7E 07              jle         0000000140EB5A09
  0000000140EB5A02: 32 C0              xor         al,al
  0000000140EB5A04: E9 E9 00 00 00     jmp         0000000140EB5AF2
  0000000140EB5A09: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140EB5A0E: 48 05 A0 08 00 00  add         rax,8A0h
  0000000140EB5A14: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140EB5A19: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB5A1E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140EB5A21: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140EB5A26: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140EB5A2C: 48 8B C8           mov         rcx,rax
  0000000140EB5A2F: E8 9C 52 DC FF     call        0000000140C7ACD0
  0000000140EB5A34: 48 05 F8 15 00 00  add         rax,15F8h
  0000000140EB5A3A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140EB5A3F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140EB5A44: E8 A7 F0 99 FF     call        0000000140854AF0
  0000000140EB5A49: 48 85 C0           test        rax,rax
  0000000140EB5A4C: 75 07              jne         0000000140EB5A55
  0000000140EB5A4E: 32 C0              xor         al,al
  0000000140EB5A50: E9 9D 00 00 00     jmp         0000000140EB5AF2
  0000000140EB5A55: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140EB5A5A: E8 41 B4 27 FF     call        0000000140130EA0
  0000000140EB5A5F: 0F B6 C0           movzx       eax,al
  0000000140EB5A62: 85 C0              test        eax,eax
  0000000140EB5A64: 74 0A              je          0000000140EB5A70
  0000000140EB5A66: C7 44 24 24 02 00  mov         dword ptr [rsp+24h],2
                    00 00
  0000000140EB5A6E: EB 08              jmp         0000000140EB5A78
  0000000140EB5A70: C7 44 24 24 03 00  mov         dword ptr [rsp+24h],3
                    00 00
  0000000140EB5A78: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB5A80: EB 0B              jmp         0000000140EB5A8D
  0000000140EB5A82: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140EB5A86: 83 C0 01           add         eax,1
  0000000140EB5A89: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140EB5A8D: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  0000000140EB5A91: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140EB5A95: 7D 3A              jge         0000000140EB5AD1
  0000000140EB5A97: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140EB5A9C: E8 4F F0 99 FF     call        0000000140854AF0
  0000000140EB5AA1: 4C 8B D8           mov         r11,rax
  0000000140EB5AA4: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  0000000140EB5AA8: 83 E8 01           sub         eax,1
  0000000140EB5AAB: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140EB5AB0: 0F AF 01           imul        eax,dword ptr [rcx]
  0000000140EB5AB3: 03 44 24 20        add         eax,dword ptr [rsp+20h]
  0000000140EB5AB7: 48 63 D0           movsxd      rdx,eax
  0000000140EB5ABA: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140EB5ABF: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EB5AC4: F2 41 0F 10 04 D3  movsd       xmm0,mmword ptr [r11+rdx*8]
  0000000140EB5ACA: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  0000000140EB5ACF: EB B1              jmp         0000000140EB5A82
  0000000140EB5AD1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140EB5AD6: E8 C5 B3 27 FF     call        0000000140130EA0
  0000000140EB5ADB: 0F B6 C0           movzx       eax,al
  0000000140EB5ADE: 85 C0              test        eax,eax
  0000000140EB5AE0: 74 0E              je          0000000140EB5AF0
  0000000140EB5AE2: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EB5AE7: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140EB5AEB: F2 0F 11 40 10     movsd       mmword ptr [rax+10h],xmm0
  0000000140EB5AF0: B0 01              mov         al,1
  0000000140EB5AF2: 48 83 C4 48        add         rsp,48h
  0000000140EB5AF6: C3                 ret

  Summary

     1E02000 .text
