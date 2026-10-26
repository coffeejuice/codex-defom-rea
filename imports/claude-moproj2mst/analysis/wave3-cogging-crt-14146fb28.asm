Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FB28: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000014146FB2D: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014146FB32: 57                 push        rdi
  000000014146FB33: 48 83 EC 30        sub         rsp,30h
  000000014146FB37: 48 8B D9           mov         rbx,rcx
  000000014146FB3A: 83 CF FF           or          edi,0FFFFFFFFh
  000000014146FB3D: 33 C0              xor         eax,eax
  000000014146FB3F: 48 85 C9           test        rcx,rcx
  000000014146FB42: 0F 95 C0           setne       al
  000000014146FB45: 85 C0              test        eax,eax
  000000014146FB47: 75 24              jne         000000014146FB6D
  000000014146FB49: E8 E6 45 00 00     call        0000000141474134
  000000014146FB4E: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FB54: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FB5A: 45 33 C9           xor         r9d,r9d
  000000014146FB5D: 45 33 C0           xor         r8d,r8d
  000000014146FB60: 33 D2              xor         edx,edx
  000000014146FB62: 33 C9              xor         ecx,ecx
  000000014146FB64: E8 FF 09 00 00     call        0000000141470568
  000000014146FB69: 8B C7              mov         eax,edi
  000000014146FB6B: EB 26              jmp         000000014146FB93
  000000014146FB6D: F6 41 18 40        test        byte ptr [rcx+18h],40h
  000000014146FB71: 74 06              je          000000014146FB79
  000000014146FB73: 83 61 18 00        and         dword ptr [rcx+18h],0
  000000014146FB77: EB 18              jmp         000000014146FB91
  000000014146FB79: E8 1A E1 FF FF     call        000000014146DC98
  000000014146FB7E: 90                 nop
  000000014146FB7F: 48 8B CB           mov         rcx,rbx
  000000014146FB82: E8 15 FF FF FF     call        000000014146FA9C
  000000014146FB87: 8B F8              mov         edi,eax
  000000014146FB89: 48 8B CB           mov         rcx,rbx
  000000014146FB8C: E8 97 E1 FF FF     call        000000014146DD28
  000000014146FB91: 8B C7              mov         eax,edi
  000000014146FB93: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000014146FB98: 48 83 C4 30        add         rsp,30h
  000000014146FB9C: 5F                 pop         rdi
  000000014146FB9D: C3                 ret

  Summary

     1E02000 .text
