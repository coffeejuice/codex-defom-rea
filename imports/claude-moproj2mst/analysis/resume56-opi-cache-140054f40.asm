Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054F40: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140054F44: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054F49: 48 83 EC 48        sub         rsp,48h
  0000000140054F4D: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140054F56: 0F B6 44 24 58     movzx       eax,byte ptr [rsp+58h]
  0000000140054F5B: 85 C0              test        eax,eax
  0000000140054F5D: 74 67              je          0000000140054FC6
  0000000140054F5F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054F64: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054F6B: E8 80 5F BC 00     call        0000000140C1AEF0
  0000000140054F70: 48 85 C0           test        rax,rax
  0000000140054F73: 75 51              jne         0000000140054FC6
  0000000140054F75: B9 50 00 00 00     mov         ecx,50h
  0000000140054F7A: E8 3D 9B 41 01     call        000000014146EABC
  0000000140054F7F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140054F84: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140054F8A: 74 11              je          0000000140054F9D
  0000000140054F8C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140054F91: E8 FA F8 06 00     call        00000001400C4890
  0000000140054F96: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140054F9B: EB 09              jmp         0000000140054FA6
  0000000140054F9D: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140054FA6: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140054FAB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140054FB0: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054FB5: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054FBC: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140054FC1: E8 5A 79 C6 00     call        0000000140CBC920
  0000000140054FC6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054FCB: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054FD2: E8 19 5F BC 00     call        0000000140C1AEF0
  0000000140054FD7: 48 83 C4 48        add         rsp,48h
  0000000140054FDB: C3                 ret

  Summary

     1E02000 .text
