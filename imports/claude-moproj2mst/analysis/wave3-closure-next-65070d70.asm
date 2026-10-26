Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065070D70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065070D75: 56                 push        rsi
  0000000065070D76: 57                 push        rdi
  0000000065070D77: 41 54              push        r12
  0000000065070D79: 48 83 EC 30        sub         rsp,30h
  0000000065070D7D: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065070D86: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  0000000065070D8B: 49 8B F0           mov         rsi,r8
  0000000065070D8E: 4C 8B E2           mov         r12,rdx
  0000000065070D91: 48 8B F9           mov         rdi,rcx
  0000000065070D94: B9 E0 01 00 00     mov         ecx,1E0h
  0000000065070D99: E8 D4 03 57 00     call        00000000655E1172
  0000000065070D9E: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000065070DA3: 48 85 C0           test        rax,rax
  0000000065070DA6: 74 0F              je          0000000065070DB7
  0000000065070DA8: BA 04 08 04 00     mov         edx,40804h
  0000000065070DAD: 48 8B C8           mov         rcx,rax
  0000000065070DB0: E8 CB D9 FE FF     call        000000006505E780
  0000000065070DB5: EB 02              jmp         0000000065070DB9
  0000000065070DB7: 33 C0              xor         eax,eax
  0000000065070DB9: 45 33 C0           xor         r8d,r8d
  0000000065070DBC: 48 8B D0           mov         rdx,rax
  0000000065070DBF: 48 8B CF           mov         rcx,rdi
  0000000065070DC2: FF 15 68 5C 62 00  call        qword ptr [0000000065696A30h]
  0000000065070DC8: 90                 nop
  0000000065070DC9: 48 8D 4F 10        lea         rcx,[rdi+10h]
  0000000065070DCD: E8 6E 73 0C 00     call        0000000065138140
  0000000065070DD2: 90                 nop
  0000000065070DD3: 48 8D 05 BE AA 63  lea         rax,[00000000656AB898h]
                    00
  0000000065070DDA: 48 89 07           mov         qword ptr [rdi],rax
  0000000065070DDD: 48 8D 05 7C AA 63  lea         rax,[00000000656AB860h]
                    00
  0000000065070DE4: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  0000000065070DE8: 8B 06              mov         eax,dword ptr [rsi]
  0000000065070DEA: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  0000000065070DEE: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000065070DF3: 49 8B D4           mov         rdx,r12
  0000000065070DF6: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  0000000065070DFA: E8 D1 D6 FF FF     call        000000006506E4D0
  0000000065070DFF: 90                 nop
  0000000065070E00: 48 8B C7           mov         rax,rdi
  0000000065070E03: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  0000000065070E08: 48 83 C4 30        add         rsp,30h
  0000000065070E0C: 41 5C              pop         r12
  0000000065070E0E: 5F                 pop         rdi
  0000000065070E0F: 5E                 pop         rsi
  0000000065070E10: C3                 ret

  Summary

      695000 .text
