Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AD260: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400AD265: 48 83 EC 28        sub         rsp,28h
  00000001400AD269: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400AD26E: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001400AD271: FF 15 71 70 D5 01  call        qword ptr [0000000141E042E8h]
  00000001400AD277: 0F B6 C0           movzx       eax,al
  00000001400AD27A: 85 C0              test        eax,eax
  00000001400AD27C: 75 12              jne         00000001400AD290
  00000001400AD27E: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001400AD283: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000001400AD286: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400AD28B: E8 30 09 00 00     call        00000001400ADBC0
  00000001400AD290: 48 83 C4 28        add         rsp,28h
  00000001400AD294: C3                 ret

  Summary

     1E02000 .text
