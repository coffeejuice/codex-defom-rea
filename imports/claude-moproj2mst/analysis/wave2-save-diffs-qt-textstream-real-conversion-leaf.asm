Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704DCE0: movzx       r9d,word ptr [rcx+2]
  000000006704DCE5: movzx       eax,word ptr [rcx]
  000000006704DCE8: mov         ecx,187h
  000000006704DCED: shr         r9d,1
  000000006704DCF0: not         r9d
  000000006704DCF3: and         r9d,1
  000000006704DCF7: cmp         ax,cx
  000000006704DCFA: jne         000000006704DD08
  000000006704DCFC: mov         rcx,qword ptr [00000000672F3D38h]
  000000006704DD03: jmp         000000006704C340
  000000006704DD08: movzx       eax,ax
  000000006704DD0B: lea         rcx,[rax+rax*8]
  000000006704DD0F: lea         rax,[00000000671B2040h]
  000000006704DD16: shl         rcx,4
  000000006704DD1A: add         rcx,rax
  000000006704DD1D: jmp         000000006704C340
  000000006704DD22: int         3
  000000006704DD23: int         3
  000000006704DD24: int         3
  000000006704DD25: int         3
  000000006704DD26: int         3
  000000006704DD27: int         3
  000000006704DD28: int         3
  000000006704DD29: int         3
  000000006704DD2A: int         3
  000000006704DD2B: int         3
  000000006704DD2C: int         3
  000000006704DD2D: int         3
  000000006704DD2E: int         3
  000000006704DD2F: int         3
  000000006704DD30: mov         rax,rsp
  000000006704DD33: push        rsi
  000000006704DD34: push        rdi
  000000006704DD35: push        r12
  000000006704DD37: push        r13
  000000006704DD39: push        r14
  000000006704DD3B: sub         rsp,50h
  000000006704DD3F: 48
  000000006704DD40: C7

  Summary

      1A7000 .text
