Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067118AF0: 85 D2              test        edx,edx
  0000000067118AF2: 0F 88 F3 01 00 00  js          0000000067118CEB
  0000000067118AF8: 55                 push        rbp
  0000000067118AF9: 56                 push        rsi
  0000000067118AFA: 57                 push        rdi
  0000000067118AFB: 41 54              push        r12
  0000000067118AFD: 41 55              push        r13
  0000000067118AFF: 41 56              push        r14
  0000000067118B01: 41 57              push        r15
  0000000067118B03: 48 83 EC 50        sub         rsp,50h
  0000000067118B07: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067118B10: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  0000000067118B18: 44 8B EA           mov         r13d,edx
  0000000067118B1B: 48 8B D9           mov         rbx,rcx
  0000000067118B1E: BE 25 00 00 00     mov         esi,25h
  0000000067118B23: 44 8D 4E DC        lea         r9d,[rsi-24h]
  0000000067118B27: 45 33 C0           xor         r8d,r8d
  0000000067118B2A: 0F B7 D6           movzx       edx,si
  0000000067118B2D: E8 3E 53 F5 FF     call        000000006706DE70
  0000000067118B32: 8B F8              mov         edi,eax
  0000000067118B34: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000067118B37: 0F 84 97 01 00 00  je          0000000067118CD4
  0000000067118B3D: 66 89 B4 24 A0 00  mov         word ptr [rsp+00000000000000A0h],si
                    00 00
  0000000067118B45: 4C 8D 35 E4 7F 1D  lea         r14,[00000000672F0B30h]
                    00
  0000000067118B4C: 44 8D 7E FB        lea         r15d,[rsi-5]
  0000000067118B50: BD 01 00 00 00     mov         ebp,1
  0000000067118B55: 44 8B E5           mov         r12d,ebp
  0000000067118B58: 4C 89 B4 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],r14
                    00 00
  0000000067118B60: F0 01 2D C9 7F 1D  lock add    dword ptr [00000000672F0B30h],ebp
                    00
  0000000067118B67: 48 63 F7           movsxd      rsi,edi
  0000000067118B6A: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067118B6D: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  0000000067118B71: 66 83 7C 71 02 4C  cmp         word ptr [rcx+rsi*2+2],4Ch
  0000000067118B77: 75 46              jne         0000000067118BBF
  0000000067118B79: BD 02 00 00 00     mov         ebp,2
  0000000067118B7E: 44 8B E5           mov         r12d,ebp
  0000000067118B81: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  0000000067118B85: 48 8D 15 44 0D 12  lea         rdx,[00000000672398D0h]
                    00
  0000000067118B8C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067118B91: E8 4A B6 F5 FF     call        00000000670741E0
  0000000067118B96: 90                 nop
  0000000067118B97: 48 8B D0           mov         rdx,rax
  0000000067118B9A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000067118BA2: E8 29 78 F5 FF     call        00000000670703D0
  0000000067118BA7: 90                 nop
  0000000067118BA8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000067118BAD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067118BB1: 75 48              jne         0000000067118BFB
  0000000067118BB3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000067118BB8: E8 13 75 F5 FF     call        00000000670700D0
  0000000067118BBD: EB 3C              jmp         0000000067118BFB
  0000000067118BBF: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  0000000067118BC3: 48 8D 15 D2 8D 09  lea         rdx,[00000000671B199Ch]
                    00
  0000000067118BCA: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000067118BCF: E8 0C B6 F5 FF     call        00000000670741E0
  0000000067118BD4: 90                 nop
  0000000067118BD5: 48 8B D0           mov         rdx,rax
  0000000067118BD8: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000067118BE0: E8 EB 77 F5 FF     call        00000000670703D0
  0000000067118BE5: 90                 nop
  0000000067118BE6: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000067118BEB: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067118BEF: 75 0A              jne         0000000067118BFB
  0000000067118BF1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000067118BF6: E8 D5 74 F5 FF     call        00000000670700D0
  0000000067118BFB: 48 8D 14 2E        lea         rdx,[rsi+rbp]
  0000000067118BFF: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067118C02: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  0000000067118C06: 66 83 3C 51 6E     cmp         word ptr [rcx+rdx*2],6Eh
  0000000067118C0B: 0F 85 82 00 00 00  jne         0000000067118C93
  0000000067118C11: 66 44 89 BC 24 98  mov         word ptr [rsp+0000000000000098h],r15w
                    00 00 00
  0000000067118C1A: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  0000000067118C22: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000067118C27: C7 44 24 20 0A 00  mov         dword ptr [rsp+20h],0Ah
                    00 00
  0000000067118C2F: 45 33 C9           xor         r9d,r9d
  0000000067118C32: 45 8B C5           mov         r8d,r13d
  0000000067118C35: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000067118C3A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000067118C42: E8 59 F4 05 00     call        00000000671780A0
  0000000067118C47: 90                 nop
  0000000067118C48: 48 8B D0           mov         rdx,rax
  0000000067118C4B: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000067118C53: E8 78 77 F5 FF     call        00000000670703D0
  0000000067118C58: 90                 nop
  0000000067118C59: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000067118C5E: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067118C62: 75 0A              jne         0000000067118C6E
  0000000067118C64: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067118C69: E8 62 74 F5 FF     call        00000000670700D0
  0000000067118C6E: 45 8D 44 24 01     lea         r8d,[r12+1]
  0000000067118C73: 4C 8D 8C 24 A8 00  lea         r9,[rsp+00000000000000A8h]
                    00 00
  0000000067118C7B: 8B D7              mov         edx,edi
  0000000067118C7D: 48 8B CB           mov         rcx,rbx
  0000000067118C80: E8 7B ED F5 FF     call        0000000067077A00
  0000000067118C85: 4C 8B 9C 24 A8 00  mov         r11,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000067118C8D: 45 8B 63 08        mov         r12d,dword ptr [r11+8]
  0000000067118C91: EB 08              jmp         0000000067118C9B
  0000000067118C93: 4C 8B 9C 24 A8 00  mov         r11,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000067118C9B: F0 41 83 03 FF     lock add    dword ptr [r11],0FFFFFFFFh
  0000000067118CA0: 75 0D              jne         0000000067118CAF
  0000000067118CA2: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000067118CAA: E8 21 74 F5 FF     call        00000000670700D0
  0000000067118CAF: 45 8D 04 3C        lea         r8d,[r12+rdi]
  0000000067118CB3: 41 B9 01 00 00 00  mov         r9d,1
  0000000067118CB9: 0F B7 94 24 A0 00  movzx       edx,word ptr [rsp+00000000000000A0h]
                    00 00
  0000000067118CC1: 48 8B CB           mov         rcx,rbx
  0000000067118CC4: E8 A7 51 F5 FF     call        000000006706DE70
  0000000067118CC9: 8B F8              mov         edi,eax
  0000000067118CCB: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000067118CCE: 0F 85 7C FE FF FF  jne         0000000067118B50
  0000000067118CD4: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000067118CDC: 48 83 C4 50        add         rsp,50h
  0000000067118CE0: 41 5F              pop         r15
  0000000067118CE2: 41 5E              pop         r14
  0000000067118CE4: 41 5D              pop         r13
  0000000067118CE6: 41 5C              pop         r12
  0000000067118CE8: 5F                 pop         rdi
  0000000067118CE9: 5E                 pop         rsi
  0000000067118CEA: 5D                 pop         rbp
  0000000067118CEB: F3 C3              rep ret

  Summary

      1A7000 .text
