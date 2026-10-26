Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file analysis\resume48-distortion-vt198-rea-session\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB2470: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140EB2475: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140EB247A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EB247F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB2484: 56                 push        rsi
  0000000140EB2485: 57                 push        rdi
  0000000140EB2486: 48 83 EC 78        sub         rsp,78h
  0000000140EB248A: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EB2493: 45 33 C0           xor         r8d,r8d
  0000000140EB2496: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140EB249E: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB24A6: FF 15 E4 64 F5 00  call        qword ptr [0000000141E08990h]
  0000000140EB24AC: 90                 nop
  0000000140EB24AD: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB24B5: 48 8D 05 7C 40 C6  lea         rax,[0000000142B16538h]
                    01
  0000000140EB24BC: 48 89 01           mov         qword ptr [rcx],rax
  0000000140EB24BF: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB24C7: 48 83 C1 30        add         rcx,30h
  0000000140EB24CB: E8 80 7F 22 00     call        00000001410DA450
  0000000140EB24D0: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  0000000140EB24D5: 48 8B B4 24 A0 00  mov         rsi,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EB24DD: B9 40 00 00 00     mov         ecx,40h
  0000000140EB24E2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140EB24E4: 48 8B BC 24 90 00  mov         rdi,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB24EC: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  0000000140EB24F1: 48 83 C7 30        add         rdi,30h
  0000000140EB24F5: B9 40 00 00 00     mov         ecx,40h
  0000000140EB24FA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140EB24FC: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB2504: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140EB250C: 48 89 42 70        mov         qword ptr [rdx+70h],rax
  0000000140EB2510: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB2518: 48 83 C4 78        add         rsp,78h
  0000000140EB251C: 5F                 pop         rdi
  0000000140EB251D: 5E                 pop         rsi
  0000000140EB251E: C3                 ret
  0000000140EB251F: CC                 int         3

  Summary

     1E02000 .text
