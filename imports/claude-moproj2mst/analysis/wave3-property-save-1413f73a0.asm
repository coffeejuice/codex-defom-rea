Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001413F73A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001413F73A5: 48 83 EC 78        sub         rsp,78h
  00000001413F73A9: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001413F73B2: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001413F73B7: FF 15 03 D1 A0 00  call        qword ptr [0000000141E044C0h]
  00000001413F73BD: 90                 nop
  00000001413F73BE: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001413F73C6: 48 83 B8 D0 00 00  cmp         qword ptr [rax+00000000000000D0h],0
                    00 00
  00000001413F73CE: 74 19              je          00000001413F73E9
  00000001413F73D0: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001413F73D8: 48 8B 89 D0 00 00  mov         rcx,qword ptr [rcx+00000000000000D0h]
                    00
  00000001413F73DF: E8 AC 19 20 FF     call        00000001405F8D90
  00000001413F73E4: 48 85 C0           test        rax,rax
  00000001413F73E7: 75 1C              jne         00000001413F7405
  00000001413F73E9: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001413F73F1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001413F73F6: FF 15 E4 E5 A0 00  call        qword ptr [0000000141E059E0h]
  00000001413F73FC: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  00000001413F7400: E9 90 00 00 00     jmp         00000001413F7495
  00000001413F7405: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001413F740D: 48 8B 89 D0 00 00  mov         rcx,qword ptr [rcx+00000000000000D0h]
                    00
  00000001413F7414: E8 77 19 20 FF     call        00000001405F8D90
  00000001413F7419: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001413F741E: 48 8B C8           mov         rcx,rax
  00000001413F7421: E8 CA 99 C5 FE     call        0000000140050DF0
  00000001413F7426: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001413F742B: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001413F7430: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001413F7435: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  00000001413F743A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001413F743F: FF 15 2B D0 A0 00  call        qword ptr [0000000141E04470h]
  00000001413F7445: 90                 nop
  00000001413F7446: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001413F744B: FF 15 8F E5 A0 00  call        qword ptr [0000000141E059E0h]
  00000001413F7451: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000001413F7456: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001413F745B: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001413F7460: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001413F7465: FF 15 6D E5 A0 00  call        qword ptr [0000000141E059D8h]
  00000001413F746B: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001413F7470: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  00000001413F7475: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001413F747D: E8 1E 00 00 00     call        00000001413F74A0
  00000001413F7482: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001413F7486: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001413F748B: FF 15 4F E5 A0 00  call        qword ptr [0000000141E059E0h]
  00000001413F7491: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001413F7495: 48 83 C4 78        add         rsp,78h
  00000001413F7499: C3                 ret

  Summary

     1E02000 .text
