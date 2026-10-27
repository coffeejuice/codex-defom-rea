Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091E520: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091E524: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091E529: 48 83 EC 38        sub         rsp,38h
  000000014091E52D: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  000000014091E535: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  000000014091E53A: 7E 2D              jle         000000014091E569
  000000014091E53C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E541: 48 83 C1 18        add         rcx,18h
  000000014091E545: E8 96 65 6E FF     call        0000000140004AE0
  000000014091E54A: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  000000014091E54E: 7D 19              jge         000000014091E569
  000000014091E550: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E555: 48 83 C1 18        add         rcx,18h
  000000014091E559: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  000000014091E55D: E8 BE 15 00 00     call        000000014091FB20
  000000014091E562: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000014091E565: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091E569: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091E56D: 48 83 C4 38        add         rsp,38h
  000000014091E571: C3                 ret

  Summary

     1E02000 .text
