Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403675F0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001403675F5: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001403675F9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001403675FE: 56                 push        rsi
  00000001403675FF: 57                 push        rdi
  0000000140367600: 48 81 EC 48 02 00  sub         rsp,248h
                    00
  0000000140367607: 48 83 BC 24 60 02  cmp         qword ptr [rsp+0000000000000260h],0
                    00 00 00
  0000000140367610: 75 05              jne         0000000140367617
  0000000140367612: E9 1B 03 00 00     jmp         0000000140367932
  0000000140367617: 83 BC 24 70 02 00  cmp         dword ptr [rsp+0000000000000270h],3
                    00 03
  000000014036761F: 75 05              jne         0000000140367626
  0000000140367621: E9 0C 03 00 00     jmp         0000000140367932
  0000000140367626: B2 01              mov         dl,1
  0000000140367628: 48 8B 8C 24 60 02  mov         rcx,qword ptr [rsp+0000000000000260h]
                    00 00
  0000000140367630: E8 0B 3B 91 00     call        0000000140C7B140
  0000000140367635: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014036763A: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140367640: 0F 84 EC 02 00 00  je          0000000140367932
  0000000140367646: 33 D2              xor         edx,edx
  0000000140367648: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014036764D: E8 0E 4B 50 00     call        000000014086C160
  0000000140367652: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140367657: C6 84 24 F0 00 00  mov         byte ptr [rsp+00000000000000F0h],1
                    00 01
  000000014036765F: 4C 8D 84 24 F0 00  lea         r8,[rsp+00000000000000F0h]
                    00 00
  0000000140367667: 48 8D 15 62 EB FB  lea         rdx,[00000001423261D0h]
                    01
  000000014036766E: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140367676: E8 05 DA CB FF     call        0000000140025080
  000000014036767B: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  0000000140367683: 48 8B F0           mov         rsi,rax
  0000000140367686: B9 10 00 00 00     mov         ecx,10h
  000000014036768B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014036768D: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  0000000140367695: 48 8D B4 24 30 02  lea         rsi,[rsp+0000000000000230h]
                    00 00
  000000014036769D: B9 10 00 00 00     mov         ecx,10h
  00000001403676A2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403676A4: 4C 8D 84 24 68 02  lea         r8,[rsp+0000000000000268h]
                    00 00
  00000001403676AC: 48 8D 15 25 EB FB  lea         rdx,[00000001423261D8h]
                    01
  00000001403676B3: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001403676BB: E8 C0 D9 CB FF     call        0000000140025080
  00000001403676C0: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  00000001403676C8: 48 8B F0           mov         rsi,rax
  00000001403676CB: B9 10 00 00 00     mov         ecx,10h
  00000001403676D0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403676D2: 48 8D BC 24 F8 00  lea         rdi,[rsp+00000000000000F8h]
                    00 00
  00000001403676DA: 48 8D B4 24 20 02  lea         rsi,[rsp+0000000000000220h]
                    00 00
  00000001403676E2: B9 10 00 00 00     mov         ecx,10h
  00000001403676E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403676E9: 4C 8D 84 24 60 02  lea         r8,[rsp+0000000000000260h]
                    00 00
  00000001403676F1: 48 8D 15 E8 EA FB  lea         rdx,[00000001423261E0h]
                    01
  00000001403676F8: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140367700: E8 7B D9 CB FF     call        0000000140025080
  0000000140367705: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  000000014036770D: 48 8B F0           mov         rsi,rax
  0000000140367710: B9 10 00 00 00     mov         ecx,10h
  0000000140367715: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367717: 48 8D BC 24 18 01  lea         rdi,[rsp+0000000000000118h]
                    00 00
  000000014036771F: 48 8D B4 24 10 02  lea         rsi,[rsp+0000000000000210h]
                    00 00
  0000000140367727: B9 10 00 00 00     mov         ecx,10h
  000000014036772C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014036772E: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140367733: 48 8D 15 B6 EA FB  lea         rdx,[00000001423261F0h]
                    01
  000000014036773A: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140367742: E8 39 D9 CB FF     call        0000000140025080
  0000000140367747: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  000000014036774F: 48 8B F0           mov         rsi,rax
  0000000140367752: B9 10 00 00 00     mov         ecx,10h
  0000000140367757: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367759: 48 8D BC 24 38 01  lea         rdi,[rsp+0000000000000138h]
                    00 00
  0000000140367761: 48 8D B4 24 00 02  lea         rsi,[rsp+0000000000000200h]
                    00 00
  0000000140367769: B9 10 00 00 00     mov         ecx,10h
  000000014036776E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367770: 45 33 C0           xor         r8d,r8d
  0000000140367773: 33 D2              xor         edx,edx
  0000000140367775: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014036777A: FF 15 E8 C9 A9 01  call        qword ptr [0000000141E04168h]
  0000000140367780: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140367788: 48 8B F0           mov         rsi,rax
  000000014036778B: B9 10 00 00 00     mov         ecx,10h
  0000000140367790: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367792: 45 33 C0           xor         r8d,r8d
  0000000140367795: 33 D2              xor         edx,edx
  0000000140367797: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014036779F: FF 15 C3 C9 A9 01  call        qword ptr [0000000141E04168h]
  00000001403677A5: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  00000001403677AD: 48 8B F0           mov         rsi,rax
  00000001403677B0: B9 10 00 00 00     mov         ecx,10h
  00000001403677B5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403677B7: 45 33 C0           xor         r8d,r8d
  00000001403677BA: 33 D2              xor         edx,edx
  00000001403677BC: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001403677C4: FF 15 9E C9 A9 01  call        qword ptr [0000000141E04168h]
  00000001403677CA: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  00000001403677D2: 48 8B F0           mov         rsi,rax
  00000001403677D5: B9 10 00 00 00     mov         ecx,10h
  00000001403677DA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403677DC: 45 33 C0           xor         r8d,r8d
  00000001403677DF: 33 D2              xor         edx,edx
  00000001403677E1: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001403677E9: FF 15 79 C9 A9 01  call        qword ptr [0000000141E04168h]
  00000001403677EF: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  00000001403677F7: 48 8B F0           mov         rsi,rax
  00000001403677FA: B9 10 00 00 00     mov         ecx,10h
  00000001403677FF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367801: 45 33 C0           xor         r8d,r8d
  0000000140367804: 33 D2              xor         edx,edx
  0000000140367806: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014036780E: FF 15 54 C9 A9 01  call        qword ptr [0000000141E04168h]
  0000000140367814: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  000000014036781C: 48 8B F0           mov         rsi,rax
  000000014036781F: B9 10 00 00 00     mov         ecx,10h
  0000000140367824: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367826: 45 33 C0           xor         r8d,r8d
  0000000140367829: 33 D2              xor         edx,edx
  000000014036782B: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140367833: FF 15 2F C9 A9 01  call        qword ptr [0000000141E04168h]
  0000000140367839: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140367841: 48 8B F0           mov         rsi,rax
  0000000140367844: B9 10 00 00 00     mov         ecx,10h
  0000000140367849: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014036784B: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140367853: 48 8D B4 24 D0 00  lea         rsi,[rsp+00000000000000D0h]
                    00 00
  000000014036785B: B9 10 00 00 00     mov         ecx,10h
  0000000140367860: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367862: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  000000014036786A: 48 8D B4 24 F8 00  lea         rsi,[rsp+00000000000000F8h]
                    00 00
  0000000140367872: B9 10 00 00 00     mov         ecx,10h
  0000000140367877: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367879: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140367881: 48 8D B4 24 18 01  lea         rsi,[rsp+0000000000000118h]
                    00 00
  0000000140367889: B9 10 00 00 00     mov         ecx,10h
  000000014036788E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140367890: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140367898: 48 8D B4 24 38 01  lea         rsi,[rsp+0000000000000138h]
                    00 00
  00000001403678A0: B9 10 00 00 00     mov         ecx,10h
  00000001403678A5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001403678A7: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  00000001403678AF: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001403678B4: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  00000001403678BC: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001403678C1: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000001403678C9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001403678CE: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  00000001403678D6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001403678DB: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  00000001403678E3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001403678E8: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  00000001403678F0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001403678F5: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  00000001403678FD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140367902: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  000000014036790A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014036790F: 4C 8D 8C 24 70 01  lea         r9,[rsp+0000000000000170h]
                    00 00
  0000000140367917: 4C 8D 84 24 60 01  lea         r8,[rsp+0000000000000160h]
                    00 00
  000000014036791F: 48 8D 15 DA E8 FB  lea         rdx,[0000000142326200h]
                    01
  0000000140367926: 48 8D 0D D3 ED A0  lea         rcx,[0000000143D76700h]
                    03
  000000014036792D: E8 8E 7A F6 FF     call        00000001402CF3C0
  0000000140367932: 48 81 C4 48 02 00  add         rsp,248h
                    00
  0000000140367939: 5F                 pop         rdi
  000000014036793A: 5E                 pop         rsi
  000000014036793B: C3                 ret

  Summary

     1E02000 .text
