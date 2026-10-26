Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065021A70: 40 53              push        rbx
  0000000065021A72: 48 83 EC 30        sub         rsp,30h
  0000000065021A76: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065021A7F: 48 8B D9           mov         rbx,rcx
  0000000065021A82: 48 8B 0D 37 7C 9A  mov         rcx,qword ptr [00000000659C96C0h]
                    00
  0000000065021A89: 48 85 C9           test        rcx,rcx
  0000000065021A8C: 75 2C              jne         0000000065021ABA
  0000000065021A8E: B9 10 00 00 00     mov         ecx,10h
  0000000065021A93: E8 DA F6 5B 00     call        00000000655E1172
  0000000065021A98: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000065021A9D: 48 85 C0           test        rax,rax
  0000000065021AA0: 74 0D              je          0000000065021AAF
  0000000065021AA2: 48 8B D3           mov         rdx,rbx
  0000000065021AA5: 48 8B C8           mov         rcx,rax
  0000000065021AA8: E8 13 76 21 00     call        00000000652390C0
  0000000065021AAD: EB 02              jmp         0000000065021AB1
  0000000065021AAF: 33 C0              xor         eax,eax
  0000000065021AB1: 48 89 05 08 7C 9A  mov         qword ptr [00000000659C96C0h],rax
                    00
  0000000065021AB8: EB 0F              jmp         0000000065021AC9
  0000000065021ABA: 48 8B D3           mov         rdx,rbx
  0000000065021ABD: E8 9E 15 1D 00     call        00000000651F3060
  0000000065021AC2: 48 8B 05 F7 7B 9A  mov         rax,qword ptr [00000000659C96C0h]
                    00
  0000000065021AC9: 48 83 3D F7 7B 9A  cmp         qword ptr [00000000659C96C8h],0
                    00 00
  0000000065021AD1: 75 0A              jne         0000000065021ADD
  0000000065021AD3: 33 D2              xor         edx,edx
  0000000065021AD5: 48 8B C8           mov         rcx,rax
  0000000065021AD8: E8 03 FD FF FF     call        00000000650217E0
  0000000065021ADD: 48 83 C4 30        add         rsp,30h
  0000000065021AE1: 5B                 pop         rbx
  0000000065021AE2: C3                 ret

  Summary

      695000 .text
