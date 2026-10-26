Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400048F0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400048F5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400048FA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400048FF: 48 83 EC 48        sub         rsp,48h
  0000000140004903: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014000490C: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140004914: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140004919: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014000491E: FF 15 B4 10 E0 01  call        qword ptr [0000000141E059D8h]
  0000000140004924: 90                 nop
  0000000140004925: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014000492A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014000492F: FF 15 03 FB DF 01  call        qword ptr [0000000141E04438h]
  0000000140004935: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014000493A: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014000493F: FF 15 93 10 E0 01  call        qword ptr [0000000141E059D8h]
  0000000140004945: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140004949: 83 C8 01           or          eax,1
  000000014000494C: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140004950: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140004955: FF 15 85 10 E0 01  call        qword ptr [0000000141E059E0h]
  000000014000495B: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140004960: 48 83 C4 48        add         rsp,48h
  0000000140004964: C3                 ret
  0000000140004965: CC                 int         3

  Summary

     1E02000 .text
