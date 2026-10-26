Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401313C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401313C5: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  00000001401313CC: 48 C7 44 24 70 FE  mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001401313D5: 48 83 BC 24 90 00  cmp         qword ptr [rsp+0000000000000090h],0
                    00 00 00
  00000001401313DE: 75 07              jne         00000001401313E7
  00000001401313E0: 32 C0              xor         al,al
  00000001401313E2: E9 3B 01 00 00     jmp         0000000140131522
  00000001401313E7: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401313EF: E8 6C 5F F7 FF     call        00000001400A7360
  00000001401313F4: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001401313F8: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001401313FD: 7F 07              jg          0000000140131406
  00000001401313FF: 32 C0              xor         al,al
  0000000140131401: E9 1C 01 00 00     jmp         0000000140131522
  0000000140131406: 41 B0 01           mov         r8b,1
  0000000140131409: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014013140E: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140131416: E8 85 3C F7 FF     call        00000001400A50A0
  000000014013141B: 90                 nop
  000000014013141C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140131421: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140131426: FF 15 AC 2B CD 01  call        qword ptr [0000000141E03FD8h]
  000000014013142C: 90                 nop
  000000014013142D: 48 8D 44 24 58     lea         rax,[rsp+58h]
  0000000140131432: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140131437: BA 02 00 00 00     mov         edx,2
  000000014013143C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140131441: E8 1A 36 ED FF     call        0000000140004A60
  0000000140131446: 48 8B D0           mov         rdx,rax
  0000000140131449: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014013144E: FF 15 8C 2B CD 01  call        qword ptr [0000000141E03FE0h]
  0000000140131454: 0F B6 C0           movzx       eax,al
  0000000140131457: 85 C0              test        eax,eax
  0000000140131459: 75 25              jne         0000000140131480
  000000014013145B: C6 44 24 68 00     mov         byte ptr [rsp+68h],0
  0000000140131460: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140131465: FF 15 B5 2B CD 01  call        qword ptr [0000000141E04020h]
  000000014013146B: 90                 nop
  000000014013146C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140131471: FF 15 69 45 CD 01  call        qword ptr [0000000141E059E0h]
  0000000140131477: 8A 44 24 68        mov         al,byte ptr [rsp+68h]
  000000014013147B: E9 A2 00 00 00     jmp         0000000140131522
  0000000140131480: E8 0B FF FF FF     call        0000000140131390
  0000000140131485: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014013148A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014013148F: FF 15 43 29 CD 01  call        qword ptr [0000000141E03DD8h]
  0000000140131495: 90                 nop
  0000000140131496: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014013149E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001401314A3: E8 D8 01 00 00     call        0000000140131680
  00000001401314A8: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401314B0: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001401314B5: E8 F6 03 00 00     call        00000001401318B0
  00000001401314BA: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001401314C2: 44 8B 44 24 20     mov         r8d,dword ptr [rsp+20h]
  00000001401314C7: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401314CF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001401314D4: E8 17 09 00 00     call        0000000140131DF0
  00000001401314D9: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401314E1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001401314E6: E8 25 08 00 00     call        0000000140131D10
  00000001401314EB: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001401314F0: FF 15 22 2B CD 01  call        qword ptr [0000000141E04018h]
  00000001401314F6: C6 44 24 69 01     mov         byte ptr [rsp+69h],1
  00000001401314FB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140131500: FF 15 F2 28 CD 01  call        qword ptr [0000000141E03DF8h]
  0000000140131506: 90                 nop
  0000000140131507: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014013150C: FF 15 0E 2B CD 01  call        qword ptr [0000000141E04020h]
  0000000140131512: 90                 nop
  0000000140131513: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140131518: FF 15 C2 44 CD 01  call        qword ptr [0000000141E059E0h]
  000000014013151E: 8A 44 24 69        mov         al,byte ptr [rsp+69h]
  0000000140131522: 48 81 C4 88 00 00  add         rsp,88h
                    00
  0000000140131529: C3                 ret

  Summary

     1E02000 .text
