Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650451C0: 40 53              push        rbx
  00000000650451C2: 55                 push        rbp
  00000000650451C3: 56                 push        rsi
  00000000650451C4: 57                 push        rdi
  00000000650451C5: 48 81 EC B8 01 00  sub         rsp,1B8h
                    00
  00000000650451CC: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000650451D0: 48 8B F9           mov         rdi,rcx
  00000000650451D3: 48 8B F2           mov         rsi,rdx
  00000000650451D6: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650451D9: 48 8B CB           mov         rcx,rbx
  00000000650451DC: 48 89 9C 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rbx
                    00 00
  00000000650451E4: FF 50 78           call        qword ptr [rax+78h]
  00000000650451E7: 48 8B 8F B8 00 00  mov         rcx,qword ptr [rdi+00000000000000B8h]
                    00
  00000000650451EE: 8B D0              mov         edx,eax
  00000000650451F0: 89 84 24 E8 01 00  mov         dword ptr [rsp+00000000000001E8h],eax
                    00
  00000000650451F7: 48 85 C9           test        rcx,rcx
  00000000650451FA: 74 21              je          000000006504521D
  00000000650451FC: FF 15 66 16 65 00  call        qword ptr [0000000065696868h]
  0000000065045202: 48 3B C6           cmp         rax,rsi
  0000000065045205: 74 0F              je          0000000065045216
  0000000065045207: 48 8B 8F B8 00 00  mov         rcx,qword ptr [rdi+00000000000000B8h]
                    00
  000000006504520E: 48 8B D6           mov         rdx,rsi
  0000000065045211: E8 4A B8 02 00     call        0000000065070A60
  0000000065045216: 8B 94 24 E8 01 00  mov         edx,dword ptr [rsp+00000000000001E8h]
                    00
  000000006504521D: 48 85 F6           test        rsi,rsi
  0000000065045220: 74 17              je          0000000065045239
  0000000065045222: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065045226: C6 84 24 E0 01 00  mov         byte ptr [rsp+00000000000001E0h],1
                    00 01
  000000006504522E: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065045231: C1 E9 0F           shr         ecx,0Fh
  0000000065045234: F6 C1 01           test        cl,1
  0000000065045237: 75 08              jne         0000000065045241
  0000000065045239: C6 84 24 E0 01 00  mov         byte ptr [rsp+00000000000001E0h],0
                    00 00
  0000000065045241: 33 ED              xor         ebp,ebp
  0000000065045243: 85 D2              test        edx,edx
  0000000065045245: 0F 8E E1 02 00 00  jle         000000006504552C
  000000006504524B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  0000000065045250: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065045253: 8B D5              mov         edx,ebp
  0000000065045255: 48 8B CB           mov         rcx,rbx
  0000000065045258: FF 50 60           call        qword ptr [rax+60h]
  000000006504525B: 48 8B C8           mov         rcx,rax
  000000006504525E: 48 8B F8           mov         rdi,rax
  0000000065045261: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000065045264: FF 52 60           call        qword ptr [rdx+60h]
  0000000065045267: 48 8B D8           mov         rbx,rax
  000000006504526A: 48 85 C0           test        rax,rax
  000000006504526D: 0F 84 88 02 00 00  je          00000000650454FB
  0000000065045273: 48 8B C8           mov         rcx,rax
  0000000065045276: FF 15 EC 15 65 00  call        qword ptr [0000000065696868h]
  000000006504527C: 80 BC 24 E0 01 00  cmp         byte ptr [rsp+00000000000001E0h],0
                    00 00
  0000000065045284: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  000000006504528C: 74 28              je          00000000650452B6
  000000006504528E: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  0000000065045292: F6 41 0A 01        test        byte ptr [rcx+0Ah],1
  0000000065045296: 74 19              je          00000000650452B1
  0000000065045298: BA 45 00 00 00     mov         edx,45h
  000000006504529D: 48 8B CB           mov         rcx,rbx
  00000000650452A0: E8 6B 8D 01 00     call        000000006505E010
  00000000650452A5: 84 C0              test        al,al
  00000000650452A7: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000000650452AF: 75 05              jne         00000000650452B6
  00000000650452B1: 40 B7 01           mov         dil,1
  00000000650452B4: EB 03              jmp         00000000650452B9
  00000000650452B6: 40 32 FF           xor         dil,dil
  00000000650452B9: 48 3B C6           cmp         rax,rsi
  00000000650452BC: 74 0B              je          00000000650452C9
  00000000650452BE: 48 8B D6           mov         rdx,rsi
  00000000650452C1: 48 8B CB           mov         rcx,rbx
  00000000650452C4: E8 97 B7 02 00     call        0000000065070A60
  00000000650452C9: 40 84 FF           test        dil,dil
  00000000650452CC: 0F 84 43 02 00 00  je          0000000065045515
  00000000650452D2: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000000650452DA: 45 33 C0           xor         r8d,r8d
  00000000650452DD: 33 D2              xor         edx,edx
  00000000650452DF: FF 15 0B 1F 65 00  call        qword ptr [00000000656971F0h]
  00000000650452E5: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  00000000650452ED: 45 33 C0           xor         r8d,r8d
  00000000650452F0: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650452F3: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650452F7: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650452FA: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650452FE: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000065045306: 33 D2              xor         edx,edx
  0000000065045308: FF 15 E2 1E 65 00  call        qword ptr [00000000656971F0h]
  000000006504530E: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000065045316: 45 33 C0           xor         r8d,r8d
  0000000065045319: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006504531C: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045320: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045323: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045327: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  000000006504532F: 33 D2              xor         edx,edx
  0000000065045331: FF 15 B9 1E 65 00  call        qword ptr [00000000656971F0h]
  0000000065045337: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  000000006504533F: 45 33 C0           xor         r8d,r8d
  0000000065045342: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045345: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045349: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504534C: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045350: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000065045358: 33 D2              xor         edx,edx
  000000006504535A: FF 15 90 1E 65 00  call        qword ptr [00000000656971F0h]
  0000000065045360: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  0000000065045368: 45 33 C0           xor         r8d,r8d
  000000006504536B: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006504536E: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045372: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045375: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045379: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000065045381: 33 D2              xor         edx,edx
  0000000065045383: FF 15 67 1E 65 00  call        qword ptr [00000000656971F0h]
  0000000065045389: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  0000000065045391: 45 33 C0           xor         r8d,r8d
  0000000065045394: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045397: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006504539B: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504539E: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650453A2: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000000650453AA: 33 D2              xor         edx,edx
  00000000650453AC: FF 15 3E 1E 65 00  call        qword ptr [00000000656971F0h]
  00000000650453B2: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000650453BA: 45 33 C0           xor         r8d,r8d
  00000000650453BD: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650453C0: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650453C4: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650453C7: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650453CB: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000000650453D3: 33 D2              xor         edx,edx
  00000000650453D5: FF 15 15 1E 65 00  call        qword ptr [00000000656971F0h]
  00000000650453DB: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000650453E3: 45 33 C0           xor         r8d,r8d
  00000000650453E6: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650453E9: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650453ED: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650453F0: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650453F4: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  00000000650453FC: 33 D2              xor         edx,edx
  00000000650453FE: FF 15 EC 1D 65 00  call        qword ptr [00000000656971F0h]
  0000000065045404: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  000000006504540C: 45 33 C0           xor         r8d,r8d
  000000006504540F: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045412: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045416: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045419: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006504541D: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  0000000065045425: 33 D2              xor         edx,edx
  0000000065045427: FF 15 C3 1D 65 00  call        qword ptr [00000000656971F0h]
  000000006504542D: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000065045435: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045438: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006504543C: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504543F: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045443: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  000000006504544B: 45 33 C0           xor         r8d,r8d
  000000006504544E: 33 D2              xor         edx,edx
  0000000065045450: FF 15 9A 1D 65 00  call        qword ptr [00000000656971F0h]
  0000000065045456: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006504545B: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  0000000065045460: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045463: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045467: 41 B8 02 00 00 00  mov         r8d,2
  000000006504546D: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045470: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045474: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  000000006504547C: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000065045481: 48 8D 84 24 90 00  lea         rax,[rsp+0000000000000090h]
                    00 00
  0000000065045489: 48 8D 15 48 01 66  lea         rdx,[00000000656A55D8h]
                    00
  0000000065045490: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000065045495: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  000000006504549D: 48 8B CB           mov         rcx,rbx
  00000000650454A0: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000650454A5: 48 8D 84 24 B0 00  lea         rax,[rsp+00000000000000B0h]
                    00 00
  00000000650454AD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650454B2: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000000650454BA: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000650454BF: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  00000000650454C7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000650454CC: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  00000000650454D4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000650454D9: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  00000000650454E1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000000650454E6: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  00000000650454EE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000650454F3: FF 15 EF 1C 65 00  call        qword ptr [00000000656971E8h]
  00000000650454F9: EB 1A              jmp         0000000065045515
  00000000650454FB: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000650454FE: 48 8B CF           mov         rcx,rdi
  0000000065045501: FF 50 68           call        qword ptr [rax+68h]
  0000000065045504: 48 85 C0           test        rax,rax
  0000000065045507: 74 0C              je          0000000065045515
  0000000065045509: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006504550D: 48 8B D6           mov         rdx,rsi
  0000000065045510: E8 AB FC FF FF     call        00000000650451C0
  0000000065045515: 48 8B 9C 24 F8 01  mov         rbx,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000006504551D: FF C5              inc         ebp
  000000006504551F: 3B AC 24 E8 01 00  cmp         ebp,dword ptr [rsp+00000000000001E8h]
                    00
  0000000065045526: 0F 8C 24 FD FF FF  jl          0000000065045250
  000000006504552C: 48 81 C4 B8 01 00  add         rsp,1B8h
                    00
  0000000065045533: 5F                 pop         rdi
  0000000065045534: 5E                 pop         rsi
  0000000065045535: 5D                 pop         rbp
  0000000065045536: 5B                 pop         rbx
  0000000065045537: C3                 ret

  Summary

      695000 .text
