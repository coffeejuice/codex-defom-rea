Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC9980: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC9985: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC998A: 48 81 EC 38 01 00  sub         rsp,138h
                    00
  0000000140EC9991: 48 C7 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140EC999D: 48 8B 05 8C 46 C9  mov         rax,qword ptr [0000000143B5E030h]
                    02
  0000000140EC99A4: 48 33 C4           xor         rax,rsp
  0000000140EC99A7: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140EC99AF: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140EC99B7: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140EC99BF: FF 15 03 A6 F3 00  call        qword ptr [0000000141E03FC8h]
  0000000140EC99C5: 0F B6 C0           movzx       eax,al
  0000000140EC99C8: 85 C0              test        eax,eax
  0000000140EC99CA: 74 04              je          0000000140EC99D0
  0000000140EC99CC: 33 C0              xor         eax,eax
  0000000140EC99CE: EB 5F              jmp         0000000140EC9A2F
  0000000140EC99D0: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140EC99D5: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140EC99DD: FF 15 6D A5 F3 00  call        qword ptr [0000000141E03F50h]
  0000000140EC99E3: 90                 nop
  0000000140EC99E4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC99E9: FF 15 39 A8 F3 00  call        qword ptr [0000000141E04228h]
  0000000140EC99EF: 48 8B D0           mov         rdx,rax
  0000000140EC99F2: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EC99F7: E8 C4 64 5A 00     call        000000014146FEC0
  0000000140EC99FC: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  0000000140EC9A01: 48 8D 15 7C B5 C4  lea         rdx,[0000000142B14F84h]
                    01
  0000000140EC9A08: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EC9A0D: E8 C2 45 5A 00     call        000000014146DFD4
  0000000140EC9A12: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140EC9A16: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  0000000140EC9A1D: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC9A22: FF 15 68 A8 F3 00  call        qword ptr [0000000141E04290h]
  0000000140EC9A28: 8B 84 24 10 01 00  mov         eax,dword ptr [rsp+0000000000000110h]
                    00
  0000000140EC9A2F: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EC9A37: 48 33 CC           xor         rcx,rsp
  0000000140EC9A3A: E8 D1 44 5A 00     call        000000014146DF10
  0000000140EC9A3F: 48 81 C4 38 01 00  add         rsp,138h
                    00
  0000000140EC9A46: C3                 ret

  Summary

     1E02000 .text
