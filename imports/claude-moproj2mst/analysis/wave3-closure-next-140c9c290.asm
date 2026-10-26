Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C9C290: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C9C295: 48 83 EC 78        sub         rsp,78h
  0000000140C9C299: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C9C2A2: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C9C2AA: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140C9C2B1: E8 3A EC F7 FF     call        0000000140C1AEF0
  0000000140C9C2B6: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C9C2BB: 48 8B C8           mov         rcx,rax
  0000000140C9C2BE: FF 15 E4 B9 16 01  call        qword ptr [0000000141E07CA8h]
  0000000140C9C2C4: 90                 nop
  0000000140C9C2C5: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C9C2CA: E8 11 88 36 FF     call        0000000140004AE0
  0000000140C9C2CF: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C9C2D3: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C9C2DB: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140C9C2E2: E8 09 EC F7 FF     call        0000000140C1AEF0
  0000000140C9C2E7: 48 8B C8           mov         rcx,rax
  0000000140C9C2EA: 48 83 C1 10        add         rcx,10h
  0000000140C9C2EE: E8 FD D1 05 00     call        0000000140CF94F0
  0000000140C9C2F3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C9C2F8: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C9C301: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C9C309: 48 83 C1 48        add         rcx,48h
  0000000140C9C30D: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140C9C312: E8 49 AF 3C FF     call        0000000140067260
  0000000140C9C317: 0F B6 C0           movzx       eax,al
  0000000140C9C31A: 85 C0              test        eax,eax
  0000000140C9C31C: 74 26              je          0000000140C9C344
  0000000140C9C31E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C9C326: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140C9C32D: E8 BE EB F7 FF     call        0000000140C1AEF0
  0000000140C9C332: 48 8B C8           mov         rcx,rax
  0000000140C9C335: E8 86 EF 02 00     call        0000000140CCB2C0
  0000000140C9C33A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C9C33F: E9 A9 00 00 00     jmp         0000000140C9C3ED
  0000000140C9C344: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  0000000140C9C34C: EB 0B              jmp         0000000140C9C359
  0000000140C9C34E: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140C9C352: 83 C0 01           add         eax,1
  0000000140C9C355: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140C9C359: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140C9C35D: 39 44 24 3C        cmp         dword ptr [rsp+3Ch],eax
  0000000140C9C361: 0F 8D 86 00 00 00  jge         0000000140C9C3ED
  0000000140C9C367: 8B 54 24 3C        mov         edx,dword ptr [rsp+3Ch]
  0000000140C9C36B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C9C370: E8 3B F2 4A FF     call        000000014014B5B0
  0000000140C9C375: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9C378: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C9C37D: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140C9C382: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9C385: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140C9C38A: FF 50 50           call        qword ptr [rax+50h]
  0000000140C9C38D: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140C9C391: 81 7C 24 40 0D 00  cmp         dword ptr [rsp+40h],1000Dh
                    01 00
  0000000140C9C399: 75 4D              jne         0000000140C9C3E8
  0000000140C9C39B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140C9C3A0: E8 FB 31 02 00     call        0000000140CBF5A0
  0000000140C9C3A5: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C9C3AA: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140C9C3B0: 74 36              je          0000000140C9C3E8
  0000000140C9C3B2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C9C3B7: E8 D4 CE B3 00     call        00000001417D9290
  0000000140C9C3BC: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C9C3C4: 48 83 C1 48        add         rcx,48h
  0000000140C9C3C8: 48 8B D0           mov         rdx,rax
  0000000140C9C3CB: E8 90 AE 3C FF     call        0000000140067260
  0000000140C9C3D0: 0F B6 C0           movzx       eax,al
  0000000140C9C3D3: 85 C0              test        eax,eax
  0000000140C9C3D5: 74 11              je          0000000140C9C3E8
  0000000140C9C3D7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C9C3DC: E8 BF CF B3 00     call        00000001417D93A0
  0000000140C9C3E1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C9C3E6: EB 05              jmp         0000000140C9C3ED
  0000000140C9C3E8: E9 61 FF FF FF     jmp         0000000140C9C34E
  0000000140C9C3ED: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C9C3F2: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C9C3F7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C9C3FC: E8 9F BD 56 FF     call        00000001402081A0
  0000000140C9C401: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140C9C406: 48 83 C4 78        add         rsp,78h
  0000000140C9C40A: C3                 ret

  Summary

     1E02000 .text
