Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712B1E0: 48 83 EC 28        sub         rsp,28h
  000000006712B1E4: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006712B1E8: 85 D2              test        edx,edx
  000000006712B1EA: 79 13              jns         000000006712B1FF
  000000006712B1EC: 48 8D 0D 0D 13 11  lea         rcx,[000000006723C500h]
                    00
  000000006712B1F3: E8 E8 1C EF FF     call        000000006701CEE0
  000000006712B1F8: 33 C0              xor         eax,eax
  000000006712B1FA: 48 83 C4 28        add         rsp,28h
  000000006712B1FE: C3                 ret
  000000006712B1FF: 83 48 20 02        or          dword ptr [rax+20h],2
  000000006712B203: 48 8B 40 40        mov         rax,qword ptr [rax+40h]
  000000006712B207: 4C 8B 48 20        mov         r9,qword ptr [rax+20h]
  000000006712B20B: 4D 85 C9           test        r9,r9
  000000006712B20E: 75 13              jne         000000006712B223
  000000006712B210: 48 8D 0D 99 12 11  lea         rcx,[000000006723C4B0h]
                    00
  000000006712B217: E8 C4 1C EF FF     call        000000006701CEE0
  000000006712B21C: 33 C0              xor         eax,eax
  000000006712B21E: 48 83 C4 28        add         rsp,28h
  000000006712B222: C3                 ret
  000000006712B223: 4C 8B C1           mov         r8,rcx
  000000006712B226: 49 8B C9           mov         rcx,r9
  000000006712B229: 48 83 C4 28        add         rsp,28h
  000000006712B22D: E9 6E 13 FE FF     jmp         000000006710C5A0

  Summary

      1A7000 .text
