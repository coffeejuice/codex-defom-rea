Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400980D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400980D5: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  00000001400980DC: 48 C7 44 24 70 FE  mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400980E5: C7 44 24 24 00 40  mov         dword ptr [rsp+24h],4000h
                    00 00
  00000001400980ED: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000001400980F2: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400980FA: E8 91 BF 00 00     call        00000001400A4090
  00000001400980FF: 0F B6 C0           movzx       eax,al
  0000000140098102: 85 C0              test        eax,eax
  0000000140098104: 0F 84 8E 01 00 00  je          0000000140098298
  000000014009810A: FF 15 40 C3 D6 01  call        qword ptr [0000000141E04450h]
  0000000140098110: 4C 8D 5C 24 58     lea         r11,[rsp+58h]
  0000000140098115: 4C 89 5C 24 60     mov         qword ptr [rsp+60h],r11
  000000014009811A: 33 D2              xor         edx,edx
  000000014009811C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140098121: E8 3A C9 F6 FF     call        0000000140004A60
  0000000140098126: 48 8B C8           mov         rcx,rax
  0000000140098129: FF 15 19 C3 D6 01  call        qword ptr [0000000141E04448h]
  000000014009812F: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140098137: E8 24 F2 00 00     call        00000001400A7360
  000000014009813C: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140098140: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140098148: EB 0B              jmp         0000000140098155
  000000014009814A: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014009814E: 83 C0 01           add         eax,1
  0000000140098151: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140098155: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140098159: 39 44 24 38        cmp         dword ptr [rsp+38h],eax
  000000014009815D: 7D 41              jge         00000001400981A0
  000000014009815F: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140098163: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014009816B: E8 80 F3 00 00     call        00000001400A74F0
  0000000140098170: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140098175: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  000000014009817B: 74 21              je          000000014009819E
  000000014009817D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140098182: E8 29 0E 02 00     call        00000001400B8FB0
  0000000140098187: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  000000014009818B: 81 7C 24 24 00 00  cmp         dword ptr [rsp+24h],10000h
                    01 00
  0000000140098193: 74 09              je          000000014009819E
  0000000140098195: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  0000000140098199: E9 06 01 00 00     jmp         00000001400982A4
  000000014009819E: EB AA              jmp         000000014009814A
  00000001400981A0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400981A8: E8 03 D7 00 00     call        00000001400A58B0
  00000001400981AD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400981B2: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400981B7: E8 84 F8 FA FF     call        0000000140047A40
  00000001400981BC: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  00000001400981C0: 48 8D 54 24 3C     lea         rdx,[rsp+3Ch]
  00000001400981C5: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400981CA: E8 F1 FB FA FF     call        0000000140047DC0
  00000001400981CF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400981D4: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400981DA: 74 0A              je          00000001400981E6
  00000001400981DC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400981E1: E8 DA AC FB FF     call        0000000140052EC0
  00000001400981E6: 33 D2              xor         edx,edx
  00000001400981E8: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400981F0: E8 CB 01 00 00     call        00000001400983C0
  00000001400981F5: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400981F9: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140098201: E8 DA 00 00 00     call        00000001400982E0
  0000000140098206: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014009820E: E8 BD 9E 00 00     call        00000001400A20D0
  0000000140098213: 0F B6 C0           movzx       eax,al
  0000000140098216: 85 C0              test        eax,eax
  0000000140098218: 74 13              je          000000014009822D
  000000014009821A: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014009821F: 85 C0              test        eax,eax
  0000000140098221: 74 0A              je          000000014009822D
  0000000140098223: C7 44 24 78 01 00  mov         dword ptr [rsp+78h],1
                    00 00
  000000014009822B: EB 08              jmp         0000000140098235
  000000014009822D: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  0000000140098235: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014009823A: 88 44 24 20        mov         byte ptr [rsp+20h],al
  000000014009823E: E8 8D FC 22 00     call        00000001402C7ED0
  0000000140098243: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140098248: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014009824D: E8 5E 3A 01 00     call        00000001400ABCB0
  0000000140098252: 0F B6 C0           movzx       eax,al
  0000000140098255: 85 C0              test        eax,eax
  0000000140098257: 74 13              je          000000014009826C
  0000000140098259: 48 8D 15 70 7E 0A  lea         rdx,[00000001421400D0h]
                    02
  0000000140098260: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140098265: E8 56 3A 01 00     call        00000001400ABCC0
  000000014009826A: EB 2A              jmp         0000000140098296
  000000014009826C: 48 8D 15 75 7E 0A  lea         rdx,[00000001421400E8h]
                    02
  0000000140098273: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140098278: FF 15 02 C2 D6 01  call        qword ptr [0000000141E04480h]
  000000014009827E: 90                 nop
  000000014009827F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140098284: FF 15 D6 BE D6 01  call        qword ptr [0000000141E04160h]
  000000014009828A: 90                 nop
  000000014009828B: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140098290: FF 15 4A D7 D6 01  call        qword ptr [0000000141E059E0h]
  0000000140098296: EB 08              jmp         00000001400982A0
  0000000140098298: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],10000h
                    01 00
  00000001400982A0: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001400982A4: 48 81 C4 88 00 00  add         rsp,88h
                    00
  00000001400982AB: C3                 ret

  Summary

     1E02000 .text
