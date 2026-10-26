Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F1060: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412F1065: 53                 push        rbx
  00000001412F1066: 48 83 EC 30        sub         rsp,30h
  00000001412F106A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412F1073: 48 8B D9           mov         rbx,rcx
  00000001412F1076: B9 08 00 00 00     mov         ecx,8
  00000001412F107B: E8 3C DA 17 00     call        000000014146EABC
  00000001412F1080: 48 85 C0           test        rax,rax
  00000001412F1083: 74 05              je          00000001412F108A
  00000001412F1085: 48 89 18           mov         qword ptr [rax],rbx
  00000001412F1088: EB 02              jmp         00000001412F108C
  00000001412F108A: 33 C0              xor         eax,eax
  00000001412F108C: 48 89 03           mov         qword ptr [rbx],rax
  00000001412F108F: 48 8B CB           mov         rcx,rbx
  00000001412F1092: E8 29 CA FF FF     call        00000001412EDAC0
  00000001412F1097: 48 89 43 30        mov         qword ptr [rbx+30h],rax
  00000001412F109B: C6 40 29 01        mov         byte ptr [rax+29h],1
  00000001412F109F: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412F10A3: 48 89 40 08        mov         qword ptr [rax+8],rax
  00000001412F10A7: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412F10AB: 48 89 00           mov         qword ptr [rax],rax
  00000001412F10AE: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412F10B2: 48 89 40 10        mov         qword ptr [rax+10h],rax
  00000001412F10B6: 48 C7 43 38 00 00  mov         qword ptr [rbx+38h],0
                    00 00
  00000001412F10BE: 48 8B C3           mov         rax,rbx
  00000001412F10C1: 48 83 C4 30        add         rsp,30h
  00000001412F10C5: 5B                 pop         rbx
  00000001412F10C6: C3                 ret

  Summary

     1E02000 .text
