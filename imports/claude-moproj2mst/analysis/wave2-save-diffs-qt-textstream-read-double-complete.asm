Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C62C0: mov         qword ptr [rsp+10h],rbx
  00000000670C62C5: push        rdi
  00000000670C62C6: sub         rsp,20h
  00000000670C62CA: mov         rbx,rcx
  00000000670C62CD: mov         rcx,qword ptr [rcx+8]
  00000000670C62D1: mov         rdi,rdx
  00000000670C62D4: cmp         qword ptr [rcx+28h],0
  00000000670C62D9: jne         00000000670C62FB
  00000000670C62DB: cmp         qword ptr [rcx],0
  00000000670C62DF: jne         00000000670C62FB
  00000000670C62E1: lea         rcx,[000000006722C3C8h]
  00000000670C62E8: call        000000006701CEE0
  00000000670C62ED: mov         rax,rbx
  00000000670C62F0: mov         rbx,qword ptr [rsp+38h]
  00000000670C62F5: add         rsp,20h
  00000000670C62F9: pop         rdi
  00000000670C62FA: ret
  00000000670C62FB: lea         rdx,[rsp+30h]
  00000000670C6300: call        00000000670C59B0
  00000000670C6305: test        al,al
  00000000670C6307: je          00000000670C6321
  00000000670C6309: movsd       xmm0,mmword ptr [rsp+30h]
  00000000670C630F: mov         rax,rbx
  00000000670C6312: movsd       mmword ptr [rdi],xmm0
  00000000670C6316: mov         rbx,qword ptr [rsp+38h]
  00000000670C631B: add         rsp,20h
  00000000670C631F: pop         rdi
  00000000670C6320: ret
  00000000670C6321: mov         rcx,rbx
  00000000670C6324: mov         qword ptr [rdi],0
  00000000670C632B: call        00000000670C2660
  00000000670C6330: neg         al
  00000000670C6332: mov         rax,qword ptr [rbx+8]
  00000000670C6336: sbb         ecx,ecx
  00000000670C6338: add         ecx,2
  00000000670C633B: cmp         dword ptr [rax+00000000000000CCh],0
  00000000670C6342: jne         00000000670C634A
  00000000670C6344: mov         dword ptr [rax+00000000000000CCh],ecx
  00000000670C634A: mov         rax,rbx
  00000000670C634D: mov         rbx,qword ptr [rsp+38h]
  00000000670C6352: add         rsp,20h
  00000000670C6356: pop         rdi
  00000000670C6357: ret
  00000000670C6358: int         3

  Summary

      1A7000 .text
