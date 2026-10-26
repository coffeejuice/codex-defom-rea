Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000655662D0: 40 57              push        rdi
  00000000655662D2: 48 83 EC 30        sub         rsp,30h
  00000000655662D6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000655662DF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000655662E4: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000655662E9: 48 8B F9           mov         rdi,rcx
  00000000655662EC: 48 8B 1D 45 08 13  mov         rbx,qword ptr [0000000065696B38h]
                    00
  00000000655662F3: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000655662F8: 48 8D 4B 10        lea         rcx,[rbx+10h]
  00000000655662FC: FF 15 BE 05 13 00  call        qword ptr [00000000656968C0h]
  0000000065566302: 90                 nop
  0000000065566303: 48 39 1F           cmp         qword ptr [rdi],rbx
  0000000065566306: 74 3D              je          0000000065566345
  0000000065566308: 48 8D 4B 10        lea         rcx,[rbx+10h]
  000000006556630C: FF 15 AE 05 13 00  call        qword ptr [00000000656968C0h]
  0000000065566312: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000065566315: 48 83 C1 10        add         rcx,10h
  0000000065566319: FF 15 59 06 13 00  call        qword ptr [0000000065696978h]
  000000006556631F: 84 C0              test        al,al
  0000000065566321: 75 10              jne         0000000065566333
  0000000065566323: 48 8D 15 86 8F B3  lea         rdx,[000000006509F2B0h]
                    FF
  000000006556632A: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006556632D: FF 15 7D 08 13 00  call        qword ptr [0000000065696BB0h]
  0000000065566333: 48 89 1F           mov         qword ptr [rdi],rbx
  0000000065566336: F6 43 24 01        test        byte ptr [rbx+24h],1
  000000006556633A: 75 09              jne         0000000065566345
  000000006556633C: 48 8B CF           mov         rcx,rdi
  000000006556633F: E8 4C 5A CF FF     call        000000006525BD90
  0000000065566344: 90                 nop
  0000000065566345: 48 8D 4B 10        lea         rcx,[rbx+10h]
  0000000065566349: FF 15 29 06 13 00  call        qword ptr [0000000065696978h]
  000000006556634F: 84 C0              test        al,al
  0000000065566351: 75 10              jne         0000000065566363
  0000000065566353: 48 8D 15 56 8F B3  lea         rdx,[000000006509F2B0h]
                    FF
  000000006556635A: 48 8B CB           mov         rcx,rbx
  000000006556635D: FF 15 4D 08 13 00  call        qword ptr [0000000065696BB0h]
  0000000065566363: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065566368: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  000000006556636D: 48 83 C4 30        add         rsp,30h
  0000000065566371: 5F                 pop         rdi
  0000000065566372: C3                 ret

  Summary

      695000 .text
