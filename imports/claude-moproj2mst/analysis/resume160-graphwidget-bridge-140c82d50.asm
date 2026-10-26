Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C82D50: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C82D55: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C82D5A: 48 83 EC 58        sub         rsp,58h
  0000000140C82D5E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C82D67: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C82D6C: 83 38 00           cmp         dword ptr [rax],0
  0000000140C82D6F: 7F 07              jg          0000000140C82D78
  0000000140C82D71: 33 C0              xor         eax,eax
  0000000140C82D73: E9 B6 00 00 00     jmp         0000000140C82E2E
  0000000140C82D78: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C82D7D: 48 83 C1 18        add         rcx,18h
  0000000140C82D81: E8 5A 1D 38 FF     call        0000000140004AE0
  0000000140C82D86: 44 8B D8           mov         r11d,eax
  0000000140C82D89: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C82D8E: 44 3B 18           cmp         r11d,dword ptr [rax]
  0000000140C82D91: 7D 5F              jge         0000000140C82DF2
  0000000140C82D93: B9 C0 87 00 00     mov         ecx,87C0h
  0000000140C82D98: E8 1F BD 7E 00     call        000000014146EABC
  0000000140C82D9D: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C82DA2: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140C82DA8: 74 16              je          0000000140C82DC0
  0000000140C82DAA: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140C82DAF: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C82DB4: E8 17 DA 56 00     call        00000001411F07D0
  0000000140C82DB9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C82DBE: EB 09              jmp         0000000140C82DC9
  0000000140C82DC0: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  0000000140C82DC9: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140C82DCE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C82DD3: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C82DD8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C82DDD: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C82DE2: 48 83 C1 18        add         rcx,18h
  0000000140C82DE6: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C82DEB: E8 F0 A9 AA 00     call        000000014172D7E0
  0000000140C82DF0: EB 86              jmp         0000000140C82D78
  0000000140C82DF2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C82DF7: 48 83 C1 18        add         rcx,18h
  0000000140C82DFB: E8 E0 1C 38 FF     call        0000000140004AE0
  0000000140C82E00: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C82E04: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140C82E09: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C82E0E: E8 1D FB FF FF     call        0000000140C82930
  0000000140C82E13: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C82E18: 8B 10              mov         edx,dword ptr [rax]
  0000000140C82E1A: 83 EA 01           sub         edx,1
  0000000140C82E1D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C82E22: 48 83 C1 18        add         rcx,18h
  0000000140C82E26: E8 85 87 4C FF     call        000000014014B5B0
  0000000140C82E2B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C82E2E: 48 83 C4 58        add         rsp,58h
  0000000140C82E32: C3                 ret

  Summary

     1E02000 .text
