Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091E460: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091E464: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091E469: 48 83 EC 38        sub         rsp,38h
  000000014091E46D: 66 0F 57 C0        xorpd       xmm0,xmm0
  000000014091E471: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014091E477: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  000000014091E47C: 7E 31              jle         000000014091E4AF
  000000014091E47E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E483: 48 83 C1 18        add         rcx,18h
  000000014091E487: E8 54 66 6E FF     call        0000000140004AE0
  000000014091E48C: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  000000014091E490: 7D 1D              jge         000000014091E4AF
  000000014091E492: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091E497: 48 83 C1 18        add         rcx,18h
  000000014091E49B: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  000000014091E49F: E8 7C 16 00 00     call        000000014091FB20
  000000014091E4A4: F2 0F 10 40 18     movsd       xmm0,mmword ptr [rax+18h]
  000000014091E4A9: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014091E4AF: F2 0F 10 44 24 20  movsd       xmm0,mmword ptr [rsp+20h]
  000000014091E4B5: 48 83 C4 38        add         rsp,38h
  000000014091E4B9: C3                 ret

  Summary

     1E02000 .text
