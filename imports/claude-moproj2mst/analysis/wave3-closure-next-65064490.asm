Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065064490: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000065064495: 53                 push        rbx
  0000000065064496: 55                 push        rbp
  0000000065064497: 56                 push        rsi
  0000000065064498: 57                 push        rdi
  0000000065064499: 41 54              push        r12
  000000006506449B: 48 83 EC 50        sub         rsp,50h
  000000006506449F: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650644A8: 45 8B E0           mov         r12d,r8d
  00000000650644AB: 48 8B FA           mov         rdi,rdx
  00000000650644AE: 48 8B E9           mov         rbp,rcx
  00000000650644B1: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  00000000650644BC: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000650644C0: 48 8B D3           mov         rdx,rbx
  00000000650644C3: 48 8B CF           mov         rcx,rdi
  00000000650644C6: E8 15 C7 FB FF     call        0000000065020BE0
  00000000650644CB: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],1
                    00 01 00 00 00
  00000000650644D6: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000650644DA: 8B 81 D0 01 00 00  mov         eax,dword ptr [rcx+00000000000001D0h]
  00000000650644E0: D1 E8              shr         eax,1
  00000000650644E2: 24 01              and         al,1
  00000000650644E4: 0F 85 60 01 00 00  jne         000000006506464A
  00000000650644EA: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  00000000650644EE: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  00000000650644F2: A8 01              test        al,1
  00000000650644F4: 74 26              je          000000006506451C
  00000000650644F6: 0F B6 81 CE 01 00  movzx       eax,byte ptr [rcx+00000000000001CEh]
                    00
  00000000650644FD: 24 01              and         al,1
  00000000650644FF: 75 1B              jne         000000006506451C
  0000000065064501: 48 8B 85 88 00 00  mov         rax,qword ptr [rbp+0000000000000088h]
                    00
  0000000065064508: 48 85 C0           test        rax,rax
  000000006506450B: 0F 84 39 01 00 00  je          000000006506464A
  0000000065064511: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000065064516: 0F 84 2E 01 00 00  je          000000006506464A
  000000006506451C: 48 8B CB           mov         rcx,rbx
  000000006506451F: FF 15 43 23 63 00  call        qword ptr [0000000065696868h]
  0000000065064525: 48 8B F0           mov         rsi,rax
  0000000065064528: 48 85 C0           test        rax,rax
  000000006506452B: 0F 84 B1 00 00 00  je          00000000650645E2
  0000000065064531: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065064535: 8B 91 D0 01 00 00  mov         edx,dword ptr [rcx+00000000000001D0h]
  000000006506453B: D1 EA              shr         edx,1
  000000006506453D: 80 E2 01           and         dl,1
  0000000065064540: 0F 85 04 01 00 00  jne         000000006506464A
  0000000065064546: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506454B: E8 20 2A FB FF     call        0000000065016F70
  0000000065064550: 90                 nop
  0000000065064551: 48 8B D0           mov         rdx,rax
  0000000065064554: 48 8B CF           mov         rcx,rdi
  0000000065064557: E8 F4 12 1A 00     call        0000000065205850
  000000006506455C: 84 C0              test        al,al
  000000006506455E: 0F 94 C3           sete        bl
  0000000065064561: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065064566: E8 65 5C FE FF     call        000000006504A1D0
  000000006506456B: 84 DB              test        bl,bl
  000000006506456D: 74 5E              je          00000000650645CD
  000000006506456F: 48 8B CE           mov         rcx,rsi
  0000000065064572: E8 99 FE FF FF     call        0000000065064410
  0000000065064577: 48 8B D0           mov         rdx,rax
  000000006506457A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506457F: E8 2C 63 FE FF     call        000000006504A8B0
  0000000065064584: 90                 nop
  0000000065064585: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000065064589: 83 E0 0F           and         eax,0Fh
  000000006506458C: 41 C1 E4 04        shl         r12d,4
  0000000065064590: 41 0B C4           or          eax,r12d
  0000000065064593: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000065064597: 4C 8B C7           mov         r8,rdi
  000000006506459A: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006506459F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650645A4: E8 47 5D FE FF     call        000000006504A2F0
  00000000650645A9: 90                 nop
  00000000650645AA: 48 8B D0           mov         rdx,rax
  00000000650645AD: 48 8B CF           mov         rcx,rdi
  00000000650645B0: E8 AB 55 FE FF     call        0000000065049B60
  00000000650645B5: 90                 nop
  00000000650645B6: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650645BB: E8 10 5C FE FF     call        000000006504A1D0
  00000000650645C0: 90                 nop
  00000000650645C1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650645C6: E8 05 5C FE FF     call        000000006504A1D0
  00000000650645CB: EB 7D              jmp         000000006506464A
  00000000650645CD: 48 8B CE           mov         rcx,rsi
  00000000650645D0: E8 3B FE FF FF     call        0000000065064410
  00000000650645D5: 48 8B D0           mov         rdx,rax
  00000000650645D8: 48 8B CF           mov         rcx,rdi
  00000000650645DB: E8 80 55 FE FF     call        0000000065049B60
  00000000650645E0: EB 68              jmp         000000006506464A
  00000000650645E2: 48 8B 8D 88 00 00  mov         rcx,qword ptr [rbp+0000000000000088h]
                    00
  00000000650645E9: 48 85 C9           test        rcx,rcx
  00000000650645EC: 74 5C              je          000000006506464A
  00000000650645EE: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  00000000650645F3: 74 55              je          000000006506464A
  00000000650645F5: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000650645FA: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000000650645FE: E8 FD C3 52 00     call        0000000065590A00
  0000000065064603: 90                 nop
  0000000065064604: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000065064608: 83 E0 0F           and         eax,0Fh
  000000006506460B: 41 C1 E4 04        shl         r12d,4
  000000006506460F: 41 0B C4           or          eax,r12d
  0000000065064612: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000065064616: 4C 8B C7           mov         r8,rdi
  0000000065064619: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006506461E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065064623: E8 C8 5C FE FF     call        000000006504A2F0
  0000000065064628: 90                 nop
  0000000065064629: 48 8B D0           mov         rdx,rax
  000000006506462C: 48 8B CF           mov         rcx,rdi
  000000006506462F: E8 2C 55 FE FF     call        0000000065049B60
  0000000065064634: 90                 nop
  0000000065064635: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506463A: E8 91 5B FE FF     call        000000006504A1D0
  000000006506463F: 90                 nop
  0000000065064640: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065064645: E8 86 5B FE FF     call        000000006504A1D0
  000000006506464A: 83 67 08 0F        and         dword ptr [rdi+8],0Fh
  000000006506464E: 48 8B C7           mov         rax,rdi
  0000000065064651: 48 83 C4 50        add         rsp,50h
  0000000065064655: 41 5C              pop         r12
  0000000065064657: 5F                 pop         rdi
  0000000065064658: 5E                 pop         rsi
  0000000065064659: 5D                 pop         rbp
  000000006506465A: 5B                 pop         rbx
  000000006506465B: C3                 ret
  000000006506465C: CC                 int         3

  Summary

      695000 .text
