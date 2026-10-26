Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C3A70: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670C3A75: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670C3A7A: 57                 push        rdi
  00000000670C3A7B: 48 83 EC 30        sub         rsp,30h
  00000000670C3A7F: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670C3A83: 48 8B F9           mov         rdi,rcx
  00000000670C3A86: 48 8B F2           mov         rsi,rdx
  00000000670C3A89: 48 8B CB           mov         rcx,rbx
  00000000670C3A8C: E8 BF F4 FF FF     call        00000000670C2F50
  00000000670C3A91: 80 7B 20 00        cmp         byte ptr [rbx+20h],0
  00000000670C3A95: 74 28              je          00000000670C3ABF
  00000000670C3A97: 48 8D 4B 08        lea         rcx,[rbx+8]
  00000000670C3A9B: 45 33 C9           xor         r9d,r9d
  00000000670C3A9E: 45 33 C0           xor         r8d,r8d
  00000000670C3AA1: 33 D2              xor         edx,edx
  00000000670C3AA3: E8 18 C3 06 00     call        000000006712FDC0
  00000000670C3AA8: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670C3AAB: 48 85 C9           test        rcx,rcx
  00000000670C3AAE: 74 0B              je          00000000670C3ABB
  00000000670C3AB0: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C3AB3: BA 01 00 00 00     mov         edx,1
  00000000670C3AB8: FF 50 18           call        qword ptr [rax+18h]
  00000000670C3ABB: C6 43 20 00        mov         byte ptr [rbx+20h],0
  00000000670C3ABF: 48 8B CB           mov         rcx,rbx
  00000000670C3AC2: E8 49 E7 FF FF     call        00000000670C2210
  00000000670C3AC7: 48 8B CB           mov         rcx,rbx
  00000000670C3ACA: C7 83 CC 00 00 00  mov         dword ptr [rbx+00000000000000CCh],0
                    00 00 00 00
  00000000670C3AD4: 48 89 33           mov         qword ptr [rbx],rsi
  00000000670C3AD7: E8 44 E8 FF FF     call        00000000670C2320
  00000000670C3ADC: 48 8B 33           mov         rsi,qword ptr [rbx]
  00000000670C3ADF: 48 8D 4B 08        lea         rcx,[rbx+8]
  00000000670C3AE3: 45 33 C9           xor         r9d,r9d
  00000000670C3AE6: 45 33 C0           xor         r8d,r8d
  00000000670C3AE9: 33 D2              xor         edx,edx
  00000000670C3AEB: E8 D0 C2 06 00     call        000000006712FDC0
  00000000670C3AF0: 48 85 F6           test        rsi,rsi
  00000000670C3AF3: 74 22              je          00000000670C3B17
  00000000670C3AF5: 4C 8D 0D 1C 88 16  lea         r9,[000000006722C318h]
                    00
  00000000670C3AFC: 4C 8D 43 08        lea         r8,[rbx+8]
  00000000670C3B00: 48 8D 15 01 88 16  lea         rdx,[000000006722C308h]
                    00
  00000000670C3B07: 48 8B CE           mov         rcx,rsi
  00000000670C3B0A: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000670C3B12: E8 A9 DA 06 00     call        00000000671315C0
  00000000670C3B17: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000670C3B1C: 48 89 7B 18        mov         qword ptr [rbx+18h],rdi
  00000000670C3B20: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670C3B25: 48 83 C4 30        add         rsp,30h
  00000000670C3B29: 5F                 pop         rdi
  00000000670C3B2A: C3                 ret

  Summary

      1A7000 .text
