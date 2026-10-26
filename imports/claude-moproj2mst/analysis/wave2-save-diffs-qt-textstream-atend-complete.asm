Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2660: sub         rsp,28h
  00000000670C2664: mov         r8,qword ptr [rcx+8]
  00000000670C2668: mov         rcx,qword ptr [r8+28h]
  00000000670C266C: test        rcx,rcx
  00000000670C266F: jne         00000000670C268E
  00000000670C2671: cmp         qword ptr [r8],rcx
  00000000670C2674: jne         00000000670C2689
  00000000670C2676: lea         rcx,[000000006722C3C8h]
  00000000670C267D: call        000000006701CEE0
  00000000670C2682: mov         al,1
  00000000670C2684: add         rsp,28h
  00000000670C2688: ret
  00000000670C2689: test        rcx,rcx
  00000000670C268C: je          00000000670C26A0
  00000000670C268E: mov         eax,dword ptr [r8+30h]
  00000000670C2692: mov         rcx,qword ptr [rcx]
  00000000670C2695: cmp         dword ptr [rcx+8],eax
  00000000670C2698: sete        al
  00000000670C269B: add         rsp,28h
  00000000670C269F: ret
  00000000670C26A0: mov         rax,qword ptr [r8+0000000000000098h]
  00000000670C26A7: cmp         dword ptr [rax+8],0
  00000000670C26AB: jne         00000000670C26C9
  00000000670C26AD: mov         rcx,qword ptr [r8]
  00000000670C26B0: mov         rax,qword ptr [rcx]
  00000000670C26B3: call        qword ptr [rax+0000000000000088h]
  00000000670C26B9: test        al,al
  00000000670C26BB: je          00000000670C26C9
  00000000670C26BD: mov         edx,1
  00000000670C26C2: mov         al,dl
  00000000670C26C4: add         rsp,28h
  00000000670C26C8: ret
  00000000670C26C9: xor         edx,edx
  00000000670C26CB: mov         al,dl
  00000000670C26CD: add         rsp,28h
  00000000670C26D1: ret
  00000000670C26D2: int         3

  Summary

      1A7000 .text
