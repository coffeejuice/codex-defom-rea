Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014149CFB0: 40 53              push        rbx
  000000014149CFB2: 55                 push        rbp
  000000014149CFB3: 56                 push        rsi
  000000014149CFB4: 57                 push        rdi
  000000014149CFB5: 41 54              push        r12
  000000014149CFB7: 41 55              push        r13
  000000014149CFB9: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  000000014149CFC0: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014149CFCC: 48 8B D9           mov         rbx,rcx
  000000014149CFCF: 48 8D 69 38        lea         rbp,[rcx+38h]
  000000014149CFD3: 48 8B CD           mov         rcx,rbp
  000000014149CFD6: E8 75 FA FF FF     call        000000014149CA50
  000000014149CFDB: 48 8D 53 30        lea         rdx,[rbx+30h]
  000000014149CFDF: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014149CFE4: E8 47 6F BA FE     call        0000000140043F30
  000000014149CFE9: 90                 nop
  000000014149CFEA: 83 CF FF           or          edi,0FFFFFFFFh
  000000014149CFED: 4C 8B AC 24 88 00  mov         r13,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014149CFF5: 8B 8C 24 80 00 00  mov         ecx,dword ptr [rsp+0000000000000080h]
                    00
  000000014149CFFC: 0F 1F 40 00        nop         dword ptr [rax]
  000000014149D000: 8B C1              mov         eax,ecx
  000000014149D002: FF C1              inc         ecx
  000000014149D004: 89 8C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ecx
                    00
  000000014149D00B: 85 C0              test        eax,eax
  000000014149D00D: 0F 85 6B 02 00 00  jne         000000014149D27E
  000000014149D013: 4C 3B AC 24 90 00  cmp         r13,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014149D01B: 0F 84 5D 02 00 00  je          000000014149D27E
  000000014149D021: 85 C9              test        ecx,ecx
  000000014149D023: 0F 84 3C 02 00 00  je          000000014149D265
  000000014149D029: 49 8B D5           mov         rdx,r13
  000000014149D02C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014149D031: FF 15 29 70 96 00  call        qword ptr [0000000141E04060h]
  000000014149D037: 90                 nop
  000000014149D038: 89 BC 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],edi
                    00
  000000014149D03F: C7 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],2
                    00 02 00 00 00
  000000014149D04A: 4C 8D 8C 24 08 01  lea         r9,[rsp+0000000000000108h]
                    00 00
  000000014149D052: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  000000014149D05A: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000014149D05F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014149D064: FF 15 BE 6F 96 00  call        qword ptr [0000000141E04028h]
  000000014149D06A: 90                 nop
  000000014149D06B: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000014149D070: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014149D075: E8 B6 6E BA FE     call        0000000140043F30
  000000014149D07A: 90                 nop
  000000014149D07B: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000014149D080: 8B 4C 24 40        mov         ecx,dword ptr [rsp+40h]
  000000014149D084: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  000000014149D090: 8B C1              mov         eax,ecx
  000000014149D092: FF C1              inc         ecx
  000000014149D094: 89 4C 24 40        mov         dword ptr [rsp+40h],ecx
  000000014149D098: 85 C0              test        eax,eax
  000000014149D09A: 0F 85 00 01 00 00  jne         000000014149D1A0
  000000014149D0A0: 48 3B 5C 24 50     cmp         rbx,qword ptr [rsp+50h]
  000000014149D0A5: 0F 84 F5 00 00 00  je          000000014149D1A0
  000000014149D0AB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000014149D0B0: 85 C9              test        ecx,ecx
  000000014149D0B2: 0F 84 D5 00 00 00  je          000000014149D18D
  000000014149D0B8: 48 8B CB           mov         rcx,rbx
  000000014149D0BB: FF 15 D7 6B 96 00  call        qword ptr [0000000141E03C98h]
  000000014149D0C1: 84 C0              test        al,al
  000000014149D0C3: 0F 84 B5 00 00 00  je          000000014149D17E
  000000014149D0C9: 49 8B D5           mov         rdx,r13
  000000014149D0CC: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  000000014149D0D4: FF 15 FE 88 96 00  call        qword ptr [0000000141E059D8h]
  000000014149D0DA: 90                 nop
  000000014149D0DB: B2 2F              mov         dl,2Fh
  000000014149D0DD: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000014149D0E5: FF 15 6D 72 96 00  call        qword ptr [0000000141E04358h]
  000000014149D0EB: 0F B7 10           movzx       edx,word ptr [rax]
  000000014149D0EE: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  000000014149D0F6: FF 15 4C 71 96 00  call        qword ptr [0000000141E04248h]
  000000014149D0FC: 48 8B D3           mov         rdx,rbx
  000000014149D0FF: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  000000014149D107: FF 15 03 74 96 00  call        qword ptr [0000000141E04510h]
  000000014149D10D: 45 33 C0           xor         r8d,r8d
  000000014149D110: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  000000014149D118: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014149D120: FF 15 7A 6B 96 00  call        qword ptr [0000000141E03CA0h]
  000000014149D126: 90                 nop
  000000014149D127: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014149D12F: FF 15 73 6B 96 00  call        qword ptr [0000000141E03CA8h]
  000000014149D135: 84 C0              test        al,al
  000000014149D137: 74 1A              je          000000014149D153
  000000014149D139: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014149D141: FF 15 69 6B 96 00  call        qword ptr [0000000141E03CB0h]
  000000014149D147: 48 8B C8           mov         rcx,rax
  000000014149D14A: 48 8B D5           mov         rdx,rbp
  000000014149D14D: E8 3E FB FF FF     call        000000014149CC90
  000000014149D152: 90                 nop
  000000014149D153: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014149D15B: FF 15 57 6B 96 00  call        qword ptr [0000000141E03CB8h]
  000000014149D161: 90                 nop
  000000014149D162: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014149D16A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D16E: 75 0E              jne         000000014149D17E
  000000014149D170: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014149D178: FF 15 B2 6B 96 00  call        qword ptr [0000000141E03D30h]
  000000014149D17E: 8B 4C 24 40        mov         ecx,dword ptr [rsp+40h]
  000000014149D182: FF C9              dec         ecx
  000000014149D184: 89 4C 24 40        mov         dword ptr [rsp+40h],ecx
  000000014149D188: E9 23 FF FF FF     jmp         000000014149D0B0
  000000014149D18D: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000014149D192: 48 83 C3 08        add         rbx,8
  000000014149D196: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000014149D19B: E9 F0 FE FF FF     jmp         000000014149D090
  000000014149D1A0: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014149D1A5: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D1A9: 75 4A              jne         000000014149D1F5
  000000014149D1AB: 4C 8B 64 24 38     mov         r12,qword ptr [rsp+38h]
  000000014149D1B0: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000014149D1B5: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000014149D1BA: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000014149D1BF: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000014149D1C4: 48 3B F3           cmp         rsi,rbx
  000000014149D1C7: 74 22              je          000000014149D1EB
  000000014149D1C9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000014149D1D0: 48 83 EB 08        sub         rbx,8
  000000014149D1D4: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000014149D1D7: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000014149D1DB: 75 09              jne         000000014149D1E6
  000000014149D1DD: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000014149D1E0: FF 15 4A 6B 96 00  call        qword ptr [0000000141E03D30h]
  000000014149D1E6: 48 3B DE           cmp         rbx,rsi
  000000014149D1E9: 75 E5              jne         000000014149D1D0
  000000014149D1EB: 49 8B CC           mov         rcx,r12
  000000014149D1EE: FF 15 04 70 96 00  call        qword ptr [0000000141E041F8h]
  000000014149D1F4: 90                 nop
  000000014149D1F5: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014149D1FA: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D1FE: 75 45              jne         000000014149D245
  000000014149D200: 4C 8B 64 24 28     mov         r12,qword ptr [rsp+28h]
  000000014149D205: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000014149D20A: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000014149D20F: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000014149D214: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000014149D219: 48 3B F3           cmp         rsi,rbx
  000000014149D21C: 74 1D              je          000000014149D23B
  000000014149D21E: 66 90              nop
  000000014149D220: 48 83 EB 08        sub         rbx,8
  000000014149D224: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000014149D227: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000014149D22B: 75 09              jne         000000014149D236
  000000014149D22D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000014149D230: FF 15 FA 6A 96 00  call        qword ptr [0000000141E03D30h]
  000000014149D236: 48 3B DE           cmp         rbx,rsi
  000000014149D239: 75 E5              jne         000000014149D220
  000000014149D23B: 49 8B CC           mov         rcx,r12
  000000014149D23E: FF 15 B4 6F 96 00  call        qword ptr [0000000141E041F8h]
  000000014149D244: 90                 nop
  000000014149D245: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014149D24A: FF 15 20 6E 96 00  call        qword ptr [0000000141E04070h]
  000000014149D250: 8B 8C 24 80 00 00  mov         ecx,dword ptr [rsp+0000000000000080h]
                    00
  000000014149D257: FF C9              dec         ecx
  000000014149D259: 89 8C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ecx
                    00
  000000014149D260: E9 BC FD FF FF     jmp         000000014149D021
  000000014149D265: 4C 8B AC 24 88 00  mov         r13,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014149D26D: 49 83 C5 08        add         r13,8
  000000014149D271: 4C 89 AC 24 88 00  mov         qword ptr [rsp+0000000000000088h],r13
                    00 00
  000000014149D279: E9 82 FD FF FF     jmp         000000014149D000
  000000014149D27E: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  000000014149D283: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D287: 75 4B              jne         000000014149D2D4
  000000014149D289: 4C 8B 64 24 78     mov         r12,qword ptr [rsp+78h]
  000000014149D28E: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000014149D293: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000014149D298: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000014149D29D: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000014149D2A2: 48 3B F3           cmp         rsi,rbx
  000000014149D2A5: 74 24              je          000000014149D2CB
  000000014149D2A7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  000000014149D2B0: 48 83 EB 08        sub         rbx,8
  000000014149D2B4: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000014149D2B7: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000014149D2BB: 75 09              jne         000000014149D2C6
  000000014149D2BD: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000014149D2C0: FF 15 6A 6A 96 00  call        qword ptr [0000000141E03D30h]
  000000014149D2C6: 48 3B DE           cmp         rbx,rsi
  000000014149D2C9: 75 E5              jne         000000014149D2B0
  000000014149D2CB: 49 8B CC           mov         rcx,r12
  000000014149D2CE: FF 15 24 6F 96 00  call        qword ptr [0000000141E041F8h]
  000000014149D2D4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014149D2D9: FF 15 E1 69 96 00  call        qword ptr [0000000141E03CC0h]
  000000014149D2DF: 90                 nop
  000000014149D2E0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014149D2E5: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000014149D2E8: 39 48 0C           cmp         dword ptr [rax+0Ch],ecx
  000000014149D2EB: 0F 84 82 00 00 00  je          000000014149D373
  000000014149D2F1: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014149D2F6: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014149D2FB: E8 F0 07 BD FE     call        000000014006DAF0
  000000014149D300: 90                 nop
  000000014149D301: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014149D306: 8B 4C 24 60        mov         ecx,dword ptr [rsp+60h]
  000000014149D30A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000014149D310: 8B C1              mov         eax,ecx
  000000014149D312: FF C1              inc         ecx
  000000014149D314: 89 4C 24 60        mov         dword ptr [rsp+60h],ecx
  000000014149D318: 85 C0              test        eax,eax
  000000014149D31A: 75 35              jne         000000014149D351
  000000014149D31C: 48 3B 54 24 70     cmp         rdx,qword ptr [rsp+70h]
  000000014149D321: 74 2E              je          000000014149D351
  000000014149D323: 48 8B 1A           mov         rbx,qword ptr [rdx]
  000000014149D326: 85 C9              test        ecx,ecx
  000000014149D328: 74 17              je          000000014149D341
  000000014149D32A: 48 8B D5           mov         rdx,rbp
  000000014149D32D: 48 8B CB           mov         rcx,rbx
  000000014149D330: E8 5B F9 FF FF     call        000000014149CC90
  000000014149D335: 8B 4C 24 60        mov         ecx,dword ptr [rsp+60h]
  000000014149D339: FF C9              dec         ecx
  000000014149D33B: 89 4C 24 60        mov         dword ptr [rsp+60h],ecx
  000000014149D33F: EB E5              jmp         000000014149D326
  000000014149D341: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014149D346: 48 83 C2 08        add         rdx,8
  000000014149D34A: 48 89 54 24 68     mov         qword ptr [rsp+68h],rdx
  000000014149D34F: EB BF              jmp         000000014149D310
  000000014149D351: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014149D356: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D35A: 75 12              jne         000000014149D36E
  000000014149D35C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014149D361: FF 15 91 6E 96 00  call        qword ptr [0000000141E041F8h]
  000000014149D367: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014149D36C: EB 05              jmp         000000014149D373
  000000014149D36E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014149D373: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000014149D377: 75 0B              jne         000000014149D384
  000000014149D379: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014149D37E: FF 15 74 6E 96 00  call        qword ptr [0000000141E041F8h]
  000000014149D384: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  000000014149D38B: 41 5D              pop         r13
  000000014149D38D: 41 5C              pop         r12
  000000014149D38F: 5F                 pop         rdi
  000000014149D390: 5E                 pop         rsi
  000000014149D391: 5D                 pop         rbp
  000000014149D392: 5B                 pop         rbx
  000000014149D393: C3                 ret

  Summary

     1E02000 .text
