Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407BC4A0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001407BC4A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407BC4A9: 48 83 EC 28        sub         rsp,28h
  00000001407BC4AD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407BC4B2: E8 29 F3 FF FF     call        00000001407BB7E0
  00000001407BC4B7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001407BC4BB: 83 E0 01           and         eax,1
  00000001407BC4BE: 85 C0              test        eax,eax
  00000001407BC4C0: 74 0A              je          00000001407BC4CC
  00000001407BC4C2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407BC4C7: E8 9C 37 D2 00     call        00000001414DFC68
  00000001407BC4CC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001407BC4D1: 48 83 C4 28        add         rsp,28h
  00000001407BC4D5: C3                 ret

  Summary

     1E02000 .text
