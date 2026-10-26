Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065049BF0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065049BF5: 57                 push        rdi
  0000000065049BF6: 48 83 EC 20        sub         rsp,20h
  0000000065049BFA: 83 FA 03           cmp         edx,3
  0000000065049BFD: 49 63 F8           movsxd      rdi,r8d
  0000000065049C00: 48 8B D9           mov         rbx,rcx
  0000000065049C03: 7C 1C              jl          0000000065049C21
  0000000065049C05: 83 FA 04           cmp         edx,4
  0000000065049C08: 75 08              jne         0000000065049C12
  0000000065049C0A: 8B 51 08           mov         edx,dword ptr [rcx+8]
  0000000065049C0D: 83 E2 0F           and         edx,0Fh
  0000000065049C10: EB 0F              jmp         0000000065049C21
  0000000065049C12: 48 8D 0D 6F BD 65  lea         rcx,[00000000656A5988h]
                    00
  0000000065049C19: FF 15 79 CD 64 00  call        qword ptr [0000000065696998h]
  0000000065049C1F: 33 D2              xor         edx,edx
  0000000065049C21: 48 63 C2           movsxd      rax,edx
  0000000065049C24: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  0000000065049C28: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065049C2B: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065049C30: 48 8D 14 8F        lea         rdx,[rdi+rcx*4]
  0000000065049C34: 48 8D 44 D0 08     lea         rax,[rax+rdx*8+8]
  0000000065049C39: 48 83 C4 20        add         rsp,20h
  0000000065049C3D: 5F                 pop         rdi
  0000000065049C3E: C3                 ret

  Summary

      695000 .text
