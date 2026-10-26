Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140029300: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140029305: 48 83 EC 38        sub         rsp,38h
  0000000140029309: 48 8D 0D A8 D3 D4  lea         rcx,[0000000143D766B8h]
                    03
  0000000140029310: E8 AB 24 2A 00     call        00000001402CB7C0
  0000000140029315: 0F B6 C0           movzx       eax,al
  0000000140029318: 85 C0              test        eax,eax
  000000014002931A: 74 21              je          000000014002933D
  000000014002931C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140029321: E8 6A 42 FF FF     call        000000014001D590
  0000000140029326: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002932B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140029331: 74 0A              je          000000014002933D
  0000000140029333: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140029338: E8 83 9B 02 00     call        0000000140052EC0
  000000014002933D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140029342: E8 59 BD C9 00     call        0000000140CC50A0
  0000000140029347: 48 83 C4 38        add         rsp,38h
  000000014002934B: C3                 ret

  Summary

     1E02000 .text
