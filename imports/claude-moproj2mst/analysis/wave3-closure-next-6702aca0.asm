Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006702ACA0: 40 53              push        rbx
  000000006702ACA2: 56                 push        rsi
  000000006702ACA3: 57                 push        rdi
  000000006702ACA4: 48 83 EC 50        sub         rsp,50h
  000000006702ACA8: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006702ACB1: E8 4A E9 FF FF     call        0000000067029600
  000000006702ACB6: 8B 0D 2C 5C 2C 00  mov         ecx,dword ptr [00000000672F08E8h]
  000000006702ACBC: FF 15 E6 D3 17 00  call        qword ptr [00000000671A80A8h]
  000000006702ACC2: 48 8B F0           mov         rsi,rax
  000000006702ACC5: 48 85 C0           test        rax,rax
  000000006702ACC8: 0F 85 74 01 00 00  jne         000000006702AE42
  000000006702ACCE: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006702ACD3: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006702ACD8: 8D 48 02           lea         ecx,[rax+2]
  000000006702ACDB: E8 A0 2F FF FF     call        000000006701DC80
  000000006702ACE0: 84 C0              test        al,al
  000000006702ACE2: 74 67              je          000000006702AD4B
  000000006702ACE4: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000006702ACE9: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006702ACED: 48 8B B1 C0 00 00  mov         rsi,qword ptr [rcx+00000000000000C0h]
                    00
  000000006702ACF4: 48 8B D6           mov         rdx,rsi
  000000006702ACF7: 8B 0D EB 5B 2C 00  mov         ecx,dword ptr [00000000672F08E8h]
  000000006702ACFD: FF 15 8D D3 17 00  call        qword ptr [00000000671A8090h]
  000000006702AD03: 4C 8B 5C 24 70     mov         r11,qword ptr [rsp+70h]
  000000006702AD08: 49 8B 43 08        mov         rax,qword ptr [r11+8]
  000000006702AD0C: C6 80 90 00 00 00  mov         byte ptr [rax+0000000000000090h],1
                    01
  000000006702AD13: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000006702AD18: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006702AD1C: C6 81 91 00 00 00  mov         byte ptr [rcx+0000000000000091h],0
                    00
  000000006702AD23: 48 8B 7C 24 70     mov         rdi,qword ptr [rsp+70h]
  000000006702AD28: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  000000006702AD2C: FF 15 4E D6 17 00  call        qword ptr [00000000671A8380h]
  000000006702AD32: 48 89 83 A8 00 00  mov         qword ptr [rbx+00000000000000A8h],rax
                    00
  000000006702AD39: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  000000006702AD3D: FF 15 5D D6 17 00  call        qword ptr [00000000671A83A0h]
  000000006702AD43: 89 83 B0 00 00 00  mov         dword ptr [rbx+00000000000000B0h],eax
  000000006702AD49: EB 74              jmp         000000006702ADBF
  000000006702AD4B: B9 68 00 00 00     mov         ecx,68h
  000000006702AD50: E8 31 DD 14 00     call        0000000067178A86
  000000006702AD55: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006702AD5A: 48 85 C0           test        rax,rax
  000000006702AD5D: 74 17              je          000000006702AD76
  000000006702AD5F: BA 01 00 00 00     mov         edx,1
  000000006702AD64: 48 8B C8           mov         rcx,rax
  000000006702AD67: E8 E4 D4 FF FF     call        0000000067028250
  000000006702AD6C: 48 8B F0           mov         rsi,rax
  000000006702AD6F: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006702AD74: EB 07              jmp         000000006702AD7D
  000000006702AD76: 33 F6              xor         esi,esi
  000000006702AD78: 48 89 74 24 78     mov         qword ptr [rsp+78h],rsi
  000000006702AD7D: 48 8B D6           mov         rdx,rsi
  000000006702AD80: 8B 0D 62 5B 2C 00  mov         ecx,dword ptr [00000000672F08E8h]
  000000006702AD86: FF 15 04 D3 17 00  call        qword ptr [00000000671A8090h]
  000000006702AD8C: 90                 nop
  000000006702AD8D: B9 10 00 00 00     mov         ecx,10h
  000000006702AD92: E8 EF DC 14 00     call        0000000067178A86
  000000006702AD97: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000006702AD9F: 48 85 C0           test        rax,rax
  000000006702ADA2: 74 0D              je          000000006702ADB1
  000000006702ADA4: 48 8B D6           mov         rdx,rsi
  000000006702ADA7: 48 8B C8           mov         rcx,rax
  000000006702ADAA: E8 91 D8 FF FF     call        0000000067028640
  000000006702ADAF: EB 02              jmp         000000006702ADB3
  000000006702ADB1: 33 C0              xor         eax,eax
  000000006702ADB3: 48 89 46 08        mov         qword ptr [rsi+8],rax
  000000006702ADB7: 48 8B CE           mov         rcx,rsi
  000000006702ADBA: E8 51 D8 FF FF     call        0000000067028610
  000000006702ADBF: C6 46 60 01        mov         byte ptr [rsi+60h],1
  000000006702ADC3: FF 15 D7 D5 17 00  call        qword ptr [00000000671A83A0h]
  000000006702ADC9: 44 8B D8           mov         r11d,eax
  000000006702ADCC: 4C 89 5E 10        mov         qword ptr [rsi+10h],r11
  000000006702ADD0: 48 83 3D 58 94 2C  cmp         qword ptr [00000000672F4230h],0
                    00 00
  000000006702ADD8: 75 0D              jne         000000006702ADE7
  000000006702ADDA: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006702ADDE: 48 89 05 4B 94 2C  mov         qword ptr [00000000672F4230h],rax
                    00
  000000006702ADE5: EB 5B              jmp         000000006702AE42
  000000006702ADE7: 48 C7 44 24 78 FF  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFFh
                    FF FF FF
  000000006702ADF0: FF 15 D2 D2 17 00  call        qword ptr [00000000671A80C8h]
  000000006702ADF6: 48 8B F8           mov         rdi,rax
  000000006702ADF9: FF 15 81 D5 17 00  call        qword ptr [00000000671A8380h]
  000000006702ADFF: 48 8B D8           mov         rbx,rax
  000000006702AE02: FF 15 C0 D2 17 00  call        qword ptr [00000000671A80C8h]
  000000006702AE08: 48 8B C8           mov         rcx,rax
  000000006702AE0B: C7 44 24 30 02 00  mov         dword ptr [rsp+30h],2
                    00 00
  000000006702AE13: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000006702AE1B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000006702AE23: 4C 8D 4C 24 78     lea         r9,[rsp+78h]
  000000006702AE28: 4C 8B C7           mov         r8,rdi
  000000006702AE2B: 48 8B D3           mov         rdx,rbx
  000000006702AE2E: FF 15 8C D2 17 00  call        qword ptr [00000000671A80C0h]
  000000006702AE34: 48 8B 56 08        mov         rdx,qword ptr [rsi+8]
  000000006702AE38: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000006702AE3D: E8 1E FB FF FF     call        000000006702A960
  000000006702AE42: 48 8B C6           mov         rax,rsi
  000000006702AE45: 48 83 C4 50        add         rsp,50h
  000000006702AE49: 5F                 pop         rdi
  000000006702AE4A: 5E                 pop         rsi
  000000006702AE4B: 5B                 pop         rbx
  000000006702AE4C: C3                 ret

  Summary

      1A7000 .text
