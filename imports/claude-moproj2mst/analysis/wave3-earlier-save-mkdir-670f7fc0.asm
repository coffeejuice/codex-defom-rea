Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F7FC0: 4C 8B DC           mov         r11,rsp
  00000000670F7FC3: 49 89 4B 08        mov         qword ptr [r11+8],rcx
  00000000670F7FC7: 57                 push        rdi
  00000000670F7FC8: 48 83 EC 60        sub         rsp,60h
  00000000670F7FCC: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F7FD5: 49 89 5B 10        mov         qword ptr [r11+10h],rbx
  00000000670F7FD9: 48 8B DA           mov         rbx,rdx
  00000000670F7FDC: 48 8B F9           mov         rdi,rcx
  00000000670F7FDF: 41 C7 43 18 00 00  mov         dword ptr [r11+18h],0
                    00 00
  00000000670F7FE7: 48 8D 05 9A E1 13  lea         rax,[0000000067236188h]
                    00
  00000000670F7FEE: 49 89 43 C8        mov         qword ptr [r11-38h],rax
  00000000670F7FF2: 41 B8 01 00 00 00  mov         r8d,1
  00000000670F7FF8: 49 8D 53 C8        lea         rdx,[r11-38h]
  00000000670F7FFC: 48 8B CB           mov         rcx,rbx
  00000000670F7FFF: E8 FC 60 F7 FF     call        000000006706E100
  00000000670F8004: 84 C0              test        al,al
  00000000670F8006: 74 12              je          00000000670F801A
  00000000670F8008: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670F800B: 48 89 07           mov         qword ptr [rdi],rax
  00000000670F800E: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670F8012: 48 8B C7           mov         rax,rdi
  00000000670F8015: E9 1C 01 00 00     jmp         00000000670F8136
  00000000670F801A: 48 8B D3           mov         rdx,rbx
  00000000670F801D: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000670F8025: E8 C6 7B 00 00     call        00000000670FFBF0
  00000000670F802A: 90                 nop
  00000000670F802B: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670F802E: 48 8D 0D 4B E1 13  lea         rcx,[0000000067236180h]
                    00
  00000000670F8035: E8 06 3F F7 FF     call        000000006706BF40
  00000000670F803A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670F803F: 48 8D 05 46 A4 0F  lea         rax,[00000000671F248Ch]
                    00
  00000000670F8046: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000670F804B: 41 B8 01 00 00 00  mov         r8d,1
  00000000670F8051: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000670F8056: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000670F805E: E8 9D 60 F7 FF     call        000000006706E100
  00000000670F8063: 84 C0              test        al,al
  00000000670F8065: 74 47              je          00000000670F80AE
  00000000670F8067: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F806F: 83 78 08 02        cmp         dword ptr [rax+8],2
  00000000670F8073: 7E 39              jle         00000000670F80AE
  00000000670F8075: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670F8079: 66 83 79 04 2E     cmp         word ptr [rcx+4],2Eh
  00000000670F807E: 74 2E              je          00000000670F80AE
  00000000670F8080: 48 8D 05 F1 E0 13  lea         rax,[0000000067236178h]
                    00
  00000000670F8087: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670F808C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670F8091: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670F8096: E8 65 84 F7 FF     call        0000000067070500
  00000000670F809B: 33 D2              xor         edx,edx
  00000000670F809D: 44 8D 42 02        lea         r8d,[rdx+2]
  00000000670F80A1: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000670F80A9: E8 B2 AD F7 FF     call        0000000067072E60
  00000000670F80AE: 48 8D 44 24 20     lea         rax,[rsp+20h]
  00000000670F80B3: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000670F80B8: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000000670F80C0: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000670F80C5: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670F80CA: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000670F80CF: E8 EC 60 F2 FF     call        000000006701E1C0
  00000000670F80D4: 4C 8B 5C 24 28     mov         r11,qword ptr [rsp+28h]
  00000000670F80D9: 4C 89 1F           mov         qword ptr [rdi],r11
  00000000670F80DC: F0 41 83 03 01     lock add    dword ptr [r11],1
  00000000670F80E1: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  00000000670F80EC: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670F80F1: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F80F5: 75 0B              jne         00000000670F8102
  00000000670F80F7: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670F80FC: E8 CF 7F F7 FF     call        00000000670700D0
  00000000670F8101: 90                 nop
  00000000670F8102: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670F8107: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F810B: 75 0B              jne         00000000670F8118
  00000000670F810D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670F8112: E8 B9 7F F7 FF     call        00000000670700D0
  00000000670F8117: 90                 nop
  00000000670F8118: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F8120: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F8124: 75 0D              jne         00000000670F8133
  00000000670F8126: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F812E: E8 9D 7F F7 FF     call        00000000670700D0
  00000000670F8133: 48 8B C7           mov         rax,rdi
  00000000670F8136: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  00000000670F813B: 48 83 C4 60        add         rsp,60h
  00000000670F813F: 5F                 pop         rdi
  00000000670F8140: C3                 ret
  00000000670F8141: CC                 int         3

  Summary

      1A7000 .text
