Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140002480: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140002485: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014000248A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014000248F: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  0000000140002496: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400024A2: 4C 8B 84 24 10 01  mov         r8,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400024AA: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400024B2: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400024BA: FF 15 88 9A E0 01  call        qword ptr [0000000141E0BF48h]
  00000001400024C0: 88 84 24 88 00 00  mov         byte ptr [rsp+0000000000000088h],al
                    00
  00000001400024C7: 0F B6 84 24 88 00  movzx       eax,byte ptr [rsp+0000000000000088h]
                    00 00
  00000001400024CF: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400024D3: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  00000001400024D7: EB 0E              jmp         00000001400024E7
  00000001400024D9: EB 02              jmp         00000001400024DD
  00000001400024DB: EB 08              jmp         00000001400024E5
  00000001400024DD: EB 06              jmp         00000001400024E5
  00000001400024DF: 8A 44 24 70        mov         al,byte ptr [rsp+70h]
  00000001400024E3: EB 02              jmp         00000001400024E7
  00000001400024E5: 32 C0              xor         al,al
  00000001400024E7: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  00000001400024EE: C3                 ret
  00000001400024EF: CC                 int         3

  Summary

     1E02000 .text
