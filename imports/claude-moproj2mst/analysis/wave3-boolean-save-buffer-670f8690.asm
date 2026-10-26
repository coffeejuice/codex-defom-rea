Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F8690: 40 55              push        rbp
  00000000670F8692: 56                 push        rsi
  00000000670F8693: 57                 push        rdi
  00000000670F8694: 41 54              push        r12
  00000000670F8696: 41 55              push        r13
  00000000670F8698: 41 56              push        r14
  00000000670F869A: 41 57              push        r15
  00000000670F869C: 48 83 EC 40        sub         rsp,40h
  00000000670F86A0: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F86A9: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  00000000670F86B1: 49 8B E8           mov         rbp,r8
  00000000670F86B4: 4C 8B E2           mov         r12,rdx
  00000000670F86B7: 48 8B F1           mov         rsi,rcx
  00000000670F86BA: 4C 8B 69 18        mov         r13,qword ptr [rcx+18h]
  00000000670F86BE: 48 83 79 70 00     cmp         qword ptr [rcx+70h],0
  00000000670F86C3: 0F 85 CC 00 00 00  jne         00000000670F8795
  00000000670F86C9: 83 B9 98 00 00 00  cmp         dword ptr [rcx+0000000000000098h],0FFFFFFFFh
                    FF
  00000000670F86D0: 0F 85 BF 00 00 00  jne         00000000670F8795
  00000000670F86D6: 48 83 79 78 FF     cmp         qword ptr [rcx+78h],0FFFFFFFFFFFFFFFFh
  00000000670F86DB: 75 09              jne         00000000670F86E6
  00000000670F86DD: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670F86E1: E9 B4 00 00 00     jmp         00000000670F879A
  00000000670F86E6: 41 8B F8           mov         edi,r8d
  00000000670F86E9: 45 33 F6           xor         r14d,r14d
  00000000670F86EC: 49 8B DE           mov         rbx,r14
  00000000670F86EF: 4C 8D 3D 62 DB 13  lea         r15,[0000000067236258h]
                    00
  00000000670F86F6: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  00000000670F8700: 89 BC 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],edi
                    00
  00000000670F8707: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  00000000670F870F: 81 FF 00 00 00 02  cmp         edi,2000000h
  00000000670F8715: 49 0F 43 C7        cmovae      rax,r15
  00000000670F8719: 4A 8D 14 23        lea         rdx,[rbx+r12]
  00000000670F871D: 4C 89 74 24 20     mov         qword ptr [rsp+20h],r14
  00000000670F8722: 4C 8D 8C 24 98 00  lea         r9,[rsp+0000000000000098h]
                    00 00
  00000000670F872A: 44 8B 00           mov         r8d,dword ptr [rax]
  00000000670F872D: 48 8B 4E 78        mov         rcx,qword ptr [rsi+78h]
  00000000670F8731: FF 15 39 FA 0A 00  call        qword ptr [00000000671A8170h]
  00000000670F8737: 85 C0              test        eax,eax
  00000000670F8739: 74 18              je          00000000670F8753
  00000000670F873B: 8B 84 24 98 00 00  mov         eax,dword ptr [rsp+0000000000000098h]
                    00
  00000000670F8742: 85 C0              test        eax,eax
  00000000670F8744: 74 4A              je          00000000670F8790
  00000000670F8746: 48 03 D8           add         rbx,rax
  00000000670F8749: 48 2B F8           sub         rdi,rax
  00000000670F874C: 48 3B DD           cmp         rbx,rbp
  00000000670F874F: 7D 3F              jge         00000000670F8790
  00000000670F8751: EB AD              jmp         00000000670F8700
  00000000670F8753: 48 85 DB           test        rbx,rbx
  00000000670F8756: 75 38              jne         00000000670F8790
  00000000670F8758: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670F875B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F8760: E8 AB 3F F2 FF     call        000000006701C710
  00000000670F8765: 90                 nop
  00000000670F8766: 4C 8B C0           mov         r8,rax
  00000000670F8769: 8D 53 02           lea         edx,[rbx+2]
  00000000670F876C: 49 8B CD           mov         rcx,r13
  00000000670F876F: E8 CC F2 FA FF     call        00000000670A7A40
  00000000670F8774: 90                 nop
  00000000670F8775: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670F877A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F877E: 75 0A              jne         00000000670F878A
  00000000670F8780: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670F8785: E8 46 79 F7 FF     call        00000000670700D0
  00000000670F878A: 48 83 C8 FF        or          rax,0FFFFFFFFFFFFFFFFh
  00000000670F878E: EB 0A              jmp         00000000670F879A
  00000000670F8790: 48 8B C3           mov         rax,rbx
  00000000670F8793: EB 05              jmp         00000000670F879A
  00000000670F8795: E8 46 1B FF FF     call        00000000670EA2E0
  00000000670F879A: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F87A2: 48 83 C4 40        add         rsp,40h
  00000000670F87A6: 41 5F              pop         r15
  00000000670F87A8: 41 5E              pop         r14
  00000000670F87AA: 41 5D              pop         r13
  00000000670F87AC: 41 5C              pop         r12
  00000000670F87AE: 5F                 pop         rdi
  00000000670F87AF: 5E                 pop         rsi
  00000000670F87B0: 5D                 pop         rbp
  00000000670F87B1: C3                 ret

  Summary

      1A7000 .text
