Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014014C220: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014014C225: 48 83 EC 38        sub         rsp,38h
  000000014014C229: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014014C232: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014C237: 48 8D 05 9A 12 08  lea         rax,[00000001421CD4D8h]
                    02
  000000014014C23E: 48 89 01           mov         qword ptr [rcx],rax
  000000014014C241: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014C246: 48 83 C1 10        add         rcx,10h
  000000014014C24A: E8 91 37 04 00     call        000000014018F9E0
  000000014014C24F: 90                 nop
  000000014014C250: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014C255: 48 83 C1 18        add         rcx,18h
  000000014014C259: F2 0F 10 15 5F 12  movsd       xmm2,mmword ptr [00000001421CD4C0h]
                    08 02
  000000014014C261: 66 0F 57 C9        xorpd       xmm1,xmm1
  000000014014C265: FF 15 6D 95 CB 01  call        qword ptr [0000000141E057D8h]
  000000014014C26B: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000014014C270: 41 C7 43 28 00 00  mov         dword ptr [r11+28h],0
                    00 00
  000000014014C278: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014C27D: 48 83 C1 30        add         rcx,30h
  000000014014C281: E8 6A 31 29 01     call        00000001413DF3F0
  000000014014C286: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000014014C28B: 41 C7 43 2C FF FF  mov         dword ptr [r11+2Ch],0FFFFFFFFh
                    FF FF
  000000014014C293: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014014C298: 48 C7 40 08 00 00  mov         qword ptr [rax+8],0
                    00 00
  000000014014C2A0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014014C2A5: 48 83 C4 38        add         rsp,38h
  000000014014C2A9: C3                 ret

  Summary

     1E02000 .text
