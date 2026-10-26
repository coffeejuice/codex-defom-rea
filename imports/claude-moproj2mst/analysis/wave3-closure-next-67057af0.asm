Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067057AF0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067057AF5: 57                 push        rdi
  0000000067057AF6: 48 83 EC 20        sub         rsp,20h
  0000000067057AFA: 8B F9              mov         edi,ecx
  0000000067057AFC: B9 80 00 00 00     mov         ecx,80h
  0000000067057B01: E8 80 0F 12 00     call        0000000067178A86
  0000000067057B06: 48 8B D8           mov         rbx,rax
  0000000067057B09: 48 85 C0           test        rax,rax
  0000000067057B0C: 75 05              jne         0000000067057B13
  0000000067057B0E: E8 9D 44 FC FF     call        000000006701BFB0
  0000000067057B13: 33 D2              xor         edx,edx
  0000000067057B15: 48 89 1B           mov         qword ptr [rbx],rbx
  0000000067057B18: 48 89 5B 08        mov         qword ptr [rbx+8],rbx
  0000000067057B1C: C7 43 68 01 00 00  mov         dword ptr [rbx+68h],1
                    00
  0000000067057B23: 89 53 6C           mov         dword ptr [rbx+6Ch],edx
  0000000067057B26: 89 53 70           mov         dword ptr [rbx+70h],edx
  0000000067057B29: 89 53 74           mov         dword ptr [rbx+74h],edx
  0000000067057B2C: 83 FF 08           cmp         edi,8
  0000000067057B2F: 48 8B C3           mov         rax,rbx
  0000000067057B32: 0F 9F C2           setg        dl
  0000000067057B35: 83 E2 01           and         edx,1
  0000000067057B38: C1 E2 02           shl         edx,2
  0000000067057B3B: 83 CA 02           or          edx,2
  0000000067057B3E: 89 53 78           mov         dword ptr [rbx+78h],edx
  0000000067057B41: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067057B46: 48 83 C4 20        add         rsp,20h
  0000000067057B4A: 5F                 pop         rdi
  0000000067057B4B: C3                 ret

  Summary

      1A7000 .text
