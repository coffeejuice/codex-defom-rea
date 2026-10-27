Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014036FCC0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014036FCC5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014036FCCA: 53                 push        rbx
  000000014036FCCB: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  000000014036FCD2: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014036FCDB: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  000000014036FCE3: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014036FCEB: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014036FCEF: 83 78 48 FF        cmp         dword ptr [rax+48h],0FFFFFFFFh
  000000014036FCF3: 0F 8E 0E 01 00 00  jle         000000014036FE07
  000000014036FCF9: B2 20              mov         dl,20h
  000000014036FCFB: 48 8D 4C 24 3A     lea         rcx,[rsp+3Ah]
  000000014036FD00: E8 5B 2B C9 FF     call        0000000140002860
  000000014036FD05: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014036FD08: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014036FD0D: FF 15 45 46 A9 01  call        qword ptr [0000000141E04358h]
  000000014036FD13: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014036FD1B: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014036FD1F: 8B 58 48           mov         ebx,dword ptr [rax+48h]
  000000014036FD22: 83 C3 01           add         ebx,1
  000000014036FD25: 48 8D 15 B8 78 FB  lea         rdx,[00000001423275E4h]
                    01
  000000014036FD2C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014036FD31: FF 15 49 47 A9 01  call        qword ptr [0000000141E04480h]
  000000014036FD37: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014036FD3C: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014036FD41: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014036FD46: 48 8D 44 24 38     lea         rax,[rsp+38h]
  000000014036FD4B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014036FD50: C7 44 24 20 0A 00  mov         dword ptr [rsp+20h],0Ah
                    00 00
  000000014036FD58: 45 33 C9           xor         r9d,r9d
  000000014036FD5B: 44 8B C3           mov         r8d,ebx
  000000014036FD5E: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014036FD63: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014036FD68: FF 15 0A 44 A9 01  call        qword ptr [0000000141E04178h]
  000000014036FD6E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014036FD73: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014036FD78: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014036FD7D: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014036FD85: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014036FD89: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014036FD8D: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014036FD90: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014036FD95: FF 15 3D 5C A9 01  call        qword ptr [0000000141E059D8h]
  000000014036FD9B: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014036FDA3: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014036FDAB: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014036FDB3: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  000000014036FDB8: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014036FDC0: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014036FDC8: E8 23 4B C9 FF     call        00000001400048F0
  000000014036FDCD: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014036FDD1: 83 C8 01           or          eax,1
  000000014036FDD4: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014036FDD8: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014036FDDD: FF 15 FD 5B A9 01  call        qword ptr [0000000141E059E0h]
  000000014036FDE3: 90                 nop
  000000014036FDE4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014036FDE9: FF 15 F1 5B A9 01  call        qword ptr [0000000141E059E0h]
  000000014036FDEF: 90                 nop
  000000014036FDF0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014036FDF5: FF 15 E5 5B A9 01  call        qword ptr [0000000141E059E0h]
  000000014036FDFB: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014036FE03: EB 36              jmp         000000014036FE3B
  000000014036FE05: EB 34              jmp         000000014036FE3B
  000000014036FE07: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014036FE0F: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014036FE13: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014036FE17: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014036FE1A: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014036FE22: FF 15 B0 5B A9 01  call        qword ptr [0000000141E059D8h]
  000000014036FE28: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014036FE2C: 83 C8 01           or          eax,1
  000000014036FE2F: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014036FE33: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014036FE3B: 48 81 C4 90 00 00  add         rsp,90h
                    00
  000000014036FE42: 5B                 pop         rbx
  000000014036FE43: C3                 ret

  Summary

     1E02000 .text
