Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100BD20: 40 56              push        rsi
  000000006100BD22: 57                 push        rdi
  000000006100BD23: 41 54              push        r12
  000000006100BD25: 48 83 EC 30        sub         rsp,30h
  000000006100BD29: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006100BD32: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  000000006100BD37: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  000000006100BD3C: 49 8B E8           mov         rbp,r8
  000000006100BD3F: 4C 8B E2           mov         r12,rdx
  000000006100BD42: 48 8B F1           mov         rsi,rcx
  000000006100BD45: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  000000006100BD49: 48 83 C1 08        add         rcx,8
  000000006100BD4D: E8 DE B9 FF FF     call        0000000061007730
  000000006100BD52: 48 8B C8           mov         rcx,rax
  000000006100BD55: 48 85 C0           test        rax,rax
  000000006100BD58: 0F 85 8C 00 00 00  jne         000000006100BDEA
  000000006100BD5E: 8D 48 70           lea         ecx,[rax+70h]
  000000006100BD61: E8 D4 AA 02 00     call        000000006103683A
  000000006100BD66: 48 8B F8           mov         rdi,rax
  000000006100BD69: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006100BD6E: 48 85 C0           test        rax,rax
  000000006100BD71: 74 51              je          000000006100BDC4
  000000006100BD73: 48 8B DE           mov         rbx,rsi
  000000006100BD76: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006100BD79: 48 8B CB           mov         rcx,rbx
  000000006100BD7C: FF 50 68           call        qword ptr [rax+68h]
  000000006100BD7F: 84 C0              test        al,al
  000000006100BD81: 75 15              jne         000000006100BD98
  000000006100BD83: F6 43 58 02        test        byte ptr [rbx+58h],2
  000000006100BD87: 74 0B              je          000000006100BD94
  000000006100BD89: 48 8B 5B 20        mov         rbx,qword ptr [rbx+20h]
  000000006100BD8D: 48 85 DB           test        rbx,rbx
  000000006100BD90: 75 E4              jne         000000006100BD76
  000000006100BD92: EB 04              jmp         000000006100BD98
  000000006100BD94: 48 8B 5B 20        mov         rbx,qword ptr [rbx+20h]
  000000006100BD98: 4C 8B C6           mov         r8,rsi
  000000006100BD9B: 48 8B D3           mov         rdx,rbx
  000000006100BD9E: 48 8B CF           mov         rcx,rdi
  000000006100BDA1: E8 1A 66 FF FF     call        00000000610023C0
  000000006100BDA6: 90                 nop
  000000006100BDA7: 48 8D 05 6A 89 03  lea         rax,[0000000061044718h]
                    00
  000000006100BDAE: 48 89 07           mov         qword ptr [rdi],rax
  000000006100BDB1: 48 8D 4F 38        lea         rcx,[rdi+38h]
  000000006100BDB5: 49 8B D4           mov         rdx,r12
  000000006100BDB8: FF 15 F2 7A 03 00  call        qword ptr [00000000610438B0h]
  000000006100BDBE: C6 47 68 00        mov         byte ptr [rdi+68h],0
  000000006100BDC2: EB 02              jmp         000000006100BDC6
  000000006100BDC4: 33 FF              xor         edi,edi
  000000006100BDC6: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006100BDC9: 48 8B D5           mov         rdx,rbp
  000000006100BDCC: 48 8B CF           mov         rcx,rdi
  000000006100BDCF: FF 50 08           call        qword ptr [rax+8]
  000000006100BDD2: 48 8D 4F 08        lea         rcx,[rdi+8]
  000000006100BDD6: FF 15 4C 7A 03 00  call        qword ptr [0000000061043828h]
  000000006100BDDC: 48 8B D7           mov         rdx,rdi
  000000006100BDDF: 48 8B 4E 68        mov         rcx,qword ptr [rsi+68h]
  000000006100BDE3: E8 18 F4 FF FF     call        000000006100B200
  000000006100BDE8: EB 09              jmp         000000006100BDF3
  000000006100BDEA: 48 8B 00           mov         rax,qword ptr [rax]
  000000006100BDED: 48 8B D5           mov         rdx,rbp
  000000006100BDF0: FF 50 08           call        qword ptr [rax+8]
  000000006100BDF3: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000006100BDF8: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  000000006100BDFD: 48 83 C4 30        add         rsp,30h
  000000006100BE01: 41 5C              pop         r12
  000000006100BE03: 5F                 pop         rdi
  000000006100BE04: 5E                 pop         rsi
  000000006100BE05: C3                 ret

  Summary

       42000 .text
