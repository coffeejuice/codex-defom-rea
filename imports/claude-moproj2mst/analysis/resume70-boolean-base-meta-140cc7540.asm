Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC7540: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CC7545: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC754A: 48 83 EC 28        sub         rsp,28h
  0000000140CC754E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140CC7554: 75 04              jne         0000000140CC755A
  0000000140CC7556: 33 C0              xor         eax,eax
  0000000140CC7558: EB 2C              jmp         0000000140CC7586
  0000000140CC755A: 48 8D 15 DF F2 C4  lea         rdx,[0000000142916840h]
                    01
  0000000140CC7561: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CC7566: E8 D5 7A 7A 00     call        000000014146F040
  0000000140CC756B: 85 C0              test        eax,eax
  0000000140CC756D: 75 07              jne         0000000140CC7576
  0000000140CC756F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CC7574: EB 10              jmp         0000000140CC7586
  0000000140CC7576: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  0000000140CC757B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CC7580: FF 15 4A 23 14 01  call        qword ptr [0000000141E098D0h]
  0000000140CC7586: 48 83 C4 28        add         rsp,28h
  0000000140CC758A: C3                 ret

  Summary

     1E02000 .text
