Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006501FEA0: 40 53              push        rbx
  000000006501FEA2: 48 83 EC 30        sub         rsp,30h
  000000006501FEA6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006501FEAF: 48 8B 05 FA 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FEB6: 48 85 C0           test        rax,rax
  000000006501FEB9: 0F 85 D5 00 00 00  jne         000000006501FF94
  000000006501FEBF: 38 05 F3 98 9A 00  cmp         byte ptr [00000000659C97B8h],al
  000000006501FEC5: 0F 85 C0 00 00 00  jne         000000006501FF8B
  000000006501FECB: 8D 48 08           lea         ecx,[rax+8]
  000000006501FECE: E8 9F 12 5C 00     call        00000000655E1172
  000000006501FED3: 48 8B D8           mov         rbx,rax
  000000006501FED6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006501FEDB: 48 85 C0           test        rax,rax
  000000006501FEDE: 74 1D              je          000000006501FEFD
  000000006501FEE0: 48 8B 05 51 6C 67  mov         rax,qword ptr [0000000065696B38h]
                    00
  000000006501FEE7: 48 89 03           mov         qword ptr [rbx],rax
  000000006501FEEA: 48 8B 0D 47 6C 67  mov         rcx,qword ptr [0000000065696B38h]
                    00
  000000006501FEF1: 48 83 C1 10        add         rcx,10h
  000000006501FEF5: FF 15 C5 69 67 00  call        qword ptr [00000000656968C0h]
  000000006501FEFB: EB 02              jmp         000000006501FEFF
  000000006501FEFD: 33 DB              xor         ebx,ebx
  000000006501FEFF: 33 C0              xor         eax,eax
  000000006501FF01: F0 48 0F B1 1D A6  lock cmpxchg qword ptr [00000000659C97B0h],rbx
                    98 9A 00
  000000006501FF0A: 74 40              je          000000006501FF4C
  000000006501FF0C: 48 85 DB           test        rbx,rbx
  000000006501FF0F: 74 32              je          000000006501FF43
  000000006501FF11: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006501FF14: 48 83 C1 10        add         rcx,10h
  000000006501FF18: FF 15 5A 6A 67 00  call        qword ptr [0000000065696978h]
  000000006501FF1E: 84 C0              test        al,al
  000000006501FF20: 75 10              jne         000000006501FF32
  000000006501FF22: 48 8D 15 A7 AA FF  lea         rdx,[000000006501A9D0h]
                    FF
  000000006501FF29: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006501FF2C: FF 15 7E 6C 67 00  call        qword ptr [0000000065696BB0h]
  000000006501FF32: 48 8B CB           mov         rcx,rbx
  000000006501FF35: E8 74 11 5C 00     call        00000000655E10AE
  000000006501FF3A: 48 8B 05 6F 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF41: EB 58              jmp         000000006501FF9B
  000000006501FF43: 48 8B 05 66 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF4A: EB 4F              jmp         000000006501FF9B
  000000006501FF4C: 8B 05 56 98 9A 00  mov         eax,dword ptr [00000000659C97A8h]
  000000006501FF52: A8 01              test        al,1
  000000006501FF54: 75 2C              jne         000000006501FF82
  000000006501FF56: 83 C8 01           or          eax,1
  000000006501FF59: 89 05 49 98 9A 00  mov         dword ptr [00000000659C97A8h],eax
  000000006501FF5F: 48 8D 05 4A 98 9A  lea         rax,[00000000659C97B0h]
                    00
  000000006501FF66: 48 89 05 33 98 9A  mov         qword ptr [00000000659C97A0h],rax
                    00
  000000006501FF6D: 48 8D 0D 6C 47 67  lea         rcx,[00000000656946E0h]
                    00
  000000006501FF74: E8 B3 10 5C 00     call        00000000655E102C
  000000006501FF79: 48 8B 05 30 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF80: EB 19              jmp         000000006501FF9B
  000000006501FF82: 48 8B 05 27 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF89: EB 10              jmp         000000006501FF9B
  000000006501FF8B: 48 8B 05 1E 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF92: EB 07              jmp         000000006501FF9B
  000000006501FF94: 48 8B 05 15 98 9A  mov         rax,qword ptr [00000000659C97B0h]
                    00
  000000006501FF9B: 48 83 C4 30        add         rsp,30h
  000000006501FF9F: 5B                 pop         rbx
  000000006501FFA0: C3                 ret

  Summary

      695000 .text
