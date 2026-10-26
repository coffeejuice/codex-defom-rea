Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D9BA0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400D9BA5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400D9BAA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D9BAE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D9BB3: 48 83 EC 28        sub         rsp,28h
  00000001400D9BB7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001400D9BBC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001400D9BC1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001400D9BC5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400D9BCA: E8 01 D0 29 00     call        0000000140376BD0
  00000001400D9BCF: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001400D9BD3: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001400D9BD8: 7D 06              jge         00000001400D9BE0
  00000001400D9BDA: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400D9BDE: EB 35              jmp         00000001400D9C15
  00000001400D9BE0: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001400D9BE5: 75 2A              jne         00000001400D9C11
  00000001400D9BE7: 83 7C 24 40 16     cmp         dword ptr [rsp+40h],16h
  00000001400D9BEC: 7D 18              jge         00000001400D9C06
  00000001400D9BEE: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001400D9BF3: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001400D9BF8: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001400D9BFC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400D9C01: E8 7A FC FF FF     call        00000001400D9880
  00000001400D9C06: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400D9C0A: 83 E8 16           sub         eax,16h
  00000001400D9C0D: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001400D9C11: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001400D9C15: 48 83 C4 28        add         rsp,28h
  00000001400D9C19: C3                 ret

  Summary

     1E02000 .text
