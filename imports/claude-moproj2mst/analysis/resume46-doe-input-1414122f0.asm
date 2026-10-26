Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414122F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001414122F5: 48 83 EC 38        sub         rsp,38h
  00000001414122F9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000141412302: 33 D2              xor         edx,edx
  0000000141412304: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141412309: FF 15 B1 1E 9F 00  call        qword ptr [0000000141E041C0h]
  000000014141230F: 90                 nop
  0000000141412310: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141412315: 48 8D 05 CC CB A8  lea         rax,[0000000142E9EEE8h]
                    01
  000000014141231C: 48 89 01           mov         qword ptr [rcx],rax
  000000014141231F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141412324: 48 83 C1 10        add         rcx,10h
  0000000141412328: FF 15 92 21 9F 00  call        qword ptr [0000000141E044C0h]
  000000014141232E: 90                 nop
  000000014141232F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141412334: 48 83 C1 18        add         rcx,18h
  0000000141412338: FF 15 82 21 9F 00  call        qword ptr [0000000141E044C0h]
  000000014141233E: 90                 nop
  000000014141233F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141412344: E8 87 00 00 00     call        00000001414123D0
  0000000141412349: 90                 nop
  000000014141234A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014141234F: 48 83 C4 38        add         rsp,38h
  0000000141412353: C3                 ret

  Summary

     1E02000 .text
