Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065426BB0: mov         qword ptr [rsp+10h],rbx
  0000000065426BB5: mov         qword ptr [rsp+18h],rbp
  0000000065426BBA: push        rsi
  0000000065426BBB: push        rdi
  0000000065426BBC: push        r12
  0000000065426BBE: sub         rsp,20h
  0000000065426BC2: mov         rsi,qword ptr [rcx+8]
  0000000065426BC6: mov         r12,rdx
  0000000065426BC9: mov         rbp,rcx
  0000000065426BCC: mov         r8d,dword ptr [rsi+0000000000000090h]
  0000000065426BD3: mov         ebx,1
  0000000065426BD8: test        r8d,r8d
  0000000065426BDB: je          0000000065426BE7
  0000000065426BDD: cmp         r8d,ebx
  0000000065426BE0: jne         0000000065426BE7
  0000000065426BE2: mov         ebx,2
  0000000065426BE7: lea         rdx,[rsi+0000000000000088h]
  0000000065426BEE: lea         rcx,[rsp+40h]
  0000000065426BF3: call        qword ptr [0000000065696A90h]
  0000000065426BF9: lea         r9,[rsp+40h]
  0000000065426BFE: mov         r8d,ebx
  0000000065426C01: mov         rdx,r12
  0000000065426C04: mov         rcx,rsi
  0000000065426C07: call        00000000654264F0
  0000000065426C0C: mov         edi,eax
  0000000065426C0E: mov         dword ptr [rsp+58h],eax
  0000000065426C12: cmp         eax,2
  0000000065426C15: je          0000000065426C78
  0000000065426C17: mov         rdx,qword ptr [rbp+8]
  0000000065426C1B: lea         rcx,[rsp+40h]
  0000000065426C20: add         rdx,88h
  0000000065426C27: call        qword ptr [0000000065696A90h]
  0000000065426C2D: lea         rcx,[rsp+40h]
  0000000065426C32: call        qword ptr [0000000065698EF8h]
  0000000065426C38: cmp         eax,1
  0000000065426C3B: je          0000000065426C78
  0000000065426C3D: xor         r8d,r8d
  0000000065426C40: lea         rcx,[rsp+40h]
  0000000065426C45: lea         edx,[r8+1]
  0000000065426C49: call        qword ptr [0000000065696B20h]
  0000000065426C4F: mov         r8d,ebx
  0000000065426C52: mov         rdx,r12
  0000000065426C55: mov         r9,rax
  0000000065426C58: mov         rcx,rsi
  0000000065426C5B: call        00000000654264F0
  0000000065426C60: lea         rcx,[rsp+40h]
  0000000065426C65: lea         rdx,[rsp+58h]
  0000000065426C6A: cmp         edi,eax
  0000000065426C6C: mov         dword ptr [rsp+40h],eax
  0000000065426C70: cmovge      rcx,rdx
  0000000065426C74: mov         eax,dword ptr [rcx]
  0000000065426C76: jmp         0000000065426C7A
  0000000065426C78: mov         eax,edi
  0000000065426C7A: mov         rbx,qword ptr [rsp+48h]
  0000000065426C7F: mov         rbp,qword ptr [rsp+50h]
  0000000065426C84: add         rsp,20h
  0000000065426C88: pop         r12
  0000000065426C8A: pop         rdi
  0000000065426C8B: pop         rsi
  0000000065426C8C: ret
  0000000065426C8D: int         3

  Summary

      695000 .text
