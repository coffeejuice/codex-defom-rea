Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654A4D00: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000654A4D05: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000654A4D0A: 57                 push        rdi
  00000000654A4D0B: 48 83 EC 30        sub         rsp,30h
  00000000654A4D0F: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000654A4D13: 48 8B F1           mov         rsi,rcx
  00000000654A4D16: 48 8B CA           mov         rcx,rdx
  00000000654A4D19: 48 8B FA           mov         rdi,rdx
  00000000654A4D1C: E8 0F C8 04 00     call        00000000654F1530
  00000000654A4D21: 48 3B 83 80 02 00  cmp         rax,qword ptr [rbx+0000000000000280h]
                    00
  00000000654A4D28: 74 1D              je          00000000654A4D47
  00000000654A4D2A: 48 8D 0D 4F 62 28  lea         rcx,[000000006572AF80h]
                    00
  00000000654A4D31: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000654A4D36: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000654A4D3B: 48 83 C4 30        add         rsp,30h
  00000000654A4D3F: 5F                 pop         rdi
  00000000654A4D40: 48 FF 25 51 1C 1F  jmp         qword ptr [0000000065696998h]
                    00
  00000000654A4D47: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654A4D4E: 48 85 C9           test        rcx,rcx
  00000000654A4D51: 74 35              je          00000000654A4D88
  00000000654A4D53: 4C 8D 0D E6 61 28  lea         r9,[000000006572AF40h]
                    00
  00000000654A4D5A: 48 8D 15 D7 84 27  lea         rdx,[000000006571D238h]
                    00
  00000000654A4D61: 4C 8B C6           mov         r8,rsi
  00000000654A4D64: FF 15 B6 19 1F 00  call        qword ptr [0000000065696720h]
  00000000654A4D6A: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654A4D71: 4C 8D 0D 98 61 28  lea         r9,[000000006572AF10h]
                    00
  00000000654A4D78: 48 8D 15 61 4B 25  lea         rdx,[00000000656F98E0h]
                    00
  00000000654A4D7F: 4C 8B C6           mov         r8,rsi
  00000000654A4D82: FF 15 98 19 1F 00  call        qword ptr [0000000065696720h]
  00000000654A4D88: 48 39 BB A0 02 00  cmp         qword ptr [rbx+00000000000002A0h],rdi
                    00
  00000000654A4D8F: 74 10              je          00000000654A4DA1
  00000000654A4D91: 48 8D 8B A0 02 00  lea         rcx,[rbx+00000000000002A0h]
                    00
  00000000654A4D98: 48 8B D7           mov         rdx,rdi
  00000000654A4D9B: FF 15 47 1B 1F 00  call        qword ptr [00000000656968E8h]
  00000000654A4DA1: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654A4DA8: 48 85 C9           test        rcx,rcx
  00000000654A4DAB: 74 45              je          00000000654A4DF2
  00000000654A4DAD: 4C 8D 0D 8C 61 28  lea         r9,[000000006572AF40h]
                    00
  00000000654A4DB4: 48 8D 15 7D 84 27  lea         rdx,[000000006571D238h]
                    00
  00000000654A4DBB: 4C 8B C6           mov         r8,rsi
  00000000654A4DBE: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000654A4DC6: FF 15 4C 19 1F 00  call        qword ptr [0000000065696718h]
  00000000654A4DCC: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654A4DD3: 4C 8D 0D 36 61 28  lea         r9,[000000006572AF10h]
                    00
  00000000654A4DDA: 48 8D 15 FF 4A 25  lea         rdx,[00000000656F98E0h]
                    00
  00000000654A4DE1: 4C 8B C6           mov         r8,rsi
  00000000654A4DE4: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000654A4DEC: FF 15 26 19 1F 00  call        qword ptr [0000000065696718h]
  00000000654A4DF2: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000654A4DF7: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000654A4DFC: 48 83 C4 30        add         rsp,30h
  00000000654A4E00: 5F                 pop         rdi
  00000000654A4E01: C3                 ret

  Summary

      695000 .text
