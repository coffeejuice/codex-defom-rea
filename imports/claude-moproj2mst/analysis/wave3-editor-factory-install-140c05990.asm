Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C05990: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C05995: 48 83 EC 48        sub         rsp,48h
  0000000140C05999: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C059A2: B9 18 00 00 00     mov         ecx,18h
  0000000140C059A7: E8 10 91 86 00     call        000000014146EABC
  0000000140C059AC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C059B1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C059B7: 74 11              je          0000000140C059CA
  0000000140C059B9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C059BE: E8 0D 1A 00 00     call        0000000140C073D0
  0000000140C059C3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C059C8: EB 09              jmp         0000000140C059D3
  0000000140C059CA: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C059D3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C059D8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C059DD: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C059E2: 48 83 C4 48        add         rsp,48h
  0000000140C059E6: C3                 ret

  Summary

     1E02000 .text
