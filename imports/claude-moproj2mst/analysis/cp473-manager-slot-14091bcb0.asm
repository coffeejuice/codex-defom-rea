Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091BCB0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091BCB5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091BCBA: 48 83 EC 58        sub         rsp,58h
  000000014091BCBE: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091BCC7: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  000000014091BCCF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014091BCD4: E8 07 3D 87 FF     call        000000014018F9E0
  000000014091BCD9: 90                 nop
  000000014091BCDA: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014091BCDF: 48 83 C2 38        add         rdx,38h
  000000014091BCE3: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BCE8: E8 E3 DD 93 FF     call        0000000140259AD0
  000000014091BCED: 90                 nop
  000000014091BCEE: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091BCF6: EB 0B              jmp         000000014091BD03
  000000014091BCF8: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091BCFC: 83 C0 01           add         eax,1
  000000014091BCFF: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091BD03: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BD08: E8 D3 8D 6E FF     call        0000000140004AE0
  000000014091BD0D: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091BD11: 7D 50              jge         000000014091BD63
  000000014091BD13: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091BD17: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BD1C: E8 EF 4E 84 FF     call        0000000140160C10
  000000014091BD21: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091BD24: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091BD28: 89 41 18           mov         dword ptr [rcx+18h],eax
  000000014091BD2B: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091BD2F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BD34: E8 D7 4E 84 FF     call        0000000140160C10
  000000014091BD39: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091BD3C: E8 6F 09 8B FF     call        00000001401CC6B0
  000000014091BD41: 48 85 C0           test        rax,rax
  000000014091BD44: 75 1B              jne         000000014091BD61
  000000014091BD46: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091BD4A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BD4F: E8 BC 4E 84 FF     call        0000000140160C10
  000000014091BD54: 48 8B D0           mov         rdx,rax
  000000014091BD57: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014091BD5C: E8 7F 1A E1 00     call        000000014172D7E0
  000000014091BD61: EB 95              jmp         000000014091BCF8
  000000014091BD63: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014091BD68: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014091BD6D: E8 5E DD 93 FF     call        0000000140259AD0
  000000014091BD72: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014091BD76: 83 C8 01           or          eax,1
  000000014091BD79: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014091BD7D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091BD82: E8 19 C4 8E FF     call        00000001402081A0
  000000014091BD87: 90                 nop
  000000014091BD88: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014091BD8D: E8 0E C4 8E FF     call        00000001402081A0
  000000014091BD92: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014091BD97: 48 83 C4 58        add         rsp,58h
  000000014091BD9B: C3                 ret
  000000014091BD9C: CC                 int         3

  Summary

     1E02000 .text
