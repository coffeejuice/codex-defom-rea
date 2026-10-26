Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file analysis\resume48-distortion-vt198-rea-session\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005A9B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005A9B5: 56                 push        rsi
  000000014005A9B6: 57                 push        rdi
  000000014005A9B7: B8 78 35 00 00     mov         eax,3578h
  000000014005A9BC: E8 9F AE 79 01     call        00000001417F5860
  000000014005A9C1: 48 2B E0           sub         rsp,rax
  000000014005A9C4: 48 C7 84 24 10 35  mov         qword ptr [rsp+0000000000003510h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014005A9D0: 48 8B 05 59 36 B0  mov         rax,qword ptr [0000000143B5E030h]
                    03
  000000014005A9D7: 48 33 C4           xor         rax,rsp
  000000014005A9DA: 48 89 84 24 60 35  mov         qword ptr [rsp+0000000000003560h],rax
                    00 00
  000000014005A9E2: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  000000014005A9E7: 48 8B 8C 24 90 35  mov         rcx,qword ptr [rsp+0000000000003590h]
                    00 00
  000000014005A9EF: E8 AC 4D 00 00     call        000000014005F7A0
  000000014005A9F4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014005A9F9: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014005A9FF: 75 07              jne         000000014005AA08
  000000014005AA01: 32 C0              xor         al,al
  000000014005AA03: E9 96 04 00 00     jmp         000000014005AE9E
  000000014005AA08: 48 8B 8C 24 90 35  mov         rcx,qword ptr [rsp+0000000000003590h]
                    00 00
  000000014005AA10: E8 7B AD 00 00     call        0000000140065790
  000000014005AA15: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014005AA1A: 48 8B 8C 24 90 35  mov         rcx,qword ptr [rsp+0000000000003590h]
                    00 00
  000000014005AA22: E8 09 A6 FF FF     call        0000000140055030
  000000014005AA27: 48 8B C8           mov         rcx,rax
  000000014005AA2A: E8 81 AF 04 00     call        00000001400A59B0
  000000014005AA2F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014005AA34: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  000000014005AA3A: 0F 84 5A 04 00 00  je          000000014005AE9A
  000000014005AA40: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  000000014005AA46: 0F 84 4E 04 00 00  je          000000014005AE9A
  000000014005AA4C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005AA51: E8 2A AD 00 00     call        0000000140065780
  000000014005AA56: 0F B6 C0           movzx       eax,al
  000000014005AA59: 85 C0              test        eax,eax
  000000014005AA5B: 0F 84 39 04 00 00  je          000000014005AE9A
  000000014005AA61: 48 8B 8C 24 90 35  mov         rcx,qword ptr [rsp+0000000000003590h]
                    00 00
  000000014005AA69: E8 82 AD 00 00     call        00000001400657F0
  000000014005AA6E: 89 84 24 98 34 00  mov         dword ptr [rsp+0000000000003498h],eax
                    00
  000000014005AA75: 48 C7 84 24 A0 34  mov         qword ptr [rsp+00000000000034A0h],0
                    00 00 00 00 00 00
  000000014005AA81: BA 03 00 00 00     mov         edx,3
  000000014005AA86: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014005AA8B: E8 60 71 7C 00     call        0000000140821BF0
  000000014005AA90: 90                 nop
  000000014005AA91: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005AA96: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  000000014005AA9D: E8 1E C2 88 00     call        00000001408E6CC0
  000000014005AAA2: 83 BC 24 98 34 00  cmp         dword ptr [rsp+0000000000003498h],0
                    00 00
  000000014005AAAA: 0F 8E B6 00 00 00  jle         000000014005AB66
  000000014005AAB0: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014005AAB5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014005AAB8: 4C 8D 84 24 98 34  lea         r8,[rsp+0000000000003498h]
                    00 00
  000000014005AAC0: 48 8D 94 24 A8 34  lea         rdx,[rsp+00000000000034A8h]
                    00 00
  000000014005AAC8: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014005AACD: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  000000014005AAD3: 90                 nop
  000000014005AAD4: 48 8D 94 24 E8 34  lea         rdx,[rsp+00000000000034E8h]
                    00 00
  000000014005AADC: 48 8D 8C 24 A8 34  lea         rcx,[rsp+00000000000034A8h]
                    00 00
  000000014005AAE4: FF 15 66 94 DA 01  call        qword ptr [0000000141E03F50h]
  000000014005AAEA: 48 89 84 24 18 35  mov         qword ptr [rsp+0000000000003518h],rax
                    00 00
  000000014005AAF2: 48 8B 84 24 18 35  mov         rax,qword ptr [rsp+0000000000003518h]
                    00 00
  000000014005AAFA: 48 89 84 24 20 35  mov         qword ptr [rsp+0000000000003520h],rax
                    00 00
  000000014005AB02: 48 8B 8C 24 20 35  mov         rcx,qword ptr [rsp+0000000000003520h]
                    00 00
  000000014005AB0A: FF 15 48 94 DA 01  call        qword ptr [0000000141E03F58h]
  000000014005AB10: 48 8B D0           mov         rdx,rax
  000000014005AB13: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014005AB18: E8 B3 D4 83 00     call        0000000140897FD0
  000000014005AB1D: 88 84 24 E0 34 00  mov         byte ptr [rsp+00000000000034E0h],al
                    00
  000000014005AB24: 48 8D 8C 24 E8 34  lea         rcx,[rsp+00000000000034E8h]
                    00 00
  000000014005AB2C: FF 15 5E 97 DA 01  call        qword ptr [0000000141E04290h]
  000000014005AB32: 0F B6 94 24 E0 34  movzx       edx,byte ptr [rsp+00000000000034E0h]
                    00 00
  000000014005AB3A: 85 D2              test        edx,edx
  000000014005AB3C: 74 1A              je          000000014005AB58
  000000014005AB3E: BA 01 00 00 00     mov         edx,1
  000000014005AB43: 48 8D 8C 24 28 14  lea         rcx,[rsp+0000000000001428h]
                    00 00
  000000014005AB4B: E8 20 C0 88 00     call        00000001408E6B70
  000000014005AB50: 48 89 84 24 A0 34  mov         qword ptr [rsp+00000000000034A0h],rax
                    00 00
  000000014005AB58: 48 8D 8C 24 A8 34  lea         rcx,[rsp+00000000000034A8h]
                    00 00
  000000014005AB60: FF 15 7A AE DA 01  call        qword ptr [0000000141E059E0h]
  000000014005AB66: 48 8B 8C 24 90 35  mov         rcx,qword ptr [rsp+0000000000003590h]
                    00 00
  000000014005AB6E: E8 1D A3 FF FF     call        0000000140054E90
  000000014005AB73: 48 89 84 24 28 35  mov         qword ptr [rsp+0000000000003528h],rax
                    00 00
  000000014005AB7B: 48 8B 84 24 28 35  mov         rax,qword ptr [rsp+0000000000003528h]
                    00 00
  000000014005AB83: 48 8B 00           mov         rax,qword ptr [rax]
  000000014005AB86: 48 8D 94 24 90 34  lea         rdx,[rsp+0000000000003490h]
                    00 00
  000000014005AB8E: 48 8B 8C 24 28 35  mov         rcx,qword ptr [rsp+0000000000003528h]
                    00 00
  000000014005AB96: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000014005AB9C: 90                 nop
  000000014005AB9D: 48 8D 8C 24 70 34  lea         rcx,[rsp+0000000000003470h]
                    00 00
  000000014005ABA5: E8 F6 AB 00 00     call        00000001400657A0
  000000014005ABAA: 48 8D 94 24 90 34  lea         rdx,[rsp+0000000000003490h]
                    00 00
  000000014005ABB2: 48 8D 8C 24 B0 34  lea         rcx,[rsp+00000000000034B0h]
                    00 00
  000000014005ABBA: E8 61 C6 00 00     call        0000000140067220
  000000014005ABBF: 90                 nop
  000000014005ABC0: 48 8D 84 24 B0 34  lea         rax,[rsp+00000000000034B0h]
                    00 00
  000000014005ABC8: 48 89 84 24 D0 34  mov         qword ptr [rsp+00000000000034D0h],rax
                    00 00
  000000014005ABD0: EB 4B              jmp         000000014005AC1D
  000000014005ABD2: 33 C0              xor         eax,eax
  000000014005ABD4: 83 F8 01           cmp         eax,1
  000000014005ABD7: 74 0E              je          000000014005ABE7
  000000014005ABD9: 48 C7 84 24 30 35  mov         qword ptr [rsp+0000000000003530h],0
                    00 00 00 00 00 00
  000000014005ABE5: EB 15              jmp         000000014005ABFC
  000000014005ABE7: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005ABEF: E8 5C A0 12 00     call        0000000140184C50
  000000014005ABF4: 48 89 84 24 30 35  mov         qword ptr [rsp+0000000000003530h],rax
                    00 00
  000000014005ABFC: 48 8B 94 24 30 35  mov         rdx,qword ptr [rsp+0000000000003530h]
                    00 00
  000000014005AC04: 48 8B 8C 24 D0 34  mov         rcx,qword ptr [rsp+00000000000034D0h]
                    00 00
  000000014005AC0C: E8 6F 17 DA 00     call        0000000140DFC380
  000000014005AC11: 48 8B C8           mov         rcx,rax
  000000014005AC14: 48 83 C1 10        add         rcx,10h
  000000014005AC18: E8 E3 EB D8 00     call        0000000140DE9800
  000000014005AC1D: 33 C0              xor         eax,eax
  000000014005AC1F: 83 F8 01           cmp         eax,1
  000000014005AC22: 74 0E              je          000000014005AC32
  000000014005AC24: 48 C7 84 24 38 35  mov         qword ptr [rsp+0000000000003538h],0
                    00 00 00 00 00 00
  000000014005AC30: EB 15              jmp         000000014005AC47
  000000014005AC32: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005AC3A: E8 11 A0 12 00     call        0000000140184C50
  000000014005AC3F: 48 89 84 24 38 35  mov         qword ptr [rsp+0000000000003538h],rax
                    00 00
  000000014005AC47: 48 8B 94 24 38 35  mov         rdx,qword ptr [rsp+0000000000003538h]
                    00 00
  000000014005AC4F: 48 8B 8C 24 D0 34  mov         rcx,qword ptr [rsp+00000000000034D0h]
                    00 00
  000000014005AC57: E8 24 17 DA 00     call        0000000140DFC380
  000000014005AC5C: 48 8B C8           mov         rcx,rax
  000000014005AC5F: E8 1C 14 DA 00     call        0000000140DFC080
  000000014005AC64: 0F B6 C0           movzx       eax,al
  000000014005AC67: 85 C0              test        eax,eax
  000000014005AC69: 0F 84 05 02 00 00  je          000000014005AE74
  000000014005AC6F: 33 C0              xor         eax,eax
  000000014005AC71: 83 F8 01           cmp         eax,1
  000000014005AC74: 74 0E              je          000000014005AC84
  000000014005AC76: 48 C7 84 24 40 35  mov         qword ptr [rsp+0000000000003540h],0
                    00 00 00 00 00 00
  000000014005AC82: EB 15              jmp         000000014005AC99
  000000014005AC84: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005AC8C: E8 BF 9F 12 00     call        0000000140184C50
  000000014005AC91: 48 89 84 24 40 35  mov         qword ptr [rsp+0000000000003540h],rax
                    00 00
  000000014005AC99: 48 8B 94 24 40 35  mov         rdx,qword ptr [rsp+0000000000003540h]
                    00 00
  000000014005ACA1: 48 8B 8C 24 D0 34  mov         rcx,qword ptr [rsp+00000000000034D0h]
                    00 00
  000000014005ACA9: E8 D2 16 DA 00     call        0000000140DFC380
  000000014005ACAE: 48 8B C8           mov         rcx,rax
  000000014005ACB1: 48 83 C1 10        add         rcx,10h
  000000014005ACB5: E8 F6 C8 D7 00     call        0000000140DD75B0
  000000014005ACBA: 48 8D BC 24 F0 34  lea         rdi,[rsp+00000000000034F0h]
                    00 00
  000000014005ACC2: 48 8B F0           mov         rsi,rax
  000000014005ACC5: B9 20 00 00 00     mov         ecx,20h
  000000014005ACCA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014005ACCC: 48 8D BC 24 70 34  lea         rdi,[rsp+0000000000003470h]
                    00 00
  000000014005ACD4: 48 8D B4 24 F0 34  lea         rsi,[rsp+00000000000034F0h]
                    00 00
  000000014005ACDC: B9 20 00 00 00     mov         ecx,20h
  000000014005ACE1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014005ACE3: EB 62              jmp         000000014005AD47
  000000014005ACE5: 33 C0              xor         eax,eax
  000000014005ACE7: 83 F8 01           cmp         eax,1
  000000014005ACEA: 74 0E              je          000000014005ACFA
  000000014005ACEC: 48 C7 84 24 48 35  mov         qword ptr [rsp+0000000000003548h],0
                    00 00 00 00 00 00
  000000014005ACF8: EB 15              jmp         000000014005AD0F
  000000014005ACFA: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005AD02: E8 49 9F 12 00     call        0000000140184C50
  000000014005AD07: 48 89 84 24 48 35  mov         qword ptr [rsp+0000000000003548h],rax
                    00 00
  000000014005AD0F: 48 8B 94 24 48 35  mov         rdx,qword ptr [rsp+0000000000003548h]
                    00 00
  000000014005AD17: 48 8B 8C 24 D0 34  mov         rcx,qword ptr [rsp+00000000000034D0h]
                    00 00
  000000014005AD1F: E8 5C 16 DA 00     call        0000000140DFC380
  000000014005AD24: 48 83 C0 08        add         rax,8
  000000014005AD28: 48 89 84 24 50 35  mov         qword ptr [rsp+0000000000003550h],rax
                    00 00
  000000014005AD30: 48 8B 84 24 50 35  mov         rax,qword ptr [rsp+0000000000003550h]
                    00 00
  000000014005AD38: 8B 08              mov         ecx,dword ptr [rax]
  000000014005AD3A: 83 E9 01           sub         ecx,1
  000000014005AD3D: 48 8B 84 24 50 35  mov         rax,qword ptr [rsp+0000000000003550h]
                    00 00
  000000014005AD45: 89 08              mov         dword ptr [rax],ecx
  000000014005AD47: 33 C0              xor         eax,eax
  000000014005AD49: 83 F8 01           cmp         eax,1
  000000014005AD4C: 74 0E              je          000000014005AD5C
  000000014005AD4E: 48 C7 84 24 58 35  mov         qword ptr [rsp+0000000000003558h],0
                    00 00 00 00 00 00
  000000014005AD5A: EB 15              jmp         000000014005AD71
  000000014005AD5C: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005AD64: E8 E7 9E 12 00     call        0000000140184C50
  000000014005AD69: 48 89 84 24 58 35  mov         qword ptr [rsp+0000000000003558h],rax
                    00 00
  000000014005AD71: 48 8B 94 24 58 35  mov         rdx,qword ptr [rsp+0000000000003558h]
                    00 00
  000000014005AD79: 48 8B 8C 24 D0 34  mov         rcx,qword ptr [rsp+00000000000034D0h]
                    00 00
  000000014005AD81: E8 FA 15 DA 00     call        0000000140DFC380
  000000014005AD86: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000014005AD8A: 0F 84 DF 00 00 00  je          000000014005AE6F
  000000014005AD90: 8B 94 24 74 34 00  mov         edx,dword ptr [rsp+0000000000003474h]
                    00
  000000014005AD97: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005AD9C: E8 CF 54 7D 00     call        0000000140830270
  000000014005ADA1: 48 89 84 24 D8 34  mov         qword ptr [rsp+00000000000034D8h],rax
                    00 00
  000000014005ADA9: 48 83 BC 24 D8 34  cmp         qword ptr [rsp+00000000000034D8h],0
                    00 00 00
  000000014005ADB2: 74 29              je          000000014005ADDD
  000000014005ADB4: 4C 8B 4C 24 30     mov         r9,qword ptr [rsp+30h]
  000000014005ADB9: 49 81 C1 40 16 00  add         r9,1640h
                    00
  000000014005ADC0: 4C 8D 84 24 88 34  lea         r8,[rsp+0000000000003488h]
                    00 00
  000000014005ADC8: 48 8D 94 24 70 34  lea         rdx,[rsp+0000000000003470h]
                    00 00
  000000014005ADD0: 48 8B 8C 24 D8 34  mov         rcx,qword ptr [rsp+00000000000034D8h]
                    00 00
  000000014005ADD8: E8 43 AC 81 00     call        0000000140875A20
  000000014005ADDD: 48 83 BC 24 A0 34  cmp         qword ptr [rsp+00000000000034A0h],0
                    00 00 00
  000000014005ADE6: 74 3A              je          000000014005AE22
  000000014005ADE8: 0F B6 84 24 89 34  movzx       eax,byte ptr [rsp+0000000000003489h]
                    00 00
  000000014005ADF0: 88 44 24 20        mov         byte ptr [rsp+20h],al
  000000014005ADF4: 4C 8B 8C 24 A0 34  mov         r9,qword ptr [rsp+00000000000034A0h]
                    00 00
  000000014005ADFC: 4C 8D 84 24 74 34  lea         r8,[rsp+0000000000003474h]
                    00 00
  000000014005AE04: 48 8D 94 24 70 34  lea         rdx,[rsp+0000000000003470h]
                    00 00
  000000014005AE0C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005AE11: E8 2A 37 30 00     call        000000014035E540
  000000014005AE16: 48 85 C0           test        rax,rax
  000000014005AE19: 74 05              je          000000014005AE20
  000000014005AE1B: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  000000014005AE20: EB 48              jmp         000000014005AE6A
  000000014005AE22: 0F B6 84 24 89 34  movzx       eax,byte ptr [rsp+0000000000003489h]
                    00 00
  000000014005AE2A: 88 44 24 28        mov         byte ptr [rsp+28h],al
  000000014005AE2E: F2 0F 10 84 24 80  movsd       xmm0,mmword ptr [rsp+0000000000003480h]
                    34 00 00
  000000014005AE37: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014005AE3D: F2 0F 10 9C 24 78  movsd       xmm3,mmword ptr [rsp+0000000000003478h]
                    34 00 00
  000000014005AE46: 4C 8D 84 24 74 34  lea         r8,[rsp+0000000000003474h]
                    00 00
  000000014005AE4E: 48 8D 94 24 70 34  lea         rdx,[rsp+0000000000003470h]
                    00 00
  000000014005AE56: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005AE5B: E8 90 38 30 00     call        000000014035E6F0
  000000014005AE60: 48 85 C0           test        rax,rax
  000000014005AE63: 74 05              je          000000014005AE6A
  000000014005AE65: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  000000014005AE6A: E9 76 FE FF FF     jmp         000000014005ACE5
  000000014005AE6F: E9 5E FD FF FF     jmp         000000014005ABD2
  000000014005AE74: 48 8D 8C 24 B0 34  lea         rcx,[rsp+00000000000034B0h]
                    00 00
  000000014005AE7C: E8 4F 0D 10 00     call        000000014015BBD0
  000000014005AE81: 90                 nop
  000000014005AE82: 48 8D 8C 24 90 34  lea         rcx,[rsp+0000000000003490h]
                    00 00
  000000014005AE8A: E8 A1 51 58 00     call        00000001405E0030
  000000014005AE8F: 90                 nop
  000000014005AE90: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014005AE95: E8 C6 96 7C 00     call        0000000140824560
  000000014005AE9A: 8A 44 24 38        mov         al,byte ptr [rsp+38h]
  000000014005AE9E: 48 8B 8C 24 60 35  mov         rcx,qword ptr [rsp+0000000000003560h]
                    00 00
  000000014005AEA6: 48 33 CC           xor         rcx,rsp
  000000014005AEA9: E8 62 30 41 01     call        000000014146DF10
  000000014005AEAE: 48 81 C4 78 35 00  add         rsp,3578h
                    00
  000000014005AEB5: 5F                 pop         rdi
  000000014005AEB6: 5E                 pop         rsi
  000000014005AEB7: C3                 ret

  Summary

     1E02000 .text
