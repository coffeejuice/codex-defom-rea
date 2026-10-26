Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140645030: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140645035: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014064503A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014064503F: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  0000000140645046: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140645052: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014064505A: 48 83 C1 18        add         rcx,18h
  000000014064505E: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140645066: FF 15 64 F0 7B 01  call        qword ptr [0000000141E040D0h]
  000000014064506C: 0F B6 C0           movzx       eax,al
  000000014064506F: 85 C0              test        eax,eax
  0000000140645071: 74 1A              je          000000014064508D
  0000000140645073: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014064507B: 48 83 C1 18        add         rcx,18h
  000000014064507F: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140645087: FF 15 E3 F3 7B 01  call        qword ptr [0000000141E04470h]
  000000014064508D: 0F B6 94 24 E0 00  movzx       edx,byte ptr [rsp+00000000000000E0h]
                    00 00
  0000000140645095: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014064509D: E8 8E 57 00 00     call        000000014064A830
  00000001406450A2: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001406450AA: 0F B6 40 2A        movzx       eax,byte ptr [rax+2Ah]
  00000001406450AE: 85 C0              test        eax,eax
  00000001406450B0: 74 5E              je          0000000140645110
  00000001406450B2: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000001406450B7: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001406450BC: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001406450C4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001406450C9: FF 15 09 09 7C 01  call        qword ptr [0000000141E059D8h]
  00000001406450CF: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001406450D7: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001406450DF: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001406450E7: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001406450EF: E8 2C 02 00 00     call        0000000140645320
  00000001406450F4: 48 8B C8           mov         rcx,rax
  00000001406450F7: E8 E4 62 01 00     call        000000014065B3E0
  00000001406450FC: 90                 nop
  00000001406450FD: 45 33 C0           xor         r8d,r8d
  0000000140645100: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140645108: 48 8B C8           mov         rcx,rax
  000000014064510B: E8 A0 A4 00 00     call        000000014064F5B0
  0000000140645110: C7 44 24 5C 00 00  mov         dword ptr [rsp+5Ch],0
                    00 00
  0000000140645118: EB 0B              jmp         0000000140645125
  000000014064511A: 8B 44 24 5C        mov         eax,dword ptr [rsp+5Ch]
  000000014064511E: 83 C0 01           add         eax,1
  0000000140645121: 89 44 24 5C        mov         dword ptr [rsp+5Ch],eax
  0000000140645125: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014064512D: E8 5E 23 00 00     call        0000000140647490
  0000000140645132: 39 44 24 5C        cmp         dword ptr [rsp+5Ch],eax
  0000000140645136: 0F 8D 83 00 00 00  jge         00000001406451BF
  000000014064513C: 8B 54 24 5C        mov         edx,dword ptr [rsp+5Ch]
  0000000140645140: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140645148: E8 D3 17 00 00     call        0000000140646920
  000000014064514D: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140645152: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140645157: E8 A4 37 01 00     call        0000000140658900
  000000014064515C: 48 85 C0           test        rax,rax
  000000014064515F: 74 59              je          00000001406451BA
  0000000140645161: 48 8D 44 24 78     lea         rax,[rsp+78h]
  0000000140645166: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014064516E: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140645176: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014064517E: FF 15 54 08 7C 01  call        qword ptr [0000000141E059D8h]
  0000000140645184: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000014064518C: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140645194: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014064519C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001406451A1: E8 5A 37 01 00     call        0000000140658900
  00000001406451A6: 90                 nop
  00000001406451A7: 41 B0 01           mov         r8b,1
  00000001406451AA: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001406451B2: 48 8B C8           mov         rcx,rax
  00000001406451B5: E8 F6 A3 00 00     call        000000014064F5B0
  00000001406451BA: E9 5B FF FF FF     jmp         000000014064511A
  00000001406451BF: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001406451C4: FF 15 F6 F2 7B 01  call        qword ptr [0000000141E044C0h]
  00000001406451CA: 90                 nop
  00000001406451CB: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001406451D3: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001406451D8: FF 15 FA ED 7B 01  call        qword ptr [0000000141E03FD8h]
  00000001406451DE: 90                 nop
  00000001406451DF: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000001406451E7: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001406451EF: BA 02 00 00 00     mov         edx,2
  00000001406451F4: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001406451FC: E8 5F F8 9B FF     call        0000000140004A60
  0000000140645201: 48 8B D0           mov         rdx,rax
  0000000140645204: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140645209: FF 15 D1 ED 7B 01  call        qword ptr [0000000141E03FE0h]
  000000014064520F: 0F B6 C0           movzx       eax,al
  0000000140645212: 85 C0              test        eax,eax
  0000000140645214: 75 1C              jne         0000000140645232
  0000000140645216: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014064521B: FF 15 FF ED 7B 01  call        qword ptr [0000000141E04020h]
  0000000140645221: 90                 nop
  0000000140645222: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140645227: FF 15 B3 07 7C 01  call        qword ptr [0000000141E059E0h]
  000000014064522D: E9 D4 00 00 00     jmp         0000000140645306
  0000000140645232: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140645237: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014064523C: FF 15 96 EB 7B 01  call        qword ptr [0000000141E03DD8h]
  0000000140645242: 90                 nop
  0000000140645243: 48 8D 0D 7A 9F E9  lea         rcx,[00000001424DF1C4h]
                    01
  000000014064524A: FF 15 E8 F0 7B 01  call        qword ptr [0000000141E04338h]
  0000000140645250: 48 8B D0           mov         rdx,rax
  0000000140645253: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140645258: FF 15 4A F5 7B 01  call        qword ptr [0000000141E047A8h]
  000000014064525E: 41 B8 01 00 00 00  mov         r8d,1
  0000000140645264: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140645269: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140645271: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  0000000140645275: FF 15 95 73 7C 01  call        qword ptr [0000000141E0C610h]
  000000014064527B: 90                 nop
  000000014064527C: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140645281: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140645286: FF 15 64 EB 7B 01  call        qword ptr [0000000141E03DF0h]
  000000014064528C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140645291: FF 15 81 ED 7B 01  call        qword ptr [0000000141E04018h]
  0000000140645297: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014064529C: FF 15 0E F5 7B 01  call        qword ptr [0000000141E047B0h]
  00000001406452A2: 85 C0              test        eax,eax
  00000001406452A4: 74 31              je          00000001406452D7
  00000001406452A6: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001406452AB: FF 15 2F 07 7C 01  call        qword ptr [0000000141E059E0h]
  00000001406452B1: 90                 nop
  00000001406452B2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001406452B7: FF 15 3B EB 7B 01  call        qword ptr [0000000141E03DF8h]
  00000001406452BD: 90                 nop
  00000001406452BE: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001406452C3: FF 15 57 ED 7B 01  call        qword ptr [0000000141E04020h]
  00000001406452C9: 90                 nop
  00000001406452CA: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001406452CF: FF 15 0B 07 7C 01  call        qword ptr [0000000141E059E0h]
  00000001406452D5: EB 2F              jmp         0000000140645306
  00000001406452D7: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001406452DC: FF 15 FE 06 7C 01  call        qword ptr [0000000141E059E0h]
  00000001406452E2: 90                 nop
  00000001406452E3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001406452E8: FF 15 0A EB 7B 01  call        qword ptr [0000000141E03DF8h]
  00000001406452EE: 90                 nop
  00000001406452EF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001406452F4: FF 15 26 ED 7B 01  call        qword ptr [0000000141E04020h]
  00000001406452FA: 90                 nop
  00000001406452FB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140645300: FF 15 DA 06 7C 01  call        qword ptr [0000000141E059E0h]
  0000000140645306: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  000000014064530D: C3                 ret

  Summary

     1E02000 .text
