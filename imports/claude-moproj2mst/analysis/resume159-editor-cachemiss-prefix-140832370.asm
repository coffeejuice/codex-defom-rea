Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140832370: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140832375: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014083237A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014083237F: 48 83 EC 28        sub         rsp,28h
  0000000140832383: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140832389: 75 05              jne         0000000140832390
  000000014083238B: E9 9A 00 00 00     jmp         000000014083242A
  0000000140832390: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140832395: 83 B8 98 53 00 00  cmp         dword ptr [rax+0000000000005398h],64h
                    64
  000000014083239C: 7C 05              jl          00000001408323A3
  000000014083239E: E9 87 00 00 00     jmp         000000014083242A
  00000001408323A3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001408323A8: E8 13 01 00 00     call        00000001408324C0
  00000001408323AD: 44 8B D8           mov         r11d,eax
  00000001408323B0: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408323B5: 48 63 88 98 53 00  movsxd      rcx,dword ptr [rax+0000000000005398h]
                    00
  00000001408323BC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408323C1: 44 89 9C 88 E8 4E  mov         dword ptr [rax+rcx*4+0000000000004EE8h],r11d
                    00 00
  00000001408323C9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408323CE: 48 63 88 98 53 00  movsxd      rcx,dword ptr [rax+0000000000005398h]
                    00
  00000001408323D5: 48 69 C9 C8 00 00  imul        rcx,rcx,0C8h
                    00
  00000001408323DC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408323E1: 48 8D 8C 08 C8 00  lea         rcx,[rax+rcx+00000000000000C8h]
                    00 00
  00000001408323E9: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408323EE: E8 CD DA C3 00     call        000000014146FEC0
  00000001408323F3: 4C 8B 5C 24 30     mov         r11,qword ptr [rsp+30h]
  00000001408323F8: 49 63 93 98 53 00  movsxd      rdx,dword ptr [r11+0000000000005398h]
                    00
  00000001408323FF: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140832404: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140832409: 48 89 84 D1 78 50  mov         qword ptr [rcx+rdx*8+0000000000005078h],rax
                    00 00
  0000000140832411: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140832416: 8B 88 98 53 00 00  mov         ecx,dword ptr [rax+0000000000005398h]
  000000014083241C: 83 C1 01           add         ecx,1
  000000014083241F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140832424: 89 88 98 53 00 00  mov         dword ptr [rax+0000000000005398h],ecx
  000000014083242A: 48 83 C4 28        add         rsp,28h
  000000014083242E: C3                 ret

  Summary

     1E02000 .text
