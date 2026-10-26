Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065081B10: 40 57              push        rdi
  0000000065081B12: 48 83 EC 30        sub         rsp,30h
  0000000065081B16: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065081B1F: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  0000000065081B24: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  0000000065081B29: 0F B6 DA           movzx       ebx,dl
  0000000065081B2C: 48 8B F9           mov         rdi,rcx
  0000000065081B2F: 8B 05 8B 93 94 00  mov         eax,dword ptr [00000000659CAEC0h]
  0000000065081B35: A8 01              test        al,1
  0000000065081B37: 75 2E              jne         0000000065081B67
  0000000065081B39: 83 C8 01           or          eax,1
  0000000065081B3C: 89 05 7E 93 94 00  mov         dword ptr [00000000659CAEC0h],eax
  0000000065081B42: 48 C7 05 6B 93 94  mov         qword ptr [00000000659CAEB8h],0
                    00 00 00 00 00
  0000000065081B4D: 48 8D 0D 64 93 94  lea         rcx,[00000000659CAEB8h]
                    00
  0000000065081B54: FF 15 7E 4D 61 00  call        qword ptr [00000000656968D8h]
  0000000065081B5A: 48 8D 0D CF 2D 61  lea         rcx,[0000000065694930h]
                    00
  0000000065081B61: E8 C6 F4 55 00     call        00000000655E102C
  0000000065081B66: 90                 nop
  0000000065081B67: 84 DB              test        bl,bl
  0000000065081B69: 74 1B              je          0000000065081B86
  0000000065081B6B: 48 39 3D 46 93 94  cmp         qword ptr [00000000659CAEB8h],rdi
                    00
  0000000065081B72: 74 4A              je          0000000065081BBE
  0000000065081B74: 48 8B D7           mov         rdx,rdi
  0000000065081B77: 48 8D 0D 3A 93 94  lea         rcx,[00000000659CAEB8h]
                    00
  0000000065081B7E: FF 15 64 4D 61 00  call        qword ptr [00000000656968E8h]
  0000000065081B84: EB 38              jmp         0000000065081BBE
  0000000065081B86: BA 3C 00 00 00     mov         edx,3Ch
  0000000065081B8B: 48 8B CF           mov         rcx,rdi
  0000000065081B8E: E8 7D C4 FD FF     call        000000006505E010
  0000000065081B93: 84 C0              test        al,al
  0000000065081B95: 74 27              je          0000000065081BBE
  0000000065081B97: 48 8B 0D 1A 93 94  mov         rcx,qword ptr [00000000659CAEB8h]
                    00
  0000000065081B9E: 48 85 C9           test        rcx,rcx
  0000000065081BA1: 74 1B              je          0000000065081BBE
  0000000065081BA3: E8 58 D3 FD FF     call        000000006505EF00
  0000000065081BA8: 48 8B D8           mov         rbx,rax
  0000000065081BAB: 48 8B CF           mov         rcx,rdi
  0000000065081BAE: E8 4D D3 FD FF     call        000000006505EF00
  0000000065081BB3: 48 3B D8           cmp         rbx,rax
  0000000065081BB6: 48 0F 44 3D FA 92  cmove       rdi,qword ptr [00000000659CAEB8h]
                    94 00
  0000000065081BBE: 48 8B 0D 63 92 94  mov         rcx,qword ptr [00000000659CAE28h]
                    00
  0000000065081BC5: 48 85 C9           test        rcx,rcx
  0000000065081BC8: 75 12              jne         0000000065081BDC
  0000000065081BCA: 48 85 FF           test        rdi,rdi
  0000000065081BCD: 74 21              je          0000000065081BF0
  0000000065081BCF: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065081BD3: 48 39 08           cmp         qword ptr [rax],rcx
  0000000065081BD6: 0F 85 84 00 00 00  jne         0000000065081C60
  0000000065081BDC: 48 85 FF           test        rdi,rdi
  0000000065081BDF: 74 0F              je          0000000065081BF0
  0000000065081BE1: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065081BE5: 48 83 38 00        cmp         qword ptr [rax],0
  0000000065081BE9: 75 05              jne         0000000065081BF0
  0000000065081BEB: 48 8B F7           mov         rsi,rdi
  0000000065081BEE: EB 08              jmp         0000000065081BF8
  0000000065081BF0: E8 9B D2 FD FF     call        000000006505EE90
  0000000065081BF5: 48 8B F0           mov         rsi,rax
  0000000065081BF8: 48 85 F6           test        rsi,rsi
  0000000065081BFB: 74 63              je          0000000065081C60
  0000000065081BFD: 48 8B CE           mov         rcx,rsi
  0000000065081C00: E8 5B AF FD FF     call        000000006505CB60
  0000000065081C05: 48 8B D8           mov         rbx,rax
  0000000065081C08: 48 8B CF           mov         rcx,rdi
  0000000065081C0B: E8 50 AF FD FF     call        000000006505CB60
  0000000065081C10: 48 3B D8           cmp         rbx,rax
  0000000065081C13: 75 4B              jne         0000000065081C60
  0000000065081C15: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065081C19: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065081C1C: 8B C1              mov         eax,ecx
  0000000065081C1E: C1 E8 0F           shr         eax,0Fh
  0000000065081C21: 24 01              and         al,1
  0000000065081C23: 74 3B              je          0000000065081C60
  0000000065081C25: D1 E9              shr         ecx,1
  0000000065081C27: 80 E1 01           and         cl,1
  0000000065081C2A: 74 34              je          0000000065081C60
  0000000065081C2C: E8 5F 0F FA FF     call        0000000065022B90
  0000000065081C31: 48 85 C0           test        rax,rax
  0000000065081C34: 75 2A              jne         0000000065081C60
  0000000065081C36: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065081C3B: 48 8B CE           mov         rcx,rsi
  0000000065081C3E: E8 FD DA FD FF     call        000000006505F740
  0000000065081C43: 90                 nop
  0000000065081C44: 48 8B C8           mov         rcx,rax
  0000000065081C47: E8 F4 A4 00 00     call        000000006508C140
  0000000065081C4C: 48 8B C8           mov         rcx,rax
  0000000065081C4F: FF 15 2B 7E 61 00  call        qword ptr [0000000065699A80h]
  0000000065081C55: 90                 nop
  0000000065081C56: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065081C5B: E8 40 D5 FA FF     call        000000006502F1A0
  0000000065081C60: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000065081C65: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  0000000065081C6A: 48 83 C4 30        add         rsp,30h
  0000000065081C6E: 5F                 pop         rdi
  0000000065081C6F: C3                 ret

  Summary

      695000 .text
