Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091E3A0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091E3A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091E3A9: 48 83 EC 38        sub         rsp,38h
  000000014091E3AD: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  000000014091E3B5: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  000000014091E3BA: 7E 2D              jle         000000014091E3E9
  000000014091E3BC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E3C1: 48 83 C1 18        add         rcx,18h
  000000014091E3C5: E8 16 67 6E FF     call        0000000140004AE0
  000000014091E3CA: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  000000014091E3CE: 7D 19              jge         000000014091E3E9
  000000014091E3D0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E3D5: 48 83 C1 18        add         rcx,18h
  000000014091E3D9: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  000000014091E3DD: E8 3E 17 00 00     call        000000014091FB20
  000000014091E3E2: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014091E3E5: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091E3E9: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091E3ED: 48 83 C4 38        add         rsp,38h
  000000014091E3F1: C3                 ret

  Summary

     1E02000 .text
