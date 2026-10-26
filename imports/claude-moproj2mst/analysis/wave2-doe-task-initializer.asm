Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140741500: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140741505: 48 83 EC 38        sub         rsp,38h
  0000000140741509: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014074150E: C7 80 B8 00 00 00  mov         dword ptr [rax+00000000000000B8h],0
                    00 00 00 00
  0000000140741518: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014074151D: E8 BE 21 CC 00     call        00000001414036E0
  0000000140741522: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140741527: E8 E4 B1 CC 00     call        000000014140C710
  000000014074152C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140741531: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140741536: E8 D5 5E BC FF     call        0000000140307410
  000000014074153B: 0F B6 C0           movzx       eax,al
  000000014074153E: 85 C0              test        eax,eax
  0000000140741540: 75 11              jne         0000000140741553
  0000000140741542: 33 D2              xor         edx,edx
  0000000140741544: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140741549: E8 62 A0 A0 FF     call        000000014014B5B0
  000000014074154E: 83 38 00           cmp         dword ptr [rax],0
  0000000140741551: 75 21              jne         0000000140741574
  0000000140741553: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140741558: E8 53 0F 90 FF     call        00000001400424B0
  000000014074155D: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140741565: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000014074156A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014074156F: E8 8C 3B 8E FF     call        0000000140025100
  0000000140741574: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140741579: C6 80 A8 00 00 00  mov         byte ptr [rax+00000000000000A8h],1
                    01
  0000000140741580: 48 83 C4 38        add         rsp,38h
  0000000140741584: C3                 ret
  0000000140741585: CC                 int         3
  0000000140741586: CC                 int         3
  0000000140741587: CC                 int         3
  0000000140741588: CC                 int         3
  0000000140741589: CC                 int         3
  000000014074158A: CC                 int         3
  000000014074158B: CC                 int         3
  000000014074158C: CC                 int         3
  000000014074158D: CC                 int         3
  000000014074158E: CC                 int         3
  000000014074158F: CC                 int         3

  Summary

     1E02000 .text
