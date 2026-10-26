Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539CB90: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006539CB95: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  000000006539CB9A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006539CB9F: 56                 push        rsi
  000000006539CBA0: 57                 push        rdi
  000000006539CBA1: 41 54              push        r12
  000000006539CBA3: 48 83 EC 30        sub         rsp,30h
  000000006539CBA7: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006539CBAB: 48 8B FA           mov         rdi,rdx
  000000006539CBAE: 41 0F B6 E9        movzx       ebp,r9b
  000000006539CBB2: 49 63 F0           movsxd      rsi,r8d
  000000006539CBB5: 4C 8B E1           mov         r12,rcx
  000000006539CBB8: 4C 8D 0D 99 03 36  lea         r9,[00000000656FCF58h]
                    00
  000000006539CBBF: 48 8D 15 82 03 36  lea         rdx,[00000000656FCF48h]
                    00
  000000006539CBC6: 4C 8B C3           mov         r8,rbx
  000000006539CBC9: 48 8B CF           mov         rcx,rdi
  000000006539CBCC: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006539CBD4: FF 15 3E 9B 2F 00  call        qword ptr [0000000065696718h]
  000000006539CBDA: 4C 8D 0D 17 03 36  lea         r9,[00000000656FCEF8h]
                    00
  000000006539CBE1: 48 8D 15 20 9E 30  lea         rdx,[00000000656A6A08h]
                    00
  000000006539CBE8: 4C 8B C3           mov         r8,rbx
  000000006539CBEB: 48 8B CF           mov         rcx,rdi
  000000006539CBEE: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006539CBF6: FF 15 1C 9B 2F 00  call        qword ptr [0000000065696718h]
  000000006539CBFC: 49 8D 8C F4 E0 01  lea         rcx,[r12+rsi*8+00000000000001E0h]
                    00 00
  000000006539CC04: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006539CC09: E8 72 74 E0 FF     call        00000000651A4080
  000000006539CC0E: 40 84 ED           test        bpl,bpl
  000000006539CC11: 74 08              je          000000006539CC1B
  000000006539CC13: 49 8B CC           mov         rcx,r12
  000000006539CC16: E8 65 FA FF FF     call        000000006539C680
  000000006539CC1B: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006539CC20: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  000000006539CC25: 48 83 C4 30        add         rsp,30h
  000000006539CC29: 41 5C              pop         r12
  000000006539CC2B: 5F                 pop         rdi
  000000006539CC2C: 5E                 pop         rsi
  000000006539CC2D: C3                 ret

  Summary

      695000 .text
