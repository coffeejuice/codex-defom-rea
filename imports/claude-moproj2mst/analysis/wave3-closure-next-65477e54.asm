Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065477E54: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065477E59: 48 8B 5E 08        mov         rbx,qword ptr [rsi+8]
  0000000065477E5D: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  0000000065477E62: E8 79 F2 FF FF     call        00000000654770E0
  0000000065477E67: 48 8B 8B 58 02 00  mov         rcx,qword ptr [rbx+0000000000000258h]
                    00
  0000000065477E6E: 41 B8 03 00 00 00  mov         r8d,3
  0000000065477E74: 48 8B D5           mov         rdx,rbp
  0000000065477E77: E8 B4 4D F2 FF     call        000000006539CC30
  0000000065477E7C: 48 8D 8B 60 02 00  lea         rcx,[rbx+0000000000000260h]
                    00
  0000000065477E83: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065477E88: E8 F3 C1 D2 FF     call        00000000651A4080
  0000000065477E8D: C6 83 91 02 00 00  mov         byte ptr [rbx+0000000000000291h],0
                    00
  0000000065477E94: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065477E99: 48 8B CF           mov         rcx,rdi
  0000000065477E9C: E8 0F E0 FF FF     call        0000000065475EB0
  0000000065477EA1: 48 8B CF           mov         rcx,rdi
  0000000065477EA4: E8 87 C7 FF FF     call        0000000065474630
  0000000065477EA9: 33 D2              xor         edx,edx
  0000000065477EAB: 48 8B CE           mov         rcx,rsi
  0000000065477EAE: 44 8D 42 02        lea         r8d,[rdx+2]
  0000000065477EB2: E8 A9 AA 02 00     call        00000000654A2960
  0000000065477EB7: 48 8B CE           mov         rcx,rsi
  0000000065477EBA: E8 81 1E BF FF     call        0000000065069D40
  0000000065477EBF: 33 D2              xor         edx,edx
  0000000065477EC1: 48 8B C8           mov         rcx,rax
  0000000065477EC4: FF 15 D6 19 22 00  call        qword ptr [00000000656998A0h]
  0000000065477ECA: 48 83 BF 98 02 00  cmp         qword ptr [rdi+0000000000000298h],0
                    00 00
  0000000065477ED2: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  0000000065477ED7: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  0000000065477EDC: 75 08              jne         0000000065477EE6
  0000000065477EDE: 41 B8 01 00 00 00  mov         r8d,1
  0000000065477EE4: EB 03              jmp         0000000065477EE9
  0000000065477EE6: 45 33 C0           xor         r8d,r8d
  0000000065477EE9: BA 60 F0 00 00     mov         edx,0F060h
  0000000065477EEE: 48 8B C8           mov         rcx,rax
  0000000065477EF1: FF 15 B1 19 22 00  call        qword ptr [00000000656998A8h]
  0000000065477EF7: 49 8B D4           mov         rdx,r12
  0000000065477EFA: 48 8B CE           mov         rcx,rsi
  0000000065477EFD: 48 83 C4 28        add         rsp,28h
  0000000065477F01: 41 5C              pop         r12
  0000000065477F03: 5E                 pop         rsi
  0000000065477F04: E9 B7 30 FE FF     jmp         000000006545AFC0
  0000000065477F09: CC                 int         3
  0000000065477F0A: CC                 int         3
  0000000065477F0B: CC                 int         3
  0000000065477F0C: CC                 int         3
  0000000065477F0D: CC                 int         3
  0000000065477F0E: CC                 int         3
  0000000065477F0F: CC                 int         3
  0000000065477F10: 4C 8B DC           mov         r11,rsp
  0000000065477F13: 41 54              push        r12
  0000000065477F15: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  0000000065477F1C: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000065477F28: 49 89 5B 08        mov         qword ptr [r11+8],rbx
  0000000065477F2C: 49 89 6B 10        mov         qword ptr [r11+10h],rbp
  0000000065477F30: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  0000000065477F34: 49 89 7B 20        mov         qword ptr [r11+20h],rdi
  0000000065477F38: C7 44 24 50 03 01  mov         dword ptr [rsp+50h],103h
                    00 00
  0000000065477F40: 45 33 E4           xor         r12d,r12d
  0000000065477F43: 45 89 63 A8        mov         dword ptr [r11-58h],r12d
  0000000065477F47: 49 8D 43 B8        lea         rax,[r11-48h]
  0000000065477F4B: 49 89 43 98        mov         qword ptr [r11-68h],rax
  0000000065477F4F: 49 89 4B 90        mov         qword ptr [r11-70h],rcx
  0000000065477F53: 49 8D 43 A8        lea         rax,[r11-58h]
  0000000065477F57: 49 89 43 88        mov         qword ptr [r11-78h],rax
  0000000065477F5B: 49 8D 4B C0        lea         rcx,[r11-40h]
  0000000065477F5F: E8 DC ED FF FF     call        0000000065476D40
  0000000065477F64: 90                 nop
  0000000065477F65: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000065477F6D: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000065477F72: FF 15 78 EA 21 00  call        qword ptr [00000000656969F0h]
  0000000065477F78: 90                 nop
  0000000065477F79: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000065477F7E: FF 15 64 EA 21 00  call        qword ptr [00000000656969E8h]
  0000000065477F84: 84 C0              test        al,al
  0000000065477F86: 74 2F              je          0000000065477FB7
  0000000065477F88: 45 33 C0           xor         r8d,r8d
  0000000065477F8B: 48 8D 15 66 5A 22  lea         rdx,[000000006569D9F8h]
                    00
  0000000065477F92: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065477F97: E8 54 6B E3 FF     call        00000000652AEAF0
  0000000065477F9C: 90                 nop
  0000000065477F9D: 48 8B D0           mov         rdx,rax
  0000000065477FA0: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000065477FA5: FF 15 D5 E7 21 00  call        qword ptr [0000000065696780h]
  0000000065477FAB: 90                 nop
  0000000065477FAC: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065477FB1: FF 15 A9 E7 21 00  call        qword ptr [0000000065696760h]
  0000000065477FB7: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  0000000065477FBC: B9 28 00 00 00     mov         ecx,28h
  0000000065477FC1: E8 AC 91 16 00     call        00000000655E1172
  0000000065477FC6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000065477FCB: 48 85 C0           test        rax,rax
  0000000065477FCE: 74 15              je          0000000065477FE5
  0000000065477FD0: 45 33 C0           xor         r8d,r8d
  0000000065477FD3: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000065477FD8: 48 8B C8           mov         rcx,rax
  0000000065477FDB: E8 90 02 F8 FF     call        00000000653F8270
  0000000065477FE0: 48 8B D8           mov         rbx,rax
  0000000065477FE3: EB 03              jmp         0000000065477FE8
  0000000065477FE5: 49 8B DC           mov         rbx,r12
  0000000065477FE8: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000065477FED: 48 8B 7C 24 60     mov         rdi,qword ptr [rsp+60h]
  0000000065477FF2: 48 85 DB           test        rbx,rbx
  0000000065477FF5: 74 7A              je          0000000065478071
  0000000065477FF7: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  0000000065477FFC: 48
  0000000065477FFD: 8D
  0000000065477FFE: 54                 push        rsp
  0000000065477FFF: 24

  Summary

      695000 .text
