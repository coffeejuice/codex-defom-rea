Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D3FC70: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D3FC75: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D3FC7A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3FC7F: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  0000000140D3FC86: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D3FC92: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140D3FC9B: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D3FCA4: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140D3FCAD: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140D3FCB6: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D3FCBB: FF 15 FF 47 0C 01  call        qword ptr [0000000141E044C0h]
  0000000140D3FCC1: 90                 nop
  0000000140D3FCC2: B9 30 00 00 00     mov         ecx,30h
  0000000140D3FCC7: E8 F0 ED 72 00     call        000000014146EABC
  0000000140D3FCCC: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D3FCD1: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140D3FCD7: 74 14              je          0000000140D3FCED
  0000000140D3FCD9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D3FCDE: E8 ED C1 00 00     call        0000000140D4BED0
  0000000140D3FCE3: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140D3FCEB: EB 0C              jmp         0000000140D3FCF9
  0000000140D3FCED: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140D3FCF9: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D3FD01: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D3FD06: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D3FD0B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D3FD10: B2 01              mov         dl,1
  0000000140D3FD12: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D3FD17: E8 C4 B8 3A FF     call        00000001400EB5E0
  0000000140D3FD1C: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140D3FD21: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140D3FD29: FF 15 89 B8 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D3FD2F: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140D3FD34: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D3FD3C: E8 CF 15 F5 FF     call        0000000140C91310
  0000000140D3FD41: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D3FD49: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D3FD51: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D3FD59: 48 8B 05 30 44 0C  mov         rax,qword ptr [0000000141E04190h]
                    01
  0000000140D3FD60: 0F B6 10           movzx       edx,byte ptr [rax]
  0000000140D3FD63: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D3FD6B: E8 E0 1D 30 FF     call        0000000140041B50
  0000000140D3FD70: 88 44 24 58        mov         byte ptr [rsp+58h],al
  0000000140D3FD74: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140D3FD79: FF 15 61 5C 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D3FD7F: 44 0F B6 5C 24 58  movzx       r11d,byte ptr [rsp+58h]
  0000000140D3FD85: 45 85 DB           test        r11d,r11d
  0000000140D3FD88: 74 14              je          0000000140D3FD9E
  0000000140D3FD8A: 48 8D 15 8F ED CC  lea         rdx,[0000000142A0EB20h]
                    01
  0000000140D3FD91: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D3FD96: FF 15 BC 40 0C 01  call        qword ptr [0000000141E03E58h]
  0000000140D3FD9C: EB 49              jmp         0000000140D3FDE7
  0000000140D3FD9E: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140D3FDA3: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D3FDAB: E8 60 15 F5 FF     call        0000000140C91310
  0000000140D3FDB0: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140D3FDB8: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140D3FDC0: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140D3FDC8: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140D3FDD0: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D3FDD5: FF 15 95 46 0C 01  call        qword ptr [0000000141E04470h]
  0000000140D3FDDB: 90                 nop
  0000000140D3FDDC: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140D3FDE1: FF 15 F9 5B 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D3FDE7: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140D3FDEC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D3FDF1: E8 0A ED 00 00     call        0000000140D4EB00
  0000000140D3FDF6: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D3FDFB: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D3FE03: E8 48 16 F5 FF     call        0000000140C91450
  0000000140D3FE08: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140D3FE10: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140D3FE18: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D3FE20: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D3FE28: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D3FE2D: FF 15 AD 9F 0C 01  call        qword ptr [0000000141E09DE0h]
  0000000140D3FE33: 90                 nop
  0000000140D3FE34: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D3FE39: FF 15 A1 5B 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D3FE3F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D3FE44: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D3FE49: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D3FE4E: FF 15 8C 5B 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D3FE54: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140D3FE59: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  0000000140D3FE60: C3                 ret

  Summary

     1E02000 .text
