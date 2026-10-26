Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400B5BB0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400B5BB5: 48 83 EC 48        sub         rsp,48h
  00000001400B5BB9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400B5BBE: 48 83 C1 30        add         rcx,30h
  00000001400B5BC2: E8 29 53 B6 00     call        0000000140C1AEF0
  00000001400B5BC7: 48 85 C0           test        rax,rax
  00000001400B5BCA: 74 5A              je          00000001400B5C26
  00000001400B5BCC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400B5BD1: 48 83 C1 30        add         rcx,30h
  00000001400B5BD5: E8 16 53 B6 00     call        0000000140C1AEF0
  00000001400B5BDA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400B5BDF: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400B5BE4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400B5BE9: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400B5BEF: 74 1C              je          00000001400B5C0D
  00000001400B5BF1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400B5BF6: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400B5BF9: BA 01 00 00 00     mov         edx,1
  00000001400B5BFE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400B5C03: FF 50 18           call        qword ptr [rax+18h]
  00000001400B5C06: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400B5C0B: EB 09              jmp         00000001400B5C16
  00000001400B5C0D: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400B5C16: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400B5C1B: 48 83 C1 30        add         rcx,30h
  00000001400B5C1F: 33 D2              xor         edx,edx
  00000001400B5C21: E8 FA 6C C0 00     call        0000000140CBC920
  00000001400B5C26: 48 83 C4 48        add         rsp,48h
  00000001400B5C2A: C3                 ret

  Summary

     1E02000 .text
