Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141202D60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141202D65: 48 83 EC 48        sub         rsp,48h
  0000000141202D69: 33 D2              xor         edx,edx
  0000000141202D6B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000141202D70: E8 1B 01 00 00     call        0000000141202E90
  0000000141202D75: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000141202D7A: 48 83 B8 88 43 00  cmp         qword ptr [rax+0000000000004388h],0
                    00 00
  0000000141202D82: 74 48              je          0000000141202DCC
  0000000141202D84: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000141202D89: 48 8B 80 88 43 00  mov         rax,qword ptr [rax+0000000000004388h]
                    00
  0000000141202D90: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000141202D95: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000141202D9A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141202D9F: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000141202DA5: 74 1C              je          0000000141202DC3
  0000000141202DA7: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141202DAC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000141202DAF: BA 01 00 00 00     mov         edx,1
  0000000141202DB4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141202DB9: FF 50 18           call        qword ptr [rax+18h]
  0000000141202DBC: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000141202DC1: EB 09              jmp         0000000141202DCC
  0000000141202DC3: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000141202DCC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000141202DD1: E8 FA 8B 00 00     call        000000014120B9D0
  0000000141202DD6: 48 83 C4 48        add         rsp,48h
  0000000141202DDA: C3                 ret

  Summary

     1E02000 .text
