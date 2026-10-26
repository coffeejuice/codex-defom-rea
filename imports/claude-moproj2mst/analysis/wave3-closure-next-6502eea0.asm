Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006502EEA0: 40 57              push        rdi
  000000006502EEA2: 48 83 EC 30        sub         rsp,30h
  000000006502EEA6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006502EEAF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006502EEB4: 80 3D B5 A9 99 00  cmp         byte ptr [00000000659C9870h],0
                    00
  000000006502EEBB: 75 40              jne         000000006502EEFD
  000000006502EEBD: 33 DB              xor         ebx,ebx
  000000006502EEBF: 48 8D 3D FA A8 99  lea         rdi,[00000000659C97C0h]
                    00
  000000006502EEC6: B9 40 00 00 00     mov         ecx,40h
  000000006502EECB: E8 A2 22 5B 00     call        00000000655E1172
  000000006502EED0: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006502EED5: 48 85 C0           test        rax,rax
  000000006502EED8: 74 0C              je          000000006502EEE6
  000000006502EEDA: 8B D3              mov         edx,ebx
  000000006502EEDC: 48 8B C8           mov         rcx,rax
  000000006502EEDF: E8 1C C4 05 00     call        000000006508B300
  000000006502EEE4: EB 02              jmp         000000006502EEE8
  000000006502EEE6: 33 C0              xor         eax,eax
  000000006502EEE8: 48 89 07           mov         qword ptr [rdi],rax
  000000006502EEEB: FF C3              inc         ebx
  000000006502EEED: 48 83 C7 08        add         rdi,8
  000000006502EEF1: 83 FB 15           cmp         ebx,15h
  000000006502EEF4: 7E D0              jle         000000006502EEC6
  000000006502EEF6: C6 05 73 A9 99 00  mov         byte ptr [00000000659C9870h],1
                    01
  000000006502EEFD: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006502EF02: 48 83 C4 30        add         rsp,30h
  000000006502EF06: 5F                 pop         rdi
  000000006502EF07: C3                 ret

  Summary

      695000 .text
