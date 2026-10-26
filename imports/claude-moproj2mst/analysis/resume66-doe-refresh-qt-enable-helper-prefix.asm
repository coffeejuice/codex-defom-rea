Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065069F00: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000065069F04: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065069F09: 53                 push        rbx
  0000000065069F0A: 55                 push        rbp
  0000000065069F0B: 56                 push        rsi
  0000000065069F0C: 57                 push        rdi
  0000000065069F0D: 48 83 EC 58        sub         rsp,58h
  0000000065069F11: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065069F1A: 0F B6 FA           movzx       edi,dl
  0000000065069F1D: 48 8B E9           mov         rbp,rcx
  0000000065069F20: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  0000000065069F24: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065069F29: 84 D2              test        dl,dl
  0000000065069F2B: 74 33              je          0000000065069F60
  0000000065069F2D: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065069F31: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065069F35: A8 01              test        al,1
  0000000065069F37: 75 27              jne         0000000065069F60
  0000000065069F39: 48 8B CE           mov         rcx,rsi
  0000000065069F3C: FF 15 26 C9 62 00  call        qword ptr [0000000065696868h]
  0000000065069F42: 48 85 C0           test        rax,rax
  0000000065069F45: 74 19              je          0000000065069F60
  0000000065069F47: 48 8B CE           mov         rcx,rsi
  0000000065069F4A: FF 15 18 C9 62 00  call        qword ptr [0000000065696868h]
  0000000065069F50: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065069F54: 0F B6 41 08        movzx       eax,byte ptr [rcx+8]
  0000000065069F58: 24 01              and         al,1
  0000000065069F5A: 0F 85 B5 02 00 00  jne         000000006506A215
  0000000065069F60: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065069F64: 0F B6 48 08        movzx       ecx,byte ptr [rax+8]
  0000000065069F68: 80 E1 01           and         cl,1
  0000000065069F6B: 40 3A F9           cmp         dil,cl
  0000000065069F6E: 0F 85 A1 02 00 00  jne         000000006506A215
  0000000065069F74: 40 84 FF           test        dil,dil
  0000000065069F77: 0F 94 C0           sete        al
  0000000065069F7A: 88 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],al
                    00
  0000000065069F81: 44 0F B6 C0        movzx       r8d,al
  0000000065069F85: 33 D2              xor         edx,edx
  0000000065069F87: 48 8B CE           mov         rcx,rsi
  0000000065069F8A: E8 51 F0 FF FF     call        0000000065068FE0

  Summary

      695000 .text
