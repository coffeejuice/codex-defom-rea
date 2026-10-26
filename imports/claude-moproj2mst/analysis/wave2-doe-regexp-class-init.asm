Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006705ADA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006705ADA5: 57                 push        rdi
  000000006705ADA6: 48 83 EC 30        sub         rsp,30h
  000000006705ADAA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006705ADB3: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006705ADB8: 48 8B D9           mov         rbx,rcx
  000000006705ADBB: C7 01 00 00 00 00  mov         dword ptr [rcx],0
  000000006705ADC1: 48 8D 05 F0 5D 29  lea         rax,[00000000672F0BB8h]
                    00
  000000006705ADC8: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006705ADCC: F0 83 05 E4 5D 29  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006705ADD4: C6 41 10 00        mov         byte ptr [rcx+10h],0
  000000006705ADD8: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  000000006705ADDC: F0 83 05 D4 5D 29  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006705ADE4: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  000000006705ADE8: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006705ADEC: 41 83 F8 40        cmp         r8d,40h
  000000006705ADF0: 7C 19              jl          000000006705AE0B
  000000006705ADF2: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006705ADF6: 75 29              jne         000000006705AE21
  000000006705ADF8: 83 78 08 40        cmp         dword ptr [rax+8],40h
  000000006705ADFC: 7E 23              jle         000000006705AE21
  000000006705ADFE: 41 8B C0           mov         eax,r8d
  000000006705AE01: 83 E0 FE           and         eax,0FFFFFFFEh
  000000006705AE04: 3D 80 00 00 00     cmp         eax,80h
  000000006705AE09: 7E 16              jle         000000006705AE21
  000000006705AE0B: 45 33 C9           xor         r9d,r9d
  000000006705AE0E: 41 8D 51 40        lea         edx,[r9+40h]
  000000006705AE12: 8D 4A D4           lea         ecx,[rdx-2Ch]
  000000006705AE15: 45 8D 41 04        lea         r8d,[r9+4]
  000000006705AE19: E8 82 17 02 00     call        000000006707C5A0
  000000006705AE1E: 44 8B C0           mov         r8d,eax
  000000006705AE21: BA 40 00 00 00     mov         edx,40h
  000000006705AE26: 48 8D 4B 18        lea         rcx,[rbx+18h]
  000000006705AE2A: E8 71 D4 0A 00     call        00000000671082A0
  000000006705AE2F: 4C 8B 5B 18        mov         r11,qword ptr [rbx+18h]
  000000006705AE33: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006705AE37: 85 C0              test        eax,eax
  000000006705AE39: 74 2E              je          000000006705AE69
  000000006705AE3B: 49 8D 4C 83 10     lea         rcx,[r11+rax*4+10h]
  000000006705AE40: 49 8D 43 10        lea         rax,[r11+10h]
  000000006705AE44: 48 3B C8           cmp         rcx,rax
  000000006705AE47: 74 1B              je          000000006705AE64
  000000006705AE49: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006705AE50: 48 83 E9 04        sub         rcx,4
  000000006705AE54: C7 01 FF FF FF 7F  mov         dword ptr [rcx],7FFFFFFFh
  000000006705AE5A: 48 3B C8           cmp         rcx,rax
  000000006705AE5D: 75 F1              jne         000000006705AE50
  000000006705AE5F: 48 8B C3           mov         rax,rbx
  000000006705AE62: EB 08              jmp         000000006705AE6C
  000000006705AE64: 48 8B C3           mov         rax,rbx
  000000006705AE67: EB 03              jmp         000000006705AE6C
  000000006705AE69: 48 8B C3           mov         rax,rbx
  000000006705AE6C: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006705AE71: 48 83 C4 30        add         rsp,30h
  000000006705AE75: 5F                 pop         rdi
  000000006705AE76: C3                 ret

  Summary

      1A7000 .text
