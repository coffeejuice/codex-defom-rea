Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005F820: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005F825: 48 83 EC 38        sub         rsp,38h
  000000014005F829: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F82E: E8 5D 56 FF FF     call        0000000140054E90
  000000014005F833: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014005F838: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014005F83E: 75 07              jne         000000014005F847
  000000014005F840: 33 C0              xor         eax,eax
  000000014005F842: E9 A3 00 00 00     jmp         000000014005F8EA
  000000014005F847: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014005F84C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014005F84F: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014005F854: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005F859: FF 90 E0 00 00 00  call        qword ptr [rax+00000000000000E0h]
  000000014005F85F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014005F864: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014005F86A: 74 07              je          000000014005F873
  000000014005F86C: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014005F871: EB 77              jmp         000000014005F8EA
  000000014005F873: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F878: 48 83 C1 38        add         rcx,38h
  000000014005F87C: E8 6F B6 BB 00     call        0000000140C1AEF0
  000000014005F881: 48 85 C0           test        rax,rax
  000000014005F884: 74 10              je          000000014005F896
  000000014005F886: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F88B: 48 83 C1 38        add         rcx,38h
  000000014005F88F: E8 5C B6 BB 00     call        0000000140C1AEF0
  000000014005F894: EB 54              jmp         000000014005F8EA
  000000014005F896: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F89B: 48 83 C1 38        add         rcx,38h
  000000014005F89F: E8 4C B6 BB 00     call        0000000140C1AEF0
  000000014005F8A4: 48 85 C0           test        rax,rax
  000000014005F8A7: 75 33              jne         000000014005F8DC
  000000014005F8A9: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014005F8AE: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005F8B3: E8 C8 F9 58 00     call        00000001405EF280
  000000014005F8B8: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014005F8BD: 48 8B 00           mov         rax,qword ptr [rax]
  000000014005F8C0: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005F8C5: FF 90 B0 01 00 00  call        qword ptr [rax+00000000000001B0h]
  000000014005F8CB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F8D0: 48 83 C1 38        add         rcx,38h
  000000014005F8D4: 48 8B D0           mov         rdx,rax
  000000014005F8D7: E8 44 D0 C5 00     call        0000000140CBC920
  000000014005F8DC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005F8E1: 48 83 C1 38        add         rcx,38h
  000000014005F8E5: E8 06 B6 BB 00     call        0000000140C1AEF0
  000000014005F8EA: 48 83 C4 38        add         rsp,38h
  000000014005F8EE: C3                 ret

  Summary

     1E02000 .text
