Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141470440: 40 53              push        rbx
  0000000141470442: 48 81 EC E0 05 00  sub         rsp,5E0h
                    00
  0000000141470449: 83 64 24 70 00     and         dword ptr [rsp+70h],0
  000000014147044E: 48 8D 4C 24 74     lea         rcx,[rsp+74h]
  0000000141470453: 33 D2              xor         edx,edx
  0000000141470455: 41 B8 94 00 00 00  mov         r8d,94h
  000000014147045B: E8 F0 13 00 00     call        0000000141471850
  0000000141470460: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  0000000141470465: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  000000014147046D: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000141470475: 4C 89 5C 24 48     mov         qword ptr [rsp+48h],r11
  000000014147047A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014147047F: FF 15 93 2E 99 00  call        qword ptr [0000000141E03318h]
  0000000141470485: 48 8B 9C 24 08 02  mov         rbx,qword ptr [rsp+0000000000000208h]
                    00 00
  000000014147048D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000141470492: 48 8B CB           mov         rcx,rbx
  0000000141470495: 45 33 C0           xor         r8d,r8d
  0000000141470498: E8 11 E6 10 00     call        000000014157EAAE
  000000014147049D: 48 85 C0           test        rax,rax
  00000001414704A0: 74 3B              je          00000001414704DD
  00000001414704A2: 48 83 64 24 38 00  and         qword ptr [rsp+38h],0
  00000001414704A8: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001414704AD: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001414704B2: 48 89 4C 24 30     mov         qword ptr [rsp+30h],rcx
  00000001414704B7: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001414704BC: 4C 8B C8           mov         r9,rax
  00000001414704BF: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  00000001414704C4: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001414704CC: 4C 8B C3           mov         r8,rbx
  00000001414704CF: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000001414704D4: 33 C9              xor         ecx,ecx
  00000001414704D6: E8 E5 E5 10 00     call        000000014157EAC0
  00000001414704DB: EB 20              jmp         00000001414704FD
  00000001414704DD: 48 8B 84 24 E8 05  mov         rax,qword ptr [rsp+00000000000005E8h]
                    00 00
  00000001414704E5: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  00000001414704ED: 48 8D 84 24 E8 05  lea         rax,[rsp+00000000000005E8h]
                    00 00
  00000001414704F5: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001414704FD: 48 8B 84 24 E8 05  mov         rax,qword ptr [rsp+00000000000005E8h]
                    00 00
  0000000141470505: C7 44 24 70 17 04  mov         dword ptr [rsp+70h],0C0000417h
                    00 C0
  000000014147050D: C7 44 24 74 01 00  mov         dword ptr [rsp+74h],1
                    00 00
  0000000141470515: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014147051D: FF 15 1D 2E 99 00  call        qword ptr [0000000141E03340h]
  0000000141470523: 33 C9              xor         ecx,ecx
  0000000141470525: 8B D8              mov         ebx,eax
  0000000141470527: FF 15 E3 2D 99 00  call        qword ptr [0000000141E03310h]
  000000014147052D: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000141470532: FF 15 D0 2D 99 00  call        qword ptr [0000000141E03308h]
  0000000141470538: 85 C0              test        eax,eax
  000000014147053A: 75 0C              jne         0000000141470548
  000000014147053C: 85 DB              test        ebx,ebx
  000000014147053E: 75 08              jne         0000000141470548
  0000000141470540: 8D 48 02           lea         ecx,[rax+2]
  0000000141470543: E8 A8 2F 01 00     call        00000001414834F0
  0000000141470548: FF 15 3A 2C 99 00  call        qword ptr [0000000141E03188h]
  000000014147054E: BA 17 04 00 C0     mov         edx,0C0000417h
  0000000141470553: 48 8B C8           mov         rcx,rax
  0000000141470556: FF 15 24 30 99 00  call        qword ptr [0000000141E03580h]
  000000014147055C: 48 81 C4 E0 05 00  add         rsp,5E0h
                    00
  0000000141470563: 5B                 pop         rbx
  0000000141470564: C3                 ret

  Summary

     1E02000 .text
