Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014175F540: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014175F545: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014175F54A: 48 83 EC 28        sub         rsp,28h
  000000014175F54E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014175F554: 75 04              jne         000000014175F55A
  000000014175F556: 33 C0              xor         eax,eax
  000000014175F558: EB 2C              jmp         000000014175F586
  000000014175F55A: 48 8D 15 1F 26 8D  lea         rdx,[0000000143031B80h]
                    01
  000000014175F561: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014175F566: E8 D5 FA D0 FF     call        000000014146F040
  000000014175F56B: 85 C0              test        eax,eax
  000000014175F56D: 75 07              jne         000000014175F576
  000000014175F56F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014175F574: EB 10              jmp         000000014175F586
  000000014175F576: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  000000014175F57B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014175F580: FF 15 FA 50 6A 00  call        qword ptr [0000000141E04680h]
  000000014175F586: 48 83 C4 28        add         rsp,28h
  000000014175F58A: C3                 ret

  Summary

     1E02000 .text
