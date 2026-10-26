Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140F9A8C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140F9A8C5: 48 83 EC 38        sub         rsp,38h
  0000000140F9A8C9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A8CE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9A8D1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A8D6: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9A8DC: 48 85 C0           test        rax,rax
  0000000140F9A8DF: 0F 84 B6 00 00 00  je          0000000140F9A99B
  0000000140F9A8E5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A8EA: 83 78 58 01        cmp         dword ptr [rax+58h],1
  0000000140F9A8EE: 74 16              je          0000000140F9A906
  0000000140F9A8F0: B2 01              mov         dl,1
  0000000140F9A8F2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A8F7: E8 24 F7 FF FF     call        0000000140F9A020
  0000000140F9A8FC: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140F9A901: 41 C6 43 5C 01     mov         byte ptr [r11+5Ch],1
  0000000140F9A906: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A90B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9A90E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A913: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9A919: 41 B1 01           mov         r9b,1
  0000000140F9A91C: 41 B0 01           mov         r8b,1
  0000000140F9A91F: 33 D2              xor         edx,edx
  0000000140F9A921: 48 8B C8           mov         rcx,rax
  0000000140F9A924: E8 97 80 3C FF     call        00000001403629C0
  0000000140F9A929: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000140F9A92D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140F9A932: 85 C0              test        eax,eax
  0000000140F9A934: 74 65              je          0000000140F9A99B
  0000000140F9A936: B2 01              mov         dl,1
  0000000140F9A938: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A93D: E8 DE F6 FF FF     call        0000000140F9A020
  0000000140F9A942: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A947: 83 78 58 01        cmp         dword ptr [rax+58h],1
  0000000140F9A94B: 74 1D              je          0000000140F9A96A
  0000000140F9A94D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A952: 48 83 C1 28        add         rcx,28h
  0000000140F9A956: E8 95 05 C8 FF     call        0000000140C1AEF0
  0000000140F9A95B: 48 8B C8           mov         rcx,rax
  0000000140F9A95E: E8 3D 17 CE FF     call        0000000140C7C0A0
  0000000140F9A963: 0F B6 C0           movzx       eax,al
  0000000140F9A966: 85 C0              test        eax,eax
  0000000140F9A968: 74 0C              je          0000000140F9A976
  0000000140F9A96A: 33 D2              xor         edx,edx
  0000000140F9A96C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A971: E8 AA F6 FF FF     call        0000000140F9A020
  0000000140F9A976: B2 01              mov         dl,1
  0000000140F9A978: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A97D: E8 CE E4 3D FF     call        0000000140378E50
  0000000140F9A982: BA F3 03 00 00     mov         edx,3F3h
  0000000140F9A987: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A98C: E8 0F 69 19 FF     call        00000001401312A0
  0000000140F9A991: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A996: E8 15 0C 00 00     call        0000000140F9B5B0
  0000000140F9A99B: 48 83 C4 38        add         rsp,38h
  0000000140F9A99F: C3                 ret

  Summary

     1E02000 .text
