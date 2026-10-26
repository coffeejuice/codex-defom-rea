Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054540: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054545: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  000000014005454C: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140054555: 48 8D 0D 5C 21 D2  lea         rcx,[0000000143D766B8h]
                    03
  000000014005455C: E8 3F 72 27 00     call        00000001402CB7A0
  0000000140054561: 83 F8 05           cmp         eax,5
  0000000140054564: 75 0D              jne         0000000140054573
  0000000140054566: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  0000000140054571: EB 0B              jmp         000000014005457E
  0000000140054573: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  000000014005457E: 0F B6 84 24 80 00  movzx       eax,byte ptr [rsp+0000000000000080h]
                    00 00
  0000000140054586: 88 44 24 50        mov         byte ptr [rsp+50h],al
  000000014005458A: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  000000014005458F: 85 C0              test        eax,eax
  0000000140054591: 75 05              jne         0000000140054598
  0000000140054593: E9 01 02 00 00     jmp         0000000140054799
  0000000140054598: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400545A0: E8 AB 0A 00 00     call        0000000140055050
  00000001400545A5: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400545AA: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  00000001400545B0: 75 05              jne         00000001400545B7
  00000001400545B2: E9 E2 01 00 00     jmp         0000000140054799
  00000001400545B7: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400545BC: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400545C4: E8 F7 C4 FF FF     call        0000000140050AC0
  00000001400545C9: 90                 nop
  00000001400545CA: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400545D2: E8 49 C5 FF FF     call        0000000140050B20
  00000001400545D7: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001400545DB: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400545E0: FF 15 DA FE DA 01  call        qword ptr [0000000141E044C0h]
  00000001400545E6: 90                 nop
  00000001400545E7: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  00000001400545EF: 44 8B 44 24 28     mov         r8d,dword ptr [rsp+28h]
  00000001400545F4: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001400545F9: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400545FE: E8 3D 3F 06 00     call        00000001400B8540
  0000000140054603: 90                 nop
  0000000140054604: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140054609: E8 D2 04 FB FF     call        0000000140004AE0
  000000014005460E: 85 C0              test        eax,eax
  0000000140054610: 0F 8E 61 01 00 00  jle         0000000140054777
  0000000140054616: 33 D2              xor         edx,edx
  0000000140054618: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014005461D: E8 CE D3 FD FF     call        00000001400319F0
  0000000140054622: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140054627: 48 8B C8           mov         rcx,rax
  000000014005462A: E8 C1 0E 01 00     call        00000001400654F0
  000000014005462F: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140054637: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014005463F: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140054647: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014005464F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140054654: FF 15 16 FE DA 01  call        qword ptr [0000000141E04470h]
  000000014005465A: 90                 nop
  000000014005465B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140054660: FF 15 7A 13 DB 01  call        qword ptr [0000000141E059E0h]
  0000000140054666: 33 D2              xor         edx,edx
  0000000140054668: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014005466D: E8 7E D3 FD FF     call        00000001400319F0
  0000000140054672: 48 8B C8           mov         rcx,rax
  0000000140054675: E8 06 80 C6 00     call        0000000140CBC680
  000000014005467A: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014005467E: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140054686: E8 E5 B0 00 00     call        000000014005F770
  000000014005468B: 48 8B C8           mov         rcx,rax
  000000014005468E: E8 3D 6C C2 00     call        0000000140C7B2D0
  0000000140054693: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140054698: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  000000014005469E: 0F 84 D3 00 00 00  je          0000000140054777
  00000001400546A4: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000001400546A9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400546AE: FF 15 9C F8 DA 01  call        qword ptr [0000000141E03F50h]
  00000001400546B4: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400546BC: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400546C4: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400546CC: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400546D4: FF 15 4E FB DA 01  call        qword ptr [0000000141E04228h]
  00000001400546DA: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400546DF: 48 83 C1 51        add         rcx,51h
  00000001400546E3: 41 B8 40 00 00 00  mov         r8d,40h
  00000001400546E9: 48 8B D0           mov         rdx,rax
  00000001400546EC: E8 2F B9 41 01     call        0000000141470020
  00000001400546F1: 90                 nop
  00000001400546F2: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400546F7: FF 15 93 FB DA 01  call        qword ptr [0000000141E04290h]
  00000001400546FD: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140054702: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140054706: 89 81 E8 00 00 00  mov         dword ptr [rcx+00000000000000E8h],eax
  000000014005470C: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140054711: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140054716: FF 15 34 F8 DA 01  call        qword ptr [0000000141E03F50h]
  000000014005471C: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140054724: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014005472C: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140054734: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014005473C: FF 15 E6 FA DA 01  call        qword ptr [0000000141E04228h]
  0000000140054742: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140054747: 48 81 C1 92 00 00  add         rcx,92h
                    00
  000000014005474E: 41 B8 50 00 00 00  mov         r8d,50h
  0000000140054754: 48 8B D0           mov         rdx,rax
  0000000140054757: E8 C4 B8 41 01     call        0000000141470020
  000000014005475C: 90                 nop
  000000014005475D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140054762: FF 15 28 FB DA 01  call        qword ptr [0000000141E04290h]
  0000000140054768: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014005476D: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140054771: 89 81 E4 00 00 00  mov         dword ptr [rcx+00000000000000E4h],eax
  0000000140054777: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014005477C: E8 8F D1 FD FF     call        0000000140031910
  0000000140054781: 90                 nop
  0000000140054782: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140054787: FF 15 53 12 DB 01  call        qword ptr [0000000141E059E0h]
  000000014005478D: 90                 nop
  000000014005478E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140054793: FF 15 47 12 DB 01  call        qword ptr [0000000141E059E0h]
  0000000140054799: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  00000001400547A0: C3                 ret

  Summary

     1E02000 .text
