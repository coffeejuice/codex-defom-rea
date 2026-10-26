Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2B60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670C2B65: 57                 push        rdi
  00000000670C2B66: 48 83 EC 30        sub         rsp,30h
  00000000670C2B6A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C2B73: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C2B78: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000670C2B7D: 48 8B FA           mov         rdi,rdx
  00000000670C2B80: 48 8B F1           mov         rsi,rcx
  00000000670C2B83: 33 D2              xor         edx,edx
  00000000670C2B85: 48 83 C1 08        add         rcx,8
  00000000670C2B89: E8 42 E8 06 00     call        00000000671313D0
  00000000670C2B8E: 4C 8D 1D F3 96 16  lea         r11,[000000006722C288h]
                    00
  00000000670C2B95: 4C 89 5E 08        mov         qword ptr [rsi+8],r11
  00000000670C2B99: 33 C9              xor         ecx,ecx
  00000000670C2B9B: 89 4E 34           mov         dword ptr [rsi+34h],ecx
  00000000670C2B9E: 89 4E 40           mov         dword ptr [rsi+40h],ecx
  00000000670C2BA1: 89 4E 44           mov         dword ptr [rsi+44h],ecx
  00000000670C2BA4: 89 4E 48           mov         dword ptr [rsi+48h],ecx
  00000000670C2BA7: 48 89 4E 58        mov         qword ptr [rsi+58h],rcx
  00000000670C2BAB: 89 4E 54           mov         dword ptr [rsi+54h],ecx
  00000000670C2BAE: 89 4E 50           mov         dword ptr [rsi+50h],ecx
  00000000670C2BB1: 89 4E 4C           mov         dword ptr [rsi+4Ch],ecx
  00000000670C2BB4: 89 4E 60           mov         dword ptr [rsi+60h],ecx
  00000000670C2BB7: 89 4E 64           mov         dword ptr [rsi+64h],ecx
  00000000670C2BBA: 89 4E 68           mov         dword ptr [rsi+68h],ecx
  00000000670C2BBD: 48 89 4E 78        mov         qword ptr [rsi+78h],rcx
  00000000670C2BC1: 89 4E 74           mov         dword ptr [rsi+74h],ecx
  00000000670C2BC4: 89 4E 70           mov         dword ptr [rsi+70h],ecx
  00000000670C2BC7: 89 4E 6C           mov         dword ptr [rsi+6Ch],ecx
  00000000670C2BCA: 48 89 8E 80 00 00  mov         qword ptr [rsi+0000000000000080h],rcx
                    00
  00000000670C2BD1: 48 8D 05 58 DF 22  lea         rax,[00000000672F0B30h]
                    00
  00000000670C2BD8: 48 89 86 90 00 00  mov         qword ptr [rsi+0000000000000090h],rax
                    00
  00000000670C2BDF: F0 83 05 49 DF 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C2BE7: 48 89 86 98 00 00  mov         qword ptr [rsi+0000000000000098h],rax
                    00
  00000000670C2BEE: F0 83 05 3A DF 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C2BF6: 89 8E A4 00 00 00  mov         dword ptr [rsi+00000000000000A4h],ecx
  00000000670C2BFC: 66 89 8E BC 00 00  mov         word ptr [rsi+00000000000000BCh],cx
                    00
  00000000670C2C03: 89 8E C8 00 00 00  mov         dword ptr [rsi+00000000000000C8h],ecx
  00000000670C2C09: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C2C10: 45 33 C0           xor         r8d,r8d
  00000000670C2C13: 41 8D 50 01        lea         edx,[r8+1]
  00000000670C2C17: E8 84 A9 F8 FF     call        000000006704D5A0
  00000000670C2C1C: 48 89 BE D8 00 00  mov         qword ptr [rsi+00000000000000D8h],rdi
                    00
  00000000670C2C23: 48 8B CE           mov         rcx,rsi
  00000000670C2C26: E8 E5 F5 FF FF     call        00000000670C2210
  00000000670C2C2B: 90                 nop
  00000000670C2C2C: 48 8B C6           mov         rax,rsi
  00000000670C2C2F: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C2C34: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000670C2C39: 48 83 C4 30        add         rsp,30h
  00000000670C2C3D: 5F                 pop         rdi
  00000000670C2C3E: C3                 ret
  00000000670C2C3F: CC                 int         3
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
  00000000670C2D1F: 48
  00000000670C2D20: 83

  Summary

      1A7000 .text
