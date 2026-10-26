Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412047E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412047E5: 48 83 EC 18        sub         rsp,18h
  00000001412047E9: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  00000001412047F0: EB 09              jmp         00000001412047FB
  00000001412047F2: 8B 04 24           mov         eax,dword ptr [rsp]
  00000001412047F5: 83 C0 01           add         eax,1
  00000001412047F8: 89 04 24           mov         dword ptr [rsp],eax
  00000001412047FB: 83 3C 24 0A        cmp         dword ptr [rsp],0Ah
  00000001412047FF: 7D 16              jge         0000000141204817
  0000000141204801: 48 63 0C 24        movsxd      rcx,dword ptr [rsp]
  0000000141204805: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014120480A: C7 84 88 24 43 00  mov         dword ptr [rax+rcx*4+0000000000004324h],0
                    00 00 00 00 00
  0000000141204815: EB DB              jmp         00000001412047F2
  0000000141204817: 48 83 C4 18        add         rsp,18h
  000000014120481B: C3                 ret

  Summary

     1E02000 .text
