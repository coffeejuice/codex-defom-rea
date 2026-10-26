Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538E2C0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000006538E2C5: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  000000006538E2CA: 56                 push        rsi
  000000006538E2CB: 57                 push        rdi
  000000006538E2CC: 41 54              push        r12
  000000006538E2CE: 48 83 EC 20        sub         rsp,20h
  000000006538E2D2: 80 B9 1C 02 00 00  cmp         byte ptr [rcx+000000000000021Ch],0
                    00
  000000006538E2D9: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006538E2DD: 45 8B E1           mov         r12d,r9d
  000000006538E2E0: 41 8B E8           mov         ebp,r8d
  000000006538E2E3: 48 8B FA           mov         rdi,rdx
  000000006538E2E6: 48 8B D9           mov         rbx,rcx
  000000006538E2E9: 0F 85 EC 00 00 00  jne         000000006538E3DB
  000000006538E2EF: 48 81 C1 50 02 00  add         rcx,250h
                    00
  000000006538E2F6: FF 15 54 A5 30 00  call        qword ptr [0000000065698850h]
  000000006538E2FC: 44 8B 17           mov         r10d,dword ptr [rdi]
  000000006538E2FF: 44 39 10           cmp         dword ptr [rax],r10d
  000000006538E302: 0F 85 D3 00 00 00  jne         000000006538E3DB
  000000006538E308: 4C 8B 47 08        mov         r8,qword ptr [rdi+8]
  000000006538E30C: 4C 39 40 08        cmp         qword ptr [rax+8],r8
  000000006538E310: 0F 85 C5 00 00 00  jne         000000006538E3DB
  000000006538E316: 8B 4F 04           mov         ecx,dword ptr [rdi+4]
  000000006538E319: 39 48 04           cmp         dword ptr [rax+4],ecx
  000000006538E31C: 0F 85 B9 00 00 00  jne         000000006538E3DB
  000000006538E322: 48 8B 4F 10        mov         rcx,qword ptr [rdi+10h]
  000000006538E326: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006538E32A: 0F 85 AB 00 00 00  jne         000000006538E3DB
  000000006538E330: 83 BB FC 01 00 00  cmp         dword ptr [rbx+00000000000001FCh],0
                    00
  000000006538E337: 75 3B              jne         000000006538E374
  000000006538E339: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006538E33E: FF 15 34 84 30 00  call        qword ptr [0000000065696778h]
  000000006538E344: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006538E347: 48 89 8B 28 02 00  mov         qword ptr [rbx+0000000000000228h],rcx
                    00
  000000006538E34E: 48 8B CB           mov         rcx,rbx
  000000006538E351: E8 7A E2 FF FF     call        000000006538C5D0
  000000006538E356: 4C 8B 83 F0 01 00  mov         r8,qword ptr [rbx+00000000000001F0h]
                    00
  000000006538E35D: BA 14 00 00 00     mov         edx,14h
  000000006538E362: 48 8D 48 48        lea         rcx,[rax+48h]
  000000006538E366: FF 15 0C 8A 30 00  call        qword ptr [0000000065696D78h]
  000000006538E36C: 48 8B CE           mov         rcx,rsi
  000000006538E36F: E8 1C 42 CD FF     call        0000000065062590
  000000006538E374: 85 ED              test        ebp,ebp
  000000006538E376: 75 2E              jne         000000006538E3A6
  000000006538E378: 48 8B CE           mov         rcx,rsi
  000000006538E37B: E8 60 8F FF FF     call        00000000653872E0
  000000006538E380: 41 8D 54 24 01     lea         edx,[r12+1]
  000000006538E385: 3B D0              cmp         edx,eax
  000000006538E387: 75 1D              jne         000000006538E3A6
  000000006538E389: 48 8D 8B 48 02 00  lea         rcx,[rbx+0000000000000248h]
                    00
  000000006538E390: FF 15 E2 A4 30 00  call        qword ptr [0000000065698878h]
  000000006538E396: 84 C0              test        al,al
  000000006538E398: 75 0C              jne         000000006538E3A6
  000000006538E39A: 33 D2              xor         edx,edx
  000000006538E39C: 48 8B CE           mov         rcx,rsi
  000000006538E39F: E8 AC DD FF FF     call        000000006538C150
  000000006538E3A4: EB 35              jmp         000000006538E3DB
  000000006538E3A6: 48 8D 8B 48 02 00  lea         rcx,[rbx+0000000000000248h]
                    00
  000000006538E3AD: FF 15 8D A4 30 00  call        qword ptr [0000000065698840h]
  000000006538E3B3: 3B 83 5C 02 00 00  cmp         eax,dword ptr [rbx+000000000000025Ch]
  000000006538E3B9: 74 20              je          000000006538E3DB
  000000006538E3BB: 48 8B CE           mov         rcx,rsi
  000000006538E3BE: E8 ED 6E CD FF     call        00000000650652B0
  000000006538E3C3: 48 8D 8B 48 02 00  lea         rcx,[rbx+0000000000000248h]
                    00
  000000006538E3CA: FF 15 80 A4 30 00  call        qword ptr [0000000065698850h]
  000000006538E3D0: 48 8B CB           mov         rcx,rbx
  000000006538E3D3: 48 8B D0           mov         rdx,rax
  000000006538E3D6: E8 B5 C7 FF FF     call        000000006538AB90
  000000006538E3DB: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006538E3E0: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006538E3E5: 48 83 C4 20        add         rsp,20h
  000000006538E3E9: 41 5C              pop         r12
  000000006538E3EB: 5F                 pop         rdi
  000000006538E3EC: 5E                 pop         rsi
  000000006538E3ED: C3                 ret

  Summary

      695000 .text
