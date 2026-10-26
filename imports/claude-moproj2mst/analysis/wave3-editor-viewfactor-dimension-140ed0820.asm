Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ED0820: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140ED0824: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ED0829: 48 83 EC 28        sub         rsp,28h
  0000000140ED082D: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140ED0832: 75 1D              jne         0000000140ED0851
  0000000140ED0834: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140ED0839: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED0840: 33 D2              xor         edx,edx
  0000000140ED0842: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140ED0849: FF 15 F1 B1 F3 00  call        qword ptr [0000000141E0BA40h]
  0000000140ED084F: EB 1B              jmp         0000000140ED086C
  0000000140ED0851: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140ED0856: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED085D: B2 01              mov         dl,1
  0000000140ED085F: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140ED0866: FF 15 D4 B1 F3 00  call        qword ptr [0000000141E0BA40h]
  0000000140ED086C: 48 83 C4 28        add         rsp,28h
  0000000140ED0870: C3                 ret

  Summary

     1E02000 .text
