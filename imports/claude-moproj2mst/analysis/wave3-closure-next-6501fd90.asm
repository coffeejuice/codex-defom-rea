Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006501FD90: 40 53              push        rbx
  000000006501FD92: 48 83 EC 30        sub         rsp,30h
  000000006501FD96: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006501FD9F: 48 8B 05 EA 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FDA6: 48 85 C0           test        rax,rax
  000000006501FDA9: 0F 85 D5 00 00 00  jne         000000006501FE84
  000000006501FDAF: 38 05 E3 99 9A 00  cmp         byte ptr [00000000659C9798h],al
  000000006501FDB5: 0F 85 C0 00 00 00  jne         000000006501FE7B
  000000006501FDBB: 8D 48 08           lea         ecx,[rax+8]
  000000006501FDBE: E8 AF 13 5C 00     call        00000000655E1172
  000000006501FDC3: 48 8B D8           mov         rbx,rax
  000000006501FDC6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006501FDCB: 48 85 C0           test        rax,rax
  000000006501FDCE: 74 1D              je          000000006501FDED
  000000006501FDD0: 48 8B 05 61 6D 67  mov         rax,qword ptr [0000000065696B38h]
                    00
  000000006501FDD7: 48 89 03           mov         qword ptr [rbx],rax
  000000006501FDDA: 48 8B 0D 57 6D 67  mov         rcx,qword ptr [0000000065696B38h]
                    00
  000000006501FDE1: 48 83 C1 10        add         rcx,10h
  000000006501FDE5: FF 15 D5 6A 67 00  call        qword ptr [00000000656968C0h]
  000000006501FDEB: EB 02              jmp         000000006501FDEF
  000000006501FDED: 33 DB              xor         ebx,ebx
  000000006501FDEF: 33 C0              xor         eax,eax
  000000006501FDF1: F0 48 0F B1 1D 96  lock cmpxchg qword ptr [00000000659C9790h],rbx
                    99 9A 00
  000000006501FDFA: 74 40              je          000000006501FE3C
  000000006501FDFC: 48 85 DB           test        rbx,rbx
  000000006501FDFF: 74 32              je          000000006501FE33
  000000006501FE01: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006501FE04: 48 83 C1 10        add         rcx,10h
  000000006501FE08: FF 15 6A 6B 67 00  call        qword ptr [0000000065696978h]
  000000006501FE0E: 84 C0              test        al,al
  000000006501FE10: 75 10              jne         000000006501FE22
  000000006501FE12: 48 8D 15 87 AB FF  lea         rdx,[000000006501A9A0h]
                    FF
  000000006501FE19: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006501FE1C: FF 15 8E 6D 67 00  call        qword ptr [0000000065696BB0h]
  000000006501FE22: 48 8B CB           mov         rcx,rbx
  000000006501FE25: E8 84 12 5C 00     call        00000000655E10AE
  000000006501FE2A: 48 8B 05 5F 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE31: EB 58              jmp         000000006501FE8B
  000000006501FE33: 48 8B 05 56 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE3A: EB 4F              jmp         000000006501FE8B
  000000006501FE3C: 8B 05 46 99 9A 00  mov         eax,dword ptr [00000000659C9788h]
  000000006501FE42: A8 01              test        al,1
  000000006501FE44: 75 2C              jne         000000006501FE72
  000000006501FE46: 83 C8 01           or          eax,1
  000000006501FE49: 89 05 39 99 9A 00  mov         dword ptr [00000000659C9788h],eax
  000000006501FE4F: 48 8D 05 3A 99 9A  lea         rax,[00000000659C9790h]
                    00
  000000006501FE56: 48 89 05 23 99 9A  mov         qword ptr [00000000659C9780h],rax
                    00
  000000006501FE5D: 48 8D 0D 1C 48 67  lea         rcx,[0000000065694680h]
                    00
  000000006501FE64: E8 C3 11 5C 00     call        00000000655E102C
  000000006501FE69: 48 8B 05 20 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE70: EB 19              jmp         000000006501FE8B
  000000006501FE72: 48 8B 05 17 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE79: EB 10              jmp         000000006501FE8B
  000000006501FE7B: 48 8B 05 0E 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE82: EB 07              jmp         000000006501FE8B
  000000006501FE84: 48 8B 05 05 99 9A  mov         rax,qword ptr [00000000659C9790h]
                    00
  000000006501FE8B: 48 83 C4 30        add         rsp,30h
  000000006501FE8F: 5B                 pop         rbx
  000000006501FE90: C3                 ret

  Summary

      695000 .text
