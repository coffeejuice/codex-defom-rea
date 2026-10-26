Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EDA70: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412EDA75: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000001412EDA7A: 57                 push        rdi
  00000001412EDA7B: 48 83 EC 20        sub         rsp,20h
  00000001412EDA7F: 80 7A 29 00        cmp         byte ptr [rdx+29h],0
  00000001412EDA83: 48 8B FA           mov         rdi,rdx
  00000001412EDA86: 48 8B F1           mov         rsi,rcx
  00000001412EDA89: 48 8B DA           mov         rbx,rdx
  00000001412EDA8C: 75 22              jne         00000001412EDAB0
  00000001412EDA8E: 66 90              nop
  00000001412EDA90: 48 8B 53 10        mov         rdx,qword ptr [rbx+10h]
  00000001412EDA94: 48 8B CE           mov         rcx,rsi
  00000001412EDA97: E8 D4 FF FF FF     call        00000001412EDA70
  00000001412EDA9C: 48 8B 1B           mov         rbx,qword ptr [rbx]
  00000001412EDA9F: 48 8B CF           mov         rcx,rdi
  00000001412EDAA2: E8 C1 21 1F 00     call        00000001414DFC68
  00000001412EDAA7: 80 7B 29 00        cmp         byte ptr [rbx+29h],0
  00000001412EDAAB: 48 8B FB           mov         rdi,rbx
  00000001412EDAAE: 74 E0              je          00000001412EDA90
  00000001412EDAB0: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000001412EDAB5: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000001412EDABA: 48 83 C4 20        add         rsp,20h
  00000001412EDABE: 5F                 pop         rdi
  00000001412EDABF: C3                 ret

  Summary

     1E02000 .text
