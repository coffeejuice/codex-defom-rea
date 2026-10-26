Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067062420: 66 44 89 44 24 18  mov         word ptr [rsp+18h],r8w
  0000000067062426: 66 89 54 24 10     mov         word ptr [rsp+10h],dx
  000000006706242B: 53                 push        rbx
  000000006706242C: 56                 push        rsi
  000000006706242D: 57                 push        rdi
  000000006706242E: 41 54              push        r12
  0000000067062430: 48 83 EC 28        sub         rsp,28h
  0000000067062434: 4C 8B E1           mov         r12,rcx
  0000000067062437: 66 41 3B D0        cmp         dx,r8w
  000000006706243B: 76 0B              jbe         0000000067062448
  000000006706243D: 66 44 89 44 24 58  mov         word ptr [rsp+58h],r8w
  0000000067062443: 66 89 54 24 60     mov         word ptr [rsp+60h],dx
  0000000067062448: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006706244C: 48 63 70 08        movsxd      rsi,dword ptr [rax+8]
  0000000067062450: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  0000000067062454: 8D 7E 01           lea         edi,[rsi+1]
  0000000067062457: 41 3B F8           cmp         edi,r8d
  000000006706245A: 7F 14              jg          0000000067062470
  000000006706245C: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  0000000067062460: 75 23              jne         0000000067062485
  0000000067062462: 3B 78 08           cmp         edi,dword ptr [rax+8]
  0000000067062465: 7D 1E              jge         0000000067062485
  0000000067062467: 41 8B C0           mov         eax,r8d
  000000006706246A: D1 F8              sar         eax,1
  000000006706246C: 3B F8              cmp         edi,eax
  000000006706246E: 7D 15              jge         0000000067062485
  0000000067062470: 45 33 C9           xor         r9d,r9d
  0000000067062473: 8B D7              mov         edx,edi
  0000000067062475: 45 8D 41 04        lea         r8d,[r9+4]
  0000000067062479: 41 8D 49 14        lea         ecx,[r9+14h]
  000000006706247D: E8 1E A1 01 00     call        000000006707C5A0
  0000000067062482: 44 8B C0           mov         r8d,eax
  0000000067062485: 49 8D 4C 24 08     lea         rcx,[r12+8]
  000000006706248A: 8B D7              mov         edx,edi
  000000006706248C: E8 0F 5E 0A 00     call        00000000671082A0
  0000000067062491: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  0000000067062496: 8B 08              mov         ecx,dword ptr [rax]
  0000000067062498: 83 F9 01           cmp         ecx,1
  000000006706249B: 74 16              je          00000000670624B3
  000000006706249D: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000000670624A2: 49 8D 4C 24 08     lea         rcx,[r12+8]
  00000000670624A7: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000000670624AB: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670624AE: E8 ED 5D 0A 00     call        00000000671082A0
  00000000670624B3: 49 8B 4C 24 08     mov         rcx,qword ptr [r12+8]
  00000000670624B8: 0F B7 44 24 58     movzx       eax,word ptr [rsp+58h]
  00000000670624BD: 66 89 44 B1 10     mov         word ptr [rcx+rsi*4+10h],ax
  00000000670624C2: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000000670624C7: 8B 08              mov         ecx,dword ptr [rax]
  00000000670624C9: 83 F9 01           cmp         ecx,1
  00000000670624CC: 74 16              je          00000000670624E4
  00000000670624CE: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000000670624D3: 49 8D 4C 24 08     lea         rcx,[r12+8]
  00000000670624D8: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000000670624DC: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670624DF: E8 BC 5D 0A 00     call        00000000671082A0
  00000000670624E4: 0F B7 4C 24 58     movzx       ecx,word ptr [rsp+58h]
  00000000670624E9: 44 0F B7 44 24 60  movzx       r8d,word ptr [rsp+60h]
  00000000670624EF: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000000670624F4: 41 0F B7 D0        movzx       edx,r8w
  00000000670624F8: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  00000000670624FD: 4C 89 6C 24 20     mov         qword ptr [rsp+20h],r13
  0000000067062502: 66 2B D1           sub         dx,cx
  0000000067062505: 66 FF C2           inc         dx
  0000000067062508: 66 89 54 B0 12     mov         word ptr [rax+rsi*4+12h],dx
  000000006706250D: 8B C1              mov         eax,ecx
  000000006706250F: 41 8B C8           mov         ecx,r8d
  0000000067062512: 2B C8              sub         ecx,eax
  0000000067062514: 83 F9 40           cmp         ecx,40h
  0000000067062517: 0F 8D 0D 01 00 00  jge         000000006706262A
  000000006706251D: 99                 cdq
  000000006706251E: 83 E2 3F           and         edx,3Fh
  0000000067062521: 03 C2              add         eax,edx
  0000000067062523: 83 E0 3F           and         eax,3Fh
  0000000067062526: 2B C2              sub         eax,edx
  0000000067062528: 4C 63 E8           movsxd      r13,eax
  000000006706252B: 41 8B C0           mov         eax,r8d
  000000006706252E: 99                 cdq
  000000006706252F: 83 E2 3F           and         edx,3Fh
  0000000067062532: 03 C2              add         eax,edx
  0000000067062534: 83 E0 3F           and         eax,3Fh
  0000000067062537: 2B C2              sub         eax,edx
  0000000067062539: 44 3B E8           cmp         r13d,eax
  000000006706253C: 7F 4C              jg          000000006706258A
  000000006706253E: 41 2B C5           sub         eax,r13d
  0000000067062541: 4A 8D 34 AD 10 00  lea         rsi,[r13*4+0000000000000010h]
                    00 00
  0000000067062549: 33 FF              xor         edi,edi
  000000006706254B: 8D 68 01           lea         ebp,[rax+1]
  000000006706254E: 66 90              nop
  0000000067062550: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  0000000067062555: 8B 08              mov         ecx,dword ptr [rax]
  0000000067062557: 83 F9 01           cmp         ecx,1
  000000006706255A: 74 16              je          0000000067062572
  000000006706255C: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  0000000067062561: 49 8D 4C 24 18     lea         rcx,[r12+18h]
  0000000067062566: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006706256A: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006706256D: E8 2E 5D 0A 00     call        00000000671082A0
  0000000067062572: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  0000000067062577: 48 83 C6 04        add         rsi,4
  000000006706257B: 48 83 ED 01        sub         rbp,1
  000000006706257F: 89 7C 06 FC        mov         dword ptr [rsi+rax-4],edi
  0000000067062583: 75 CB              jne         0000000067062550
  0000000067062585: E9 15 01 00 00     jmp         000000006706269F
  000000006706258A: 33 FF              xor         edi,edi
  000000006706258C: 85 C0              test        eax,eax
  000000006706258E: 78 45              js          00000000670625D5
  0000000067062590: 8D 77 10           lea         esi,[rdi+10h]
  0000000067062593: 8D 68 01           lea         ebp,[rax+1]
  0000000067062596: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  00000000670625A0: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  00000000670625A5: 8B 08              mov         ecx,dword ptr [rax]
  00000000670625A7: 83 F9 01           cmp         ecx,1
  00000000670625AA: 74 16              je          00000000670625C2
  00000000670625AC: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  00000000670625B1: 49 8D 4C 24 18     lea         rcx,[r12+18h]
  00000000670625B6: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000000670625BA: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670625BD: E8 DE 5C 0A 00     call        00000000671082A0
  00000000670625C2: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  00000000670625C7: 48 83 C6 04        add         rsi,4
  00000000670625CB: 48 83 ED 01        sub         rbp,1
  00000000670625CF: 89 7C 06 FC        mov         dword ptr [rsi+rax-4],edi
  00000000670625D3: 75 CB              jne         00000000670625A0
  00000000670625D5: 49 83 FD 40        cmp         r13,40h
  00000000670625D9: 0F 8D C0 00 00 00  jge         000000006706269F
  00000000670625DF: 4A 8D 1C AD 10 00  lea         rbx,[r13*4+0000000000000010h]
                    00 00
  00000000670625E7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  00000000670625F0: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  00000000670625F5: 8B 08              mov         ecx,dword ptr [rax]
  00000000670625F7: 83 F9 01           cmp         ecx,1
  00000000670625FA: 74 16              je          0000000067062612
  00000000670625FC: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  0000000067062601: 49 8D 4C 24 18     lea         rcx,[r12+18h]
  0000000067062606: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006706260A: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006706260D: E8 8E 5C 0A 00     call        00000000671082A0
  0000000067062612: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  0000000067062617: 48 83 C3 04        add         rbx,4
  000000006706261B: 48 81 FB 10 01 00  cmp         rbx,110h
                    00
  0000000067062622: 89 7C 03 FC        mov         dword ptr [rbx+rax-4],edi
  0000000067062626: 7C C8              jl          00000000670625F0
  0000000067062628: EB 75              jmp         000000006706269F
  000000006706262A: 49 8B 44 24 18     mov         rax,qword ptr [r12+18h]
  000000006706262F: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  0000000067062633: 41 83 F8 40        cmp         r8d,40h
  0000000067062637: 7C 19              jl          0000000067062652
  0000000067062639: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006706263D: 75 29              jne         0000000067062668
  000000006706263F: 83 78 08 40        cmp         dword ptr [rax+8],40h
  0000000067062643: 7E 23              jle         0000000067062668
  0000000067062645: 41 8B C0           mov         eax,r8d
  0000000067062648: 83 E0 FE           and         eax,0FFFFFFFEh
  000000006706264B: 3D 80 00 00 00     cmp         eax,80h
  0000000067062650: 7E 16              jle         0000000067062668
  0000000067062652: 45 33 C9           xor         r9d,r9d
  0000000067062655: 41 8D 51 40        lea         edx,[r9+40h]
  0000000067062659: 45 8D 41 04        lea         r8d,[r9+4]
  000000006706265D: 8D 4A D4           lea         ecx,[rdx-2Ch]
  0000000067062660: E8 3B 9F 01 00     call        000000006707C5A0
  0000000067062665: 44 8B C0           mov         r8d,eax
  0000000067062668: 49 8D 4C 24 18     lea         rcx,[r12+18h]
  000000006706266D: BA 40 00 00 00     mov         edx,40h
  0000000067062672: E8 29 5C 0A 00     call        00000000671082A0
  0000000067062677: 4D 8B 5C 24 18     mov         r11,qword ptr [r12+18h]
  000000006706267C: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  0000000067062680: 85 C0              test        eax,eax
  0000000067062682: 74 1B              je          000000006706269F
  0000000067062684: 49 8D 4C 83 10     lea         rcx,[r11+rax*4+10h]
  0000000067062689: 49 8D 43 10        lea         rax,[r11+10h]
  000000006706268D: 48 3B C8           cmp         rcx,rax
  0000000067062690: 74 0D              je          000000006706269F
  0000000067062692: 33 FF              xor         edi,edi
  0000000067062694: 48 83 E9 04        sub         rcx,4
  0000000067062698: 89 39              mov         dword ptr [rcx],edi
  000000006706269A: 48 3B C8           cmp         rcx,rax
  000000006706269D: 75 F5              jne         0000000067062694
  000000006706269F: 4C 8B 6C 24 20     mov         r13,qword ptr [rsp+20h]
  00000000670626A4: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  00000000670626A9: 48 83 C4 28        add         rsp,28h
  00000000670626AD: 41 5C              pop         r12
  00000000670626AF: 5F                 pop         rdi
  00000000670626B0: 5E                 pop         rsi
  00000000670626B1: 5B                 pop         rbx
  00000000670626B2: C3                 ret

  Summary

      1A7000 .text
