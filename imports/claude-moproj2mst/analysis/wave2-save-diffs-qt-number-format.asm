Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070FD0: mov         qword ptr [rsp+8],rcx
  0000000067070FD5: push        rbx
  0000000067070FD6: sub         rsp,30h
  0000000067070FDA: mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
  0000000067070FE3: mov         rbx,rcx
  0000000067070FE6: mov         dword ptr [rsp+20h],0
  0000000067070FEE: lea         rax,[00000000672F0B30h]
  0000000067070FF5: mov         qword ptr [rcx],rax
  0000000067070FF8: lock add    dword ptr [00000000672F0B30h],1
  0000000067071000: mov         dword ptr [rsp+20h],1
  0000000067071008: call        0000000067070E30
  000000006707100D: mov         rax,rbx
  0000000067071010: add         rsp,30h
  0000000067071014: pop         rbx
  0000000067071015: ret
  0000000067071016: int         3
  0000000067071017: int         3
  0000000067071018: int         3
  0000000067071019: int         3
  000000006707101A: int         3
  000000006707101B: int         3
  000000006707101C: int         3
  000000006707101D: int         3
  000000006707101E: int         3
  000000006707101F: int         3
  0000000067071020: mov         qword ptr [rsp+10h],rdx
  0000000067071025: push        rbp
  0000000067071026: push        rsi
  0000000067071027: push        rdi
  0000000067071028: push        r12
  000000006707102A: push        r13
  000000006707102C: push        r14
  000000006707102E: push        r15
  0000000067071030: sub         rsp,30h
  0000000067071034: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  000000006707103D: mov         qword ptr [rsp+0000000000000080h],rbx
  0000000067071045: mov         rsi,rdx
  0000000067071048: mov         rbp,rcx
  000000006707104B: xor         r15d,r15d
  000000006707104E: mov         dword ptr [rsp+70h],r15d
  0000000067071053: mov         rax,qword ptr [rcx]
  0000000067071056: mov         r13d,dword ptr [rax+8]
  000000006707105A: test        r13d,r13d
  000000006707105D: jne         000000006707106E
  000000006707105F: mov         qword ptr [rdx],rax
  0000000067071062: lock add    dword ptr [rax],1
  0000000067071066: mov         rax,rdx
  0000000067071069: jmp         00000000670711D3
  000000006707106E: cmp         r8d,1
  0000000067071072: jg          00000000670710A3
  0000000067071074: cmp         r8d,1
  0000000067071078: jne         0000000067071089
  000000006707107A: mov         qword ptr [rdx],rax
  000000006707107D: lock add    dword ptr [rax],r8d
  0000000067071081: mov         rax,rsi
  0000000067071084: jmp         00000000670711D3
  0000000067071089: lea         rdi,[00000000672F0B30h]
  0000000067071090: mov         qword ptr [rdx],rdi
  0000000067071093: lock add    dword ptr [00000000672F0B30h],1
  000000006707109B: mov         rax,rsi
  000000006707109E: jmp         00000000670711D3
  00000000670710A3: imul        r13d,r8d
  00000000670710A7: lea         rdi,[00000000672F0B30h]
  00000000670710AE: 48
  00000000670710AF: 89
  00000000670710B0: BC

  Summary

      1A7000 .text
