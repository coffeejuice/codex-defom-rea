Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653F8A70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653F8A75: 57                 push        rdi
  00000000653F8A76: 48 83 EC 30        sub         rsp,30h
  00000000653F8A7A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653F8A83: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653F8A88: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000653F8A8D: 48 8B FA           mov         rdi,rdx
  00000000653F8A90: 48 8B F1           mov         rsi,rcx
  00000000653F8A93: B9 38 02 00 00     mov         ecx,238h
  00000000653F8A98: E8 D5 86 1E 00     call        00000000655E1172
  00000000653F8A9D: 48 8B D8           mov         rbx,rax
  00000000653F8AA0: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000653F8AA5: 48 85 C0           test        rax,rax
  00000000653F8AA8: 74 20              je          00000000653F8ACA
  00000000653F8AAA: BA 00 04 00 00     mov         edx,400h
  00000000653F8AAF: 48 8B C8           mov         rcx,rax
  00000000653F8AB2: E8 29 81 F7 FF     call        0000000065370BE0
  00000000653F8AB7: 4C 8D 1D 42 07 31  lea         r11,[0000000065709200h]
                    00
  00000000653F8ABE: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000653F8AC1: 83 8B 30 02 00 00  or          dword ptr [rbx+0000000000000230h],1
                    01
  00000000653F8AC8: EB 02              jmp         00000000653F8ACC
  00000000653F8ACA: 33 DB              xor         ebx,ebx
  00000000653F8ACC: 4C 8B C7           mov         r8,rdi
  00000000653F8ACF: 48 8B D3           mov         rdx,rbx
  00000000653F8AD2: 48 8B CE           mov         rcx,rsi
  00000000653F8AD5: E8 76 85 F7 FF     call        0000000065371050
  00000000653F8ADA: 90                 nop
  00000000653F8ADB: 48 8D 05 A6 07 31  lea         rax,[0000000065709288h]
                    00
  00000000653F8AE2: 48 89 06           mov         qword ptr [rsi],rax
  00000000653F8AE5: 48 8D 05 5C 07 31  lea         rax,[0000000065709248h]
                    00
  00000000653F8AEC: 48 89 46 10        mov         qword ptr [rsi+10h],rax
  00000000653F8AF0: 48 8B 7E 08        mov         rdi,qword ptr [rsi+8]
  00000000653F8AF4: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000653F8AF8: B2 01              mov         dl,1
  00000000653F8AFA: 48 8B CB           mov         rcx,rbx
  00000000653F8AFD: E8 EE 86 F7 FF     call        00000000653711F0
  00000000653F8B02: B2 01              mov         dl,1
  00000000653F8B04: 48 8B CB           mov         rcx,rbx
  00000000653F8B07: E8 D4 88 F7 FF     call        00000000653713E0
  00000000653F8B0C: 41 B0 01           mov         r8b,1
  00000000653F8B0F: BA 02 00 00 00     mov         edx,2
  00000000653F8B14: 48 8B CB           mov         rcx,rbx
  00000000653F8B17: E8 C4 04 C7 FF     call        0000000065068FE0
  00000000653F8B1C: 33 D2              xor         edx,edx
  00000000653F8B1E: 48 8B CB           mov         rcx,rbx
  00000000653F8B21: E8 9A DD C6 FF     call        00000000650668C0
  00000000653F8B26: 45 33 C0           xor         r8d,r8d
  00000000653F8B29: 41 8D 50 31        lea         edx,[r8+31h]
  00000000653F8B2D: 48 8B CF           mov         rcx,rdi
  00000000653F8B30: E8 EB 58 C6 FF     call        000000006505E420
  00000000653F8B35: 90                 nop
  00000000653F8B36: 48 8B C6           mov         rax,rsi
  00000000653F8B39: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653F8B3E: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000653F8B43: 48 83 C4 30        add         rsp,30h
  00000000653F8B47: 5F                 pop         rdi
  00000000653F8B48: C3                 ret

  Summary

      695000 .text
