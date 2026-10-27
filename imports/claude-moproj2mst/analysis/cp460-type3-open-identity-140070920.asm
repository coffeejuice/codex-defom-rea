Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140070920: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140070925: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014007092A: 48 83 EC 38        sub         rsp,38h
  000000014007092E: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140070933: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140070938: E8 73 41 BF 00     call        0000000140C64AB0
  000000014007093D: 4C 8B D8           mov         r11,rax
  0000000140070940: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140070945: 4C 3B 18           cmp         r11,qword ptr [rax]
  0000000140070948: 74 0A              je          0000000140070954
  000000014007094A: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140070952: EB 08              jmp         000000014007095C
  0000000140070954: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014007095C: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140070960: 48 83 C4 38        add         rsp,38h
  0000000140070964: C3                 ret

  Summary

     1E02000 .text
