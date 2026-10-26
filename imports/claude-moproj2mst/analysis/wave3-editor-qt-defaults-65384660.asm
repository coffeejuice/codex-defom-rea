Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065384660: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065384665: 57                 push        rdi
  0000000065384666: 48 83 EC 30        sub         rsp,30h
  000000006538466A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065384673: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065384678: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006538467D: 48 8B FA           mov         rdi,rdx
  0000000065384680: 48 8B F1           mov         rsi,rcx
  0000000065384683: B9 38 02 00 00     mov         ecx,238h
  0000000065384688: E8 E5 CA 25 00     call        00000000655E1172
  000000006538468D: 48 8B D8           mov         rbx,rax
  0000000065384690: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065384695: 48 85 C0           test        rax,rax
  0000000065384698: 74 2B              je          00000000653846C5
  000000006538469A: BA 04 00 00 00     mov         edx,4
  000000006538469F: 48 8B C8           mov         rcx,rax
  00000000653846A2: E8 39 C5 FE FF     call        0000000065370BE0
  00000000653846A7: 4C 8D 1D C2 29 37  lea         r11,[00000000656F7070h]
                    00
  00000000653846AE: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000653846B1: 8B 83 30 02 00 00  mov         eax,dword ptr [rbx+0000000000000230h]
  00000000653846B7: 83 E0 E4           and         eax,0FFFFFFE4h
  00000000653846BA: 83 C8 04           or          eax,4
  00000000653846BD: 89 83 30 02 00 00  mov         dword ptr [rbx+0000000000000230h],eax
  00000000653846C3: EB 02              jmp         00000000653846C7
  00000000653846C5: 33 DB              xor         ebx,ebx
  00000000653846C7: 4C 8B C7           mov         r8,rdi
  00000000653846CA: 48 8B D3           mov         rdx,rbx
  00000000653846CD: 48 8B CE           mov         rcx,rsi
  00000000653846D0: E8 7B C9 FE FF     call        0000000065371050
  00000000653846D5: 90                 nop
  00000000653846D6: 48 8D 05 1B 2A 37  lea         rax,[00000000656F70F8h]
                    00
  00000000653846DD: 48 89 06           mov         qword ptr [rsi],rax
  00000000653846E0: 48 8D 05 D1 29 37  lea         rax,[00000000656F70B8h]
                    00
  00000000653846E7: 48 89 46 10        mov         qword ptr [rsi+10h],rax
  00000000653846EB: 48 8B 7E 08        mov         rdi,qword ptr [rsi+8]
  00000000653846EF: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000653846F3: B2 01              mov         dl,1
  00000000653846F5: 48 8B CB           mov         rcx,rbx
  00000000653846F8: E8 F3 CA FE FF     call        00000000653711F0
  00000000653846FD: 41 B0 01           mov         r8b,1
  0000000065384700: BA 02 00 00 00     mov         edx,2
  0000000065384705: 48 8B CB           mov         rcx,rbx
  0000000065384708: E8 D3 48 CE FF     call        0000000065068FE0
  000000006538470D: 33 D2              xor         edx,edx
  000000006538470F: 48 8B CB           mov         rcx,rbx
  0000000065384712: E8 A9 21 CE FF     call        00000000650668C0
  0000000065384717: 45 33 C0           xor         r8d,r8d
  000000006538471A: 41 8D 50 2A        lea         edx,[r8+2Ah]
  000000006538471E: 48 8B CF           mov         rcx,rdi
  0000000065384721: E8 FA 9C CD FF     call        000000006505E420
  0000000065384726: 90                 nop
  0000000065384727: 48 8B C6           mov         rax,rsi
  000000006538472A: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006538472F: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  0000000065384734: 48 83 C4 30        add         rsp,30h
  0000000065384738: 5F                 pop         rdi
  0000000065384739: C3                 ret

  Summary

      695000 .text
