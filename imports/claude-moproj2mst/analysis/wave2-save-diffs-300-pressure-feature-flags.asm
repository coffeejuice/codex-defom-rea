Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EB000: 48 8B C4           mov         rax,rsp
  00000001412EB003: 57                 push        rdi
  00000001412EB004: 41 54              push        r12
  00000001412EB006: 41 55              push        r13
  00000001412EB008: 48 81 EC F0 04 00  sub         rsp,4F0h
                    00
  00000001412EB00F: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412EB018: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000001412EB01C: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  00000001412EB020: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  00000001412EB024: 48 8B 05 05 30 87  mov         rax,qword ptr [0000000143B5E030h]
                    02
  00000001412EB02B: 48 33 C4           xor         rax,rsp
  00000001412EB02E: 48 89 84 24 E0 04  mov         qword ptr [rsp+00000000000004E0h],rax
                    00 00
  00000001412EB036: 4C 8B E1           mov         r12,rcx
  00000001412EB039: E8 D2 B8 FF FF     call        00000001412E6910
  00000001412EB03E: 85 C0              test        eax,eax
  00000001412EB040: 0F 84 3A 17 00 00  je          00000001412EC780
  00000001412EB046: 48 8B B9 50 14 00  mov         rdi,qword ptr [rcx+0000000000001450h]
                    00
  00000001412EB04D: 48 63 47 38        movsxd      rax,dword ptr [rdi+38h]
  00000001412EB051: 48 8D 74 87 40     lea         rsi,[rdi+rax*4+40h]
  00000001412EB056: 41 83 F8 03        cmp         r8d,3
  00000001412EB05A: 0F 85 E9 0D 00 00  jne         00000001412EBE49
  00000001412EB060: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB065: E8 26 D1 01 00     call        0000000141308190
  00000001412EB06A: 90                 nop
  00000001412EB06B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB070: E8 8B C2 01 00     call        0000000141307300
  00000001412EB075: 48 8B D0           mov         rdx,rax
  00000001412EB078: 4C 8D 05 39 A3 AC  lea         r8,[0000000142DB53B8h]
                    01
  00000001412EB07F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB084: E8 B7 C6 01 00     call        0000000141307740
  00000001412EB089: 48 85 C0           test        rax,rax
  00000001412EB08C: 75 24              jne         00000001412EB0B2
  00000001412EB08E: 33 DB              xor         ebx,ebx
  00000001412EB090: 8B EB              mov         ebp,ebx
  00000001412EB092: 48 8D 15 1F A3 AC  lea         rdx,[0000000142DB53B8h]
                    01
  00000001412EB099: 48 8D 0D 38 AF AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB0A0: E8 EB 76 18 00     call        0000000141472790
  00000001412EB0A5: 90                 nop
  00000001412EB0A6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB0AB: E8 10 CB 01 00     call        0000000141307BC0
  00000001412EB0B0: EB 3E              jmp         00000001412EB0F0
  00000001412EB0B2: 48 8B C8           mov         rcx,rax
  00000001412EB0B5: E8 F2 86 18 00     call        00000001414737AC
  00000001412EB0BA: 48 63 D8           movsxd      rbx,eax
  00000001412EB0BD: 85 C0              test        eax,eax
  00000001412EB0BF: 78 05              js          00000001412EB0C6
  00000001412EB0C1: 3B 5F 3C           cmp         ebx,dword ptr [rdi+3Ch]
  00000001412EB0C4: 7C 12              jl          00000001412EB0D8
  00000001412EB0C6: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB0CA: 8B D3              mov         edx,ebx
  00000001412EB0CC: 48 8D 0D BD AE AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB0D3: E8 B8 76 18 00     call        0000000141472790
  00000001412EB0D8: 48 8B C3           mov         rax,rbx
  00000001412EB0DB: 33 DB              xor         ebx,ebx
  00000001412EB0DD: 8B EB              mov         ebp,ebx
  00000001412EB0DF: 39 1C 86           cmp         dword ptr [rsi+rax*4],ebx
  00000001412EB0E2: 40 0F 95 C5        setne       bpl
  00000001412EB0E6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB0EB: E8 D0 CA 01 00     call        0000000141307BC0
  00000001412EB0F0: 45 33 C9           xor         r9d,r9d
  00000001412EB0F3: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB0F8: 48 8D 15 B9 A2 AC  lea         rdx,[0000000142DB53B8h]
                    01
  00000001412EB0FF: 49 8B CC           mov         rcx,r12
  00000001412EB102: E8 B9 C5 FF FF     call        00000001412E76C0
  00000001412EB107: 85 ED              test        ebp,ebp
  00000001412EB109: 0F 95 C1           setne       cl
  00000001412EB10C: 88 08              mov         byte ptr [rax],cl
  00000001412EB10E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB113: E8 78 D0 01 00     call        0000000141308190
  00000001412EB118: 90                 nop
  00000001412EB119: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB11E: E8 DD C1 01 00     call        0000000141307300
  00000001412EB123: 48 8B D0           mov         rdx,rax
  00000001412EB126: 4C 8D 05 7B A2 AC  lea         r8,[0000000142DB53A8h]
                    01
  00000001412EB12D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB132: E8 09 C6 01 00     call        0000000141307740
  00000001412EB137: 48 85 C0           test        rax,rax
  00000001412EB13A: 75 22              jne         00000001412EB15E
  00000001412EB13C: 8B EB              mov         ebp,ebx
  00000001412EB13E: 48 8D 15 63 A2 AC  lea         rdx,[0000000142DB53A8h]
                    01
  00000001412EB145: 48 8D 0D 8C AE AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB14C: E8 3F 76 18 00     call        0000000141472790
  00000001412EB151: 90                 nop
  00000001412EB152: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB157: E8 64 CA 01 00     call        0000000141307BC0
  00000001412EB15C: EB 3D              jmp         00000001412EB19B
  00000001412EB15E: 48 8B C8           mov         rcx,rax
  00000001412EB161: E8 46 86 18 00     call        00000001414737AC
  00000001412EB166: 48 63 E8           movsxd      rbp,eax
  00000001412EB169: 85 C0              test        eax,eax
  00000001412EB16B: 78 05              js          00000001412EB172
  00000001412EB16D: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB170: 7C 12              jl          00000001412EB184
  00000001412EB172: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB176: 8B D5              mov         edx,ebp
  00000001412EB178: 48 8D 0D 11 AE AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB17F: E8 0C 76 18 00     call        0000000141472790
  00000001412EB184: 48 8B C5           mov         rax,rbp
  00000001412EB187: 8B EB              mov         ebp,ebx
  00000001412EB189: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB18D: 40 0F 95 C5        setne       bpl
  00000001412EB191: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB196: E8 25 CA 01 00     call        0000000141307BC0
  00000001412EB19B: 45 33 C9           xor         r9d,r9d
  00000001412EB19E: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB1A3: 48 8D 15 FE A1 AC  lea         rdx,[0000000142DB53A8h]
                    01
  00000001412EB1AA: 49 8B CC           mov         rcx,r12
  00000001412EB1AD: E8 0E C5 FF FF     call        00000001412E76C0
  00000001412EB1B2: 85 ED              test        ebp,ebp
  00000001412EB1B4: 0F 95 C1           setne       cl
  00000001412EB1B7: 88 08              mov         byte ptr [rax],cl
  00000001412EB1B9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB1BE: E8 CD CF 01 00     call        0000000141308190
  00000001412EB1C3: 90                 nop
  00000001412EB1C4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB1C9: E8 32 C1 01 00     call        0000000141307300
  00000001412EB1CE: 48 8B D0           mov         rdx,rax
  00000001412EB1D1: 4C 8D 05 A8 A1 AC  lea         r8,[0000000142DB5380h]
                    01
  00000001412EB1D8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB1DD: E8 5E C5 01 00     call        0000000141307740
  00000001412EB1E2: 48 85 C0           test        rax,rax
  00000001412EB1E5: 75 22              jne         00000001412EB209
  00000001412EB1E7: 8B EB              mov         ebp,ebx
  00000001412EB1E9: 48 8D 15 90 A1 AC  lea         rdx,[0000000142DB5380h]
                    01
  00000001412EB1F0: 48 8D 0D E1 AD AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB1F7: E8 94 75 18 00     call        0000000141472790
  00000001412EB1FC: 90                 nop
  00000001412EB1FD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB202: E8 B9 C9 01 00     call        0000000141307BC0
  00000001412EB207: EB 3D              jmp         00000001412EB246
  00000001412EB209: 48 8B C8           mov         rcx,rax
  00000001412EB20C: E8 9B 85 18 00     call        00000001414737AC
  00000001412EB211: 48 63 E8           movsxd      rbp,eax
  00000001412EB214: 85 C0              test        eax,eax
  00000001412EB216: 78 05              js          00000001412EB21D
  00000001412EB218: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB21B: 7C 12              jl          00000001412EB22F
  00000001412EB21D: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB221: 8B D5              mov         edx,ebp
  00000001412EB223: 48 8D 0D 66 AD AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB22A: E8 61 75 18 00     call        0000000141472790
  00000001412EB22F: 48 8B C5           mov         rax,rbp
  00000001412EB232: 8B EB              mov         ebp,ebx
  00000001412EB234: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB238: 40 0F 95 C5        setne       bpl
  00000001412EB23C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB241: E8 7A C9 01 00     call        0000000141307BC0
  00000001412EB246: 45 33 C9           xor         r9d,r9d
  00000001412EB249: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB24E: 48 8D 15 2B A1 AC  lea         rdx,[0000000142DB5380h]
                    01
  00000001412EB255: 49 8B CC           mov         rcx,r12
  00000001412EB258: E8 63 C4 FF FF     call        00000001412E76C0
  00000001412EB25D: 85 ED              test        ebp,ebp
  00000001412EB25F: 0F 95 C1           setne       cl
  00000001412EB262: 88 08              mov         byte ptr [rax],cl
  00000001412EB264: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB269: E8 22 CF 01 00     call        0000000141308190
  00000001412EB26E: 90                 nop
  00000001412EB26F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB274: E8 87 C0 01 00     call        0000000141307300
  00000001412EB279: 48 8B D0           mov         rdx,rax
  00000001412EB27C: 4C 8D 05 E5 A0 AC  lea         r8,[0000000142DB5368h]
                    01
  00000001412EB283: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB288: E8 B3 C4 01 00     call        0000000141307740
  00000001412EB28D: 48 85 C0           test        rax,rax
  00000001412EB290: 75 22              jne         00000001412EB2B4
  00000001412EB292: 8B EB              mov         ebp,ebx
  00000001412EB294: 48 8D 15 CD A0 AC  lea         rdx,[0000000142DB5368h]
                    01
  00000001412EB29B: 48 8D 0D 36 AD AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB2A2: E8 E9 74 18 00     call        0000000141472790
  00000001412EB2A7: 90                 nop
  00000001412EB2A8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB2AD: E8 0E C9 01 00     call        0000000141307BC0
  00000001412EB2B2: EB 3D              jmp         00000001412EB2F1
  00000001412EB2B4: 48 8B C8           mov         rcx,rax
  00000001412EB2B7: E8 F0 84 18 00     call        00000001414737AC
  00000001412EB2BC: 48 63 E8           movsxd      rbp,eax
  00000001412EB2BF: 85 C0              test        eax,eax
  00000001412EB2C1: 78 05              js          00000001412EB2C8
  00000001412EB2C3: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB2C6: 7C 12              jl          00000001412EB2DA
  00000001412EB2C8: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB2CC: 8B D5              mov         edx,ebp
  00000001412EB2CE: 48 8D 0D BB AC AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB2D5: E8 B6 74 18 00     call        0000000141472790
  00000001412EB2DA: 48 8B C5           mov         rax,rbp
  00000001412EB2DD: 8B EB              mov         ebp,ebx
  00000001412EB2DF: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB2E3: 40 0F 95 C5        setne       bpl
  00000001412EB2E7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB2EC: E8 CF C8 01 00     call        0000000141307BC0
  00000001412EB2F1: 45 33 C9           xor         r9d,r9d
  00000001412EB2F4: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB2F9: 48 8D 15 68 A0 AC  lea         rdx,[0000000142DB5368h]
                    01
  00000001412EB300: 49 8B CC           mov         rcx,r12
  00000001412EB303: E8 B8 C3 FF FF     call        00000001412E76C0
  00000001412EB308: 85 ED              test        ebp,ebp
  00000001412EB30A: 0F 95 C1           setne       cl
  00000001412EB30D: 88 08              mov         byte ptr [rax],cl
  00000001412EB30F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB314: E8 77 CE 01 00     call        0000000141308190
  00000001412EB319: 90                 nop
  00000001412EB31A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB31F: E8 DC BF 01 00     call        0000000141307300
  00000001412EB324: 48 8B D0           mov         rdx,rax
  00000001412EB327: 4C 8D 05 22 A0 AC  lea         r8,[0000000142DB5350h]
                    01
  00000001412EB32E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB333: E8 08 C4 01 00     call        0000000141307740
  00000001412EB338: 48 85 C0           test        rax,rax
  00000001412EB33B: 75 22              jne         00000001412EB35F
  00000001412EB33D: 8B EB              mov         ebp,ebx
  00000001412EB33F: 48 8D 15 0A A0 AC  lea         rdx,[0000000142DB5350h]
                    01
  00000001412EB346: 48 8D 0D 8B AC AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB34D: E8 3E 74 18 00     call        0000000141472790
  00000001412EB352: 90                 nop
  00000001412EB353: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB358: E8 63 C8 01 00     call        0000000141307BC0
  00000001412EB35D: EB 3D              jmp         00000001412EB39C
  00000001412EB35F: 48 8B C8           mov         rcx,rax
  00000001412EB362: E8 45 84 18 00     call        00000001414737AC
  00000001412EB367: 48 63 E8           movsxd      rbp,eax
  00000001412EB36A: 85 C0              test        eax,eax
  00000001412EB36C: 78 05              js          00000001412EB373
  00000001412EB36E: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB371: 7C 12              jl          00000001412EB385
  00000001412EB373: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB377: 8B D5              mov         edx,ebp
  00000001412EB379: 48 8D 0D 10 AC AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB380: E8 0B 74 18 00     call        0000000141472790
  00000001412EB385: 48 8B C5           mov         rax,rbp
  00000001412EB388: 8B EB              mov         ebp,ebx
  00000001412EB38A: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB38E: 40 0F 95 C5        setne       bpl
  00000001412EB392: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB397: E8 24 C8 01 00     call        0000000141307BC0
  00000001412EB39C: 45 33 C9           xor         r9d,r9d
  00000001412EB39F: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB3A4: 48 8D 15 A5 9F AC  lea         rdx,[0000000142DB5350h]
                    01
  00000001412EB3AB: 49 8B CC           mov         rcx,r12
  00000001412EB3AE: E8 0D C3 FF FF     call        00000001412E76C0
  00000001412EB3B3: 85 ED              test        ebp,ebp
  00000001412EB3B5: 0F 95 C1           setne       cl
  00000001412EB3B8: 88 08              mov         byte ptr [rax],cl
  00000001412EB3BA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB3BF: E8 CC CD 01 00     call        0000000141308190
  00000001412EB3C4: 90                 nop
  00000001412EB3C5: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB3CA: E8 31 BF 01 00     call        0000000141307300
  00000001412EB3CF: 48 8B D0           mov         rdx,rax
  00000001412EB3D2: 4C 8D 05 5F 9F AC  lea         r8,[0000000142DB5338h]
                    01
  00000001412EB3D9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB3DE: E8 5D C3 01 00     call        0000000141307740
  00000001412EB3E3: 48 85 C0           test        rax,rax
  00000001412EB3E6: 75 22              jne         00000001412EB40A
  00000001412EB3E8: 8B EB              mov         ebp,ebx
  00000001412EB3EA: 48 8D 15 47 9F AC  lea         rdx,[0000000142DB5338h]
                    01
  00000001412EB3F1: 48 8D 0D E0 AB AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB3F8: E8 93 73 18 00     call        0000000141472790
  00000001412EB3FD: 90                 nop
  00000001412EB3FE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB403: E8 B8 C7 01 00     call        0000000141307BC0
  00000001412EB408: EB 3D              jmp         00000001412EB447
  00000001412EB40A: 48 8B C8           mov         rcx,rax
  00000001412EB40D: E8 9A 83 18 00     call        00000001414737AC
  00000001412EB412: 48 63 E8           movsxd      rbp,eax
  00000001412EB415: 85 C0              test        eax,eax
  00000001412EB417: 78 05              js          00000001412EB41E
  00000001412EB419: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB41C: 7C 12              jl          00000001412EB430
  00000001412EB41E: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB422: 8B D5              mov         edx,ebp
  00000001412EB424: 48 8D 0D 65 AB AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB42B: E8 60 73 18 00     call        0000000141472790
  00000001412EB430: 48 8B C5           mov         rax,rbp
  00000001412EB433: 8B EB              mov         ebp,ebx
  00000001412EB435: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB439: 40 0F 95 C5        setne       bpl
  00000001412EB43D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB442: E8 79 C7 01 00     call        0000000141307BC0
  00000001412EB447: 45 33 C9           xor         r9d,r9d
  00000001412EB44A: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB44F: 48 8D 15 E2 9E AC  lea         rdx,[0000000142DB5338h]
                    01
  00000001412EB456: 49 8B CC           mov         rcx,r12
  00000001412EB459: E8 62 C2 FF FF     call        00000001412E76C0
  00000001412EB45E: 85 ED              test        ebp,ebp
  00000001412EB460: 0F 95 C1           setne       cl
  00000001412EB463: 88 08              mov         byte ptr [rax],cl
  00000001412EB465: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB46A: E8 21 CD 01 00     call        0000000141308190
  00000001412EB46F: 90                 nop
  00000001412EB470: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB475: E8 86 BE 01 00     call        0000000141307300
  00000001412EB47A: 48 8B D0           mov         rdx,rax
  00000001412EB47D: 4C 8D 05 9C 9E AC  lea         r8,[0000000142DB5320h]
                    01
  00000001412EB484: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB489: E8 B2 C2 01 00     call        0000000141307740
  00000001412EB48E: 48 85 C0           test        rax,rax
  00000001412EB491: 75 22              jne         00000001412EB4B5
  00000001412EB493: 8B EB              mov         ebp,ebx
  00000001412EB495: 48 8D 15 84 9E AC  lea         rdx,[0000000142DB5320h]
                    01
  00000001412EB49C: 48 8D 0D 35 AB AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB4A3: E8 E8 72 18 00     call        0000000141472790
  00000001412EB4A8: 90                 nop
  00000001412EB4A9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB4AE: E8 0D C7 01 00     call        0000000141307BC0
  00000001412EB4B3: EB 3D              jmp         00000001412EB4F2
  00000001412EB4B5: 48 8B C8           mov         rcx,rax
  00000001412EB4B8: E8 EF 82 18 00     call        00000001414737AC
  00000001412EB4BD: 48 63 E8           movsxd      rbp,eax
  00000001412EB4C0: 85 C0              test        eax,eax
  00000001412EB4C2: 78 05              js          00000001412EB4C9
  00000001412EB4C4: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB4C7: 7C 12              jl          00000001412EB4DB
  00000001412EB4C9: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB4CD: 8B D5              mov         edx,ebp
  00000001412EB4CF: 48 8D 0D BA AA AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB4D6: E8 B5 72 18 00     call        0000000141472790
  00000001412EB4DB: 48 8B C5           mov         rax,rbp
  00000001412EB4DE: 8B EB              mov         ebp,ebx
  00000001412EB4E0: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB4E4: 40 0F 95 C5        setne       bpl
  00000001412EB4E8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB4ED: E8 CE C6 01 00     call        0000000141307BC0
  00000001412EB4F2: 45 33 C9           xor         r9d,r9d
  00000001412EB4F5: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB4FA: 48 8D 15 1F 9E AC  lea         rdx,[0000000142DB5320h]
                    01
  00000001412EB501: 49 8B CC           mov         rcx,r12
  00000001412EB504: E8 B7 C1 FF FF     call        00000001412E76C0
  00000001412EB509: 85 ED              test        ebp,ebp
  00000001412EB50B: 0F 95 C1           setne       cl
  00000001412EB50E: 88 08              mov         byte ptr [rax],cl
  00000001412EB510: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB515: E8 76 CC 01 00     call        0000000141308190
  00000001412EB51A: 90                 nop
  00000001412EB51B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB520: E8 DB BD 01 00     call        0000000141307300
  00000001412EB525: 48 8B D0           mov         rdx,rax
  00000001412EB528: 4C 8D 05 E1 9D AC  lea         r8,[0000000142DB5310h]
                    01
  00000001412EB52F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB534: E8 07 C2 01 00     call        0000000141307740
  00000001412EB539: 48 85 C0           test        rax,rax
  00000001412EB53C: 75 22              jne         00000001412EB560
  00000001412EB53E: 8B EB              mov         ebp,ebx
  00000001412EB540: 48 8D 15 C9 9D AC  lea         rdx,[0000000142DB5310h]
                    01
  00000001412EB547: 48 8D 0D 8A AA AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB54E: E8 3D 72 18 00     call        0000000141472790
  00000001412EB553: 90                 nop
  00000001412EB554: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB559: E8 62 C6 01 00     call        0000000141307BC0
  00000001412EB55E: EB 3D              jmp         00000001412EB59D
  00000001412EB560: 48 8B C8           mov         rcx,rax
  00000001412EB563: E8 44 82 18 00     call        00000001414737AC
  00000001412EB568: 48 63 E8           movsxd      rbp,eax
  00000001412EB56B: 85 C0              test        eax,eax
  00000001412EB56D: 78 05              js          00000001412EB574
  00000001412EB56F: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB572: 7C 12              jl          00000001412EB586
  00000001412EB574: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB578: 8B D5              mov         edx,ebp
  00000001412EB57A: 48 8D 0D 0F AA AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB581: E8 0A 72 18 00     call        0000000141472790
  00000001412EB586: 48 8B C5           mov         rax,rbp
  00000001412EB589: 8B EB              mov         ebp,ebx
  00000001412EB58B: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB58F: 40 0F 95 C5        setne       bpl
  00000001412EB593: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB598: E8 23 C6 01 00     call        0000000141307BC0
  00000001412EB59D: 45 33 C9           xor         r9d,r9d
  00000001412EB5A0: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB5A5: 48 8D 15 64 9D AC  lea         rdx,[0000000142DB5310h]
                    01
  00000001412EB5AC: 49 8B CC           mov         rcx,r12
  00000001412EB5AF: E8 0C C1 FF FF     call        00000001412E76C0
  00000001412EB5B4: 85 ED              test        ebp,ebp
  00000001412EB5B6: 0F 95 C1           setne       cl
  00000001412EB5B9: 88 08              mov         byte ptr [rax],cl
  00000001412EB5BB: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB5C0: E8 CB CB 01 00     call        0000000141308190
  00000001412EB5C5: 90                 nop
  00000001412EB5C6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB5CB: E8 30 BD 01 00     call        0000000141307300
  00000001412EB5D0: 48 8B D0           mov         rdx,rax
  00000001412EB5D3: 4C 8D 05 26 9D AC  lea         r8,[0000000142DB5300h]
                    01
  00000001412EB5DA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB5DF: E8 5C C1 01 00     call        0000000141307740
  00000001412EB5E4: 48 85 C0           test        rax,rax
  00000001412EB5E7: 75 22              jne         00000001412EB60B
  00000001412EB5E9: 8B EB              mov         ebp,ebx
  00000001412EB5EB: 48 8D 15 0E 9D AC  lea         rdx,[0000000142DB5300h]
                    01
  00000001412EB5F2: 48 8D 0D DF A9 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB5F9: E8 92 71 18 00     call        0000000141472790
  00000001412EB5FE: 90                 nop
  00000001412EB5FF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB604: E8 B7 C5 01 00     call        0000000141307BC0
  00000001412EB609: EB 3D              jmp         00000001412EB648
  00000001412EB60B: 48 8B C8           mov         rcx,rax
  00000001412EB60E: E8 99 81 18 00     call        00000001414737AC
  00000001412EB613: 48 63 E8           movsxd      rbp,eax
  00000001412EB616: 85 C0              test        eax,eax
  00000001412EB618: 78 05              js          00000001412EB61F
  00000001412EB61A: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB61D: 7C 12              jl          00000001412EB631
  00000001412EB61F: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB623: 8B D5              mov         edx,ebp
  00000001412EB625: 48 8D 0D 64 A9 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB62C: E8 5F 71 18 00     call        0000000141472790
  00000001412EB631: 48 8B C5           mov         rax,rbp
  00000001412EB634: 8B EB              mov         ebp,ebx
  00000001412EB636: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB63A: 40 0F 95 C5        setne       bpl
  00000001412EB63E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB643: E8 78 C5 01 00     call        0000000141307BC0
  00000001412EB648: 45 33 C9           xor         r9d,r9d
  00000001412EB64B: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB650: 48 8D 15 A9 9C AC  lea         rdx,[0000000142DB5300h]
                    01
  00000001412EB657: 49 8B CC           mov         rcx,r12
  00000001412EB65A: E8 61 C0 FF FF     call        00000001412E76C0
  00000001412EB65F: 85 ED              test        ebp,ebp
  00000001412EB661: 0F 95 C1           setne       cl
  00000001412EB664: 88 08              mov         byte ptr [rax],cl
  00000001412EB666: 41 0F B6 44 24 1F  movzx       eax,byte ptr [r12+1Fh]
  00000001412EB66C: 41 88 44 24 21     mov         byte ptr [r12+21h],al
  00000001412EB671: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB676: E8 15 CB 01 00     call        0000000141308190
  00000001412EB67B: 90                 nop
  00000001412EB67C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB681: E8 7A BC 01 00     call        0000000141307300
  00000001412EB686: 48 8B D0           mov         rdx,rax
  00000001412EB689: 4C 8D 05 48 9C AC  lea         r8,[0000000142DB52D8h]
                    01
  00000001412EB690: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB695: E8 A6 C0 01 00     call        0000000141307740
  00000001412EB69A: 48 85 C0           test        rax,rax
  00000001412EB69D: 75 22              jne         00000001412EB6C1
  00000001412EB69F: 8B EB              mov         ebp,ebx
  00000001412EB6A1: 48 8D 15 30 9C AC  lea         rdx,[0000000142DB52D8h]
                    01
  00000001412EB6A8: 48 8D 0D 29 A9 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB6AF: E8 DC 70 18 00     call        0000000141472790
  00000001412EB6B4: 90                 nop
  00000001412EB6B5: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB6BA: E8 01 C5 01 00     call        0000000141307BC0
  00000001412EB6BF: EB 3D              jmp         00000001412EB6FE
  00000001412EB6C1: 48 8B C8           mov         rcx,rax
  00000001412EB6C4: E8 E3 80 18 00     call        00000001414737AC
  00000001412EB6C9: 48 63 E8           movsxd      rbp,eax
  00000001412EB6CC: 85 C0              test        eax,eax
  00000001412EB6CE: 78 05              js          00000001412EB6D5
  00000001412EB6D0: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB6D3: 7C 12              jl          00000001412EB6E7
  00000001412EB6D5: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB6D9: 8B D5              mov         edx,ebp
  00000001412EB6DB: 48 8D 0D AE A8 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB6E2: E8 A9 70 18 00     call        0000000141472790
  00000001412EB6E7: 48 8B C5           mov         rax,rbp
  00000001412EB6EA: 8B EB              mov         ebp,ebx
  00000001412EB6EC: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB6F0: 40 0F 95 C5        setne       bpl
  00000001412EB6F4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB6F9: E8 C2 C4 01 00     call        0000000141307BC0
  00000001412EB6FE: 45 33 C9           xor         r9d,r9d
  00000001412EB701: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB706: 48 8D 15 CB 9B AC  lea         rdx,[0000000142DB52D8h]
                    01
  00000001412EB70D: 49 8B CC           mov         rcx,r12
  00000001412EB710: E8 AB BF FF FF     call        00000001412E76C0
  00000001412EB715: 85 ED              test        ebp,ebp
  00000001412EB717: 0F 95 C1           setne       cl
  00000001412EB71A: 88 08              mov         byte ptr [rax],cl
  00000001412EB71C: 45 38 4C 24 23     cmp         byte ptr [r12+23h],r9b
  00000001412EB721: 0F 85 36 01 00 00  jne         00000001412EB85D
  00000001412EB727: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB72C: E8 5F CA 01 00     call        0000000141308190
  00000001412EB731: 90                 nop
  00000001412EB732: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB737: E8 C4 BB 01 00     call        0000000141307300
  00000001412EB73C: 48 8B D0           mov         rdx,rax
  00000001412EB73F: 4C 8D 05 32 A8 AC  lea         r8,[0000000142DB5F78h]
                    01
  00000001412EB746: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB74B: E8 F0 BF 01 00     call        0000000141307740
  00000001412EB750: 48 85 C0           test        rax,rax
  00000001412EB753: 75 22              jne         00000001412EB777
  00000001412EB755: 8B EB              mov         ebp,ebx
  00000001412EB757: 48 8D 15 1A A8 AC  lea         rdx,[0000000142DB5F78h]
                    01
  00000001412EB75E: 48 8D 0D 73 A8 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB765: E8 26 70 18 00     call        0000000141472790
  00000001412EB76A: 90                 nop
  00000001412EB76B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB770: E8 4B C4 01 00     call        0000000141307BC0
  00000001412EB775: EB 3D              jmp         00000001412EB7B4
  00000001412EB777: 48 8B C8           mov         rcx,rax
  00000001412EB77A: E8 2D 80 18 00     call        00000001414737AC
  00000001412EB77F: 48 63 E8           movsxd      rbp,eax
  00000001412EB782: 85 C0              test        eax,eax
  00000001412EB784: 78 05              js          00000001412EB78B
  00000001412EB786: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB789: 7C 12              jl          00000001412EB79D
  00000001412EB78B: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB78F: 8B D5              mov         edx,ebp
  00000001412EB791: 48 8D 0D F8 A7 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB798: E8 F3 6F 18 00     call        0000000141472790
  00000001412EB79D: 48 8B C5           mov         rax,rbp
  00000001412EB7A0: 8B EB              mov         ebp,ebx
  00000001412EB7A2: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB7A6: 40 0F 95 C5        setne       bpl
  00000001412EB7AA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB7AF: E8 0C C4 01 00     call        0000000141307BC0
  00000001412EB7B4: 85 ED              test        ebp,ebp
  00000001412EB7B6: 0F 95 C0           setne       al
  00000001412EB7B9: 41 88 44 24 23     mov         byte ptr [r12+23h],al
  00000001412EB7BE: 84 C0              test        al,al
  00000001412EB7C0: 0F 85 97 00 00 00  jne         00000001412EB85D
  00000001412EB7C6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB7CB: E8 C0 C9 01 00     call        0000000141308190
  00000001412EB7D0: 90                 nop
  00000001412EB7D1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB7D6: E8 25 BB 01 00     call        0000000141307300
  00000001412EB7DB: 48 8B D0           mov         rdx,rax
  00000001412EB7DE: 4C 8D 05 83 A7 AC  lea         r8,[0000000142DB5F68h]
                    01
  00000001412EB7E5: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB7EA: E8 51 BF 01 00     call        0000000141307740
  00000001412EB7EF: 48 85 C0           test        rax,rax
  00000001412EB7F2: 75 22              jne         00000001412EB816
  00000001412EB7F4: 8B EB              mov         ebp,ebx
  00000001412EB7F6: 48 8D 15 6B A7 AC  lea         rdx,[0000000142DB5F68h]
                    01
  00000001412EB7FD: 48 8D 0D D4 A7 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB804: E8 87 6F 18 00     call        0000000141472790
  00000001412EB809: 90                 nop
  00000001412EB80A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB80F: E8 AC C3 01 00     call        0000000141307BC0
  00000001412EB814: EB 3D              jmp         00000001412EB853
  00000001412EB816: 48 8B C8           mov         rcx,rax
  00000001412EB819: E8 8E 7F 18 00     call        00000001414737AC
  00000001412EB81E: 48 63 E8           movsxd      rbp,eax
  00000001412EB821: 85 C0              test        eax,eax
  00000001412EB823: 78 05              js          00000001412EB82A
  00000001412EB825: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB828: 7C 12              jl          00000001412EB83C
  00000001412EB82A: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB82E: 8B D5              mov         edx,ebp
  00000001412EB830: 48 8D 0D 59 A7 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB837: E8 54 6F 18 00     call        0000000141472790
  00000001412EB83C: 48 8B C5           mov         rax,rbp
  00000001412EB83F: 8B EB              mov         ebp,ebx
  00000001412EB841: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB845: 40 0F 95 C5        setne       bpl
  00000001412EB849: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB84E: E8 6D C3 01 00     call        0000000141307BC0
  00000001412EB853: 85 ED              test        ebp,ebp
  00000001412EB855: 0F 95 C0           setne       al
  00000001412EB858: 41 88 44 24 23     mov         byte ptr [r12+23h],al
  00000001412EB85D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB862: E8 29 C9 01 00     call        0000000141308190
  00000001412EB867: 90                 nop
  00000001412EB868: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB86D: E8 8E BA 01 00     call        0000000141307300
  00000001412EB872: 48 8B D0           mov         rdx,rax
  00000001412EB875: 4C 8D 05 4C 9A AC  lea         r8,[0000000142DB52C8h]
                    01
  00000001412EB87C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB881: E8 BA BE 01 00     call        0000000141307740
  00000001412EB886: 48 85 C0           test        rax,rax
  00000001412EB889: 75 22              jne         00000001412EB8AD
  00000001412EB88B: 8B EB              mov         ebp,ebx
  00000001412EB88D: 48 8D 15 34 9A AC  lea         rdx,[0000000142DB52C8h]
                    01
  00000001412EB894: 48 8D 0D 3D A7 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB89B: E8 F0 6E 18 00     call        0000000141472790
  00000001412EB8A0: 90                 nop
  00000001412EB8A1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB8A6: E8 15 C3 01 00     call        0000000141307BC0
  00000001412EB8AB: EB 3D              jmp         00000001412EB8EA
  00000001412EB8AD: 48 8B C8           mov         rcx,rax
  00000001412EB8B0: E8 F7 7E 18 00     call        00000001414737AC
  00000001412EB8B5: 48 63 E8           movsxd      rbp,eax
  00000001412EB8B8: 85 C0              test        eax,eax
  00000001412EB8BA: 78 05              js          00000001412EB8C1
  00000001412EB8BC: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB8BF: 7C 12              jl          00000001412EB8D3
  00000001412EB8C1: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB8C5: 8B D5              mov         edx,ebp
  00000001412EB8C7: 48 8D 0D C2 A6 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB8CE: E8 BD 6E 18 00     call        0000000141472790
  00000001412EB8D3: 48 8B C5           mov         rax,rbp
  00000001412EB8D6: 8B EB              mov         ebp,ebx
  00000001412EB8D8: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB8DC: 40 0F 95 C5        setne       bpl
  00000001412EB8E0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB8E5: E8 D6 C2 01 00     call        0000000141307BC0
  00000001412EB8EA: 45 33 C9           xor         r9d,r9d
  00000001412EB8ED: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB8F2: 48 8D 15 CF 99 AC  lea         rdx,[0000000142DB52C8h]
                    01
  00000001412EB8F9: 49 8B CC           mov         rcx,r12
  00000001412EB8FC: E8 BF BD FF FF     call        00000001412E76C0
  00000001412EB901: 85 ED              test        ebp,ebp
  00000001412EB903: 0F 95 C1           setne       cl
  00000001412EB906: 88 08              mov         byte ptr [rax],cl
  00000001412EB908: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB90D: E8 7E C8 01 00     call        0000000141308190
  00000001412EB912: 90                 nop
  00000001412EB913: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB918: E8 E3 B9 01 00     call        0000000141307300
  00000001412EB91D: 48 8B D0           mov         rdx,rax
  00000001412EB920: 4C 8D 05 89 99 AC  lea         r8,[0000000142DB52B0h]
                    01
  00000001412EB927: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB92C: E8 0F BE 01 00     call        0000000141307740
  00000001412EB931: 48 85 C0           test        rax,rax
  00000001412EB934: 75 22              jne         00000001412EB958
  00000001412EB936: 8B EB              mov         ebp,ebx
  00000001412EB938: 48 8D 15 71 99 AC  lea         rdx,[0000000142DB52B0h]
                    01
  00000001412EB93F: 48 8D 0D 92 A6 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB946: E8 45 6E 18 00     call        0000000141472790
  00000001412EB94B: 90                 nop
  00000001412EB94C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB951: E8 6A C2 01 00     call        0000000141307BC0
  00000001412EB956: EB 3D              jmp         00000001412EB995
  00000001412EB958: 48 8B C8           mov         rcx,rax
  00000001412EB95B: E8 4C 7E 18 00     call        00000001414737AC
  00000001412EB960: 48 63 E8           movsxd      rbp,eax
  00000001412EB963: 85 C0              test        eax,eax
  00000001412EB965: 78 05              js          00000001412EB96C
  00000001412EB967: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EB96A: 7C 12              jl          00000001412EB97E
  00000001412EB96C: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EB970: 8B D5              mov         edx,ebp
  00000001412EB972: 48 8D 0D 17 A6 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EB979: E8 12 6E 18 00     call        0000000141472790
  00000001412EB97E: 48 8B C5           mov         rax,rbp
  00000001412EB981: 8B EB              mov         ebp,ebx
  00000001412EB983: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EB987: 40 0F 95 C5        setne       bpl
  00000001412EB98B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB990: E8 2B C2 01 00     call        0000000141307BC0
  00000001412EB995: 45 33 C9           xor         r9d,r9d
  00000001412EB998: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EB99D: 48 8D 15 0C 99 AC  lea         rdx,[0000000142DB52B0h]
                    01
  00000001412EB9A4: 49 8B CC           mov         rcx,r12
  00000001412EB9A7: E8 14 BD FF FF     call        00000001412E76C0
  00000001412EB9AC: 85 ED              test        ebp,ebp
  00000001412EB9AE: 0F 95 C1           setne       cl
  00000001412EB9B1: 88 08              mov         byte ptr [rax],cl
  00000001412EB9B3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB9B8: E8 D3 C7 01 00     call        0000000141308190
  00000001412EB9BD: 90                 nop
  00000001412EB9BE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB9C3: E8 38 B9 01 00     call        0000000141307300
  00000001412EB9C8: 48 8B D0           mov         rdx,rax
  00000001412EB9CB: 4C 8D 05 CE 98 AC  lea         r8,[0000000142DB52A0h]
                    01
  00000001412EB9D2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB9D7: E8 64 BD 01 00     call        0000000141307740
  00000001412EB9DC: 48 85 C0           test        rax,rax
  00000001412EB9DF: 75 22              jne         00000001412EBA03
  00000001412EB9E1: 8B EB              mov         ebp,ebx
  00000001412EB9E3: 48 8D 15 B6 98 AC  lea         rdx,[0000000142DB52A0h]
                    01
  00000001412EB9EA: 48 8D 0D E7 A5 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EB9F1: E8 9A 6D 18 00     call        0000000141472790
  00000001412EB9F6: 90                 nop
  00000001412EB9F7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EB9FC: E8 BF C1 01 00     call        0000000141307BC0
  00000001412EBA01: EB 3D              jmp         00000001412EBA40
  00000001412EBA03: 48 8B C8           mov         rcx,rax
  00000001412EBA06: E8 A1 7D 18 00     call        00000001414737AC
  00000001412EBA0B: 48 63 E8           movsxd      rbp,eax
  00000001412EBA0E: 85 C0              test        eax,eax
  00000001412EBA10: 78 05              js          00000001412EBA17
  00000001412EBA12: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBA15: 7C 12              jl          00000001412EBA29
  00000001412EBA17: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBA1B: 8B D5              mov         edx,ebp
  00000001412EBA1D: 48 8D 0D 6C A5 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBA24: E8 67 6D 18 00     call        0000000141472790
  00000001412EBA29: 48 8B C5           mov         rax,rbp
  00000001412EBA2C: 8B EB              mov         ebp,ebx
  00000001412EBA2E: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBA32: 40 0F 95 C5        setne       bpl
  00000001412EBA36: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBA3B: E8 80 C1 01 00     call        0000000141307BC0
  00000001412EBA40: 45 33 C9           xor         r9d,r9d
  00000001412EBA43: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBA48: 48 8D 15 51 98 AC  lea         rdx,[0000000142DB52A0h]
                    01
  00000001412EBA4F: 49 8B CC           mov         rcx,r12
  00000001412EBA52: E8 69 BC FF FF     call        00000001412E76C0
  00000001412EBA57: 85 ED              test        ebp,ebp
  00000001412EBA59: 0F 95 C1           setne       cl
  00000001412EBA5C: 88 08              mov         byte ptr [rax],cl
  00000001412EBA5E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBA63: E8 28 C7 01 00     call        0000000141308190
  00000001412EBA68: 90                 nop
  00000001412EBA69: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBA6E: E8 8D B8 01 00     call        0000000141307300
  00000001412EBA73: 48 8B D0           mov         rdx,rax
  00000001412EBA76: 4C 8D 05 0B 98 AC  lea         r8,[0000000142DB5288h]
                    01
  00000001412EBA7D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBA82: E8 B9 BC 01 00     call        0000000141307740
  00000001412EBA87: 48 85 C0           test        rax,rax
  00000001412EBA8A: 75 22              jne         00000001412EBAAE
  00000001412EBA8C: 8B EB              mov         ebp,ebx
  00000001412EBA8E: 48 8D 15 F3 97 AC  lea         rdx,[0000000142DB5288h]
                    01
  00000001412EBA95: 48 8D 0D 3C A5 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBA9C: E8 EF 6C 18 00     call        0000000141472790
  00000001412EBAA1: 90                 nop
  00000001412EBAA2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBAA7: E8 14 C1 01 00     call        0000000141307BC0
  00000001412EBAAC: EB 3D              jmp         00000001412EBAEB
  00000001412EBAAE: 48 8B C8           mov         rcx,rax
  00000001412EBAB1: E8 F6 7C 18 00     call        00000001414737AC
  00000001412EBAB6: 48 63 E8           movsxd      rbp,eax
  00000001412EBAB9: 85 C0              test        eax,eax
  00000001412EBABB: 78 05              js          00000001412EBAC2
  00000001412EBABD: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBAC0: 7C 12              jl          00000001412EBAD4
  00000001412EBAC2: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBAC6: 8B D5              mov         edx,ebp
  00000001412EBAC8: 48 8D 0D C1 A4 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBACF: E8 BC 6C 18 00     call        0000000141472790
  00000001412EBAD4: 48 8B C5           mov         rax,rbp
  00000001412EBAD7: 8B EB              mov         ebp,ebx
  00000001412EBAD9: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBADD: 40 0F 95 C5        setne       bpl
  00000001412EBAE1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBAE6: E8 D5 C0 01 00     call        0000000141307BC0
  00000001412EBAEB: 45 33 C9           xor         r9d,r9d
  00000001412EBAEE: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBAF3: 48 8D 15 8E 97 AC  lea         rdx,[0000000142DB5288h]
                    01
  00000001412EBAFA: 49 8B CC           mov         rcx,r12
  00000001412EBAFD: E8 BE BB FF FF     call        00000001412E76C0
  00000001412EBB02: 85 ED              test        ebp,ebp
  00000001412EBB04: 0F 95 C1           setne       cl
  00000001412EBB07: 88 08              mov         byte ptr [rax],cl
  00000001412EBB09: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBB0E: E8 7D C6 01 00     call        0000000141308190
  00000001412EBB13: 90                 nop
  00000001412EBB14: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBB19: E8 E2 B7 01 00     call        0000000141307300
  00000001412EBB1E: 48 8B D0           mov         rdx,rax
  00000001412EBB21: 4C 8D 05 48 97 AC  lea         r8,[0000000142DB5270h]
                    01
  00000001412EBB28: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBB2D: E8 0E BC 01 00     call        0000000141307740
  00000001412EBB32: 48 85 C0           test        rax,rax
  00000001412EBB35: 75 22              jne         00000001412EBB59
  00000001412EBB37: 8B EB              mov         ebp,ebx
  00000001412EBB39: 48 8D 15 30 97 AC  lea         rdx,[0000000142DB5270h]
                    01
  00000001412EBB40: 48 8D 0D 91 A4 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBB47: E8 44 6C 18 00     call        0000000141472790
  00000001412EBB4C: 90                 nop
  00000001412EBB4D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBB52: E8 69 C0 01 00     call        0000000141307BC0
  00000001412EBB57: EB 3D              jmp         00000001412EBB96
  00000001412EBB59: 48 8B C8           mov         rcx,rax
  00000001412EBB5C: E8 4B 7C 18 00     call        00000001414737AC
  00000001412EBB61: 48 63 E8           movsxd      rbp,eax
  00000001412EBB64: 85 C0              test        eax,eax
  00000001412EBB66: 78 05              js          00000001412EBB6D
  00000001412EBB68: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBB6B: 7C 12              jl          00000001412EBB7F
  00000001412EBB6D: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBB71: 8B D5              mov         edx,ebp
  00000001412EBB73: 48 8D 0D 16 A4 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBB7A: E8 11 6C 18 00     call        0000000141472790
  00000001412EBB7F: 48 8B C5           mov         rax,rbp
  00000001412EBB82: 8B EB              mov         ebp,ebx
  00000001412EBB84: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBB88: 40 0F 95 C5        setne       bpl
  00000001412EBB8C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBB91: E8 2A C0 01 00     call        0000000141307BC0
  00000001412EBB96: 45 33 C9           xor         r9d,r9d
  00000001412EBB99: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBB9E: 48 8D 15 CB 96 AC  lea         rdx,[0000000142DB5270h]
                    01
  00000001412EBBA5: 49 8B CC           mov         rcx,r12
  00000001412EBBA8: E8 13 BB FF FF     call        00000001412E76C0
  00000001412EBBAD: 85 ED              test        ebp,ebp
  00000001412EBBAF: 0F 95 C1           setne       cl
  00000001412EBBB2: 88 08              mov         byte ptr [rax],cl
  00000001412EBBB4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBBB9: E8 D2 C5 01 00     call        0000000141308190
  00000001412EBBBE: 90                 nop
  00000001412EBBBF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBBC4: E8 37 B7 01 00     call        0000000141307300
  00000001412EBBC9: 48 8B D0           mov         rdx,rax
  00000001412EBBCC: 4C 8D 05 85 96 AC  lea         r8,[0000000142DB5258h]
                    01
  00000001412EBBD3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBBD8: E8 63 BB 01 00     call        0000000141307740
  00000001412EBBDD: 48 85 C0           test        rax,rax
  00000001412EBBE0: 75 22              jne         00000001412EBC04
  00000001412EBBE2: 8B EB              mov         ebp,ebx
  00000001412EBBE4: 48 8D 15 6D 96 AC  lea         rdx,[0000000142DB5258h]
                    01
  00000001412EBBEB: 48 8D 0D E6 A3 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBBF2: E8 99 6B 18 00     call        0000000141472790
  00000001412EBBF7: 90                 nop
  00000001412EBBF8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBBFD: E8 BE BF 01 00     call        0000000141307BC0
  00000001412EBC02: EB 3D              jmp         00000001412EBC41
  00000001412EBC04: 48 8B C8           mov         rcx,rax
  00000001412EBC07: E8 A0 7B 18 00     call        00000001414737AC
  00000001412EBC0C: 48 63 E8           movsxd      rbp,eax
  00000001412EBC0F: 85 C0              test        eax,eax
  00000001412EBC11: 78 05              js          00000001412EBC18
  00000001412EBC13: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBC16: 7C 12              jl          00000001412EBC2A
  00000001412EBC18: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBC1C: 8B D5              mov         edx,ebp
  00000001412EBC1E: 48 8D 0D 6B A3 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBC25: E8 66 6B 18 00     call        0000000141472790
  00000001412EBC2A: 48 8B C5           mov         rax,rbp
  00000001412EBC2D: 8B EB              mov         ebp,ebx
  00000001412EBC2F: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBC33: 40 0F 95 C5        setne       bpl
  00000001412EBC37: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBC3C: E8 7F BF 01 00     call        0000000141307BC0
  00000001412EBC41: 45 33 C9           xor         r9d,r9d
  00000001412EBC44: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBC49: 48 8D 15 08 96 AC  lea         rdx,[0000000142DB5258h]
                    01
  00000001412EBC50: 49 8B CC           mov         rcx,r12
  00000001412EBC53: E8 68 BA FF FF     call        00000001412E76C0
  00000001412EBC58: 85 ED              test        ebp,ebp
  00000001412EBC5A: 0F 95 C1           setne       cl
  00000001412EBC5D: 88 08              mov         byte ptr [rax],cl
  00000001412EBC5F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBC64: E8 27 C5 01 00     call        0000000141308190
  00000001412EBC69: 90                 nop
  00000001412EBC6A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBC6F: E8 8C B6 01 00     call        0000000141307300
  00000001412EBC74: 48 8B D0           mov         rdx,rax
  00000001412EBC77: 4C 8D 05 CA 95 AC  lea         r8,[0000000142DB5248h]
                    01
  00000001412EBC7E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBC83: E8 B8 BA 01 00     call        0000000141307740
  00000001412EBC88: 48 85 C0           test        rax,rax
  00000001412EBC8B: 75 22              jne         00000001412EBCAF
  00000001412EBC8D: 8B EB              mov         ebp,ebx
  00000001412EBC8F: 48 8D 15 B2 95 AC  lea         rdx,[0000000142DB5248h]
                    01
  00000001412EBC96: 48 8D 0D 3B A3 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBC9D: E8 EE 6A 18 00     call        0000000141472790
  00000001412EBCA2: 90                 nop
  00000001412EBCA3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBCA8: E8 13 BF 01 00     call        0000000141307BC0
  00000001412EBCAD: EB 3D              jmp         00000001412EBCEC
  00000001412EBCAF: 48 8B C8           mov         rcx,rax
  00000001412EBCB2: E8 F5 7A 18 00     call        00000001414737AC
  00000001412EBCB7: 48 63 E8           movsxd      rbp,eax
  00000001412EBCBA: 85 C0              test        eax,eax
  00000001412EBCBC: 78 05              js          00000001412EBCC3
  00000001412EBCBE: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBCC1: 7C 12              jl          00000001412EBCD5
  00000001412EBCC3: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBCC7: 8B D5              mov         edx,ebp
  00000001412EBCC9: 48 8D 0D C0 A2 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBCD0: E8 BB 6A 18 00     call        0000000141472790
  00000001412EBCD5: 48 8B C5           mov         rax,rbp
  00000001412EBCD8: 8B EB              mov         ebp,ebx
  00000001412EBCDA: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBCDE: 40 0F 95 C5        setne       bpl
  00000001412EBCE2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBCE7: E8 D4 BE 01 00     call        0000000141307BC0
  00000001412EBCEC: 45 33 C9           xor         r9d,r9d
  00000001412EBCEF: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBCF4: 48 8D 15 4D 95 AC  lea         rdx,[0000000142DB5248h]
                    01
  00000001412EBCFB: 49 8B CC           mov         rcx,r12
  00000001412EBCFE: E8 BD B9 FF FF     call        00000001412E76C0
  00000001412EBD03: 85 ED              test        ebp,ebp
  00000001412EBD05: 0F 95 C1           setne       cl
  00000001412EBD08: 88 08              mov         byte ptr [rax],cl
  00000001412EBD0A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBD0F: E8 7C C4 01 00     call        0000000141308190
  00000001412EBD14: 90                 nop
  00000001412EBD15: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBD1A: E8 E1 B5 01 00     call        0000000141307300
  00000001412EBD1F: 48 8B D0           mov         rdx,rax
  00000001412EBD22: 4C 8D 05 BF 95 AC  lea         r8,[0000000142DB52E8h]
                    01
  00000001412EBD29: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBD2E: E8 0D BA 01 00     call        0000000141307740
  00000001412EBD33: 48 85 C0           test        rax,rax
  00000001412EBD36: 75 20              jne         00000001412EBD58
  00000001412EBD38: 48 8D 15 A9 95 AC  lea         rdx,[0000000142DB52E8h]
                    01
  00000001412EBD3F: 48 8D 0D 92 A2 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBD46: E8 45 6A 18 00     call        0000000141472790
  00000001412EBD4B: 90                 nop
  00000001412EBD4C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBD51: E8 6A BE 01 00     call        0000000141307BC0
  00000001412EBD56: EB 37              jmp         00000001412EBD8F
  00000001412EBD58: 48 8B C8           mov         rcx,rax
  00000001412EBD5B: E8 4C 7A 18 00     call        00000001414737AC
  00000001412EBD60: 48 63 E8           movsxd      rbp,eax
  00000001412EBD63: 85 C0              test        eax,eax
  00000001412EBD65: 78 05              js          00000001412EBD6C
  00000001412EBD67: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBD6A: 7C 12              jl          00000001412EBD7E
  00000001412EBD6C: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBD70: 8B D5              mov         edx,ebp
  00000001412EBD72: 48 8D 0D 17 A2 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBD79: E8 12 6A 18 00     call        0000000141472790
  00000001412EBD7E: 83 3C AE 00        cmp         dword ptr [rsi+rbp*4],0
  00000001412EBD82: 0F 95 C3           setne       bl
  00000001412EBD85: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBD8A: E8 31 BE 01 00     call        0000000141307BC0
  00000001412EBD8F: 45 33 C9           xor         r9d,r9d
  00000001412EBD92: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBD97: 48 8D 15 4A 95 AC  lea         rdx,[0000000142DB52E8h]
                    01
  00000001412EBD9E: 49 8B CC           mov         rcx,r12
  00000001412EBDA1: E8 1A B9 FF FF     call        00000001412E76C0
  00000001412EBDA6: 85 DB              test        ebx,ebx
  00000001412EBDA8: 0F 95 C1           setne       cl
  00000001412EBDAB: 88 08              mov         byte ptr [rax],cl
  00000001412EBDAD: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBDB2: 48 8D 15 87 94 AC  lea         rdx,[0000000142DB5240h]
                    01
  00000001412EBDB9: 49 8B CC           mov         rcx,r12
  00000001412EBDBC: E8 FF B8 FF FF     call        00000001412E76C0
  00000001412EBDC1: 44 38 08           cmp         byte ptr [rax],r9b
  00000001412EBDC4: 75 2F              jne         00000001412EBDF5
  00000001412EBDC6: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBDCB: 48 8D 15 6E 94 AC  lea         rdx,[0000000142DB5240h]
                    01
  00000001412EBDD2: 49 8B CC           mov         rcx,r12
  00000001412EBDD5: E8 E6 B8 FF FF     call        00000001412E76C0
  00000001412EBDDA: 48 8B D8           mov         rbx,rax
  00000001412EBDDD: 4C 8D 4F 40        lea         r9,[rdi+40h]
  00000001412EBDE1: 4C 8D 05 58 94 AC  lea         r8,[0000000142DB5240h]
                    01
  00000001412EBDE8: 48 8B D7           mov         rdx,rdi
  00000001412EBDEB: 49 8B CC           mov         rcx,r12
  00000001412EBDEE: E8 DD B7 FF FF     call        00000001412E75D0
  00000001412EBDF3: 88 03              mov         byte ptr [rbx],al
  00000001412EBDF5: 45 33 C9           xor         r9d,r9d
  00000001412EBDF8: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBDFD: 48 8D 15 34 94 AC  lea         rdx,[0000000142DB5238h]
                    01
  00000001412EBE04: 49 8B CC           mov         rcx,r12
  00000001412EBE07: E8 B4 B8 FF FF     call        00000001412E76C0
  00000001412EBE0C: 44 38 08           cmp         byte ptr [rax],r9b
  00000001412EBE0F: 0F 85 6B 09 00 00  jne         00000001412EC780
  00000001412EBE15: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBE1A: 48 8D 15 17 94 AC  lea         rdx,[0000000142DB5238h]
                    01
  00000001412EBE21: 49 8B CC           mov         rcx,r12
  00000001412EBE24: E8 97 B8 FF FF     call        00000001412E76C0
  00000001412EBE29: 48 8B D8           mov         rbx,rax
  00000001412EBE2C: 4C 8D 4F 40        lea         r9,[rdi+40h]
  00000001412EBE30: 4C 8D 05 01 94 AC  lea         r8,[0000000142DB5238h]
                    01
  00000001412EBE37: 48 8B D7           mov         rdx,rdi
  00000001412EBE3A: 49 8B CC           mov         rcx,r12
  00000001412EBE3D: E8 8E B7 FF FF     call        00000001412E75D0
  00000001412EBE42: 88 03              mov         byte ptr [rbx],al
  00000001412EBE44: E9 37 09 00 00     jmp         00000001412EC780
  00000001412EBE49: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBE4E: E8 3D C3 01 00     call        0000000141308190
  00000001412EBE53: 90                 nop
  00000001412EBE54: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBE59: E8 A2 B4 01 00     call        0000000141307300
  00000001412EBE5E: 48 8B D0           mov         rdx,rax
  00000001412EBE61: 4C 8D 05 78 96 AC  lea         r8,[0000000142DB54E0h]
                    01
  00000001412EBE68: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBE6D: E8 CE B8 01 00     call        0000000141307740
  00000001412EBE72: 48 85 C0           test        rax,rax
  00000001412EBE75: 75 24              jne         00000001412EBE9B
  00000001412EBE77: 33 DB              xor         ebx,ebx
  00000001412EBE79: 8B EB              mov         ebp,ebx
  00000001412EBE7B: 48 8D 15 5E 96 AC  lea         rdx,[0000000142DB54E0h]
                    01
  00000001412EBE82: 48 8D 0D 4F A1 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBE89: E8 02 69 18 00     call        0000000141472790
  00000001412EBE8E: 90                 nop
  00000001412EBE8F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBE94: E8 27 BD 01 00     call        0000000141307BC0
  00000001412EBE99: EB 3E              jmp         00000001412EBED9
  00000001412EBE9B: 48 8B C8           mov         rcx,rax
  00000001412EBE9E: E8 09 79 18 00     call        00000001414737AC
  00000001412EBEA3: 48 63 D8           movsxd      rbx,eax
  00000001412EBEA6: 85 C0              test        eax,eax
  00000001412EBEA8: 78 05              js          00000001412EBEAF
  00000001412EBEAA: 3B 5F 3C           cmp         ebx,dword ptr [rdi+3Ch]
  00000001412EBEAD: 7C 12              jl          00000001412EBEC1
  00000001412EBEAF: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBEB3: 8B D3              mov         edx,ebx
  00000001412EBEB5: 48 8D 0D D4 A0 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBEBC: E8 CF 68 18 00     call        0000000141472790
  00000001412EBEC1: 48 8B C3           mov         rax,rbx
  00000001412EBEC4: 33 DB              xor         ebx,ebx
  00000001412EBEC6: 8B EB              mov         ebp,ebx
  00000001412EBEC8: 39 1C 86           cmp         dword ptr [rsi+rax*4],ebx
  00000001412EBECB: 40 0F 95 C5        setne       bpl
  00000001412EBECF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBED4: E8 E7 BC 01 00     call        0000000141307BC0
  00000001412EBED9: 45 33 C9           xor         r9d,r9d
  00000001412EBEDC: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBEE1: 48 8D 15 F8 95 AC  lea         rdx,[0000000142DB54E0h]
                    01
  00000001412EBEE8: 49 8B CC           mov         rcx,r12
  00000001412EBEEB: E8 D0 B7 FF FF     call        00000001412E76C0
  00000001412EBEF0: 85 ED              test        ebp,ebp
  00000001412EBEF2: 0F 95 C1           setne       cl
  00000001412EBEF5: 88 08              mov         byte ptr [rax],cl
  00000001412EBEF7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBEFC: E8 8F C2 01 00     call        0000000141308190
  00000001412EBF01: 90                 nop
  00000001412EBF02: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBF07: E8 F4 B3 01 00     call        0000000141307300
  00000001412EBF0C: 48 8B D0           mov         rdx,rax
  00000001412EBF0F: 4C 8D 05 BA 95 AC  lea         r8,[0000000142DB54D0h]
                    01
  00000001412EBF16: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBF1B: E8 20 B8 01 00     call        0000000141307740
  00000001412EBF20: 48 85 C0           test        rax,rax
  00000001412EBF23: 75 22              jne         00000001412EBF47
  00000001412EBF25: 8B EB              mov         ebp,ebx
  00000001412EBF27: 48 8D 15 A2 95 AC  lea         rdx,[0000000142DB54D0h]
                    01
  00000001412EBF2E: 48 8D 0D A3 A0 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBF35: E8 56 68 18 00     call        0000000141472790
  00000001412EBF3A: 90                 nop
  00000001412EBF3B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBF40: E8 7B BC 01 00     call        0000000141307BC0
  00000001412EBF45: EB 3D              jmp         00000001412EBF84
  00000001412EBF47: 48 8B C8           mov         rcx,rax
  00000001412EBF4A: E8 5D 78 18 00     call        00000001414737AC
  00000001412EBF4F: 48 63 E8           movsxd      rbp,eax
  00000001412EBF52: 85 C0              test        eax,eax
  00000001412EBF54: 78 05              js          00000001412EBF5B
  00000001412EBF56: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EBF59: 7C 12              jl          00000001412EBF6D
  00000001412EBF5B: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EBF5F: 8B D5              mov         edx,ebp
  00000001412EBF61: 48 8D 0D 28 A0 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EBF68: E8 23 68 18 00     call        0000000141472790
  00000001412EBF6D: 48 8B C5           mov         rax,rbp
  00000001412EBF70: 8B EB              mov         ebp,ebx
  00000001412EBF72: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EBF76: 40 0F 95 C5        setne       bpl
  00000001412EBF7A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBF7F: E8 3C BC 01 00     call        0000000141307BC0
  00000001412EBF84: 45 33 C9           xor         r9d,r9d
  00000001412EBF87: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EBF8C: 48 8D 15 3D 95 AC  lea         rdx,[0000000142DB54D0h]
                    01
  00000001412EBF93: 49 8B CC           mov         rcx,r12
  00000001412EBF96: E8 25 B7 FF FF     call        00000001412E76C0
  00000001412EBF9B: 85 ED              test        ebp,ebp
  00000001412EBF9D: 0F 95 C1           setne       cl
  00000001412EBFA0: 88 08              mov         byte ptr [rax],cl
  00000001412EBFA2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBFA7: E8 E4 C1 01 00     call        0000000141308190
  00000001412EBFAC: 90                 nop
  00000001412EBFAD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBFB2: E8 49 B3 01 00     call        0000000141307300
  00000001412EBFB7: 48 8B D0           mov         rdx,rax
  00000001412EBFBA: 4C 8D 05 E7 94 AC  lea         r8,[0000000142DB54A8h]
                    01
  00000001412EBFC1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBFC6: E8 75 B7 01 00     call        0000000141307740
  00000001412EBFCB: 48 85 C0           test        rax,rax
  00000001412EBFCE: 75 22              jne         00000001412EBFF2
  00000001412EBFD0: 8B EB              mov         ebp,ebx
  00000001412EBFD2: 48 8D 15 CF 94 AC  lea         rdx,[0000000142DB54A8h]
                    01
  00000001412EBFD9: 48 8D 0D F8 9F AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EBFE0: E8 AB 67 18 00     call        0000000141472790
  00000001412EBFE5: 90                 nop
  00000001412EBFE6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EBFEB: E8 D0 BB 01 00     call        0000000141307BC0
  00000001412EBFF0: EB 3D              jmp         00000001412EC02F
  00000001412EBFF2: 48 8B C8           mov         rcx,rax
  00000001412EBFF5: E8 B2 77 18 00     call        00000001414737AC
  00000001412EBFFA: 48 63 E8           movsxd      rbp,eax
  00000001412EBFFD: 85 C0              test        eax,eax
  00000001412EBFFF: 78 05              js          00000001412EC006
  00000001412EC001: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC004: 7C 12              jl          00000001412EC018
  00000001412EC006: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC00A: 8B D5              mov         edx,ebp
  00000001412EC00C: 48 8D 0D 7D 9F AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC013: E8 78 67 18 00     call        0000000141472790
  00000001412EC018: 48 8B C5           mov         rax,rbp
  00000001412EC01B: 8B EB              mov         ebp,ebx
  00000001412EC01D: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC021: 40 0F 95 C5        setne       bpl
  00000001412EC025: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC02A: E8 91 BB 01 00     call        0000000141307BC0
  00000001412EC02F: 45 33 C9           xor         r9d,r9d
  00000001412EC032: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC037: 48 8D 15 6A 94 AC  lea         rdx,[0000000142DB54A8h]
                    01
  00000001412EC03E: 49 8B CC           mov         rcx,r12
  00000001412EC041: E8 7A B6 FF FF     call        00000001412E76C0
  00000001412EC046: 85 ED              test        ebp,ebp
  00000001412EC048: 0F 95 C1           setne       cl
  00000001412EC04B: 88 08              mov         byte ptr [rax],cl
  00000001412EC04D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC052: E8 39 C1 01 00     call        0000000141308190
  00000001412EC057: 90                 nop
  00000001412EC058: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC05D: E8 9E B2 01 00     call        0000000141307300
  00000001412EC062: 48 8B D0           mov         rdx,rax
  00000001412EC065: 4C 8D 05 24 94 AC  lea         r8,[0000000142DB5490h]
                    01
  00000001412EC06C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC071: E8 CA B6 01 00     call        0000000141307740
  00000001412EC076: 48 85 C0           test        rax,rax
  00000001412EC079: 75 22              jne         00000001412EC09D
  00000001412EC07B: 8B EB              mov         ebp,ebx
  00000001412EC07D: 48 8D 15 0C 94 AC  lea         rdx,[0000000142DB5490h]
                    01
  00000001412EC084: 48 8D 0D 4D 9F AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC08B: E8 00 67 18 00     call        0000000141472790
  00000001412EC090: 90                 nop
  00000001412EC091: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC096: E8 25 BB 01 00     call        0000000141307BC0
  00000001412EC09B: EB 3D              jmp         00000001412EC0DA
  00000001412EC09D: 48 8B C8           mov         rcx,rax
  00000001412EC0A0: E8 07 77 18 00     call        00000001414737AC
  00000001412EC0A5: 48 63 E8           movsxd      rbp,eax
  00000001412EC0A8: 85 C0              test        eax,eax
  00000001412EC0AA: 78 05              js          00000001412EC0B1
  00000001412EC0AC: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC0AF: 7C 12              jl          00000001412EC0C3
  00000001412EC0B1: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC0B5: 8B D5              mov         edx,ebp
  00000001412EC0B7: 48 8D 0D D2 9E AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC0BE: E8 CD 66 18 00     call        0000000141472790
  00000001412EC0C3: 48 8B C5           mov         rax,rbp
  00000001412EC0C6: 8B EB              mov         ebp,ebx
  00000001412EC0C8: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC0CC: 40 0F 95 C5        setne       bpl
  00000001412EC0D0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC0D5: E8 E6 BA 01 00     call        0000000141307BC0
  00000001412EC0DA: 45 33 C9           xor         r9d,r9d
  00000001412EC0DD: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC0E2: 48 8D 15 A7 93 AC  lea         rdx,[0000000142DB5490h]
                    01
  00000001412EC0E9: 49 8B CC           mov         rcx,r12
  00000001412EC0EC: E8 CF B5 FF FF     call        00000001412E76C0
  00000001412EC0F1: 85 ED              test        ebp,ebp
  00000001412EC0F3: 0F 95 C1           setne       cl
  00000001412EC0F6: 88 08              mov         byte ptr [rax],cl
  00000001412EC0F8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC0FD: E8 8E C0 01 00     call        0000000141308190
  00000001412EC102: 90                 nop
  00000001412EC103: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC108: E8 F3 B1 01 00     call        0000000141307300
  00000001412EC10D: 48 8B D0           mov         rdx,rax
  00000001412EC110: 4C 8D 05 61 93 AC  lea         r8,[0000000142DB5478h]
                    01
  00000001412EC117: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC11C: E8 1F B6 01 00     call        0000000141307740
  00000001412EC121: 48 85 C0           test        rax,rax
  00000001412EC124: 75 22              jne         00000001412EC148
  00000001412EC126: 8B EB              mov         ebp,ebx
  00000001412EC128: 48 8D 15 49 93 AC  lea         rdx,[0000000142DB5478h]
                    01
  00000001412EC12F: 48 8D 0D A2 9E AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC136: E8 55 66 18 00     call        0000000141472790
  00000001412EC13B: 90                 nop
  00000001412EC13C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC141: E8 7A BA 01 00     call        0000000141307BC0
  00000001412EC146: EB 3D              jmp         00000001412EC185
  00000001412EC148: 48 8B C8           mov         rcx,rax
  00000001412EC14B: E8 5C 76 18 00     call        00000001414737AC
  00000001412EC150: 48 63 E8           movsxd      rbp,eax
  00000001412EC153: 85 C0              test        eax,eax
  00000001412EC155: 78 05              js          00000001412EC15C
  00000001412EC157: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC15A: 7C 12              jl          00000001412EC16E
  00000001412EC15C: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC160: 8B D5              mov         edx,ebp
  00000001412EC162: 48 8D 0D 27 9E AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC169: E8 22 66 18 00     call        0000000141472790
  00000001412EC16E: 48 8B C5           mov         rax,rbp
  00000001412EC171: 8B EB              mov         ebp,ebx
  00000001412EC173: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC177: 40 0F 95 C5        setne       bpl
  00000001412EC17B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC180: E8 3B BA 01 00     call        0000000141307BC0
  00000001412EC185: 45 33 C9           xor         r9d,r9d
  00000001412EC188: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC18D: 48 8D 15 E4 92 AC  lea         rdx,[0000000142DB5478h]
                    01
  00000001412EC194: 49 8B CC           mov         rcx,r12
  00000001412EC197: E8 24 B5 FF FF     call        00000001412E76C0
  00000001412EC19C: 85 ED              test        ebp,ebp
  00000001412EC19E: 0F 95 C1           setne       cl
  00000001412EC1A1: 88 08              mov         byte ptr [rax],cl
  00000001412EC1A3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC1A8: E8 E3 BF 01 00     call        0000000141308190
  00000001412EC1AD: 90                 nop
  00000001412EC1AE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC1B3: E8 48 B1 01 00     call        0000000141307300
  00000001412EC1B8: 48 8B D0           mov         rdx,rax
  00000001412EC1BB: 4C 8D 05 9E 92 AC  lea         r8,[0000000142DB5460h]
                    01
  00000001412EC1C2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC1C7: E8 74 B5 01 00     call        0000000141307740
  00000001412EC1CC: 48 85 C0           test        rax,rax
  00000001412EC1CF: 75 22              jne         00000001412EC1F3
  00000001412EC1D1: 8B EB              mov         ebp,ebx
  00000001412EC1D3: 48 8D 15 86 92 AC  lea         rdx,[0000000142DB5460h]
                    01
  00000001412EC1DA: 48 8D 0D F7 9D AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC1E1: E8 AA 65 18 00     call        0000000141472790
  00000001412EC1E6: 90                 nop
  00000001412EC1E7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC1EC: E8 CF B9 01 00     call        0000000141307BC0
  00000001412EC1F1: EB 3D              jmp         00000001412EC230
  00000001412EC1F3: 48 8B C8           mov         rcx,rax
  00000001412EC1F6: E8 B1 75 18 00     call        00000001414737AC
  00000001412EC1FB: 48 63 E8           movsxd      rbp,eax
  00000001412EC1FE: 85 C0              test        eax,eax
  00000001412EC200: 78 05              js          00000001412EC207
  00000001412EC202: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC205: 7C 12              jl          00000001412EC219
  00000001412EC207: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC20B: 8B D5              mov         edx,ebp
  00000001412EC20D: 48 8D 0D 7C 9D AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC214: E8 77 65 18 00     call        0000000141472790
  00000001412EC219: 48 8B C5           mov         rax,rbp
  00000001412EC21C: 8B EB              mov         ebp,ebx
  00000001412EC21E: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC222: 40 0F 95 C5        setne       bpl
  00000001412EC226: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC22B: E8 90 B9 01 00     call        0000000141307BC0
  00000001412EC230: 45 33 C9           xor         r9d,r9d
  00000001412EC233: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC238: 48 8D 15 21 92 AC  lea         rdx,[0000000142DB5460h]
                    01
  00000001412EC23F: 49 8B CC           mov         rcx,r12
  00000001412EC242: E8 79 B4 FF FF     call        00000001412E76C0
  00000001412EC247: 85 ED              test        ebp,ebp
  00000001412EC249: 0F 95 C1           setne       cl
  00000001412EC24C: 88 08              mov         byte ptr [rax],cl
  00000001412EC24E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC253: E8 38 BF 01 00     call        0000000141308190
  00000001412EC258: 90                 nop
  00000001412EC259: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC25E: E8 9D B0 01 00     call        0000000141307300
  00000001412EC263: 48 8B D0           mov         rdx,rax
  00000001412EC266: 4C 8D 05 DB 91 AC  lea         r8,[0000000142DB5448h]
                    01
  00000001412EC26D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC272: E8 C9 B4 01 00     call        0000000141307740
  00000001412EC277: 48 85 C0           test        rax,rax
  00000001412EC27A: 75 22              jne         00000001412EC29E
  00000001412EC27C: 8B EB              mov         ebp,ebx
  00000001412EC27E: 48 8D 15 C3 91 AC  lea         rdx,[0000000142DB5448h]
                    01
  00000001412EC285: 48 8D 0D 4C 9D AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC28C: E8 FF 64 18 00     call        0000000141472790
  00000001412EC291: 90                 nop
  00000001412EC292: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC297: E8 24 B9 01 00     call        0000000141307BC0
  00000001412EC29C: EB 3D              jmp         00000001412EC2DB
  00000001412EC29E: 48 8B C8           mov         rcx,rax
  00000001412EC2A1: E8 06 75 18 00     call        00000001414737AC
  00000001412EC2A6: 48 63 E8           movsxd      rbp,eax
  00000001412EC2A9: 85 C0              test        eax,eax
  00000001412EC2AB: 78 05              js          00000001412EC2B2
  00000001412EC2AD: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC2B0: 7C 12              jl          00000001412EC2C4
  00000001412EC2B2: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC2B6: 8B D5              mov         edx,ebp
  00000001412EC2B8: 48 8D 0D D1 9C AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC2BF: E8 CC 64 18 00     call        0000000141472790
  00000001412EC2C4: 48 8B C5           mov         rax,rbp
  00000001412EC2C7: 8B EB              mov         ebp,ebx
  00000001412EC2C9: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC2CD: 40 0F 95 C5        setne       bpl
  00000001412EC2D1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC2D6: E8 E5 B8 01 00     call        0000000141307BC0
  00000001412EC2DB: 45 33 C9           xor         r9d,r9d
  00000001412EC2DE: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC2E3: 48 8D 15 5E 91 AC  lea         rdx,[0000000142DB5448h]
                    01
  00000001412EC2EA: 49 8B CC           mov         rcx,r12
  00000001412EC2ED: E8 CE B3 FF FF     call        00000001412E76C0
  00000001412EC2F2: 85 ED              test        ebp,ebp
  00000001412EC2F4: 0F 95 C1           setne       cl
  00000001412EC2F7: 88 08              mov         byte ptr [rax],cl
  00000001412EC2F9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC2FE: E8 8D BE 01 00     call        0000000141308190
  00000001412EC303: 90                 nop
  00000001412EC304: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC309: E8 F2 AF 01 00     call        0000000141307300
  00000001412EC30E: 48 8B D0           mov         rdx,rax
  00000001412EC311: 4C 8D 05 20 91 AC  lea         r8,[0000000142DB5438h]
                    01
  00000001412EC318: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC31D: E8 1E B4 01 00     call        0000000141307740
  00000001412EC322: 48 85 C0           test        rax,rax
  00000001412EC325: 75 22              jne         00000001412EC349
  00000001412EC327: 8B EB              mov         ebp,ebx
  00000001412EC329: 48 8D 15 08 91 AC  lea         rdx,[0000000142DB5438h]
                    01
  00000001412EC330: 48 8D 0D A1 9C AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC337: E8 54 64 18 00     call        0000000141472790
  00000001412EC33C: 90                 nop
  00000001412EC33D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC342: E8 79 B8 01 00     call        0000000141307BC0
  00000001412EC347: EB 3D              jmp         00000001412EC386
  00000001412EC349: 48 8B C8           mov         rcx,rax
  00000001412EC34C: E8 5B 74 18 00     call        00000001414737AC
  00000001412EC351: 48 63 E8           movsxd      rbp,eax
  00000001412EC354: 85 C0              test        eax,eax
  00000001412EC356: 78 05              js          00000001412EC35D
  00000001412EC358: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC35B: 7C 12              jl          00000001412EC36F
  00000001412EC35D: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC361: 8B D5              mov         edx,ebp
  00000001412EC363: 48 8D 0D 26 9C AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC36A: E8 21 64 18 00     call        0000000141472790
  00000001412EC36F: 48 8B C5           mov         rax,rbp
  00000001412EC372: 8B EB              mov         ebp,ebx
  00000001412EC374: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC378: 40 0F 95 C5        setne       bpl
  00000001412EC37C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC381: E8 3A B8 01 00     call        0000000141307BC0
  00000001412EC386: 45 33 C9           xor         r9d,r9d
  00000001412EC389: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC38E: 48 8D 15 A3 90 AC  lea         rdx,[0000000142DB5438h]
                    01
  00000001412EC395: 49 8B CC           mov         rcx,r12
  00000001412EC398: E8 23 B3 FF FF     call        00000001412E76C0
  00000001412EC39D: 85 ED              test        ebp,ebp
  00000001412EC39F: 0F 95 C1           setne       cl
  00000001412EC3A2: 88 08              mov         byte ptr [rax],cl
  00000001412EC3A4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC3A9: E8 E2 BD 01 00     call        0000000141308190
  00000001412EC3AE: 90                 nop
  00000001412EC3AF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC3B4: E8 47 AF 01 00     call        0000000141307300
  00000001412EC3B9: 48 8B D0           mov         rdx,rax
  00000001412EC3BC: 4C 8D 05 65 90 AC  lea         r8,[0000000142DB5428h]
                    01
  00000001412EC3C3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC3C8: E8 73 B3 01 00     call        0000000141307740
  00000001412EC3CD: 48 85 C0           test        rax,rax
  00000001412EC3D0: 75 22              jne         00000001412EC3F4
  00000001412EC3D2: 8B EB              mov         ebp,ebx
  00000001412EC3D4: 48 8D 15 4D 90 AC  lea         rdx,[0000000142DB5428h]
                    01
  00000001412EC3DB: 48 8D 0D F6 9B AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC3E2: E8 A9 63 18 00     call        0000000141472790
  00000001412EC3E7: 90                 nop
  00000001412EC3E8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC3ED: E8 CE B7 01 00     call        0000000141307BC0
  00000001412EC3F2: EB 3D              jmp         00000001412EC431
  00000001412EC3F4: 48 8B C8           mov         rcx,rax
  00000001412EC3F7: E8 B0 73 18 00     call        00000001414737AC
  00000001412EC3FC: 48 63 E8           movsxd      rbp,eax
  00000001412EC3FF: 85 C0              test        eax,eax
  00000001412EC401: 78 05              js          00000001412EC408
  00000001412EC403: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC406: 7C 12              jl          00000001412EC41A
  00000001412EC408: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC40C: 8B D5              mov         edx,ebp
  00000001412EC40E: 48 8D 0D 7B 9B AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC415: E8 76 63 18 00     call        0000000141472790
  00000001412EC41A: 48 8B C5           mov         rax,rbp
  00000001412EC41D: 8B EB              mov         ebp,ebx
  00000001412EC41F: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC423: 40 0F 95 C5        setne       bpl
  00000001412EC427: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC42C: E8 8F B7 01 00     call        0000000141307BC0
  00000001412EC431: 45 33 C9           xor         r9d,r9d
  00000001412EC434: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC439: 48 8D 15 E8 8F AC  lea         rdx,[0000000142DB5428h]
                    01
  00000001412EC440: 49 8B CC           mov         rcx,r12
  00000001412EC443: E8 78 B2 FF FF     call        00000001412E76C0
  00000001412EC448: 85 ED              test        ebp,ebp
  00000001412EC44A: 0F 95 C1           setne       cl
  00000001412EC44D: 88 08              mov         byte ptr [rax],cl
  00000001412EC44F: 41 0F B6 44 24 1E  movzx       eax,byte ptr [r12+1Eh]
  00000001412EC455: 41 88 44 24 20     mov         byte ptr [r12+20h],al
  00000001412EC45A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC45F: E8 2C BD 01 00     call        0000000141308190
  00000001412EC464: 90                 nop
  00000001412EC465: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC46A: E8 91 AE 01 00     call        0000000141307300
  00000001412EC46F: 48 8B D0           mov         rdx,rax
  00000001412EC472: 4C 8D 05 87 8F AC  lea         r8,[0000000142DB5400h]
                    01
  00000001412EC479: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC47E: E8 BD B2 01 00     call        0000000141307740
  00000001412EC483: 48 85 C0           test        rax,rax
  00000001412EC486: 75 22              jne         00000001412EC4AA
  00000001412EC488: 8B EB              mov         ebp,ebx
  00000001412EC48A: 48 8D 15 6F 8F AC  lea         rdx,[0000000142DB5400h]
                    01
  00000001412EC491: 48 8D 0D 40 9B AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC498: E8 F3 62 18 00     call        0000000141472790
  00000001412EC49D: 90                 nop
  00000001412EC49E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC4A3: E8 18 B7 01 00     call        0000000141307BC0
  00000001412EC4A8: EB 3D              jmp         00000001412EC4E7
  00000001412EC4AA: 48 8B C8           mov         rcx,rax
  00000001412EC4AD: E8 FA 72 18 00     call        00000001414737AC
  00000001412EC4B2: 48 63 E8           movsxd      rbp,eax
  00000001412EC4B5: 85 C0              test        eax,eax
  00000001412EC4B7: 78 05              js          00000001412EC4BE
  00000001412EC4B9: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC4BC: 7C 12              jl          00000001412EC4D0
  00000001412EC4BE: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC4C2: 8B D5              mov         edx,ebp
  00000001412EC4C4: 48 8D 0D C5 9A AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC4CB: E8 C0 62 18 00     call        0000000141472790
  00000001412EC4D0: 48 8B C5           mov         rax,rbp
  00000001412EC4D3: 8B EB              mov         ebp,ebx
  00000001412EC4D5: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC4D9: 40 0F 95 C5        setne       bpl
  00000001412EC4DD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC4E2: E8 D9 B6 01 00     call        0000000141307BC0
  00000001412EC4E7: 45 33 C9           xor         r9d,r9d
  00000001412EC4EA: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC4EF: 48 8D 15 0A 8F AC  lea         rdx,[0000000142DB5400h]
                    01
  00000001412EC4F6: 49 8B CC           mov         rcx,r12
  00000001412EC4F9: E8 C2 B1 FF FF     call        00000001412E76C0
  00000001412EC4FE: 85 ED              test        ebp,ebp
  00000001412EC500: 0F 95 C1           setne       cl
  00000001412EC503: 88 08              mov         byte ptr [rax],cl
  00000001412EC505: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC50A: E8 81 BC 01 00     call        0000000141308190
  00000001412EC50F: 90                 nop
  00000001412EC510: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC515: E8 E6 AD 01 00     call        0000000141307300
  00000001412EC51A: 48 8B D0           mov         rdx,rax
  00000001412EC51D: 4C 8D 05 CC 8E AC  lea         r8,[0000000142DB53F0h]
                    01
  00000001412EC524: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC529: E8 12 B2 01 00     call        0000000141307740
  00000001412EC52E: 48 85 C0           test        rax,rax
  00000001412EC531: 75 22              jne         00000001412EC555
  00000001412EC533: 8B EB              mov         ebp,ebx
  00000001412EC535: 48 8D 15 B4 8E AC  lea         rdx,[0000000142DB53F0h]
                    01
  00000001412EC53C: 48 8D 0D 95 9A AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC543: E8 48 62 18 00     call        0000000141472790
  00000001412EC548: 90                 nop
  00000001412EC549: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC54E: E8 6D B6 01 00     call        0000000141307BC0
  00000001412EC553: EB 3D              jmp         00000001412EC592
  00000001412EC555: 48 8B C8           mov         rcx,rax
  00000001412EC558: E8 4F 72 18 00     call        00000001414737AC
  00000001412EC55D: 48 63 E8           movsxd      rbp,eax
  00000001412EC560: 85 C0              test        eax,eax
  00000001412EC562: 78 05              js          00000001412EC569
  00000001412EC564: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC567: 7C 12              jl          00000001412EC57B
  00000001412EC569: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC56D: 8B D5              mov         edx,ebp
  00000001412EC56F: 48 8D 0D 1A 9A AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC576: E8 15 62 18 00     call        0000000141472790
  00000001412EC57B: 48 8B C5           mov         rax,rbp
  00000001412EC57E: 8B EB              mov         ebp,ebx
  00000001412EC580: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC584: 40 0F 95 C5        setne       bpl
  00000001412EC588: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC58D: E8 2E B6 01 00     call        0000000141307BC0
  00000001412EC592: 45 33 C9           xor         r9d,r9d
  00000001412EC595: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC59A: 48 8D 15 4F 8E AC  lea         rdx,[0000000142DB53F0h]
                    01
  00000001412EC5A1: 49 8B CC           mov         rcx,r12
  00000001412EC5A4: E8 17 B1 FF FF     call        00000001412E76C0
  00000001412EC5A9: 85 ED              test        ebp,ebp
  00000001412EC5AB: 0F 95 C1           setne       cl
  00000001412EC5AE: 88 08              mov         byte ptr [rax],cl
  00000001412EC5B0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC5B5: E8 D6 BB 01 00     call        0000000141308190
  00000001412EC5BA: 90                 nop
  00000001412EC5BB: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC5C0: E8 3B AD 01 00     call        0000000141307300
  00000001412EC5C5: 48 8B D0           mov         rdx,rax
  00000001412EC5C8: 4C 8D 05 09 8E AC  lea         r8,[0000000142DB53D8h]
                    01
  00000001412EC5CF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC5D4: E8 67 B1 01 00     call        0000000141307740
  00000001412EC5D9: 48 85 C0           test        rax,rax
  00000001412EC5DC: 75 22              jne         00000001412EC600
  00000001412EC5DE: 8B EB              mov         ebp,ebx
  00000001412EC5E0: 48 8D 15 F1 8D AC  lea         rdx,[0000000142DB53D8h]
                    01
  00000001412EC5E7: 48 8D 0D EA 99 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC5EE: E8 9D 61 18 00     call        0000000141472790
  00000001412EC5F3: 90                 nop
  00000001412EC5F4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC5F9: E8 C2 B5 01 00     call        0000000141307BC0
  00000001412EC5FE: EB 3D              jmp         00000001412EC63D
  00000001412EC600: 48 8B C8           mov         rcx,rax
  00000001412EC603: E8 A4 71 18 00     call        00000001414737AC
  00000001412EC608: 48 63 E8           movsxd      rbp,eax
  00000001412EC60B: 85 C0              test        eax,eax
  00000001412EC60D: 78 05              js          00000001412EC614
  00000001412EC60F: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC612: 7C 12              jl          00000001412EC626
  00000001412EC614: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC618: 8B D5              mov         edx,ebp
  00000001412EC61A: 48 8D 0D 6F 99 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC621: E8 6A 61 18 00     call        0000000141472790
  00000001412EC626: 48 8B C5           mov         rax,rbp
  00000001412EC629: 8B EB              mov         ebp,ebx
  00000001412EC62B: 83 3C 86 00        cmp         dword ptr [rsi+rax*4],0
  00000001412EC62F: 40 0F 95 C5        setne       bpl
  00000001412EC633: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC638: E8 83 B5 01 00     call        0000000141307BC0
  00000001412EC63D: 45 33 C9           xor         r9d,r9d
  00000001412EC640: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC645: 48 8D 15 8C 8D AC  lea         rdx,[0000000142DB53D8h]
                    01
  00000001412EC64C: 49 8B CC           mov         rcx,r12
  00000001412EC64F: E8 6C B0 FF FF     call        00000001412E76C0
  00000001412EC654: 85 ED              test        ebp,ebp
  00000001412EC656: 0F 95 C1           setne       cl
  00000001412EC659: 88 08              mov         byte ptr [rax],cl
  00000001412EC65B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC660: E8 2B BB 01 00     call        0000000141308190
  00000001412EC665: 90                 nop
  00000001412EC666: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC66B: E8 90 AC 01 00     call        0000000141307300
  00000001412EC670: 48 8B D0           mov         rdx,rax
  00000001412EC673: 4C 8D 05 96 8D AC  lea         r8,[0000000142DB5410h]
                    01
  00000001412EC67A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC67F: E8 BC B0 01 00     call        0000000141307740
  00000001412EC684: 48 85 C0           test        rax,rax
  00000001412EC687: 75 20              jne         00000001412EC6A9
  00000001412EC689: 48 8D 15 80 8D AC  lea         rdx,[0000000142DB5410h]
                    01
  00000001412EC690: 48 8D 0D 41 99 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412EC697: E8 F4 60 18 00     call        0000000141472790
  00000001412EC69C: 90                 nop
  00000001412EC69D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC6A2: E8 19 B5 01 00     call        0000000141307BC0
  00000001412EC6A7: EB 37              jmp         00000001412EC6E0
  00000001412EC6A9: 48 8B C8           mov         rcx,rax
  00000001412EC6AC: E8 FB 70 18 00     call        00000001414737AC
  00000001412EC6B1: 48 63 E8           movsxd      rbp,eax
  00000001412EC6B4: 85 C0              test        eax,eax
  00000001412EC6B6: 78 05              js          00000001412EC6BD
  00000001412EC6B8: 3B 6F 3C           cmp         ebp,dword ptr [rdi+3Ch]
  00000001412EC6BB: 7C 12              jl          00000001412EC6CF
  00000001412EC6BD: 44 8B 47 3C        mov         r8d,dword ptr [rdi+3Ch]
  00000001412EC6C1: 8B D5              mov         edx,ebp
  00000001412EC6C3: 48 8D 0D C6 98 AC  lea         rcx,[0000000142DB5F90h]
                    01
  00000001412EC6CA: E8 C1 60 18 00     call        0000000141472790
  00000001412EC6CF: 83 3C AE 00        cmp         dword ptr [rsi+rbp*4],0
  00000001412EC6D3: 0F 95 C3           setne       bl
  00000001412EC6D6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001412EC6DB: E8 E0 B4 01 00     call        0000000141307BC0
  00000001412EC6E0: 45 33 C9           xor         r9d,r9d
  00000001412EC6E3: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC6E8: 48 8D 15 21 8D AC  lea         rdx,[0000000142DB5410h]
                    01
  00000001412EC6EF: 49 8B CC           mov         rcx,r12
  00000001412EC6F2: E8 C9 AF FF FF     call        00000001412E76C0
  00000001412EC6F7: 85 DB              test        ebx,ebx
  00000001412EC6F9: 0F 95 C1           setne       cl
  00000001412EC6FC: 88 08              mov         byte ptr [rax],cl
  00000001412EC6FE: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC703: 48 8D 15 C6 8C AC  lea         rdx,[0000000142DB53D0h]
                    01
  00000001412EC70A: 49 8B CC           mov         rcx,r12
  00000001412EC70D: E8 AE AF FF FF     call        00000001412E76C0
  00000001412EC712: 44 38 08           cmp         byte ptr [rax],r9b
  00000001412EC715: 75 2F              jne         00000001412EC746
  00000001412EC717: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC71C: 48 8D 15 AD 8C AC  lea         rdx,[0000000142DB53D0h]
                    01
  00000001412EC723: 49 8B CC           mov         rcx,r12
  00000001412EC726: E8 95 AF FF FF     call        00000001412E76C0
  00000001412EC72B: 48 8B D8           mov         rbx,rax
  00000001412EC72E: 4C 8D 4F 40        lea         r9,[rdi+40h]
  00000001412EC732: 4C 8D 05 97 8C AC  lea         r8,[0000000142DB53D0h]
                    01
  00000001412EC739: 48 8B D7           mov         rdx,rdi
  00000001412EC73C: 49 8B CC           mov         rcx,r12
  00000001412EC73F: E8 8C AE FF FF     call        00000001412E75D0
  00000001412EC744: 88 03              mov         byte ptr [rbx],al
  00000001412EC746: 41 80 7C 24 3A 00  cmp         byte ptr [r12+3Ah],0
  00000001412EC74C: 75 32              jne         00000001412EC780
  00000001412EC74E: 45 33 C9           xor         r9d,r9d
  00000001412EC751: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000001412EC756: 48 8D 15 6B 8C AC  lea         rdx,[0000000142DB53C8h]
                    01
  00000001412EC75D: 49 8B CC           mov         rcx,r12
  00000001412EC760: E8 5B AF FF FF     call        00000001412E76C0
  00000001412EC765: 48 8B D8           mov         rbx,rax
  00000001412EC768: 4C 8D 4F 40        lea         r9,[rdi+40h]
  00000001412EC76C: 4C 8D 05 55 8C AC  lea         r8,[0000000142DB53C8h]
                    01
  00000001412EC773: 48 8B D7           mov         rdx,rdi
  00000001412EC776: 49 8B CC           mov         rcx,r12
  00000001412EC779: E8 52 AE FF FF     call        00000001412E75D0
  00000001412EC77E: 88 03              mov         byte ptr [rbx],al
  00000001412EC780: 48 8B 8C 24 E0 04  mov         rcx,qword ptr [rsp+00000000000004E0h]
                    00 00
  00000001412EC788: 48 33 CC           xor         rcx,rsp
  00000001412EC78B: E8 80 17 18 00     call        000000014146DF10
  00000001412EC790: 4C 8D 9C 24 F0 04  lea         r11,[rsp+00000000000004F0h]
                    00 00
  00000001412EC798: 49 8B 5B 28        mov         rbx,qword ptr [r11+28h]
  00000001412EC79C: 49 8B 6B 30        mov         rbp,qword ptr [r11+30h]
  00000001412EC7A0: 49 8B 73 38        mov         rsi,qword ptr [r11+38h]
  00000001412EC7A4: 49 8B E3           mov         rsp,r11
  00000001412EC7A7: 41 5D              pop         r13
  00000001412EC7A9: 41 5C              pop         r12
  00000001412EC7AB: 5F                 pop         rdi
  00000001412EC7AC: C3                 ret
  00000001412EC7AD: CC                 int         3

  Summary

     1E02000 .text
