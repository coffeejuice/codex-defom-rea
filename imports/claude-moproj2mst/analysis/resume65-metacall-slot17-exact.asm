Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DB4F5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB4FA: E8 01 07 F5 FF     call        000000014002BC00
  00000001400DB4FF: E9 F0 13 00 00     jmp         00000001400DC8F4
  00000001400DB504: 48 8D 84 24 F8 00  lea         rax,[rsp+00000000000000F8h]
                    00 00
  00000001400DB50C: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400DB514: 48 8B 94 24 78 03  mov         rdx,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB51C: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  00000001400DB520: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400DB528: FF 15 AA A4 D2 01  call        qword ptr [0000000141E059D8h]
  00000001400DB52E: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  00000001400DB536: 48 8B 94 24 78 02  mov         rdx,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001400DB53E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB543: E8 E8 10 F6 FF     call        000000014003C630
  00000001400DB548: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400DB54D: 48 8B 84 24 78 03  mov         rax,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB555: 48 83 38 00        cmp         qword ptr [rax],0
  00000001400DB559: 74 13              je          00000001400DB56E
  00000001400DB55B: 48 8B 8C 24 78 03  mov         rcx,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB563: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001400DB566: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400DB56B: 48 89 01           mov         qword ptr [rcx],rax
  00000001400DB56E: E9 81 13 00 00     jmp         00000001400DC8F4
  00000001400DB573: 48

  Summary

     1E02000 .text
