Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014004E810: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014004E814: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014004E819: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014004E81E: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  000000014004E822: 89 81 04 01 00 00  mov         dword ptr [rcx+0000000000000104h],eax
  000000014004E828: C3                 ret
  000000014004E829: CC                 int         3
  000000014004E82A: CC                 int         3
  000000014004E82B: CC                 int         3
  000000014004E82C: CC                 int         3
  000000014004E82D: CC                 int         3
  000000014004E82E: CC                 int         3
  000000014004E82F: CC                 int         3
  000000014004E830: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014004E835: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014004E83A: 48 83 EC 48        sub         rsp,48h
  000000014004E83E: 0F 29 74 24 30     movaps      xmmword ptr [rsp+30h],xmm6
  000000014004E843: 0F 29 7C 24 20     movaps      xmmword ptr [rsp+20h],xmm7
  000000014004E848: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014004E84D: E8 3E EF 6D 01     call        000000014172D790
  000000014004E852: 0F B6 C0           movzx       eax,al
  000000014004E855: 85 C0              test        eax,eax
  000000014004E857: 74 7B              je          000000014004E8D4
  000000014004E859: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014004E85E: E8 DD 32 C1 00     call        0000000140C61B40
  000000014004E863: 66 0F 28 F0        movapd      xmm6,xmm0
  000000014004E867: 48 8D 15 1A 09 0D  lea         rdx,[000000014211F188h]
                    02
  000000014004E86E: 48
  000000014004E86F: 8B

  Summary

     1E02000 .text
