Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB5500: mov         qword ptr [rsp+8],rcx
  0000000140EB5505: sub         rsp,48h
  0000000140EB5509: mov         dword ptr [rsp+34h],0
  0000000140EB5511: mov         dword ptr [rsp+28h],0
  0000000140EB5519: mov         rax,qword ptr [rsp+50h]
  0000000140EB551E: mov         rax,qword ptr [rax]
  0000000140EB5521: mov         rcx,qword ptr [rsp+50h]
  0000000140EB5526: call        qword ptr [rax+0000000000000198h]
  0000000140EB552C: mov         qword ptr [rsp+20h],rax
  0000000140EB5531: lea         r8,[rsp+28h]
  0000000140EB5536: lea         rdx,[rsp+34h]
  0000000140EB553B: mov         rcx,qword ptr [rsp+50h]
  0000000140EB5540: call        0000000140EB6210
  0000000140EB5545: mov         rcx,qword ptr [rsp+50h]
  0000000140EB554A: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EB5551: mov         rcx,qword ptr [rcx+8]
  0000000140EB5555: call        qword ptr [0000000141E05C38h]
  0000000140EB555B: add         eax,1
  0000000140EB555E: mov         dword ptr [rsp+30h],eax
  0000000140EB5562: mov         rax,qword ptr [rsp+50h]
  0000000140EB5567: mov         eax,dword ptr [rax+00000000000000BCh]
  0000000140EB556D: add         eax,1
  0000000140EB5570: mov         dword ptr [rsp+2Ch],eax
  0000000140EB5574: cmp         dword ptr [rsp+30h],4
  0000000140EB5579: jne         0000000140EB55C4
  0000000140EB557B: mov         eax,dword ptr [rsp+2Ch]
  0000000140EB557F: and         eax,2
  0000000140EB5582: test        eax,eax
  0000000140EB5584: je          0000000140EB55C4
  0000000140EB5586: mov         dl,1
  0000000140EB5588: mov         rcx,qword ptr [rsp+50h]
  0000000140EB558D: call        0000000140ECD430
  0000000140EB5592: mov         dl,1
  0000000140EB5594: mov         rcx,qword ptr [rsp+50h]
  0000000140EB5599: call        0000000140ECD2F0
  0000000140EB559E: mov         r8b,1
  0000000140EB55A1: mov         edx,1
  0000000140EB55A6: mov         rcx,qword ptr [rsp+50h]
  0000000140EB55AB: call        0000000140ECE670
  0000000140EB55B0: xor         r8d,r8d
  0000000140EB55B3: mov         edx,2
  0000000140EB55B8: mov         rcx,qword ptr [rsp+50h]
  0000000140EB55BD: call        0000000140ECE670
  0000000140EB55C2: jmp         0000000140EB561A
  0000000140EB55C4: mov         dl,1
  0000000140EB55C6: mov         rcx,qword ptr [rsp+50h]
  0000000140EB55CB: call        0000000140ECD430
  0000000140EB55D0: xor         edx,edx
  0000000140EB55D2: mov         rcx,qword ptr [rsp+50h]
  0000000140EB55D7: call        0000000140ECD2F0
  0000000140EB55DC: mov         rax,qword ptr [rsp+50h]
  0000000140EB55E1: mov         rax,qword ptr [rax]
  0000000140EB55E4: mov         rcx,qword ptr [rsp+50h]
  0000000140EB55E9: call        qword ptr [rax+00000000000001B0h]
  0000000140EB55EF: mov         rcx,rax
  0000000140EB55F2: call        000000014120B2C0
  0000000140EB55F7: mov         rax,qword ptr [rsp+50h]
  0000000140EB55FC: mov         rax,qword ptr [rax]
  0000000140EB55FF: mov         rcx,qword ptr [rsp+50h]
  0000000140EB5604: call        qword ptr [rax+0000000000000198h]
  0000000140EB560A: mov         r9b,1
  0000000140EB560D: xor         r8d,r8d
  0000000140EB5610: xor         edx,edx
  0000000140EB5612: mov         rcx,rax
  0000000140EB5615: call        0000000140C83CA0
  0000000140EB561A: add         rsp,48h
  0000000140EB561E: ret
  0000000140EB561F: int         3

  Summary

     1E02000 .text
