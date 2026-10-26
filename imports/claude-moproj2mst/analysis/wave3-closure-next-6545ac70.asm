Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006545AC70: 40 53              push        rbx
  000000006545AC72: 55                 push        rbp
  000000006545AC73: 56                 push        rsi
  000000006545AC74: 57                 push        rdi
  000000006545AC75: 41 54              push        r12
  000000006545AC77: 48 83 EC 60        sub         rsp,60h
  000000006545AC7B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006545AC84: 0F B6 F2           movzx       esi,dl
  000000006545AC87: 48 8B F9           mov         rdi,rcx
  000000006545AC8A: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  000000006545AC8E: 84 D2              test        dl,dl
  000000006545AC90: 0F 84 04 02 00 00  je          000000006545AE9A
  000000006545AC96: BA 45 00 00 00     mov         edx,45h
  000000006545AC9B: E8 70 33 C0 FF     call        000000006505E010
  000000006545ACA0: 84 C0              test        al,al
  000000006545ACA2: 74 11              je          000000006545ACB5
  000000006545ACA4: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006545ACA8: 0F B6 48 0A        movzx       ecx,byte ptr [rax+0Ah]
  000000006545ACAC: 80 E1 01           and         cl,1
  000000006545ACAF: 0F 84 FE 02 00 00  je          000000006545AFB3
  000000006545ACB5: BA 2B 00 00 00     mov         edx,2Bh
  000000006545ACBA: 48 8B CF           mov         rcx,rdi
  000000006545ACBD: E8 4E 33 C0 FF     call        000000006505E010
  000000006545ACC2: 84 C0              test        al,al
  000000006545ACC4: 75 6C              jne         000000006545AD32
  000000006545ACC6: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  000000006545ACCE: 48 8B CF           mov         rcx,rdi
  000000006545ACD1: E8 9A 1C C0 FF     call        000000006505C970
  000000006545ACD6: 48 8B CF           mov         rcx,rdi
  000000006545ACD9: FF 15 89 BB 23 00  call        qword ptr [0000000065696868h]
  000000006545ACDF: 48 8B D0           mov         rdx,rax
  000000006545ACE2: 48 8B CF           mov         rcx,rdi
  000000006545ACE5: E8 E6 F4 FF FF     call        000000006545A1D0
  000000006545ACEA: 45 33 C0           xor         r8d,r8d
  000000006545ACED: 41 8D 50 2B        lea         edx,[r8+2Bh]
  000000006545ACF1: 48 8B CF           mov         rcx,rdi
  000000006545ACF4: E8 E7 E2 C0 FF     call        0000000065068FE0
  000000006545ACF9: 8B 9C 24 98 00 00  mov         ebx,dword ptr [rsp+0000000000000098h]
                    00
  000000006545AD00: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000006545AD08: 48 8B CF           mov         rcx,rdi
  000000006545AD0B: E8 60 1C C0 FF     call        000000006505C970
  000000006545AD10: 3B 18              cmp         ebx,dword ptr [rax]
  000000006545AD12: 74 1E              je          000000006545AD32
  000000006545AD14: 8B 84 24 98 00 00  mov         eax,dword ptr [rsp+0000000000000098h]
                    00
  000000006545AD1B: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  000000006545AD22: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000006545AD2A: 48 8B CF           mov         rcx,rdi
  000000006545AD2D: E8 7E D7 C3 FF     call        00000000650984B0
  000000006545AD32: 40 0F B6 D6        movzx       edx,sil
  000000006545AD36: 48 8B CF           mov         rcx,rdi
  000000006545AD39: E8 D2 56 C1 FF     call        0000000065070410
  000000006545AD3E: 41 0F B6 94 24 F8  movzx       edx,byte ptr [r12+00000000000001F8h]
                    01 00 00
  000000006545AD47: 48 8B CF           mov         rcx,rdi
  000000006545AD4A: E8 61 F9 FF FF     call        000000006545A6B0
  000000006545AD4F: 48 8B CF           mov         rcx,rdi
  000000006545AD52: E8 09 1E C0 FF     call        000000006505CB60
  000000006545AD57: 48 8B C8           mov         rcx,rax
  000000006545AD5A: E8 C1 2E C0 FF     call        000000006505DC20
  000000006545AD5F: 48 8B E8           mov         rbp,rax
  000000006545AD62: 48 85 C0           test        rax,rax
  000000006545AD65: 48 0F 44 EF        cmove       rbp,rdi
  000000006545AD69: 49 83 BC 24 E0 01  cmp         qword ptr [r12+00000000000001E0h],0
                    00 00 00
  000000006545AD72: 74 5C              je          000000006545ADD0
  000000006545AD74: 48 8B CD           mov         rcx,rbp
  000000006545AD77: E8 24 16 C0 FF     call        000000006505C3A0
  000000006545AD7C: 85 C0              test        eax,eax
  000000006545AD7E: 75 50              jne         000000006545ADD0
  000000006545AD80: 48 8B DD           mov         rbx,rbp
  000000006545AD83: 48 8B CB           mov         rcx,rbx
  000000006545AD86: E8 15 D2 E4 FF     call        00000000652A7FA0
  000000006545AD8B: 48 8B D8           mov         rbx,rax
  000000006545AD8E: 48 3B C5           cmp         rax,rbp
  000000006545AD91: 74 0C              je          000000006545AD9F
  000000006545AD93: 48 8B C8           mov         rcx,rax
  000000006545AD96: E8 05 16 C0 FF     call        000000006505C3A0
  000000006545AD9B: 85 C0              test        eax,eax
  000000006545AD9D: 74 E4              je          000000006545AD83
  000000006545AD9F: 49 3B 9C 24 E0 01  cmp         rbx,qword ptr [r12+00000000000001E0h]
                    00 00
  000000006545ADA7: 74 27              je          000000006545ADD0
  000000006545ADA9: 48 8B D3           mov         rdx,rbx
  000000006545ADAC: 48 8D 0D DD DF 2A  lea         rcx,[0000000065708D90h]
                    00
  000000006545ADB3: FF 15 4F BC 23 00  call        qword ptr [0000000065696A08h]
  000000006545ADB9: 48 85 C0           test        rax,rax
  000000006545ADBC: 74 12              je          000000006545ADD0
  000000006545ADBE: BA 07 00 00 00     mov         edx,7
  000000006545ADC3: 49 8B 8C 24 E0 01  mov         rcx,qword ptr [r12+00000000000001E0h]
                    00 00
  000000006545ADCB: E8 90 99 C0 FF     call        0000000065064760
  000000006545ADD0: 49 83 BC 24 E0 01  cmp         qword ptr [r12+00000000000001E0h],0
                    00 00 00
  000000006545ADD9: 75 71              jne         000000006545AE4C
  000000006545ADDB: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006545ADDF: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006545ADE3: A8 01              test        al,1
  000000006545ADE5: 74 65              je          000000006545AE4C
  000000006545ADE7: 48 8B CD           mov         rcx,rbp
  000000006545ADEA: E8 B1 D1 E4 FF     call        00000000652A7FA0
  000000006545ADEF: 48 8B D8           mov         rbx,rax
  000000006545ADF2: 48 3B C5           cmp         rax,rbp
  000000006545ADF5: 74 55              je          000000006545AE4C
  000000006545ADF7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  000000006545AE00: 48 8B D3           mov         rdx,rbx
  000000006545AE03: 48 8D 0D 86 DF 2A  lea         rcx,[0000000065708D90h]
                    00
  000000006545AE0A: FF 15 F8 BB 23 00  call        qword ptr [0000000065696A08h]
  000000006545AE10: 48 8B F0           mov         rsi,rax
  000000006545AE13: 48 85 C0           test        rax,rax
  000000006545AE16: 74 18              je          000000006545AE30
  000000006545AE18: 48 8B C8           mov         rcx,rax
  000000006545AE1B: E8 F0 C5 F9 FF     call        00000000653F7410
  000000006545AE20: 84 C0              test        al,al
  000000006545AE22: 74 0C              je          000000006545AE30
  000000006545AE24: 48 8B CE           mov         rcx,rsi
  000000006545AE27: E8 74 15 C0 FF     call        000000006505C3A0
  000000006545AE2C: 85 C0              test        eax,eax
  000000006545AE2E: 75 12              jne         000000006545AE42
  000000006545AE30: 48 8B CB           mov         rcx,rbx
  000000006545AE33: E8 68 D1 E4 FF     call        00000000652A7FA0
  000000006545AE38: 48 8B D8           mov         rbx,rax
  000000006545AE3B: 48 3B C5           cmp         rax,rbp
  000000006545AE3E: 74 0C              je          000000006545AE4C
  000000006545AE40: EB BE              jmp         000000006545AE00
  000000006545AE42: B2 01              mov         dl,1
  000000006545AE44: 48 8B CE           mov         rcx,rsi
  000000006545AE47: E8 44 C6 F9 FF     call        00000000653F7490
  000000006545AE4C: 48 85 ED           test        rbp,rbp
  000000006545AE4F: 74 39              je          000000006545AE8A
  000000006545AE51: 48 8B CD           mov         rcx,rbp
  000000006545AE54: E8 47 4C C0 FF     call        000000006505FAA0
  000000006545AE59: 84 C0              test        al,al
  000000006545AE5B: 75 2D              jne         000000006545AE8A
  000000006545AE5D: BA 08 00 00 00     mov         edx,8
  000000006545AE62: 44 8D 42 F9        lea         r8d,[rdx-7]
  000000006545AE66: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006545AE6B: E8 30 70 BD FF     call        0000000065031EA0
  000000006545AE70: 90                 nop
  000000006545AE71: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006545AE76: 48 8B CD           mov         rcx,rbp
  000000006545AE79: FF 15 91 B9 23 00  call        qword ptr [0000000065696810h]
  000000006545AE7F: 90                 nop
  000000006545AE80: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006545AE85: E8 56 70 BD FF     call        0000000065031EE0
  000000006545AE8A: 33 D2              xor         edx,edx
  000000006545AE8C: 44 8D 42 10        lea         r8d,[rdx+10h]
  000000006545AE90: 48 8B CF           mov         rcx,rdi
  000000006545AE93: E8 C8 7A 04 00     call        00000000654A2960
  000000006545AE98: EB 58              jmp         000000006545AEF2
  000000006545AE9A: BA 45 00 00 00     mov         edx,45h
  000000006545AE9F: E8 6C 31 C0 FF     call        000000006505E010
  000000006545AEA4: 84 C0              test        al,al
  000000006545AEA6: 74 0E              je          000000006545AEB6
  000000006545AEA8: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006545AEAC: F6 40 0A 01        test        byte ptr [rax+0Ah],1
  000000006545AEB0: 0F 85 FD 00 00 00  jne         000000006545AFB3
  000000006545AEB6: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006545AEBA: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006545AEBD: C1 E9 0F           shr         ecx,0Fh
  000000006545AEC0: F6 C1 01           test        cl,1
  000000006545AEC3: 74 0E              je          000000006545AED3
  000000006545AEC5: 33 D2              xor         edx,edx
  000000006545AEC7: 44 8D 42 11        lea         r8d,[rdx+11h]
  000000006545AECB: 48 8B CF           mov         rcx,rdi
  000000006545AECE: E8 8D 7A 04 00     call        00000000654A2960
  000000006545AED3: 33 D2              xor         edx,edx
  000000006545AED5: 48 8B CF           mov         rcx,rdi
  000000006545AED8: E8 33 55 C1 FF     call        0000000065070410
  000000006545AEDD: 49 8B 8C 24 38 02  mov         rcx,qword ptr [r12+0000000000000238h]
                    00 00
  000000006545AEE5: 48 85 C9           test        rcx,rcx
  000000006545AEE8: 74 08              je          000000006545AEF2
  000000006545AEEA: 33 D2              xor         edx,edx
  000000006545AEEC: FF 15 2E DE 23 00  call        qword ptr [0000000065698D20h]
  000000006545AEF2: 49 83 BC 24 E0 01  cmp         qword ptr [r12+00000000000001E0h],0
                    00 00 00
  000000006545AEFB: 0F 84 B2 00 00 00  je          000000006545AFB3
  000000006545AF01: 48 8B CF           mov         rcx,rdi
  000000006545AF04: E8 C7 6D C0 FF     call        0000000065061CD0
  000000006545AF09: 84 C0              test        al,al
  000000006545AF0B: 0F 84 A2 00 00 00  je          000000006545AFB3
  000000006545AF11: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  000000006545AF1C: 45 33 C9           xor         r9d,r9d
  000000006545AF1F: 4C 8D 84 24 98 00  lea         r8,[rsp+0000000000000098h]
                    00 00
  000000006545AF27: 33 D2              xor         edx,edx
  000000006545AF29: 8D 4A 5F           lea         ecx,[rdx+5Fh]
  000000006545AF2C: FF 15 B6 EB 23 00  call        qword ptr [0000000065699AE8h]
  000000006545AF32: 85 C0              test        eax,eax
  000000006545AF34: 74 7D              je          000000006545AFB3
  000000006545AF36: 83 BC 24 98 00 00  cmp         dword ptr [rsp+0000000000000098h],0
                    00 00
  000000006545AF3E: 74 73              je          000000006545AFB3
  000000006545AF40: 49 8B BC 24 E0 01  mov         rdi,qword ptr [r12+00000000000001E0h]
                    00 00
  000000006545AF48: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  000000006545AF4C: 48 83 C1 14        add         rcx,14h
  000000006545AF50: FF 15 D2 B7 23 00  call        qword ptr [0000000065696728h]
  000000006545AF56: 8B D8              mov         ebx,eax
  000000006545AF58: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  000000006545AF5C: 48 83 C1 14        add         rcx,14h
  000000006545AF60: FF 15 FA B8 23 00  call        qword ptr [0000000065696860h]
  000000006545AF66: 44 8B C8           mov         r9d,eax
  000000006545AF69: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  000000006545AF6D: 45 33 C0           xor         r8d,r8d
  000000006545AF70: 33 D2              xor         edx,edx
  000000006545AF72: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006545AF77: FF 15 F3 B7 23 00  call        qword ptr [0000000065696770h]
  000000006545AF7D: 49 8B 9C 24 E0 01  mov         rbx,qword ptr [r12+00000000000001E0h]
                    00 00
  000000006545AF85: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000006545AF8D: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006545AF92: FF 15 18 C5 23 00  call        qword ptr [00000000656974B0h]
  000000006545AF98: 4C 8B C0           mov         r8,rax
  000000006545AF9B: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006545AFA3: 48 8B CB           mov         rcx,rbx
  000000006545AFA6: E8 B5 CE C3 FF     call        0000000065097E60
  000000006545AFAB: 48 8B C8           mov         rcx,rax
  000000006545AFAE: E8 2D FE F7 FF     call        00000000653DADE0
  000000006545AFB3: 48 83 C4 60        add         rsp,60h
  000000006545AFB7: 41 5C              pop         r12
  000000006545AFB9: 5F                 pop         rdi
  000000006545AFBA: 5E                 pop         rsi
  000000006545AFBB: 5D                 pop         rbp
  000000006545AFBC: 5B                 pop         rbx
  000000006545AFBD: C3                 ret

  Summary

      695000 .text
