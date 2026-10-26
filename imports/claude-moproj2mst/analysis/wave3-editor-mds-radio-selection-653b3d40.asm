Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653B3D40: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653B3D45: 53                 push        rbx
  00000000653B3D46: 56                 push        rsi
  00000000653B3D47: 57                 push        rdi
  00000000653B3D48: 48 83 EC 30        sub         rsp,30h
  00000000653B3D4C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653B3D55: 48 8B F2           mov         rsi,rdx
  00000000653B3D58: 48 8B F9           mov         rdi,rcx
  00000000653B3D5B: B9 00 02 00 00     mov         ecx,200h
  00000000653B3D60: E8 0D D4 22 00     call        00000000655E1172
  00000000653B3D65: 48 8B D8           mov         rbx,rax
  00000000653B3D68: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000000653B3D6D: 48 85 C0           test        rax,rax
  00000000653B3D70: 74 28              je          00000000653B3D9A
  00000000653B3D72: BA 04 08 04 00     mov         edx,40804h
  00000000653B3D77: 48 8B C8           mov         rcx,rax
  00000000653B3D7A: E8 01 AA CA FF     call        000000006505E780
  00000000653B3D7F: 90                 nop
  00000000653B3D80: 48 8D 05 99 C0 34  lea         rax,[00000000656FFE20h]
                    00
  00000000653B3D87: 48 89 03           mov         qword ptr [rbx],rax
  00000000653B3D8A: 48 8D 8B E0 01 00  lea         rcx,[rbx+00000000000001E0h]
                    00
  00000000653B3D91: FF 15 D1 29 2E 00  call        qword ptr [0000000065696768h]
  00000000653B3D97: 90                 nop
  00000000653B3D98: EB 02              jmp         00000000653B3D9C
  00000000653B3D9A: 33 DB              xor         ebx,ebx
  00000000653B3D9C: C7 44 24 60 00 00  mov         dword ptr [rsp+60h],0
                    00 00
  00000000653B3DA4: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  00000000653B3DA9: 4C 8B C6           mov         r8,rsi
  00000000653B3DAC: 48 8B D3           mov         rdx,rbx
  00000000653B3DAF: 48 8B CF           mov         rcx,rdi
  00000000653B3DB2: E8 49 D1 CB FF     call        0000000065070F00
  00000000653B3DB7: 90                 nop
  00000000653B3DB8: 48 8D 05 B9 C0 34  lea         rax,[00000000656FFE78h]
                    00
  00000000653B3DBF: 48 89 07           mov         qword ptr [rdi],rax
  00000000653B3DC2: 48 8D 05 77 C0 34  lea         rax,[00000000656FFE40h]
                    00
  00000000653B3DC9: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  00000000653B3DCD: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000653B3DD1: E8 EA FA FF FF     call        00000000653B38C0
  00000000653B3DD6: 90                 nop
  00000000653B3DD7: 48 8B C7           mov         rax,rdi
  00000000653B3DDA: 48 83 C4 30        add         rsp,30h
  00000000653B3DDE: 5F                 pop         rdi
  00000000653B3DDF: 5E                 pop         rsi
  00000000653B3DE0: 5B                 pop         rbx
  00000000653B3DE1: C3                 ret

  Summary

      695000 .text
