Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140816AE0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140816AE4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140816AE9: 48 83 EC 38        sub         rsp,38h
  0000000140816AED: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140816AF5: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  0000000140816AFA: 7E 28              jle         0000000140816B24
  0000000140816AFC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140816B01: E8 FA 13 8C FF     call        00000001400D7F00
  0000000140816B06: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  0000000140816B0A: 7D 18              jge         0000000140816B24
  0000000140816B0C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140816B11: 48 83 C1 38        add         rcx,38h
  0000000140816B15: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000140816B19: E8 A2 E5 80 FF     call        00000001400250C0
  0000000140816B1E: 8B 00              mov         eax,dword ptr [rax]
  0000000140816B20: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140816B24: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140816B28: 48 83 C4 38        add         rsp,38h
  0000000140816B2C: C3                 ret

  Summary

     1E02000 .text
