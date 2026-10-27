Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400512C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400512C5: 48 83 EC 48        sub         rsp,48h
  00000001400512C9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400512CE: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  00000001400512D3: 74 52              je          0000000140051327
  00000001400512D5: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400512DA: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001400512DE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400512E3: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400512E8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400512ED: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400512F3: 74 1C              je          0000000140051311
  00000001400512F5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400512FA: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400512FD: BA 01 00 00 00     mov         edx,1
  0000000140051302: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140051307: FF 50 18           call        qword ptr [rax+18h]
  000000014005130A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014005130F: EB 09              jmp         000000014005131A
  0000000140051311: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014005131A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014005131F: 48 C7 40 10 00 00  mov         qword ptr [rax+10h],0
                    00 00
  0000000140051327: 48 83 C4 48        add         rsp,48h
  000000014005132B: C3                 ret

  Summary

     1E02000 .text
