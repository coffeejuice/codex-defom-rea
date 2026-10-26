Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140F9B270: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140F9B275: 53                 push        rbx
  0000000140F9B276: 48 83 EC 20        sub         rsp,20h
  0000000140F9B27A: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140F9B27F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9B282: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140F9B287: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9B28D: 48 85 C0           test        rax,rax
  0000000140F9B290: 74 60              je          0000000140F9B2F2
  0000000140F9B292: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140F9B297: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140F9B29B: 48 8B 89 80 00 00  mov         rcx,qword ptr [rcx+0000000000000080h]
                    00
  0000000140F9B2A2: FF 15 C0 AA E6 00  call        qword ptr [0000000141E05D68h]
  0000000140F9B2A8: 44 0F B6 D8        movzx       r11d,al
  0000000140F9B2AC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140F9B2B1: 44 88 58 5C        mov         byte ptr [rax+5Ch],r11b
  0000000140F9B2B5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140F9B2BA: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9B2BD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140F9B2C2: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9B2C8: 48 8B C8           mov         rcx,rax
  0000000140F9B2CB: E8 10 4C CE FF     call        0000000140C7FEE0
  0000000140F9B2D0: 8B D8              mov         ebx,eax
  0000000140F9B2D2: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140F9B2D7: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140F9B2DA: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140F9B2DF: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140F9B2E5: 44 8B C3           mov         r8d,ebx
  0000000140F9B2E8: B2 01              mov         dl,1
  0000000140F9B2EA: 48 8B C8           mov         rcx,rax
  0000000140F9B2ED: E8 FE C2 3C FF     call        00000001403675F0
  0000000140F9B2F2: 48 83 C4 20        add         rsp,20h
  0000000140F9B2F6: 5B                 pop         rbx
  0000000140F9B2F7: C3                 ret

  Summary

     1E02000 .text
