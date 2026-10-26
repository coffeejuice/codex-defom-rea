Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140048640: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140048645: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014004864A: 48 83 EC 58        sub         rsp,58h
  000000014004864E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140048657: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014004865C: E8 9F F3 FF FF     call        0000000140047A00
  0000000140048661: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140048666: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014004866C: 75 05              jne         0000000140048673
  000000014004866E: E9 8B 00 00 00     jmp         00000001400486FE
  0000000140048673: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140048678: E8 13 D9 FF FF     call        0000000140045F90
  000000014004867D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140048682: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140048688: 75 02              jne         000000014004868C
  000000014004868A: EB 72              jmp         00000001400486FE
  000000014004868C: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140048691: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140048696: E8 65 86 00 00     call        0000000140050D00
  000000014004869B: 90                 nop
  000000014004869C: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400486A1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400486A6: E8 55 F5 FF FF     call        0000000140047C00
  00000001400486AB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400486B0: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400486B6: 75 0D              jne         00000001400486C5
  00000001400486B8: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400486BD: FF 15 1D D3 DB 01  call        qword ptr [0000000141E059E0h]
  00000001400486C3: EB 39              jmp         00000001400486FE
  00000001400486C5: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001400486CB: 75 0D              jne         00000001400486DA
  00000001400486CD: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400486D2: FF 15 08 D3 DB 01  call        qword ptr [0000000141E059E0h]
  00000001400486D8: EB 24              jmp         00000001400486FE
  00000001400486DA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400486DF: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400486E2: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  00000001400486E7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400486EC: FF 90 58 01 00 00  call        qword ptr [rax+0000000000000158h]
  00000001400486F2: 90                 nop
  00000001400486F3: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400486F8: FF 15 E2 D2 DB 01  call        qword ptr [0000000141E059E0h]
  00000001400486FE: 48 83 C4 58        add         rsp,58h
  0000000140048702: C3                 ret

  Summary

     1E02000 .text
