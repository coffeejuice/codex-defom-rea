Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D7A80: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400D7A85: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D7A89: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D7A8E: 48 83 EC 48        sub         rsp,48h
  00000001400D7A92: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D7A9B: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400D7AA0: 83 7C 24 58 FF     cmp         dword ptr [rsp+58h],0FFFFFFFFh
  00000001400D7AA5: 0F 8E 82 00 00 00  jle         00000001400D7B2D
  00000001400D7AAB: 83 7C 24 58 03     cmp         dword ptr [rsp+58h],3
  00000001400D7AB0: 7D 7B              jge         00000001400D7B2D
  00000001400D7AB2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D7AB7: E8 04 07 00 00     call        00000001400D81C0
  00000001400D7ABC: 90                 nop
  00000001400D7ABD: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  00000001400D7AC2: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400D7AC7: 8B 4C 24 58        mov         ecx,dword ptr [rsp+58h]
  00000001400D7ACB: E8 90 D8 83 00     call        0000000140915360
  00000001400D7AD0: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400D7AD4: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D7AD9: 85 C0              test        eax,eax
  00000001400D7ADB: 74 46              je          00000001400D7B23
  00000001400D7ADD: 48 63 4C 24 58     movsxd      rcx,dword ptr [rsp+58h]
  00000001400D7AE2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7AE7: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D7AEC: E8 4F DA 9D 00     call        0000000140AB5540
  00000001400D7AF1: 48 85 C0           test        rax,rax
  00000001400D7AF4: 76 2D              jbe         00000001400D7B23
  00000001400D7AF6: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7AFB: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D7AFF: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7B04: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D7B09: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400D7B0E: E8 1D 07 00 00     call        00000001400D8230
  00000001400D7B13: 4C 63 5C 24 58     movsxd      r11,dword ptr [rsp+58h]
  00000001400D7B18: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7B1D: 42 C6 44 18 70 00  mov         byte ptr [rax+r11+70h],0
  00000001400D7B23: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D7B28: E8 D3 4B FE FF     call        00000001400BC700
  00000001400D7B2D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D7B32: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001400D7B36: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400D7B3B: FF 15 9F DE D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7B41: 8A 44 24 30        mov         al,byte ptr [rsp+30h]
  00000001400D7B45: 48 83 C4 48        add         rsp,48h
  00000001400D7B49: C3                 ret

  Summary

     1E02000 .text
