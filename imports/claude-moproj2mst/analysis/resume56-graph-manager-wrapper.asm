Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C15910: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C15915: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C1591A: 48 83 EC 38        sub         rsp,38h
  0000000140C1591E: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140C15927: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C1592C: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140C15931: 75 04              jne         0000000140C15937
  0000000140C15933: 33 C0              xor         eax,eax
  0000000140C15935: EB 27              jmp         0000000140C1595E
  0000000140C15937: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C1593C: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140C15940: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C15945: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140C15949: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C1594C: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140C15951: FF 50 60           call        qword ptr [rax+60h]
  0000000140C15954: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C15959: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C1595E: 48 83 C4 38        add         rsp,38h
  0000000140C15962: C3                 ret
  0000000140C15963: CC                 int         3

  Summary

     1E02000 .text
