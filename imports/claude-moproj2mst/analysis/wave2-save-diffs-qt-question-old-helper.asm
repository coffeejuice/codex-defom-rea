Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065476F80: push        rbx
  0000000065476F82: sub         rsp,80h
  0000000065476F89: mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
  0000000065476F92: mov         dword ptr [rsp+40h],103h
  0000000065476F9A: mov         dword ptr [rsp+44h],0
  0000000065476FA2: lea         rax,[rsp+40h]
  0000000065476FA7: mov         qword ptr [rsp+30h],rax
  0000000065476FAC: mov         qword ptr [rsp+28h],rcx
  0000000065476FB1: lea         rax,[rsp+44h]
  0000000065476FB6: mov         qword ptr [rsp+20h],rax
  0000000065476FBB: lea         rcx,[rsp+50h]
  0000000065476FC0: call        0000000065476D40
  0000000065476FC5: nop
  0000000065476FC6: mov         r9d,dword ptr [rsp+00000000000000C0h]
  0000000065476FCE: mov         r8d,dword ptr [rsp+00000000000000B8h]
  0000000065476FD6: mov         edx,dword ptr [rsp+00000000000000B0h]
  0000000065476FDD: mov         rcx,qword ptr [rsp+58h]
  0000000065476FE2: call        0000000065476640
  0000000065476FE7: lea         rcx,[rsp+50h]
  0000000065476FEC: call        0000000065459420
  0000000065476FF1: mov         ebx,eax
  0000000065476FF3: lea         rax,[0000000065720EB8h]
  0000000065476FFA: mov         qword ptr [rsp+50h],rax
  0000000065476FFF: lea         rax,[0000000065720E80h]
  0000000065477006: mov         qword ptr [rsp+60h],rax
  000000006547700B: lea         rcx,[rsp+50h]
  0000000065477010: call        00000000654590D0
  0000000065477015: mov         eax,ebx
  0000000065477017: add         rsp,80h
  000000006547701E: pop         rbx
  000000006547701F: ret
  0000000065477020: sub         rsp,48h
  0000000065477024: mov         eax,dword ptr [rsp+78h]
  0000000065477028: mov         dword ptr [rsp+30h],eax
  000000006547702C: mov         eax,dword ptr [rsp+70h]
  0000000065477030: mov         dword ptr [rsp+28h],eax
  0000000065477034: mov         dword ptr [rsp+20h],r9d
  0000000065477039: mov         r9,r8
  000000006547703C: mov         r8,rdx
  000000006547703F: mov         edx,1
  0000000065477044: call        0000000065476F80
  0000000065477049: add         rsp,48h
  000000006547704D: ret
  000000006547704E: int         3
  000000006547704F: int         3
  0000000065477050: 48

  Summary

      695000 .text
