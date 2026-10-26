Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140412430: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140412435: 48 83 EC 58        sub         rsp,58h
  0000000140412439: 33 D2              xor         edx,edx
  000000014041243B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140412440: E8 BB F3 FF FF     call        0000000140411800
  0000000140412445: 48 8D 0D 6C 42 96  lea         rcx,[0000000143D766B8h]
                    03
  000000014041244C: E8 3F 5C EB FF     call        00000001402C8090
  0000000140412451: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140412456: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014041245C: 74 18              je          0000000140412476
  000000014041245E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140412463: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140412466: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014041246B: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  0000000140412471: 48 85 C0           test        rax,rax
  0000000140412474: 75 02              jne         0000000140412478
  0000000140412476: EB 63              jmp         00000001404124DB
  0000000140412478: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014041247D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140412480: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140412485: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  000000014041248B: 48 8B C8           mov         rcx,rax
  000000014041248E: E8 4D B4 8C 00     call        0000000140CDD8E0
  0000000140412493: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140412498: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014041249E: 75 02              jne         00000001404124A2
  00000001404124A0: EB 39              jmp         00000001404124DB
  00000001404124A2: 48 8D 44 24 30     lea         rax,[rsp+30h]
  00000001404124A7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001404124AC: 48 8D 15 A5 BB F5  lea         rdx,[000000014236E058h]
                    01
  00000001404124B3: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001404124B8: FF 15 C2 1F 9F 01  call        qword ptr [0000000141E04480h]
  00000001404124BE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001404124C3: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001404124C8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001404124CB: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001404124D0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001404124D5: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  00000001404124DB: 48 83 C4 58        add         rsp,58h
  00000001404124DF: C3                 ret

  Summary

     1E02000 .text
