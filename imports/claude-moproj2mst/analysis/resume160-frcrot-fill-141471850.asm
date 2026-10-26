Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141471850: 48 8B C1           mov         rax,rcx
  0000000141471853: 49 83 F8 08        cmp         r8,8
  0000000141471857: 72 53              jb          00000001414718AC
  0000000141471859: 0F B6 D2           movzx       edx,dl
  000000014147185C: 49 B9 01 01 01 01  mov         r9,101010101010101h
                    01 01 01 01
  0000000141471866: 49 0F AF D1        imul        rdx,r9
  000000014147186A: 49 83 F8 40        cmp         r8,40h
  000000014147186E: 72 1E              jb          000000014147188E
  0000000141471870: 48 F7 D9           neg         rcx
  0000000141471873: 83 E1 07           and         ecx,7
  0000000141471876: 74 06              je          000000014147187E
  0000000141471878: 4C 2B C1           sub         r8,rcx
  000000014147187B: 48 89 10           mov         qword ptr [rax],rdx
  000000014147187E: 48 03 C8           add         rcx,rax
  0000000141471881: 4D 8B C8           mov         r9,r8
  0000000141471884: 49 83 E0 3F        and         r8,3Fh
  0000000141471888: 49 C1 E9 06        shr         r9,6
  000000014147188C: 75 39              jne         00000001414718C7
  000000014147188E: 4D 8B C8           mov         r9,r8
  0000000141471891: 49 83 E0 07        and         r8,7
  0000000141471895: 49 C1 E9 03        shr         r9,3
  0000000141471899: 74 11              je          00000001414718AC
  000000014147189B: 66 66 66 90        nop
  000000014147189F: 90                 nop
  00000001414718A0: 48 89 11           mov         qword ptr [rcx],rdx
  00000001414718A3: 48 83 C1 08        add         rcx,8
  00000001414718A7: 49 FF C9           dec         r9
  00000001414718AA: 75 F4              jne         00000001414718A0
  00000001414718AC: 4D 85 C0           test        r8,r8
  00000001414718AF: 74 0A              je          00000001414718BB
  00000001414718B1: 88 11              mov         byte ptr [rcx],dl
  00000001414718B3: 48 FF C1           inc         rcx
  00000001414718B6: 49 FF C8           dec         r8
  00000001414718B9: 75 F6              jne         00000001414718B1
  00000001414718BB: C3                 ret
  00000001414718BC: 0F 1F 40 00        nop         dword ptr [rax]
  00000001414718C0: 66 66 66 90        nop
  00000001414718C4: 66 66 90           nop
  00000001414718C7: 49 81 F9 00 1C 00  cmp         r9,1C00h
                    00
  00000001414718CE: 73 30              jae         0000000141471900
  00000001414718D0: 48 89 11           mov         qword ptr [rcx],rdx
  00000001414718D3: 48 89 51 08        mov         qword ptr [rcx+8],rdx
  00000001414718D7: 48 89 51 10        mov         qword ptr [rcx+10h],rdx
  00000001414718DB: 48 83 C1 40        add         rcx,40h
  00000001414718DF: 48 89 51 D8        mov         qword ptr [rcx-28h],rdx
  00000001414718E3: 48 89 51 E0        mov         qword ptr [rcx-20h],rdx
  00000001414718E7: 49 FF C9           dec         r9
  00000001414718EA: 48 89 51 E8        mov         qword ptr [rcx-18h],rdx
  00000001414718EE: 48
  00000001414718EF: 89

  Summary

     1E02000 .text
