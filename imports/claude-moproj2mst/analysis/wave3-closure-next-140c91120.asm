Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C91120: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C91125: 48 83 EC 48        sub         rsp,48h
  0000000140C91129: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C9112E: 48 83 B8 50 02 00  cmp         qword ptr [rax+0000000000000250h],0
                    00 00
  0000000140C91136: 74 52              je          0000000140C9118A
  0000000140C91138: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C9113D: 48 8B 80 50 02 00  mov         rax,qword ptr [rax+0000000000000250h]
                    00
  0000000140C91144: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C91149: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C9114E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C91153: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C91159: 74 16              je          0000000140C91171
  0000000140C9115B: BA 01 00 00 00     mov         edx,1
  0000000140C91160: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C91165: E8 96 08 00 00     call        0000000140C91A00
  0000000140C9116A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C9116F: EB 09              jmp         0000000140C9117A
  0000000140C91171: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140C9117A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C9117F: 48 C7 80 50 02 00  mov         qword ptr [rax+0000000000000250h],0
                    00 00 00 00 00
  0000000140C9118A: 48 83 C4 48        add         rsp,48h
  0000000140C9118E: C3                 ret

  Summary

     1E02000 .text
