Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140058CC0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140058CC5: 53                 push        rbx
  0000000140058CC6: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000140058CCD: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140058CD6: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140058CDE: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140058CE6: E8 25 C4 FF FF     call        0000000140055110
  0000000140058CEB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140058CF0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140058CF8: E8 73 6A 00 00     call        000000014005F770
  0000000140058CFD: 48 8B C8           mov         rcx,rax
  0000000140058D00: E8 DB 3D C2 00     call        0000000140C7CAE0
  0000000140058D05: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140058D09: C7 44 24 34 01 00  mov         dword ptr [rsp+34h],1
                    00 00
  0000000140058D11: EB 0B              jmp         0000000140058D1E
  0000000140058D13: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  0000000140058D17: 83 C0 01           add         eax,1
  0000000140058D1A: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140058D1E: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140058D22: 39 44 24 34        cmp         dword ptr [rsp+34h],eax
  0000000140058D26: 0F 8F E9 00 00 00  jg          0000000140058E15
  0000000140058D2C: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140058D34: E8 67 6A 00 00     call        000000014005F7A0
  0000000140058D39: 8B 54 24 34        mov         edx,dword ptr [rsp+34h]
  0000000140058D3D: 48 8B C8           mov         rcx,rax
  0000000140058D40: E8 1B 77 7D 00     call        0000000140830460
  0000000140058D45: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140058D4A: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140058D50: 0F 84 BA 00 00 00  je          0000000140058E10
  0000000140058D56: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140058D5B: 83 B8 C8 1C 00 00  cmp         dword ptr [rax+0000000000001CC8h],2
                    02
  0000000140058D62: 0F 84 A8 00 00 00  je          0000000140058E10
  0000000140058D68: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140058D6D: 8B 00              mov         eax,dword ptr [rax]
  0000000140058D6F: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140058D73: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140058D78: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140058D7B: 44 8B 44 24 50     mov         r8d,dword ptr [rsp+50h]
  0000000140058D80: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140058D85: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140058D8A: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  0000000140058D90: 90                 nop
  0000000140058D91: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140058D96: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140058D9B: FF 15 AF B1 DA 01  call        qword ptr [0000000141E03F50h]
  0000000140058DA1: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140058DA6: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140058DAB: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140058DB0: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140058DB5: FF 15 6D B4 DA 01  call        qword ptr [0000000141E04228h]
  0000000140058DBB: 48 8B D8           mov         rbx,rax
  0000000140058DBE: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140058DC6: E8 A5 69 00 00     call        000000014005F770
  0000000140058DCB: 48 8B C8           mov         rcx,rax
  0000000140058DCE: E8 FD 1E C2 00     call        0000000140C7ACD0
  0000000140058DD3: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140058DDC: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140058DE5: 45 33 C9           xor         r9d,r9d
  0000000140058DE8: 44 8B 44 24 50     mov         r8d,dword ptr [rsp+50h]
  0000000140058DED: 48 8B D3           mov         rdx,rbx
  0000000140058DF0: 48 8B C8           mov         rcx,rax
  0000000140058DF3: E8 E8 CB 83 00     call        00000001408959E0
  0000000140058DF8: 90                 nop
  0000000140058DF9: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140058DFE: FF 15 8C B4 DA 01  call        qword ptr [0000000141E04290h]
  0000000140058E04: 90                 nop
  0000000140058E05: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140058E0A: FF 15 D0 CB DA 01  call        qword ptr [0000000141E059E0h]
  0000000140058E10: E9 FE FE FF FF     jmp         0000000140058D13
  0000000140058E15: 48 81 C4 80 00 00  add         rsp,80h
                    00
  0000000140058E1C: 5B                 pop         rbx
  0000000140058E1D: C3                 ret

  Summary

     1E02000 .text
