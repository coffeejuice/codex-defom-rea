Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140817A80: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140817A85: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140817A89: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140817A8E: 48 83 EC 38        sub         rsp,38h
  0000000140817A92: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140817A97: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  0000000140817A9C: 7E 34              jle         0000000140817AD2
  0000000140817A9E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140817AA3: 48 83 C1 28        add         rcx,28h
  0000000140817AA7: E8 34 D0 7E FF     call        0000000140004AE0
  0000000140817AAC: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  0000000140817AB0: 7D 20              jge         0000000140817AD2
  0000000140817AB2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140817AB7: 48 83 C1 28        add         rcx,28h
  0000000140817ABB: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000140817ABF: E8 4C 73 AD FF     call        00000001402EEE10
  0000000140817AC4: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000140817AC7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140817ACC: FF 15 9E C9 5E 01  call        qword ptr [0000000141E04470h]
  0000000140817AD2: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140817AD6: 48 83 C4 38        add         rsp,38h
  0000000140817ADA: C3                 ret

  Summary

     1E02000 .text
