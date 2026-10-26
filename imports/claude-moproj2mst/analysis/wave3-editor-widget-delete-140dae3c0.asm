Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE3C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE3C5: 48 83 EC 48        sub         rsp,48h
  0000000140DAE3C9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE3CE: 48 83 C1 18        add         rcx,18h
  0000000140DAE3D2: E8 19 CB E6 FF     call        0000000140C1AEF0
  0000000140DAE3D7: 48 85 C0           test        rax,rax
  0000000140DAE3DA: 74 5D              je          0000000140DAE439
  0000000140DAE3DC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE3E1: 48 83 C1 18        add         rcx,18h
  0000000140DAE3E5: E8 06 CB E6 FF     call        0000000140C1AEF0
  0000000140DAE3EA: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DAE3EF: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140DAE3F5: 74 42              je          0000000140DAE439
  0000000140DAE3F7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DAE3FF: 4C 8D 0D F2 E9 7B  lea         r9,[000000014356CDF8h]
                    02
  0000000140DAE406: 4C 8D 05 4B 91 7B  lea         r8,[0000000143567558h]
                    02
  0000000140DAE40D: 33 D2              xor         edx,edx
  0000000140DAE40F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DAE414: E8 E3 14 6C 00     call        000000014146F8FC
  0000000140DAE419: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DAE41E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140DAE424: 74 13              je          0000000140DAE439
  0000000140DAE426: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140DAE42B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DAE42E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140DAE433: FF 90 D0 01 00 00  call        qword ptr [rax+00000000000001D0h]
  0000000140DAE439: 48 83 C4 48        add         rsp,48h
  0000000140DAE43D: C3                 ret

  Summary

     1E02000 .text
