Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054350: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054355: 48 83 EC 38        sub         rsp,38h
  0000000140054359: 48 8D 0D 58 23 D2  lea         rcx,[0000000143D766B8h]
                    03
  0000000140054360: E8 2B 3D 27 00     call        00000001402C8090
  0000000140054365: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014005436A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005436F: E8 BC CE FF FF     call        0000000140051230
  0000000140054374: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140054379: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014005437F: 74 10              je          0000000140054391
  0000000140054381: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140054386: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054389: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005438E: FF 50 58           call        qword ptr [rax+58h]
  0000000140054391: 48 83 C4 38        add         rsp,38h
  0000000140054395: C3                 ret

  Summary

     1E02000 .text
