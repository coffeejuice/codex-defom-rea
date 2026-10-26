Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140174120: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140174125: 56                 push        rsi
  0000000140174126: 57                 push        rdi
  0000000140174127: 48 81 EC 28 01 00  sub         rsp,128h
                    00
  000000014017412E: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140174136: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014017413A: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014017413F: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140174143: FF 15 F7 3A C9 01  call        qword ptr [0000000141E07C40h]
  0000000140174149: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014017414E: FF 15 A4 14 C9 01  call        qword ptr [0000000141E055F8h]
  0000000140174154: 0F B6 C0           movzx       eax,al
  0000000140174157: 85 C0              test        eax,eax
  0000000140174159: 75 6F              jne         00000001401741CA
  000000014017415B: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140174163: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140174167: 66 0F 6E 80 2C 01  movd        xmm0,dword ptr [rax+000000000000012Ch]
                    00 00
  000000014017416F: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  0000000140174173: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014017417B: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017417F: 66 0F 6E 98 28 01  movd        xmm3,dword ptr [rax+0000000000000128h]
                    00 00
  0000000140174187: F3 0F E6 DB        cvtdq2pd    xmm3,xmm3
  000000014017418B: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140174191: 66 0F 57 D2        xorpd       xmm2,xmm2
  0000000140174195: 66 0F 57 C9        xorpd       xmm1,xmm1
  0000000140174199: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014017419E: FF 15 4C 05 C9 01  call        qword ptr [0000000141E046F0h]
  00000001401741A4: 48 8D BC 24 F8 00  lea         rdi,[rsp+00000000000000F8h]
                    00 00
  00000001401741AC: 48 8B F0           mov         rsi,rax
  00000001401741AF: B9 20 00 00 00     mov         ecx,20h
  00000001401741B4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401741B6: 48 8D 7C 24 38     lea         rdi,[rsp+38h]
  00000001401741BB: 48 8D B4 24 F8 00  lea         rsi,[rsp+00000000000000F8h]
                    00 00
  00000001401741C3: B9 20 00 00 00     mov         ecx,20h
  00000001401741C8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401741CA: F2 0F 10 05 4E A3  movsd       xmm0,mmword ptr [00000001421CE520h]
                    05 02
  00000001401741D2: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001401741D8: F2 0F 10 1D 40 A3  movsd       xmm3,mmword ptr [00000001421CE520h]
                    05 02
  00000001401741E0: F2 0F 10 15 F0 52  movsd       xmm2,mmword ptr [00000001421D94D8h]
                    06 02
  00000001401741E8: F2 0F 10 0D E8 52  movsd       xmm1,mmword ptr [00000001421D94D8h]
                    06 02
  00000001401741F0: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001401741F5: FF 15 85 15 C9 01  call        qword ptr [0000000141E05780h]
  00000001401741FB: 41 B8 01 00 00 00  mov         r8d,1
  0000000140174201: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140174206: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014017420E: FF 15 24 3A C9 01  call        qword ptr [0000000141E07C38h]
  0000000140174214: 48 8D 44 24 78     lea         rax,[rsp+78h]
  0000000140174219: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140174221: BA FF FF 00 00     mov         edx,0FFFFh
  0000000140174226: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014017422E: E8 2D 08 E9 FF     call        0000000140004A60
  0000000140174233: 48 8B F8           mov         rdi,rax
  0000000140174236: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014017423E: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140174242: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  000000014017424A: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014017424E: FF 15 3C 34 C9 01  call        qword ptr [0000000141E07690h]
  0000000140174254: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014017425C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140174260: 4C 8B C7           mov         r8,rdi
  0000000140174263: 48 8B D0           mov         rdx,rax
  0000000140174266: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014017426A: FF 15 C0 39 C9 01  call        qword ptr [0000000141E07C30h]
  0000000140174270: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140174278: FF 15 DA FD C8 01  call        qword ptr [0000000141E04058h]
  000000014017427E: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140174286: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014017428A: 48 8B D0           mov         rdx,rax
  000000014017428D: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140174291: FF 15 91 39 C9 01  call        qword ptr [0000000141E07C28h]
  0000000140174297: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000014017429F: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001401742A7: FF 15 73 39 C9 01  call        qword ptr [0000000141E07C20h]
  00000001401742AD: 48 8B C8           mov         rcx,rax
  00000001401742B0: FF 15 62 39 C9 01  call        qword ptr [0000000141E07C18h]
  00000001401742B6: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001401742BC: F2 0F 10 4C 24 30  movsd       xmm1,mmword ptr [rsp+30h]
  00000001401742C2: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001401742CA: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401742CE: E8 DD CC 00 00     call        0000000140180FB0
  00000001401742D3: 8B D0              mov         edx,eax
  00000001401742D5: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001401742DD: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401742E1: 89 50 54           mov         dword ptr [rax+54h],edx
  00000001401742E4: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001401742EC: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401742F0: 8B 50 54           mov         edx,dword ptr [rax+54h]
  00000001401742F3: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001401742FB: E8 C0 8F 01 00     call        000000014018D2C0
  0000000140174300: F2 0F 10 4C 24 30  movsd       xmm1,mmword ptr [rsp+30h]
  0000000140174306: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014017430E: E8 FD 8F 01 00     call        000000014018D310
  0000000140174313: 48 81 C4 28 01 00  add         rsp,128h
                    00
  000000014017431A: 5F                 pop         rdi
  000000014017431B: 5E                 pop         rsi
  000000014017431C: C3                 ret

  Summary

     1E02000 .text
