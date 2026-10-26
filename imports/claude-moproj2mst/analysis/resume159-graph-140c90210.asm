Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C90210: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C90215: 48 83 EC 48        sub         rsp,48h
  0000000140C90219: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C90222: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90227: 48 83 B8 30 02 00  cmp         qword ptr [rax+0000000000000230h],0
                    00 00
  0000000140C9022F: 75 51              jne         0000000140C90282
  0000000140C90231: B9 60 00 00 00     mov         ecx,60h
  0000000140C90236: E8 81 E8 7D 00     call        000000014146EABC
  0000000140C9023B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C90240: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C90246: 74 16              je          0000000140C9025E
  0000000140C90248: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140C9024D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C90252: E8 A9 7E 0E 00     call        0000000140D78100
  0000000140C90257: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C9025C: EB 09              jmp         0000000140C90267
  0000000140C9025E: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C90267: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C9026C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C90271: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C90276: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C9027B: 48 89 81 30 02 00  mov         qword ptr [rcx+0000000000000230h],rax
                    00
  0000000140C90282: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90287: 48 8B 80 30 02 00  mov         rax,qword ptr [rax+0000000000000230h]
                    00
  0000000140C9028E: 48 83 C4 48        add         rsp,48h
  0000000140C90292: C3                 ret

  Summary

     1E02000 .text
