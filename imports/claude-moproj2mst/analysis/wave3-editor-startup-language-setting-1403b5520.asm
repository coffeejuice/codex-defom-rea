Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403B5520: 48 83 EC 38        sub         rsp,38h
  00000001403B5524: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  00000001403B552C: 8B 05 E6 41 9C 03  mov         eax,dword ptr [0000000143D79718h]
  00000001403B5532: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001403B5536: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  00000001403B553B: 7D 12              jge         00000001403B554F
  00000001403B553D: E8 AE FF FF FF     call        00000001403B54F0
  00000001403B5542: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001403B5546: 8B 4C 24 20        mov         ecx,dword ptr [rsp+20h]
  00000001403B554A: E8 B1 FF FF FF     call        00000001403B5500
  00000001403B554F: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001403B5553: 48 83 C4 38        add         rsp,38h
  00000001403B5557: C3                 ret

  Summary

     1E02000 .text
