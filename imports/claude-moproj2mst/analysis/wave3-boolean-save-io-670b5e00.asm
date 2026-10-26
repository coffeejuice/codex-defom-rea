Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5E00: 40 57              push        rdi
  00000000670B5E02: 48 83 EC 30        sub         rsp,30h
  00000000670B5E06: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B5E0F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670B5E14: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670B5E18: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5E1F: 48 85 C9           test        rcx,rcx
  00000000670B5E22: 75 13              jne         00000000670B5E37
  00000000670B5E24: 48 8D 0D 75 45 17  lea         rcx,[000000006722A3A0h]
                    00
  00000000670B5E2B: E8 B0 70 F6 FF     call        000000006701CEE0
  00000000670B5E30: 32 C0              xor         al,al
  00000000670B5E32: E9 EB 00 00 00     jmp         00000000670B5F22
  00000000670B5E37: 48 8D 97 00 01 00  lea         rdx,[rdi+0000000000000100h]
                    00
  00000000670B5E3E: 83 7A 10 00        cmp         dword ptr [rdx+10h],0
  00000000670B5E42: 75 06              jne         00000000670B5E4A
  00000000670B5E44: 83 7A 0C 00        cmp         dword ptr [rdx+0Ch],0
  00000000670B5E48: 74 6B              je          00000000670B5EB5
  00000000670B5E4A: 48 63 9F 18 01 00  movsxd      rbx,dword ptr [rdi+0000000000000118h]
                    00
  00000000670B5E51: E8 2A FF FF FF     call        00000000670B5D80
  00000000670B5E56: 48 3B C3           cmp         rax,rbx
  00000000670B5E59: 74 5A              je          00000000670B5EB5
  00000000670B5E5B: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5E62: E8 A9 1B FF FF     call        00000000670A7A10
  00000000670B5E67: 8B D8              mov         ebx,eax
  00000000670B5E69: B8 02 00 00 00     mov         eax,2
  00000000670B5E6E: 83 FB 08           cmp         ebx,8
  00000000670B5E71: 0F 44 D8           cmove       ebx,eax
  00000000670B5E74: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B5E79: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5E80: E8 9B 1B FF FF     call        00000000670A7A20
  00000000670B5E85: 90                 nop
  00000000670B5E86: 89 9F 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],ebx
  00000000670B5E8C: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B5E93: 48 8B D0           mov         rdx,rax
  00000000670B5E96: E8 35 A5 FB FF     call        00000000670703D0
  00000000670B5E9B: 90                 nop
  00000000670B5E9C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B5EA1: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B5EA5: 75 0A              jne         00000000670B5EB1
  00000000670B5EA7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670B5EAC: E8 1F A2 FB FF     call        00000000670700D0
  00000000670B5EB1: 32 C0              xor         al,al
  00000000670B5EB3: EB 6D              jmp         00000000670B5F22
  00000000670B5EB5: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5EBC: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B5EBF: FF 50 18           call        qword ptr [rax+18h]
  00000000670B5EC2: 84 C0              test        al,al
  00000000670B5EC4: 75 5A              jne         00000000670B5F20
  00000000670B5EC6: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5ECD: E8 3E 1B FF FF     call        00000000670A7A10
  00000000670B5ED2: 8B D8              mov         ebx,eax
  00000000670B5ED4: B8 02 00 00 00     mov         eax,2
  00000000670B5ED9: 83 FB 08           cmp         ebx,8
  00000000670B5EDC: 0F 44 D8           cmove       ebx,eax
  00000000670B5EDF: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670B5EE4: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B5EEB: E8 30 1B FF FF     call        00000000670A7A20
  00000000670B5EF0: 90                 nop
  00000000670B5EF1: 89 9F 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],ebx
  00000000670B5EF7: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B5EFE: 48 8B D0           mov         rdx,rax
  00000000670B5F01: E8 CA A4 FB FF     call        00000000670703D0
  00000000670B5F06: 90                 nop
  00000000670B5F07: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000000670B5F0C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B5F10: 75 0A              jne         00000000670B5F1C
  00000000670B5F12: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670B5F17: E8 B4 A1 FB FF     call        00000000670700D0
  00000000670B5F1C: 32 C0              xor         al,al
  00000000670B5F1E: EB 02              jmp         00000000670B5F22
  00000000670B5F20: B0 01              mov         al,1
  00000000670B5F22: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670B5F27: 48 83 C4 30        add         rsp,30h
  00000000670B5F2B: 5F                 pop         rdi
  00000000670B5F2C: C3                 ret

  Summary

      1A7000 .text
