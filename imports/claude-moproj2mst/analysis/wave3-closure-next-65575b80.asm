Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065575B80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065575B85: 57                 push        rdi
  0000000065575B86: 48 83 EC 30        sub         rsp,30h
  0000000065575B8A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065575B93: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065575B98: 48 8B FA           mov         rdi,rdx
  0000000065575B9B: 48 8B D9           mov         rbx,rcx
  0000000065575B9E: B9 F8 02 00 00     mov         ecx,2F8h
  0000000065575BA3: E8 CA B5 06 00     call        00000000655E1172
  0000000065575BA8: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065575BAD: 48 85 C0           test        rax,rax
  0000000065575BB0: 74 0A              je          0000000065575BBC
  0000000065575BB2: 48 8B C8           mov         rcx,rax
  0000000065575BB5: E8 B6 E2 FF FF     call        0000000065573E70
  0000000065575BBA: EB 02              jmp         0000000065575BBE
  0000000065575BBC: 33 C0              xor         eax,eax
  0000000065575BBE: 4C 8B C7           mov         r8,rdi
  0000000065575BC1: 48 8B D0           mov         rdx,rax
  0000000065575BC4: 48 8B CB           mov         rcx,rbx
  0000000065575BC7: FF 15 63 0E 12 00  call        qword ptr [0000000065696A30h]
  0000000065575BCD: 90                 nop
  0000000065575BCE: 48 8D 05 13 1F 1D  lea         rax,[0000000065747AE8h]
                    00
  0000000065575BD5: 48 89 03           mov         qword ptr [rbx],rax
  0000000065575BD8: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065575BDC: E8 9F 42 FF FF     call        0000000065569E80
  0000000065575BE1: 90                 nop
  0000000065575BE2: 48 8B C3           mov         rax,rbx
  0000000065575BE5: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065575BEA: 48 83 C4 30        add         rsp,30h
  0000000065575BEE: 5F                 pop         rdi
  0000000065575BEF: C3                 ret

  Summary

      695000 .text
