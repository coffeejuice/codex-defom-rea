Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C832D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C832D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C832DA: 48 83 EC 58        sub         rsp,58h
  0000000140C832DE: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C832E7: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C832EC: 83 38 00           cmp         dword ptr [rax],0
  0000000140C832EF: 7E 1B              jle         0000000140C8330C
  0000000140C832F1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C832F6: 48 83 C1 20        add         rcx,20h
  0000000140C832FA: E8 E1 17 38 FF     call        0000000140004AE0
  0000000140C832FF: 44 8B D8           mov         r11d,eax
  0000000140C83302: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C83307: 44 39 18           cmp         dword ptr [rax],r11d
  0000000140C8330A: 7E 07              jle         0000000140C83313
  0000000140C8330C: 33 C0              xor         eax,eax
  0000000140C8330E: E9 95 00 00 00     jmp         0000000140C833A8
  0000000140C83313: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C83318: 48 83 C1 20        add         rcx,20h
  0000000140C8331C: E8 BF 17 38 FF     call        0000000140004AE0
  0000000140C83321: 44 8B D8           mov         r11d,eax
  0000000140C83324: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C83329: 44 3B 18           cmp         r11d,dword ptr [rax]
  0000000140C8332C: 7D 5F              jge         0000000140C8338D
  0000000140C8332E: B9 48 00 00 00     mov         ecx,48h
  0000000140C83333: E8 84 B7 7E 00     call        000000014146EABC
  0000000140C83338: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C8333D: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140C83343: 74 16              je          0000000140C8335B
  0000000140C83345: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140C8334A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C8334F: E8 5C B2 56 00     call        00000001411EE5B0
  0000000140C83354: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C83359: EB 09              jmp         0000000140C83364
  0000000140C8335B: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140C83364: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C83369: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C8336E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C83373: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C83378: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C8337D: 48 83 C1 20        add         rcx,20h
  0000000140C83381: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C83386: E8 55 A4 AA 00     call        000000014172D7E0
  0000000140C8338B: EB 86              jmp         0000000140C83313
  0000000140C8338D: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140C83392: 8B 10              mov         edx,dword ptr [rax]
  0000000140C83394: 83 EA 01           sub         edx,1
  0000000140C83397: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C8339C: 48 83 C1 20        add         rcx,20h
  0000000140C833A0: E8 0B 82 4C FF     call        000000014014B5B0
  0000000140C833A5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C833A8: 48 83 C4 58        add         rsp,58h
  0000000140C833AC: C3                 ret

  Summary

     1E02000 .text
