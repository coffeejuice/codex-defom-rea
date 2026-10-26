Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414733AC: 40 53              push        rbx
  00000001414733AE: 56                 push        rsi
  00000001414733AF: 57                 push        rdi
  00000001414733B0: 41 54              push        r12
  00000001414733B2: 41 55              push        r13
  00000001414733B4: 48 83 EC 40        sub         rsp,40h
  00000001414733B8: 49 8B D8           mov         rbx,r8
  00000001414733BB: 8B FA              mov         edi,edx
  00000001414733BD: 4C 8B E9           mov         r13,rcx
  00000001414733C0: 4C 8B E1           mov         r12,rcx
  00000001414733C3: 48 8B F1           mov         rsi,rcx
  00000001414733C6: 48 85 C9           test        rcx,rcx
  00000001414733C9: 75 2A              jne         00000001414733F5
  00000001414733CB: 85 D2              test        edx,edx
  00000001414733CD: 74 26              je          00000001414733F5
  00000001414733CF: E8 60 0D 00 00     call        0000000141474134
  00000001414733D4: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  00000001414733DA: 48 21 74 24 20     and         qword ptr [rsp+20h],rsi
  00000001414733DF: 45 33 C9           xor         r9d,r9d
  00000001414733E2: 45 33 C0           xor         r8d,r8d
  00000001414733E5: 33 D2              xor         edx,edx
  00000001414733E7: 33 C9              xor         ecx,ecx
  00000001414733E9: E8 7A D1 FF FF     call        0000000141470568
  00000001414733EE: 33 C0              xor         eax,eax
  00000001414733F0: E9 8B 01 00 00     jmp         0000000141473580
  00000001414733F5: 33 C0              xor         eax,eax
  00000001414733F7: 85 D2              test        edx,edx
  00000001414733F9: 0F 99 C0           setns       al
  00000001414733FC: 85 C0              test        eax,eax
  00000001414733FE: 75 27              jne         0000000141473427
  0000000141473400: E8 2F 0D 00 00     call        0000000141474134
  0000000141473405: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014147340B: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141473411: 45 33 C9           xor         r9d,r9d
  0000000141473414: 45 33 C0           xor         r8d,r8d
  0000000141473417: 33 D2              xor         edx,edx
  0000000141473419: 33 C9              xor         ecx,ecx
  000000014147341B: E8 48 D1 FF FF     call        0000000141470568
  0000000141473420: 33 C0              xor         eax,eax
  0000000141473422: E9 59 01 00 00     jmp         0000000141473580
  0000000141473427: 33 C0              xor         eax,eax
  0000000141473429: 48 85 DB           test        rbx,rbx
  000000014147342C: 0F 95 C0           setne       al
  000000014147342F: 85 C0              test        eax,eax
  0000000141473431: 75 27              jne         000000014147345A
  0000000141473433: E8 FC 0C 00 00     call        0000000141474134
  0000000141473438: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014147343E: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141473444: 45 33 C9           xor         r9d,r9d
  0000000141473447: 45 33 C0           xor         r8d,r8d
  000000014147344A: 33 D2              xor         edx,edx
  000000014147344C: 33 C9              xor         ecx,ecx
  000000014147344E: E8 15 D1 FF FF     call        0000000141470568
  0000000141473453: 33 C0              xor         eax,eax
  0000000141473455: E9 26 01 00 00     jmp         0000000141473580
  000000014147345A: 85 D2              test        edx,edx
  000000014147345C: 75 07              jne         0000000141473465
  000000014147345E: 33 C0              xor         eax,eax
  0000000141473460: E9 1B 01 00 00     jmp         0000000141473580
  0000000141473465: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  000000014147346D: 48 8B CB           mov         rcx,rbx
  0000000141473470: E8 23 A8 FF FF     call        000000014146DC98
  0000000141473475: 90                 nop
  0000000141473476: F6 43 18 40        test        byte ptr [rbx+18h],40h
  000000014147347A: 0F 85 9A 00 00 00  jne         000000014147351A
  0000000141473480: 48 8B CB           mov         rcx,rbx
  0000000141473483: E8 C0 65 00 00     call        0000000141479A48
  0000000141473488: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000014147348B: 74 2A              je          00000001414734B7
  000000014147348D: 83 F8 FE           cmp         eax,0FFFFFFFEh
  0000000141473490: 74 25              je          00000001414734B7
  0000000141473492: 48 63 D0           movsxd      rdx,eax
  0000000141473495: 48 8B CA           mov         rcx,rdx
  0000000141473498: 48 C1 F9 05        sar         rcx,5
  000000014147349C: 4C 8D 05 5D 0C BE  lea         r8,[0000000145054100h]
                    03
  00000001414734A3: 83 E2 1F           and         edx,1Fh
  00000001414734A6: 48 6B D2 58        imul        rdx,rdx,58h
  00000001414734AA: 49 03 14 C8        add         rdx,qword ptr [r8+rcx*8]
  00000001414734AE: 48 8D 0D 3B B2 6E  lea         rcx,[0000000143B5E6F0h]
                    02
  00000001414734B5: EB 11              jmp         00000001414734C8
  00000001414734B7: 48 8D 0D 32 B2 6E  lea         rcx,[0000000143B5E6F0h]
                    02
  00000001414734BE: 48 8B D1           mov         rdx,rcx
  00000001414734C1: 4C 8D 05 38 0C BE  lea         r8,[0000000145054100h]
                    03
  00000001414734C8: F6 42 38 7F        test        byte ptr [rdx+38h],7Fh
  00000001414734CC: 75 25              jne         00000001414734F3
  00000001414734CE: 83 F8 FF           cmp         eax,0FFFFFFFFh
  00000001414734D1: 74 1A              je          00000001414734ED
  00000001414734D3: 83 F8 FE           cmp         eax,0FFFFFFFEh
  00000001414734D6: 74 15              je          00000001414734ED
  00000001414734D8: 48 63 C8           movsxd      rcx,eax
  00000001414734DB: 48 8B C1           mov         rax,rcx
  00000001414734DE: 48 C1 F8 05        sar         rax,5
  00000001414734E2: 83 E1 1F           and         ecx,1Fh
  00000001414734E5: 48 6B C9 58        imul        rcx,rcx,58h
  00000001414734E9: 49 03 0C C0        add         rcx,qword ptr [r8+rax*8]
  00000001414734ED: F6 41 38 80        test        byte ptr [rcx+38h],80h
  00000001414734F1: 74 27              je          000000014147351A
  00000001414734F3: E8 3C 0C 00 00     call        0000000141474134
  00000001414734F8: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  00000001414734FE: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141473504: 45 33 C9           xor         r9d,r9d
  0000000141473507: 45 33 C0           xor         r8d,r8d
  000000014147350A: 33 D2              xor         edx,edx
  000000014147350C: 33 C9              xor         ecx,ecx
  000000014147350E: E8 55 D0 FF FF     call        0000000141470568
  0000000141473513: 33 F6              xor         esi,esi
  0000000141473515: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000014147351A: 48 85 F6           test        rsi,rsi
  000000014147351D: 74 56              je          0000000141473575
  000000014147351F: 83 EF 01           sub         edi,1
  0000000141473522: 89 7C 24 78        mov         dword ptr [rsp+78h],edi
  0000000141473526: 74 48              je          0000000141473570
  0000000141473528: 83 43 08 FF        add         dword ptr [rbx+8],0FFFFFFFFh
  000000014147352C: 78 0E              js          000000014147353C
  000000014147352E: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000141473531: 0F B6 08           movzx       ecx,byte ptr [rax]
  0000000141473534: 48 FF C0           inc         rax
  0000000141473537: 48 89 03           mov         qword ptr [rbx],rax
  000000014147353A: EB 0A              jmp         0000000141473546
  000000014147353C: 48 8B CB           mov         rcx,rbx
  000000014147353F: E8 D0 2D 01 00     call        0000000141486314
  0000000141473544: 8B C8              mov         ecx,eax
  0000000141473546: 89 4C 24 70        mov         dword ptr [rsp+70h],ecx
  000000014147354A: 83 F9 FF           cmp         ecx,0FFFFFFFFh
  000000014147354D: 75 0E              jne         000000014147355D
  000000014147354F: 4D 3B E5           cmp         r12,r13
  0000000141473552: 75 1C              jne         0000000141473570
  0000000141473554: 33 F6              xor         esi,esi
  0000000141473556: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000014147355B: EB 18              jmp         0000000141473575
  000000014147355D: 41 88 0C 24        mov         byte ptr [r12],cl
  0000000141473561: 49 FF C4           inc         r12
  0000000141473564: 4C 89 64 24 38     mov         qword ptr [rsp+38h],r12
  0000000141473569: 80 F9 0A           cmp         cl,0Ah
  000000014147356C: 74 02              je          0000000141473570
  000000014147356E: EB AF              jmp         000000014147351F
  0000000141473570: 41 C6 04 24 00     mov         byte ptr [r12],0
  0000000141473575: 48 8B CB           mov         rcx,rbx
  0000000141473578: E8 AB A7 FF FF     call        000000014146DD28
  000000014147357D: 48 8B C6           mov         rax,rsi
  0000000141473580: 48 83 C4 40        add         rsp,40h
  0000000141473584: 41 5D              pop         r13
  0000000141473586: 41 5C              pop         r12
  0000000141473588: 5F                 pop         rdi
  0000000141473589: 5E                 pop         rsi
  000000014147358A: 5B                 pop         rbx
  000000014147358B: C3                 ret
  000000014147358C: CC                 int         3

  Summary

     1E02000 .text
