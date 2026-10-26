Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006504A2F0: 48 8B C4           mov         rax,rsp
  000000006504A2F3: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  000000006504A2F7: 55                 push        rbp
  000000006504A2F8: 56                 push        rsi
  000000006504A2F9: 57                 push        rdi
  000000006504A2FA: 41 54              push        r12
  000000006504A2FC: 41 55              push        r13
  000000006504A2FE: 41 56              push        r14
  000000006504A300: 41 57              push        r15
  000000006504A302: 48 83 EC 40        sub         rsp,40h
  000000006504A306: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006504A30F: 48 89 58 08        mov         qword ptr [rax+8],rbx
  000000006504A313: 4D 8B F0           mov         r14,r8
  000000006504A316: 48 8B F2           mov         rsi,rdx
  000000006504A319: 4C 8B F9           mov         r15,rcx
  000000006504A31C: 33 DB              xor         ebx,ebx
  000000006504A31E: 89 58 20           mov         dword ptr [rax+20h],ebx
  000000006504A321: 49 8B D0           mov         rdx,r8
  000000006504A324: E8 17 FA FF FF     call        0000000065049D40
  000000006504A329: 84 C0              test        al,al
  000000006504A32B: 74 0F              je          000000006504A33C
  000000006504A32D: 41 8B 46 08        mov         eax,dword ptr [r14+8]
  000000006504A331: 41 33 47 08        xor         eax,dword ptr [r15+8]
  000000006504A335: A9 F0 FF FF FF     test        eax,0FFFFFFF0h
  000000006504A33A: 74 0A              je          000000006504A346
  000000006504A33C: 41 F7 47 08 F0 FF  test        dword ptr [r15+8],0FFFFFFF0h
                    FF FF
  000000006504A344: 75 7A              jne         000000006504A3C0
  000000006504A346: 49 8B 3E           mov         rdi,qword ptr [r14]
  000000006504A349: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  000000006504A34E: 48 8B CF           mov         rcx,rdi
  000000006504A351: FF 15 69 C5 64 00  call        qword ptr [00000000656968C0h]
  000000006504A357: 90                 nop
  000000006504A358: 41 8B 5F 08        mov         ebx,dword ptr [r15+8]
  000000006504A35C: 41 33 5E 08        xor         ebx,dword ptr [r14+8]
  000000006504A360: 83 E3 0F           and         ebx,0Fh
  000000006504A363: 41 33 5F 08        xor         ebx,dword ptr [r15+8]
  000000006504A367: 89 5C 24 30        mov         dword ptr [rsp+30h],ebx
  000000006504A36B: 48 89 3E           mov         qword ptr [rsi],rdi
  000000006504A36E: 48 8B CF           mov         rcx,rdi
  000000006504A371: FF 15 49 C5 64 00  call        qword ptr [00000000656968C0h]
  000000006504A377: 89 5E 08           mov         dword ptr [rsi+8],ebx
  000000006504A37A: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],1
                    00 01 00 00 00
  000000006504A385: 48 8B CF           mov         rcx,rdi
  000000006504A388: FF 15 EA C5 64 00  call        qword ptr [0000000065696978h]
  000000006504A38E: 84 C0              test        al,al
  000000006504A390: 75 26              jne         000000006504A3B8
  000000006504A392: 48 85 FF           test        rdi,rdi
  000000006504A395: 74 21              je          000000006504A3B8
  000000006504A397: 48 8D 4F 08        lea         rcx,[rdi+8]
  000000006504A39B: 4C 8D 0D 1E 28 0D  lea         r9,[000000006511CBC0h]
                    00
  000000006504A3A2: BA 08 00 00 00     mov         edx,8
  000000006504A3A7: 44 8D 42 34        lea         r8d,[rdx+34h]
  000000006504A3AB: E8 58 6D 59 00     call        00000000655E1108
  000000006504A3B0: 48 8B CF           mov         rcx,rdi
  000000006504A3B3: E8 F6 6C 59 00     call        00000000655E10AE
  000000006504A3B8: 48 8B C6           mov         rax,rsi
  000000006504A3BB: E9 E4 00 00 00     jmp         000000006504A4A4
  000000006504A3C0: 49 8B 0F           mov         rcx,qword ptr [r15]
  000000006504A3C3: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  000000006504A3C8: FF 15 F2 C4 64 00  call        qword ptr [00000000656968C0h]
  000000006504A3CE: 45 8B 5F 08        mov         r11d,dword ptr [r15+8]
  000000006504A3D2: 44 89 5C 24 30     mov         dword ptr [rsp+30h],r11d
  000000006504A3D7: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006504A3DC: E8 5F F8 FF FF     call        0000000065049C40
  000000006504A3E1: 4C 8B E3           mov         r12,rbx
  000000006504A3E4: 4C 8B 6C 24 28     mov         r13,qword ptr [rsp+28h]
  000000006504A3E9: 49 83 C5 08        add         r13,8
  000000006504A3ED: 0F 1F 00           nop         dword ptr [rax]
  000000006504A3F0: 41 8B 47 08        mov         eax,dword ptr [r15+8]
  000000006504A3F4: C1 E8 04           shr         eax,4
  000000006504A3F7: 0F A3 D8           bt          eax,ebx
  000000006504A3FA: 72 38              jb          000000006504A434
  000000006504A3FC: 49 8B FC           mov         rdi,r12
  000000006504A3FF: 49 8B F5           mov         rsi,r13
  000000006504A402: BD 03 00 00 00     mov         ebp,3
  000000006504A407: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  000000006504A410: 49 8B 06           mov         rax,qword ptr [r14]
  000000006504A413: 48 8D 54 07 08     lea         rdx,[rdi+rax+8]
  000000006504A418: 48 8B CE           mov         rcx,rsi
  000000006504A41B: E8 D0 29 0D 00     call        000000006511CDF0
  000000006504A420: 48 81 C7 A0 00 00  add         rdi,0A0h
                    00
  000000006504A427: 48 81 C6 A0 00 00  add         rsi,0A0h
                    00
  000000006504A42E: 48 83 ED 01        sub         rbp,1
  000000006504A432: 75 DC              jne         000000006504A410
  000000006504A434: FF C3              inc         ebx
  000000006504A436: 49 83 C4 08        add         r12,8
  000000006504A43A: 49 83 C5 08        add         r13,8
  000000006504A43E: 83 FB 14           cmp         ebx,14h
  000000006504A441: 7C AD              jl          000000006504A3F0
  000000006504A443: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000006504A44B: 48 8B 7C 24 28     mov         rdi,qword ptr [rsp+28h]
  000000006504A450: 48 89 3B           mov         qword ptr [rbx],rdi
  000000006504A453: 48 8B CF           mov         rcx,rdi
  000000006504A456: FF 15 64 C4 64 00  call        qword ptr [00000000656968C0h]
  000000006504A45C: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000006504A460: 89 43 08           mov         dword ptr [rbx+8],eax
  000000006504A463: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],1
                    00 01 00 00 00
  000000006504A46E: 48 8B CF           mov         rcx,rdi
  000000006504A471: FF 15 01 C5 64 00  call        qword ptr [0000000065696978h]
  000000006504A477: 84 C0              test        al,al
  000000006504A479: 75 26              jne         000000006504A4A1
  000000006504A47B: 48 85 FF           test        rdi,rdi
  000000006504A47E: 74 21              je          000000006504A4A1
  000000006504A480: 4C 8D 0D 39 27 0D  lea         r9,[000000006511CBC0h]
                    00
  000000006504A487: BA 08 00 00 00     mov         edx,8
  000000006504A48C: 44 8D 42 34        lea         r8d,[rdx+34h]
  000000006504A490: 48 8D 4F 08        lea         rcx,[rdi+8]
  000000006504A494: E8 6F 6C 59 00     call        00000000655E1108
  000000006504A499: 48 8B CF           mov         rcx,rdi
  000000006504A49C: E8 0D 6C 59 00     call        00000000655E10AE
  000000006504A4A1: 48 8B C3           mov         rax,rbx
  000000006504A4A4: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006504A4AC: 48 83 C4 40        add         rsp,40h
  000000006504A4B0: 41 5F              pop         r15
  000000006504A4B2: 41 5E              pop         r14
  000000006504A4B4: 41 5D              pop         r13
  000000006504A4B6: 41 5C              pop         r12
  000000006504A4B8: 5F                 pop         rdi
  000000006504A4B9: 5E                 pop         rsi
  000000006504A4BA: 5D                 pop         rbp
  000000006504A4BB: C3                 ret
  000000006504A4BC: CC                 int         3

  Summary

      695000 .text
