Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C60E0: mov         qword ptr [rsp+10h],rbx
  00000000670C60E5: push        rdi
  00000000670C60E6: sub         rsp,20h
  00000000670C60EA: mov         rbx,rcx
  00000000670C60ED: mov         rcx,qword ptr [rcx+8]
  00000000670C60F1: mov         rdi,rdx
  00000000670C60F4: cmp         qword ptr [rcx+28h],0
  00000000670C60F9: jne         00000000670C611B
  00000000670C60FB: cmp         qword ptr [rcx],0
  00000000670C60FF: jne         00000000670C611B
  00000000670C6101: lea         rcx,[000000006722C3C8h]
  00000000670C6108: call        000000006701CEE0
  00000000670C610D: mov         rax,rbx
  00000000670C6110: mov         rbx,qword ptr [rsp+38h]
  00000000670C6115: add         rsp,20h
  00000000670C6119: pop         rdi
  00000000670C611A: ret
  00000000670C611B: lea         rdx,[rsp+30h]
  00000000670C6120: call        00000000670C5210
  00000000670C6125: test        eax,eax
  00000000670C6127: je          00000000670C6166
  00000000670C6129: dec         eax
  00000000670C612B: cmp         eax,1
  00000000670C612E: ja          00000000670C616C
  00000000670C6130: mov         rcx,rbx
  00000000670C6133: mov         dword ptr [rdi],0
  00000000670C6139: call        00000000670C2660
  00000000670C613E: neg         al
  00000000670C6140: mov         rax,qword ptr [rbx+8]
  00000000670C6144: sbb         ecx,ecx
  00000000670C6146: add         ecx,2
  00000000670C6149: cmp         dword ptr [rax+00000000000000CCh],0
  00000000670C6150: jne         00000000670C616C
  00000000670C6152: mov         dword ptr [rax+00000000000000CCh],ecx
  00000000670C6158: mov         rax,rbx
  00000000670C615B: mov         rbx,qword ptr [rsp+38h]
  00000000670C6160: add         rsp,20h
  00000000670C6164: pop         rdi
  00000000670C6165: ret
  00000000670C6166: mov         eax,dword ptr [rsp+30h]
  00000000670C616A: mov         dword ptr [rdi],eax
  00000000670C616C: mov         rax,rbx
  00000000670C616F: mov         rbx,qword ptr [rsp+38h]
  00000000670C6174: add         rsp,20h
  00000000670C6178: pop         rdi
  00000000670C6179: ret
  00000000670C617A: int         3

  Summary

      1A7000 .text
