Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408F83A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408F83A5: 56                 push        rsi
  00000001408F83A6: 57                 push        rdi
  00000001408F83A7: 48 81 EC 38 08 00  sub         rsp,838h
                    00
  00000001408F83AE: 48 8B 05 7B 5C 26  mov         rax,qword ptr [0000000143B5E030h]
                    03
  00000001408F83B5: 48 33 C4           xor         rax,rsp
  00000001408F83B8: 48 89 84 24 28 08  mov         qword ptr [rsp+0000000000000828h],rax
                    00 00
  00000001408F83C0: 48 8D BC 24 18 08  lea         rdi,[rsp+0000000000000818h]
                    00 00
  00000001408F83C8: 48 8D 35 ED F5 EB  lea         rsi,[00000001427B79BCh]
                    01
  00000001408F83CF: B9 06 00 00 00     mov         ecx,6
  00000001408F83D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001408F83D6: C7 84 24 20 08 00  mov         dword ptr [rsp+0000000000000820h],0
                    00 00 00 00 00
  00000001408F83E1: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001408F83E6: 48 8D 15 D7 F5 EB  lea         rdx,[00000001427B79C4h]
                    01
  00000001408F83ED: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001408F83F5: E8 DE 78 B7 00     call        000000014146FCD8
  00000001408F83FA: 48 89 84 24 10 08  mov         qword ptr [rsp+0000000000000810h],rax
                    00 00
  00000001408F8402: 48 83 BC 24 10 08  cmp         qword ptr [rsp+0000000000000810h],0
                    00 00 00
  00000001408F840B: 75 07              jne         00000001408F8414
  00000001408F840D: 32 C0              xor         al,al
  00000001408F840F: E9 94 00 00 00     jmp         00000001408F84A8
  00000001408F8414: 4C 8B 84 24 10 08  mov         r8,qword ptr [rsp+0000000000000810h]
                    00 00
  00000001408F841C: BA D0 07 00 00     mov         edx,7D0h
  00000001408F8421: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001408F8426: E8 81 AF B7 00     call        00000001414733AC
  00000001408F842B: 4C 8B 84 24 10 08  mov         r8,qword ptr [rsp+0000000000000810h]
                    00 00
  00000001408F8433: BA D0 07 00 00     mov         edx,7D0h
  00000001408F8438: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001408F843D: E8 6A AF B7 00     call        00000001414733AC
  00000001408F8442: 48 8D 94 24 18 08  lea         rdx,[rsp+0000000000000818h]
                    00 00
  00000001408F844A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001408F844F: E8 1C A4 B7 00     call        0000000141472870
  00000001408F8454: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001408F8459: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001408F845F: 74 27              je          00000001408F8488
  00000001408F8461: 8B 84 24 20 08 00  mov         eax,dword ptr [rsp+0000000000000820h]
                    00
  00000001408F8468: 83 C0 01           add         eax,1
  00000001408F846B: 89 84 24 20 08 00  mov         dword ptr [rsp+0000000000000820h],eax
                    00
  00000001408F8472: 48 8D 94 24 18 08  lea         rdx,[rsp+0000000000000818h]
                    00 00
  00000001408F847A: 33 C9              xor         ecx,ecx
  00000001408F847C: E8 EF A3 B7 00     call        0000000141472870
  00000001408F8481: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001408F8486: EB D1              jmp         00000001408F8459
  00000001408F8488: 83 BC 24 20 08 00  cmp         dword ptr [rsp+0000000000000820h],2
                    00 02
  00000001408F8490: 75 05              jne         00000001408F8497
  00000001408F8492: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000001408F8497: 48 8B 8C 24 10 08  mov         rcx,qword ptr [rsp+0000000000000810h]
                    00 00
  00000001408F849F: E8 84 76 B7 00     call        000000014146FB28
  00000001408F84A4: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  00000001408F84A8: 48 8B 8C 24 28 08  mov         rcx,qword ptr [rsp+0000000000000828h]
                    00 00
  00000001408F84B0: 48 33 CC           xor         rcx,rsp
  00000001408F84B3: E8 58 5A B7 00     call        000000014146DF10
  00000001408F84B8: 48 81 C4 38 08 00  add         rsp,838h
                    00
  00000001408F84BF: 5F                 pop         rdi
  00000001408F84C0: 5E                 pop         rsi
  00000001408F84C1: C3                 ret

  Summary

     1E02000 .text
