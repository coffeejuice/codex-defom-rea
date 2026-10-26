Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EFDE0: 48 8B C4           mov         rax,rsp
  00000001412EFDE3: 57                 push        rdi
  00000001412EFDE4: 41 54              push        r12
  00000001412EFDE6: 41 55              push        r13
  00000001412EFDE8: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  00000001412EFDEF: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412EFDF8: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000001412EFDFC: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000001412EFE00: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000001412EFE04: 4D 8B E0           mov         r12,r8
  00000001412EFE07: 4C 8B EA           mov         r13,rdx
  00000001412EFE0A: 48 8B F1           mov         rsi,rcx
  00000001412EFE0D: 49 8B 68 08        mov         rbp,qword ptr [r8+8]
  00000001412EFE11: 80 7D 29 00        cmp         byte ptr [rbp+29h],0
  00000001412EFE15: 0F 84 8E 00 00 00  je          00000001412EFEA9
  00000001412EFE1B: 48 C7 40 98 0F 00  mov         qword ptr [rax-68h],0Fh
                    00 00
  00000001412EFE23: 33 DB              xor         ebx,ebx
  00000001412EFE25: 48 89 58 90        mov         qword ptr [rax-70h],rbx
  00000001412EFE29: 88 58 80           mov         byte ptr [rax-80h],bl
  00000001412EFE2C: 44 8D 43 1B        lea         r8d,[rbx+1Bh]
  00000001412EFE30: 48 8D 15 F9 1A 4F  lea         rdx,[00000001427E1930h]
                    01
  00000001412EFE37: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EFE3C: E8 4F 08 E5 FE     call        0000000140140690
  00000001412EFE41: 90                 nop
  00000001412EFE42: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001412EFE47: E8 5C 08 18 00     call        00000001414706A8
  00000001412EFE4C: 90                 nop
  00000001412EFE4D: 48 8D 05 8C BF 4D  lea         rax,[00000001427CBDE0h]
                    01
  00000001412EFE54: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001412EFE59: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0Fh
                    00 00 0F 00 00 00
  00000001412EFE65: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  00000001412EFE6D: 88 9C 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],bl
                    00
  00000001412EFE74: 49 83 C9 FF        or          r9,0FFFFFFFFFFFFFFFFh
  00000001412EFE78: 45 33 C0           xor         r8d,r8d
  00000001412EFE7B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001412EFE80: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001412EFE85: E8 66 EB DB FE     call        00000001400AE9F0
  00000001412EFE8A: 90                 nop
  00000001412EFE8B: 48 8D 05 C6 1A 4F  lea         rax,[00000001427E1958h]
                    01
  00000001412EFE92: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001412EFE97: 48 8D 15 0A B9 21  lea         rdx,[000000014350B7A8h]
                    02
  00000001412EFE9E: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001412EFEA3: E8 B0 EC 17 00     call        000000014146EB58
  00000001412EFEA8: 90                 nop
  00000001412EFEA9: 49 8B C8           mov         rcx,r8
  00000001412EFEAC: E8 FF 88 FF FF     call        00000001412E87B0
  00000001412EFEB1: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  00000001412EFEB5: 41 80 7B 29 00     cmp         byte ptr [r11+29h],0
  00000001412EFEBA: 74 06              je          00000001412EFEC2
  00000001412EFEBC: 48 8B 7D 10        mov         rdi,qword ptr [rbp+10h]
  00000001412EFEC0: EB 21              jmp         00000001412EFEE3
  00000001412EFEC2: 48 8B 45 10        mov         rax,qword ptr [rbp+10h]
  00000001412EFEC6: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFECA: 74 05              je          00000001412EFED1
  00000001412EFECC: 49 8B FB           mov         rdi,r11
  00000001412EFECF: EB 12              jmp         00000001412EFEE3
  00000001412EFED1: 49 8B 4C 24 08     mov         rcx,qword ptr [r12+8]
  00000001412EFED6: 48 8B 79 10        mov         rdi,qword ptr [rcx+10h]
  00000001412EFEDA: 48 3B CD           cmp         rcx,rbp
  00000001412EFEDD: 0F 85 A0 00 00 00  jne         00000001412EFF83
  00000001412EFEE3: 48 8B 5D 08        mov         rbx,qword ptr [rbp+8]
  00000001412EFEE7: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412EFEEB: 75 04              jne         00000001412EFEF1
  00000001412EFEED: 48 89 5F 08        mov         qword ptr [rdi+8],rbx
  00000001412EFEF1: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFEF5: 48 39 68 08        cmp         qword ptr [rax+8],rbp
  00000001412EFEF9: 75 06              jne         00000001412EFF01
  00000001412EFEFB: 48 89 78 08        mov         qword ptr [rax+8],rdi
  00000001412EFEFF: EB 0E              jmp         00000001412EFF0F
  00000001412EFF01: 48 39 2B           cmp         qword ptr [rbx],rbp
  00000001412EFF04: 75 05              jne         00000001412EFF0B
  00000001412EFF06: 48 89 3B           mov         qword ptr [rbx],rdi
  00000001412EFF09: EB 04              jmp         00000001412EFF0F
  00000001412EFF0B: 48 89 7B 10        mov         qword ptr [rbx+10h],rdi
  00000001412EFF0F: 48 8B 56 30        mov         rdx,qword ptr [rsi+30h]
  00000001412EFF13: 48 39 2A           cmp         qword ptr [rdx],rbp
  00000001412EFF16: 75 27              jne         00000001412EFF3F
  00000001412EFF18: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412EFF1C: 74 05              je          00000001412EFF23
  00000001412EFF1E: 48 8B CB           mov         rcx,rbx
  00000001412EFF21: EB 19              jmp         00000001412EFF3C
  00000001412EFF23: 48 8B CF           mov         rcx,rdi
  00000001412EFF26: 48 8B 07           mov         rax,qword ptr [rdi]
  00000001412EFF29: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFF2D: 75 0D              jne         00000001412EFF3C
  00000001412EFF2F: 90                 nop
  00000001412EFF30: 48 8B C8           mov         rcx,rax
  00000001412EFF33: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412EFF36: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFF3A: 74 F4              je          00000001412EFF30
  00000001412EFF3C: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412EFF3F: 48 8B 56 30        mov         rdx,qword ptr [rsi+30h]
  00000001412EFF43: 48 39 6A 10        cmp         qword ptr [rdx+10h],rbp
  00000001412EFF47: 0F 85 B0 00 00 00  jne         00000001412EFFFD
  00000001412EFF4D: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412EFF51: 74 05              je          00000001412EFF58
  00000001412EFF53: 48 8B CB           mov         rcx,rbx
  00000001412EFF56: EB 25              jmp         00000001412EFF7D
  00000001412EFF58: 48 8B CF           mov         rcx,rdi
  00000001412EFF5B: 48 8B 47 10        mov         rax,qword ptr [rdi+10h]
  00000001412EFF5F: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFF63: 75 18              jne         00000001412EFF7D
  00000001412EFF65: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000001412EFF70: 48 8B C8           mov         rcx,rax
  00000001412EFF73: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412EFF77: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFF7B: 74 F3              je          00000001412EFF70
  00000001412EFF7D: 48 89 4A 10        mov         qword ptr [rdx+10h],rcx
  00000001412EFF81: EB 7A              jmp         00000001412EFFFD
  00000001412EFF83: 49 89 4B 08        mov         qword ptr [r11+8],rcx
  00000001412EFF87: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000001412EFF8B: 48 89 01           mov         qword ptr [rcx],rax
  00000001412EFF8E: 48 3B 4D 10        cmp         rcx,qword ptr [rbp+10h]
  00000001412EFF92: 75 05              jne         00000001412EFF99
  00000001412EFF94: 48 8B D9           mov         rbx,rcx
  00000001412EFF97: EB 21              jmp         00000001412EFFBA
  00000001412EFF99: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000001412EFF9D: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412EFFA1: 75 04              jne         00000001412EFFA7
  00000001412EFFA3: 48 89 5F 08        mov         qword ptr [rdi+8],rbx
  00000001412EFFA7: 48 89 3B           mov         qword ptr [rbx],rdi
  00000001412EFFAA: 48 8B 45 10        mov         rax,qword ptr [rbp+10h]
  00000001412EFFAE: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001412EFFB2: 48 8B 45 10        mov         rax,qword ptr [rbp+10h]
  00000001412EFFB6: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFFBA: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFFBE: 48 39 68 08        cmp         qword ptr [rax+8],rbp
  00000001412EFFC2: 75 06              jne         00000001412EFFCA
  00000001412EFFC4: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFFC8: EB 12              jmp         00000001412EFFDC
  00000001412EFFCA: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000001412EFFCE: 48 39 28           cmp         qword ptr [rax],rbp
  00000001412EFFD1: 75 05              jne         00000001412EFFD8
  00000001412EFFD3: 48 89 08           mov         qword ptr [rax],rcx
  00000001412EFFD6: EB 04              jmp         00000001412EFFDC
  00000001412EFFD8: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  00000001412EFFDC: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000001412EFFE0: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412EFFE4: 48 8D 55 28        lea         rdx,[rbp+28h]
  00000001412EFFE8: 4C 8D 41 28        lea         r8,[rcx+28h]
  00000001412EFFEC: 4C 3B C2           cmp         r8,rdx
  00000001412EFFEF: 74 0C              je          00000001412EFFFD
  00000001412EFFF1: 41 0F B6 08        movzx       ecx,byte ptr [r8]
  00000001412EFFF5: 0F B6 02           movzx       eax,byte ptr [rdx]
  00000001412EFFF8: 41 88 00           mov         byte ptr [r8],al
  00000001412EFFFB: 88 0A              mov         byte ptr [rdx],cl
  00000001412EFFFD: 80 7D 28 01        cmp         byte ptr [rbp+28h],1
  00000001412F0001: 0F 85 48 01 00 00  jne         00000001412F014F
  00000001412F0007: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412F000B: 48 3B 78 08        cmp         rdi,qword ptr [rax+8]
  00000001412F000F: 0F 84 36 01 00 00  je          00000001412F014B
  00000001412F0015: 80 7F 28 01        cmp         byte ptr [rdi+28h],1
  00000001412F0019: 0F 85 2C 01 00 00  jne         00000001412F014B
  00000001412F001F: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000001412F0022: 48 3B F9           cmp         rdi,rcx
  00000001412F0025: 75 48              jne         00000001412F006F
  00000001412F0027: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000001412F002B: 80 79 28 00        cmp         byte ptr [rcx+28h],0
  00000001412F002F: 75 17              jne         00000001412F0048
  00000001412F0031: C6 41 28 01        mov         byte ptr [rcx+28h],1
  00000001412F0035: C6 43 28 00        mov         byte ptr [rbx+28h],0
  00000001412F0039: 48 8B D3           mov         rdx,rbx
  00000001412F003C: 48 8B CE           mov         rcx,rsi
  00000001412F003F: E8 9C C7 FF FF     call        00000001412EC7E0
  00000001412F0044: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000001412F0048: 80 79 29 00        cmp         byte ptr [rcx+29h],0
  00000001412F004C: 74 05              je          00000001412F0053
  00000001412F004E: 48 8B FB           mov         rdi,rbx
  00000001412F0051: EB 5D              jmp         00000001412F00B0
  00000001412F0053: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F0056: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F005A: 75 6B              jne         00000001412F00C7
  00000001412F005C: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F0060: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F0064: 75 61              jne         00000001412F00C7
  00000001412F0066: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412F006A: 48 8B FB           mov         rdi,rbx
  00000001412F006D: EB 41              jmp         00000001412F00B0
  00000001412F006F: 80 79 28 00        cmp         byte ptr [rcx+28h],0
  00000001412F0073: 75 16              jne         00000001412F008B
  00000001412F0075: C6 41 28 01        mov         byte ptr [rcx+28h],1
  00000001412F0079: C6 43 28 00        mov         byte ptr [rbx+28h],0
  00000001412F007D: 48 8B D3           mov         rdx,rbx
  00000001412F0080: 48 8B CE           mov         rcx,rsi
  00000001412F0083: E8 B8 C7 FF FF     call        00000001412EC840
  00000001412F0088: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000001412F008B: 80 79 29 00        cmp         byte ptr [rcx+29h],0
  00000001412F008F: 74 05              je          00000001412F0096
  00000001412F0091: 48 8B FB           mov         rdi,rbx
  00000001412F0094: EB 1A              jmp         00000001412F00B0
  00000001412F0096: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F009A: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F009E: 75 6B              jne         00000001412F010B
  00000001412F00A0: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F00A3: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F00A7: 75 62              jne         00000001412F010B
  00000001412F00A9: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412F00AD: 48 8B FB           mov         rdi,rbx
  00000001412F00B0: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  00000001412F00B4: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412F00B8: 48 3B 78 08        cmp         rdi,qword ptr [rax+8]
  00000001412F00BC: 0F 84 89 00 00 00  je          00000001412F014B
  00000001412F00C2: E9 4E FF FF FF     jmp         00000001412F0015
  00000001412F00C7: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F00CB: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F00CF: 75 1A              jne         00000001412F00EB
  00000001412F00D1: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F00D4: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412F00D8: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412F00DC: 48 8B D1           mov         rdx,rcx
  00000001412F00DF: 48 8B CE           mov         rcx,rsi
  00000001412F00E2: E8 59 C7 FF FF     call        00000001412EC840
  00000001412F00E7: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000001412F00EB: 0F B6 43 28        movzx       eax,byte ptr [rbx+28h]
  00000001412F00EF: 88 41 28           mov         byte ptr [rcx+28h],al
  00000001412F00F2: C6 43 28 01        mov         byte ptr [rbx+28h],1
  00000001412F00F6: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F00FA: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412F00FE: 48 8B D3           mov         rdx,rbx
  00000001412F0101: 48 8B CE           mov         rcx,rsi
  00000001412F0104: E8 D7 C6 FF FF     call        00000001412EC7E0
  00000001412F0109: EB 40              jmp         00000001412F014B
  00000001412F010B: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F010E: 80 78 28 01        cmp         byte ptr [rax+28h],1
  00000001412F0112: 75 1A              jne         00000001412F012E
  00000001412F0114: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412F0118: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412F011C: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412F0120: 48 8B D1           mov         rdx,rcx
  00000001412F0123: 48 8B CE           mov         rcx,rsi
  00000001412F0126: E8 B5 C6 FF FF     call        00000001412EC7E0
  00000001412F012B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000001412F012E: 0F B6 43 28        movzx       eax,byte ptr [rbx+28h]
  00000001412F0132: 88 41 28           mov         byte ptr [rcx+28h],al
  00000001412F0135: C6 43 28 01        mov         byte ptr [rbx+28h],1
  00000001412F0139: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F013C: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412F0140: 48 8B D3           mov         rdx,rbx
  00000001412F0143: 48 8B CE           mov         rcx,rsi
  00000001412F0146: E8 F5 C6 FF FF     call        00000001412EC840
  00000001412F014B: C6 47 28 01        mov         byte ptr [rdi+28h],1
  00000001412F014F: 48 8B CD           mov         rcx,rbp
  00000001412F0152: E8 11 FB 1E 00     call        00000001414DFC68
  00000001412F0157: 48 8B 46 38        mov         rax,qword ptr [rsi+38h]
  00000001412F015B: 48 85 C0           test        rax,rax
  00000001412F015E: 74 07              je          00000001412F0167
  00000001412F0160: 48 FF C8           dec         rax
  00000001412F0163: 48 89 46 38        mov         qword ptr [rsi+38h],rax
  00000001412F0167: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001412F016C: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000001412F0170: 48 89 01           mov         qword ptr [rcx],rax
  00000001412F0173: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000001412F0178: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412F017C: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001412F0181: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001412F0186: 48 8B 06           mov         rax,qword ptr [rsi]
  00000001412F0189: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412F018E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001412F0193: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412F0196: 49 89 45 00        mov         qword ptr [r13],rax
  00000001412F019A: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412F019E: 49 89 45 08        mov         qword ptr [r13+8],rax
  00000001412F01A2: 49 8B C5           mov         rax,r13
  00000001412F01A5: 4C 8D 9C 24 A0 00  lea         r11,[rsp+00000000000000A0h]
                    00 00
  00000001412F01AD: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  00000001412F01B1: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  00000001412F01B5: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  00000001412F01B9: 49 8B E3           mov         rsp,r11
  00000001412F01BC: 41 5D              pop         r13
  00000001412F01BE: 41 5C              pop         r12
  00000001412F01C0: 5F                 pop         rdi
  00000001412F01C1: C3                 ret

  Summary

     1E02000 .text
