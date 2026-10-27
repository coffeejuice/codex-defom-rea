Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140025100: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140025105: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002510A: 48 83 EC 58        sub         rsp,58h
  000000014002510E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140025117: BA 01 00 00 00     mov         edx,1
  000000014002511C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140025121: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140025124: FF 15 AE F1 DD 01  call        qword ptr [0000000141E042D8h]
  000000014002512A: 0F B6 C0           movzx       eax,al
  000000014002512D: 85 C0              test        eax,eax
  000000014002512F: 74 31              je          0000000140025162
  0000000140025131: 41 B8 01 00 00 00  mov         r8d,1
  0000000140025137: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  000000014002513C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140025141: E8 7A 02 00 00     call        00000001400253C0
  0000000140025146: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002514B: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140025150: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140025155: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002515A: E8 21 04 00 00     call        0000000140025580
  000000014002515F: 90                 nop
  0000000140025160: EB 69              jmp         00000001400251CB
  0000000140025162: 33 C0              xor         eax,eax
  0000000140025164: 85 C0              test        eax,eax
  0000000140025166: 74 27              je          000000014002518F
  0000000140025168: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002516D: FF 15 8D F0 DD 01  call        qword ptr [0000000141E04200h]
  0000000140025173: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140025178: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014002517D: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140025182: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140025187: E8 F4 03 00 00     call        0000000140025580
  000000014002518C: 90                 nop
  000000014002518D: EB 3C              jmp         00000001400251CB
  000000014002518F: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140025194: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140025199: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002519E: E8 DD 03 00 00     call        0000000140025580
  00000001400251A3: 90                 nop
  00000001400251A4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400251A9: FF 15 51 F0 DD 01  call        qword ptr [0000000141E04200h]
  00000001400251AF: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400251B4: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400251B9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400251BE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400251C3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400251C8: 48 89 01           mov         qword ptr [rcx],rax
  00000001400251CB: 48 83 C4 58        add         rsp,58h
  00000001400251CF: C3                 ret

  Summary

     1E02000 .text
