Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C540F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5417: E8 54 4A FA FF     call        0000000067069E70
  00000000670C541C: 84 C0              test        al,al
  00000000670C541E: 74 28              je          00000000670C5448
  00000000670C5420: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5428: E8 D3 4A FA FF     call        0000000067069F00
  00000000670C542D: 85 C0              test        eax,eax
  00000000670C542F: 78 17              js          00000000670C5448
  00000000670C5431: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5439: E8 C2 4A FA FF     call        0000000067069F00
  00000000670C543E: BB 08 00 00 00     mov         ebx,8
  00000000670C5443: 83 F8 07           cmp         eax,7
  00000000670C5446: 7E 05              jle         00000000670C544D
  00000000670C5448: BB 0A 00 00 00     mov         ebx,0Ah
  00000000670C544D: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5455: 48 8B CE           mov         rcx,rsi
  00000000670C5458: E8 93 CC FF FF     call        00000000670C20F0
  00000000670C545D: EB 76              jmp         00000000670C54D5
  00000000670C545F: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C5466: E8 D5 53 F8 FF     call        000000006704A840
  00000000670C546B: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C546E: 66 39 84 24 B0 00  cmp         word ptr [rsp+00000000000000B0h],ax
                    00 00
  00000000670C5476: 74 58              je          00000000670C54D0
  00000000670C5478: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5480: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C5487: E8 F4 53 F8 FF     call        000000006704A880
  00000000670C548C: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C548F: 66 39 84 24 B0 00  cmp         word ptr [rsp+00000000000000B0h],ax
                    00 00
  00000000670C5497: 74 37              je          00000000670C54D0
  00000000670C5499: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670C54A1: E8 CA 49 FA FF     call        0000000067069E70
  00000000670C54A6: 84 C0              test        al,al
  00000000670C54A8: 75 26              jne         00000000670C54D0
  00000000670C54AA: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C54B2: 48 8B CE           mov         rcx,rsi
  00000000670C54B5: E8 36 CC FF FF     call        00000000670C20F0
  00000000670C54BA: B8 02 00 00 00     mov         eax,2
  00000000670C54BF: 48 83 C4 58        add         rsp,58h
  00000000670C54C3: 41 5F              pop         r15
  00000000670C54C5: 41 5E              pop         r14
  00000000670C54C7: 41 5D              pop         r13
  00000000670C54C9: 41 5C              pop         r12
  00000000670C54CB: 5F                 pop         rdi
  00000000670C54CC: 5E                 pop         rsi
  00000000670C54CD: 5D                 pop         rbp
  00000000670C54CE: 5B                 pop         rbx
  00000000670C54CF: C3                 ret
  00000000670C54D0: BB 0A 00 00 00     mov         ebx,0Ah
  00000000670C54D5: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C54DD: 48 8B CE           mov         rcx,rsi
  00000000670C54E0: E8 0B CC FF FF     call        00000000670C20F0
  00000000670C54E5: 49

  Summary

      1A7000 .text
