Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140646E90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140646E95: 48 83 EC 48        sub         rsp,48h
  0000000140646E99: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140646E9E: 48 83 C1 40        add         rcx,40h
  0000000140646EA2: E8 39 DC 9B FF     call        0000000140004AE0
  0000000140646EA7: 85 C0              test        eax,eax
  0000000140646EA9: 0F 8E 89 00 00 00  jle         0000000140646F38
  0000000140646EAF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140646EB4: 48 83 C1 40        add         rcx,40h
  0000000140646EB8: E8 53 05 CC FF     call        0000000140307410
  0000000140646EBD: 0F B6 C0           movzx       eax,al
  0000000140646EC0: 85 C0              test        eax,eax
  0000000140646EC2: 75 74              jne         0000000140646F38
  0000000140646EC4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140646EC9: 48 83 C1 40        add         rcx,40h
  0000000140646ECD: E8 4E A8 60 00     call        0000000140C51720
  0000000140646ED2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140646ED7: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140646EDC: E8 BF 1A 01 00     call        00000001406589A0
  0000000140646EE1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140646EE7: 74 4A              je          0000000140646F33
  0000000140646EE9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140646EEE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140646EF3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140646EF8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140646EFD: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140646F03: 74 1C              je          0000000140646F21
  0000000140646F05: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140646F0A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140646F0D: BA 01 00 00 00     mov         edx,1
  0000000140646F12: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140646F17: FF 50 18           call        qword ptr [rax+18h]
  0000000140646F1A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140646F1F: EB 09              jmp         0000000140646F2A
  0000000140646F21: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140646F2A: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140646F33: E9 77 FF FF FF     jmp         0000000140646EAF
  0000000140646F38: 48 83 C4 48        add         rsp,48h
  0000000140646F3C: C3                 ret

  Summary

     1E02000 .text
