Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538C150: 40 57              push        rdi
  000000006538C152: 48 83 EC 50        sub         rsp,50h
  000000006538C156: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006538C15F: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006538C164: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  000000006538C169: 8B FA              mov         edi,edx
  000000006538C16B: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006538C16F: 48 8B 86 E0 01 00  mov         rax,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538C176: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538C179: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538C180: FF 15 CA C6 30 00  call        qword ptr [0000000065698850h]
  000000006538C186: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538C18B: 44 8B 8E 18 02 00  mov         r9d,dword ptr [rsi+0000000000000218h]
                    00
  000000006538C192: 44 8B C7           mov         r8d,edi
  000000006538C195: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538C19A: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538C1A1: FF 53 58           call        qword ptr [rbx+58h]
  000000006538C1A4: 90                 nop
  000000006538C1A5: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538C1AA: 48 8B CE           mov         rcx,rsi
  000000006538C1AD: E8 3E EC FF FF     call        000000006538ADF0
  000000006538C1B2: 90                 nop
  000000006538C1B3: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  000000006538C1B8: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  000000006538C1BD: 48 83 C4 50        add         rsp,50h
  000000006538C1C1: 5F                 pop         rdi
  000000006538C1C2: C3                 ret

  Summary

      695000 .text
