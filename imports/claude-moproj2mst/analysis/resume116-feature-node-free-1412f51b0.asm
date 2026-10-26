Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F51B0: 48 8B C4           mov         rax,rsp
  00000001412F51B3: 41 54              push        r12
  00000001412F51B5: 41 55              push        r13
  00000001412F51B7: 41 56              push        r14
  00000001412F51B9: 48 83 EC 70        sub         rsp,70h
  00000001412F51BD: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412F51C6: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000001412F51CA: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000001412F51CE: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000001412F51D2: 48 89 78 20        mov         qword ptr [rax+20h],rdi
  00000001412F51D6: 44 8B F2           mov         r14d,edx
  00000001412F51D9: 4C 8B E1           mov         r12,rcx
  00000001412F51DC: 48 83 C1 20        add         rcx,20h
  00000001412F51E0: E8 DB 4D 01 00     call        0000000141309FC0
  00000001412F51E5: 90                 nop
  00000001412F51E6: 49 8B 7C 24 10     mov         rdi,qword ptr [r12+10h]
  00000001412F51EB: 48 85 FF           test        rdi,rdi
  00000001412F51EE: 75 0F              jne         00000001412F51FF
  00000001412F51F0: 49 8D 4C 24 20     lea         rcx,[r12+20h]
  00000001412F51F5: E8 06 4E 01 00     call        000000014130A000
  00000001412F51FA: E9 2C 01 00 00     jmp         00000001412F532B
  00000001412F51FF: 41 80 BC 24 50 04  cmp         byte ptr [r12+0000000000000450h],0
                    00 00 00
  00000001412F5208: 75 0F              jne         00000001412F5219
  00000001412F520A: 49 8D 4C 24 20     lea         rcx,[r12+20h]
  00000001412F520F: E8 EC 4D 01 00     call        000000014130A000
  00000001412F5214: E9 12 01 00 00     jmp         00000001412F532B
  00000001412F5219: 48 8B 47 28        mov         rax,qword ptr [rdi+28h]
  00000001412F521D: 48 8B 18           mov         rbx,qword ptr [rax]
  00000001412F5220: 48 8B 3F           mov         rdi,qword ptr [rdi]
  00000001412F5223: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  00000001412F5230: 49 8B 44 24 10     mov         rax,qword ptr [r12+10h]
  00000001412F5235: 48 8B 70 28        mov         rsi,qword ptr [rax+28h]
  00000001412F5239: 48 85 FF           test        rdi,rdi
  00000001412F523C: 74 05              je          00000001412F5243
  00000001412F523E: 48 3B 38           cmp         rdi,qword ptr [rax]
  00000001412F5241: 74 05              je          00000001412F5248
  00000001412F5243: E8 A8 B3 17 00     call        00000001414705F0
  00000001412F5248: 48 3B DE           cmp         rbx,rsi
  00000001412F524B: 0F 84 D0 00 00 00  je          00000001412F5321
  00000001412F5251: 48 85 FF           test        rdi,rdi
  00000001412F5254: 75 0A              jne         00000001412F5260
  00000001412F5256: E8 95 B3 17 00     call        00000001414705F0
  00000001412F525B: 45 33 DB           xor         r11d,r11d
  00000001412F525E: EB 03              jmp         00000001412F5263
  00000001412F5260: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000001412F5263: 49 3B 5B 28        cmp         rbx,qword ptr [r11+28h]
  00000001412F5267: 75 05              jne         00000001412F526E
  00000001412F5269: E8 82 B3 17 00     call        00000001414705F0
  00000001412F526E: 48 8B 73 10        mov         rsi,qword ptr [rbx+10h]
  00000001412F5272: 44 3B 36           cmp         r14d,dword ptr [rsi]
  00000001412F5275: 0F 85 81 00 00 00  jne         00000001412F52FC
  00000001412F527B: 49 8B 6C 24 10     mov         rbp,qword ptr [r12+10h]
  00000001412F5280: 48 85 FF           test        rdi,rdi
  00000001412F5283: 75 0A              jne         00000001412F528F
  00000001412F5285: E8 66 B3 17 00     call        00000001414705F0
  00000001412F528A: 45 33 DB           xor         r11d,r11d
  00000001412F528D: EB 03              jmp         00000001412F5292
  00000001412F528F: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000001412F5292: 49 3B 5B 28        cmp         rbx,qword ptr [r11+28h]
  00000001412F5296: 75 05              jne         00000001412F529D
  00000001412F5298: E8 53 B3 17 00     call        00000001414705F0
  00000001412F529D: 48 8B 3B           mov         rdi,qword ptr [rbx]
  00000001412F52A0: 48 3B 5D 28        cmp         rbx,qword ptr [rbp+28h]
  00000001412F52A4: 74 1E              je          00000001412F52C4
  00000001412F52A6: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F52AA: 48 89 38           mov         qword ptr [rax],rdi
  00000001412F52AD: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000001412F52B0: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000001412F52B4: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000001412F52B8: 48 8B CB           mov         rcx,rbx
  00000001412F52BB: E8 A8 A9 1E 00     call        00000001414DFC68
  00000001412F52C0: 48 FF 4D 30        dec         qword ptr [rbp+30h]
  00000001412F52C4: 48 89 7C 24 38     mov         qword ptr [rsp+38h],rdi
  00000001412F52C9: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000001412F52CD: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001412F52D2: 0F 28 44 24 30     movaps      xmm0,xmmword ptr [rsp+30h]
  00000001412F52D7: 66 0F 7F 44 24 40  movdqa      xmmword ptr [rsp+40h],xmm0
  00000001412F52DD: 48 8B CE           mov         rcx,rsi
  00000001412F52E0: E8 9B F9 FF FF     call        00000001412F4C80
  00000001412F52E5: 48 8B CE           mov         rcx,rsi
  00000001412F52E8: E8 7B A9 1E 00     call        00000001414DFC68
  00000001412F52ED: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000001412F52F2: 48 8B 7C 24 40     mov         rdi,qword ptr [rsp+40h]
  00000001412F52F7: E9 34 FF FF FF     jmp         00000001412F5230
  00000001412F52FC: 48 85 FF           test        rdi,rdi
  00000001412F52FF: 75 0A              jne         00000001412F530B
  00000001412F5301: E8 EA B2 17 00     call        00000001414705F0
  00000001412F5306: 45 33 DB           xor         r11d,r11d
  00000001412F5309: EB 03              jmp         00000001412F530E
  00000001412F530B: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000001412F530E: 49 3B 5B 28        cmp         rbx,qword ptr [r11+28h]
  00000001412F5312: 75 05              jne         00000001412F5319
  00000001412F5314: E8 D7 B2 17 00     call        00000001414705F0
  00000001412F5319: 48 8B 1B           mov         rbx,qword ptr [rbx]
  00000001412F531C: E9 0F FF FF FF     jmp         00000001412F5230
  00000001412F5321: 49 8D 4C 24 20     lea         rcx,[r12+20h]
  00000001412F5326: E8 D5 4C 01 00     call        000000014130A000
  00000001412F532B: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  00000001412F5330: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  00000001412F5334: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  00000001412F5338: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  00000001412F533C: 49 8B 7B 38        mov         rdi,qword ptr [r11+38h]
  00000001412F5340: 49 8B E3           mov         rsp,r11
  00000001412F5343: 41 5E              pop         r14
  00000001412F5345: 41 5D              pop         r13
  00000001412F5347: 41 5C              pop         r12
  00000001412F5349: C3                 ret

  Summary

     1E02000 .text
