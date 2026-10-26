Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2C40: 40 55              push        rbp
  00000000670C2C42: 56                 push        rsi
  00000000670C2C43: 57                 push        rdi
  00000000670C2C44: 41 54              push        r12
  00000000670C2C46: 41 55              push        r13
  00000000670C2C48: B8 70 40 00 00     mov         eax,4070h
  00000000670C2C4D: E8 1E 69 0B 00     call        0000000067179570
  00000000670C2C52: 48 2B E0           sub         rsp,rax
  00000000670C2C55: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C2C5E: 48 89 9C 24 B0 40  mov         qword ptr [rsp+00000000000040B0h],rbx
                    00 00
  00000000670C2C66: 48 8B 05 33 0F 23  mov         rax,qword ptr [00000000672F3BA0h]
                    00
  00000000670C2C6D: 48 33 C4           xor         rax,rsp
  00000000670C2C70: 48 89 84 24 60 40  mov         qword ptr [rsp+0000000000004060h],rax
                    00 00
  00000000670C2C78: 48 8B DA           mov         rbx,rdx
  00000000670C2C7B: 48 8B E9           mov         rbp,rcx
  00000000670C2C7E: 48 89 54 24 38     mov         qword ptr [rsp+38h],rdx
  00000000670C2C83: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670C2C86: E8 45 77 FF FF     call        00000000670BA3D0
  00000000670C2C8B: 0F B6 F8           movzx       edi,al
  00000000670C2C8E: 84 C0              test        al,al
  00000000670C2C90: 74 0B              je          00000000670C2C9D
  00000000670C2C92: 33 D2              xor         edx,edx
  00000000670C2C94: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2C98: E8 A3 95 FF FF     call        00000000670BC240
  00000000670C2C9D: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2CA1: 48 8B 11           mov         rdx,qword ptr [rcx]
  00000000670C2CA4: FF 52 58           call        qword ptr [rdx+58h]
  00000000670C2CA7: 84 C0              test        al,al
  00000000670C2CA9: 74 74              je          00000000670C2D1F
  00000000670C2CAB: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  00000000670C2CAF: 48 8D 0D 5A 36 19  lea         rcx,[0000000067256310h]
                    00
  00000000670C2CB6: E8 15 B1 05 00     call        000000006711DDD0
  00000000670C2CBB: 48 85 C0           test        rax,rax
  00000000670C2CBE: 74 5F              je          00000000670C2D1F
  00000000670C2CC0: 48 8B C8           mov         rcx,rax
  00000000670C2CC3: E8 F8 1A FF FF     call        00000000670B47C0
  00000000670C2CC8: 85 C0              test        eax,eax
  00000000670C2CCA: 75 53              jne         00000000670C2D1F
  00000000670C2CCC: 48 83 FB FF        cmp         rbx,0FFFFFFFFFFFFFFFFh
  00000000670C2CD0: 74 34              je          00000000670C2D06
  00000000670C2CD2: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],4000h
                    40 00 00
  00000000670C2CDB: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  00000000670C2CE0: 48 8D 44 24 38     lea         rax,[rsp+38h]
  00000000670C2CE5: 48 81 FB 00 40 00  cmp         rbx,4000h
                    00
  00000000670C2CEC: 4C 0F 4E C0        cmovle      r8,rax
  00000000670C2CF0: 4D 8B 00           mov         r8,qword ptr [r8]
  00000000670C2CF3: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670C2CF8: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2CFC: E8 DF 81 FF FF     call        00000000670BAEE0
  00000000670C2D01: 48 8B D8           mov         rbx,rax
  00000000670C2D04: EB 6A              jmp         00000000670C2D70
  00000000670C2D06: 41 B8 00 40 00 00  mov         r8d,4000h
  00000000670C2D0C: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670C2D11: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2D15: E8 C6 81 FF FF     call        00000000670BAEE0
  00000000670C2D1A: 48 8B D8           mov         rbx,rax
  00000000670C2D1D: EB 51              jmp         00000000670C2D70
  00000000670C2D1F: 48 83 FB FF        cmp         rbx,0FFFFFFFFFFFFFFFFh
  00000000670C2D23: 74 34              je          00000000670C2D59
  00000000670C2D25: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],4000h
                    40 00 00
  00000000670C2D2E: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  00000000670C2D33: 48 8D 44 24 38     lea         rax,[rsp+38h]
  00000000670C2D38: 48 81 FB 00 40 00  cmp         rbx,4000h
                    00
  00000000670C2D3F: 4C 0F 4E C0        cmovle      r8,rax
  00000000670C2D43: 4D 8B 00           mov         r8,qword ptr [r8]
  00000000670C2D46: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670C2D4B: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2D4F: E8 1C 7A FF FF     call        00000000670BA770
  00000000670C2D54: 48 8B D8           mov         rbx,rax
  00000000670C2D57: EB 17              jmp         00000000670C2D70
  00000000670C2D59: 41 B8 00 40 00 00  mov         r8d,4000h
  00000000670C2D5F: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670C2D64: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2D68: E8 03 7A FF FF     call        00000000670BA770
  00000000670C2D6D: 48 8B D8           mov         rbx,rax
  00000000670C2D70: 48 83 7D 38 00     cmp         qword ptr [rbp+38h],0
  00000000670C2D75: 74 09              je          00000000670C2D80
  00000000670C2D77: 80 BD 88 00 00 00  cmp         byte ptr [rbp+0000000000000088h],0
                    00
  00000000670C2D7E: 74 53              je          00000000670C2DD3
  00000000670C2D80: C6 85 88 00 00 00  mov         byte ptr [rbp+0000000000000088h],0
                    00
  00000000670C2D87: 44 8B C3           mov         r8d,ebx
  00000000670C2D8A: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670C2D8F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670C2D94: E8 77 A3 F6 FF     call        000000006702D110
  00000000670C2D99: 90                 nop
  00000000670C2D9A: 48 8B 55 38        mov         rdx,qword ptr [rbp+38h]
  00000000670C2D9E: 48 8B C8           mov         rcx,rax
  00000000670C2DA1: E8 AA B2 08 00     call        000000006714E050
  00000000670C2DA6: 48 89 45 38        mov         qword ptr [rbp+38h],rax
  00000000670C2DAA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670C2DAF: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C2DB3: 75 0A              jne         00000000670C2DBF
  00000000670C2DB5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670C2DBA: E8 F1 D5 F5 FF     call        00000000670203B0
  00000000670C2DBF: 48 83 7D 38 00     cmp         qword ptr [rbp+38h],0
  00000000670C2DC4: 75 0D              jne         00000000670C2DD3
  00000000670C2DC6: E8 A5 B1 08 00     call        000000006714DF70
  00000000670C2DCB: 48 89 45 38        mov         qword ptr [rbp+38h],rax
  00000000670C2DCF: 83 4D 60 01        or          dword ptr [rbp+60h],1
  00000000670C2DD3: 48 85 DB           test        rbx,rbx
  00000000670C2DD6: 7F 07              jg          00000000670C2DDF
  00000000670C2DD8: 32 C0              xor         al,al
  00000000670C2DDA: E9 44 01 00 00     jmp         00000000670C2F23
  00000000670C2DDF: 48 8B 85 98 00 00  mov         rax,qword ptr [rbp+0000000000000098h]
                    00
  00000000670C2DE6: 4C 63 60 08        movsxd      r12,dword ptr [rax+8]
  00000000670C2DEA: 48 8D 45 40        lea         rax,[rbp+40h]
  00000000670C2DEE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670C2DF3: 44 8B CB           mov         r9d,ebx
  00000000670C2DF6: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000000670C2DFB: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670C2E00: 48 8B 4D 38        mov         rcx,qword ptr [rbp+38h]
  00000000670C2E04: E8 57 F6 FA FF     call        0000000067072460
  00000000670C2E09: 90                 nop
  00000000670C2E0A: 48 8B D0           mov         rdx,rax
  00000000670C2E0D: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2E14: E8 F7 D5 FA FF     call        0000000067070410
  00000000670C2E19: 90                 nop
  00000000670C2E1A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670C2E1F: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C2E23: 75 0A              jne         00000000670C2E2F
  00000000670C2E25: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670C2E2A: E8 A1 D2 FA FF     call        00000000670700D0
  00000000670C2E2F: 40 84 FF           test        dil,dil
  00000000670C2E32: 74 0B              je          00000000670C2E3F
  00000000670C2E34: B2 01              mov         dl,1
  00000000670C2E36: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670C2E3A: E8 01 94 FF FF     call        00000000670BC240
  00000000670C2E3F: 48 8B 85 98 00 00  mov         rax,qword ptr [rbp+0000000000000098h]
                    00
  00000000670C2E46: 44 39 60 08        cmp         dword ptr [rax+8],r12d
  00000000670C2E4A: 0F 8E D1 00 00 00  jle         00000000670C2F21
  00000000670C2E50: 40 84 FF           test        dil,dil
  00000000670C2E53: 0F 84 C8 00 00 00  je          00000000670C2F21
  00000000670C2E59: 49 8B DC           mov         rbx,r12
  00000000670C2E5C: 48 03 DB           add         rbx,rbx
  00000000670C2E5F: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2E66: E8 A5 2B 0B 00     call        0000000067175A10
  00000000670C2E6B: 48 8D 34 03        lea         rsi,[rbx+rax]
  00000000670C2E6F: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2E76: E8 95 2B 0B 00     call        0000000067175A10
  00000000670C2E7B: 48 8D 3C 03        lea         rdi,[rbx+rax]
  00000000670C2E7F: 48 8B 8D 98 00 00  mov         rcx,qword ptr [rbp+0000000000000098h]
                    00
  00000000670C2E86: 48 63 59 08        movsxd      rbx,dword ptr [rcx+8]
  00000000670C2E8A: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2E91: E8 7A 2B 0B 00     call        0000000067175A10
  00000000670C2E96: 48 8D 14 58        lea         rdx,[rax+rbx*2]
  00000000670C2E9A: 48 3B FA           cmp         rdi,rdx
  00000000670C2E9D: 73 62              jae         00000000670C2F01
  00000000670C2E9F: 66 83 3F 0D        cmp         word ptr [rdi],0Dh
  00000000670C2EA3: 74 21              je          00000000670C2EC6
  00000000670C2EA5: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000000670C2EB0: 48 83 C7 02        add         rdi,2
  00000000670C2EB4: 41 FF C4           inc         r12d
  00000000670C2EB7: 48 83 C6 02        add         rsi,2
  00000000670C2EBB: 48 3B F2           cmp         rsi,rdx
  00000000670C2EBE: 74 0A              je          00000000670C2ECA
  00000000670C2EC0: 66 83 3F 0D        cmp         word ptr [rdi],0Dh
  00000000670C2EC4: 75 EA              jne         00000000670C2EB0
  00000000670C2EC6: 48 83 C7 02        add         rdi,2
  00000000670C2ECA: 48 3B FA           cmp         rdi,rdx
  00000000670C2ECD: 73 32              jae         00000000670C2F01
  00000000670C2ECF: 90                 nop
  00000000670C2ED0: 0F B7 07           movzx       eax,word ptr [rdi]
  00000000670C2ED3: 48 83 C7 02        add         rdi,2
  00000000670C2ED7: 66 83 F8 0D        cmp         ax,0Dh
  00000000670C2EDB: 74 09              je          00000000670C2EE6
  00000000670C2EDD: 66 89 06           mov         word ptr [rsi],ax
  00000000670C2EE0: 48 83 C6 02        add         rsi,2
  00000000670C2EE4: EB 13              jmp         00000000670C2EF9
  00000000670C2EE6: 8B 85 A0 00 00 00  mov         eax,dword ptr [rbp+00000000000000A0h]
  00000000670C2EEC: 44 3B E0           cmp         r12d,eax
  00000000670C2EEF: 7D 08              jge         00000000670C2EF9
  00000000670C2EF1: FF C8              dec         eax
  00000000670C2EF3: 89 85 A0 00 00 00  mov         dword ptr [rbp+00000000000000A0h],eax
  00000000670C2EF9: 41 FF C4           inc         r12d
  00000000670C2EFC: 48 3B FA           cmp         rdi,rdx
  00000000670C2EFF: 72 CF              jb          00000000670C2ED0
  00000000670C2F01: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2F08: E8 03 2B 0B 00     call        0000000067175A10
  00000000670C2F0D: 48 2B F0           sub         rsi,rax
  00000000670C2F10: 48 D1 FE           sar         rsi,1
  00000000670C2F13: 8B D6              mov         edx,esi
  00000000670C2F15: 48 8D 8D 98 00 00  lea         rcx,[rbp+0000000000000098h]
                    00
  00000000670C2F1C: E8 9F FA FA FF     call        00000000670729C0
  00000000670C2F21: B0 01              mov         al,1
  00000000670C2F23: 48 8B 8C 24 60 40  mov         rcx,qword ptr [rsp+0000000000004060h]
                    00 00
  00000000670C2F2B: 48 33 CC           xor         rcx,rsp
  00000000670C2F2E: E8 DD 5C 0B 00     call        0000000067178C10
  00000000670C2F33: 48 8B 9C 24 B0 40  mov         rbx,qword ptr [rsp+00000000000040B0h]
                    00 00
  00000000670C2F3B: 48 81 C4 70 40 00  add         rsp,4070h
                    00
  00000000670C2F42: 41 5D              pop         r13
  00000000670C2F44: 41 5C              pop         r12
  00000000670C2F46: 5F                 pop         rdi
  00000000670C2F47: 5E                 pop         rsi
  00000000670C2F48: 5D                 pop         rbp
  00000000670C2F49: C3                 ret
  00000000670C2F4A: CC                 int         3
  00000000670C2F4B: CC                 int         3
  00000000670C2F4C: CC                 int         3
  00000000670C2F4D: CC                 int         3
  00000000670C2F4E: CC                 int         3
  00000000670C2F4F: CC                 int         3
  00000000670C2F50: 40

  Summary

      1A7000 .text
