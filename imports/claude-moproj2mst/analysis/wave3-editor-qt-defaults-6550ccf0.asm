Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006550CCF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006550CCF5: 55                 push        rbp
  000000006550CCF6: 56                 push        rsi
  000000006550CCF7: 57                 push        rdi
  000000006550CCF8: 48 83 EC 30        sub         rsp,30h
  000000006550CCFC: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006550CD05: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  000000006550CD0A: 48 8B EA           mov         rbp,rdx
  000000006550CD0D: 48 8B F9           mov         rdi,rcx
  000000006550CD10: B9 10 04 00 00     mov         ecx,410h
  000000006550CD15: E8 58 44 0D 00     call        00000000655E1172
  000000006550CD1A: 48 8B D8           mov         rbx,rax
  000000006550CD1D: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006550CD22: 33 F6              xor         esi,esi
  000000006550CD24: 48 85 C0           test        rax,rax
  000000006550CD27: 74 14              je          000000006550CD3D
  000000006550CD29: 48 8B C8           mov         rcx,rax
  000000006550CD2C: E8 9F AB FC FF     call        00000000654D78D0
  000000006550CD31: 4C 8D 1D 30 AC 22  lea         r11,[0000000065737968h]
                    00
  000000006550CD38: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006550CD3B: EB 03              jmp         000000006550CD40
  000000006550CD3D: 48 8B DE           mov         rbx,rsi
  000000006550CD40: 4C 8B C5           mov         r8,rbp
  000000006550CD43: 48 8B D3           mov         rdx,rbx
  000000006550CD46: 48 8B CF           mov         rcx,rdi
  000000006550CD49: E8 92 96 FC FF     call        00000000654D63E0
  000000006550CD4E: 90                 nop
  000000006550CD4F: 48 8D 05 52 9C 22  lea         rax,[00000000657369A8h]
                    00
  000000006550CD56: 48 89 07           mov         qword ptr [rdi],rax
  000000006550CD59: 48 8D 05 10 9C 22  lea         rax,[0000000065736970h]
                    00
  000000006550CD60: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  000000006550CD64: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  000000006550CD68: B9 38 00 00 00     mov         ecx,38h
  000000006550CD6D: E8 00 44 0D 00     call        00000000655E1172
  000000006550CD72: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006550CD77: 48 85 C0           test        rax,rax
  000000006550CD7A: 74 13              je          000000006550CD8F
  000000006550CD7C: 4C 8B CF           mov         r9,rdi
  000000006550CD7F: 45 33 C0           xor         r8d,r8d
  000000006550CD82: 33 D2              xor         edx,edx
  000000006550CD84: 48 8B C8           mov         rcx,rax
  000000006550CD87: E8 64 B6 FF FF     call        00000000655083F0
  000000006550CD8C: 48 8B F0           mov         rsi,rax
  000000006550CD8F: 48 8B D6           mov         rdx,rsi
  000000006550CD92: 48 8B CF           mov         rcx,rdi
  000000006550CD95: E8 76 37 FC FF     call        00000000654D0510
  000000006550CD9A: 48 8B CB           mov         rcx,rbx
  000000006550CD9D: E8 6E 86 FF FF     call        0000000065505410
  000000006550CDA2: 90                 nop
  000000006550CDA3: 48 8B C7           mov         rax,rdi
  000000006550CDA6: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000006550CDAB: 48 83 C4 30        add         rsp,30h
  000000006550CDAF: 5F                 pop         rdi
  000000006550CDB0: 5E                 pop         rsi
  000000006550CDB1: 5D                 pop         rbp
  000000006550CDB2: C3                 ret

  Summary

      695000 .text
