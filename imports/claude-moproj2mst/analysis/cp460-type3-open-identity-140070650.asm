Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140070650: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140070655: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014007065A: 48 83 EC 48        sub         rsp,48h
  000000014007065E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140070667: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014007066C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140070670: 48 81 C1 50 01 00  add         rcx,150h
                    00
  0000000140070677: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014007067C: E8 9F 02 00 00     call        0000000140070920
  0000000140070681: 0F B6 C0           movzx       eax,al
  0000000140070684: 85 C0              test        eax,eax
  0000000140070686: 74 36              je          00000001400706BE
  0000000140070688: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014007068D: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140070691: 48 81 C1 50 01 00  add         rcx,150h
                    00
  0000000140070698: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014007069D: E8 AE 49 C0 00     call        0000000140C75050
  00000001400706A2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400706A5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400706AA: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400706AF: FF 15 2B 53 D9 01  call        qword ptr [0000000141E059E0h]
  00000001400706B5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400706BA: EB 29              jmp         00000001400706E5
  00000001400706BC: EB 1B              jmp         00000001400706D9
  00000001400706BE: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001400706C7: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400706CC: FF 15 0E 53 D9 01  call        qword ptr [0000000141E059E0h]
  00000001400706D2: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400706D7: EB 0C              jmp         00000001400706E5
  00000001400706D9: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400706DE: FF 15 FC 52 D9 01  call        qword ptr [0000000141E059E0h]
  00000001400706E4: 90                 nop
  00000001400706E5: 48 83 C4 48        add         rsp,48h
  00000001400706E9: C3                 ret

  Summary

     1E02000 .text
