Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650B3300: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650B3305: 57                 push        rdi
  00000000650B3306: 48 83 EC 30        sub         rsp,30h
  00000000650B330A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650B3313: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000650B3318: 48 8B D9           mov         rbx,rcx
  00000000650B331B: 48 8D 05 36 F4 5F  lea         rax,[00000000656B2758h]
                    00
  00000000650B3322: 48 89 01           mov         qword ptr [rcx],rax
  00000000650B3325: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000000650B3329: 48 85 C9           test        rcx,rcx
  00000000650B332C: 74 24              je          00000000650B3352
  00000000650B332E: FF 15 44 36 5E 00  call        qword ptr [0000000065696978h]
  00000000650B3334: 84 C0              test        al,al
  00000000650B3336: 75 1A              jne         00000000650B3352
  00000000650B3338: 48 8B 7B 10        mov         rdi,qword ptr [rbx+10h]
  00000000650B333C: 48 85 FF           test        rdi,rdi
  00000000650B333F: 74 11              je          00000000650B3352
  00000000650B3341: 48 8B CF           mov         rcx,rdi
  00000000650B3344: E8 F7 F4 FF FF     call        00000000650B2840
  00000000650B3349: 48 8B CF           mov         rcx,rdi
  00000000650B334C: E8 5D DD 52 00     call        00000000655E10AE
  00000000650B3351: 90                 nop
  00000000650B3352: 48 8B CB           mov         rcx,rbx
  00000000650B3355: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000650B335A: 48 83 C4 30        add         rsp,30h
  00000000650B335E: 5F                 pop         rdi
  00000000650B335F: E9 2C 4E 08 00     jmp         0000000065138190

  Summary

      695000 .text
