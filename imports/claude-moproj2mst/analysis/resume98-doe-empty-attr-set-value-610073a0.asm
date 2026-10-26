Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  00000000610073A0: 40 57              push        rdi
  00000000610073A2: 48 83 EC 30        sub         rsp,30h
  00000000610073A6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000610073AF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000610073B4: 48 8B FA           mov         rdi,rdx
  00000000610073B7: 48 8B D9           mov         rbx,rcx
  00000000610073BA: 48 83 C1 40        add         rcx,40h
  00000000610073BE: FF 15 EC C4 03 00  call        qword ptr [00000000610438B0h]
  00000000610073C4: B9 68 00 00 00     mov         ecx,68h
  00000000610073C9: E8 6C F4 02 00     call        000000006103683A
  00000000610073CE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000610073D3: 48 85 C0           test        rax,rax
  00000000610073D6: 74 15              je          00000000610073ED
  00000000610073D8: 4C 8B CF           mov         r9,rdi
  00000000610073DB: 4C 8B C3           mov         r8,rbx
  00000000610073DE: 33 D2              xor         edx,edx
  00000000610073E0: 48 8B C8           mov         rcx,rax
  00000000610073E3: E8 F8 D8 FF FF     call        0000000061004CE0
  00000000610073E8: 48 8B F8           mov         rdi,rax
  00000000610073EB: EB 02              jmp         00000000610073EF
  00000000610073ED: 33 FF              xor         edi,edi
  00000000610073EF: 48 8D 4F 08        lea         rcx,[rdi+8]
  00000000610073F3: FF 15 2F C4 03 00  call        qword ptr [0000000061043828h]
  00000000610073F9: 48 8B 53 28        mov         rdx,qword ptr [rbx+28h]
  00000000610073FD: 48 85 D2           test        rdx,rdx
  0000000061007400: 74 1C              je          000000006100741E
  0000000061007402: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000061007405: 48 8B CB           mov         rcx,rbx
  0000000061007408: FF 50 28           call        qword ptr [rax+28h]
  000000006100740B: 48 85 C0           test        rax,rax
  000000006100740E: 74 0E              je          000000006100741E
  0000000061007410: 4C 8B 00           mov         r8,qword ptr [rax]
  0000000061007413: BA 01 00 00 00     mov         edx,1
  0000000061007418: 48 8B C8           mov         rcx,rax
  000000006100741B: 41 FF 10           call        qword ptr [r8]
  000000006100741E: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000061007421: 48 8B D7           mov         rdx,rdi
  0000000061007424: 48 8B CB           mov         rcx,rbx
  0000000061007427: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006100742C: 48 83 C4 30        add         rsp,30h
  0000000061007430: 5F                 pop         rdi
  0000000061007431: 48 FF 60 30        jmp         qword ptr [rax+30h]

  Summary

       42000 .text
