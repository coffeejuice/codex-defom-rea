Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400597D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400597D5: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  00000001400597DC: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400597E8: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400597F0: E8 AB 5F 00 00     call        000000014005F7A0
  00000001400597F5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400597FA: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140059800: 75 07              jne         0000000140059809
  0000000140059802: 32 C0              xor         al,al
  0000000140059804: E9 A3 01 00 00     jmp         00000001400599AC
  0000000140059809: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000014005980E: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140059816: E8 75 FA FF FF     call        0000000140059290
  000000014005981B: 90                 nop
  000000014005981C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140059824: C7 44 24 20 0F 00  mov         dword ptr [rsp+20h],0Fh
                    00 00
  000000014005982C: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140059831: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140059836: FF 15 14 A7 DA 01  call        qword ptr [0000000141E03F50h]
  000000014005983C: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140059844: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014005984C: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140059854: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014005985C: FF 15 F6 A6 DA 01  call        qword ptr [0000000141E03F58h]
  0000000140059862: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  0000000140059867: 48 8B D0           mov         rdx,rax
  000000014005986A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005986F: E8 AC C0 83 00     call        0000000140895920
  0000000140059874: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140059878: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014005987D: FF 15 0D AA DA 01  call        qword ptr [0000000141E04290h]
  0000000140059883: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014005988B: E8 70 0B 00 00     call        000000014005A400
  0000000140059890: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140059895: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014005989D: E8 8E 68 91 00     call        0000000140970130
  00000001400598A2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400598A7: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400598AF: E8 4C 2E 17 00     call        00000001401CC700
  00000001400598B4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400598B9: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400598BE: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400598C6: E8 E5 FA FF FF     call        00000001400593B0
  00000001400598CB: 90                 nop
  00000001400598CC: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400598D4: E8 E7 72 FF FF     call        0000000140050BC0
  00000001400598D9: 0F B6 C0           movzx       eax,al
  00000001400598DC: 85 C0              test        eax,eax
  00000001400598DE: 74 40              je          0000000140059920
  00000001400598E0: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000001400598E5: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400598EA: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400598EF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400598F4: FF 15 DE C0 DA 01  call        qword ptr [0000000141E059D8h]
  00000001400598FA: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140059902: 4C 8B 4C 24 38     mov         r9,qword ptr [rsp+38h]
  0000000140059907: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  000000014005990C: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140059914: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140059919: E8 C2 16 CF 00     call        0000000140D4AFE0
  000000014005991E: EB 40              jmp         0000000140059960
  0000000140059920: 48 8D 44 24 78     lea         rax,[rsp+78h]
  0000000140059925: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014005992D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140059932: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005993A: FF 15 98 C0 DA 01  call        qword ptr [0000000141E059D8h]
  0000000140059940: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140059948: 45 33 C9           xor         r9d,r9d
  000000014005994B: 45 33 C0           xor         r8d,r8d
  000000014005994E: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140059956: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014005995B: E8 80 16 CF 00     call        0000000140D4AFE0
  0000000140059960: 83 7C 24 50 00     cmp         dword ptr [rsp+50h],0
  0000000140059965: 75 0D              jne         0000000140059974
  0000000140059967: C7 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],1
                    00 01 00 00 00
  0000000140059972: EB 0B              jmp         000000014005997F
  0000000140059974: C7 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00
  000000014005997F: 0F B6 84 24 B8 00  movzx       eax,byte ptr [rsp+00000000000000B8h]
                    00 00
  0000000140059987: 88 84 24 88 00 00  mov         byte ptr [rsp+0000000000000088h],al
                    00
  000000014005998E: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140059993: FF 15 47 C0 DA 01  call        qword ptr [0000000141E059E0h]
  0000000140059999: 90                 nop
  000000014005999A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014005999F: FF 15 3B C0 DA 01  call        qword ptr [0000000141E059E0h]
  00000001400599A5: 8A 84 24 88 00 00  mov         al,byte ptr [rsp+0000000000000088h]
                    00
  00000001400599AC: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  00000001400599B3: C3                 ret

  Summary

     1E02000 .text
