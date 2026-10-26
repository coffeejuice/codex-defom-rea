Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C340: 4C 8B DC           mov         r11,rsp
  000000006704C343: 57                 push        rdi
  000000006704C344: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  000000006704C34B: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006704C354: 49 89 5B 10        mov         qword ptr [r11+10h],rbx
  000000006704C358: 49 89 6B 18        mov         qword ptr [r11+18h],rbp
  000000006704C35C: 49 89 73 20        mov         qword ptr [r11+20h],rsi
  000000006704C360: 41 0F 29 73 E8     movaps      xmmword ptr [r11-18h],xmm6
  000000006704C365: 41 8B E9           mov         ebp,r9d
  000000006704C368: 49 8B D8           mov         rbx,r8
  000000006704C36B: 48 8B C2           mov         rax,rdx
  000000006704C36E: 48 8B F1           mov         rsi,rcx
  000000006704C371: 33 FF              xor         edi,edi
  000000006704C373: 41 89 7B 08        mov         dword ptr [r11+8],edi
  000000006704C377: 89 7C 24 34        mov         dword ptr [rsp+34h],edi
  000000006704C37B: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006704C380: 48 89 4C 24 38     mov         qword ptr [rsp+38h],rcx
  000000006704C385: C7 44 24 30 00 01  mov         dword ptr [rsp+30h],100h
                    00 00
  000000006704C38D: B9 A0 00 00 00     mov         ecx,0A0h
  000000006704C392: 66 39 4E 08        cmp         word ptr [rsi+8],cx
  000000006704C396: 75 1A              jne         000000006704C3B2
  000000006704C398: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006704C39D: 48 8B C8           mov         rcx,rax
  000000006704C3A0: E8 FB 80 02 00     call        00000000670744A0
  000000006704C3A5: 90                 nop
  000000006704C3A6: BF 01 00 00 00     mov         edi,1
  000000006704C3AB: 89 BC 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],edi
                    00
  000000006704C3B2: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  000000006704C3B7: 44 8B C5           mov         r8d,ebp
  000000006704C3BA: 48 8B D0           mov         rdx,rax
  000000006704C3BD: 48 8B CE           mov         rcx,rsi
  000000006704C3C0: E8 CB FD FF FF     call        000000006704C190
  000000006704C3C5: 84 C0              test        al,al
  000000006704C3C7: 40 0F 94 C6        sete        sil
  000000006704C3CB: 40 F6 C7 01        test        dil,1
  000000006704C3CF: 74 15              je          000000006704C3E6
  000000006704C3D1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000006704C3D6: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704C3DA: 75 0A              jne         000000006704C3E6
  000000006704C3DC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006704C3E1: E8 EA 3C 02 00     call        00000000670700D0
  000000006704C3E6: 40 84 F6           test        sil,sil
  000000006704C3E9: 74 22              je          000000006704C40D
  000000006704C3EB: 48 85 DB           test        rbx,rbx
  000000006704C3EE: 74 03              je          000000006704C3F3
  000000006704C3F0: C6 03 00           mov         byte ptr [rbx],0
  000000006704C3F3: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000006704C3F8: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000006704C3FD: 48 3B C8           cmp         rcx,rax
  000000006704C400: 74 05              je          000000006704C407
  000000006704C402: E8 A9 3F FD FF     call        00000000670203B0
  000000006704C407: 66 0F 57 C0        xorpd       xmm0,xmm0
  000000006704C40B: EB 2C              jmp         000000006704C439
  000000006704C40D: 45 33 C0           xor         r8d,r8d
  000000006704C410: 48 8B D3           mov         rdx,rbx
  000000006704C413: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000006704C418: E8 93 D0 FF FF     call        00000000670494B0
  000000006704C41D: 66 0F 28 F0        movapd      xmm6,xmm0
  000000006704C421: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000006704C426: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000006704C42B: 48 3B C8           cmp         rcx,rax
  000000006704C42E: 74 05              je          000000006704C435
  000000006704C430: E8 7B 3F FD FF     call        00000000670203B0
  000000006704C435: 66 0F 28 C6        movapd      xmm0,xmm6
  000000006704C439: 4C 8D 9C 24 60 01  lea         r11,[rsp+0000000000000160h]
                    00 00
  000000006704C441: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  000000006704C445: 49 8B 6B 20        mov         rbp,qword ptr [r11+20h]
  000000006704C449: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  000000006704C44D: 41 0F 28 73 F0     movaps      xmm6,xmmword ptr [r11-10h]
  000000006704C452: 49 8B E3           mov         rsp,r11
  000000006704C455: 5F                 pop         rdi
  000000006704C456: C3                 ret

  Summary

      1A7000 .text
