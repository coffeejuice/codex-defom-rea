Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403B6480: 15 5B F5 A4 01     adc         eax,1A4F55Bh
  00000001403B6485: 90                 nop
  00000001403B6486: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001403B648B: FF 15 DF DB A4 01  call        qword ptr [0000000141E04070h]
  00000001403B6491: 90                 nop
  00000001403B6492: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001403B6497: FF 15 43 F5 A4 01  call        qword ptr [0000000141E059E0h]
  00000001403B649D: E9 5E 10 00 00     jmp         00000001403B7500
  00000001403B64A2: 48 8B 8C 24 E0 08  mov         rcx,qword ptr [rsp+00000000000008E0h]
                    00 00
  00000001403B64AA: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001403B64AE: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001403B64B2: FF 15 08 50 A5 01  call        qword ptr [0000000141E0B4C0h]
  00000001403B64B8: 89 44 24 64        mov         dword ptr [rsp+64h],eax
  00000001403B64BC: E8 5F F0 FF FF     call        00000001403B5520
  00000001403B64C1: 3B 44 24 64        cmp         eax,dword ptr [rsp+64h]
  00000001403B64C5: 74 09              je          00000001403B64D0
  00000001403B64C7: 8B 4C 24 64        mov         ecx,dword ptr [rsp+64h]
  00000001403B64CB: E8 30 F0 FF FF     call        00000001403B5500
  00000001403B64D0: 48 8B 8C 24 E0 08  mov         rcx,qword ptr [rsp+00000000000008E0h]
                    00 00
  00000001403B64D8: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001403B64DC: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001403B64E0: FF 15 DA 4F A5 01  call        qword ptr [0000000141E0B4C0h]
  00000001403B64E6: 85 C0              test        eax,eax
  00000001403B64E8: 75 0C              jne         00000001403B64F6
  00000001403B64EA: B9 01 00 00 00     mov         ecx,1
  00000001403B64EF: E8 6C F0 FF FF     call        00000001403B5560
  00000001403B64F4: EB 0A              jmp         00000001403B6500
  00000001403B64F6: B9 02 00 00 00     mov         ecx,2
  00000001403B64FB: E8 60 F0 FF FF     call        00000001403B5560
  00000001403B6500: 48 8B 8C 24 E0 08  mov         rcx,qword ptr [rsp+00000000000008E0h]
                    00 00
  00000001403B6508: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001403B650C: 48 8B 49 58        mov         rcx,qword ptr [rcx+58h]
  00000001403B6510: FF

  Summary

     1E02000 .text
