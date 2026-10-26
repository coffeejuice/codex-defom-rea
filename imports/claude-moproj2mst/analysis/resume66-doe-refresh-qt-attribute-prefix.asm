Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065068FE0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000065068FE4: 55                 push        rbp
  0000000065068FE5: 56                 push        rsi
  0000000065068FE6: 57                 push        rdi
  0000000065068FE7: 48 83 EC 40        sub         rsp,40h
  0000000065068FEB: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065068FF4: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065068FF9: 41 0F B6 E8        movzx       ebp,r8b
  0000000065068FFD: 44 8B D2           mov         r10d,edx
  0000000065069000: 48 8B F9           mov         rdi,rcx
  0000000065069003: 83 FA 20           cmp         edx,20h
  0000000065069006: 7D 11              jge         0000000065069019
  0000000065069008: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506900C: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000065069010: 41 0F A3 D1        bt          r9d,edx
  0000000065069014: 0F 92 C0           setb        al
  0000000065069017: EB 31              jmp         000000006506904A
  0000000065069019: 4C 8B 49 08        mov         r9,qword ptr [rcx+8]
  000000006506901D: 8D 42 E0           lea         eax,[rdx-20h]
  0000000065069020: 48 98              cdqe
  0000000065069022: 4C 8B C0           mov         r8,rax
  0000000065069025: 49 C1 E8 05        shr         r8,5
  0000000065069029: 42 8D 0C C5 00 00  lea         ecx,[r8*8+0000000000000000h]
                    00 00
  0000000065069031: C0 E1 02           shl         cl,2
  0000000065069034: 2A C1              sub         al,cl
  0000000065069036: 0F B6 D0           movzx       edx,al
  0000000065069039: 49 63 C0           movsxd      rax,r8d
  000000006506903C: 41 8B 8C 81 C8 01  mov         ecx,dword ptr [r9+rax*4+00000000000001C8h]
                    00 00
  0000000065069044: 0F A3 D1           bt          ecx,edx
  0000000065069047: 0F 92 C0           setb        al
  000000006506904A: 40 3A C5           cmp         al,bpl
  000000006506904D: 0F 84 9E 06 00 00  je          00000000650696F1

  Summary

      695000 .text
