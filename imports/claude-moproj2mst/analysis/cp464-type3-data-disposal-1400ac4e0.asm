Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AC4E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400AC4E5: 48 83 EC 48        sub         rsp,48h
  00000001400AC4E9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400AC4EE: 48 83 38 00        cmp         qword ptr [rax],0
  00000001400AC4F2: 74 53              je          00000001400AC547
  00000001400AC4F4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400AC4F9: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001400AC4FC: FF 15 E6 7D D5 01  call        qword ptr [0000000141E042E8h]
  00000001400AC502: 0F B6 C0           movzx       eax,al
  00000001400AC505: 85 C0              test        eax,eax
  00000001400AC507: 75 3E              jne         00000001400AC547
  00000001400AC509: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400AC50E: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400AC511: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400AC516: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400AC51B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400AC520: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400AC526: 74 16              je          00000001400AC53E
  00000001400AC528: BA 01 00 00 00     mov         edx,1
  00000001400AC52D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400AC532: E8 B9 03 00 00     call        00000001400AC8F0
  00000001400AC537: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400AC53C: EB 09              jmp         00000001400AC547
  00000001400AC53E: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400AC547: 48 83 C4 48        add         rsp,48h
  00000001400AC54B: C3                 ret

  Summary

     1E02000 .text
