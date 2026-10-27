Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670700D0: 40 55              push        rbp
  00000000670700D2: 56                 push        rsi
  00000000670700D3: 57                 push        rdi
  00000000670700D4: 48 83 EC 30        sub         rsp,30h
  00000000670700D8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670700E1: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000670700E6: 48 8B F9           mov         rdi,rcx
  00000000670700E9: F6 41 18 08        test        byte ptr [rcx+18h],8
  00000000670700ED: 0F 84 98 00 00 00  je          000000006707018B
  00000000670700F3: E8 38 FB FF FF     call        000000006706FC30
  00000000670700F8: 48 8B D8           mov         rbx,rax
  00000000670700FB: 33 F6              xor         esi,esi
  00000000670700FD: 8D 6E 01           lea         ebp,[rsi+1]
  0000000067070100: 48 85 C0           test        rax,rax
  0000000067070103: 74 32              je          0000000067070137
  0000000067070105: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067070108: 40 84 69 04        test        byte ptr [rcx+4],bpl
  000000006707010C: 74 0A              je          0000000067070118
  000000006707010E: 48 8B C8           mov         rcx,rax
  0000000067070111: E8 9A 5C FB FF     call        0000000067025DB0
  0000000067070116: EB 15              jmp         000000006707012D
  0000000067070118: 33 C0              xor         eax,eax
  000000006707011A: F0 0F B1 29        lock cmpxchg dword ptr [rcx],ebp
  000000006707011E: 0F 94 C1           sete        cl
  0000000067070121: 84 C9              test        cl,cl
  0000000067070123: 75 08              jne         000000006707012D
  0000000067070125: 48 8B CB           mov         rcx,rbx
  0000000067070128: E8 83 5A FB FF     call        0000000067025BB0
  000000006707012D: 48 0B DD           or          rbx,rbp
  0000000067070130: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000067070135: EB 08              jmp         000000006707013F
  0000000067070137: 48 8B DE           mov         rbx,rsi
  000000006707013A: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  000000006707013F: 48 89 7C 24 58     mov         qword ptr [rsp+58h],rdi
  0000000067070144: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000067070149: 48 8B 0D C8 3C 28  mov         rcx,qword ptr [00000000672F3E18h]
                    00
  0000000067070150: E8 0B FD FF FF     call        000000006706FE60
  0000000067070155: 90                 nop
  0000000067070156: F6 C3 01           test        bl,1
  0000000067070159: 74 30              je          000000006707018B
  000000006707015B: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006707015F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000067070164: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067070168: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006707016B: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006707016F: 74 0A              je          000000006707017B
  0000000067070171: 48 8B CB           mov         rcx,rbx
  0000000067070174: E8 B7 59 FB FF     call        0000000067025B30
  0000000067070179: EB 10              jmp         000000006707018B
  000000006707017B: 8B C5              mov         eax,ebp
  000000006707017D: F0 0F B1 31        lock cmpxchg dword ptr [rcx],esi
  0000000067070181: 74 08              je          000000006707018B
  0000000067070183: 48 8B CB           mov         rcx,rbx
  0000000067070186: E8 E5 59 FB FF     call        0000000067025B70
  000000006707018B: 48 8B CF           mov         rcx,rdi
  000000006707018E: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000067070193: 48 83 C4 30        add         rsp,30h
  0000000067070197: 5F                 pop         rdi
  0000000067070198: 5E                 pop         rsi
  0000000067070199: 5D                 pop         rbp
  000000006707019A: E9 11 02 FB FF     jmp         00000000670203B0

  Summary

      1A7000 .text
