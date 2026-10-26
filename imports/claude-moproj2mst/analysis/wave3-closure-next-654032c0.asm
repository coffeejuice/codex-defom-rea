Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654032C0: 40 57              push        rdi
  00000000654032C2: 48 83 EC 30        sub         rsp,30h
  00000000654032C6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654032CF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000654032D4: 48 8B D9           mov         rbx,rcx
  00000000654032D7: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000654032DB: B9 28 00 00 00     mov         ecx,28h
  00000000654032E0: E8 8D DE 1D 00     call        00000000655E1172
  00000000654032E5: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000654032EA: 48 85 C0           test        rax,rax
  00000000654032ED: 74 14              je          0000000065403303
  00000000654032EF: 4C 8B C3           mov         r8,rbx
  00000000654032F2: 8B 97 18 02 00 00  mov         edx,dword ptr [rdi+0000000000000218h]
  00000000654032F8: 48 8B C8           mov         rcx,rax
  00000000654032FB: E8 00 F8 FF FF     call        0000000065402B00
  0000000065403300: 90                 nop
  0000000065403301: EB 02              jmp         0000000065403305
  0000000065403303: 33 C0              xor         eax,eax
  0000000065403305: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006540330A: 48 83 C4 30        add         rsp,30h
  000000006540330E: 5F                 pop         rdi
  000000006540330F: C3                 ret

  Summary

      695000 .text
