Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400322A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400322A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400322AA: 48 83 EC 48        sub         rsp,48h
  00000001400322AE: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001400322B3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400322B8: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  00000001400322BF: E8 2C D1 CB 00     call        0000000140CEF3F0
  00000001400322C4: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001400322C9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400322CE: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  00000001400322D5: E8 36 09 CC 00     call        0000000140CF2C10
  00000001400322DA: BA 01 00 00 00     mov         edx,1
  00000001400322DF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400322E4: E8 F7 67 FE FF     call        0000000140018AE0
  00000001400322E9: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  00000001400322EE: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400322F6: 4C 8D 0D FB AA 53  lea         r9,[000000014356CDF8h]
                    03
  00000001400322FD: 4C 8D 05 54 52 53  lea         r8,[0000000143567558h]
                    03
  0000000140032304: 33 D2              xor         edx,edx
  0000000140032306: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014003230B: E8 EC D5 43 01     call        000000014146F8FC
  0000000140032310: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140032315: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014003231B: 74 0E              je          000000014003232B
  000000014003231D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140032322: E8 B9 D2 00 00     call        000000014003F5E0
  0000000140032327: 88 44 24 38        mov         byte ptr [rsp+38h],al
  000000014003232B: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140032330: 85 C0              test        eax,eax
  0000000140032332: 74 0A              je          000000014003233E
  0000000140032334: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140032339: E8 C2 02 00 00     call        0000000140032600
  000000014003233E: 48 83 C4 48        add         rsp,48h
  0000000140032342: C3                 ret

  Summary

     1E02000 .text
