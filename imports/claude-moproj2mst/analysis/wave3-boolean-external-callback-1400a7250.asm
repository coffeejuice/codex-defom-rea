Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A7250: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400A7254: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A7259: 48 83 EC 38        sub         rsp,38h
  00000001400A725D: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  00000001400A7262: 75 0C              jne         00000001400A7270
  00000001400A7264: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400A7269: 8B 40 50           mov         eax,dword ptr [rax+50h]
  00000001400A726C: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  00000001400A7270: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A7275: E8 36 E6 FF FF     call        00000001400A58B0
  00000001400A727A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400A727F: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000001400A7283: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A7288: E8 03 09 00 00     call        00000001400A7B90
  00000001400A728D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A7292: E8 E9 AE FF FF     call        00000001400A2180
  00000001400A7297: 8B C8              mov         ecx,eax
  00000001400A7299: E8 C2 0A 87 00     call        0000000140917D60
  00000001400A729E: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400A72A4: 74 26              je          00000001400A72CC
  00000001400A72A6: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A72AB: 48 83 C1 58        add         rcx,58h
  00000001400A72AF: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000001400A72B3: E8 58 99 0B 00     call        0000000140160C10
  00000001400A72B8: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400A72BB: E8 70 EB 00 00     call        00000001400B5E30
  00000001400A72C0: 8B D0              mov         edx,eax
  00000001400A72C2: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400A72C7: E8 24 08 FA FF     call        0000000140047AF0
  00000001400A72CC: 48 83 C4 38        add         rsp,38h
  00000001400A72D0: C3                 ret

  Summary

     1E02000 .text
