Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001405EF3A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001405EF3A5: 48 83 EC 38        sub         rsp,38h
  00000001405EF3A9: B2 01              mov         dl,1
  00000001405EF3AB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405EF3B0: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001405EF3B4: E8 97 54 AD FF     call        00000001400C4850
  00000001405EF3B9: 48 8D 0D F8 72 78  lea         rcx,[0000000143D766B8h]
                    03
  00000001405EF3C0: E8 CB 8C CD FF     call        00000001402C8090
  00000001405EF3C5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405EF3CA: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001405EF3D0: 74 49              je          00000001405EF41B
  00000001405EF3D2: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001405EF3D7: E8 04 87 00 00     call        00000001405F7AE0
  00000001405EF3DC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405EF3E1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001405EF3E7: 74 32              je          00000001405EF41B
  00000001405EF3E9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001405EF3EE: E8 7D 4D A2 FF     call        0000000140014170
  00000001405EF3F3: 0F B6 C0           movzx       eax,al
  00000001405EF3F6: 85 C0              test        eax,eax
  00000001405EF3F8: 75 11              jne         00000001405EF40B
  00000001405EF3FA: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001405EF3FF: E8 BC 4D A2 FF     call        00000001400141C0
  00000001405EF404: 0F B6 C0           movzx       eax,al
  00000001405EF407: 85 C0              test        eax,eax
  00000001405EF409: 74 10              je          00000001405EF41B
  00000001405EF40B: 33 D2              xor         edx,edx
  00000001405EF40D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001405EF412: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001405EF416: E8 35 54 AD FF     call        00000001400C4850
  00000001405EF41B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001405EF420: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001405EF424: 48 83 C4 38        add         rsp,38h
  00000001405EF428: C3                 ret

  Summary

     1E02000 .text
