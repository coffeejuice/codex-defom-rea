Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014081A6E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014081A6E5: 48 83 EC 38        sub         rsp,38h
  000000014081A6E9: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014081A6EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014081A6F3: FF 15 CF 98 5E 01  call        qword ptr [0000000141E03FC8h]
  000000014081A6F9: 0F B6 C0           movzx       eax,al
  000000014081A6FC: 85 C0              test        eax,eax
  000000014081A6FE: 74 04              je          000000014081A704
  000000014081A700: 32 C0              xor         al,al
  000000014081A702: EB 29              jmp         000000014081A72D
  000000014081A704: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014081A709: FF 15 49 9B 5E 01  call        qword ptr [0000000141E04258h]
  000000014081A70F: 0F B6 C0           movzx       eax,al
  000000014081A712: 85 C0              test        eax,eax
  000000014081A714: 75 04              jne         000000014081A71A
  000000014081A716: B0 01              mov         al,1
  000000014081A718: EB 13              jmp         000000014081A72D
  000000014081A71A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014081A71F: FF 15 3B 9A 5E 01  call        qword ptr [0000000141E04160h]
  000000014081A725: 88 44 24 20        mov         byte ptr [rsp+20h],al
  000000014081A729: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  000000014081A72D: 48 83 C4 38        add         rsp,38h
  000000014081A731: C3                 ret

  Summary

     1E02000 .text
