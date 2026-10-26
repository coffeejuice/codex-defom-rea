Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140F9A780: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140F9A785: 48 83 EC 38        sub         rsp,38h
  0000000140F9A789: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A78E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9A791: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A796: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9A79C: 48 85 C0           test        rax,rax
  0000000140F9A79F: 0F 84 B6 00 00 00  je          0000000140F9A85B
  0000000140F9A7A5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A7AA: 83 78 58 01        cmp         dword ptr [rax+58h],1
  0000000140F9A7AE: 74 16              je          0000000140F9A7C6
  0000000140F9A7B0: B2 01              mov         dl,1
  0000000140F9A7B2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A7B7: E8 64 F8 FF FF     call        0000000140F9A020
  0000000140F9A7BC: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140F9A7C1: 41 C6 43 5C 01     mov         byte ptr [r11+5Ch],1
  0000000140F9A7C6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A7CB: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9A7CE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A7D3: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9A7D9: 45 33 C9           xor         r9d,r9d
  0000000140F9A7DC: 41 B0 01           mov         r8b,1
  0000000140F9A7DF: 33 D2              xor         edx,edx
  0000000140F9A7E1: 48 8B C8           mov         rcx,rax
  0000000140F9A7E4: E8 D7 81 3C FF     call        00000001403629C0
  0000000140F9A7E9: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000140F9A7ED: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140F9A7F2: 85 C0              test        eax,eax
  0000000140F9A7F4: 74 65              je          0000000140F9A85B
  0000000140F9A7F6: B2 01              mov         dl,1
  0000000140F9A7F8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A7FD: E8 1E F8 FF FF     call        0000000140F9A020
  0000000140F9A802: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140F9A807: 83 78 58 01        cmp         dword ptr [rax+58h],1
  0000000140F9A80B: 74 1D              je          0000000140F9A82A
  0000000140F9A80D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A812: 48 83 C1 28        add         rcx,28h
  0000000140F9A816: E8 D5 06 C8 FF     call        0000000140C1AEF0
  0000000140F9A81B: 48 8B C8           mov         rcx,rax
  0000000140F9A81E: E8 7D 18 CE FF     call        0000000140C7C0A0
  0000000140F9A823: 0F B6 C0           movzx       eax,al
  0000000140F9A826: 85 C0              test        eax,eax
  0000000140F9A828: 74 0C              je          0000000140F9A836
  0000000140F9A82A: 33 D2              xor         edx,edx
  0000000140F9A82C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A831: E8 EA F7 FF FF     call        0000000140F9A020
  0000000140F9A836: B2 01              mov         dl,1
  0000000140F9A838: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A83D: E8 0E E6 3D FF     call        0000000140378E50
  0000000140F9A842: BA F3 03 00 00     mov         edx,3F3h
  0000000140F9A847: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A84C: E8 4F 6A 19 FF     call        00000001401312A0
  0000000140F9A851: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140F9A856: E8 55 0D 00 00     call        0000000140F9B5B0
  0000000140F9A85B: 48 83 C4 38        add         rsp,38h
  0000000140F9A85F: C3                 ret

  Summary

     1E02000 .text
