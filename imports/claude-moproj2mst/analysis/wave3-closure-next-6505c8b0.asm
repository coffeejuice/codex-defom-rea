Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505C8B0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006505C8B5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006505C8BA: 57                 push        rdi
  000000006505C8BB: 48 83 EC 20        sub         rsp,20h
  000000006505C8BF: 8B 01              mov         eax,dword ptr [rcx]
  000000006505C8C1: 48 8B D9           mov         rbx,rcx
  000000006505C8C4: A9 00 F8 01 0A     test        eax,0A01F800h
  000000006505C8C9: 0F B6 F8           movzx       edi,al
  000000006505C8CC: 40 0F 95 C6        setne       sil
  000000006505C8D0: 84 C0              test        al,al
  000000006505C8D2: 74 05              je          000000006505C8D9
  000000006505C8D4: 83 FF 12           cmp         edi,12h
  000000006505C8D7: 75 1A              jne         000000006505C8F3
  000000006505C8D9: 48 85 D2           test        rdx,rdx
  000000006505C8DC: 74 15              je          000000006505C8F3
  000000006505C8DE: 48 8B CA           mov         rcx,rdx
  000000006505C8E1: FF 15 81 9F 63 00  call        qword ptr [0000000065696868h]
  000000006505C8E7: 48 85 C0           test        rax,rax
  000000006505C8EA: 75 07              jne         000000006505C8F3
  000000006505C8EC: BF 01 00 00 00     mov         edi,1
  000000006505C8F1: 09 3B              or          dword ptr [rbx],edi
  000000006505C8F3: 8B 03              mov         eax,dword ptr [rbx]
  000000006505C8F5: 0F BA E0 19        bt          eax,19h
  000000006505C8F9: 73 0D              jae         000000006505C908
  000000006505C8FB: A9 00 C0 01 08     test        eax,801C000h
  000000006505C900: 74 18              je          000000006505C91A
  000000006505C902: 0F BA F0 0B        btr         eax,0Bh
  000000006505C906: EB 0B              jmp         000000006505C913
  000000006505C908: 40 84 F6           test        sil,sil
  000000006505C90B: 74 12              je          000000006505C91F
  000000006505C90D: 0F BA E0 0B        bt          eax,0Bh
  000000006505C911: 72 07              jb          000000006505C91A
  000000006505C913: 0D 00 30 00 00     or          eax,3000h
  000000006505C918: 89 03              mov         dword ptr [rbx],eax
  000000006505C91A: 40 84 F6           test        sil,sil
  000000006505C91D: 75 41              jne         000000006505C960
  000000006505C91F: 83 FF 03           cmp         edi,3
  000000006505C922: 74 36              je          000000006505C95A
  000000006505C924: 83 FF 05           cmp         edi,5
  000000006505C927: 74 31              je          000000006505C95A
  000000006505C929: 83 FF 0B           cmp         edi,0Bh
  000000006505C92C: 75 16              jne         000000006505C944
  000000006505C92E: 81 0B 00 30 00 08  or          dword ptr [rbx],8003000h
  000000006505C934: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006505C939: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006505C93E: 48 83 C4 20        add         rsp,20h
  000000006505C942: 5F                 pop         rdi
  000000006505C943: C3                 ret
  000000006505C944: 81 0B 00 F0 00 08  or          dword ptr [rbx],800F000h
  000000006505C94A: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006505C94F: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006505C954: 48 83 C4 20        add         rsp,20h
  000000006505C958: 5F                 pop         rdi
  000000006505C959: C3                 ret
  000000006505C95A: 81 0B 00 30 01 08  or          dword ptr [rbx],8013000h
  000000006505C960: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006505C965: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006505C96A: 48 83 C4 20        add         rsp,20h
  000000006505C96E: 5F                 pop         rdi
  000000006505C96F: C3                 ret

  Summary

      695000 .text
