Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140375FC0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140375FC5: 48 83 EC 48        sub         rsp,48h
  0000000140375FC9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140375FD2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140375FD7: 48 83 78 60 00     cmp         qword ptr [rax+60h],0
  0000000140375FDC: 74 3F              je          000000014037601D
  0000000140375FDE: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140375FE3: 48 8B 40 60        mov         rax,qword ptr [rax+60h]
  0000000140375FE7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140375FEC: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140375FF1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140375FF6: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140375FFC: 74 16              je          0000000140376014
  0000000140375FFE: BA 01 00 00 00     mov         edx,1
  0000000140376003: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140376008: E8 73 CF C8 FF     call        0000000140002F80
  000000014037600D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140376012: EB 09              jmp         000000014037601D
  0000000140376014: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  000000014037601D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140376022: 48 83 C1 58        add         rcx,58h
  0000000140376026: E8 35 72 D3 FF     call        00000001400AD260
  000000014037602B: 90                 nop
  000000014037602C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140376031: 48 83 C1 50        add         rcx,50h
  0000000140376035: E8 A6 64 D3 FF     call        00000001400AC4E0
  000000014037603A: 90                 nop
  000000014037603B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140376040: 48 83 C1 30        add         rcx,30h
  0000000140376044: FF 15 6E F9 A8 01  call        qword ptr [0000000141E059B8h]
  000000014037604A: 90                 nop
  000000014037604B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140376050: 48 83 C1 20        add         rcx,20h
  0000000140376054: FF 15 5E F9 A8 01  call        qword ptr [0000000141E059B8h]
  000000014037605A: 90                 nop
  000000014037605B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140376060: 48 83 C1 10        add         rcx,10h
  0000000140376064: FF 15 4E F9 A8 01  call        qword ptr [0000000141E059B8h]
  000000014037606A: 48 83 C4 48        add         rsp,48h
  000000014037606E: C3                 ret

  Summary

     1E02000 .text
