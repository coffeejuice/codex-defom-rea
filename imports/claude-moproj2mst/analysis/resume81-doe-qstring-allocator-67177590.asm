Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067177590: 48 89 0D 79 C8 17  mov         qword ptr [00000000672F3E10h],rcx
                    00
  0000000067177597: C3                 ret
  0000000067177598: CC                 int         3
  0000000067177599: CC                 int         3
  000000006717759A: CC                 int         3
  000000006717759B: CC                 int         3
  000000006717759C: CC                 int         3
  000000006717759D: CC                 int         3
  000000006717759E: CC                 int         3
  000000006717759F: CC                 int         3
  00000000671775A0: B8 FF 00 00 00     mov         eax,0FFh
  00000000671775A5: 66 39 01           cmp         word ptr [rcx],ax
  00000000671775A8: 76 03              jbe         00000000671775AD
  00000000671775AA: 32 C0              xor         al,al
  00000000671775AC: C3                 ret
  00000000671775AD: 8A 01              mov         al,byte ptr [rcx]
  00000000671775AF: C3                 ret
  00000000671775B0: 40 53              push        rbx
  00000000671775B2: 48 83 EC 30        sub         rsp,30h
  00000000671775B6: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  00000000671775BB: 48 8B DA           mov         rbx,rdx
  00000000671775BE: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  00000000671775C6: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000000671775CA: E8 61 83 EB FF     call        000000006702F930
  00000000671775CF: 48 8B C3           mov         rax,rbx
  00000000671775D2: 48 83 C4 30        add         rsp,30h
  00000000671775D6: 5B                 pop         rbx
  00000000671775D7: C3                 ret
  00000000671775D8: CC                 int         3
  00000000671775D9: CC                 int         3
  00000000671775DA: CC                 int         3
  00000000671775DB: CC                 int         3
  00000000671775DC: CC                 int         3
  00000000671775DD: CC                 int         3
  00000000671775DE: CC                 int         3
  00000000671775DF: CC                 int         3

  Summary

      1A7000 .text
