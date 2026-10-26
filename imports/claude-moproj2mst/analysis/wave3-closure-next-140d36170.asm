Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D36170: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D36175: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D3617A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3617F: 48 83 EC 38        sub         rsp,38h
  0000000140D36183: 48 8D 15 96 06 CD  lea         rdx,[0000000142A06820h]
                    01
  0000000140D3618A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140D3618F: FF 15 7B DF 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D36195: 48 8B D0           mov         rdx,rax
  0000000140D36198: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D3619D: E8 BE C2 40 FF     call        0000000140142460
  0000000140D361A2: 0F B6 C0           movzx       eax,al
  0000000140D361A5: 85 C0              test        eax,eax
  0000000140D361A7: 74 11              je          0000000140D361BA
  0000000140D361A9: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140D361AE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D361B3: E8 18 E9 FF FF     call        0000000140D34AD0
  0000000140D361B8: EB 35              jmp         0000000140D361EF
  0000000140D361BA: 48 8D 15 67 06 CD  lea         rdx,[0000000142A06828h]
                    01
  0000000140D361C1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D361C6: FF 15 44 DF 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D361CC: 48 8B D0           mov         rdx,rax
  0000000140D361CF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D361D4: E8 87 C2 40 FF     call        0000000140142460
  0000000140D361D9: 0F B6 C0           movzx       eax,al
  0000000140D361DC: 85 C0              test        eax,eax
  0000000140D361DE: 74 0F              je          0000000140D361EF
  0000000140D361E0: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140D361E5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D361EA: E8 E1 E8 FF FF     call        0000000140D34AD0
  0000000140D361EF: 48 83 C4 38        add         rsp,38h
  0000000140D361F3: C3                 ret

  Summary

     1E02000 .text
