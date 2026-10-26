Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CFAE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CFAE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CFAEA: 48 83 EC 78        sub         rsp,78h
  00000001402CFAEE: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001402CFAF7: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001402CFAFC: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001402CFB04: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001402CFB09: FF 15 71 49 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFB0F: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001402CFB14: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001402CFB19: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001402CFB1E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CFB26: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFB2D: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001402CFB32: E8 E9 0D DA FF     call        0000000140070920
  00000001402CFB37: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001402CFB3B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001402CFB40: FF 15 9A 5E B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFB46: 44 0F B6 5C 24 30  movzx       r11d,byte ptr [rsp+30h]
  00000001402CFB4C: 45 85 DB           test        r11d,r11d
  00000001402CFB4F: 74 74              je          00000001402CFBC5
  00000001402CFB51: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001402CFB59: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001402CFB5E: FF 15 1C 49 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CFB64: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001402CFB69: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001402CFB6E: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001402CFB73: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CFB7B: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CFB82: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  00000001402CFB87: E8 64 08 00 00     call        00000001402D03F0
  00000001402CFB8C: 48 8B D0           mov         rdx,rax
  00000001402CFB8F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFB94: E8 D7 F4 01 00     call        00000001402EF070
  00000001402CFB99: 90                 nop
  00000001402CFB9A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001402CFB9F: FF 15 3B 5E B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CFBA5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFBAA: E8 61 78 03 00     call        0000000140307410
  00000001402CFBAF: 0F B6 C0           movzx       eax,al
  00000001402CFBB2: 85 C0              test        eax,eax
  00000001402CFBB4: 75 05              jne         00000001402CFBBB
  00000001402CFBB6: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000001402CFBBB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001402CFBC0: E8 DB 85 F3 FF     call        00000001402081A0
  00000001402CFBC5: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  00000001402CFBC9: 48 83 C4 78        add         rsp,78h
  00000001402CFBCD: C3                 ret

  Summary

     1E02000 .text
