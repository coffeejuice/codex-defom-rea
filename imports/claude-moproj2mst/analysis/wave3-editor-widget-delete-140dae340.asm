Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE340: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE345: 48 83 EC 48        sub         rsp,48h
  0000000140DAE349: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE34E: 48 83 C1 18        add         rcx,18h
  0000000140DAE352: E8 99 CB E6 FF     call        0000000140C1AEF0
  0000000140DAE357: 48 85 C0           test        rax,rax
  0000000140DAE35A: 74 55              je          0000000140DAE3B1
  0000000140DAE35C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE361: 48 83 C1 18        add         rcx,18h
  0000000140DAE365: E8 86 CB E6 FF     call        0000000140C1AEF0
  0000000140DAE36A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DAE36F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DAE377: 4C 8D 0D 7A EA 7B  lea         r9,[000000014356CDF8h]
                    02
  0000000140DAE37E: 4C 8D 05 D3 91 7B  lea         r8,[0000000143567558h]
                    02
  0000000140DAE385: 33 D2              xor         edx,edx
  0000000140DAE387: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140DAE38C: E8 6B 15 6C 00     call        000000014146F8FC
  0000000140DAE391: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DAE396: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140DAE39C: 74 13              je          0000000140DAE3B1
  0000000140DAE39E: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140DAE3A3: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DAE3A6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DAE3AB: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  0000000140DAE3B1: 48 83 C4 48        add         rsp,48h
  0000000140DAE3B5: C3                 ret

  Summary

     1E02000 .text
