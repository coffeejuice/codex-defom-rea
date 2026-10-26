Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711ACD0: 40 53              push        rbx
  000000006711ACD2: 56                 push        rsi
  000000006711ACD3: 57                 push        rdi
  000000006711ACD4: 48 83 EC 20        sub         rsp,20h
  000000006711ACD8: 8B 72 10           mov         esi,dword ptr [rdx+10h]
  000000006711ACDB: 48 8B D9           mov         rbx,rcx
  000000006711ACDE: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006711ACE1: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006711ACE4: 48 8B FA           mov         rdi,rdx
  000000006711ACE7: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  000000006711ACEA: 0F 84 DE 00 00 00  je          000000006711ADCE
  000000006711ACF0: 8B 01              mov         eax,dword ptr [rcx]
  000000006711ACF2: 83 F8 01           cmp         eax,1
  000000006711ACF5: 74 0E              je          000000006711AD05
  000000006711ACF7: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006711ACFA: 48 8B CB           mov         rcx,rbx
  000000006711ACFD: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711AD00: E8 BB E1 FF FF     call        0000000067118EC0
  000000006711AD05: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006711AD08: 48 63 42 0C        movsxd      rax,dword ptr [rdx+0Ch]
  000000006711AD0C: 48 8B 4C C2 10     mov         rcx,qword ptr [rdx+rax*8+10h]
  000000006711AD11: 39 71 10           cmp         dword ptr [rcx+10h],esi
  000000006711AD14: 0F 8D B4 00 00 00  jge         000000006711ADCE
  000000006711AD1A: 8B 02              mov         eax,dword ptr [rdx]
  000000006711AD1C: 83 F8 01           cmp         eax,1
  000000006711AD1F: 74 0E              je          000000006711AD2F
  000000006711AD21: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006711AD24: 48 8B CB           mov         rcx,rbx
  000000006711AD27: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711AD2A: E8 91 E1 FF FF     call        0000000067118EC0
  000000006711AD2F: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006711AD32: 48 63 42 0C        movsxd      rax,dword ptr [rdx+0Ch]
  000000006711AD36: 48 8D 4C C2 18     lea         rcx,[rdx+rax*8+18h]
  000000006711AD3B: 8B 02              mov         eax,dword ptr [rdx]
  000000006711AD3D: 48 89 4C 24 40     mov         qword ptr [rsp+40h],rcx
  000000006711AD42: 83 F8 01           cmp         eax,1
  000000006711AD45: 74 0E              je          000000006711AD55
  000000006711AD47: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006711AD4A: 48 8B CB           mov         rcx,rbx
  000000006711AD4D: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711AD50: E8 6B E1 FF FF     call        0000000067118EC0
  000000006711AD55: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006711AD58: 48 63 43 10        movsxd      rax,dword ptr [rbx+10h]
  000000006711AD5C: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  000000006711AD61: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  000000006711AD65: 48 03 C8           add         rcx,rax
  000000006711AD68: 48 8D 4C CA 18     lea         rcx,[rdx+rcx*8+18h]
  000000006711AD6D: 4C 2B C1           sub         r8,rcx
  000000006711AD70: 49 C1 F8 03        sar         r8,3
  000000006711AD74: 45 85 C0           test        r8d,r8d
  000000006711AD77: 7E 33              jle         000000006711ADAC
  000000006711AD79: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006711AD80: 45 8B C8           mov         r9d,r8d
  000000006711AD83: 41 D1 F9           sar         r9d,1
  000000006711AD86: 49 63 C1           movsxd      rax,r9d
  000000006711AD89: 48 8D 04 C1        lea         rax,[rcx+rax*8]
  000000006711AD8D: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006711AD90: 39 72 10           cmp         dword ptr [rdx+10h],esi
  000000006711AD93: 7D 05              jge         000000006711AD9A
  000000006711AD95: 45 8B C1           mov         r8d,r9d
  000000006711AD98: EB 0D              jmp         000000006711ADA7
  000000006711AD9A: 48 8D 48 08        lea         rcx,[rax+8]
  000000006711AD9E: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006711ADA1: 41 2B C1           sub         eax,r9d
  000000006711ADA4: 44 03 C0           add         r8d,eax
  000000006711ADA7: 45 85 C0           test        r8d,r8d
  000000006711ADAA: 7F D4              jg          000000006711AD80
  000000006711ADAC: 48 89 4C 24 48     mov         qword ptr [rsp+48h],rcx
  000000006711ADB1: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000006711ADB6: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006711ADBB: 4C 8B CF           mov         r9,rdi
  000000006711ADBE: 48 8B CB           mov         rcx,rbx
  000000006711ADC1: E8 6A DF FF FF     call        0000000067118D30
  000000006711ADC6: 48 83 C4 20        add         rsp,20h
  000000006711ADCA: 5F                 pop         rdi
  000000006711ADCB: 5E                 pop         rsi
  000000006711ADCC: 5B                 pop         rbx
  000000006711ADCD: C3                 ret
  000000006711ADCE: 48 8B D7           mov         rdx,rdi
  000000006711ADD1: 48 8B CB           mov         rcx,rbx
  000000006711ADD4: 48 83 C4 20        add         rsp,20h
  000000006711ADD8: 5F                 pop         rdi
  000000006711ADD9: 5E                 pop         rsi
  000000006711ADDA: 5B                 pop         rbx
  000000006711ADDB: E9 20 F4 FF FF     jmp         000000006711A200

  Summary

      1A7000 .text
