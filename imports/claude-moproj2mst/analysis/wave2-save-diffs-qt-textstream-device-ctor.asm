Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C4B00: mov         qword ptr [rsp+8],rcx
  00000000670C4B05: push        rbp
  00000000670C4B06: push        rsi
  00000000670C4B07: push        rdi
  00000000670C4B08: sub         rsp,40h
  00000000670C4B0C: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
  00000000670C4B15: mov         qword ptr [rsp+70h],rbx
  00000000670C4B1A: mov         rbp,rdx
  00000000670C4B1D: mov         rbx,rcx
  00000000670C4B20: lea         rax,[000000006722C6E0h]
  00000000670C4B27: mov         qword ptr [rcx],rax
  00000000670C4B2A: mov         ecx,0E0h
  00000000670C4B2F: call        0000000067178A86
  00000000670C4B34: mov         qword ptr [rsp+68h],rax
  00000000670C4B39: test        rax,rax
  00000000670C4B3C: je          00000000670C4B4B
  00000000670C4B3E: mov         rdx,rbx
  00000000670C4B41: mov         rcx,rax
  00000000670C4B44: call        00000000670C2B60
  00000000670C4B49: jmp         00000000670C4B4D
  00000000670C4B4B: xor         eax,eax
  00000000670C4B4D: mov         qword ptr [rbx+8],rax
  00000000670C4B51: mov         rsi,rax
  00000000670C4B54: mov         qword ptr [rax],rbp
  00000000670C4B57: xor         r9d,r9d
  00000000670C4B5A: xor         r8d,r8d
  00000000670C4B5D: xor         edx,edx
  00000000670C4B5F: lea         rcx,[rax+8]
  00000000670C4B63: call        000000006712FDC0
  00000000670C4B68: test        rbp,rbp
  00000000670C4B6B: je          00000000670C4B8F
  00000000670C4B6D: mov         dword ptr [rsp+20h],3
  00000000670C4B75: lea         r9,[000000006722C318h]
  00000000670C4B7C: lea         r8,[rsi+8]
  00000000670C4B80: lea         rdx,[000000006722C308h]
  00000000670C4B87: mov         rcx,rbp
  00000000670C4B8A: call        00000000671315C0
  00000000670C4B8F: mov         qword ptr [rsi+18h],rbx
  00000000670C4B93: mov         dword ptr [rsi+00000000000000CCh],0
  00000000670C4B9D: mov         rax,rbx
  00000000670C4BA0: mov         rbx,qword ptr [rsp+70h]
  00000000670C4BA5: add         rsp,40h
  00000000670C4BA9: pop         rdi
  00000000670C4BAA: pop         rsi
  00000000670C4BAB: pop         rbp
  00000000670C4BAC: ret
  00000000670C4BAD: int         3
  00000000670C4BAE: int         3
  00000000670C4BAF: int         3
  00000000670C4BB0: 40

  Summary

      1A7000 .text
