Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140002900: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140002905: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140002909: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014000290E: 53                 push        rbx
  000000014000290F: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  0000000140002916: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140002922: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014000292A: E8 C1 CA 3D 01     call        00000001413DF3F0
  000000014000292F: 90                 nop
  0000000140002930: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140002938: EB 0B              jmp         0000000140002945
  000000014000293A: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014000293E: 83 C0 01           add         eax,1
  0000000140002941: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140002945: 8B 84 24 E8 00 00  mov         eax,dword ptr [rsp+00000000000000E8h]
                    00
  000000014000294C: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140002950: 0F 8D 45 02 00 00  jge         0000000140002B9B
  0000000140002956: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014000295B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140002963: 48 8B 04 C8        mov         rax,qword ptr [rax+rcx*8]
  0000000140002967: 0F BE 00           movsx       eax,byte ptr [rax]
  000000014000296A: 83 F8 2D           cmp         eax,2Dh
  000000014000296D: 0F 85 23 02 00 00  jne         0000000140002B96
  0000000140002973: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140002977: 83 C0 01           add         eax,1
  000000014000297A: 3B 84 24 E8 00 00  cmp         eax,dword ptr [rsp+00000000000000E8h]
                    00
  0000000140002981: 0F 8D 0F 02 00 00  jge         0000000140002B96
  0000000140002987: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014000298B: 83 C0 01           add         eax,1
  000000014000298E: 48 63 C8           movsxd      rcx,eax
  0000000140002991: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140002999: 48 8B 04 C8        mov         rax,qword ptr [rax+rcx*8]
  000000014000299D: 0F BE 00           movsx       eax,byte ptr [rax]
  00000001400029A0: 83 F8 2D           cmp         eax,2Dh
  00000001400029A3: 0F 84 ED 01 00 00  je          0000000140002B96
  00000001400029A9: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001400029AD: 83 C0 01           add         eax,1
  00000001400029B0: 48 98              cdqe
  00000001400029B2: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400029BA: 48 8B 14 C2        mov         rdx,qword ptr [rdx+rax*8]
  00000001400029BE: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400029C3: FF 15 B7 1A E0 01  call        qword ptr [0000000141E04480h]
  00000001400029C9: 90                 nop
  00000001400029CA: 48 8D 44 24 30     lea         rax,[rsp+30h]
  00000001400029CF: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400029D4: B2 22              mov         dl,22h
  00000001400029D6: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400029DB: FF 15 27 19 E0 01  call        qword ptr [0000000141E04308h]
  00000001400029E1: 48 8B D8           mov         rbx,rax
  00000001400029E4: 45 33 C0           xor         r8d,r8d
  00000001400029E7: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400029EC: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400029F1: FF 15 19 19 E0 01  call        qword ptr [0000000141E04310h]
  00000001400029F7: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001400029FC: 48 8B C8           mov         rcx,rax
  00000001400029FF: FF 15 13 19 E0 01  call        qword ptr [0000000141E04318h]
  0000000140002A05: 0F B7 13           movzx       edx,word ptr [rbx]
  0000000140002A08: 0F B7 08           movzx       ecx,word ptr [rax]
  0000000140002A0B: E8 A0 01 00 00     call        0000000140002BB0
  0000000140002A10: 0F B6 C0           movzx       eax,al
  0000000140002A13: 85 C0              test        eax,eax
  0000000140002A15: 0F 84 BB 00 00 00  je          0000000140002AD6
  0000000140002A1B: 48 8D 44 24 52     lea         rax,[rsp+52h]
  0000000140002A20: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140002A25: B2 22              mov         dl,22h
  0000000140002A27: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140002A2C: FF 15 D6 18 E0 01  call        qword ptr [0000000141E04308h]
  0000000140002A32: 48 8B D8           mov         rbx,rax
  0000000140002A35: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002A3A: FF 15 70 1A E0 01  call        qword ptr [0000000141E044B0h]
  0000000140002A40: 44 8B C0           mov         r8d,eax
  0000000140002A43: 41 83 E8 01        sub         r8d,1
  0000000140002A47: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140002A4C: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002A51: FF 15 B9 18 E0 01  call        qword ptr [0000000141E04310h]
  0000000140002A57: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140002A5C: 48 8B C8           mov         rcx,rax
  0000000140002A5F: FF 15 B3 18 E0 01  call        qword ptr [0000000141E04318h]
  0000000140002A65: 0F B7 13           movzx       edx,word ptr [rbx]
  0000000140002A68: 0F B7 08           movzx       ecx,word ptr [rax]
  0000000140002A6B: E8 40 01 00 00     call        0000000140002BB0
  0000000140002A70: 0F B6 C0           movzx       eax,al
  0000000140002A73: 85 C0              test        eax,eax
  0000000140002A75: 74 5F              je          0000000140002AD6
  0000000140002A77: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002A7C: FF 15 2E 1A E0 01  call        qword ptr [0000000141E044B0h]
  0000000140002A82: 44 8B C8           mov         r9d,eax
  0000000140002A85: 41 83 E9 02        sub         r9d,2
  0000000140002A89: 41 B8 01 00 00 00  mov         r8d,1
  0000000140002A8F: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140002A94: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002A99: FF 15 81 18 E0 01  call        qword ptr [0000000141E04320h]
  0000000140002A9F: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140002AA7: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140002AAF: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140002AB7: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140002ABF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002AC4: FF 15 A6 19 E0 01  call        qword ptr [0000000141E04470h]
  0000000140002ACA: 90                 nop
  0000000140002ACB: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140002AD0: FF 15 0A 2F E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002AD6: 48 63 44 24 20     movsxd      rax,dword ptr [rsp+20h]
  0000000140002ADB: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140002AE3: 48 8B 14 C2        mov         rdx,qword ptr [rdx+rax*8]
  0000000140002AE7: 48 83 C2 01        add         rdx,1
  0000000140002AEB: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140002AF3: FF 15 87 19 E0 01  call        qword ptr [0000000141E04480h]
  0000000140002AF9: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140002B01: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140002B09: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140002B11: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140002B19: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140002B21: FF 15 01 18 E0 01  call        qword ptr [0000000141E04328h]
  0000000140002B27: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140002B2F: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140002B37: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140002B3F: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140002B47: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140002B4F: E8 9C 02 00 00     call        0000000140002DF0
  0000000140002B54: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140002B59: 48 8B C8           mov         rcx,rax
  0000000140002B5C: FF 15 0E 19 E0 01  call        qword ptr [0000000141E04470h]
  0000000140002B62: 90                 nop
  0000000140002B63: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140002B6B: FF 15 6F 2E E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002B71: 90                 nop
  0000000140002B72: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140002B7A: FF 15 60 2E E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002B80: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140002B84: 83 C0 01           add         eax,1
  0000000140002B87: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140002B8B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140002B90: FF 15 4A 2E E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002B96: E9 9F FD FF FF     jmp         000000014000293A
  0000000140002B9B: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140002BA3: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  0000000140002BAA: 5B                 pop         rbx
  0000000140002BAB: C3                 ret

  Summary

     1E02000 .text
