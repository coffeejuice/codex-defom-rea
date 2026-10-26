Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065506A20: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065506A25: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000065506A2A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000065506A2F: 57                 push        rdi
  0000000065506A30: 48 83 EC 20        sub         rsp,20h
  0000000065506A34: 48 8B D9           mov         rbx,rcx
  0000000065506A37: 8D 0C D5 10 00 00  lea         ecx,[rdx*8+0000000000000010h]
                    00
  0000000065506A3E: 8B F2              mov         esi,edx
  0000000065506A40: BA 08 00 00 00     mov         edx,8
  0000000065506A45: 49 8B F8           mov         rdi,r8
  0000000065506A48: FF 15 2A 01 19 00  call        qword ptr [0000000065696B78h]
  0000000065506A4E: 48 8B E8           mov         rbp,rax
  0000000065506A51: 48 85 C0           test        rax,rax
  0000000065506A54: 75 06              jne         0000000065506A5C
  0000000065506A56: FF 15 14 01 19 00  call        qword ptr [0000000065696B70h]
  0000000065506A5C: BA 01 00 00 00     mov         edx,1
  0000000065506A61: 48 8B CD           mov         rcx,rbp
  0000000065506A64: 48 89 2B           mov         qword ptr [rbx],rbp
  0000000065506A67: FF 15 53 01 19 00  call        qword ptr [0000000065696BC0h]
  0000000065506A6D: 4C 8B 1B           mov         r11,qword ptr [rbx]
  0000000065506A70: 41 89 73 08        mov         dword ptr [r11+8],esi
  0000000065506A74: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065506A77: 89 70 04           mov         dword ptr [rax+4],esi
  0000000065506A7A: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065506A7D: 83 48 0C 01        or          dword ptr [rax+0Ch],1
  0000000065506A81: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065506A84: 83 60 0C FD        and         dword ptr [rax+0Ch],0FFFFFFFDh
  0000000065506A88: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000065506A8B: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000065506A8F: 48 8D 54 C1 10     lea         rdx,[rcx+rax*8+10h]
  0000000065506A94: 48 8D 41 10        lea         rax,[rcx+10h]
  0000000065506A98: 48 3B D0           cmp         rdx,rax
  0000000065506A9B: 74 1B              je          0000000065506AB8
  0000000065506A9D: 0F 1F 00           nop         dword ptr [rax]
  0000000065506AA0: 48 83 EA 08        sub         rdx,8
  0000000065506AA4: 74 06              je          0000000065506AAC
  0000000065506AA6: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065506AA9: 48 89 02           mov         qword ptr [rdx],rax
  0000000065506AAC: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065506AAF: 48 83 C0 10        add         rax,10h
  0000000065506AB3: 48 3B D0           cmp         rdx,rax
  0000000065506AB6: 75 E8              jne         0000000065506AA0
  0000000065506AB8: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  0000000065506ABD: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  0000000065506AC2: 48 8B C3           mov         rax,rbx
  0000000065506AC5: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065506ACA: 48 83 C4 20        add         rsp,20h
  0000000065506ACE: 5F                 pop         rdi
  0000000065506ACF: C3                 ret

  Summary

      695000 .text
