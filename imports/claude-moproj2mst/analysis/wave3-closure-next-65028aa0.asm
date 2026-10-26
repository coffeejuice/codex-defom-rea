Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065028AA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065028AA5: 57                 push        rdi
  0000000065028AA6: 48 83 EC 40        sub         rsp,40h
  0000000065028AAA: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065028AB3: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  0000000065028AB8: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  0000000065028ABD: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  0000000065028AC2: 41 8B F9           mov         edi,r9d
  0000000065028AC5: 49 8B F0           mov         rsi,r8
  0000000065028AC8: 48 8B EA           mov         rbp,rdx
  0000000065028ACB: 48 8B D9           mov         rbx,rcx
  0000000065028ACE: B9 D0 01 00 00     mov         ecx,1D0h
  0000000065028AD3: E8 9A 86 5B 00     call        00000000655E1172
  0000000065028AD8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000065028ADD: 48 85 C0           test        rax,rax
  0000000065028AE0: 74 1A              je          0000000065028AFC
  0000000065028AE2: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  0000000065028AE6: 41 B9 01 00 00 00  mov         r9d,1
  0000000065028AEC: 4C 8B C6           mov         r8,rsi
  0000000065028AEF: 48 8B D5           mov         rdx,rbp
  0000000065028AF2: 48 8B C8           mov         rcx,rax
  0000000065028AF5: E8 66 3C FF FF     call        000000006501C760
  0000000065028AFA: EB 02              jmp         0000000065028AFE
  0000000065028AFC: 33 C0              xor         eax,eax
  0000000065028AFE: 48 8B D0           mov         rdx,rax
  0000000065028B01: 48 8B CB           mov         rcx,rbx
  0000000065028B04: FF 15 26 E3 66 00  call        qword ptr [0000000065696E30h]
  0000000065028B0A: 90                 nop
  0000000065028B0B: 48 8D 05 A6 68 67  lea         rax,[000000006569F3B8h]
                    00
  0000000065028B12: 48 89 03           mov         qword ptr [rbx],rax
  0000000065028B15: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065028B19: E8 42 FD FF FF     call        0000000065028860
  0000000065028B1E: 90                 nop
  0000000065028B1F: 48 8B C3           mov         rax,rbx
  0000000065028B22: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  0000000065028B27: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  0000000065028B2C: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  0000000065028B31: 48 83 C4 40        add         rsp,40h
  0000000065028B35: 5F                 pop         rdi
  0000000065028B36: C3                 ret

  Summary

      695000 .text
