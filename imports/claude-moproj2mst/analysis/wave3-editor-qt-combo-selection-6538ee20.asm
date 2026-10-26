Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538EE20: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000006538EE24: 55                 push        rbp
  000000006538EE25: 56                 push        rsi
  000000006538EE26: 57                 push        rdi
  000000006538EE27: 41 54              push        r12
  000000006538EE29: 41 55              push        r13
  000000006538EE2B: 41 56              push        r14
  000000006538EE2D: 41 57              push        r15
  000000006538EE2F: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  000000006538EE36: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006538EE3F: 48 89 9C 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rbx
                    00 00
  000000006538EE47: 4D 8B E8           mov         r13,r8
  000000006538EE4A: 44 8B E2           mov         r12d,edx
  000000006538EE4D: 4C 8B F9           mov         r15,rcx
  000000006538EE50: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006538EE54: 49 8B C8           mov         rcx,r8
  000000006538EE57: FF 15 73 7A 30 00  call        qword ptr [00000000656968D0h]
  000000006538EE5D: 84 C0              test        al,al
  000000006538EE5F: 0F 85 28 03 00 00  jne         000000006538F18D
  000000006538EE65: 49 8B 7F 08        mov         rdi,qword ptr [r15+8]
  000000006538EE69: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538EE70: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538EE73: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538EE7A: FF 15 D0 99 30 00  call        qword ptr [0000000065698850h]
  000000006538EE80: 48 8B D0           mov         rdx,rax
  000000006538EE83: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538EE8A: FF 53 68           call        qword ptr [rbx+68h]
  000000006538EE8D: 89 84 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],eax
                    00
  000000006538EE94: 33 ED              xor         ebp,ebp
  000000006538EE96: 89 AC 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],ebp
                    00
  000000006538EE9D: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006538EEA5: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000006538EEAD: 41 3B C4           cmp         eax,r12d
  000000006538EEB0: 48 0F 4D CA        cmovge      rcx,rdx
  000000006538EEB4: 48 8D 84 24 D8 00  lea         rax,[rsp+00000000000000D8h]
                    00 00
  000000006538EEBC: 39 29              cmp         dword ptr [rcx],ebp
  000000006538EEBE: 48 0F 4F C1        cmovg       rax,rcx
  000000006538EEC2: 44 8B 30           mov         r14d,dword ptr [rax]
  000000006538EEC5: 49 8B CD           mov         rcx,r13
  000000006538EEC8: FF 15 EA 79 30 00  call        qword ptr [00000000656968B8h]
  000000006538EECE: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  000000006538EED5: 8B 96 14 02 00 00  mov         edx,dword ptr [rsi+0000000000000214h]
  000000006538EEDB: 41 2B D6           sub         edx,r14d
  000000006538EEDE: 89 94 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],edx
                    00
  000000006538EEE5: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006538EEED: 4C 8D 84 24 D8 00  lea         r8,[rsp+00000000000000D8h]
                    00 00
  000000006538EEF5: 3B D0              cmp         edx,eax
  000000006538EEF7: 49 0F 4D C8        cmovge      rcx,r8
  000000006538EEFB: 44 8B 21           mov         r12d,dword ptr [rcx]
  000000006538EEFE: 45 85 E4           test        r12d,r12d
  000000006538EF01: 0F 8E 86 02 00 00  jle         000000006538F18D
  000000006538EF07: 48 8B 96 E0 01 00  mov         rdx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538EF0E: 48 8D 0D 6B 81 63  lea         rcx,[00000000659C7080h]
                    00
  000000006538EF15: FF 15 ED 7A 30 00  call        qword ptr [0000000065696A08h]
  000000006538EF1B: 48 8B D8           mov         rbx,rax
  000000006538EF1E: 48 85 C0           test        rax,rax
  000000006538EF21: 0F 84 C5 00 00 00  je          000000006538EFEC
  000000006538EF27: 48 8B 0D 9A 79 30  mov         rcx,qword ptr [00000000656968C8h]
                    00
  000000006538EF2E: 48 89 8C 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rcx
                    00 00
  000000006538EF36: FF 15 84 79 30 00  call        qword ptr [00000000656968C0h]
  000000006538EF3C: 90                 nop
  000000006538EF3D: 48 8B CB           mov         rcx,rbx
  000000006538EF40: E8 9B A4 19 00     call        00000000655293E0
  000000006538EF45: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006538EF4A: 8B DD              mov         ebx,ebp
  000000006538EF4C: 45 85 E4           test        r12d,r12d
  000000006538EF4F: 7E 5E              jle         000000006538EFAF
  000000006538EF51: B9 10 00 00 00     mov         ecx,10h
  000000006538EF56: E8 17 22 25 00     call        00000000655E1172
  000000006538EF5B: 48 8B F8           mov         rdi,rax
  000000006538EF5E: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000006538EF66: 48 85 C0           test        rax,rax
  000000006538EF69: 74 18              je          000000006538EF83
  000000006538EF6B: 8B D3              mov         edx,ebx
  000000006538EF6D: 49 8B CD           mov         rcx,r13
  000000006538EF70: FF 15 CA 79 30 00  call        qword ptr [0000000065696940h]
  000000006538EF76: 48 8B D0           mov         rdx,rax
  000000006538EF79: 48 8B CF           mov         rcx,rdi
  000000006538EF7C: E8 8F BE 19 00     call        000000006552AE10
  000000006538EF81: EB 03              jmp         000000006538EF86
  000000006538EF83: 48 8B C5           mov         rax,rbp
  000000006538EF86: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000006538EF8E: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006538EF96: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006538EF9E: E8 4D B4 1B 00     call        000000006554A3F0
  000000006538EFA3: FF C3              inc         ebx
  000000006538EFA5: 41 3B DC           cmp         ebx,r12d
  000000006538EFA8: 7C A7              jl          000000006538EF51
  000000006538EFAA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006538EFAF: 4C 8D 84 24 C0 00  lea         r8,[rsp+00000000000000C0h]
                    00 00
  000000006538EFB7: 41 8B D6           mov         edx,r14d
  000000006538EFBA: 48 8B C8           mov         rcx,rax
  000000006538EFBD: E8 2E 01 1A 00     call        000000006552F0F0
  000000006538EFC2: 90                 nop
  000000006538EFC3: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000006538EFCB: FF 15 A7 79 30 00  call        qword ptr [0000000065696978h]
  000000006538EFD1: 84 C0              test        al,al
  000000006538EFD3: 0F 85 4E 01 00 00  jne         000000006538F127
  000000006538EFD9: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000006538EFE1: FF 15 79 79 30 00  call        qword ptr [0000000065696960h]
  000000006538EFE7: E9 3B 01 00 00     jmp         000000006538F127
  000000006538EFEC: C6 86 1C 02 00 00  mov         byte ptr [rsi+000000000000021Ch],1
                    01
  000000006538EFF3: 48 8B 86 E0 01 00  mov         rax,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538EFFA: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538EFFD: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538F004: FF 15 46 98 30 00  call        qword ptr [0000000065698850h]
  000000006538F00A: 4C 8B C8           mov         r9,rax
  000000006538F00D: 45 8B C4           mov         r8d,r12d
  000000006538F010: 41 8B D6           mov         edx,r14d
  000000006538F013: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F01A: FF 93 D0 00 00 00  call        qword ptr [rbx+00000000000000D0h]
  000000006538F020: 84 C0              test        al,al
  000000006538F022: 0F 84 F8 00 00 00  je          000000006538F120
  000000006538F028: C7 44 24 48 FF FF  mov         dword ptr [rsp+48h],0FFFFFFFFh
                    FF FF
  000000006538F030: C7 44 24 4C FF FF  mov         dword ptr [rsp+4Ch],0FFFFFFFFh
                    FF FF
  000000006538F038: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  000000006538F03D: 48 89 6C 24 58     mov         qword ptr [rsp+58h],rbp
  000000006538F042: 45 85 E4           test        r12d,r12d
  000000006538F045: 0F 8E A8 00 00 00  jle         000000006538F0F3
  000000006538F04B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006538F050: 48 8B 86 E0 01 00  mov         rax,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F057: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538F05A: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538F061: FF 15 E9 97 30 00  call        qword ptr [0000000065698850h]
  000000006538F067: 46 8D 44 35 00     lea         r8d,[rbp+r14]
  000000006538F06C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538F071: 44 8B 8E 18 02 00  mov         r9d,dword ptr [rsi+0000000000000218h]
                    00
  000000006538F078: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006538F07D: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F084: FF 53 58           call        qword ptr [rbx+58h]
  000000006538F087: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006538F08C: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006538F08F: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006538F092: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006538F096: 48 89 4A 08        mov         qword ptr [rdx+8],rcx
  000000006538F09A: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000006538F09E: 48 89 42 10        mov         qword ptr [rdx+10h],rax
  000000006538F0A2: 8B D5              mov         edx,ebp
  000000006538F0A4: 49 8B CD           mov         rcx,r13
  000000006538F0A7: FF 15 93 78 30 00  call        qword ptr [0000000065696940h]
  000000006538F0AD: 48 8B D0           mov         rdx,rax
  000000006538F0B0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538F0B5: FF 15 A5 81 30 00  call        qword ptr [0000000065697260h]
  000000006538F0BB: 90                 nop
  000000006538F0BC: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F0C3: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006538F0C6: 41 B9 02 00 00 00  mov         r9d,2
  000000006538F0CC: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  000000006538F0D1: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006538F0D6: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  000000006538F0DC: 90                 nop
  000000006538F0DD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538F0E2: FF 15 F8 75 30 00  call        qword ptr [00000000656966E0h]
  000000006538F0E8: FF C5              inc         ebp
  000000006538F0EA: 41 3B EC           cmp         ebp,r12d
  000000006538F0ED: 0F 8C 5D FF FF FF  jl          000000006538F050
  000000006538F0F3: C6 86 1C 02 00 00  mov         byte ptr [rsi+000000000000021Ch],0
                    00
  000000006538F0FA: 43 8D 5C 34 FF     lea         ebx,[r12+r14-1]
  000000006538F0FF: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538F106: FF 15 44 97 30 00  call        qword ptr [0000000065698850h]
  000000006538F10C: 48 8B D0           mov         rdx,rax
  000000006538F10F: 44 8B CB           mov         r9d,ebx
  000000006538F112: 45 8B C6           mov         r8d,r14d
  000000006538F115: 48 8B CE           mov         rcx,rsi
  000000006538F118: E8 A3 F1 FF FF     call        000000006538E2C0
  000000006538F11D: 90                 nop
  000000006538F11E: EB 07              jmp         000000006538F127
  000000006538F120: 40 88 AE 1C 02 00  mov         byte ptr [rsi+000000000000021Ch],bpl
                    00
  000000006538F127: 49 8B 7F 08        mov         rdi,qword ptr [r15+8]
  000000006538F12B: 48 8B 87 E0 01 00  mov         rax,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538F132: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538F135: 48 8D 8F 50 02 00  lea         rcx,[rdi+0000000000000250h]
                    00
  000000006538F13C: FF 15 0E 97 30 00  call        qword ptr [0000000065698850h]
  000000006538F142: 48 8B D0           mov         rdx,rax
  000000006538F145: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538F14C: FF 53 68           call        qword ptr [rbx+68h]
  000000006538F14F: 8B E8              mov         ebp,eax
  000000006538F151: 3B 86 14 02 00 00  cmp         eax,dword ptr [rsi+0000000000000214h]
  000000006538F157: 7E 34              jle         000000006538F18D
  000000006538F159: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F160: 48 8B 39           mov         rdi,qword ptr [rcx]
  000000006538F163: 8B 9E 14 02 00 00  mov         ebx,dword ptr [rsi+0000000000000214h]
  000000006538F169: 48 8D 8E 50 02 00  lea         rcx,[rsi+0000000000000250h]
                    00
  000000006538F170: FF 15 DA 96 30 00  call        qword ptr [0000000065698850h]
  000000006538F176: 4C 8B C8           mov         r9,rax
  000000006538F179: 2B EB              sub         ebp,ebx
  000000006538F17B: 44 8B C5           mov         r8d,ebp
  000000006538F17E: 8B D3              mov         edx,ebx
  000000006538F180: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006538F187: FF 97 E0 00 00 00  call        qword ptr [rdi+00000000000000E0h]
  000000006538F18D: 48 8B 9C 24 D0 00  mov         rbx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006538F195: 48 81 C4 80 00 00  add         rsp,80h
                    00
  000000006538F19C: 41 5F              pop         r15
  000000006538F19E: 41 5E              pop         r14
  000000006538F1A0: 41 5D              pop         r13
  000000006538F1A2: 41 5C              pop         r12
  000000006538F1A4: 5F                 pop         rdi
  000000006538F1A5: 5E                 pop         rsi
  000000006538F1A6: 5D                 pop         rbp
  000000006538F1A7: C3                 ret

  Summary

      695000 .text
