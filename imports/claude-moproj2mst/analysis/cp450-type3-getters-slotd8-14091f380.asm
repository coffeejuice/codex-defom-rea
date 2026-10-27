Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091F380: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014091F385: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091F38A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091F38F: 48 83 EC 48        sub         rsp,48h
  000000014091F393: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091F39C: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014091F3A4: 48 8D 15 A5 F6 E9  lea         rdx,[00000001427BEA50h]
                    01
  000000014091F3AB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014091F3B0: FF 15 CA 50 4E 01  call        qword ptr [0000000141E04480h]
  000000014091F3B6: 90                 nop
  000000014091F3B7: 83 7C 24 60 FF     cmp         dword ptr [rsp+60h],0FFFFFFFFh
  000000014091F3BC: 7E 38              jle         000000014091F3F6
  000000014091F3BE: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091F3C3: 48 83 C1 20        add         rcx,20h
  000000014091F3C7: E8 14 57 6E FF     call        0000000140004AE0
  000000014091F3CC: 39 44 24 60        cmp         dword ptr [rsp+60h],eax
  000000014091F3D0: 7D 24              jge         000000014091F3F6
  000000014091F3D2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091F3D7: 48 83 C1 20        add         rcx,20h
  000000014091F3DB: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  000000014091F3DF: E8 BC 08 00 00     call        000000014091FCA0
  000000014091F3E4: 48 8B D0           mov         rdx,rax
  000000014091F3E7: 48 83 C2 08        add         rdx,8
  000000014091F3EB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014091F3F0: FF 15 7A 50 4E 01  call        qword ptr [0000000141E04470h]
  000000014091F3F6: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014091F3FB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014091F400: FF 15 D2 65 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091F406: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000014091F40A: 83 C8 01           or          eax,1
  000000014091F40D: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014091F411: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014091F416: FF 15 C4 65 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091F41C: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014091F421: 48 83 C4 48        add         rsp,48h
  000000014091F425: C3                 ret
  000000014091F426: CC                 int         3

  Summary

     1E02000 .text
