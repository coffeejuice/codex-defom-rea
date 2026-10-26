Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400EE400: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  00000001400EE405: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400EE40A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400EE40F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400EE414: 53                 push        rbx
  00000001400EE415: 48 81 EC F0 03 00  sub         rsp,3F0h
                    00
  00000001400EE41C: 48 C7 84 24 C8 02  mov         qword ptr [rsp+00000000000002C8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400EE428: 0F 29 B4 24 E0 03  movaps      xmmword ptr [rsp+00000000000003E0h],xmm6
                    00 00
  00000001400EE430: 0F 29 BC 24 D0 03  movaps      xmmword ptr [rsp+00000000000003D0h],xmm7
                    00 00
  00000001400EE438: C7 84 24 C4 02 00  mov         dword ptr [rsp+00000000000002C4h],0
                    00 00 00 00 00
  00000001400EE443: C6 44 24 51 00     mov         byte ptr [rsp+51h],0
  00000001400EE448: C6 44 24 50 00     mov         byte ptr [rsp+50h],0
  00000001400EE44D: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EE455: 48 8B 40 50        mov         rax,qword ptr [rax+50h]
  00000001400EE459: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EE461: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400EE465: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EE468: 8B 94 24 18 04 00  mov         edx,dword ptr [rsp+0000000000000418h]
                    00
  00000001400EE46F: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  00000001400EE475: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400EE47A: 48 83 BC 24 28 04  cmp         qword ptr [rsp+0000000000000428h],0
                    00 00 00
  00000001400EE483: 74 14              je          00000001400EE499
  00000001400EE485: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EE48D: E8 FE 9C FE FF     call        00000001400D8190
  00000001400EE492: 0F B6 C0           movzx       eax,al
  00000001400EE495: 85 C0              test        eax,eax
  00000001400EE497: 75 15              jne         00000001400EE4AE
  00000001400EE499: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400EE49E: E8 5D 93 72 00     call        0000000140817800
  00000001400EE4A3: 0F B6 C0           movzx       eax,al
  00000001400EE4A6: 85 C0              test        eax,eax
  00000001400EE4A8: 0F 84 6E 08 00 00  je          00000001400EED1C
  00000001400EE4AE: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  00000001400EE4B3: 48 8D 15 55 90 09  lea         rdx,[000000014218750Fh]
                    02
  00000001400EE4BA: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001400EE4C2: FF 15 B8 5F D1 01  call        qword ptr [0000000141E04480h]
  00000001400EE4C8: 90                 nop
  00000001400EE4C9: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  00000001400EE4D1: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EE4D9: FF 15 E9 7F D1 01  call        qword ptr [0000000141E064C8h]
  00000001400EE4DF: 90                 nop
  00000001400EE4E0: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001400EE4E8: FF 15 F2 74 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE4EE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400EE4F3: E8 08 93 72 00     call        0000000140817800
  00000001400EE4F8: 0F B6 C0           movzx       eax,al
  00000001400EE4FB: 85 C0              test        eax,eax
  00000001400EE4FD: 0F 84 53 01 00 00  je          00000001400EE656
  00000001400EE503: B9 38 00 00 00     mov         ecx,38h
  00000001400EE508: E8 AF 05 38 01     call        000000014146EABC
  00000001400EE50D: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  00000001400EE515: 48 83 BC 24 20 01  cmp         qword ptr [rsp+0000000000000120h],0
                    00 00 00
  00000001400EE51E: 74 62              je          00000001400EE582
  00000001400EE520: 45 33 C0           xor         r8d,r8d
  00000001400EE523: 48 8D 15 B6 CB 09  lea         rdx,[000000014218B0E0h]
                    02
  00000001400EE52A: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001400EE532: E8 D9 16 01 00     call        00000001400FFC10
  00000001400EE537: 48 89 84 24 D0 02  mov         qword ptr [rsp+00000000000002D0h],rax
                    00 00
  00000001400EE53F: 48 8B 84 24 D0 02  mov         rax,qword ptr [rsp+00000000000002D0h]
                    00 00
  00000001400EE547: 48 89 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],rax
                    00 00
  00000001400EE54F: 8B 84 24 C4 02 00  mov         eax,dword ptr [rsp+00000000000002C4h]
                    00
  00000001400EE556: 83 C8 01           or          eax,1
  00000001400EE559: 89 84 24 C4 02 00  mov         dword ptr [rsp+00000000000002C4h],eax
                    00
  00000001400EE560: 45 33 C0           xor         r8d,r8d
  00000001400EE563: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  00000001400EE56B: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001400EE573: E8 38 17 01 00     call        00000001400FFCB0
  00000001400EE578: 48 89 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],rax
                    00 00
  00000001400EE580: EB 0C              jmp         00000001400EE58E
  00000001400EE582: 48 C7 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],0
                    00 00 00 00 00 00
  00000001400EE58E: 48 8B 84 24 E0 02  mov         rax,qword ptr [rsp+00000000000002E0h]
                    00 00
  00000001400EE596: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001400EE59E: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001400EE5A6: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400EE5AB: 8B 84 24 C4 02 00  mov         eax,dword ptr [rsp+00000000000002C4h]
                    00
  00000001400EE5B2: 83 E0 01           and         eax,1
  00000001400EE5B5: 85 C0              test        eax,eax
  00000001400EE5B7: 74 16              je          00000001400EE5CF
  00000001400EE5B9: 83 A4 24 C4 02 00  and         dword ptr [rsp+00000000000002C4h],0FFFFFFFEh
                    00 FE
  00000001400EE5C1: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001400EE5C9: FF 15 11 74 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE5CF: 33 D2              xor         edx,edx
  00000001400EE5D1: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400EE5D6: E8 D5 92 72 00     call        00000001408178B0
  00000001400EE5DB: 0F B6 C0           movzx       eax,al
  00000001400EE5DE: 85 C0              test        eax,eax
  00000001400EE5E0: 75 05              jne         00000001400EE5E7
  00000001400EE5E2: C6 44 24 60 00     mov         byte ptr [rsp+60h],0
  00000001400EE5E7: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE5EC: 8B 84 24 18 04 00  mov         eax,dword ptr [rsp+0000000000000418h]
                    00
  00000001400EE5F3: 89 41 30           mov         dword ptr [rcx+30h],eax
  00000001400EE5F6: 48 8D 0D BB 80 C8  lea         rcx,[0000000143D766B8h]
                    03
  00000001400EE5FD: E8 9E D1 1D 00     call        00000001402CB7A0
  00000001400EE602: 83 F8 05           cmp         eax,5
  00000001400EE605: 74 0F              je          00000001400EE616
  00000001400EE607: 33 D2              xor         edx,edx
  00000001400EE609: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE60E: FF 15 2C D4 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EE614: EB 0D              jmp         00000001400EE623
  00000001400EE616: B2 01              mov         dl,1
  00000001400EE618: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE61D: FF 15 1D D4 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EE623: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EE62B: 4C 8D 0D C6 CA 09  lea         r9,[000000014218B0F8h]
                    02
  00000001400EE632: 4C 8B 84 24 00 04  mov         r8,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EE63A: 48 8D 15 CF CA 09  lea         rdx,[000000014218B110h]
                    02
  00000001400EE641: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE646: FF 15 84 5B D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EE64C: C6 44 24 51 01     mov         byte ptr [rsp+51h],1
  00000001400EE651: E9 65 01 00 00     jmp         00000001400EE7BB
  00000001400EE656: B9 38 00 00 00     mov         ecx,38h
  00000001400EE65B: E8 5C 04 38 01     call        000000014146EABC
  00000001400EE660: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001400EE668: 48 83 BC 24 38 01  cmp         qword ptr [rsp+0000000000000138h],0
                    00 00 00
  00000001400EE671: 74 62              je          00000001400EE6D5
  00000001400EE673: 45 33 C0           xor         r8d,r8d
  00000001400EE676: 48 8D 15 A3 CA 09  lea         rdx,[000000014218B120h]
                    02
  00000001400EE67D: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000001400EE685: E8 86 15 01 00     call        00000001400FFC10
  00000001400EE68A: 48 89 84 24 E8 02  mov         qword ptr [rsp+00000000000002E8h],rax
                    00 00
  00000001400EE692: 48 8B 84 24 E8 02  mov         rax,qword ptr [rsp+00000000000002E8h]
                    00 00
  00000001400EE69A: 48 89 84 24 F0 02  mov         qword ptr [rsp+00000000000002F0h],rax
                    00 00
  00000001400EE6A2: 8B 84 24 C4 02 00  mov         eax,dword ptr [rsp+00000000000002C4h]
                    00
  00000001400EE6A9: 83 C8 02           or          eax,2
  00000001400EE6AC: 89 84 24 C4 02 00  mov         dword ptr [rsp+00000000000002C4h],eax
                    00
  00000001400EE6B3: 45 33 C0           xor         r8d,r8d
  00000001400EE6B6: 48 8B 94 24 F0 02  mov         rdx,qword ptr [rsp+00000000000002F0h]
                    00 00
  00000001400EE6BE: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001400EE6C6: E8 E5 15 01 00     call        00000001400FFCB0
  00000001400EE6CB: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  00000001400EE6D3: EB 0C              jmp         00000001400EE6E1
  00000001400EE6D5: 48 C7 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],0
                    00 00 00 00 00 00
  00000001400EE6E1: 48 8B 84 24 F8 02  mov         rax,qword ptr [rsp+00000000000002F8h]
                    00 00
  00000001400EE6E9: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  00000001400EE6F1: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001400EE6F9: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400EE6FE: 8B 84 24 C4 02 00  mov         eax,dword ptr [rsp+00000000000002C4h]
                    00
  00000001400EE705: 83 E0 02           and         eax,2
  00000001400EE708: 85 C0              test        eax,eax
  00000001400EE70A: 74 16              je          00000001400EE722
  00000001400EE70C: 83 A4 24 C4 02 00  and         dword ptr [rsp+00000000000002C4h],0FFFFFFFDh
                    00 FD
  00000001400EE714: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000001400EE71C: FF 15 BE 72 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE722: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EE72A: E8 91 66 42 00     call        0000000140514DC0
  00000001400EE72F: 0F B6 C0           movzx       eax,al
  00000001400EE732: 85 C0              test        eax,eax
  00000001400EE734: 74 14              je          00000001400EE74A
  00000001400EE736: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EE73E: E8 BD 27 FE FF     call        00000001400D0F00
  00000001400EE743: 0F B6 C0           movzx       eax,al
  00000001400EE746: 85 C0              test        eax,eax
  00000001400EE748: 75 05              jne         00000001400EE74F
  00000001400EE74A: C6 44 24 60 00     mov         byte ptr [rsp+60h],0
  00000001400EE74F: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE754: 48 8B 84 24 28 04  mov         rax,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EE75C: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  00000001400EE760: 48 8D 0D 51 7F C8  lea         rcx,[0000000143D766B8h]
                    03
  00000001400EE767: E8 34 D0 1D 00     call        00000001402CB7A0
  00000001400EE76C: 83 F8 05           cmp         eax,5
  00000001400EE76F: 74 0F              je          00000001400EE780
  00000001400EE771: 33 D2              xor         edx,edx
  00000001400EE773: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE778: FF 15 C2 D2 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EE77E: EB 0D              jmp         00000001400EE78D
  00000001400EE780: B2 01              mov         dl,1
  00000001400EE782: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE787: FF 15 B3 D2 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EE78D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EE795: 4C 8D 0D 9C C9 09  lea         r9,[000000014218B138h]
                    02
  00000001400EE79C: 4C 8B 84 24 00 04  mov         r8,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EE7A4: 48 8D 15 A5 C9 09  lea         rdx,[000000014218B150h]
                    02
  00000001400EE7AB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE7B0: FF 15 1A 5A D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EE7B6: C6 44 24 50 01     mov         byte ptr [rsp+50h],1
  00000001400EE7BB: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400EE7C0: FF 15 C2 CE D1 01  call        qword ptr [0000000141E0B688h]
  00000001400EE7C6: 90                 nop
  00000001400EE7C7: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  00000001400EE7CF: FF 15 D3 71 D1 01  call        qword ptr [0000000141E059A8h]
  00000001400EE7D5: 48 8B D8           mov         rbx,rax
  00000001400EE7D8: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001400EE7DE: 48 8D 15 7B C9 09  lea         rdx,[000000014218B160h]
                    02
  00000001400EE7E5: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400EE7ED: FF 15 5D 59 D1 01  call        qword ptr [0000000141E04150h]
  00000001400EE7F3: 48 89 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],rax
                    00 00
  00000001400EE7FB: 48 8B 84 24 00 03  mov         rax,qword ptr [rsp+0000000000000300h]
                    00 00
  00000001400EE803: 48 89 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],rax
                    00 00
  00000001400EE80B: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001400EE813: 45 33 C9           xor         r9d,r9d
  00000001400EE816: 4C 8B C3           mov         r8,rbx
  00000001400EE819: 48 8B 94 24 08 03  mov         rdx,qword ptr [rsp+0000000000000308h]
                    00 00
  00000001400EE821: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400EE826: FF 15 64 CE D1 01  call        qword ptr [0000000141E0B690h]
  00000001400EE82C: 90                 nop
  00000001400EE82D: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400EE835: FF 15 A5 71 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE83B: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000001400EE840: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE845: FF 15 35 8A D1 01  call        qword ptr [0000000141E07280h]
  00000001400EE84B: BA 6C 00 00 00     mov         edx,6Ch
  00000001400EE850: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE855: FF 15 CD D1 D1 01  call        qword ptr [0000000141E0BA28h]
  00000001400EE85B: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  00000001400EE860: 85 C0              test        eax,eax
  00000001400EE862: 0F 85 82 01 00 00  jne         00000001400EE9EA
  00000001400EE868: 48 8D 94 24 58 01  lea         rdx,[rsp+0000000000000158h]
                    00 00
  00000001400EE870: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE875: FF 15 FD CB D1 01  call        qword ptr [0000000141E0B478h]
  00000001400EE87B: 48 8B D0           mov         rdx,rax
  00000001400EE87E: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400EE883: FF 15 6F 5E D1 01  call        qword ptr [0000000141E046F8h]
  00000001400EE889: B2 20              mov         dl,20h
  00000001400EE88B: 48 8D 8C 24 82 01  lea         rcx,[rsp+0000000000000182h]
                    00 00
  00000001400EE893: E8 C8 3F F1 FF     call        0000000140002860
  00000001400EE898: 0F B6 10           movzx       edx,byte ptr [rax]
  00000001400EE89B: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000001400EE8A3: FF 15 AF 5A D1 01  call        qword ptr [0000000141E04358h]
  00000001400EE8A9: B2 20              mov         dl,20h
  00000001400EE8AB: 48 8D 8C 24 72 01  lea         rcx,[rsp+0000000000000172h]
                    00 00
  00000001400EE8B3: E8 A8 3F F1 FF     call        0000000140002860
  00000001400EE8B8: 0F B6 10           movzx       edx,byte ptr [rax]
  00000001400EE8BB: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001400EE8C3: FF 15 8F 5A D1 01  call        qword ptr [0000000141E04358h]
  00000001400EE8C9: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400EE8CE: FF 15 B4 5E D1 01  call        qword ptr [0000000141E04788h]
  00000001400EE8D4: 66 0F 28 F8        movapd      xmm7,xmm0
  00000001400EE8D8: F2 0F 5C 3D 40 2C  subsd       xmm7,mmword ptr [0000000142101520h]
                    01 02
  00000001400EE8E0: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400EE8E5: FF 15 95 5E D1 01  call        qword ptr [0000000141E04780h]
  00000001400EE8EB: 66 0F 28 F0        movapd      xmm6,xmm0
  00000001400EE8EF: F2 0F 5C 35 29 2C  subsd       xmm6,mmword ptr [0000000142101520h]
                    01 02
  00000001400EE8F7: 48 8D 15 92 C8 09  lea         rdx,[000000014218B190h]
                    02
  00000001400EE8FE: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  00000001400EE906: FF 15 74 5B D1 01  call        qword ptr [0000000141E04480h]
  00000001400EE90C: 48 89 84 24 10 03  mov         qword ptr [rsp+0000000000000310h],rax
                    00 00
  00000001400EE914: 48 8B 84 24 10 03  mov         rax,qword ptr [rsp+0000000000000310h]
                    00 00
  00000001400EE91C: 48 89 84 24 18 03  mov         qword ptr [rsp+0000000000000318h],rax
                    00 00
  00000001400EE924: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  00000001400EE92C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400EE931: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001400EE939: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  00000001400EE93E: 45 33 C9           xor         r9d,r9d
  00000001400EE941: 66 0F 28 D6        movapd      xmm2,xmm6
  00000001400EE945: 48 8D 94 24 78 01  lea         rdx,[rsp+0000000000000178h]
                    00 00
  00000001400EE94D: 48 8B 8C 24 18 03  mov         rcx,qword ptr [rsp+0000000000000318h]
                    00 00
  00000001400EE955: FF 15 B5 5D D1 01  call        qword ptr [0000000141E04710h]
  00000001400EE95B: 48 89 84 24 20 03  mov         qword ptr [rsp+0000000000000320h],rax
                    00 00
  00000001400EE963: 48 8B 84 24 20 03  mov         rax,qword ptr [rsp+0000000000000320h]
                    00 00
  00000001400EE96B: 48 89 84 24 28 03  mov         qword ptr [rsp+0000000000000328h],rax
                    00 00
  00000001400EE973: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  00000001400EE97B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400EE980: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001400EE988: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  00000001400EE98D: 45 33 C9           xor         r9d,r9d
  00000001400EE990: 66 0F 28 D7        movapd      xmm2,xmm7
  00000001400EE994: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000001400EE99C: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  00000001400EE9A4: FF 15 66 5D D1 01  call        qword ptr [0000000141E04710h]
  00000001400EE9AA: 90                 nop
  00000001400EE9AB: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  00000001400EE9B3: FF 15 27 70 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE9B9: 90                 nop
  00000001400EE9BA: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  00000001400EE9C2: FF 15 18 70 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE9C8: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000001400EE9D0: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EE9D5: FF 15 A5 CA D1 01  call        qword ptr [0000000141E0B480h]
  00000001400EE9DB: 90                 nop
  00000001400EE9DC: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400EE9E4: FF 15 F6 6F D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EE9EA: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400EE9EF: E8 0C 8E 72 00     call        0000000140817800
  00000001400EE9F4: 0F B6 C0           movzx       eax,al
  00000001400EE9F7: 85 C0              test        eax,eax
  00000001400EE9F9: 0F 85 E1 02 00 00  jne         00000001400EECE0
  00000001400EE9FF: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000001400EEA07: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEA0F: E8 7C 27 83 00     call        0000000140921190
  00000001400EEA14: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001400EEA1B: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  00000001400EEA24: 0F 85 B6 02 00 00  jne         00000001400EECE0
  00000001400EEA2A: 48 8D 94 24 88 01  lea         rdx,[rsp+0000000000000188h]
                    00 00
  00000001400EEA32: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEA3A: E8 A1 23 FE FF     call        00000001400D0DE0
  00000001400EEA3F: 48 89 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],rax
                    00 00
  00000001400EEA47: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400EEA4F: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  00000001400EEA57: 33 D2              xor         edx,edx
  00000001400EEA59: 48 8B 8C 24 38 03  mov         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  00000001400EEA61: E8 5A 66 F3 FF     call        00000001400250C0
  00000001400EEA66: 48 8B D8           mov         rbx,rax
  00000001400EEA69: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EEA71: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400EEA75: E8 E6 11 01 00     call        00000001400FFC60
  00000001400EEA7A: 48 8B C8           mov         rcx,rax
  00000001400EEA7D: E8 BE 71 FC FF     call        00000001400B5C40
  00000001400EEA82: 48 8B C8           mov         rcx,rax
  00000001400EEA85: E8 26 6E FB FF     call        00000001400A58B0
  00000001400EEA8A: 48 8B D3           mov         rdx,rbx
  00000001400EEA8D: 48 8B C8           mov         rcx,rax
  00000001400EEA90: E8 2B 93 F5 FF     call        0000000140047DC0
  00000001400EEA95: 48 8B C8           mov         rcx,rax
  00000001400EEA98: E8 D3 0C F7 FF     call        000000014005F770
  00000001400EEA9D: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400EEAA5: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  00000001400EEAAD: E8 EE 96 11 00     call        00000001402081A0
  00000001400EEAB2: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400EEABA: E8 11 C2 B8 00     call        0000000140C7ACD0
  00000001400EEABF: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400EEAC7: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  00000001400EEAD0: 0F 84 F2 01 00 00  je          00000001400EECC8
  00000001400EEAD6: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  00000001400EEADE: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEAE6: E8 35 23 FE FF     call        00000001400D0E20
  00000001400EEAEB: 90                 nop
  00000001400EEAEC: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400EEAF4: E8 27 9D 9A 00     call        0000000140A98820
  00000001400EEAF9: 90                 nop
  00000001400EEAFA: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400EEB02: E8 D9 5F F1 FF     call        0000000140004AE0
  00000001400EEB07: 89 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],eax
                    00
  00000001400EEB0E: C7 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00
  00000001400EEB19: EB 11              jmp         00000001400EEB2C
  00000001400EEB1B: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  00000001400EEB22: 83 C0 01           add         eax,1
  00000001400EEB25: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  00000001400EEB2C: 8B 84 24 C8 00 00  mov         eax,dword ptr [rsp+00000000000000C8h]
                    00
  00000001400EEB33: 39 84 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],eax
                    00
  00000001400EEB3A: 7D 2E              jge         00000001400EEB6A
  00000001400EEB3C: 8B 94 24 D8 00 00  mov         edx,dword ptr [rsp+00000000000000D8h]
                    00
  00000001400EEB43: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400EEB4B: E8 70 65 F3 FF     call        00000001400250C0
  00000001400EEB50: 4C 8B C0           mov         r8,rax
  00000001400EEB53: 48 8D 94 24 90 01  lea         rdx,[rsp+0000000000000190h]
                    00 00
  00000001400EEB5B: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400EEB63: E8 58 D1 F7 FF     call        000000014006BCC0
  00000001400EEB68: EB B1              jmp         00000001400EEB1B
  00000001400EEB6A: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  00000001400EEB76: 48 8D 84 24 98 01  lea         rax,[rsp+0000000000000198h]
                    00 00
  00000001400EEB7E: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001400EEB86: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  00000001400EEB8E: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001400EEB96: E8 65 8C 82 00     call        0000000140917800
  00000001400EEB9B: 48 89 84 24 40 03  mov         qword ptr [rsp+0000000000000340h],rax
                    00 00
  00000001400EEBA3: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  00000001400EEBAB: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  00000001400EEBB3: 48 8D 94 24 A8 01  lea         rdx,[rsp+00000000000001A8h]
                    00 00
  00000001400EEBBB: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEBC3: E8 F8 5B 21 00     call        00000001403047C0
  00000001400EEBC8: 48 89 84 24 50 03  mov         qword ptr [rsp+0000000000000350h],rax
                    00 00
  00000001400EEBD0: 48 8B 84 24 50 03  mov         rax,qword ptr [rsp+0000000000000350h]
                    00 00
  00000001400EEBD8: 48 89 84 24 58 03  mov         qword ptr [rsp+0000000000000358h],rax
                    00 00
  00000001400EEBE0: 48 8D 94 24 B0 01  lea         rdx,[rsp+00000000000001B0h]
                    00 00
  00000001400EEBE8: 48 8B 8C 24 58 03  mov         rcx,qword ptr [rsp+0000000000000358h]
                    00 00
  00000001400EEBF0: FF 15 5A 53 D1 01  call        qword ptr [0000000141E03F50h]
  00000001400EEBF6: 48 89 84 24 60 03  mov         qword ptr [rsp+0000000000000360h],rax
                    00 00
  00000001400EEBFE: 48 8B 84 24 60 03  mov         rax,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001400EEC06: 48 89 84 24 68 03  mov         qword ptr [rsp+0000000000000368h],rax
                    00 00
  00000001400EEC0E: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  00000001400EEC16: FF 15 0C 56 D1 01  call        qword ptr [0000000141E04228h]
  00000001400EEC1C: 48 8B D8           mov         rbx,rax
  00000001400EEC1F: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEC27: E8 64 F8 77 00     call        000000014086E490
  00000001400EEC2C: 8B C8              mov         ecx,eax
  00000001400EEC2E: 48 8D 84 24 B8 00  lea         rax,[rsp+00000000000000B8h]
                    00 00
  00000001400EEC36: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400EEC3B: 4C 8B 8C 24 A8 00  mov         r9,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400EEC43: 4C 8B 84 24 48 03  mov         r8,qword ptr [rsp+0000000000000348h]
                    00 00
  00000001400EEC4B: 48 8B D3           mov         rdx,rbx
  00000001400EEC4E: E8 2D 36 83 00     call        0000000140922280
  00000001400EEC53: 90                 nop
  00000001400EEC54: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  00000001400EEC5C: FF 15 2E 56 D1 01  call        qword ptr [0000000141E04290h]
  00000001400EEC62: 90                 nop
  00000001400EEC63: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  00000001400EEC6B: FF 15 6F 6D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EEC71: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400EEC79: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400EEC81: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  00000001400EEC8A: 74 21              je          00000001400EECAD
  00000001400EEC8C: 45 33 C9           xor         r9d,r9d
  00000001400EEC8F: 44 8B 84 24 98 00  mov         r8d,dword ptr [rsp+0000000000000098h]
                    00 00
  00000001400EEC97: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400EEC9F: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EECA7: E8 24 24 83 00     call        00000001409210D0
  00000001400EECAC: 90                 nop
  00000001400EECAD: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400EECB5: E8 36 DA 7F 00     call        00000001408EC6F0
  00000001400EECBA: 90                 nop
  00000001400EECBB: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400EECC3: E8 D8 94 11 00     call        00000001402081A0
  00000001400EECC8: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  00000001400EECD1: 75 0D              jne         00000001400EECE0
  00000001400EECD3: 33 D2              xor         edx,edx
  00000001400EECD5: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400EECDA: FF 15 60 CD D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400EECE0: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EECE7: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EECEF: FF 15 DB 77 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EECF5: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  00000001400EECFA: 48 8B D0           mov         rdx,rax
  00000001400EECFD: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EED05: FF 15 B5 77 D1 01  call        qword ptr [0000000141E064C0h]
  00000001400EED0B: 90                 nop
  00000001400EED0C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400EED11: FF 15 89 C9 D1 01  call        qword ptr [0000000141E0B6A0h]
  00000001400EED17: E9 BB 05 00 00     jmp         00000001400EF2D7
  00000001400EED1C: 48 83 BC 24 28 04  cmp         qword ptr [rsp+0000000000000428h],0
                    00 00 00
  00000001400EED25: 0F 84 F6 02 00 00  je          00000001400EF021
  00000001400EED2B: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EED33: E8 48 0F 01 00     call        00000001400FFC80
  00000001400EED38: 0F B6 C0           movzx       eax,al
  00000001400EED3B: 85 C0              test        eax,eax
  00000001400EED3D: 0F 84 DE 02 00 00  je          00000001400EF021
  00000001400EED43: 48 8D 15 D0 87 09  lea         rdx,[000000014218751Ah]
                    02
  00000001400EED4A: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  00000001400EED52: FF 15 28 57 D1 01  call        qword ptr [0000000141E04480h]
  00000001400EED58: 90                 nop
  00000001400EED59: 48 8D 94 24 B8 01  lea         rdx,[rsp+00000000000001B8h]
                    00 00
  00000001400EED61: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EED69: FF 15 59 77 D1 01  call        qword ptr [0000000141E064C8h]
  00000001400EED6F: 90                 nop
  00000001400EED70: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  00000001400EED78: FF 15 62 6C D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EED7E: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400EED86: E8 55 66 F7 FF     call        00000001400653E0
  00000001400EED8B: 90                 nop
  00000001400EED8C: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  00000001400EED94: E8 47 0C 0A 00     call        000000014018F9E0
  00000001400EED99: 90                 nop
  00000001400EED9A: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  00000001400EEDA2: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  00000001400EEDAA: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEDB2: E8 89 27 83 00     call        0000000140921540
  00000001400EEDB7: B9 28 00 00 00     mov         ecx,28h
  00000001400EEDBC: E8 FB FC 37 01     call        000000014146EABC
  00000001400EEDC1: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  00000001400EEDC9: 48 83 BC 24 C8 01  cmp         qword ptr [rsp+00000000000001C8h],0
                    00 00 00
  00000001400EEDD2: 74 47              je          00000001400EEE1B
  00000001400EEDD4: 33 D2              xor         edx,edx
  00000001400EEDD6: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400EEDDE: FF 15 9C 7F D1 01  call        qword ptr [0000000141E06D80h]
  00000001400EEDE4: 4C 8B 9C 24 C8 01  mov         r11,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400EEDEC: 48 8D 05 B5 D9 09  lea         rax,[000000014218C7A8h]
                    02
  00000001400EEDF3: 49 89 03           mov         qword ptr [r11],rax
  00000001400EEDF6: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400EEDFE: 48 8D 05 6B D9 09  lea         rax,[000000014218C770h]
                    02
  00000001400EEE05: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001400EEE09: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400EEE11: 48 89 84 24 70 03  mov         qword ptr [rsp+0000000000000370h],rax
                    00 00
  00000001400EEE19: EB 0C              jmp         00000001400EEE27
  00000001400EEE1B: 48 C7 84 24 70 03  mov         qword ptr [rsp+0000000000000370h],0
                    00 00 00 00 00 00
  00000001400EEE27: 48 8B 84 24 70 03  mov         rax,qword ptr [rsp+0000000000000370h]
                    00 00
  00000001400EEE2F: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001400EEE37: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001400EEE3F: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001400EEE47: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  00000001400EEE4F: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEE57: FF 15 53 76 D1 01  call        qword ptr [0000000141E064B0h]
  00000001400EEE5D: BA 04 00 00 00     mov         edx,4
  00000001400EEE62: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  00000001400EEE6A: FF 15 28 6B D1 01  call        qword ptr [0000000141E05998h]
  00000001400EEE70: 90                 nop
  00000001400EEE71: 41 B9 24 00 00 00  mov         r9d,24h
  00000001400EEE77: 4C 8D 84 24 D0 01  lea         r8,[rsp+00000000000001D0h]
                    00 00
  00000001400EEE7F: 33 D2              xor         edx,edx
  00000001400EEE81: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEE89: FF 15 29 76 D1 01  call        qword ptr [0000000141E064B8h]
  00000001400EEE8F: 90                 nop
  00000001400EEE90: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  00000001400EEE98: FF 15 1A 6B D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EEE9E: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EEEA5: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  00000001400EEEAD: FF 15 E5 6A D1 01  call        qword ptr [0000000141E05998h]
  00000001400EEEB3: 90                 nop
  00000001400EEEB4: 41 B9 21 00 00 00  mov         r9d,21h
  00000001400EEEBA: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  00000001400EEEC2: 33 D2              xor         edx,edx
  00000001400EEEC4: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEECC: FF 15 E6 75 D1 01  call        qword ptr [0000000141E064B8h]
  00000001400EEED2: 90                 nop
  00000001400EEED3: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  00000001400EEEDB: FF 15 D7 6A D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EEEE1: 48 8B 94 24 20 04  mov         rdx,qword ptr [rsp+0000000000000420h]
                    00 00
  00000001400EEEE9: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  00000001400EEEF1: FF 15 A9 6A D1 01  call        qword ptr [0000000141E059A0h]
  00000001400EEEF7: 90                 nop
  00000001400EEEF8: 41 B9 23 00 00 00  mov         r9d,23h
  00000001400EEEFE: 4C 8D 84 24 F0 01  lea         r8,[rsp+00000000000001F0h]
                    00 00
  00000001400EEF06: 33 D2              xor         edx,edx
  00000001400EEF08: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEF10: FF 15 A2 75 D1 01  call        qword ptr [0000000141E064B8h]
  00000001400EEF16: 90                 nop
  00000001400EEF17: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  00000001400EEF1F: FF 15 93 6A D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EEF25: 33 D2              xor         edx,edx
  00000001400EEF27: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  00000001400EEF2F: FF 15 63 6A D1 01  call        qword ptr [0000000141E05998h]
  00000001400EEF35: 90                 nop
  00000001400EEF36: 41 B9 22 00 00 00  mov         r9d,22h
  00000001400EEF3C: 4C 8D 84 24 00 02  lea         r8,[rsp+0000000000000200h]
                    00 00
  00000001400EEF44: 33 D2              xor         edx,edx
  00000001400EEF46: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEF4E: FF 15 64 75 D1 01  call        qword ptr [0000000141E064B8h]
  00000001400EEF54: 90                 nop
  00000001400EEF55: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  00000001400EEF5D: FF 15 55 6A D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EEF63: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400EEF6B: 4C 8D 0D 9E C2 09  lea         r9,[000000014218B210h]
                    02
  00000001400EEF72: 4C 8B 84 24 00 04  mov         r8,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EEF7A: 48 8D 15 AF C2 09  lea         rdx,[000000014218B230h]
                    02
  00000001400EEF81: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEF89: FF 15 41 52 D1 01  call        qword ptr [0000000141E041D0h]
  00000001400EEF8F: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EEF96: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EEF9E: FF 15 2C 75 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EEFA4: 4C 8B 84 24 F0 00  mov         r8,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEFAC: 48 8B D0           mov         rdx,rax
  00000001400EEFAF: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EEFB7: FF 15 03 75 D1 01  call        qword ptr [0000000141E064C0h]
  00000001400EEFBD: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEFC5: E8 66 1F FE FF     call        00000001400D0F30
  00000001400EEFCA: 0F B6 C0           movzx       eax,al
  00000001400EEFCD: 85 C0              test        eax,eax
  00000001400EEFCF: 75 12              jne         00000001400EEFE3
  00000001400EEFD1: 33 D2              xor         edx,edx
  00000001400EEFD3: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEFDB: FF 15 E7 C4 D1 01  call        qword ptr [0000000141E0B4C8h]
  00000001400EEFE1: EB 1E              jmp         00000001400EF001
  00000001400EEFE3: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EEFEB: E8 90 27 83 00     call        0000000140921780
  00000001400EEFF0: 8B D0              mov         edx,eax
  00000001400EEFF2: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400EEFFA: FF 15 C8 C4 D1 01  call        qword ptr [0000000141E0B4C8h]
  00000001400EF000: 90                 nop
  00000001400EF001: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  00000001400EF009: E8 92 91 11 00     call        00000001402081A0
  00000001400EF00E: 90                 nop
  00000001400EF00F: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400EF017: E8 B4 58 F1 FF     call        00000001400048D0
  00000001400EF01C: E9 B6 02 00 00     jmp         00000001400EF2D7
  00000001400EF021: 48 83 BC 24 28 04  cmp         qword ptr [rsp+0000000000000428h],0
                    00 00 00
  00000001400EF02A: 0F 84 C8 00 00 00  je          00000001400EF0F8
  00000001400EF030: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EF038: E8 B3 1E FE FF     call        00000001400D0EF0
  00000001400EF03D: 0F B6 C0           movzx       eax,al
  00000001400EF040: 85 C0              test        eax,eax
  00000001400EF042: 0F 85 B0 00 00 00  jne         00000001400EF0F8
  00000001400EF048: 48 8D 15 CC 84 09  lea         rdx,[000000014218751Bh]
                    02
  00000001400EF04F: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  00000001400EF057: FF 15 23 54 D1 01  call        qword ptr [0000000141E04480h]
  00000001400EF05D: 90                 nop
  00000001400EF05E: 48 8D 94 24 10 02  lea         rdx,[rsp+0000000000000210h]
                    00 00
  00000001400EF066: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF06E: FF 15 54 74 D1 01  call        qword ptr [0000000141E064C8h]
  00000001400EF074: 90                 nop
  00000001400EF075: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  00000001400EF07D: FF 15 5D 69 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF083: 48 8D 84 24 18 02  lea         rax,[rsp+0000000000000218h]
                    00 00
  00000001400EF08B: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  00000001400EF093: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF09A: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF0A2: FF 15 28 74 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF0A8: 48 8D 94 24 28 02  lea         rdx,[rsp+0000000000000228h]
                    00 00
  00000001400EF0B0: 48 8B C8           mov         rcx,rax
  00000001400EF0B3: FF 15 27 74 D1 01  call        qword ptr [0000000141E064E0h]
  00000001400EF0B9: 41 B8 02 00 00 00  mov         r8d,2
  00000001400EF0BF: 48 8B 94 24 20 02  mov         rdx,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001400EF0C7: 48 8B C8           mov         rcx,rax
  00000001400EF0CA: E8 01 28 F4 FF     call        00000001400318D0
  00000001400EF0CF: 48 8B D8           mov         rbx,rax
  00000001400EF0D2: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF0D9: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF0E1: FF 15 E9 73 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF0E7: 48 8B D3           mov         rdx,rbx
  00000001400EF0EA: 48 8B C8           mov         rcx,rax
  00000001400EF0ED: FF 15 F5 73 D1 01  call        qword ptr [0000000141E064E8h]
  00000001400EF0F3: E9 DF 01 00 00     jmp         00000001400EF2D7
  00000001400EF0F8: 48 83 BC 24 28 04  cmp         qword ptr [rsp+0000000000000428h],0
                    00 00 00
  00000001400EF101: 0F 84 D0 01 00 00  je          00000001400EF2D7
  00000001400EF107: B2 20              mov         dl,20h
  00000001400EF109: 48 8D 8C 24 4A 02  lea         rcx,[rsp+000000000000024Ah]
                    00 00
  00000001400EF111: E8 4A 37 F1 FF     call        0000000140002860
  00000001400EF116: 0F B6 10           movzx       edx,byte ptr [rax]
  00000001400EF119: 48 8D 8C 24 48 02  lea         rcx,[rsp+0000000000000248h]
                    00 00
  00000001400EF121: FF 15 31 52 D1 01  call        qword ptr [0000000141E04358h]
  00000001400EF127: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EF12F: E8 6C 0B 01 00     call        00000001400FFCA0
  00000001400EF134: 8B C8              mov         ecx,eax
  00000001400EF136: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EF13E: 8B 50 58           mov         edx,dword ptr [rax+58h]
  00000001400EF141: E8 9A 8C 82 00     call        0000000140917DE0
  00000001400EF146: 48 8B D0           mov         rdx,rax
  00000001400EF149: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  00000001400EF151: FF 15 29 53 D1 01  call        qword ptr [0000000141E04480h]
  00000001400EF157: 90                 nop
  00000001400EF158: B2 20              mov         dl,20h
  00000001400EF15A: 48 8D 8C 24 3A 02  lea         rcx,[rsp+000000000000023Ah]
                    00 00
  00000001400EF162: E8 F9 36 F1 FF     call        0000000140002860
  00000001400EF167: 0F B6 10           movzx       edx,byte ptr [rax]
  00000001400EF16A: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  00000001400EF172: FF 15 E0 51 D1 01  call        qword ptr [0000000141E04358h]
  00000001400EF178: 48 8B 8C 24 28 04  mov         rcx,qword ptr [rsp+0000000000000428h]
                    00 00
  00000001400EF180: E8 EB FD F5 FF     call        000000014004EF70
  00000001400EF185: 66 0F 28 F0        movapd      xmm6,xmm0
  00000001400EF189: 48 8D 15 BC C0 09  lea         rdx,[000000014218B24Ch]
                    02
  00000001400EF190: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  00000001400EF198: FF 15 E2 52 D1 01  call        qword ptr [0000000141E04480h]
  00000001400EF19E: 48 89 84 24 78 03  mov         qword ptr [rsp+0000000000000378h],rax
                    00 00
  00000001400EF1A6: 48 8B 84 24 78 03  mov         rax,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400EF1AE: 48 89 84 24 80 03  mov         qword ptr [rsp+0000000000000380h],rax
                    00 00
  00000001400EF1B6: 48 8D 84 24 38 02  lea         rax,[rsp+0000000000000238h]
                    00 00
  00000001400EF1BE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400EF1C3: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001400EF1CB: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  00000001400EF1D0: 45 33 C9           xor         r9d,r9d
  00000001400EF1D3: 66 0F 28 D6        movapd      xmm2,xmm6
  00000001400EF1D7: 48 8D 94 24 40 02  lea         rdx,[rsp+0000000000000240h]
                    00 00
  00000001400EF1DF: 48 8B 8C 24 80 03  mov         rcx,qword ptr [rsp+0000000000000380h]
                    00 00
  00000001400EF1E7: FF 15 23 55 D1 01  call        qword ptr [0000000141E04710h]
  00000001400EF1ED: 48 89 84 24 88 03  mov         qword ptr [rsp+0000000000000388h],rax
                    00 00
  00000001400EF1F5: 48 8B 84 24 88 03  mov         rax,qword ptr [rsp+0000000000000388h]
                    00 00
  00000001400EF1FD: 48 89 84 24 90 03  mov         qword ptr [rsp+0000000000000390h],rax
                    00 00
  00000001400EF205: 48 8D 84 24 48 02  lea         rax,[rsp+0000000000000248h]
                    00 00
  00000001400EF20D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400EF212: 45 33 C9           xor         r9d,r9d
  00000001400EF215: 4C 8D 84 24 50 02  lea         r8,[rsp+0000000000000250h]
                    00 00
  00000001400EF21D: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  00000001400EF225: 48 8B 8C 24 90 03  mov         rcx,qword ptr [rsp+0000000000000390h]
                    00 00
  00000001400EF22D: FF 15 2D 51 D1 01  call        qword ptr [0000000141E04360h]
  00000001400EF233: 90                 nop
  00000001400EF234: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  00000001400EF23C: FF 15 9E 67 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF242: 90                 nop
  00000001400EF243: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  00000001400EF24B: FF 15 8F 67 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF251: 90                 nop
  00000001400EF252: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  00000001400EF25A: FF 15 80 67 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF260: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  00000001400EF268: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF270: FF 15 52 72 D1 01  call        qword ptr [0000000141E064C8h]
  00000001400EF276: 41 B8 01 00 00 00  mov         r8d,1
  00000001400EF27C: 48 8B 94 24 30 04  mov         rdx,qword ptr [rsp+0000000000000430h]
                    00 00
  00000001400EF284: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  00000001400EF28C: FF 15 86 C3 D1 01  call        qword ptr [0000000141E0B618h]
  00000001400EF292: 90                 nop
  00000001400EF293: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF29A: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF2A2: FF 15 28 72 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF2A8: 48 8D 94 24 58 02  lea         rdx,[rsp+0000000000000258h]
                    00 00
  00000001400EF2B0: 48 8B C8           mov         rcx,rax
  00000001400EF2B3: FF 15 1F 72 D1 01  call        qword ptr [0000000141E064D8h]
  00000001400EF2B9: 90                 nop
  00000001400EF2BA: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  00000001400EF2C2: FF 15 60 C3 D1 01  call        qword ptr [0000000141E0B628h]
  00000001400EF2C8: 90                 nop
  00000001400EF2C9: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400EF2D1: FF 15 09 67 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF2D7: BA 04 00 00 00     mov         edx,4
  00000001400EF2DC: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  00000001400EF2E4: FF 15 AE 66 D1 01  call        qword ptr [0000000141E05998h]
  00000001400EF2EA: 90                 nop
  00000001400EF2EB: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF2F2: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF2FA: FF 15 D0 71 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF300: 48 89 84 24 98 03  mov         qword ptr [rsp+0000000000000398h],rax
                    00 00
  00000001400EF308: 48 8B 84 24 98 03  mov         rax,qword ptr [rsp+0000000000000398h]
                    00 00
  00000001400EF310: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EF313: 4C 8D 84 24 60 02  lea         r8,[rsp+0000000000000260h]
                    00 00
  00000001400EF31B: BA 24 00 00 00     mov         edx,24h
  00000001400EF320: 48 8B 8C 24 98 03  mov         rcx,qword ptr [rsp+0000000000000398h]
                    00 00
  00000001400EF328: FF 50 20           call        qword ptr [rax+20h]
  00000001400EF32B: 90                 nop
  00000001400EF32C: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  00000001400EF334: FF 15 7E 66 D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EF33A: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF341: 48 8D 8C 24 70 02  lea         rcx,[rsp+0000000000000270h]
                    00 00
  00000001400EF349: FF 15 49 66 D1 01  call        qword ptr [0000000141E05998h]
  00000001400EF34F: 90                 nop
  00000001400EF350: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF357: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF35F: FF 15 6B 71 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF365: 48 89 84 24 A0 03  mov         qword ptr [rsp+00000000000003A0h],rax
                    00 00
  00000001400EF36D: 48 8B 84 24 A0 03  mov         rax,qword ptr [rsp+00000000000003A0h]
                    00 00
  00000001400EF375: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EF378: 4C 8D 84 24 70 02  lea         r8,[rsp+0000000000000270h]
                    00 00
  00000001400EF380: BA 21 00 00 00     mov         edx,21h
  00000001400EF385: 48 8B 8C 24 A0 03  mov         rcx,qword ptr [rsp+00000000000003A0h]
                    00 00
  00000001400EF38D: FF 50 20           call        qword ptr [rax+20h]
  00000001400EF390: 90                 nop
  00000001400EF391: 48 8D 8C 24 70 02  lea         rcx,[rsp+0000000000000270h]
                    00 00
  00000001400EF399: FF 15 19 66 D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EF39F: 48 8B 94 24 20 04  mov         rdx,qword ptr [rsp+0000000000000420h]
                    00 00
  00000001400EF3A7: 48 8D 8C 24 80 02  lea         rcx,[rsp+0000000000000280h]
                    00 00
  00000001400EF3AF: FF 15 EB 65 D1 01  call        qword ptr [0000000141E059A0h]
  00000001400EF3B5: 90                 nop
  00000001400EF3B6: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF3BD: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF3C5: FF 15 05 71 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF3CB: 48 89 84 24 A8 03  mov         qword ptr [rsp+00000000000003A8h],rax
                    00 00
  00000001400EF3D3: 48 8B 84 24 A8 03  mov         rax,qword ptr [rsp+00000000000003A8h]
                    00 00
  00000001400EF3DB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EF3DE: 4C 8D 84 24 80 02  lea         r8,[rsp+0000000000000280h]
                    00 00
  00000001400EF3E6: BA 23 00 00 00     mov         edx,23h
  00000001400EF3EB: 48 8B 8C 24 A8 03  mov         rcx,qword ptr [rsp+00000000000003A8h]
                    00 00
  00000001400EF3F3: FF 50 20           call        qword ptr [rax+20h]
  00000001400EF3F6: 90                 nop
  00000001400EF3F7: 48 8D 8C 24 80 02  lea         rcx,[rsp+0000000000000280h]
                    00 00
  00000001400EF3FF: FF 15 B3 65 D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EF405: 33 D2              xor         edx,edx
  00000001400EF407: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  00000001400EF40F: FF 15 83 65 D1 01  call        qword ptr [0000000141E05998h]
  00000001400EF415: 90                 nop
  00000001400EF416: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF41D: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF425: FF 15 A5 70 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF42B: 48 89 84 24 B0 03  mov         qword ptr [rsp+00000000000003B0h],rax
                    00 00
  00000001400EF433: 48 8B 84 24 B0 03  mov         rax,qword ptr [rsp+00000000000003B0h]
                    00 00
  00000001400EF43B: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EF43E: 4C 8D 84 24 90 02  lea         r8,[rsp+0000000000000290h]
                    00 00
  00000001400EF446: BA 22 00 00 00     mov         edx,22h
  00000001400EF44B: 48 8B 8C 24 B0 03  mov         rcx,qword ptr [rsp+00000000000003B0h]
                    00 00
  00000001400EF453: FF 50 20           call        qword ptr [rax+20h]
  00000001400EF456: 90                 nop
  00000001400EF457: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  00000001400EF45F: FF 15 53 65 D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EF465: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  00000001400EF46A: 85 C0              test        eax,eax
  00000001400EF46C: 75 16              jne         00000001400EF484
  00000001400EF46E: 0F B6 44 24 51     movzx       eax,byte ptr [rsp+51h]
  00000001400EF473: 85 C0              test        eax,eax
  00000001400EF475: 75 0D              jne         00000001400EF484
  00000001400EF477: C7 84 24 B8 03 00  mov         dword ptr [rsp+00000000000003B8h],0
                    00 00 00 00 00
  00000001400EF482: EB 0B              jmp         00000001400EF48F
  00000001400EF484: C7 84 24 B8 03 00  mov         dword ptr [rsp+00000000000003B8h],1
                    00 01 00 00 00
  00000001400EF48F: 8B 94 24 B8 03 00  mov         edx,dword ptr [rsp+00000000000003B8h]
                    00
  00000001400EF496: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  00000001400EF49E: FF 15 F4 64 D1 01  call        qword ptr [0000000141E05998h]
  00000001400EF4A4: 90                 nop
  00000001400EF4A5: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF4AC: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF4B4: FF 15 16 70 D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF4BA: 48 89 84 24 C0 03  mov         qword ptr [rsp+00000000000003C0h],rax
                    00 00
  00000001400EF4C2: 48 8B 84 24 C0 03  mov         rax,qword ptr [rsp+00000000000003C0h]
                    00 00
  00000001400EF4CA: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400EF4CD: 4C 8D 84 24 A0 02  lea         r8,[rsp+00000000000002A0h]
                    00 00
  00000001400EF4D5: BA 25 00 00 00     mov         edx,25h
  00000001400EF4DA: 48 8B 8C 24 C0 03  mov         rcx,qword ptr [rsp+00000000000003C0h]
                    00 00
  00000001400EF4E2: FF 50 20           call        qword ptr [rax+20h]
  00000001400EF4E5: 90                 nop
  00000001400EF4E6: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  00000001400EF4EE: FF 15 C4 64 D1 01  call        qword ptr [0000000141E059B8h]
  00000001400EF4F4: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  00000001400EF4FC: 48 89 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],rax
                    00 00
  00000001400EF504: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF50B: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF513: FF 15 B7 6F D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF519: 48 8D 94 24 C0 02  lea         rdx,[rsp+00000000000002C0h]
                    00 00
  00000001400EF521: 48 8B C8           mov         rcx,rax
  00000001400EF524: FF 15 B6 6F D1 01  call        qword ptr [0000000141E064E0h]
  00000001400EF52A: 41 B8 02 00 00 00  mov         r8d,2
  00000001400EF530: 48 8B 94 24 B8 02  mov         rdx,qword ptr [rsp+00000000000002B8h]
                    00 00
  00000001400EF538: 48 8B C8           mov         rcx,rax
  00000001400EF53B: E8 90 23 F4 FF     call        00000001400318D0
  00000001400EF540: 48 8B D8           mov         rbx,rax
  00000001400EF543: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF54A: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF552: FF 15 78 6F D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF558: 48 8B D3           mov         rdx,rbx
  00000001400EF55B: 48 8B C8           mov         rcx,rax
  00000001400EF55E: FF 15 84 6F D1 01  call        qword ptr [0000000141E064E8h]
  00000001400EF564: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EF56C: 8B 80 A4 00 00 00  mov         eax,dword ptr [rax+00000000000000A4h]
  00000001400EF572: 39 84 24 10 04 00  cmp         dword ptr [rsp+0000000000000410h],eax
                    00
  00000001400EF579: 0F 85 C2 00 00 00  jne         00000001400EF641
  00000001400EF57F: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EF587: 8B 80 A0 00 00 00  mov         eax,dword ptr [rax+00000000000000A0h]
  00000001400EF58D: 39 84 24 18 04 00  cmp         dword ptr [rsp+0000000000000418h],eax
                    00
  00000001400EF594: 0F 85 A7 00 00 00  jne         00000001400EF641
  00000001400EF59A: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF5A1: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF5A9: FF 15 21 6F D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF5AF: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  00000001400EF5B7: 48 8B C8           mov         rcx,rax
  00000001400EF5BA: FF 15 30 6F D1 01  call        qword ptr [0000000141E064F0h]
  00000001400EF5C0: 90                 nop
  00000001400EF5C1: BA 4B 00 00 00     mov         edx,4Bh
  00000001400EF5C6: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400EF5CE: FF 15 24 6F D1 01  call        qword ptr [0000000141E064F8h]
  00000001400EF5D4: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF5DB: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF5E3: FF 15 E7 6E D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF5E9: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  00000001400EF5F1: 48 8B C8           mov         rcx,rax
  00000001400EF5F4: FF 15 06 6F D1 01  call        qword ptr [0000000141E06500h]
  00000001400EF5FA: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  00000001400EF602: 83 B8 A8 00 00 00  cmp         dword ptr [rax+00000000000000A8h],6
                    06
  00000001400EF609: 7C 27              jl          00000001400EF632
  00000001400EF60B: 8B 94 24 10 04 00  mov         edx,dword ptr [rsp+0000000000000410h]
                    00
  00000001400EF612: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF61A: FF 15 B0 6E D1 01  call        qword ptr [0000000141E064D0h]
  00000001400EF620: 48 8B D0           mov         rdx,rax
  00000001400EF623: 48 8B 8C 24 08 04  mov         rcx,qword ptr [rsp+0000000000000408h]
                    00 00
  00000001400EF62B: FF 15 D7 6E D1 01  call        qword ptr [0000000141E06508h]
  00000001400EF631: 90                 nop
  00000001400EF632: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400EF63A: FF 15 E8 C8 D1 01  call        qword ptr [0000000141E0BF28h]
  00000001400EF640: 90                 nop
  00000001400EF641: 48 8B 8C 24 20 04  mov         rcx,qword ptr [rsp+0000000000000420h]
                    00 00
  00000001400EF649: FF 15 91 63 D1 01  call        qword ptr [0000000141E059E0h]
  00000001400EF64F: 0F 28 B4 24 E0 03  movaps      xmm6,xmmword ptr [rsp+00000000000003E0h]
                    00 00
  00000001400EF657: 0F 28 BC 24 D0 03  movaps      xmm7,xmmword ptr [rsp+00000000000003D0h]
                    00 00
  00000001400EF65F: 48 81 C4 F0 03 00  add         rsp,3F0h
                    00
  00000001400EF666: 5B                 pop         rbx
  00000001400EF667: C3                 ret

  Summary

     1E02000 .text
