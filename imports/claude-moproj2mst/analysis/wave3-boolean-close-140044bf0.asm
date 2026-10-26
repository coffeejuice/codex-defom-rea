Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140044BF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140044BF5: 48 83 EC 48        sub         rsp,48h
  0000000140044BF9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044BFE: 48 83 B8 C0 19 00  cmp         qword ptr [rax+00000000000019C0h],0
                    00 00
  0000000140044C06: 74 63              je          0000000140044C6B
  0000000140044C08: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140044C0D: 48 8B 89 C0 19 00  mov         rcx,qword ptr [rcx+00000000000019C0h]
                    00
  0000000140044C14: E8 A7 36 05 00     call        00000001400982C0
  0000000140044C19: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044C1E: 48 8B 80 C0 19 00  mov         rax,qword ptr [rax+00000000000019C0h]
                    00
  0000000140044C25: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140044C2A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140044C2F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140044C34: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140044C3A: 74 16              je          0000000140044C52
  0000000140044C3C: BA 01 00 00 00     mov         edx,1
  0000000140044C41: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140044C46: E8 95 07 00 00     call        00000001400453E0
  0000000140044C4B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140044C50: EB 09              jmp         0000000140044C5B
  0000000140044C52: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140044C5B: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044C60: 48 C7 80 C0 19 00  mov         qword ptr [rax+00000000000019C0h],0
                    00 00 00 00 00
  0000000140044C6B: 48 83 C4 48        add         rsp,48h
  0000000140044C6F: C3                 ret

  Summary

     1E02000 .text
