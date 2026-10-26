Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065016F70: 40 53              push        rbx
  0000000065016F72: 48 83 EC 30        sub         rsp,30h
  0000000065016F76: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065016F7F: 48 8B D9           mov         rbx,rcx
  0000000065016F82: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000065016F8A: 48 8B 05 FF 26 9B  mov         rax,qword ptr [00000000659C9690h]
                    00
  0000000065016F91: 48 85 C0           test        rax,rax
  0000000065016F94: 75 2A              jne         0000000065016FC0
  0000000065016F96: 8D 48 10           lea         ecx,[rax+10h]
  0000000065016F99: E8 D4 A1 5C 00     call        00000000655E1172
  0000000065016F9E: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000065016FA3: 48 85 C0           test        rax,rax
  0000000065016FA6: 74 0F              je          0000000065016FB7
  0000000065016FA8: BA 02 00 00 00     mov         edx,2
  0000000065016FAD: 48 8B C8           mov         rcx,rax
  0000000065016FB0: E8 CB 4F 03 00     call        000000006504BF80
  0000000065016FB5: EB 02              jmp         0000000065016FB9
  0000000065016FB7: 33 C0              xor         eax,eax
  0000000065016FB9: 48 89 05 D0 26 9B  mov         qword ptr [00000000659C9690h],rax
                    00
  0000000065016FC0: 48 8B D0           mov         rdx,rax
  0000000065016FC3: 48 8B CB           mov         rcx,rbx
  0000000065016FC6: E8 E5 38 03 00     call        000000006504A8B0
  0000000065016FCB: 48 8B C3           mov         rax,rbx
  0000000065016FCE: 48 83 C4 30        add         rsp,30h
  0000000065016FD2: 5B                 pop         rbx
  0000000065016FD3: C3                 ret

  Summary

      695000 .text
