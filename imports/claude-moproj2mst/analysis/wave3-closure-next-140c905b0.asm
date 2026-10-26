Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C905B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C905B5: 48 83 EC 48        sub         rsp,48h
  0000000140C905B9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C905C2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C905C7: 48 83 B8 40 02 00  cmp         qword ptr [rax+0000000000000240h],0
                    00 00
  0000000140C905CF: 75 51              jne         0000000140C90622
  0000000140C905D1: B9 80 05 00 00     mov         ecx,580h
  0000000140C905D6: E8 E1 E4 7D 00     call        000000014146EABC
  0000000140C905DB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C905E0: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C905E6: 74 16              je          0000000140C905FE
  0000000140C905E8: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140C905ED: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C905F2: E8 19 50 08 00     call        0000000140D15610
  0000000140C905F7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C905FC: EB 09              jmp         0000000140C90607
  0000000140C905FE: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C90607: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C9060C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C90611: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C90616: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C9061B: 48 89 81 40 02 00  mov         qword ptr [rcx+0000000000000240h],rax
                    00
  0000000140C90622: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90627: 48 8B 80 40 02 00  mov         rax,qword ptr [rax+0000000000000240h]
                    00
  0000000140C9062E: 48 83 C4 48        add         rsp,48h
  0000000140C90632: C3                 ret

  Summary

     1E02000 .text
