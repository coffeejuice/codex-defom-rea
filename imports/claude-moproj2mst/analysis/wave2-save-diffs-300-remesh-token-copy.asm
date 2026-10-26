Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FEC0: 4C 8B D9           mov         r11,rcx
  000000014146FEC3: 48 2B CA           sub         rcx,rdx
  000000014146FEC6: F6 C2 07           test        dl,7
  000000014146FEC9: 74 1F              je          000000014146FEEA
  000000014146FECB: 8A 02              mov         al,byte ptr [rdx]
  000000014146FECD: 88 04 11           mov         byte ptr [rcx+rdx],al
  000000014146FED0: 84 C0              test        al,al
  000000014146FED2: 74 0A              je          000000014146FEDE
  000000014146FED4: 48 FF C2           inc         rdx
  000000014146FED7: F6 C2 07           test        dl,7
  000000014146FEDA: 75 EF              jne         000000014146FECB
  000000014146FEDC: EB 0C              jmp         000000014146FEEA
  000000014146FEDE: 49 8B C3           mov         rax,r11
  000000014146FEE1: C3                 ret
  000000014146FEE2: 48 89 04 11        mov         qword ptr [rcx+rdx],rax
  000000014146FEE6: 48 83 C2 08        add         rdx,8
  000000014146FEEA: 48 8B 02           mov         rax,qword ptr [rdx]
  000000014146FEED: 49 B9 FF FE FE FE  mov         r9,7EFEFEFEFEFEFEFFh
                    FE FE FE 7E
  000000014146FEF7: 4C 03 C8           add         r9,rax
  000000014146FEFA: 4C 8B D0           mov         r10,rax
  000000014146FEFD: 49 83 F2 FF        xor         r10,0FFFFFFFFFFFFFFFFh
  000000014146FF01: 4D 33 D1           xor         r10,r9
  000000014146FF04: 49 B9 00 01 01 01  mov         r9,8101010101010100h
                    01 01 01 81
  000000014146FF0E: 4D 85 D1           test        r9,r10
  000000014146FF11: 74 CF              je          000000014146FEE2
  000000014146FF13: 84 C0              test        al,al
  000000014146FF15: 88 04 11           mov         byte ptr [rcx+rdx],al
  000000014146FF18: 74 59              je          000000014146FF73
  000000014146FF1A: 48 FF C2           inc         rdx
  000000014146FF1D: 84 E4              test        ah,ah
  000000014146FF1F: 88 24 11           mov         byte ptr [rcx+rdx],ah
  000000014146FF22: 74 4F              je          000000014146FF73
  000000014146FF24: 48 FF C2           inc         rdx
  000000014146FF27: 48 C1 E8 10        shr         rax,10h
  000000014146FF2B: 84 C0              test        al,al
  000000014146FF2D: 88 04 11           mov         byte ptr [rcx+rdx],al
  000000014146FF30: 74

  Summary

     1E02000 .text
