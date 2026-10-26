Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B9A00: 40 57              push        rdi
  00000000670B9A02: 48 83 EC 30        sub         rsp,30h
  00000000670B9A06: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B9A0F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670B9A14: 48 8B FA           mov         rdi,rdx
  00000000670B9A17: 48 8B D9           mov         rbx,rcx
  00000000670B9A1A: B9 E0 00 00 00     mov         ecx,0E0h
  00000000670B9A1F: E8 62 F0 0B 00     call        0000000067178A86
  00000000670B9A24: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670B9A29: 48 85 C0           test        rax,rax
  00000000670B9A2C: 74 0D              je          00000000670B9A3B
  00000000670B9A2E: 48 8B D7           mov         rdx,rdi
  00000000670B9A31: 48 8B C8           mov         rcx,rax
  00000000670B9A34: E8 F7 E5 FF FF     call        00000000670B8030
  00000000670B9A39: EB 02              jmp         00000000670B9A3D
  00000000670B9A3B: 33 C0              xor         eax,eax
  00000000670B9A3D: 48 89 03           mov         qword ptr [rbx],rax
  00000000670B9A40: 48 85 C0           test        rax,rax
  00000000670B9A43: 74 09              je          00000000670B9A4E
  00000000670B9A45: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670B9A49: 48 8B C3           mov         rax,rbx
  00000000670B9A4C: EB 03              jmp         00000000670B9A51
  00000000670B9A4E: 48 8B C3           mov         rax,rbx
  00000000670B9A51: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670B9A56: 48 83 C4 30        add         rsp,30h
  00000000670B9A5A: 5F                 pop         rdi
  00000000670B9A5B: C3                 ret

  Summary

      1A7000 .text
