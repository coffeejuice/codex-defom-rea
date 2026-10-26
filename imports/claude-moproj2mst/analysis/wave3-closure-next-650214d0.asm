Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650214D0: 40 57              push        rdi
  00000000650214D2: 41 54              push        r12
  00000000650214D4: 41 55              push        r13
  00000000650214D6: 41 56              push        r14
  00000000650214D8: 41 57              push        r15
  00000000650214DA: 48 83 EC 30        sub         rsp,30h
  00000000650214DE: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650214E7: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000650214EC: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  00000000650214F1: 48 89 74 24 78     mov         qword ptr [rsp+78h],rsi
  00000000650214F6: 4C 8B F2           mov         r14,rdx
  00000000650214F9: 4C 8B E1           mov         r12,rcx
  00000000650214FC: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  0000000065021504: E8 97 E9 FF FF     call        000000006501FEA0
  0000000065021509: 48 8B E8           mov         rbp,rax
  000000006502150C: 4D 85 F6           test        r14,r14
  000000006502150F: 0F 84 4D 01 00 00  je          0000000065021662
  0000000065021515: 48 85 C0           test        rax,rax
  0000000065021518: 0F 84 44 01 00 00  je          0000000065021662
  000000006502151E: 4C 8B 00           mov         r8,qword ptr [rax]
  0000000065021521: 41 83 78 14 00     cmp         dword ptr [r8+14h],0
  0000000065021526: 0F 84 36 01 00 00  je          0000000065021662
  000000006502152C: 49 8B 16           mov         rdx,qword ptr [r14]
  000000006502152F: 49 8B CE           mov         rcx,r14
  0000000065021532: FF 12              call        qword ptr [rdx]
  0000000065021534: 48 8B C8           mov         rcx,rax
  0000000065021537: FF 15 2B 58 67 00  call        qword ptr [0000000065696D68h]
  000000006502153D: 48 8B D0           mov         rdx,rax
  0000000065021540: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065021545: FF 15 DD 57 67 00  call        qword ptr [0000000065696D28h]
  000000006502154B: 90                 nop
  000000006502154C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065021551: FF 15 89 56 67 00  call        qword ptr [0000000065696BE0h]
  0000000065021557: 44 8B F8           mov         r15d,eax
  000000006502155A: 4C 8B 4D 00        mov         r9,qword ptr [rbp]
  000000006502155E: 41 8B 49 20        mov         ecx,dword ptr [r9+20h]
  0000000065021562: 85 C9              test        ecx,ecx
  0000000065021564: 0F 84 7F 00 00 00  je          00000000650215E9
  000000006502156A: 33 D2              xor         edx,edx
  000000006502156C: F7 F1              div         eax,ecx
  000000006502156E: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  0000000065021572: 48 8D 34 D1        lea         rsi,[rcx+rdx*8]
  0000000065021576: 48 8B 0E           mov         rcx,qword ptr [rsi]
  0000000065021579: 49 3B C9           cmp         rcx,r9
  000000006502157C: 74 6E              je          00000000650215EC
  000000006502157E: 66 90              nop
  0000000065021580: 44 3B 79 08        cmp         r15d,dword ptr [rcx+8]
  0000000065021584: 75 55              jne         00000000650215DB
  0000000065021586: 4C 8D 69 10        lea         r13,[rcx+10h]
  000000006502158A: 49 8B CD           mov         rcx,r13
  000000006502158D: FF 15 2D 55 67 00  call        qword ptr [0000000065696AC0h]
  0000000065021593: 8B D8              mov         ebx,eax
  0000000065021595: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006502159A: FF 15 20 55 67 00  call        qword ptr [0000000065696AC0h]
  00000000650215A0: 3B C3              cmp         eax,ebx
  00000000650215A2: 75 37              jne         00000000650215DB
  00000000650215A4: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000650215A9: FF 15 11 55 67 00  call        qword ptr [0000000065696AC0h]
  00000000650215AF: 48 63 F8           movsxd      rdi,eax
  00000000650215B2: 49 8B CD           mov         rcx,r13
  00000000650215B5: FF 15 9D 54 67 00  call        qword ptr [0000000065696A58h]
  00000000650215BB: 48 8B D8           mov         rbx,rax
  00000000650215BE: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000650215C3: FF 15 8F 54 67 00  call        qword ptr [0000000065696A58h]
  00000000650215C9: 48 8B C8           mov         rcx,rax
  00000000650215CC: 4C 8B C7           mov         r8,rdi
  00000000650215CF: 48 8B D3           mov         rdx,rbx
  00000000650215D2: E8 A7 FB 5B 00     call        00000000655E117E
  00000000650215D7: 85 C0              test        eax,eax
  00000000650215D9: 74 11              je          00000000650215EC
  00000000650215DB: 48 8B 36           mov         rsi,qword ptr [rsi]
  00000000650215DE: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000650215E1: 48 3B 4D 00        cmp         rcx,qword ptr [rbp]
  00000000650215E5: 75 99              jne         0000000065021580
  00000000650215E7: EB 03              jmp         00000000650215EC
  00000000650215E9: 48 8B F5           mov         rsi,rbp
  00000000650215EC: 48 8B 1E           mov         rbx,qword ptr [rsi]
  00000000650215EF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000650215F4: FF 15 56 54 67 00  call        qword ptr [0000000065696A50h]
  00000000650215FA: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000650215FE: 48 3B D9           cmp         rbx,rcx
  0000000065021601: 74 11              je          0000000065021614
  0000000065021603: 48 8D 53 18        lea         rdx,[rbx+18h]
  0000000065021607: 49 8B CC           mov         rcx,r12
  000000006502160A: E8 B1 7A 21 00     call        00000000652390C0
  000000006502160F: 49 8B C4           mov         rax,r12
  0000000065021612: EB 59              jmp         000000006502166D
  0000000065021614: FF 15 7E 55 67 00  call        qword ptr [0000000065696B98h]
  000000006502161A: 48 8B D8           mov         rbx,rax
  000000006502161D: 48 3B 45 00        cmp         rax,qword ptr [rbp]
  0000000065021621: 74 3F              je          0000000065021662
  0000000065021623: 48 8D 4B 10        lea         rcx,[rbx+10h]
  0000000065021627: FF 15 CB 56 67 00  call        qword ptr [0000000065696CF8h]
  000000006502162D: 48 8B D0           mov         rdx,rax
  0000000065021630: 49 8B CE           mov         rcx,r14
  0000000065021633: FF 15 27 57 67 00  call        qword ptr [0000000065696D60h]
  0000000065021639: 84 C0              test        al,al
  000000006502163B: 75 14              jne         0000000065021651
  000000006502163D: 48 8B CB           mov         rcx,rbx
  0000000065021640: FF 15 02 55 67 00  call        qword ptr [0000000065696B48h]
  0000000065021646: 48 8B D8           mov         rbx,rax
  0000000065021649: 48 3B 45 00        cmp         rax,qword ptr [rbp]
  000000006502164D: 75 D4              jne         0000000065021623
  000000006502164F: EB 11              jmp         0000000065021662
  0000000065021651: 48 8D 53 18        lea         rdx,[rbx+18h]
  0000000065021655: 49 8B CC           mov         rcx,r12
  0000000065021658: E8 63 7A 21 00     call        00000000652390C0
  000000006502165D: 49 8B C4           mov         rax,r12
  0000000065021660: EB 0B              jmp         000000006502166D
  0000000065021662: 49 8B CC           mov         rcx,r12
  0000000065021665: E8 C6 9C FF FF     call        000000006501B330
  000000006502166A: 49 8B C4           mov         rax,r12
  000000006502166D: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000065021672: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  0000000065021677: 48 8B 74 24 78     mov         rsi,qword ptr [rsp+78h]
  000000006502167C: 48 83 C4 30        add         rsp,30h
  0000000065021680: 41 5F              pop         r15
  0000000065021682: 41 5E              pop         r14
  0000000065021684: 41 5D              pop         r13
  0000000065021686: 41 5C              pop         r12
  0000000065021688: 5F                 pop         rdi
  0000000065021689: C3                 ret

  Summary

      695000 .text
