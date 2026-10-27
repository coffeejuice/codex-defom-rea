Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712D300: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006712D305: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006712D30A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006712D30F: 57                 push        rdi
  000000006712D310: 41 54              push        r12
  000000006712D312: 41 55              push        r13
  000000006712D314: 48 83 EC 20        sub         rsp,20h
  000000006712D318: 8B 41 20           mov         eax,dword ptr [rcx+20h]
  000000006712D31B: 45 33 ED           xor         r13d,r13d
  000000006712D31E: 48 8B F9           mov         rdi,rcx
  000000006712D321: 44 8B E0           mov         r12d,eax
  000000006712D324: 83 C8 08           or          eax,8
  000000006712D327: 41 8B ED           mov         ebp,r13d
  000000006712D32A: 89 41 20           mov         dword ptr [rcx+20h],eax
  000000006712D32D: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  000000006712D331: 41 C1 EC 03        shr         r12d,3
  000000006712D335: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006712D338: 41 80 E4 01        and         r12b,1
  000000006712D33C: 49 8B F5           mov         rsi,r13
  000000006712D33F: 2B 41 08           sub         eax,dword ptr [rcx+8]
  000000006712D342: 85 C0              test        eax,eax
  000000006712D344: 7E 70              jle         000000006712D3B6
  000000006712D346: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  000000006712D350: 48 8B 4F 18        mov         rcx,qword ptr [rdi+18h]
  000000006712D354: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  000000006712D358: 48 03 C6           add         rax,rsi
  000000006712D35B: 48 8B 44 C1 18     mov         rax,qword ptr [rcx+rax*8+18h]
  000000006712D360: 48 89 47 78        mov         qword ptr [rdi+78h],rax
  000000006712D364: 48 8B 47 18        mov         rax,qword ptr [rdi+18h]
  000000006712D368: 8B 08              mov         ecx,dword ptr [rax]
  000000006712D36A: 83 F9 01           cmp         ecx,1
  000000006712D36D: 74 10              je          000000006712D37F
  000000006712D36F: 48 8B 47 18        mov         rax,qword ptr [rdi+18h]
  000000006712D373: 48 8D 4F 18        lea         rcx,[rdi+18h]
  000000006712D377: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006712D37A: E8 81 45 02 00     call        0000000067151900
  000000006712D37F: 48 8B 4F 18        mov         rcx,qword ptr [rdi+18h]
  000000006712D383: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  000000006712D387: 48 03 C6           add         rax,rsi
  000000006712D38A: 4C 89 6C C1 18     mov         qword ptr [rcx+rax*8+18h],r13
  000000006712D38F: 48 8B 4F 78        mov         rcx,qword ptr [rdi+78h]
  000000006712D393: 48 85 C9           test        rcx,rcx
  000000006712D396: 74 0B              je          000000006712D3A3
  000000006712D398: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006712D39B: BA 01 00 00 00     mov         edx,1
  000000006712D3A0: FF 50 18           call        qword ptr [rax+18h]
  000000006712D3A3: 48 8B 47 18        mov         rax,qword ptr [rdi+18h]
  000000006712D3A7: FF C5              inc         ebp
  000000006712D3A9: 48 FF C6           inc         rsi
  000000006712D3AC: 8B 50 0C           mov         edx,dword ptr [rax+0Ch]
  000000006712D3AF: 2B 50 08           sub         edx,dword ptr [rax+8]
  000000006712D3B2: 3B EA              cmp         ebp,edx
  000000006712D3B4: 7C 9A              jl          000000006712D350
  000000006712D3B6: 48 8D 4F 18        lea         rcx,[rdi+18h]
  000000006712D3BA: E8 71 63 02 00     call        0000000067153730
  000000006712D3BF: 83 67 20 F7        and         dword ptr [rdi+20h],0FFFFFFF7h
  000000006712D3C3: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006712D3C8: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  000000006712D3CD: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  000000006712D3D2: 41 0F B6 C4        movzx       eax,r12b
  000000006712D3D6: 83 E0 01           and         eax,1
  000000006712D3D9: 4C 89 6F 78        mov         qword ptr [rdi+78h],r13
  000000006712D3DD: C1 E0 03           shl         eax,3
  000000006712D3E0: 09 47 20           or          dword ptr [rdi+20h],eax
  000000006712D3E3: 48 83 C4 20        add         rsp,20h
  000000006712D3E7: 41 5D              pop         r13
  000000006712D3E9: 41 5C              pop         r12
  000000006712D3EB: 5F                 pop         rdi
  000000006712D3EC: C3                 ret

  Summary

      1A7000 .text
