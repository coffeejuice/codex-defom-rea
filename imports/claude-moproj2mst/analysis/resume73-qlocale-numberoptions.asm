Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704A340: 0F B7 41 02        movzx       eax,word ptr [rcx+2]
  000000006704A344: 89 02              mov         dword ptr [rdx],eax
  000000006704A346: 48 8B C2           mov         rax,rdx
  000000006704A349: C3                 ret
  000000006704A34A: CC                 int         3
  000000006704A34B: CC                 int         3
  000000006704A34C: CC                 int         3
  000000006704A34D: CC                 int         3
  000000006704A34E: CC                 int         3
  000000006704A34F: CC                 int         3
  000000006704A350: 0F B7 01           movzx       eax,word ptr [rcx]
  000000006704A353: 48 8B D1           mov         rdx,rcx
  000000006704A356: B9 87 01 00 00     mov         ecx,187h
  000000006704A35B: 66 3B C1           cmp         ax,cx
  000000006704A35E: 75 19              jne         000000006704A379
  000000006704A360: 48 8B 05 D1 99 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  000000006704A367: 48 89 05 D2 99 2A  mov         qword ptr [00000000672F3D40h],rax
                    00
  000000006704A36E: 0F B7 42 02        movzx       eax,word ptr [rdx+2]
  000000006704A372: 89 05 D0 99 2A 00  mov         dword ptr [00000000672F3D48h],eax
  000000006704A378: C3                 ret
  000000006704A379: 0F B7 C0           movzx       eax,ax
  000000006704A37C: 48 8D 0C C0        lea         rcx,[rax+rax*8]
  000000006704A380: 48

  Summary

      1A7000 .text
