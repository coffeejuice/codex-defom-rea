Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7ED0: 40 57              push        rdi
  00000000670A7ED2: 48 83 EC 30        sub         rsp,30h
  00000000670A7ED6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A7EDF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670A7EE4: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  00000000670A7EE9: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000670A7EEE: 48 8B F1           mov         rsi,rcx
  00000000670A7EF1: 48 8D 05 98 35 17  lea         rax,[000000006721B490h]
                    00
  00000000670A7EF8: 48 89 01           mov         qword ptr [rcx],rax
  00000000670A7EFB: E8 00 FC FF FF     call        00000000670A7B00
  00000000670A7F00: 48 8B D8           mov         rbx,rax
  00000000670A7F03: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670A7F08: 48 85 C0           test        rax,rax
  00000000670A7F0B: 74 19              je          00000000670A7F26
  00000000670A7F0D: A8 01              test        al,1
  00000000670A7F0F: 75 15              jne         00000000670A7F26
  00000000670A7F11: 48 8B C8           mov         rcx,rax
  00000000670A7F14: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  00000000670A7F18: E8 93 E0 F7 FF     call        0000000067025FB0
  00000000670A7F1D: 48 83 CB 01        or          rbx,1
  00000000670A7F21: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670A7F26: 80 3D A4 BF 24 00  cmp         byte ptr [00000000672F3ED1h],0
                    00
  00000000670A7F2D: 0F 85 A0 00 00 00  jne         00000000670A7FD3
  00000000670A7F33: E8 28 FD FF FF     call        00000000670A7C60
  00000000670A7F38: 48 8B E8           mov         rbp,rax
  00000000670A7F3B: 48 8B 10           mov         rdx,qword ptr [rax]
  00000000670A7F3E: 48 63 42 0C        movsxd      rax,dword ptr [rdx+0Ch]
  00000000670A7F42: 4C 63 42 08        movsxd      r8,dword ptr [rdx+8]
  00000000670A7F46: 44 8B C8           mov         r9d,eax
  00000000670A7F49: 45 2B C8           sub         r9d,r8d
  00000000670A7F4C: 45 85 C9           test        r9d,r9d
  00000000670A7F4F: 7E 64              jle         00000000670A7FB5
  00000000670A7F51: 48 8D 44 C2 18     lea         rax,[rdx+rax*8+18h]
  00000000670A7F56: 4A 8D 7C C2 18     lea         rdi,[rdx+r8*8+18h]
  00000000670A7F5B: 48 3B F8           cmp         rdi,rax
  00000000670A7F5E: 74 55              je          00000000670A7FB5
  00000000670A7F60: 48 39 37           cmp         qword ptr [rdi],rsi
  00000000670A7F63: 74 0B              je          00000000670A7F70
  00000000670A7F65: 48 83 C7 08        add         rdi,8
  00000000670A7F69: 48 3B F8           cmp         rdi,rax
  00000000670A7F6C: 75 F2              jne         00000000670A7F60
  00000000670A7F6E: EB 45              jmp         00000000670A7FB5
  00000000670A7F70: 4A 8D 04 C5 18 00  lea         rax,[r8*8+0000000000000018h]
                    00 00
  00000000670A7F78: 48 2B F8           sub         rdi,rax
  00000000670A7F7B: 48 2B FA           sub         rdi,rdx
  00000000670A7F7E: 48 C1 FF 03        sar         rdi,3
  00000000670A7F82: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000000670A7F85: 74 2E              je          00000000670A7FB5
  00000000670A7F87: 85 FF              test        edi,edi
  00000000670A7F89: 78 2A              js          00000000670A7FB5
  00000000670A7F8B: 41 3B F9           cmp         edi,r9d
  00000000670A7F8E: 7D 25              jge         00000000670A7FB5
  00000000670A7F90: 8B 02              mov         eax,dword ptr [rdx]
  00000000670A7F92: 83 F8 01           cmp         eax,1
  00000000670A7F95: 74 0F              je          00000000670A7FA6
  00000000670A7F97: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000670A7F9B: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670A7F9E: 48 8B CD           mov         rcx,rbp
  00000000670A7FA1: E8 CA B6 05 00     call        0000000067103670
  00000000670A7FA6: 8B D7              mov         edx,edi
  00000000670A7FA8: 48 8B CD           mov         rcx,rbp
  00000000670A7FAB: E8 A0 0E FA FF     call        0000000067048E50
  00000000670A7FB0: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670A7FB5: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670A7FB9: 0F B6 15 10 BF 24  movzx       edx,byte ptr [00000000672F3ED0h]
                    00
  00000000670A7FC0: 45 33 C0           xor         r8d,r8d
  00000000670A7FC3: 8B 41 08           mov         eax,dword ptr [rcx+8]
  00000000670A7FC6: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  00000000670A7FC9: 41 0F 44 D0        cmove       edx,r8d
  00000000670A7FCD: 88 15 FD BE 24 00  mov         byte ptr [00000000672F3ED0h],dl
  00000000670A7FD3: 48 85 DB           test        rbx,rbx
  00000000670A7FD6: 74 11              je          00000000670A7FE9
  00000000670A7FD8: F6 C3 01           test        bl,1
  00000000670A7FDB: 74 0C              je          00000000670A7FE9
  00000000670A7FDD: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  00000000670A7FE1: 48 8B CB           mov         rcx,rbx
  00000000670A7FE4: E8 C7 EA F7 FF     call        0000000067026AB0
  00000000670A7FE9: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670A7FEE: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  00000000670A7FF3: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000670A7FF8: 48 83 C4 30        add         rsp,30h
  00000000670A7FFC: 5F                 pop         rdi
  00000000670A7FFD: C3                 ret

  Summary

      1A7000 .text
