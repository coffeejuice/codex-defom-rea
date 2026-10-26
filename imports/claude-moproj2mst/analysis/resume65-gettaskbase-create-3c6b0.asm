Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014003C6B0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014003C6B5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014003C6BA: 48 83 EC 58        sub         rsp,58h
  000000014003C6BE: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014003C6C7: 48 8D 44 24 28     lea         rax,[rsp+28h]
  000000014003C6CC: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014003C6D1: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014003C6D6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014003C6DB: FF 15 F7 92 DC 01  call        qword ptr [0000000141E059D8h]
  000000014003C6E1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014003C6E6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014003C6EB: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014003C6F0: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014003C6F5: E8 66 12 FE FF     call        000000014001D960
  000000014003C6FA: 90                 nop
  000000014003C6FB: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  000000014003C700: 48 8B C8           mov         rcx,rax
  000000014003C703: E8 88 B6 78 00     call        00000001407C7D90
  000000014003C708: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014003C70D: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014003C712: FF 15 C8 92 DC 01  call        qword ptr [0000000141E059E0h]
  000000014003C718: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014003C71D: 48 83 C4 58        add         rsp,58h
  000000014003C721: C3                 ret
  000000014003C722: CC                 int         3
  000000014003C723: CC                 int         3
  000000014003C724: CC                 int         3
  000000014003C725: CC                 int         3
  000000014003C726: CC                 int         3
  000000014003C727: CC                 int         3
  000000014003C728: CC                 int         3
  000000014003C729: CC                 int         3
  000000014003C72A: CC                 int         3
  000000014003C72B: CC                 int         3
  000000014003C72C: CC                 int         3
  000000014003C72D: CC                 int         3
  000000014003C72E: CC                 int         3
  000000014003C72F: CC                 int         3
  000000014003C730: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014003C735: 53                 push        rbx
  000000014003C736: 48 81 EC E0 01 00  sub         rsp,1E0h
                    00
  000000014003C73D: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014003C749: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003C751: FF 15 19 7A DC 01  call        qword ptr [0000000141E04170h]
  000000014003C757: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014003C75C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014003C764: 4C 8D 0D DD CB 52  lea         r9,[0000000143569348h]
                    03
  000000014003C76B: 4C 8D 05 D6 8F 52  lea         r8,[0000000143565748h]
                    03
  000000014003C772: 33 D2              xor         edx,edx
  000000014003C774: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014003C779: E8 7E 31 43 01     call        000000014146F8FC
  000000014003C77E: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014003C783: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014003C789: 75 05              jne         000000014003C790
  000000014003C78B: E9 13 05 00 00     jmp         000000014003CCA3
  000000014003C790: C6 44 24 50 01     mov         byte ptr [rsp+50h],1
  000000014003C795: 0F B6 54 24 50     movzx       edx,byte ptr [rsp+50h]
  000000014003C79A: 48 8D 0D 17 9F D3  lea         rcx,[0000000143D766B8h]
                    03
  000000014003C7A1: E8 7A E1 28 00     call        00000001402CA920
  000000014003C7A6: 0F B6 C0           movzx       eax,al
  000000014003C7A9: 85 C0              test        eax,eax
  000000014003C7AB: 74 05              je          000000014003C7B2
  000000014003C7AD: E9 F1 04 00 00     jmp         000000014003CCA3
  000000014003C7B2: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003C7BA: E8 E1 D5 FE FF     call        0000000140029DA0
  000000014003C7BF: 0F B6 C0           movzx       eax,al
  000000014003C7C2: 85 C0              test        eax,eax
  000000014003C7C4: 75 05              jne         000000014003C7CB
  000000014003C7C6: E9 D8 04 00 00     jmp         000000014003CCA3
  000000014003C7CB: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014003C7D0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014003C7D5: FF 15 FD EF DC 01  call        qword ptr [0000000141E0B7D8h]
  000000014003C7DB: 90                 nop
  000000014003C7DC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014003C7E1: FF 15 71 7A DC 01  call        qword ptr [0000000141E04258h]
  000000014003C7E7: 0F B6 C0           movzx       eax,al
  000000014003C7EA: 85 C0              test        eax,eax
  000000014003C7EC: 0F 85 2A 04 00 00  jne         000000014003CC1C
  000000014003C7F2: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014003C7F7: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014003C7FC: 41 B8 00 00 01 00  mov         r8d,10000h
  000000014003C802: BA 00 40 00 00     mov         edx,4000h
  000000014003C807: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014003C80C: E8 6F 4E FF FF     call        0000000140031680
  000000014003C811: 48 8B D8           mov         rbx,rax
  000000014003C814: 45 33 C0           xor         r8d,r8d
  000000014003C817: 48 8D 15 82 91 0C  lea         rdx,[00000001421059A0h]
                    02
  000000014003C81E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014003C823: E8 58 85 FE FF     call        0000000140024D80
  000000014003C828: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000014003C830: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014003C838: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  000000014003C840: 48 8D 15 DD 52 0C  lea         rdx,[0000000142101B24h]
                    02
  000000014003C847: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014003C84C: FF 15 2E 7C DC 01  call        qword ptr [0000000141E04480h]
  000000014003C852: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  000000014003C85A: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014003C862: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000014003C86A: 48 8D 15 E3 52 0C  lea         rdx,[0000000142101B54h]
                    02
  000000014003C871: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014003C876: FF 15 04 7C DC 01  call        qword ptr [0000000141E04480h]
  000000014003C87C: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  000000014003C884: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014003C88C: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  000000014003C894: 45 33 C0           xor         r8d,r8d
  000000014003C897: 48 8D 15 3A 91 0C  lea         rdx,[00000001421059D8h]
                    02
  000000014003C89E: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014003C8A6: E8 D5 84 FE FF     call        0000000140024D80
  000000014003C8AB: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014003C8B3: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014003C8BB: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  000000014003C8C3: 48 8D 15 EA 53 0C  lea         rdx,[0000000142101CB4h]
                    02
  000000014003C8CA: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014003C8D2: FF 15 A8 7B DC 01  call        qword ptr [0000000141E04480h]
  000000014003C8D8: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  000000014003C8E0: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  000000014003C8E8: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  000000014003C8F0: 48 8D 15 D5 55 0C  lea         rdx,[0000000142101ECCh]
                    02
  000000014003C8F7: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000014003C8FF: FF 15 7B 7B DC 01  call        qword ptr [0000000141E04480h]
  000000014003C905: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  000000014003C90D: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  000000014003C915: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  000000014003C91D: 45 33 C0           xor         r8d,r8d
  000000014003C920: 48 8D 15 D9 90 0C  lea         rdx,[0000000142105A00h]
                    02
  000000014003C927: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014003C92F: E8
  000000014003C930: 4C

  Summary

     1E02000 .text
