Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067073EF0: 40 53              push        rbx
  0000000067073EF2: 48 83 EC 20        sub         rsp,20h
  0000000067073EF6: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067073EF9: 48 8B DA           mov         rbx,rdx
  0000000067073EFC: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000067073F04: 44 3B 40 08        cmp         r8d,dword ptr [rax+8]
  0000000067073F08: 7D 1A              jge         0000000067073F24
  0000000067073F0A: 45 85 C0           test        r8d,r8d
  0000000067073F0D: 78 15              js          0000000067073F24
  0000000067073F0F: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  0000000067073F13: 48 8B CB           mov         rcx,rbx
  0000000067073F16: E8 05 74 FF FF     call        000000006706B320
  0000000067073F1B: 48 8B C3           mov         rax,rbx
  0000000067073F1E: 48 83 C4 20        add         rsp,20h
  0000000067073F22: 5B                 pop         rbx
  0000000067073F23: C3                 ret
  0000000067073F24: 48 89 02           mov         qword ptr [rdx],rax
  0000000067073F27: F0 83 00 01        lock add    dword ptr [rax],1
  0000000067073F2B: 48 8B C3           mov         rax,rbx
  0000000067073F2E: 48 83 C4 20        add         rsp,20h
  0000000067073F32: 5B                 pop         rbx
  0000000067073F33: C3                 ret

  Summary

      1A7000 .text
