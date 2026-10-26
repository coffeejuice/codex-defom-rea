Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000671306D0: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  00000000671306D5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000000671306DA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000671306DF: 53                 push        rbx
  00000000671306E0: 56                 push        rsi
  00000000671306E1: 57                 push        rdi
  00000000671306E2: 41 54              push        r12
  00000000671306E4: 41 55              push        r13
  00000000671306E6: 41 56              push        r14
  00000000671306E8: 41 57              push        r15
  00000000671306EA: 48 83 EC 50        sub         rsp,50h
  00000000671306EE: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000671306F7: 49 8B F8           mov         rdi,r8
  00000000671306FA: 4C 63 F2           movsxd      r14,edx
  00000000671306FD: 4C 8B E9           mov         r13,rcx
  0000000067130700: 48 8B 9C 24 B0 00  mov         rbx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000067130708: 48 85 DB           test        rbx,rbx
  000000006713070B: 74 35              je          0000000067130742
  000000006713070D: 48 8B CB           mov         rcx,rbx
  0000000067130710: E8 0B D7 FE FF     call        000000006711DE20
  0000000067130715: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  000000006713071C: 4C 8B 53 10        mov         r10,qword ptr [rbx+10h]
  0000000067130720: 41 83 3A 06        cmp         dword ptr [r10],6
  0000000067130724: 7C 17              jl          000000006713073D
  0000000067130726: 4C 8B 43 18        mov         r8,qword ptr [rbx+18h]
  000000006713072A: 4D 85 C0           test        r8,r8
  000000006713072D: 74 0E              je          000000006713073D
  000000006713072F: 4D 8B 78 08        mov         r15,qword ptr [r8+8]
  0000000067130733: 4C 89 7C 24 20     mov         qword ptr [rsp+20h],r15
  0000000067130738: 45 33 E4           xor         r12d,r12d
  000000006713073B: EB 15              jmp         0000000067130752
  000000006713073D: 45 33 E4           xor         r12d,r12d
  0000000067130740: EB 0B              jmp         000000006713074D
  0000000067130742: 45 33 E4           xor         r12d,r12d
  0000000067130745: 44 89 A4 24 B0 00  mov         dword ptr [rsp+00000000000000B0h],r12d
                    00 00
  000000006713074D: 4C 89 64 24 20     mov         qword ptr [rsp+20h],r12
  0000000067130752: 48 8B CF           mov         rcx,rdi
  0000000067130755: E8 66 9C FF FF     call        000000006712A3C0
  000000006713075A: 48 8B F0           mov         rsi,rax
  000000006713075D: 49 8B CD           mov         rcx,r13
  0000000067130760: E8 5B 9C FF FF     call        000000006712A3C0
  0000000067130765: 48 8B D8           mov         rbx,rax
  0000000067130768: 48 3B C6           cmp         rax,rsi
  000000006713076B: 75 0A              jne         0000000067130777
  000000006713076D: 48 8B F8           mov         rdi,rax
  0000000067130770: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000067130775: EB 0F              jmp         0000000067130786
  0000000067130777: 48 8B FE           mov         rdi,rsi
  000000006713077A: 48 3B C6           cmp         rax,rsi
  000000006713077D: 48 0F 42 F8        cmovb       rdi,rax
  0000000067130781: 48 89 7C 24 38     mov         qword ptr [rsp+38h],rdi
  0000000067130786: 48 3B C6           cmp         rax,rsi
  0000000067130789: 75 0A              jne         0000000067130795
  000000006713078B: 49 8B DC           mov         rbx,r12
  000000006713078E: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  0000000067130793: EB 0C              jmp         00000000671307A1
  0000000067130795: 48 3B C6           cmp         rax,rsi
  0000000067130798: 48 0F 42 DE        cmovb       rbx,rsi
  000000006713079C: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000671307A1: 48 85 FF           test        rdi,rdi
  00000000671307A4: 74 4D              je          00000000671307F3
  00000000671307A6: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000671307A9: F6 41 04 01        test        byte ptr [rcx+4],1
  00000000671307AD: 74 10              je          00000000671307BF
  00000000671307AF: 48 8B CF           mov         rcx,rdi
  00000000671307B2: E8 F9 55 EF FF     call        0000000067025DB0
  00000000671307B7: 41 BD 01 00 00 00  mov         r13d,1
  00000000671307BD: EB 3A              jmp         00000000671307F9
  00000000671307BF: 33 C0              xor         eax,eax
  00000000671307C1: 41 BD 01 00 00 00  mov         r13d,1
  00000000671307C7: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  00000000671307CC: 0F 94 C1           sete        cl
  00000000671307CF: 84 C9              test        cl,cl
  00000000671307D1: 75 14              jne         00000000671307E7
  00000000671307D3: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  00000000671307D8: 48 8B CF           mov         rcx,rdi
  00000000671307DB: E8 D0 53 EF FF     call        0000000067025BB0
  00000000671307E0: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000671307E5: EB 12              jmp         00000000671307F9
  00000000671307E7: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000671307EC: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  00000000671307F1: EB 06              jmp         00000000671307F9
  00000000671307F3: 41 BD 01 00 00 00  mov         r13d,1
  00000000671307F9: 48 85 DB           test        rbx,rbx
  00000000671307FC: 74 3F              je          000000006713083D
  00000000671307FE: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067130801: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067130805: 74 0A              je          0000000067130811
  0000000067130807: 48 8B CB           mov         rcx,rbx
  000000006713080A: E8 A1 55 EF FF     call        0000000067025DB0
  000000006713080F: EB 2C              jmp         000000006713083D
  0000000067130811: 33 C0              xor         eax,eax
  0000000067130813: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  0000000067130818: 0F 94 C1           sete        cl
  000000006713081B: 84 C9              test        cl,cl
  000000006713081D: 75 14              jne         0000000067130833
  000000006713081F: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067130824: 48 8B CB           mov         rcx,rbx
  0000000067130827: E8 84 53 EF FF     call        0000000067025BB0
  000000006713082C: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  0000000067130831: EB 0A              jmp         000000006713083D
  0000000067130833: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067130838: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006713083D: C6 44 24 48 01     mov         byte ptr [rsp+48h],1
  0000000067130842: 44 8B BC 24 B8 00  mov         r15d,dword ptr [rsp+00000000000000B8h]
                    00 00
  000000006713084A: 45 84 FF           test        r15b,r15b
  000000006713084D: 0F 89 E3 00 00 00  jns         0000000067130936
  0000000067130853: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006713085B: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  000000006713085F: 48 8B 48 48        mov         rcx,qword ptr [rax+48h]
  0000000067130863: 48 85 C9           test        rcx,rcx
  0000000067130866: 0F 84 C6 00 00 00  je          0000000067130932
  000000006713086C: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006713086F: 44 39 70 08        cmp         dword ptr [rax+8],r14d
  0000000067130873: 0F 8E B9 00 00 00  jle         0000000067130932
  0000000067130879: 41 8B D6           mov         edx,r14d
  000000006713087C: E8 1F DC FF FF     call        000000006712E4A0
  0000000067130881: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000067130884: 44 8B 84 24 B0 00  mov         r8d,dword ptr [rsp+00000000000000B0h]
                    00 00
  000000006713088C: 8B 8C 24 A8 00 00  mov         ecx,dword ptr [rsp+00000000000000A8h]
                    00
  0000000067130893: 44 03 C1           add         r8d,ecx
  0000000067130896: 48 85 D2           test        rdx,rdx
  0000000067130899: 0F 84 93 00 00 00  je          0000000067130932
  000000006713089F: 4C 8B 8C 24 A0 00  mov         r9,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000671308A7: 4C 39 4A 08        cmp         qword ptr [rdx+8],r9
  00000000671308AB: 75 0F              jne         00000000671308BC
  00000000671308AD: 0F B7 4A 3A        movzx       ecx,word ptr [rdx+3Ah]
  00000000671308B1: 0F B7 42 38        movzx       eax,word ptr [rdx+38h]
  00000000671308B5: 03 C8              add         ecx,eax
  00000000671308B7: 41 3B C8           cmp         ecx,r8d
  00000000671308BA: 74 0B              je          00000000671308C7
  00000000671308BC: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  00000000671308C0: 48 85 D2           test        rdx,rdx
  00000000671308C3: 74 6D              je          0000000067130932
  00000000671308C5: EB E0              jmp         00000000671308A7
  00000000671308C7: 48 85 FF           test        rdi,rdi
  00000000671308CA: 74 33              je          00000000671308FF
  00000000671308CC: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000671308CF: F6 41 04 01        test        byte ptr [rcx+4],1
  00000000671308D3: 74 0A              je          00000000671308DF
  00000000671308D5: 48 8B CF           mov         rcx,rdi
  00000000671308D8: E8 53 52 EF FF     call        0000000067025B30
  00000000671308DD: EB 20              jmp         00000000671308FF
  00000000671308DF: 41 8B C5           mov         eax,r13d
  00000000671308E2: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  00000000671308E7: 74 11              je          00000000671308FA
  00000000671308E9: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000000671308EE: E8 7D 52 EF FF     call        0000000067025B70
  00000000671308F3: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000671308F8: EB 05              jmp         00000000671308FF
  00000000671308FA: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000671308FF: 48 85 DB           test        rbx,rbx
  0000000067130902: 74 27              je          000000006713092B
  0000000067130904: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067130907: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006713090B: 74 0A              je          0000000067130917
  000000006713090D: 48 8B CB           mov         rcx,rbx
  0000000067130910: E8 1B 52 EF FF     call        0000000067025B30
  0000000067130915: EB 14              jmp         000000006713092B
  0000000067130917: 41 8B C5           mov         eax,r13d
  000000006713091A: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  000000006713091F: 74 0A              je          000000006713092B
  0000000067130921: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067130926: E8 45 52 EF FF     call        0000000067025B70
  000000006713092B: 32 C0              xor         al,al
  000000006713092D: E9 54 01 00 00     jmp         0000000067130A86
  0000000067130932: 41 83 E7 7F        and         r15d,7Fh
  0000000067130936: B9 40 00 00 00     mov         ecx,40h
  000000006713093B: E8 46 81 04 00     call        0000000067178A86
  0000000067130940: 48 8B F0           mov         rsi,rax
  0000000067130943: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000067130948: 4C 8B 8C 24 90 00  mov         r9,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000067130950: 4C 89 08           mov         qword ptr [rax],r9
  0000000067130953: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006713095B: 48 89 46 08        mov         qword ptr [rsi+8],rax
  000000006713095F: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  0000000067130966: 66 89 46 3A        mov         word ptr [rsi+3Ah],ax
  000000006713096A: 8B 84 24 B0 00 00  mov         eax,dword ptr [rsp+00000000000000B0h]
                    00
  0000000067130971: 66 89 46 38        mov         word ptr [rsi+38h],ax
  0000000067130975: B8 F8 FF 00 00     mov         eax,0FFF8h
  000000006713097A: 66 21 46 3C        and         word ptr [rsi+3Ch],ax
  000000006713097E: 66 41 83 E7 07     and         r15w,7
  0000000067130983: 66 44 09 7E 3C     or          word ptr [rsi+3Ch],r15w
  0000000067130988: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000067130990: 48 89 4E 30        mov         qword ptr [rsi+30h],rcx
  0000000067130994: 4C 89 66 18        mov         qword ptr [rsi+18h],r12
  0000000067130998: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000006713099D: 48 89 46 10        mov         qword ptr [rsi+10h],rax
  00000000671309A1: 4C 8B C6           mov         r8,rsi
  00000000671309A4: 41 8B D6           mov         edx,r14d
  00000000671309A7: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  00000000671309AB: E8 90 ED FF FF     call        000000006712F740
  00000000671309B0: 90                 nop
  00000000671309B1: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000671309B9: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000671309BD: 48 83 C1 50        add         rcx,50h
  00000000671309C1: 48 89 4E 28        mov         qword ptr [rsi+28h],rcx
  00000000671309C5: 48 8D 56 20        lea         rdx,[rsi+20h]
  00000000671309C9: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671309CC: 48 89 02           mov         qword ptr [rdx],rax
  00000000671309CF: 48 89 31           mov         qword ptr [rcx],rsi
  00000000671309D2: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000671309D5: 48 85 C0           test        rax,rax
  00000000671309D8: 74 04              je          00000000671309DE
  00000000671309DA: 48 89 50 28        mov         qword ptr [rax+28h],rdx
  00000000671309DE: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  00000000671309E6: 4D 8B 40 08        mov         r8,qword ptr [r8+8]
  00000000671309EA: 45 85 F6           test        r14d,r14d
  00000000671309ED: 79 12              jns         0000000067130A01
  00000000671309EF: 41 C7 40 64 FF FF  mov         dword ptr [r8+64h],0FFFFFFFFh
                    FF FF
  00000000671309F7: 41 C7 40 60 FF FF  mov         dword ptr [r8+60h],0FFFFFFFFh
                    FF FF
  00000000671309FF: EB 1F              jmp         0000000067130A20
  0000000067130A01: 41 83 FE 40        cmp         r14d,40h
  0000000067130A05: 7D 19              jge         0000000067130A20
  0000000067130A07: 49 8B D6           mov         rdx,r14
  0000000067130A0A: 48 C1 FA 05        sar         rdx,5
  0000000067130A0E: 41 83 E6 1F        and         r14d,1Fh
  0000000067130A12: 41 8B C5           mov         eax,r13d
  0000000067130A15: 41 0F B6 CE        movzx       ecx,r14b
  0000000067130A19: D3 E0              shl         eax,cl
  0000000067130A1B: 41 09 44 90 60     or          dword ptr [r8+rdx*4+60h],eax
  0000000067130A20: 48 85 FF           test        rdi,rdi
  0000000067130A23: 74 33              je          0000000067130A58
  0000000067130A25: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067130A28: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067130A2C: 74 0A              je          0000000067130A38
  0000000067130A2E: 48 8B CF           mov         rcx,rdi
  0000000067130A31: E8 FA 50 EF FF     call        0000000067025B30
  0000000067130A36: EB 20              jmp         0000000067130A58
  0000000067130A38: 41 8B C5           mov         eax,r13d
  0000000067130A3B: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  0000000067130A40: 74 11              je          0000000067130A53
  0000000067130A42: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000067130A47: E8 24 51 EF FF     call        0000000067025B70
  0000000067130A4C: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067130A51: EB 05              jmp         0000000067130A58
  0000000067130A53: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067130A58: 48 85 DB           test        rbx,rbx
  0000000067130A5B: 74 27              je          0000000067130A84
  0000000067130A5D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067130A60: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067130A64: 74 0A              je          0000000067130A70
  0000000067130A66: 48 8B CB           mov         rcx,rbx
  0000000067130A69: E8 C2 50 EF FF     call        0000000067025B30
  0000000067130A6E: EB 14              jmp         0000000067130A84
  0000000067130A70: 41 8B C5           mov         eax,r13d
  0000000067130A73: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  0000000067130A78: 74 0A              je          0000000067130A84
  0000000067130A7A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067130A7F: E8 EC 50 EF FF     call        0000000067025B70
  0000000067130A84: B0 01              mov         al,1
  0000000067130A86: 48 83 C4 50        add         rsp,50h
  0000000067130A8A: 41 5F              pop         r15
  0000000067130A8C: 41 5E              pop         r14
  0000000067130A8E: 41 5D              pop         r13
  0000000067130A90: 41 5C              pop         r12
  0000000067130A92: 5F                 pop         rdi
  0000000067130A93: 5E                 pop         rsi
  0000000067130A94: 5B                 pop         rbx
  0000000067130A95: C3                 ret

  Summary

      1A7000 .text
