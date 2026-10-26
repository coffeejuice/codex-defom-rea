Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067074120: 40 53              push        rbx
  0000000067074122: 48 83 EC 30        sub         rsp,30h
  0000000067074126: 48 83 3D E2 FC 27  cmp         qword ptr [00000000672F3E10h],0
                    00 00
  000000006707412E: 8B C2              mov         eax,edx
  0000000067074130: 48 8B D9           mov         rbx,rcx
  0000000067074133: 0F 84 92 00 00 00  je          00000000670741CB
  0000000067074139: 48 85 C9           test        rcx,rcx
  000000006707413C: 75 18              jne         0000000067074156
  000000006707413E: 48 8D 1D EB C9 27  lea         rbx,[00000000672F0B30h]
                    00
  0000000067074145: F0 83 05 E3 C9 27  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  000000006707414D: 48 8B C3           mov         rax,rbx
  0000000067074150: 48 83 C4 30        add         rsp,30h
  0000000067074154: 5B                 pop         rbx
  0000000067074155: C3                 ret
  0000000067074156: 85 D2              test        edx,edx
  0000000067074158: 74 09              je          0000000067074163
  000000006707415A: 80 39 00           cmp         byte ptr [rcx],0
  000000006707415D: 75 1C              jne         000000006707417B
  000000006707415F: 85 D2              test        edx,edx
  0000000067074161: 79 21              jns         0000000067074184
  0000000067074163: 48 8D 1D E6 C9 27  lea         rbx,[00000000672F0B50h]
                    00
  000000006707416A: F0 83 05 DE C9 27  lock add    dword ptr [00000000672F0B50h],1
                    00 01
  0000000067074172: 48 8B C3           mov         rax,rbx
  0000000067074175: 48 83 C4 30        add         rsp,30h
  0000000067074179: 5B                 pop         rbx
  000000006707417A: C3                 ret
  000000006707417B: 85 D2              test        edx,edx
  000000006707417D: 79 05              jns         0000000067074184
  000000006707417F: E8 2C DA F8 FF     call        0000000067001BB0
  0000000067074184: 48 8B 0D 85 FC 27  mov         rcx,qword ptr [00000000672F3E10h]
                    00
  000000006707418B: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000067074190: 44 8B C8           mov         r9d,eax
  0000000067074193: 4C 8B C3           mov         r8,rbx
  0000000067074196: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000006707419F: E8 BC E2 FF FF     call        0000000067072460
  00000000670741A4: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670741A9: F0 83 03 01        lock add    dword ptr [rbx],1
  00000000670741AD: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670741B2: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670741B6: 75 BA              jne         0000000067074172
  00000000670741B8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670741BD: E8 0E BF FF FF     call        00000000670700D0
  00000000670741C2: 48 8B C3           mov         rax,rbx
  00000000670741C5: 48 83 C4 30        add         rsp,30h
  00000000670741C9: 5B                 pop         rbx
  00000000670741CA: C3                 ret
  00000000670741CB: 48 83 C4 30        add         rsp,30h
  00000000670741CF: 5B                 pop         rbx
  00000000670741D0: E9 6B 7D FF FF     jmp         000000006706BF40

  Summary

      1A7000 .text
