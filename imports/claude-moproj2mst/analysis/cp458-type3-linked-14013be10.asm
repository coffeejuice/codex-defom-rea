Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014013BE10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014013BE15: 48 83 EC 48        sub         rsp,48h
  000000014013BE19: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014013BE22: B9 48 00 00 00     mov         ecx,48h
  000000014013BE27: E8 90 2C 33 01     call        000000014146EABC
  000000014013BE2C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014013BE31: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014013BE37: 74 2C              je          000000014013BE65
  000000014013BE39: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014013BE3E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014013BE41: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014013BE46: FF 90 88 01 00 00  call        qword ptr [rax+0000000000000188h]
  000000014013BE4C: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  000000014013BE51: 48 8B D0           mov         rdx,rax
  000000014013BE54: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014013BE59: E8 C2 67 00 00     call        0000000140142620
  000000014013BE5E: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014013BE63: EB 09              jmp         000000014013BE6E
  000000014013BE65: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  000000014013BE6E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014013BE73: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014013BE78: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014013BE7D: 48 81 C1 B0 00 00  add         rcx,0B0h
                    00
  000000014013BE84: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014013BE89: E8 92 0A B8 00     call        0000000140CBC920
  000000014013BE8E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014013BE93: 48 81 C1 B0 00 00  add         rcx,0B0h
                    00
  000000014013BE9A: E8 51 F0 AD 00     call        0000000140C1AEF0
  000000014013BE9F: 48 83 C4 48        add         rsp,48h
  000000014013BEA3: C3                 ret

  Summary

     1E02000 .text
