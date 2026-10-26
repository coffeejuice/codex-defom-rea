Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C1FB: 4C 89 64 24 70     mov         qword ptr [rsp+70h],r12
  000000006704C200: 4F 8D 64 7D 00     lea         r12,[r13+r15*2]
  000000006704C205: 49 8B D4           mov         rdx,r12
  000000006704C208: 49 8B CE           mov         rcx,r14
  000000006704C20B: E8 00 10 FE FF     call        000000006702D210
  000000006704C210: 0F B6 F8           movzx       edi,al
  000000006704C213: 84 C0              test        al,al
  000000006704C215: 75 69              jne         000000006704C280
  000000006704C217: 41 0F B7 04 24     movzx       eax,word ptr [r12]
  000000006704C21C: 66 41 3B 46 0A     cmp         ax,word ptr [r14+0Ah]
  000000006704C221: 75 05              jne         000000006704C228
  000000006704C223: 40 B7 3B           mov         dil,3Bh
  000000006704C226: EB 58              jmp         000000006704C280
  000000006704C228: 66 41 3B 46 0C     cmp         ax,word ptr [r14+0Ch]
  000000006704C22D: 75 05              jne         000000006704C234
  000000006704C22F: 40 B7 25           mov         dil,25h
  000000006704C232: EB 4C              jmp         000000006704C280
  000000006704C234: 66 83 F8 41        cmp         ax,41h
  000000006704C238: 72 27              jb          000000006704C261
  000000006704C23A: 66 83 F8 5A        cmp         ax,5Ah
  000000006704C23E: 77 21              ja          000000006704C261
  000000006704C240: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000006704C245: 49 8B CC           mov         rcx,r12
  000000006704C248: E8 03 E4 01 00     call        000000006706A650
  000000006704C24D: B9 FF 00 00 00     mov         ecx,0FFh
  000000006704C252: 66 39 08           cmp         word ptr [rax],cx
  000000006704C255: 76 05              jbe         000000006704C25C
  000000006704C257: 40 32 FF           xor         dil,dil
  000000006704C25A: EB 24              jmp         000000006704C280
  000000006704C25C: 40 8A 38           mov         dil,byte ptr [rax]
  000000006704C25F: EB 1F              jmp         000000006704C280
  000000006704C261: 66 83 F8 61        cmp         ax,61h
  000000006704C265: 72 50              jb          000000006704C2B7
  000000006704C267: 66 83 F8 7A        cmp         ax,7Ah
  000000006704C26B: 77 4A              ja          000000006704C2B7
  000000006704C26D: B9 FF 00 00 00     mov         ecx,0FFh
  000000006704C272: 66 3B C1           cmp         ax,cx
  000000006704C275: 76 05              jbe         000000006704C27C
  000000006704C277: 40 32 FF           xor         dil,dil
  000000006704C27A: EB 04              jmp         000000006704C280
  000000006704C27C: 41 8A 3C 24        mov         dil,byte ptr [r12]
  000000006704C280: 8B 56 04           mov         edx,dword ptr [rsi+4]
  000000006704C283: 3B 16              cmp         edx,dword ptr [rsi]
  000000006704C285: 75 0C              jne         000000006704C293
  000000006704C287: 44 8D 04 12        lea         r8d,[rdx+rdx]
  000000006704C28B: 48 8B CE           mov         rcx,rsi
  000000006704C28E: E8 9D 88 FB FF     call        0000000067004B30
  000000006704C293: 48 63 4E 04        movsxd      rcx,dword ptr [rsi+4]
  000000006704C297: 49 FF C7           inc         r15
  000000006704C29A: FF C3              inc         ebx
  000000006704C29C: 8D 41 01           lea         eax,[rcx+1]
  000000006704C29F: 49 83 C4 02        add         r12,2
  000000006704C2A3: 4C 3B FD           cmp         r15,rbp
  000000006704C2A6: 89 46 04           mov         dword ptr [rsi+4],eax
  000000006704C2A9: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006704C2AD: 40 88 3C 01        mov         byte ptr [rcx+rax],dil
  000000006704C2B1: 0F 8C 4E FF FF FF  jl          000000006704C205
  000000006704C2B7: 8B BC 24 80 00 00  mov         edi,dword ptr [rsp+0000000000000080h]
                    00
  000000006704C2BE: 4C 8B 64 24 70     mov         r12,qword ptr [rsp+70h]

  Summary

      1A7000 .text
