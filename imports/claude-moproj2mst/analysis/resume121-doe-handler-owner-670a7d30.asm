Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7D30: 40 57              push        rdi
  00000000670A7D32: 48 83 EC 30        sub         rsp,30h
  00000000670A7D36: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A7D3F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670A7D44: 48 8B F9           mov         rdi,rcx
  00000000670A7D47: 48 8D 05 42 37 17  lea         rax,[000000006721B490h]
                    00
  00000000670A7D4E: 48 89 01           mov         qword ptr [rcx],rax
  00000000670A7D51: E8 AA FD FF FF     call        00000000670A7B00
  00000000670A7D56: 48 8B D8           mov         rbx,rax
  00000000670A7D59: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670A7D5E: 48 85 C0           test        rax,rax
  00000000670A7D61: 74 19              je          00000000670A7D7C
  00000000670A7D63: A8 01              test        al,1
  00000000670A7D65: 75 15              jne         00000000670A7D7C
  00000000670A7D67: 48 8B C8           mov         rcx,rax
  00000000670A7D6A: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  00000000670A7D6E: E8 3D E2 F7 FF     call        0000000067025FB0
  00000000670A7D73: 48 83 CB 01        or          rbx,1
  00000000670A7D77: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670A7D7C: C6 05 4D C1 24 00  mov         byte ptr [00000000672F3ED0h],1
                    01
  00000000670A7D83: 48 89 7C 24 48     mov         qword ptr [rsp+48h],rdi
  00000000670A7D88: E8 D3 FE FF FF     call        00000000670A7C60
  00000000670A7D8D: 48 8B C8           mov         rcx,rax
  00000000670A7D90: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670A7D95: E8 36 40 0A 00     call        000000006714BDD0
  00000000670A7D9A: 90                 nop
  00000000670A7D9B: 48 85 DB           test        rbx,rbx
  00000000670A7D9E: 74 1B              je          00000000670A7DBB
  00000000670A7DA0: F6 C3 01           test        bl,1
  00000000670A7DA3: 74 11              je          00000000670A7DB6
  00000000670A7DA5: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  00000000670A7DA9: 48 8B CB           mov         rcx,rbx
  00000000670A7DAC: E8 FF EC F7 FF     call        0000000067026AB0
  00000000670A7DB1: 48 8B C7           mov         rax,rdi
  00000000670A7DB4: EB 08              jmp         00000000670A7DBE
  00000000670A7DB6: 48 8B C7           mov         rax,rdi
  00000000670A7DB9: EB 03              jmp         00000000670A7DBE
  00000000670A7DBB: 48 8B C7           mov         rax,rdi
  00000000670A7DBE: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670A7DC3: 48 83 C4 30        add         rsp,30h
  00000000670A7DC7: 5F                 pop         rdi
  00000000670A7DC8: C3                 ret

  Summary

      1A7000 .text
