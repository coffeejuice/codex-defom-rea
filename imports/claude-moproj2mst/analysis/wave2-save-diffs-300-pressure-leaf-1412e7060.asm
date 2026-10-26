Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412E7060: cmp         edx,2
  00000001412E7063: jne         00000001412E706D
  00000001412E7065: cmp         dword ptr [rcx+3Ch],0
  00000001412E7069: setne       al
  00000001412E706C: ret
  00000001412E706D: cmp         dword ptr [rcx+40h],0
  00000001412E7071: setne       al
  00000001412E7074: ret
  00000001412E7075: int         3
  00000001412E7076: int         3
  00000001412E7077: int         3
  00000001412E7078: int         3
  00000001412E7079: int         3
  00000001412E707A: int         3
  00000001412E707B: int         3
  00000001412E707C: int         3
  00000001412E707D: int         3
  00000001412E707E: int         3
  00000001412E707F: int         3
  00000001412E7080: push        rbp
  00000001412E7082: push        rsi
  00000001412E7083: push        rdi
  00000001412E7084: push        r12
  00000001412E7086: push        r15
  00000001412E7088: sub         rsp,30h
  00000001412E708C: xor         edi,edi
  00000001412E708E: mov         r15,r9
  00000001412E7091: mov         rbp,r8
  00000001412E7094: mov         qword ptr [rcx],rdi
  00000001412E7097: mov         qword ptr [rdx],rdi
  00000001412E709A: mov         rsi,rdx
  00000001412E709D: mov         qword ptr [r8],rdi
  00000001412E70A0: mov         r12,rcx
  00000001412E70A3: test        r9,r9
  00000001412E70A6: je          00000001412E73BB
  00000001412E70AC: mov         qword ptr [rsp+68h],rbx
  00000001412E70B1: lea         ecx,[rdi+40h]
  00000001412E70B4: mov         qword ptr [rsp+70h],r13
  00000001412E70B9: mov         qword ptr [rsp+78h],r14
  00000001412E70BE: call        000000014146EABC
  00000001412E70C3: lea         r9,[0000000142DB4B00h]
  00000001412E70CA: lea         r8d,[rdi+40h]
  00000001412E70CE: lea         rcx,[00000001446EFE90h]
  00000001412E70D5: mov         edx,eax
  00000001412E70D7: mov         qword ptr [r12],rax
  00000001412E70DB: C7
  00000001412E70DC: and         al,20h
  00000001412E70DF: movs        byte ptr [rdi],byte ptr [rsi]
  00000001412E70E0: 16

  Summary

     1E02000 .text
