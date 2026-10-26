Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D31570: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D31575: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140D3157A: 48 C7 80 20 05 00  mov         qword ptr [rax+0000000000000520h],0
                    00 00 00 00 00
  0000000140D31585: C3                 ret
  0000000140D31586: CC                 int         3
  0000000140D31587: CC                 int         3
  0000000140D31588: CC                 int         3
  0000000140D31589: CC                 int         3
  0000000140D3158A: CC                 int         3
  0000000140D3158B: CC                 int         3
  0000000140D3158C: CC                 int         3
  0000000140D3158D: CC                 int         3
  0000000140D3158E: CC                 int         3
  0000000140D3158F: CC                 int         3
  0000000140D31590: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D31595: 48 83 EC 48        sub         rsp,48h
  0000000140D31599: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D3159E: 48 83 B8 20 05 00  cmp         qword ptr [rax+0000000000000520h],0
                    00 00
  0000000140D315A6: 74 58              je          0000000140D31600
  0000000140D315A8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D315AD: 48 8B 80 20 05 00  mov         rax,qword ptr [rax+0000000000000520h]
                    00
  0000000140D315B4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D315B9: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D315BE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D315C3: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140D315C9: 74 1C              je          0000000140D315E7
  0000000140D315CB: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D315D0: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D315D3: BA 01 00 00 00     mov         edx,1
  0000000140D315D8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D315DD: FF 50 18           call        qword ptr [rax+18h]
  0000000140D315E0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D315E5: EB 09              jmp         0000000140D315F0
  0000000140D315E7: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140D315F0: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D315F5: 48 C7 80 20 05 00  mov         qword ptr [rax+0000000000000520h],0
                    00 00 00 00 00
  0000000140D31600: 48 83 C4 48        add         rsp,48h
  0000000140D31604: C3                 ret
  0000000140D31605: CC                 int         3
  0000000140D31606: CC                 int         3
  0000000140D31607: CC                 int         3
  0000000140D31608: CC                 int         3
  0000000140D31609: CC                 int         3
  0000000140D3160A: CC                 int         3
  0000000140D3160B: CC                 int         3
  0000000140D3160C: CC                 int         3
  0000000140D3160D: CC                 int         3
  0000000140D3160E: CC                 int         3
  0000000140D3160F: CC                 int         3
  0000000140D31610: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D31615: 48 83 EC 18        sub         rsp,18h
  0000000140D31619: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D3161E: 8B 05 C8 4D 04 03  mov         eax,dword ptr [0000000143D763ECh]
  0000000140D31624: 89 81 28 05 00 00  mov         dword ptr [rcx+0000000000000528h],eax
  0000000140D3162A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D3162F: 0F B6 05 BA 4D 04  movzx       eax,byte ptr [0000000143D763F0h]
                    03
  0000000140D31636: 88 81 2C 05 00 00  mov         byte ptr [rcx+000000000000052Ch],al
  0000000140D3163C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D31641: 0F B6 05 A9 4D 04  movzx       eax,byte ptr [0000000143D763F1h]
                    03
  0000000140D31648: 88 81 2D 05 00 00  mov         byte ptr [rcx+000000000000052Dh],al
  0000000140D3164E: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140D31655: EB 09              jmp         0000000140D31660
  0000000140D31657: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000140D3165A: 83 C0 01           add         eax,1
  0000000140D3165D: 89 04 24           mov         dword ptr [rsp],eax
  0000000140D31660: 83 3C 24 19        cmp         dword ptr [rsp],19h
  0000000140D31664: 0F 8D 96 00 00 00  jge         0000000140D31700
  0000000140D3166A: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000140D3166E: 48
  0000000140D3166F: 8D
  0000000140D31670: 05

  Summary

     1E02000 .text
