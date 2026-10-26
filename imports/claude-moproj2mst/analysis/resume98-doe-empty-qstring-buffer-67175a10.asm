Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067175A10: 40 53              push        rbx
  0000000067175A12: 48 83 EC 20        sub         rsp,20h
  0000000067175A16: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067175A19: 48 8B D9           mov         rbx,rcx
  0000000067175A1C: 8B 10              mov         edx,dword ptr [rax]
  0000000067175A1E: 83 FA 01           cmp         edx,1
  0000000067175A21: 75 17              jne         0000000067175A3A
  0000000067175A23: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067175A26: 48 8D 42 1A        lea         rax,[rdx+1Ah]
  0000000067175A2A: 48 39 42 10        cmp         qword ptr [rdx+10h],rax
  0000000067175A2E: 75 0A              jne         0000000067175A3A
  0000000067175A30: 48 8B 42 10        mov         rax,qword ptr [rdx+10h]
  0000000067175A34: 48 83 C4 20        add         rsp,20h
  0000000067175A38: 5B                 pop         rbx
  0000000067175A39: C3                 ret
  0000000067175A3A: E8 81 A9 EF FF     call        00000000670703C0
  0000000067175A3F: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067175A42: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000067175A46: 48 83 C4 20        add         rsp,20h
  0000000067175A4A: 5B                 pop         rbx
  0000000067175A4B: C3                 ret

  Summary

      1A7000 .text
