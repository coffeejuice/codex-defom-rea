Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001404122F0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001404122F5: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001404122F9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001404122FE: 48 83 EC 68        sub         rsp,68h
  0000000140412302: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  000000014041230A: 48 8D 0D A7 43 96  lea         rcx,[0000000143D766B8h]
                    03
  0000000140412311: E8 7A 5D EB FF     call        00000001402C8090
  0000000140412316: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014041231B: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140412321: 75 05              jne         0000000140412328
  0000000140412323: E9 FC 00 00 00     jmp         0000000140412424
  0000000140412328: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014041232D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140412330: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140412335: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  000000014041233B: 48 8B C8           mov         rcx,rax
  000000014041233E: E8 9D B5 8C 00     call        0000000140CDD8E0
  0000000140412343: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140412348: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014041234E: 75 05              jne         0000000140412355
  0000000140412350: E9 CF 00 00 00     jmp         0000000140412424
  0000000140412355: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014041235A: E8 D1 1C 00 00     call        0000000140414030
  000000014041235F: 48 85 C0           test        rax,rax
  0000000140412362: 74 16              je          000000014041237A
  0000000140412364: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140412369: E8 C2 1C 00 00     call        0000000140414030
  000000014041236E: 48 8B C8           mov         rcx,rax
  0000000140412371: E8 EA 43 C5 FF     call        0000000140066760
  0000000140412376: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014041237A: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014041237F: 85 C0              test        eax,eax
  0000000140412381: 0F 84 93 00 00 00  je          000000014041241A
  0000000140412387: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  000000014041238C: 0F 84 88 00 00 00  je          000000014041241A
  0000000140412392: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140412397: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014041239C: 48 8D 15 A5 BC F5  lea         rdx,[000000014236E048h]
                    01
  00000001404123A3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001404123A8: FF 15 D2 20 9F 01  call        qword ptr [0000000141E04480h]
  00000001404123AE: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001404123B3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001404123B8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001404123BB: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001404123C0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001404123C5: FF 50 78           call        qword ptr [rax+78h]
  00000001404123C8: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001404123CD: 41 B1 01           mov         r9b,1
  00000001404123D0: 41 B0 01           mov         r8b,1
  00000001404123D3: 8B 94 24 80 00 00  mov         edx,dword ptr [rsp+0000000000000080h]
                    00
  00000001404123DA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001404123DF: E8 7C F4 FF FF     call        0000000140411860
  00000001404123E4: 0F B6 C0           movzx       eax,al
  00000001404123E7: 85 C0              test        eax,eax
  00000001404123E9: 74 0E              je          00000001404123F9
  00000001404123EB: B2 01              mov         dl,1
  00000001404123ED: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001404123F2: E8 09 F4 FF FF     call        0000000140411800
  00000001404123F7: EB 0C              jmp         0000000140412405
  00000001404123F9: 33 D2              xor         edx,edx
  00000001404123FB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140412400: E8 FB F3 FF FF     call        0000000140411800
  0000000140412405: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014041240A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014041240D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140412412: FF 90 80 00 00 00  call        qword ptr [rax+0000000000000080h]
  0000000140412418: EB 0A              jmp         0000000140412424
  000000014041241A: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014041241F: E8 0C 00 00 00     call        0000000140412430
  0000000140412424: 48 83 C4 68        add         rsp,68h
  0000000140412428: C3                 ret

  Summary

     1E02000 .text
