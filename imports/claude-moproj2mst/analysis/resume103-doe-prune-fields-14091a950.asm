Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A950: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091A955: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A95A: 48 83 EC 78        sub         rsp,78h
  000000014091A95E: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091A967: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A96F: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000014091A974: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A979: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A981: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014091A986: FF 15 4C B0 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A98C: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091A991: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A999: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A99C: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014091A9A1: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9A9: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091A9AF: 0F B6 C0           movzx       eax,al
  000000014091A9B2: 85 C0              test        eax,eax
  000000014091A9B4: 74 69              je          000000014091AA1F
  000000014091A9B6: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000014091A9BB: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091A9C0: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A9C8: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014091A9CD: FF 15 05 B0 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A9D3: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091A9D8: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9E0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A9E3: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014091A9E8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9F0: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A9F6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9FE: 48 83 C1 38        add         rcx,38h
  000000014091AA02: 8B D0              mov         edx,eax
  000000014091AA04: E8 07 62 84 FF     call        0000000140160C10
  000000014091AA09: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AA0C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014091AA11: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014091AA16: E8 95 C7 EF FF     call        00000001408171B0
  000000014091AA1B: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091AA1F: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091AA23: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014091AA27: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091AA2F: FF 15 AB AF 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091AA35: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014091AA39: 48 83 C4 78        add         rsp,78h
  000000014091AA3D: C3                 ret

  Summary

     1E02000 .text
