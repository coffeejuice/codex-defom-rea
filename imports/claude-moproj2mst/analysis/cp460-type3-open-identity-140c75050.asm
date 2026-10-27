Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C75050: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C75055: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7505A: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  0000000140C75061: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C75069: E8 E2 B9 3F FF     call        0000000140070A50
  0000000140C7506E: 4C 8B 84 24 B8 00  mov         r8,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C75076: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C7507B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C75083: E8 68 BA 3F FF     call        0000000140070AF0
  0000000140C75088: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C75090: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C75098: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C7509B: 48 39 84 24 90 00  cmp         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C750A3: 75 46              jne         0000000140C750EB
  0000000140C750A5: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  0000000140C750B1: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  0000000140C750B9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C750BE: 4C 8B 8C 24 B8 00  mov         r9,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C750C6: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  0000000140C750CB: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C750D3: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140C750D6: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C750DE: E8 FD BC 3F FF     call        0000000140070DE0
  0000000140C750E3: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C750EB: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C750F3: E8 68 78 75 00     call        00000001413CC960
  0000000140C750F8: 48 83 C0 08        add         rax,8
  0000000140C750FC: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  0000000140C75103: C3                 ret

  Summary

     1E02000 .text
