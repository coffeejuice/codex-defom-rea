Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140044A90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140044A95: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  0000000140044A9C: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140044AA5: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140044AAD: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044AB5: 48 83 B8 C0 19 00  cmp         qword ptr [rax+00000000000019C0h],0
                    00 00
  0000000140044ABD: 0F 85 F0 00 00 00  jne         0000000140044BB3
  0000000140044AC3: B9 00 15 00 00     mov         ecx,1500h
  0000000140044AC8: E8 EF 9F 42 01     call        000000014146EABC
  0000000140044ACD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140044AD2: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140044AD8: 74 7B              je          0000000140044B55
  0000000140044ADA: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140044ADF: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044AE7: E8 34 8B C9 00     call        0000000140CDD620
  0000000140044AEC: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140044AF1: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044AF6: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140044AFB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140044AFF: 83 C8 01           or          eax,1
  0000000140044B02: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140044B06: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140044B0B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044B13: E8 D8 89 C9 00     call        0000000140CDD4F0
  0000000140044B18: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140044B1D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140044B22: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140044B27: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140044B2B: 83 C8 02           or          eax,2
  0000000140044B2E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140044B32: 4C 8B 4C 24 58     mov         r9,qword ptr [rsp+58h]
  0000000140044B37: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140044B3C: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044B44: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140044B49: E8 32 4D 04 00     call        0000000140089880
  0000000140044B4E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140044B53: EB 09              jmp         0000000140044B5E
  0000000140044B55: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140044B5E: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140044B63: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140044B68: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044B70: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140044B75: 48 89 81 C0 19 00  mov         qword ptr [rcx+00000000000019C0h],rax
                    00
  0000000140044B7C: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140044B80: 83 E0 02           and         eax,2
  0000000140044B83: 85 C0              test        eax,eax
  0000000140044B85: 74 11              je          0000000140044B98
  0000000140044B87: 83 64 24 40 FD     and         dword ptr [rsp+40h],0FFFFFFFDh
  0000000140044B8C: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140044B91: FF 15 49 0E DC 01  call        qword ptr [0000000141E059E0h]
  0000000140044B97: 90                 nop
  0000000140044B98: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140044B9C: 83 E0 01           and         eax,1
  0000000140044B9F: 85 C0              test        eax,eax
  0000000140044BA1: 74 10              je          0000000140044BB3
  0000000140044BA3: 83 64 24 40 FE     and         dword ptr [rsp+40h],0FFFFFFFEh
  0000000140044BA8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140044BAD: FF 15 2D 0E DC 01  call        qword ptr [0000000141E059E0h]
  0000000140044BB3: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140044BBB: 48 8B 80 C0 19 00  mov         rax,qword ptr [rax+00000000000019C0h]
                    00
  0000000140044BC2: 48 81 C4 88 00 00  add         rsp,88h
                    00
  0000000140044BC9: C3                 ret

  Summary

     1E02000 .text
