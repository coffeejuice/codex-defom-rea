Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006530EDE0: 48 8B C4           mov         rax,rsp
  000000006530EDE3: 57                 push        rdi
  000000006530EDE4: 41 54              push        r12
  000000006530EDE6: 41 55              push        r13
  000000006530EDE8: 41 56              push        r14
  000000006530EDEA: 41 57              push        r15
  000000006530EDEC: 48 81 EC 70 01 00  sub         rsp,170h
                    00
  000000006530EDF3: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006530EDFF: 48 89 58 08        mov         qword ptr [rax+8],rbx
  000000006530EE03: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  000000006530EE07: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  000000006530EE0B: 49 8B F9           mov         rdi,r9
  000000006530EE0E: 48 8B F2           mov         rsi,rdx
  000000006530EE11: 48 8B E9           mov         rbp,rcx
  000000006530EE14: 80 3D 8D DC 6B 00  cmp         byte ptr [00000000659CCAA8h],0
                    00
  000000006530EE1B: 75 1A              jne         000000006530EE37
  000000006530EE1D: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530EE25: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530EE2A: E8 61 A2 04 00     call        0000000065359090
  000000006530EE2F: 48 8B C6           mov         rax,rsi
  000000006530EE32: E9 68 05 00 00     jmp         000000006530F39F
  000000006530EE37: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530EE3C: 41 8B 41 10        mov         eax,dword ptr [r9+10h]
  000000006530EE40: 89 01              mov         dword ptr [rcx],eax
  000000006530EE42: 41 8B 41 14        mov         eax,dword ptr [r9+14h]
  000000006530EE46: 89 41 04           mov         dword ptr [rcx+4],eax
  000000006530EE49: 41 8B 41 18        mov         eax,dword ptr [r9+18h]
  000000006530EE4D: 89 41 08           mov         dword ptr [rcx+8],eax
  000000006530EE50: 41 8B 41 1C        mov         eax,dword ptr [r9+1Ch]
  000000006530EE54: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  000000006530EE57: 41 83 F8 27        cmp         r8d,27h
  000000006530EE5B: 0F 87 E8 04 00 00  ja          000000006530F349
  000000006530EE61: 49 63 C0           movsxd      rax,r8d
  000000006530EE64: 48 8D 15 95 11 CF  lea         rdx,[0000000065000000h]
                    FF
  000000006530EE6B: 0F B6 84 02 D8 F3  movzx       eax,byte ptr [rdx+rax+000000000030F3D8h]
                    30 00
  000000006530EE73: 8B 8C 82 C0 F3 30  mov         ecx,dword ptr [rdx+rax*4+000000000030F3C0h]
                    00
  000000006530EE7A: 48 03 CA           add         rcx,rdx
  000000006530EE7D: FF E1              jmp         rcx
  000000006530EE7F: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530EE87: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530EE8C: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000006530EE94: 48 8B CD           mov         rcx,rbp
  000000006530EE97: E8 F4 A1 04 00     call        0000000065359090
  000000006530EE9C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530EEA1: 8B 08              mov         ecx,dword ptr [rax]
  000000006530EEA3: 89 0A              mov         dword ptr [rdx],ecx
  000000006530EEA5: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530EEA8: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530EEAB: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530EEAE: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530EEB1: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530EEB4: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530EEB7: 41 B9 01 00 00 00  mov         r9d,1
  000000006530EEBD: 45 33 C0           xor         r8d,r8d
  000000006530EEC0: 48 8B D6           mov         rdx,rsi
  000000006530EEC3: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530EEC8: FF 15 12 8D 38 00  call        qword ptr [0000000065697BE0h]
  000000006530EECE: 48 8B C6           mov         rax,rsi
  000000006530EED1: E9 C9 04 00 00     jmp         000000006530F39F
  000000006530EED6: 41 83 39 01        cmp         dword ptr [r9],1
  000000006530EEDA: 0F 8C A1 04 00 00  jl          000000006530F381
  000000006530EEE0: 41 83 79 04 0D     cmp         dword ptr [r9+4],0Dh
  000000006530EEE5: 0F 85 96 04 00 00  jne         000000006530F381
  000000006530EEEB: 48 8B 9C 24 C0 01  mov         rbx,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530EEF3: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006530EEF8: 41 B8 1E 00 00 00  mov         r8d,1Eh
  000000006530EEFE: 48 8D 94 24 60 01  lea         rdx,[rsp+0000000000000160h]
                    00 00
  000000006530EF06: 48 8B CD           mov         rcx,rbp
  000000006530EF09: E8 82 A1 04 00     call        0000000065359090
  000000006530EF0E: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530EF13: 8B 08              mov         ecx,dword ptr [rax]
  000000006530EF15: 89 0A              mov         dword ptr [rdx],ecx
  000000006530EF17: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530EF1A: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530EF1D: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530EF20: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530EF23: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530EF26: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530EF29: 48 8B D3           mov         rdx,rbx
  000000006530EF2C: 48 8D 0D 65 E5 3F  lea         rcx,[000000006570D498h]
                    00
  000000006530EF33: FF 15 9F 82 38 00  call        qword ptr [00000000656971D8h]
  000000006530EF39: 48 85 C0           test        rax,rax
  000000006530EF3C: 74 10              je          000000006530EF4E
  000000006530EF3E: 48 8B C8           mov         rcx,rax
  000000006530EF41: E8 7A 1C 10 00     call        0000000065410BC0
  000000006530EF46: 84 C0              test        al,al
  000000006530EF48: 0F 85 33 04 00 00  jne         000000006530F381
  000000006530EF4E: C7 44 24 20 FE FF  mov         dword ptr [rsp+20h],0FFFFFFFEh
                    FF FF
  000000006530EF56: 41 B9 FE FF FF FF  mov         r9d,0FFFFFFFEh
  000000006530EF5C: 45 33 C0           xor         r8d,r8d
  000000006530EF5F: 33 D2              xor         edx,edx
  000000006530EF61: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530EF66: FF 15 9C 81 38 00  call        qword ptr [0000000065697108h]
  000000006530EF6C: E9 10 04 00 00     jmp         000000006530F381
  000000006530EF71: 4C 8B A4 24 C0 01  mov         r12,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530EF79: 4C 89 64 24 20     mov         qword ptr [rsp+20h],r12
  000000006530EF7E: 41 B8 1C 00 00 00  mov         r8d,1Ch
  000000006530EF84: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  000000006530EF8C: 48 8B CD           mov         rcx,rbp
  000000006530EF8F: E8 FC A0 04 00     call        0000000065359090
  000000006530EF94: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530EF99: 8B 08              mov         ecx,dword ptr [rax]
  000000006530EF9B: 89 0A              mov         dword ptr [rdx],ecx
  000000006530EF9D: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530EFA0: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530EFA3: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530EFA6: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530EFA9: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530EFAC: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530EFAF: 83 3F 01           cmp         dword ptr [rdi],1
  000000006530EFB2: 0F 8C C9 03 00 00  jl          000000006530F381
  000000006530EFB8: 83 7F 04 0D        cmp         dword ptr [rdi+4],0Dh
  000000006530EFBC: 0F 85 BF 03 00 00  jne         000000006530F381
  000000006530EFC2: 83 7F 0C 01        cmp         dword ptr [rdi+0Ch],1
  000000006530EFC6: 0F 85 B5 03 00 00  jne         000000006530F381
  000000006530EFCC: 8B 47 40           mov         eax,dword ptr [rdi+40h]
  000000006530EFCF: 85 C0              test        eax,eax
  000000006530EFD1: 74 09              je          000000006530EFDC
  000000006530EFD3: 83 F8 01           cmp         eax,1
  000000006530EFD6: 0F 85 A5 03 00 00  jne         000000006530F381
  000000006530EFDC: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006530EFE4: E8 37 AE F9 FF     call        00000000652A9E20
  000000006530EFE9: 90                 nop
  000000006530EFEA: BB 03 00 00 00     mov         ebx,3
  000000006530EFEF: 41 BD 01 00 00 00  mov         r13d,1
  000000006530EFF5: 83 7F 40 00        cmp         dword ptr [rdi+40h],0
  000000006530EFF9: 44 0F 44 EB        cmove       r13d,ebx
  000000006530EFFD: 44 89 AC 24 B8 00  mov         dword ptr [rsp+00000000000000B8h],r13d
                    00 00
  000000006530F005: 48 8B CD           mov         rcx,rbp
  000000006530F008: E8 93 8F F9 FF     call        00000000652A7FA0
  000000006530F00D: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006530F010: 4D 8B CC           mov         r9,r12
  000000006530F013: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  000000006530F01B: 8D 53 14           lea         edx,[rbx+14h]
  000000006530F01E: 48 8B C8           mov         rcx,rax
  000000006530F021: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  000000006530F028: 8B D8              mov         ebx,eax
  000000006530F02A: 48 8B CD           mov         rcx,rbp
  000000006530F02D: E8 6E 8F F9 FF     call        00000000652A7FA0
  000000006530F032: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006530F035: 4D 8B CC           mov         r9,r12
  000000006530F038: 4C 8B C7           mov         r8,rdi
  000000006530F03B: BA 05 00 00 00     mov         edx,5
  000000006530F040: 48 8B C8           mov         rcx,rax
  000000006530F043: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  000000006530F04A: 2B C3              sub         eax,ebx
  000000006530F04C: 45 33 F6           xor         r14d,r14d
  000000006530F04F: 44 89 74 24 20     mov         dword ptr [rsp+20h],r14d
  000000006530F054: 44 8B C8           mov         r9d,eax
  000000006530F057: 45 33 C0           xor         r8d,r8d
  000000006530F05A: 8B D0              mov         edx,eax
  000000006530F05C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530F061: FF 15 A1 80 38 00  call        qword ptr [0000000065697108h]
  000000006530F067: 90                 nop
  000000006530F068: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000006530F070: E8 BB 23 D9 FF     call        00000000650A1430
  000000006530F075: 90                 nop
  000000006530F076: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006530F07E: FF 15 DC 76 38 00  call        qword ptr [0000000065696760h]
  000000006530F084: 90                 nop
  000000006530F085: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006530F08D: E8 CE A3 F9 FF     call        00000000652A9460
  000000006530F092: E9 EA 02 00 00     jmp         000000006530F381
  000000006530F097: 41 83 39 01        cmp         dword ptr [r9],1
  000000006530F09B: 0F 8C E0 02 00 00  jl          000000006530F381
  000000006530F0A1: 41 83 79 04 02     cmp         dword ptr [r9+4],2
  000000006530F0A6: 0F 85 D5 02 00 00  jne         000000006530F381
  000000006530F0AC: 4C 8B BC 24 C0 01  mov         r15,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530F0B4: 4D 85 FF           test        r15,r15
  000000006530F0B7: 0F 84 C4 02 00 00  je          000000006530F381
  000000006530F0BD: 48 8D 15 44 DA 3D  lea         rdx,[00000000656ECB08h]
                    00
  000000006530F0C4: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  000000006530F0CC: FF 15 0E 79 38 00  call        qword ptr [00000000656969E0h]
  000000006530F0D2: 48 8B D0           mov         rdx,rax
  000000006530F0D5: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006530F0DA: FF 15 28 7B 38 00  call        qword ptr [0000000065696C08h]
  000000006530F0E0: 90                 nop
  000000006530F0E1: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000006530F0E9: FF 15 49 77 38 00  call        qword ptr [0000000065696838h]
  000000006530F0EF: 48 8B D8           mov         rbx,rax
  000000006530F0F2: 4C 89 BC 24 80 00  mov         qword ptr [rsp+0000000000000080h],r15
                    00 00
  000000006530F0FA: 45 33 F6           xor         r14d,r14d
  000000006530F0FD: 4C 89 B4 24 88 00  mov         qword ptr [rsp+0000000000000088h],r14
                    00 00
  000000006530F105: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006530F10A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006530F112: FF 15 D8 78 38 00  call        qword ptr [00000000656969F0h]
  000000006530F118: 4C 89 B4 24 98 00  mov         qword ptr [rsp+0000000000000098h],r14
                    00 00
  000000006530F120: 44 89 B4 24 A0 00  mov         dword ptr [rsp+00000000000000A0h],r14d
                    00 00
  000000006530F128: 44 89 B4 24 A4 00  mov         dword ptr [rsp+00000000000000A4h],r14d
                    00 00
  000000006530F130: 83 A4 24 A8 00 00  and         dword ptr [rsp+00000000000000A8h],0FFFFFFF0h
                    00 F0
  000000006530F138: 44 89 B4 24 AC 00  mov         dword ptr [rsp+00000000000000ACh],r14d
                    00 00
  000000006530F140: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006530F148: 8B 0B              mov         ecx,dword ptr [rbx]
  000000006530F14A: 89 0A              mov         dword ptr [rdx],ecx
  000000006530F14C: 8B 43 04           mov         eax,dword ptr [rbx+4]
  000000006530F14F: 89 42 04           mov         dword ptr [rdx+4],eax
  000000006530F152: 8B 43 08           mov         eax,dword ptr [rbx+8]
  000000006530F155: 89 42 08           mov         dword ptr [rdx+8],eax
  000000006530F158: 8B 43 0C           mov         eax,dword ptr [rbx+0Ch]
  000000006530F15B: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530F15E: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006530F163: FF 15 F7 75 38 00  call        qword ptr [0000000065696760h]
  000000006530F169: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006530F171: E8 FA D9 FF FF     call        000000006530CB70
  000000006530F176: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006530F17B: 48 85 C0           test        rax,rax
  000000006530F17E: 0F 84 2A 01 00 00  je          000000006530F2AE
  000000006530F184: 8B 57 08           mov         edx,dword ptr [rdi+8]
  000000006530F187: 8B CA              mov         ecx,edx
  000000006530F189: 41 BD 01 00 00 00  mov         r13d,1
  000000006530F18F: 45 8D 46 05        lea         r8d,[r14+5]
  000000006530F193: 41 23 CD           and         ecx,r13d
  000000006530F196: 75 05              jne         000000006530F19D
  000000006530F198: 8D 59 04           lea         ebx,[rcx+4]
  000000006530F19B: EB 2A              jmp         000000006530F1C7
  000000006530F19D: 8B C2              mov         eax,edx
  000000006530F19F: 83 E0 04           and         eax,4
  000000006530F1A2: 74 07              je          000000006530F1AB
  000000006530F1A4: BB 03 00 00 00     mov         ebx,3
  000000006530F1A9: EB 1C              jmp         000000006530F1C7
  000000006530F1AB: 81 E2 00 20 00 00  and         edx,2000h
  000000006530F1B1: 74 07              je          000000006530F1BA
  000000006530F1B3: BB 02 00 00 00     mov         ebx,2
  000000006530F1B8: EB 0D              jmp         000000006530F1C7
  000000006530F1BA: 8B 47 38           mov         eax,dword ptr [rdi+38h]
  000000006530F1BD: 83 E0 04           and         eax,4
  000000006530F1C0: 41 8B DD           mov         ebx,r13d
  000000006530F1C3: 41 0F 45 D8        cmovne      ebx,r8d
  000000006530F1C7: 48 8B CD           mov         rcx,rbp
  000000006530F1CA: E8 D1 8D F9 FF     call        00000000652A7FA0
  000000006530F1CF: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006530F1D2: 4D 8B CF           mov         r9,r15
  000000006530F1D5: 4C 8B C7           mov         r8,rdi
  000000006530F1D8: BA 05 00 00 00     mov         edx,5
  000000006530F1DD: 48 8B C8           mov         rcx,rax
  000000006530F1E0: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  000000006530F1E7: 8B C8              mov         ecx,eax
  000000006530F1E9: F7 D9              neg         ecx
  000000006530F1EB: 89 4C 24 28        mov         dword ptr [rsp+28h],ecx
  000000006530F1EF: 89 4C 24 20        mov         dword ptr [rsp+20h],ecx
  000000006530F1F3: 44 8B C8           mov         r9d,eax
  000000006530F1F6: 44 8B C0           mov         r8d,eax
  000000006530F1F9: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  000000006530F201: 48 8D 4F 10        lea         rcx,[rdi+10h]
  000000006530F205: FF 15 25 77 38 00  call        qword ptr [0000000065696930h]
  000000006530F20B: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530F210: 8B 08              mov         ecx,dword ptr [rax]
  000000006530F212: 89 0A              mov         dword ptr [rdx],ecx
  000000006530F214: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530F217: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530F21A: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530F21D: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530F220: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530F223: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530F226: 48 8D 44 24 60     lea         rax,[rsp+60h]
  000000006530F22B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006530F230: 4C 89 74 24 28     mov         qword ptr [rsp+28h],r14
  000000006530F235: C7 44 24 20 12 0E  mov         dword ptr [rsp+20h],0E12h
                    00 00
  000000006530F23D: 44 8B CB           mov         r9d,ebx
  000000006530F240: 45 8B C5           mov         r8d,r13d
  000000006530F243: 33 D2              xor         edx,edx
  000000006530F245: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000006530F24A: FF 15 F0 D7 6B 00  call        qword ptr [00000000659CCA40h]
  000000006530F250: 85 C0              test        eax,eax
  000000006530F252: 75 5A              jne         000000006530F2AE
  000000006530F254: 8B 44 24 6C        mov         eax,dword ptr [rsp+6Ch]
  000000006530F258: F7 D8              neg         eax
  000000006530F25A: 44 8B 4C 24 64     mov         r9d,dword ptr [rsp+64h]
  000000006530F25F: 41 F7 D9           neg         r9d
  000000006530F262: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000006530F266: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  000000006530F26B: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  000000006530F26F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530F274: FF 15 8E 7E 38 00  call        qword ptr [0000000065697108h]
  000000006530F27A: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  000000006530F27F: 4C 8D 47 10        lea         r8,[rdi+10h]
  000000006530F283: 8B 57 0C           mov         edx,dword ptr [rdi+0Ch]
  000000006530F286: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006530F28E: E8 AD 7D F9 FF     call        00000000652A7040
  000000006530F293: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530F298: 8B 08              mov         ecx,dword ptr [rax]
  000000006530F29A: 89 0A              mov         dword ptr [rdx],ecx
  000000006530F29C: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530F29F: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530F2A2: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530F2A5: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530F2A8: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530F2AB: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530F2AE: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006530F2B6: FF 15 A4 74 38 00  call        qword ptr [0000000065696760h]
  000000006530F2BC: E9 C0 00 00 00     jmp         000000006530F381
  000000006530F2C1: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530F2C9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530F2CE: 41 B8 0D 00 00 00  mov         r8d,0Dh
  000000006530F2D4: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  000000006530F2DC: 48 8B CD           mov         rcx,rbp
  000000006530F2DF: E8 7C DB FA FF     call        00000000652BCE60
  000000006530F2E4: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530F2E9: 8B 08              mov         ecx,dword ptr [rax]
  000000006530F2EB: 89 0A              mov         dword ptr [rdx],ecx
  000000006530F2ED: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530F2F0: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530F2F3: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530F2F6: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530F2F9: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530F2FC: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530F2FF: F6 47 08 80        test        byte ptr [rdi+8],80h
  000000006530F303: 74 22              je          000000006530F327
  000000006530F305: C7 44 24 20 FD FF  mov         dword ptr [rsp+20h],0FFFFFFFDh
                    FF FF
  000000006530F30D: BA 04 00 00 00     mov         edx,4
  000000006530F312: 44 8D 4A F8        lea         r9d,[rdx-8]
  000000006530F316: 44 8D 42 FF        lea         r8d,[rdx-1]
  000000006530F31A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530F31F: FF 15 E3 7D 38 00  call        qword ptr [0000000065697108h]
  000000006530F325: EB 5A              jmp         000000006530F381
  000000006530F327: C7 44 24 20 FE FF  mov         dword ptr [rsp+20h],0FFFFFFFEh
                    FF FF
  000000006530F32F: BA 03 00 00 00     mov         edx,3
  000000006530F334: 44 8D 4A FA        lea         r9d,[rdx-6]
  000000006530F338: 44 8D 42 FF        lea         r8d,[rdx-1]
  000000006530F33C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530F341: FF 15 C1 7D 38 00  call        qword ptr [0000000065697108h]
  000000006530F347: EB 38              jmp         000000006530F381
  000000006530F349: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530F351: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530F356: 48 8D 94 24 40 01  lea         rdx,[rsp+0000000000000140h]
                    00 00
  000000006530F35E: 48 8B CD           mov         rcx,rbp
  000000006530F361: E8 2A 9D 04 00     call        0000000065359090
  000000006530F366: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530F36B: 8B 08              mov         ecx,dword ptr [rax]
  000000006530F36D: 89 0A              mov         dword ptr [rdx],ecx
  000000006530F36F: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530F372: 89 4A 04           mov         dword ptr [rdx+4],ecx
  000000006530F375: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530F378: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530F37B: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006530F37E: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530F381: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530F386: 8B 01              mov         eax,dword ptr [rcx]
  000000006530F388: 89 06              mov         dword ptr [rsi],eax
  000000006530F38A: 8B 41 04           mov         eax,dword ptr [rcx+4]
  000000006530F38D: 89 46 04           mov         dword ptr [rsi+4],eax
  000000006530F390: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006530F393: 89 46 08           mov         dword ptr [rsi+8],eax
  000000006530F396: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006530F399: 89 46 0C           mov         dword ptr [rsi+0Ch],eax
  000000006530F39C: 48 8B C6           mov         rax,rsi
  000000006530F39F: 4C 8D 9C 24 70 01  lea         r11,[rsp+0000000000000170h]
                    00 00
  000000006530F3A7: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  000000006530F3AB: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  000000006530F3AF: 49 8B 73 40        mov         rsi,qword ptr [r11+40h]
  000000006530F3B3: 49 8B E3           mov         rsp,r11
  000000006530F3B6: 41 5F              pop         r15
  000000006530F3B8: 41 5E              pop         r14
  000000006530F3BA: 41 5D              pop         r13
  000000006530F3BC: 41 5C              pop         r12
  000000006530F3BE: 5F                 pop         rdi
  000000006530F3BF: C3                 ret
  000000006530F3C0: 97                 xchg        eax,edi
  000000006530F3C1: F0 30 00           lock xor    byte ptr [rax],al
  000000006530F3C4: C1 F2 30           sal         edx,30h
  000000006530F3C7: 00 71 EF           add         byte ptr [rcx-11h],dh
  000000006530F3CA: 30 00              xor         byte ptr [rax],al
  000000006530F3CC: D6
  000000006530F3CD: EE                 out         dx,al
  000000006530F3CE: 30 00              xor         byte ptr [rax],al
  000000006530F3D0: 7F EE              jg          000000006530F3C0
  000000006530F3D2: 30 00              xor         byte ptr [rax],al
  000000006530F3D4: 49
  000000006530F3D5: F3 30 00           rep xor     byte ptr [rax],al
  000000006530F3D8: 00 05 05 05 05 05  add         byte ptr [000000006A35F8E3h],al
  000000006530F3DE: 05 05 05 05 05     add         eax,5050505h
  000000006530F3E3: 05 05 05 01 05     add         eax,5010505h
  000000006530F3E8: 05 05 05 05 05     add         eax,5050505h
  000000006530F3ED: 05 05 05 05 05     add         eax,5050505h
  000000006530F3F2: 05 05 02 05 03     add         eax,3050205h
  000000006530F3F7: 05 05 05 05 05     add         eax,5050505h
  000000006530F3FC: 05
  000000006530F3FD: 05
  000000006530F3FE: 04 04              add         al,4

  Summary

      695000 .text
