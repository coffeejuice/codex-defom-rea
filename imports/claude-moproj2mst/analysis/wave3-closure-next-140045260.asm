Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140045260: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140045265: 48 83 EC 68        sub         rsp,68h
  0000000140045269: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140045272: 48 8D 0D 3F 14 D3  lea         rcx,[0000000143D766B8h]
                    03
  0000000140045279: E8 12 2E 28 00     call        00000001402C8090
  000000014004527E: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140045283: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140045288: 48 8B 00           mov         rax,qword ptr [rax]
  000000014004528B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140045290: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140045296: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014004529B: B9 40 00 00 00     mov         ecx,40h
  00000001400452A0: E8 17 98 42 01     call        000000014146EABC
  00000001400452A5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400452AA: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400452B0: 74 20              je          00000001400452D2
  00000001400452B2: 4C 8B 4C 24 20     mov         r9,qword ptr [rsp+20h]
  00000001400452B7: 4C 8B 44 24 70     mov         r8,qword ptr [rsp+70h]
  00000001400452BC: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  00000001400452C1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400452C6: E8 F5 65 BC 00     call        0000000140C0B8C0
  00000001400452CB: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400452D0: EB 09              jmp         00000001400452DB
  00000001400452D2: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  00000001400452DB: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400452E0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400452E5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400452EA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400452EF: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400452F4: 48 83 C4 68        add         rsp,68h
  00000001400452F8: C3                 ret

  Summary

     1E02000 .text
