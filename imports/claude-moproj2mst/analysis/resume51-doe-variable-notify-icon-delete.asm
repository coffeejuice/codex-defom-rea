Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014016E8D0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014016E8D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014016E8D9: 48 83 EC 28        sub         rsp,28h
  000000014016E8DD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014016E8E2: FF 15 B8 CD C9 01  call        qword ptr [0000000141E0B6A0h]
  000000014016E8E8: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014016E8EC: 83 E0 01           and         eax,1
  000000014016E8EF: 85 C0              test        eax,eax
  000000014016E8F1: 74 0A              je          000000014016E8FD
  000000014016E8F3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014016E8F8: E8 6B 13 37 01     call        00000001414DFC68
  000000014016E8FD: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014016E902: 48 83 C4 28        add         rsp,28h
  000000014016E906: C3                 ret

  Summary

     1E02000 .text
