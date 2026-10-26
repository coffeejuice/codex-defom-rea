Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412E75B0: 48 85 D2           test        rdx,rdx
  00000001412E75B3: 74 15              je          00000001412E75CA
  00000001412E75B5: 83 7A 34 01        cmp         dword ptr [rdx+34h],1
  00000001412E75B9: 7E 04              jle         00000001412E75BF
  00000001412E75BB: C6 41 2F 01        mov         byte ptr [rcx+2Fh],1
  00000001412E75BF: 8B 42 34           mov         eax,dword ptr [rdx+34h]
  00000001412E75C2: 39 41 44           cmp         dword ptr [rcx+44h],eax
  00000001412E75C5: 7D 03              jge         00000001412E75CA
  00000001412E75C7: 89 41 44           mov         dword ptr [rcx+44h],eax
  00000001412E75CA: F3 C3              rep ret
  00000001412E75CC: CC                 int         3
  00000001412E75CD: CC                 int         3
  00000001412E75CE: CC                 int         3
  00000001412E75CF: CC                 int         3
  00000001412E75D0: 48 8B C4           mov         rax,rsp
  00000001412E75D3: 57                 push        rdi
  00000001412E75D4: 48 81 EC F0 04 00  sub         rsp,4F0h
                    00
  00000001412E75DB: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412E75E4: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000001412E75E8: 48 89 70 10        mov         qword ptr [rax+10h],rsi
  00000001412E75EC: 48 8B 05 3D 6A 87  mov         rax,qword ptr [0000000143B5E030h]
                    02
  00000001412E75F3: 48 33 C4           xor         rax,rsp
  00000001412E75F6: 48 89 84 24 E0 04  mov         qword ptr [rsp+00000000000004E0h],rax
                    00 00
  00000001412E75FE: 49 8B D9           mov         rbx,r9

  Summary

     1E02000 .text
