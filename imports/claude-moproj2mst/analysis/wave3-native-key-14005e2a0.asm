Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005E2A0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014005E2A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005E2A9: 53                 push        rbx
  000000014005E2AA: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  000000014005E2B1: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014005E2BA: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014005E2C2: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014005E2CA: E8 A1 14 00 00     call        000000014005F770
  000000014005E2CF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014005E2D4: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014005E2DA: 74 0F              je          000000014005E2EB
  000000014005E2DC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014005E2E1: E8 EA C9 C1 00     call        0000000140C7ACD0
  000000014005E2E6: 48 85 C0           test        rax,rax
  000000014005E2E9: 75 07              jne         000000014005E2F2
  000000014005E2EB: 32 C0              xor         al,al
  000000014005E2ED: E9 78 01 00 00     jmp         000000014005E46A
  000000014005E2F2: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  000000014005E2F9: 83 C0 01           add         eax,1
  000000014005E2FC: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014005E300: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014005E304: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014005E309: E8 72 CB C1 00     call        0000000140C7AE80
  000000014005E30E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014005E313: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014005E319: 0F 84 45 01 00 00  je          000000014005E464
  000000014005E31F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005E324: E8 57 69 7D 00     call        0000000140834C80
  000000014005E329: 0F B6 C0           movzx       eax,al
  000000014005E32C: 85 C0              test        eax,eax
  000000014005E32E: 0F 84 30 01 00 00  je          000000014005E464
  000000014005E334: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000014005E33C: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014005E341: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014005E349: E8 92 E9 FF FF     call        000000014005CCE0
  000000014005E34E: 90                 nop
  000000014005E34F: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000014005E354: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014005E359: FF 15 F1 5B DA 01  call        qword ptr [0000000141E03F50h]
  000000014005E35F: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014005E364: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014005E369: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014005E36E: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014005E373: FF 15 DF 5B DA 01  call        qword ptr [0000000141E03F58h]
  000000014005E379: 48 8B D8           mov         rbx,rax
  000000014005E37C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014005E381: E8 4A C9 C1 00     call        0000000140C7ACD0
  000000014005E386: 44 8B 44 24 38     mov         r8d,dword ptr [rsp+38h]
  000000014005E38B: 48 8B D3           mov         rdx,rbx
  000000014005E38E: 48 8B C8           mov         rcx,rax
  000000014005E391: E8 AA 82 83 00     call        0000000140896640
  000000014005E396: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014005E39A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014005E39F: FF 15 EB 5E DA 01  call        qword ptr [0000000141E04290h]
  000000014005E3A5: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000014005E3AD: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014005E3B2: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014005E3BA: E8 D1 F6 FF FF     call        000000014005DA90
  000000014005E3BF: 90                 nop
  000000014005E3C0: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000014005E3C5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014005E3CA: FF 15 80 5B DA 01  call        qword ptr [0000000141E03F50h]
  000000014005E3D0: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014005E3D8: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005E3E0: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014005E3E8: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014005E3F0: FF 15 62 5B DA 01  call        qword ptr [0000000141E03F58h]
  000000014005E3F6: 48 8B D8           mov         rbx,rax
  000000014005E3F9: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014005E3FE: E8 CD C8 C1 00     call        0000000140C7ACD0
  000000014005E403: 44 8B 44 24 38     mov         r8d,dword ptr [rsp+38h]
  000000014005E408: 48 8B D3           mov         rdx,rbx
  000000014005E40B: 48 8B C8           mov         rcx,rax
  000000014005E40E: E8 3D 83 83 00     call        0000000140896750
  000000014005E413: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014005E417: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014005E41C: FF 15 6E 5E DA 01  call        qword ptr [0000000141E04290h]
  000000014005E422: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  000000014005E427: 74 22              je          000000014005E44B
  000000014005E429: C6 44 24 60 00     mov         byte ptr [rsp+60h],0
  000000014005E42E: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014005E433: FF 15 A7 75 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005E439: 90                 nop
  000000014005E43A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014005E43F: FF 15 9B 75 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005E445: 8A 44 24 60        mov         al,byte ptr [rsp+60h]
  000000014005E449: EB 1F              jmp         000000014005E46A
  000000014005E44B: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014005E450: FF 15 8A 75 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005E456: 90                 nop
  000000014005E457: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014005E45C: FF 15 7E 75 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005E462: EB 04              jmp         000000014005E468
  000000014005E464: 32 C0              xor         al,al
  000000014005E466: EB 02              jmp         000000014005E46A
  000000014005E468: B0 01              mov         al,1
  000000014005E46A: 48 81 C4 90 00 00  add         rsp,90h
                    00
  000000014005E471: 5B                 pop         rbx
  000000014005E472: C3                 ret

  Summary

     1E02000 .text
