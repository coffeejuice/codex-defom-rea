Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CCAE60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CCAE65: 56                 push        rsi
  0000000140CCAE66: 57                 push        rdi
  0000000140CCAE67: 48 83 EC 48        sub         rsp,48h
  0000000140CCAE6B: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAE70: C7 80 1C 0C 00 00  mov         dword ptr [rax+0000000000000C1Ch],1
                    01 00 00 00
  0000000140CCAE7A: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAE7F: C7 80 DC 09 00 00  mov         dword ptr [rax+00000000000009DCh],0
                    00 00 00 00
  0000000140CCAE89: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAE8E: C7 80 E0 09 00 00  mov         dword ptr [rax+00000000000009E0h],0
                    00 00 00 00
  0000000140CCAE98: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAE9D: 48 C7 80 00 0C 00  mov         qword ptr [rax+0000000000000C00h],0
                    00 00 00 00 00
  0000000140CCAEA8: 66 0F 57 D2        xorpd       xmm2,xmm2
  0000000140CCAEAC: 66 0F 57 C9        xorpd       xmm1,xmm1
  0000000140CCAEB0: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140CCAEB5: FF 15 1D A9 13 01  call        qword ptr [0000000141E057D8h]
  0000000140CCAEBB: 48 8D 7C 24 30     lea         rdi,[rsp+30h]
  0000000140CCAEC0: 48 8B F0           mov         rsi,rax
  0000000140CCAEC3: B9 10 00 00 00     mov         ecx,10h
  0000000140CCAEC8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CCAECA: 48 8B 7C 24 60     mov         rdi,qword ptr [rsp+60h]
  0000000140CCAECF: 48 8D 74 24 30     lea         rsi,[rsp+30h]
  0000000140CCAED4: 48 81 C7 08 0C 00  add         rdi,0C08h
                    00
  0000000140CCAEDB: B9 10 00 00 00     mov         ecx,10h
  0000000140CCAEE0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CCAEE2: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAEE7: 48 C7 80 E8 0B 00  mov         qword ptr [rax+0000000000000BE8h],0
                    00 00 00 00 00
  0000000140CCAEF2: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140CCAEF7: 48 C7 80 A8 0C 00  mov         qword ptr [rax+0000000000000CA8h],0
                    00 00 00 00 00
  0000000140CCAF02: 48 83 C4 48        add         rsp,48h
  0000000140CCAF06: 5F                 pop         rdi
  0000000140CCAF07: 5E                 pop         rsi
  0000000140CCAF08: C3                 ret

  Summary

     1E02000 .text
